"""Native font bitmaps: white ink, black outline, transparent surroundings."""
from pathlib import Path
from PIL import Image
import sys
R=Path(__file__).resolve().parents[1]
if '--generate' not in sys.argv:
    # Normal builds treat the production PNGs as persistent editable inputs.
    raw=bytearray()
    for name,width in (('win',80),('lose',96)):
        halves=[Image.open(R/f'resources/gfx/ending-{name}{i+1}.png').convert('RGBA') for i in range(2)]
        assert all(im.size==(width//2,8) for im in halves)
        for y in range(8):
            row=[]
            for x in range(width):
                color=halves[x//(width//2)].getpixel((x%(width//2),y))
                row.append(8 if color[3]<128 or color[:3]==(255,0,218) else 7 if color[:3]==(255,255,255) else 0)
            raw.extend((row[x]<<4)|row[x+1] for x in range(0,width,2))
    assert len(raw)==704
    (R/'data/ending-text.raw').write_bytes(raw)
    raise SystemExit(0)
