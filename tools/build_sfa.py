"""Build Barbarian with SFA2's SPI raster and three-lane ADPCM mixer."""
from pathlib import Path
import subprocess,sys,os,re,json,hashlib
ROOT=Path(__file__).resolve().parents[1]
OUT=ROOT/'default/sfa';OUT.mkdir(parents=True,exist_ok=True)
(ROOT/'data').mkdir(exist_ok=True)
KERNEL=ROOT/'platform/kernel';VIDEO=ROOT/'platform/sfa2'
FW=ROOT/'vendor/uzenet'
net=True
sid=True
public=os.environ.get('BARB_PUBLIC','0')=='1'
rom_name='BARBCENS.UZE' if public else 'BARBARIN.UZE'
subprocess.run([sys.executable,str(ROOT/'tools/build_sfx_vsync.py')],check=True)
# The SFA effect bank occupies one 64 KiB SPI bank; its runtime directory is
# deliberately 16-bit. Reject future oversized entries instead of truncating.
sfx_entries=[tuple(map(int,m)) for m in re.findall(r'(?m)^\s*(\d+),(\d+),', (ROOT/'data/sfx-huff-dir.inc').read_text())]
assert len(sfx_entries)==24 and all(0<length<=65535 and start<65535 for start,length in sfx_entries)
assert (ROOT/'data/BARB-SFX.DAT').stat().st_size==int(re.search(r'BARB_AUDIO_DATA_BYTES\s+(\d+)',(ROOT/'platform/audio.h').read_text())[1])
subprocess.run([sys.executable,str(ROOT/'tools/build_menu_font.py')],check=True)
subprocess.run([sys.executable,str(ROOT/'tools/build_blood.py')],check=True)
subprocess.run([sys.executable,str(ROOT/'tools/build_enemy_colors.py')],check=True)
subprocess.run([sys.executable,str(ROOT/'tools/build_ending_text.py')],check=True)
# Editable PNGs are persistent inputs. Source re-import is an explicit action.
subprocess.run([sys.executable,str(ROOT/'resources/make-bg-dat.py')],check=True)
subprocess.run([sys.executable,str(ROOT/'tools/build_foreground.py')],check=True)
subprocess.run([sys.executable,str(ROOT/'resources/make-debug-sprites.py')],check=True)
subprocess.run([sys.executable,str(ROOT/'tools/pack_runtime_assets.py')],check=True)
subprocess.run([sys.executable,str(ROOT/'tools/pack_pose_assets.py')],check=True)
subprocess.run([sys.executable,str(ROOT/'tools/pack_resident_score.py')],check=True)
subprocess.run([sys.executable,str(ROOT/'tools/prepare_layout.py')],check=True)
if sid:
    base=512+61440+sum((ROOT/'data'/n).stat().st_size for n in ('BARB-GFX.DAT','BARB-SPC.DAT','BARB-SFX.DAT'))
    (ROOT/'data/sid-music.h').write_text('#pragma once\n#include "platform-layout.h"\n#define BARB_SCORE_FILE_OFFSET '+str(base)+'UL\n#define BARB_SID_FILE_OFFSET '+str(base)+'UL\n')
subprocess.run([sys.executable,str(ROOT/'tools/prepare_sfa.py')],check=True)
# Assembly uses plain integer .equ values from the same generated C layout.
layout_symbols={}
for header in ('platform-layout.h','spi-timelines.inc','sprite-spanfmt.inc'):
    layout_symbols.update({n:int(v) for n,v in re.findall(r'^#define\s+(BARB_\w+)\s+(\d+)(?:[uUlL]*)\s*$',(ROOT/'data'/header).read_text(),re.M)})
for header in (ROOT/'data/sid-music.h',ROOT/'platform/audio.h'):
    layout_symbols.update({n:int(v) for n,v in re.findall(r'^#define\s+(BARB_\w+)\s+(\d+)(?:[uUlL]*)\s*$',header.read_text(),re.M)})
(OUT/'compositor_layout.inc').write_text(''.join(f'.equ {n},{layout_symbols[n]}\n' for n in ('BARB_FB_ROW_BYTES','BARB_SPIVRAM_BG_CLEAN','BARB_RUNTIME_BASE','BARB_FOREGROUND_OFFSET','BARB_SPRITE_CACHE_BYTES','BARB_SMALL_SPRITE_CACHE_BYTES','BARB_SPIVRAM_SPRITES','BARB_SPR_BANK0_BYTES','BARB_SPR_BANK1_BASE','BARB_SID_SPI_A','BARB_SID_RING_BYTES','BARB_SPR_COUNT','BARB_POSE_DIRECTORY_OFFSET')))
with (OUT/'compositor_layout.inc').open('a') as layout:
    layout.write(''.join(f'.equ {n},{layout_symbols[n]}\n' for n in ('BARB_SID_FILE_OFFSET','BARB_AUDIO_MUSIC_GAIN','BARB_MUSIC_AHEAD_BASE','BARB_MUSIC_AHEAD_BYTES')))
    # One prefix offset per eight poses bounds the assembly cache lookup to
    # at most seven size additions. Derive it from the current editable PNGs.
    pose_offsets=[int(v) for v in re.findall(r'^#define\s+BARB_SPR_\w+_OFFSET\s+(\d+)u\s*$',(ROOT/'data/sprite-spanfmt.inc').read_text(),re.M)]
    assert len(pose_offsets)==layout_symbols['BARB_SPR_COUNT']
    layout.write('.macro BARB_POSE_CHECKPOINTS\n.word '+','.join(map(str,pose_offsets[::8]))+'\n.endm\n')
    state=re.search(r'typedef struct\{\s*/\* Keep hot fields.*?\} BarbarianState;',(ROOT/'Barbarian.h').read_text(),re.S)[0]
    state_offsets={};at=0
    for kind,name in re.findall(r'\b(s8|s16|u16|s32)\s+(\w+)\s*;',state):
        state_offsets[name]=at;at+={'s8':1,'s16':2,'u16':2,'s32':4}[kind]
    assert state_offsets['joueurA']==state_offsets['joueurB']+1
    assert state_offsets['sangB']==state_offsets['sangA']+1
    assert state_offsets['vieB']==state_offsets['vieA']+1
    assert state_offsets['joueurA']<64 and state_offsets['sangA']<64
    layout.write(''.join(f'.equ BARB_STATE_{name.upper()},{state_offsets[name]}\n' for name in ('joueurA','sangA','vieA')))
flags=['-mmcu=atmega644','-Os','-g','-std=gnu99','-ffunction-sections','-fdata-sections',
 '-fno-inline-functions-called-once','-fno-toplevel-reorder','-fno-split-wide-types','-fno-tree-ter','-mcall-prologues','-fsigned-char','-Wall',
 '-DF_CPU=28636360UL','-DVIDEO_MODE=0','-DVIDEO_MODE_PATH='+str(VIDEO),
 '-DINTRO_LOGO=0','-DSOUND_MIXER=3','-DCUSTOM_SOUND_MIXER=1','-DNO_CHANNELS=1',
 '-DAUDIO_OUT_HSYNC_CYCLES=240','-DAUDIO_OUT_VSYNC_CYCLES=240','-DSCROLLING=0',
 '-DUART=0','-DENABLE_MIXER=0','-DPF_USE_LSEEK=1','-DBARB_SFA_VIDEO=1','-DBARB_BG_CROP_TOP=0',
 '-DBARB_M748_RENDER_LINES=120','-DBARB_M748_RENDER_START=72',
 '-DBARB_ENABLE_SPRITE_FLASH_DATA=0','-DBARB_USE_UZEBOX_SOUND=0',
 '-DBARB_PUBLIC='+str(int(public)),'-DBARB_SPI_TIMELINES=1','-DBARB_NET='+str(int(net)),'-DSFA2_NET='+str(int(net)),'-DBARB_SID='+str(int(sid)),
 '-I'+str(KERNEL),'-I'+str(ROOT),'-I'+str(ROOT/'platform'),'-I'+str(OUT),
 '-I'+str(FW/'client'),'-I'+str(FW/'include')]
def tool(*args):
    p=subprocess.run(list(map(str,args)),capture_output=True,text=True,cwd=ROOT/'default')
    if p.returncode:print(p.stdout+p.stderr);p.check_returncode()
    if p.stderr:print(p.stderr,end='')
    return p.stdout
kernel=(KERNEL/'uzeboxVideoEngineCore.s').read_text()
(OUT/'kernel.s').write_text(kernel)
sys.path.insert(0,str(VIDEO))
from stream_diskio import generate as generate_disk
generate_disk(KERNEL,OUT)
p=OUT/'pff.c';p.write_text(p.read_text().replace('SFA2    UZE',rom_name.replace('.','')))
# The compositor's small .progmem range helper fills the pre-mixer alignment
# tail; keep it before three.s so the mixer LUT planes stay at their old base.
sources=[OUT/'kernel.s',KERNEL/'uzeboxCore.c',KERNEL/'controllers.s',KERNEL/'uzeboxVideoEngine.c',KERNEL/'spiram.s',
 OUT/'pff.c',OUT/'diskio.c',ROOT/'platform/main.c',ROOT/'platform/audio.c',ROOT/'platform/compositor.s',VIDEO/'mixer/three.s',ROOT/'platform/blit_pairs.s',ROOT/'platform/graphics_bus.s',ROOT/'platform/audio_refill.s',ROOT/'platform/compose_frame.s',ROOT/'platform/presentation.s']
if net:
    # Active RX faults publish phase, leaving READY and PA7 to video.
    uart=(FW/'client/un_minimal_avr.S').read_text()
    fault="    std Z+PHASE,r24\n    std Z+READY,r1\n    clr r24\n.Lrelease:"
    assert uart.count(fault)==1, 'SDK fault handler changed; review UART ownership'
    uart=uart.replace(fault,"    std Z+PHASE,r24\n    clr r24\n"
        "    lds r25,_SFR_MEM_ADDR(UCSR0B)\n    sbrc r25,RXEN0\n    ret\n"
        "    std Z+READY,r1\n.Lrelease:")
    sdk=OUT/'barb_un_minimal_avr.S';sdk.write_text(uart)
    sources +=[FW/'client/un_uart_avr.c',sdk]
if sid:sources +=[ROOT/'platform/sid_pulse.s',ROOT/'platform/resident_score.c',ROOT/'platform/resident_score_io.c',ROOT/'platform/resident_envelope.c']
sources +=[ROOT/'platform/blade_hit.s']
# All objects are rebuilt below. Remove stale indices left by older source
# lists or networking/music options, retaining the linked ROM and evidence.
expected_objects={f'{i}-{source.stem}.o' for i,source in enumerate(sources)}
expected_assembly={f'{i}-fixed.s' for i,source in enumerate(sources) if source.suffix.lower()=='.s'}
for generated in (*OUT.glob('*.o'),*OUT.glob('*-fixed.s')):
    keep=generated.name in (expected_objects if generated.suffix=='.o' else expected_assembly)
    if not keep:
        assert generated.resolve().parent==OUT.resolve() and not generated.is_symlink(),generated
        generated.unlink()
objects=[]
for i,source in enumerate(sources):
    obj=OUT/f'{i}-{source.stem}.o';objects.append(obj)
    if source.suffix.lower()=='.s':
        assembly=tool('avr-gcc',*flags,'-x','assembler-with-cpp','-E',source)
        assembly=re.sub(r'(?m)^(\s*)(jmp|call)\s+([^;\n]+)',lambda m:m[1]+'.word '+('0x940c' if m[2]=='jmp' else '0x940e')+',pm('+m[3].strip()+')',assembly)
        safe=OUT/f'{i}-fixed.s';safe.write_text(assembly)
        tool('avr-gcc',*flags,'-x','assembler','-c',safe,'-o',obj)
    elif source==ROOT/'platform/main.c':
        from outline_logic import outline
        original=tool('avr-gcc',*flags,'-g0','-S',source,'-o','-')
        assembly,report=outline(original)
        (OUT/'logic-outline.json').write_text(json.dumps(report,indent=2)+'\n')
        (OUT/'main-before-outline.s').write_text(original)
        safe=OUT/'main-outlined.s';safe.write_text(assembly)
        tool('avr-gcc',*flags,'-x','assembler','-c',safe,'-o',obj)
    else:tool('avr-gcc',*flags,'-c',source,'-o',obj)
elf=ROOT/'default/Barbarian.elf'
tool('avr-gcc','-mmcu=atmega644','-Wl,--gc-sections,--relax','-Wl,-Map='+str(ROOT/'default/Barbarian.map'),*objects,'-o',elf)
tool('avr-objcopy','-O','ihex','-R','.eeprom',elf,ROOT/'default/Barbarian.hex')
(ROOT/'default/Barbarian.sym').write_text(tool('avr-nm','-n',elf))
# Custom .text lookup arrays must not leave a following AVR function on an
# odd byte address: CALL/PM addresses are word based and would lose that bit.
for line in tool('avr-readelf','-sW',elf).splitlines():
    fields=line.split()
    if len(fields)>=8 and fields[3]=='FUNC' and fields[6]!='UND':
        assert int(fields[1],16)%2==0, 'AVR function is not word aligned: '+fields[-1]

if net:
    uart_symbol=re.search(r'^([0-9a-fA-F]+)\s+\w\s+un_uart$',(ROOT/'default/Barbarian.sym').read_text(),re.M)
    assert uart_symbol and ((int(uart_symbol[1],16)+78)&255)<=248,'Scanout TX ring must fit one SRAM page'
pair_symbol=re.search(r'^([0-9a-fA-F]+)\s+\w\s+barbAsmComposePairs$',(ROOT/'default/Barbarian.sym').read_text(),re.M)
assert pair_symbol and not (int(pair_symbol[1],16)&31),'AVR pair tables must be 32-byte aligned'
assert (int(pair_symbol[1],16)&255)+4*32<=256,'All four compositor pair tables must fit one SRAM page'
sprite_symbols={n:int(v,16) for v,n in re.findall(r'^([0-9a-fA-F]+)\s+\w\s+(barbAsmSprite(?:Widths|Heights))$',(ROOT/'default/Barbarian.sym').read_text(),re.M)}
assert sprite_symbols['barbAsmSpriteHeights']-sprite_symbols['barbAsmSpriteWidths']==layout_symbols['BARB_SPR_COUNT'],'Sprite width/height tables must remain adjacent'
(ROOT/'default/Barbarian.lss').write_text(tool('avr-objdump','-d','-z',elf))
print(tool('avr-size','-C','--mcu=atmega644',elf))
subprocess.run([sys.executable,str(ROOT/'tools/check_rom_budget.py'),str(elf)],check=True)
subprocess.run(['g++','-O2','-o',str(ROOT/'default/packrom'),str(ROOT/'tools/packrom.cpp')],check=True)
data=b''.join((ROOT/'data'/name).read_bytes() for name in ('BARB-GFX.DAT','BARB-SPC.DAT','BARB-SFX.DAT'))
if sid:data+=(ROOT/'data/BARB-SCORE.DAT').read_bytes()
(ROOT/'default/BARBDATA.DAT').write_bytes(data)
uze=ROOT/'default'/rom_name
print(tool(ROOT/'default/packrom',ROOT/'default/Barbarian.hex',uze,ROOT/'gameinfo.properties'))
if sid:
    assert uze.read_bytes()[base:]==(ROOT/'data/BARB-SCORE.DAT').read_bytes(),'Packed score offset mismatch'
(OUT/'build.json').write_text(json.dumps(dict(video='SFA2 SPI framebuffer',resident_score_bytes=layout_symbols['BARB_SCORE_BYTES'] if sid else 0,music_sd_streaming=False,music_fifo_bytes=int(re.search(r'BARB_MUSIC_FIFO_BYTES\s+(\d+)',(ROOT/'platform/audio.h').read_text())[1]),music_ahead_bytes=layout_symbols['BARB_MUSIC_AHEAD_BYTES'],audio_samples_per_field=262,nominal_audio_hz=15720,physical_audio_hz=28636360/1820,audio='SID resident PCM4+ADPCM2+2' if sid else 'SFA2 ADPCM4+2+2',
    rom_filename=rom_name,network=net,public_display=public,test_shortcut=False,sha256=hashlib.sha256(uze.read_bytes()).hexdigest()),indent=2)+'\n')
