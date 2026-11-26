#!/usr/bin/env python3
"""Resample all original ADPCM2 effects to continuous 262-line audio.

BARB-SND.DAT and sfx-dir.inc remain immutable input references. The deployed
bank uses a lossless prefix code over the resampled ADPCM2 symbols.
"""
from pathlib import Path
import hashlib,json,re
import numpy as np
from adpcm_codec import decode_precedent,encode_precedent
R=Path(__file__).resolve().parents[1]

def pack(raw):
    out=bytearray();byte=used=0
    for v in raw:
        for shift in (0,2,4,6):
            for bit in ((1,1,0),(1,0),(0,),(1,1,1))[(v>>shift)&3]:
                byte|=bit<<used;used+=1
                if used==8:out.append(byte);byte=used=0
    if used:out.append(byte)
    return bytes(out)

def assets():
    source=(R/'data/BARB-SND.DAT').read_bytes();bank=bytearray();rows=[];decoded=[]
    entries=re.findall(r'(?m)^\s*(\d+),(\d+),\s*//.*?/([^/]+)\.raw',(R/'data/sfx-dir.inc').read_text())
    assert len(entries)==24
    for end,length,name in entries:
        end,length=int(end),int(length)
        pcm=np.frombuffer(decode_precedent(source[end-length:end],2,length*4),dtype=np.uint8)
        count=(len(pcm)*262+252)//253
        pcm=np.clip(np.rint(np.interp(np.arange(count)*253/262,np.arange(len(pcm)),pcm)),0,255).astype(np.uint8).tobytes()
        raw=encode_precedent(pcm,2);stored=pack(raw)
        rows.append(dict(name=name,start=len(bank),stored=len(stored),decoded=len(raw),samples=len(raw)*4))
        bank+=stored;decoded.append(raw)
    return bytes(bank),rows,decoded

if __name__=='__main__':
    bank,rows,_=assets()
    (R/'data/BARB-SFX.DAT').write_bytes(bank)
    (R/'data/sfx-huff-dir.inc').write_text('static const uint16_t sfx_dir[] __attribute__((section(".text.barbSfxDir")))={\n'+''.join(f'{r["start"]},{r["decoded"]}, // {r["name"]}\n' for r in rows)+'};\n')
    (R/'data/sfx-vsync.json').write_text(json.dumps(dict(rate=15720,codec='ADPCM2 with prefix codes 2=0,1=10,0=110,3=111; LSB first',bytes=len(bank),clips=rows),indent=2)+'\n')
    print(len(bank),hashlib.sha256(bank).hexdigest())
