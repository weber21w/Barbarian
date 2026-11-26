"""Keep raw sprite oracles; build a separate compressed resident/dependency bank."""
from pathlib import Path
import json,re,struct
from pose_codec import encode,decode
R=Path(__file__).resolve().parents[1]
h=(R/'data/sprite-spanfmt.inc').read_text();source=(R/'data/BARB-SPR.DAT').read_bytes()
offsets=[int(v) for v in re.findall(r'(\d+)u',h.split('barbSpriteOffsets')[1].split('};')[0])]
sizes=[int(v)&511 for v in re.findall(r'(\d+)u',h.split('barbSpriteBytes')[1].split('};')[0])]
assert len(offsets)==len(sizes) and len(offsets)<=128, "AVR directory index must fit one byte"
packed=bytearray();directory=bytearray();entries=[]
for f,(offset,n) in enumerate(zip(offsets,sizes)):
    raw=source[offset:offset+n];compressed=encode(raw)
    assert decode(compressed,n)==raw
    use=True;block=compressed
    directory.extend(struct.pack('<H',len(packed)))
    entries.append(dict(pose=f,offset=len(packed),stored=len(block),decoded=n,compressed=use))
    packed.extend(block)
meta=json.loads((R/'data/spi-timelines.json').read_text());rawbytes=meta['pose_bytes'];runtime=source[rawbytes:rawbytes+meta['total_bytes']];menu=(R/'data/menu.raw').read_bytes()
(R/'data/BARB-SPC.DAT').write_bytes(packed+runtime+directory+menu)
(R/'data/pose-resident.json').write_text(json.dumps(dict(raw_bytes=rawbytes,pose_bytes=len(packed),directory_offset=len(runtime),directory_bytes=len(directory),runtime_bytes=len(runtime)+len(directory),file_bytes=len(packed)+len(runtime)+len(directory)+len(menu),entries=entries),indent=2)+'\n')
print('Resident poses:',len(packed),'bytes; metadata:',len(directory),'bytes; saved:',rawbytes-len(packed)-len(directory))
