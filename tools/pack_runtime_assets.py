"""Move immutable animation programs from AVR flash to the resident SPI bank.

The flash action pool and host oracle arrays remain unchanged. Programs are
decoded using the AVR's byte-aligned state layout, then appended after poses.
"""
from pathlib import Path
import re,json
R=Path(__file__).resolve().parents[1]
header=(R/'Barbarian.h').read_text()
state=re.search(r'typedef struct\{\s*/\* Keep hot fields.*?\} BarbarianState;',header,re.S)[0]
offsets={};sizes={};size=0
for kind,name in re.findall(r'\b(s8|s16|u16|s32)\s+(\w+)\s*;',state):
 offsets[name]=size;sizes[name]={'s8':1,'s16':2,'u16':2,'s32':4}[kind];size+=sizes[name]
constants={name:245+i for i,name in enumerate(('BT_BODY_X','BT_BODY_Y','BT_CLASH','BT_GRUNT',
 'BT_REF_B','BT_REF_A','BT_TIME','BT_ZERO','BT_SOUND','BT_GOTO'))}
constants.update({name:int(value,0) for name,value in re.findall(r'#define\s+(BARB_SFX_\w+)\s+(0x[\da-fA-F]+|\d+)',header)})
text=(R/'data/animation-timelines.inc').read_text()
blob=bytearray();macros=[];programs=[]
b_name='barbTimeline'+str(min(map(int,re.findall(r'barbTimeline(\d+)',(R/'barbarian_logic_post_b.inc').read_text()))))
seen={};saved=0
for name,body in re.findall(r'static const u8 (barbTimeline\d+)\[\] PROGMEM = \{(.*?)\};',text,re.S):
 body=re.sub(r'offsetof\(BarbarianState,\s*(\w+)\)',lambda m:str(offsets[m[1]]),body)
 tokens=[x.strip() for x in body.split(',') if x.strip()]
 values=[constants[x] if x in constants else int(x) for x in tokens]
 data=bytes(v&255 for v in values)
 # Keep the A/B ranges separate: the runtime selects an actor cache by offset.
 if name==b_name:seen={}
 if data in seen:off=seen[data];saved+=len(data)
 else:off=len(blob);seen[data]=off;blob.extend(data)
 bucket=(len(data)+7)//8
 assert off<4096 and 1<=bucket<=16
 macros.append(f'#define {name} ((const u8 *)(uintptr_t){off|((bucket-1)<<12)}u)')
 programs.append(dict(name=name,offset=off,bytes=len(data)))
print('Identical timeline storage saved:',saved,'bytes')
maximum=(max(p['bytes'] for p in programs)+7)&~7
b_name='barbTimeline'+str(min(map(int,re.findall(r'barbTimeline(\d+)',(R/'barbarian_logic_post_b.inc').read_text()))))
b_start=next(p['offset'] for p in programs if p['name']==b_name)
assert maximum<256 and len(programs)==106
cache_a=max(p['bytes'] for p in programs if p['offset']<b_start)
cache_b=max(p['bytes'] for p in programs if p['offset']>=b_start)
# Only rounded bytes beyond the actual program bank need padding. Check every
# deduplicated offset and each actor's clamped read size; a whole max cache was
# needlessly pushing a complete pose across the fixed 64KiB bank boundary.
read_end=max(p['offset']+min((p['bytes']+7)&~7,cache_a if p['offset']<b_start else cache_b) for p in programs)
padding=max(0,read_end-len(blob))
blob.extend(bytes(padding))
assert all(p['offset']+min((p['bytes']+7)&~7,cache_a if p['offset']<b_start else cache_b)<=len(blob) for p in programs)
timeline_bytes=len(blob)
ending=(R/'data/ending-text.raw').read_bytes();assert len(ending)==704
blob.extend(ending)
actions_offset=len(blob)
pool=re.findall(r'static const u8 barbTimelineActions\[\]\[3\] PROGMEM = \{(.*?)\};',text,re.S)[0]
pool=re.sub(r'offsetof\(BarbarianState,\s*(\w+)\)',lambda m:str(offsets[m[1]]),pool)
for row in re.findall(r'\{([^{}]+)\}',pool):
 for token in row.split(','):
  token=token.strip();blob.append((constants[token] if token in constants else int(token))&255)
assert blob[actions_offset:actions_offset+6]==bytes([constants['BT_GOTO'],constants['BT_ZERO'],2,
    constants['BT_GOTO'],constants['BT_ZERO'],1]),'Update the SPI control-flow fast path'
blood_offset=len(blob)
blood=(R/'data/blood.raw').read_bytes()
blob.extend(blood)
palettes_offset=len(blob)
palette_text=(R/'data/bg-defs.inc').read_text().split('bg_palettes',1)[1]
bgpal=bytes(int(v,16) for v in re.findall(r'0x([0-9A-Fa-f]{2})',palette_text))
assert len(bgpal)==112
blob.extend(bgpal)
reset_offset=len(blob)
reset=(R/'data/source-round-reset.inc').read_text()
copies=[]
for field,expr in re.findall(r'g->(\w+)\s*=\s*([^;]+);',reset):
 if 'g->' in expr:
  assert re.fullmatch(r'g->x[AB](?: \+ 4)?',expr),expr
  copies.append(f' g->{field}={expr};')
 else:
  value=int(expr)
  for byte in range(sizes[field]):blob.extend((offsets[field]+byte,(value>>(byte*8))&255))
reset_bytes=len(blob)-reset_offset
assert len(copies)==8 and reset_bytes<256
initial_offset=len(blob)
values={name:0 for name in offsets}
values.update(json.loads((R/'data/startup-defaults.json').read_text()))
initial=bytearray(size)
for field,value in values.items():initial[offsets[field]:offsets[field]+sizes[field]]=value.to_bytes(sizes[field],'little')
# The score/fraction tail is zero: initialize it directly rather than pushing
# a whole pose across the fixed SPI-bank split for a few template bytes.
initial=initial.rstrip(b'\0')
zero_tail=size-len(initial)
blob.extend(initial)
foreground_offset=len(blob)
foreground=(R/'data/foreground.raw').read_bytes();assert len(foreground)==400
blob.extend(foreground)
def write(name,data):
 p=R/'data'/name
 if isinstance(data,str):data=data.encode()
 if not p.exists() or p.read_bytes()!=data:p.write_bytes(data)
write('spi-timelines.inc','/* Generated SPI offsets; host arrays remain the oracle. */\n'+
 f'#define BARB_TIMELINE_CACHE_BYTES {maximum}u\n#define BARB_TIMELINE_B_START {b_start}u\n'+
 f'#define BARB_TIMELINE_CACHE_A {cache_a}u\n#define BARB_TIMELINE_CACHE_B {cache_b}u\n'+
 f'#define BARB_ENDING_BITMAP_OFFSET {timeline_bytes}u\n'+
 f'#define BARB_ACTIONS_OFFSET {actions_offset}u\n'+
 f'#define BARB_BLOOD_OFFSET {blood_offset}u\n'+ '\n'.join(macros)+'\n')
write('spi-timelines.inc',(R/'data/spi-timelines.inc').read_text()+f'#define BARB_BG_PALETTES_OFFSET {palettes_offset}u\n')
write('spi-timelines.inc',(R/'data/spi-timelines.inc').read_text()+
 f'#define BARB_RESET_OFFSET {reset_offset}u\n#define BARB_RESET_BYTES {reset_bytes}u\n#define BARB_INITIAL_OFFSET {initial_offset}u\n')
write('spi-timelines.inc',(R/'data/spi-timelines.inc').read_text()+f'#define BARB_FOREGROUND_OFFSET {foreground_offset}u\n')
write('runtime-init.inc','/* Source-derived constant stores; retained fields and old x cells survive. */\n'+
 'static void BarbarianResetRound(BarbarianState *g){\n'+
 '#if BARB_BLADE_BLOOD\n bladeLevel[0]=bladeLevel[1]=0;\n#endif\n'+ '\n'.join(copies)+
 '\n u16 address=BARB_RUNTIME_BASE+BARB_RESET_OFFSET;u8 left=BARB_RESET_BYTES;\n'+
 ' while(left){u8 n=left>80?80:left;BarbarianSpiReadBytes(address,barbLineBuf,n);\n'+
 '  for(u8 i=0;i<n;i+=2)((u8*)g)[barbLineBuf[i]]=barbLineBuf[i+1];address+=n;left-=n;}\n}\n'+
 'static void BarbarianLoadDefaults(BarbarianState *g){\n'+
 '#if BARB_BLADE_BLOOD\n bladeLevel[0]=bladeLevel[1]=0;\n#endif\n'+
 ' u16 a=BARB_RUNTIME_BASE+BARB_INITIAL_OFFSET;\n'+
 f' BarbarianSpiReadBytes(a,(u8*)g,{len(initial)});\n'+
 (f' memset((u8*)g+{len(initial)},0,{zero_tail});\n' if zero_tail else '')+'}\n')
span=(R/'data/sprite-spanfmt.inc').read_text()
poses=int(re.search(r'BARB_SPR_DATA_SIZE\s+(\d+)',span)[1])
p=R/'data/BARB-SPR.DAT';raw=p.read_bytes();assert len(raw)>=poses
# Runtime is below the fixed sound bank; poses are split across SPI banks by
# prepare_layout.py. The old 160x128 single-bank pose limit no longer applies.
assert 3*(214*128//2)+len(blob)<65536,'Runtime assets overlap the sound bank'
write('BARB-SPR.DAT',raw[:poses]+blob+(R/'data/menu.raw').read_bytes())
write('spi-timelines.json',json.dumps(dict(deduplicated_bytes=saved,read_padding_bytes=padding,pose_bytes=poses,bytes=timeline_bytes,cache_bytes=maximum,cache_a=cache_a,cache_b=cache_b,
 ending_offset=timeline_bytes,ending_bytes=len(ending),actions_offset=actions_offset,actions_bytes=blood_offset-actions_offset,
 blood_offset=blood_offset,blood_bytes=len(blood),total_bytes=len(blob),
 bg_palettes_offset=palettes_offset,bg_palettes_bytes=len(bgpal),
 reset_offset=reset_offset,reset_bytes=reset_bytes,initial_offset=initial_offset,initial_bytes=len(initial),
 foreground_offset=foreground_offset,foreground_bytes=len(foreground),
 programs=programs),indent=2)+'\n')
print(f'SPI animation programs: {len(programs)}, {len(blob)} bytes, {maximum}-byte cache')
