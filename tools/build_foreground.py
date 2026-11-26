"""Trace original transparent side layers at the panorama's exact sample grid.

Opaque rows are contiguous from each outer edge. Store the two interior edges
as nibbles, one byte per row, preserving the original source y=104..183.
"""
from pathlib import Path
from PIL import Image
import hashlib,json
R=Path(__file__).resolve().parents[1]
blob=bytearray();scenes=[]
for name,source in [('plaine','plaine'),('forest','foret'),('throne','trone'),('arena','arene'),('boss','arene')]:
    sources=[]
    mask_path=R/f'resources/gfx/foreground-{name}.png'
    mask=Image.open(mask_path).convert('RGBA');assert mask.size==(214,128)
    rows=[]
    for y in range(128):
        opaque=[x for x in range(214) if mask.getpixel((x,y))[3]]
        if not 32<=y<112:
            assert not opaque,'Foreground must remain in the original 80 rows'
            continue
        left=[x for x in opaque if x<107];right=[x for x in opaque if x>=107]
        lo,hi=max(left)+1,min(right)
        assert left==list(range(lo)) and right==list(range(hi,214))
        assert 24<=lo<40 and 176<=hi<192,'Contour exceeds packed nibble range'
        blob.append(((lo-24)<<4)|(hi-176));rows.append([lo,hi])
    scenes.append(dict(name=name,sources=sources,rows=rows,mask=str(mask_path.relative_to(R)),
        mask_sha256=hashlib.sha256(mask_path.read_bytes()).hexdigest()))
assert len(blob)==400
(R/'data/foreground.raw').write_bytes(blob)
(R/'data/foreground.json').write_text(json.dumps(dict(top=32,height=80,bytes=len(blob),scenes=scenes),indent=2)+'\n')
print('Foreground: five original contours, 400 resident SPI bytes')
