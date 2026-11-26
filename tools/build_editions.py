"""Build BARBARIN.UZE and BARBCENS.UZE in default/, retaining edition debug files."""
from pathlib import Path
import argparse, hashlib, json, os, shutil, subprocess, sys
R=Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser()
p.add_argument('--output',type=Path,default=R/'default/editions')
a=p.parse_args();output=a.output.resolve();output.mkdir(parents=True,exist_ok=True)
results={}
for name,public in [('public',1),('original',0)]:
    env=dict(os.environ,BARB_PUBLIC=str(public))
    subprocess.run([sys.executable,str(R/'tools/build_sfa.py')],env=env,check=True)
    dest=output/name;dest.mkdir(exist_ok=True)
    rom_name='BARBCENS.UZE' if public else 'BARBARIN.UZE'
    for f in (rom_name,'Barbarian.elf','Barbarian.sym','Barbarian.lss','Barbarian.map'):
        shutil.copy2(R/'default'/f,dest/f)
    for f in ('build.json','network_compat.h','compositor_layout.inc'):
        shutil.copy2(R/'default/sfa'/f,dest/f)
    if public and (dest/'BARBARIN.UZE').exists():
        (dest/'BARBARIN.UZE').unlink() # Obsolete generated name cannot boot this edition.
    rom=(dest/rom_name).read_bytes()
    results[name]=dict(filename=rom_name,sha256=hashlib.sha256(rom).hexdigest(),bytes=len(rom),
        payload_sha256=hashlib.sha256(rom[512+61440:]).hexdigest())
assert results['public']['payload_sha256']==results['original']['payload_sha256']
assert (output/'public/network_compat.h').read_bytes()!=(output/'original/network_compat.h').read_bytes()
(output/'editions.json').write_text(json.dumps(results,indent=2)+'\n')
print('Built both editions:',output)
print('Deploy default/BARBARIN.UZE (original) and/or default/BARBCENS.UZE (censored), retaining filenames.')
