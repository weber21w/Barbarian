"""Prepared, hand-anchored sword edges; never classify clothes at runtime."""
import math
# Tip -> near hilt, in the final Amiga-derived canvas coordinates.
EDGES={
 'crouch1':(6,27,9,39),'crouch2':(0,49,5,54),
 'fall1':(16,61,6,48),'fall3':(9,76,19,76),
 'front1':(0,37,10,22),'front2':(18,3,25,19),'front3':(41,18,30,32),
 'guard-hi':(26,22,16,22),'guard-mid':(23,43,23,27),
 'idle1':(8,18,12,33),'idle2':(3,23,3,44),'idle3':(3,23,3,44),
 'jump1':(25,10,17,18),'jump2':(20,6,15,11),
 'kick1':(12,18,17,31),'kick2':(11,18,17,30),
 'knee1':(26,41,16,39),'knee3':(42,58,27,58),
 'middle1':(13,17,9,29),'middle2':(0,49,6,49),'middle3':(40,46,24,46),
 'neck2':(19,18,26,28),'neck3':(47,31,31,31),
 'roll1':(22,38,17,47),'spin1':(6,20,15,17),'spin3':(21,19,12,17),
 'stand':(8,18,12,33),'touche1':(15,61,6,49),
 'victory1':(25,70,17,55),'victory2':(9,8,22,26),'victory3':(2,13,15,13),
 'walk1':(8,18,12,33),'walk2':(8,18,12,33),'walk3':(8,18,12,33),
 'web_od1':(25,35,14,21),'web_od2':(31,53,19,38),
 'web_od3':(22,36,9,19),'web_od4':(40,52,27,34),
}

# Additional visible weapons in reactions/deaths. No masks on unarmed poses,
# detached heads, shadows, Drax, Marianna or the gnome.
STEEL_EDGES={**EDGES,
 'decap1':(12,71,5,54),'decap2':(25,78,13,69),
 'decap3':(30,78,20,69),'death1':(25,78,20,68),
 'death2':(3,77,15,66),'fall2':(29,78,17,78),
 'spin2':(12,12,12,26),'touche2':(25,49,18,49),
 'headbutt1':(8,21,13,33),'headbutt2':(18,17,18,33)}

# The blade continues across the gray tunic to the grip. A single geometric
# corridor stopped above the torso; trace these pixels explicitly so adjacent
# cloth is never whitened. These five poses share the same upper-body artwork.
CHEST_BLADE=[(11,33),(11,34),(12,34),(12,35),(12,36),(13,36),
             (12,37),(13,37),(13,38),
             # Guard, blade heel and exposed grip: retain the black outlines
             # and the fingers crossing the weapon rather than painting them.
             (14,36),(12,38),(11,39),(14,39),(13,40),(14,40),
             (15,42),(15,43)]
CHEST_POSES={'stand','idle1','walk1','walk2','walk3'}

def steel_points(image,name):
    if name not in STEEL_EDGES:return []
    ax,ay,bx,by=STEEL_EDGES[name];dx=bx-ax;dy=by-ay;norm=dx*dx+dy*dy
    result=[]
    for y in range(image.height):
        for x in range(image.width):
            if image.getpixel((x,y))[:3] not in ((72,72,72),(109,109,145)):continue
            t=((x-ax)*dx+(y-ay)*dy)/norm
            distance=abs((x-ax)*dy-(y-ay)*dx)/math.sqrt(norm)
            if -.08<=t<=1.02 and distance<=1.6:result.append((x,y))
    if name in CHEST_POSES:
        result.extend(p for p in CHEST_BLADE if image.getpixel(p)[:3] in ((72,72,72),(109,109,145)))
    return sorted(set(result))

def encode_blade(image,name):
    edge=points(image,name)
    if edge:
        edge=edge+[edge[-1]]*(7-len(edge))
    steel=sorted(set(steel_points(image,name))|set(edge),key=lambda p:(p[1],p[0]))
    if not steel:return bytes([0]),edge,steel
    # Row-ordered points carry a two-bit blood threshold and six-bit x.
    # Most diagonal/vertical row changes fold into the next point itself.
    grades={}
    for i,p in enumerate(edge):
        grade=1 if i<3 else 2 if i<5 else 3
        grades[p]=min(grades.get(p,3),grade)
    data=bytearray();row=min(y for x,y in steel);last_x=0
    for x,y in steel:
        assert x<48
        grade=grades.get((x,y),0)
        if y==row+1 and abs(x-last_x)<=1:
            data.append((grade<<6)| (49+x-last_x));row=y
        else:
            if y!=row:
                if y==row+1:data.append(63)
                else:data+=bytes([62,y])
                row=y
            data.append((grade<<6)|x)
        last_x=x
    assert len(data)<256
    return data+bytes([min(y for x,y in steel),max(y for x,y in steel),len(data)]),edge,steel


def points(image,name):
    if name not in STEEL_EDGES:return []
    ax,ay,bx,by=STEEL_EDGES[name];dx=bx-ax;dy=by-ay;norm=dx*dx+dy*dy
    candidates=[]
    for y in range(image.height):
        for x in range(image.width):
            if image.getpixel((x,y))[:3] not in ((72,72,72),(109,109,145)):continue
            t=((x-ax)*dx+(y-ay)*dy)/norm
            distance=abs((x-ax)*dy-(y-ay)*dx)/math.sqrt(norm)
            if -.08<=t<=.92 and distance<=1.6:
                candidates.append((t,distance,x,y))
    if not candidates:return []
    # At most seven exact original blade pixels, with no duplicates. Start at
    # the tip and extend coverage towards the hilt over three successful hits.
    selected=[]
    for t in (0,.12,.24,.36,.48,.66,.84):
        unused=[c for c in candidates if c[2:] not in selected]
        if not unused:break
        p=min(unused,key=lambda c:abs(c[0]-t)+c[1]*.08)
        selected.append(p[2:])
    return selected
