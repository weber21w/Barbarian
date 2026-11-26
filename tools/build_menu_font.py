"""Pack the frontend's small readable 3x5 uppercase font into 16-bit words."""
from pathlib import Path
import json
R=Path(__file__).resolve().parents[1]
patterns={
'A':'010101111101101','B':'110101110101110','C':'011100100100011',
'D':'110101101101110','E':'111100110100111','F':'111100110100100',
'G':'011100101101011','H':'101101111101101','I':'111010010010111',
'J':'001001001101010','K':'101101110101101','L':'100100100100111',
'M':'101111111101101','N':'101111111111101','O':'010101101101010',
'P':'110101110100100','Q':'010101101111011','R':'110101110101101',
'S':'011100010001110','T':'111010010010010','U':'101101101101111',
'V':'101101101101010','W':'101101111111101','X':'101101010101101',
'Y':'101101010010010','Z':'111001010100111',
'0':'111101101101111','1':'010110010010111','2':'110001010100111',
'3':'110001010001110','4':'101101111001001','5':'111100110001110',
'6':'011100111101111','7':'111001010010010','8':'111101111101111',
'9':'111101111001110','+':'000010111010000','/':'001001010100100',
':':'000010000010000','-':'000000111000000','>':'100010001010100',
' ':'000000000000000','!':'010010010000010'}
rows=[int(patterns.get(chr(c),patterns[' ']),2) for c in range(32,91)]
pool=bytearray(b''.join(v.to_bytes(2,'little') for v in rows))
defines=[]
for name,text in json.loads((R/'tools/menu_texts.json').read_text()).items():
 if name in ('SOLO','LOCAL_VERSUS','UZENET_ONLINE','CONTROLS'):assert len(pool)<256
 assert len(text)<32
 defines.append(f'#define BARB_MT_{name} {len(pool)}u')
 pool.extend(text.encode('ascii')+b'\0')
pool.extend(bytes(31))
assert len(pool)<=616,'Menu assets exceed the gap before rollback storage'
(R/'data/menu.raw').write_bytes(pool)
(R/'data/menu-font.inc').write_text('/* Generated resident frontend font/labels. */\n'+
 '\n'.join(defines)+f'\n#ifndef BARB_MENU_BYTES\n#define BARB_MENU_BYTES {len(pool)}u\n#endif\n'+
 '#ifndef __AVR__\nstatic const u8 menuData[]={'+','.join(map(str,pool))+'};\n#endif\n')
