#!/usr/bin/env python3
from pathlib import Path
from PIL import Image
import sys,json

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0,str(ROOT/'tools'))
from blade_masks import encode_blade
OUT_DAT = ROOT / 'data' / 'BARB-SPR.DAT'
OUT_INC = ROOT / 'data' / 'sprite-spanfmt.inc'
OUT_LEGACY_INC = ROOT / 'data' / 'debug-sprite-spanfmt.inc'
OUT_FLASH_INC = ROOT / 'data' / 'debug-sprite-spanfmt-data.inc'
TRANS = (255, 0, 218, 255)

# Keep this order synchronized with resources/make-makefile.sh and data/sprite-ids.inc.
FRAME_NAMES = [
    'blood',
    'crouch1', 'crouch2',
    'death1', 'death2', 'death3',
    'decap1', 'decap2', 'decap3', 'decap4',
    'drax1', 'drax2',
    'fall1', 'fall2', 'fall3',
    'front1', 'front2', 'front3',
    'gnome1', 'gnome2', 'gnome3', 'gnome4',
    'guard-hi', 'guard-mid',
    'head1', 'head2', 'head3', 'head4', 'head5', 'head6',
    'headbutt1', 'headbutt2',
    'idle1', 'idle2', 'idle3',
    'jump1', 'jump2',
    'kick1', 'kick2',
    'knee1', 'knee3',
    'life1', 'life2',
    'marianna1', 'marianna2',
    'middle1', 'middle2', 'middle3',
    'neck2', 'neck3',
    'roll1', 'roll2', 'roll3', 'roll5',
    'shadow-hd', 'shadow-j1', 'shadow-j2',
    'spin1', 'spin2', 'spin3',
    'stand',
    'touche1', 'touche2',
    'victory1', 'victory2', 'victory3',
    'walk1', 'walk2', 'walk3',
    'web_od1', 'web_od2', 'web_od3', 'web_od4',
    'fire1','fire2','fire3',
]

# Display palette index targets used by the active 4bpp frame buffer.  The fight
# BG palettes keep the common fighter colors in stable low slots; rare colors are
# folded into their nearest useful slot when a sprite has more than 3 opaque colors.
TARGET_INDEX = {
    (0, 0, 0, 255): 0,
    (255, 182, 145, 255): 1,
    (255, 218, 145, 255): 1,
    (72, 72, 72, 255): 2,
    (109, 109, 145, 255): 2,
    (255, 0, 0, 255): 3,
    (255, 36, 0, 255): 3,
    (218, 145, 72, 255): 4,
    (34, 34, 153, 255): 5,
    (255, 255, 255, 255): 7,
    (36, 0, 72, 255): 6,
    (109, 0, 218, 255): 8,
    (145, 72, 218, 255): 8,
    (0, 145, 0, 255): 10,
    (0, 255, 0, 255): 12,
    (72, 36, 72, 255): 15,
}

MERGE_FALLBACK = {
    4: 1,   # tan -> fighter skin when no dedicated Drax slot is available
    6: 0,   # dark purple outline -> black
    8: 2,   # bright purple -> grey highlight
    10: 2,  # dark green -> grey if a 3-color sprite is full
    12: 1,  # bright green -> body color if a 3-color sprite is full
    15: 0,  # near-black purple -> black
}

DEBUG_FLASH_NAMES = ['walk1', 'walk2', 'walk3']


def cident(name: str) -> str:
    return name.replace('-', '_').upper()


def pack_codes(codes):
    out = bytearray()
    for i in range(0, len(codes), 4):
        b = 0
        for j in range(4):
            c = codes[i + j] if i + j < len(codes) else 0
            b |= (c & 3) << (6 - j * 2)
        out.append(b)
    return out


def choose_palette(targets):
    counts = {}
    for t in targets:
        counts[t] = counts.get(t, 0) + 1
    ordered = sorted(counts, key=lambda v: (-counts[v], v))
    keep = ordered[:3]
    while len(keep) < 3:
        keep.append(0)
    return keep[:3]


def remap_target(t, keep):
    if t in keep:
        return t
    t = MERGE_FALLBACK.get(t, t)
    if t in keep:
        return t
    # Last resort: fold to the most common kept color.  This only affects a few
    # one/two-pixel source artifacts in imported PNGs.
    return keep[0]


def encode_frame(img, name):
    w, h = img.size
    assert w<=60 and h<=80, f'{name}: compact history bounds exceed their row/nibble limits'
    px = list(img.getdata())
    targets = []
    for p in px:
        if p == TRANS:
            continue
        if p not in TARGET_INDEX:
            raise SystemExit(f'{name}: unmapped color {p}')
        targets.append(TARGET_INDEX[p])
    keep = choose_palette(targets)
    code_for_target = {t: i + 1 for i, t in enumerate(keep)}

    frame = bytearray()
    for y in range(h):
        row = px[y * w:(y + 1) * w]
        xs = [x for x, p in enumerate(row) if p != TRANS]
        if not xs:
            frame += bytes([255, 0])
            continue
        x0 = min(xs)
        x1 = max(xs) + 1
        codes = []
        for x in range(x0, x1):
            p = row[x]
            if p == TRANS:
                codes.append(0)
                continue
            target = remap_target(TARGET_INDEX[p], keep)
            codes.append(code_for_target[target])
        frame += bytes([x0, x1])
        frame += pack_codes(codes)
    return frame, keep


def write_if_changed(path, data):
    if isinstance(data, str):
        data = data.encode()
    if path.exists() and path.read_bytes() == data:
        return
    path.write_bytes(data)


def main():
    dat = bytearray()
    infos = []
    ybounds = []
    max_bytes = 0
    debug_chunks = {}
    blades={};steel={}

    for name in FRAME_NAMES:
        img = Image.open(ROOT / 'resources' / 'gfx' / f'{name}.png').convert('RGBA')
        w, h = img.size
        frame, palette = encode_frame(img, name)
        mask,edge,metal=encode_blade(img,name);blades[name]=edge;steel[name]=metal
        # Bounds include every source row and the steel overlay. They only
        # reduce old-picture repairs; gameplay canvases and coordinates stay intact.
        at=0;used=[]
        for y in range(h):
            l,r=frame[at:at+2];at+=2
            if l!=255:used.append(y);at+=(r-l+3)//4
        used.extend(y for x,y in metal)
        ybounds.append((min(used),max(used)+1) if used else (0,0))
        frame+=mask
        # The AVR cache loader derives addresses by summing earlier sizes.
        # Keep every logical frame in order, including identical poses.
        off=len(dat)
        dat+=frame
        size = len(frame)
        max_bytes = max(max_bytes, size)
        infos.append((name, w, h, off, size, palette))
        if name in DEBUG_FLASH_NAMES:
            debug_chunks[name] = (off, size, frame)

    lines = [
        '/* Auto-generated runtime 2bpp row-span sprite directory. */',
        '/* Row format: x0, x1, packed 2bpp codes. x0=255 means empty row. */',
        '',
        f'#define BARB_SPR_COUNT\t{len(infos)}u',
        f'#define BARB_SPR_DATA_SIZE\t{len(dat)}u',
        f'#define BARB_SPRITE_CACHE_BYTES\t{max_bytes}u',
        '',
    ]

    for name, w, h, off, size, palette in infos:
        macro = cident(name)
        lines += [
            f'#define BARB_SPR_{macro}_OFFSET\t{off}u',
            f'#define BARB_SPR_{macro}_BYTES\t{size}u',
        ]
        if name in DEBUG_FLASH_NAMES:
            dm = macro.replace('WALK', 'WALK')
            lines += [
                f'#define BARB_DEBUG_{dm}_W\t{w}u',
                f'#define BARB_DEBUG_{dm}_H\t{h}u',
                f'#define BARB_DEBUG_{dm}_OFFSET\t{off}u',
                f'#define BARB_DEBUG_{dm}_BYTES\t{size}u',
            ]
    lines.append('')

    lines.append('#ifndef __AVR__ /* Independent host pixel oracle. */')
    lines.append('static const u16 barbSpriteOffsets[BARB_SPR_COUNT] PROGMEM = {')
    for i in range(0, len(infos), 8):
        lines.append('\t' + ', '.join(f'{off}u' for _, _, _, off, _, _ in infos[i:i + 8]) + ',')
    lines.append('};')
    lines.append('')

    lines.append('#endif')
    palettes=list(dict.fromkeys(tuple(info[5]) for info in infos))
    assert max_bytes<512 and len(palettes)<=128
    extra=max(size for name,_,_,_,size,_ in infos if name.startswith('gnome') or name in ('head1','head2','head3','head4','head5','head6'))
    lines.append(f'#define BARB_EXTRA_SPRITE_CACHE_BYTES {extra}u')
    small=max(size for name,_,_,_,size,_ in infos if name.startswith(('head','fire','ending-')) and not name.startswith('headbutt'))
    lines.append(f'#define BARB_SMALL_SPRITE_CACHE_BYTES {small}u')
    lines.append('/* Low 9 bits: bytes; high 7 bits: shared palette ID. */')
    lines.append('static const u16 barbSpriteBytes[BARB_SPR_COUNT] PROGMEM = {')
    for i in range(0, len(infos), 8):
        lines.append('\t' + ', '.join(f'{size | (palettes.index(tuple(palette))<<9)}u' for _, _, _, _, size, palette in infos[i:i + 8]) + ',')
    lines.append('};')
    lines.append('')

    lines.append('/* Conservative source/steel vertical bounds, bottom exclusive. */')
    # Keep bounds after normal text: putting them before the mixer's aligned
    # program-memory tables would consume an extra 512-byte alignment page.
    lines.extend(['#ifdef __AVR__','#define BARB_BOUND_STORAGE __attribute__((section(".text.barbSpriteYBounds")))','#else','#define BARB_BOUND_STORAGE PROGMEM','#endif'])
    lines.append('static const u8 barbSpriteYBounds[BARB_SPR_COUNT][2] BARB_BOUND_STORAGE = {')
    lines.extend(f' {{ {a}u, {b}u }},' for a,b in ybounds)
    lines.append('};')
    lines.append('static const u8 barbSpritePalettes[][3] PROGMEM = {')
    for palette in palettes:lines.append(f' {{ {palette[0]}u, {palette[1]}u, {palette[2]}u }},')
    lines.append('};\n#ifndef __AVR__')
    lines.append('static const u8 barbSpriteOraclePalettes[BARB_SPR_COUNT][3] = {')
    for name,_,_,_,_,palette in infos:lines.append(f' {{ {palette[0]}u, {palette[1]}u, {palette[2]}u }}, /* {name} */')
    lines.append('};\n#endif')
    lines.append('')

    write_if_changed(OUT_DAT, bytes(dat))
    write_if_changed(ROOT/'data/blade-masks.json',json.dumps(blades,indent=2)+'\n')
    write_if_changed(ROOT/'data/steel-masks.json',json.dumps(steel,indent=2)+'\n')
    write_if_changed(OUT_INC, '\n'.join(lines))
    # Keep the old filename as a compatibility shim for local branches.
    write_if_changed(OUT_LEGACY_INC, '#include "sprite-spanfmt.inc"\n')

    flash = ['/* Auto-generated optional full flash copy for no-SPI sprite debug builds. */', 'const u8 barbDebugSpriteFlashData[] PROGMEM = {']
    for i in range(0, len(dat), 16):
        flash.append('\t' + ', '.join(f'0x{x:02X}' for x in dat[i:i + 16]) + ',')
    flash.append('};')
    flash.append('')
    write_if_changed(OUT_FLASH_INC, '\n'.join(flash))
    print(f'Wrote {OUT_DAT} {len(dat)} bytes, max frame {max_bytes} bytes')


if __name__ == '__main__':
    main()

if __name__ == '__main__':
    rows=['/* Sprite IDs and original canvas dimensions. */','#define SPRITE_DATA_SIZE 0']
    rows += [f'#define SPR_{n.upper().replace("-","_")} {i}' for i,n in enumerate(FRAME_NAMES)]
    for field,axis in [('widths',0),('heights',1)]:
        rows.append(f'const unsigned char sprite_{field}[] PROGMEM = {{')
        rows += [f' {Image.open(ROOT/"resources"/"gfx"/(n+".png")).size[axis]}, /* {n} */' for n in FRAME_NAMES]
        rows.append('};')
    (ROOT/'data/sprite-ids.inc').write_text('\n'.join(rows)+'\n')
