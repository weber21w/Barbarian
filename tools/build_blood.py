"""Prepare varied cosmetic decapitation sprays and permanent floor deposits.

Commands are signed relative x, absolute y, and packed rectangle width/height.
Physics is prepared on the PC; the AVR only clips/stamps sparse masks, as in
the Mortal Kombat overlay. No gameplay random numbers or state are consumed.
"""
from pathlib import Path
import json, random, struct, math

R=Path(__file__).resolve().parents[1]
FRAMES=32
VARIANTS=8

def frames(seed):
    rng=random.Random(0xBAAB0000+seed)
    particles=[]
    for birth in range(0,64,4):
        # Initial neck jets, then lower spurts as the body crumples.
        # The 80-row source canvases include empty space above the body:
        # neck at display row 62, then row 78 as the knees buckle.
        y=62 if birth<36 else 78+(birth-36)//3
        particles.append((birth,rng.uniform(-3,3),y,rng.uniform(0.7,2.3),rng.uniform(-3.7,-1.5)))
    for birth in range(84,166,8):
        particles.append((birth,rng.uniform(-5,6),106,rng.uniform(-.6,.8),rng.uniform(-1.2,-.3)))
    landed=set()
    for frame in range(FRAMES):
        now=frame*6
        air=[];floor=[]
        for i,(birth,x,y,vx,vy) in enumerate(particles):
            t=(now-birth)/3
            if t<0:continue
            px=round(x+vx*t);py=round(y+vy*t+.23*t*t)
            if py<110:
                if -30<=px<=30 and 24<=py<110:
                    # Compact multi-row drops, stretched vertically while
                    # moving and broader near the apex. No horizontal dashes.
                    speed=vy+.46*t
                    width=4 if abs(speed)<.7 else 2
                    height=2 if width==4 else 3+((i+seed)&1)
                    air.append((px,py,width,min(height,110-py)))
            elif i not in landed:
                landed.add(i)
                px=max(-28,min(25,px))
                floor.append((px,112+(i+seed)%5,2+(i*3+seed)%5,1))
        # Overlapping rounded lobes, widening through tick 180. Narrow top and
        # bottom rows keep the revealed pool from resembling a horizontal bar.
        if now in (72,96,120,150,180):
            radius=4+(now-72)//12
            center=-3+(seed%3)*2
            for row in range(-3,4):
                half=max(1,round(radius*math.sqrt(1-(row/3.3)**2)))
                offset=center+(1 if row>0 else -1)*(seed%2)
                floor.append((offset-half,114+row,half*2,1))
        yield ([(x&~1,y,n,h) for x,y,n,h in air]+
               [(x&~1,y,((x+n+1)&~1)-(x&~1),h) for x,y,n,h in floor])

def effects():
    # One shared small bank: sword burst, airborne head drips, bounce splash.
    # Relative coordinates use y=64 as the attachment point. These are temporary
    # overlays, so corrections never leave predicted hit marks on the scenery.
    for kind,count,steps in [('sword',4,12),('head',2,8),('bounce',6,8),('drag',4,4)]:
        for frame in range(steps):
            runs=[]
            drag_rng=random.Random(0xD24A0000+frame)
            drag_x=drag_rng.sample(range(-6,6,2),4)
            for i in range(count):
                t=(frame+i%3)/2
                if kind=='sword':x=(i+1)*t; y=64-(1+i%3)*t+.35*t*t
                elif kind=='head':x=-3-i*4-frame; y=64+frame*2+i*3
                elif kind=='bounce':x=(i-2.5)*(1+t); y=64-(2+i%3)*t+.4*t*t
                else:
                    if (frame&1) and i==3:continue
                    x=drag_x[i]; y=64+drag_rng.randrange(6)
                height=2 if kind in ('head','bounce','drag') else 1
                runs.append((int(round(x))&~1,int(round(y)),2,height))
            yield runs
    # Small rounded pool under the head's final contact point, kept when kicked.
    yield [(-2,110,4),(-4,111,8),(-4,112,10),(-2,113,6)]

def build():
    commands=[];rectangles=[];index=bytearray();blob=bytearray()
    def store(runs):
        rects=[tuple(run) if len(run)==4 else (*run,1) for run in runs]
        index.extend(struct.pack('<H',len(blob)));assert len(rects)<255
        blob.append(len(rects))
        for x,y,n,h in rects:
            assert -32<=x<32 and 24<=y<118 and 0<n<32 and 1<=h<=8
            blob.extend((x&255,y,n|((h-1)<<5)))
        rectangles.append(rects)
        # Expanded independent pixel oracle; existing floor/pool commands keep
        # their format. The packed bank still spends only 3 bytes per drop.
        commands.append([(x,y+row,n) for x,y,n,h in rects for row in range(h)])
    for variant in range(VARIANTS):
        trail_rng=random.Random(0xD12A0000+variant)
        trail=[(2*trail_rng.randrange(-2,3),110+trail_rng.randrange(5),2) for _ in range(3)]
        for runs in [*frames(variant),trail]:
            store(runs)
    for runs in effects():
        store(runs)
    # The renderer only widens ordinary sword repairs within this body band.
    # Check both attachment heights and every expanded drop, independently of
    # packing; asset edits extending the band must update the compositor.
    wound_rows=[offset+y-64 for offset in (38,54)
                for runs in commands[8*33:8*33+12] for x,y,n in runs]
    assert min(wound_rows)==32 and max(wound_rows)+1==60
    assert all(0<=x and x+n<=24 and 58<=y<70
               for runs in commands[8*33:8*33+12] for x,y,n in runs)
    data=index+blob
    assert len(data)<=8000, len(data)
    (R/'data/blood.raw').write_bytes(data)
    (R/'data/blood.json').write_text(json.dumps(dict(variants=VARIANTS,frames=FRAMES,
        stride=33,ticks_per_frame=6,bytes=len(data),index_bytes=len(index),commands=commands,
        rectangles=rectangles,format='x8-y8-width5-height3',
        effects=dict(variant=8,sword=[0,12],head=[12,8],bounce=[20,8],drag=[28,4],head_pool=[32,1]),
        sword_repair_rows=[32,60]),indent=2)+'\n')
    (R/'data/blood.h').write_text(f'#define BARB_BLOOD_STRIDE 33u\n#define BARB_BLOOD_INDEX_BYTES {len(index)}u\n')
    print(f'Blood: {VARIANTS} sprays, {FRAMES} frames each, {len(data)} SPI bytes')

if __name__=='__main__':build()
