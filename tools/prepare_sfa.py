"""Generate raster, sound IDs and published network compatibility identity."""
from pathlib import Path
import json,os,re,sys
ROOT=Path(__file__).resolve().parents[1]
OUT=ROOT/'default/sfa';OUT.mkdir(parents=True,exist_ok=True)
sys.path.insert(0,str(ROOT/'platform/sfa2'))
from scanout_kernel import generate
generate(OUT)
(OUT/'sfx_ids.h').write_text('\n'.join(re.findall(r'^#define BARB_SFX_.*$',(ROOT/'Barbarian.h').read_text(),re.M))+'\n')
edition='public' if os.environ.get('BARB_PUBLIC','0')=='1' else 'original'
compat=json.loads((ROOT/'data/network-release.json').read_text())[edition]
(OUT/'network_compat.h').write_text('#define BARB_NET_COMPAT_BYTES '+','.join(map(str,compat))+'\n')
