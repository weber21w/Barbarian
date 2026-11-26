#!/usr/bin/env python3
from PIL import Image
from pathlib import Path
import math

BG_ORDER = [
        ('bg-arena', 'BG_ARENA', 'arena-palette'),
        ('bg-forest', 'BG_FOREST', 'forest-palette'),
        ('bg-menu', 'BG_MENU', 'menu-palette'),
        ('bg-plaine', 'BG_PLAINE', 'plaine-palette'),
        ('bg-throne', 'BG_THRONE', 'throne-palette'),
        ('bg-title', 'BG_TITLE', 'title-palette'),
        ('bg-boss', 'BG_BOSS', 'boss-palette'),
]

ROOT = Path(__file__).resolve().parent.parent
GFX_DIR = ROOT / 'resources' / 'gfx'
PAL_DIR = ROOT / 'resources' / 'palette'
OUT_DAT = ROOT / 'data' / 'BARB-GFX.DAT'
OUT_DEFS = ROOT / 'data' / 'bg-defs.inc'


def uze_color(rgb):
        r, g, b = rgb
        return (((r & 0xE0) >> 5) | ((g & 0xE0) >> 2) | (b & 0xC0)) & 0xFF


def read_palette_rgb(name: str | None, img: Image.Image):
        if name is not None:
                pal_img = Image.open(PAL_DIR / f'{name}.png').convert('RGB')
                colors = list(pal_img.getdata())[:16]
                if len(colors) < 16:
                        colors += [(0, 0, 0)] * (16 - len(colors))
                return colors[:16]

        # Fallback for non-gameplay screens without a hand palette. Preserve the
        # first 16 unique colors in image order. These screens are not part of the
        # 192x120 fight path.
        rgb = img.convert('RGB')
        colors = []
        seen = set()
        for c in rgb.getdata():
                if c not in seen:
                        seen.add(c)
                        colors.append(c)
                        if len(colors) == 16:
                                break
        if len(colors) < 16:
                colors += [(0, 0, 0)] * (16 - len(colors))
        return colors[:16]


def nearest_index(rgb, palette, reserve_enemy=True):
        best = 0
        best_dist = 1 << 30
        r, g, b = rgb
        for i, (pr, pg, pb) in enumerate(palette):
                if reserve_enemy and i in (6,9):continue # Enemy-only slots in fighting stages.
                dr = r - pr
                dg = g - pg
                db = b - pb
                d = dr * dr + dg * dg + db * db
                if d < best_dist:
                        best = i
                        best_dist = d
        return best


def pack_4bpp_png(path: Path, palette_rgb) -> bytes:
        img = Image.open(path).convert('RGB')
        w, h = img.size
        if (w,h) not in ((160,128),(214,128)):
                raise SystemExit(f'{path}: backgrounds must be 160x128 or 214x128 pixels; got {w}x{h}')
        if (w & 1) != 0:
                raise SystemExit(f'{path}: width must be even for packed 4bpp, got {w}')
        px = list(img.getdata())
        reserve_enemy=path.stem not in ('bg-title','bg-menu')
        out = bytearray()
        for y in range(h):
                row = px[y * w:(y + 1) * w]
                for x in range(0, w, 2):
                        a = nearest_index(row[x], palette_rgb, reserve_enemy)
                        b = nearest_index(row[x + 1], palette_rgb, reserve_enemy)
                        out.append(((a & 0x0F) << 4) | (b & 0x0F))
        if path.stem == 'panorama-boss':
                # The custom scaled Drax extends beyond the old head/body cutouts.
                # Apply the actor-removal mask to packed data, preserving editable PNGs.
                for left, top, right, bottom in ((75,24,85,34),(70,32,92,50)):
                        for y in range(top,bottom):
                                for x in range(left,right):
                                        i=y*(w//2)+x//2
                                        out[i]&=0xF0 if x&1 else 0x0F
        return bytes(out)


def write_if_changed(path: Path, data: bytes):
        old = path.read_bytes() if path.exists() else None
        if old != data:
                path.write_bytes(data)


def main():
        dat = bytearray()
        offsets = []
        sizes = []
        palettes_rgb = []
        for name, _macro, pal_name in BG_ORDER:
                path = GFX_DIR / f'{name}.png'
                panorama = GFX_DIR / f"panorama-{name[3:]}.png"
                if name not in ('bg-title','bg-menu'):path = panorama
                img = Image.open(path)
                pal_rgb = read_palette_rgb(pal_name, img)
                blob = pack_4bpp_png(path, pal_rgb)
                if img.width==160:
                        # UI remains a fixed 160-pixel viewport at the centred camera.
                        blob=b''.join(bytes(13)+blob[y*80:(y+1)*80]+bytes(14) for y in range(128))
                offsets.append(len(dat))
                sizes.append(len(blob))
                palettes_rgb.append(pal_rgb)
                dat.extend(blob)

        assert all(size == 214*128//2 for size in sizes), 'Runtime backgrounds use a fixed frame stride'
        defs = []
        for idx, (_name, macro, _pal_name) in enumerate(BG_ORDER):
                defs.append(f'#define {macro}\t\t{idx}u')
        defs.append(f'#define BG_COUNT\t\t{len(BG_ORDER)}u')
        defs.append(f'#define BG_DATA_SIZE\t{len(dat)}UL')
        defs.append('')
        for (_name, macro, _pal_name), size in zip(BG_ORDER, sizes):
                defs.append(f'#define {macro}_SIZE\t{size}UL')
        defs.append('')
        defs.append('const u32 bg_offsets[] PROGMEM = {')
        for off in offsets:
                defs.append(f'\t{off}UL,')
        defs.append('};')
        defs.append('')
        defs.append('const unsigned char bg_palettes[BG_COUNT][16] PROGMEM = {')
        for (_name, _macro, _pal_name), pal_rgb in zip(BG_ORDER, palettes_rgb):
                vals = ', '.join(f'0x{uze_color(c):02X}' for c in pal_rgb)
                defs.append(f'\t{{ {vals} }},')
        defs.append('};')
        defs_bytes = ('\n'.join(defs) + '\n').encode('utf-8')

        write_if_changed(OUT_DAT, bytes(dat))
        write_if_changed(OUT_DEFS, defs_bytes)


if __name__ == '__main__':
        main()
