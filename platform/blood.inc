/* Sparse masked sprays, then permanent red floor deposits. Cosmetic only. */
#include "data/blood.h"
#include "blood.h"
#include "platform/foreground.inc"
static u8 bloodVictim,bloodVariant,bloodMirror,bloodNext,bloodBaseNext,bloodFrame=255;
static u8 bloodVersion,bloodDrawn[2];
static s16 bloodRef,bloodX;
/* Four rectangles per guarded transfer, reusing the same command window.
 * Keep this separate from the output row, which stamping overwrites. */
static u8 bloodCommand[12];
/* Byte x bounds and pixel y bounds. Current bounds temporarily occupy the
 * command window until composition finishes; stamping may then reuse it. */
static u8 bloodBounds[2][4];
#ifdef __AVR__
extern u16 BarbBloodRow(u8 y,u8 buffer);
asm(".global barbAsmBloodBounds\n.set barbAsmBloodBounds,bloodBounds\n"
    ".global barbAsmBloodCurrent\n.set barbAsmBloodCurrent,bloodCommand\n");
#else
static u16 BarbBloodRow(u8 y,u8 buffer){
    u8 left=107,right=0;
    for(u8 set=0;set<2;set++){
        const u8 *b=set?bloodBounds[buffer]:bloodCommand;
        if(y<b[2]||y>=b[3])continue;
        if(b[0]<left)left=b[0];if(b[1]>right)right=b[1];
    }
    return left|((u16)right<<8);
}
#endif
static u8 bloodTrailColumn=255,bloodTrailStart,bloodDrawnFrame[2];
static s8 bloodY;
/* Active emitters: bit 0 head, bit 1 fighter A, bit 2 fighter B. */
static u8 bloodEffect;
static u8 bloodHeadX=255;

static BARB_NOINLINE void BloodRestore(void){
    for(u8 y=110;y<118;y++){
        BarbarianSpiReadBytes(BARB_BG_FLOOR_LEFT+(u16)(y-110)*BARB_FB_ROW_BYTES,barbLineBuf,BARB_FB_ROW_BYTES);
        BackgroundWrite(y,0,BARB_FB_ROW_BYTES);
    }
    bloodNext=bloodBaseNext;
}
static BARB_NOINLINE void BloodStamp(u8 frame,u8 floor){
    u8 *cmd=bloodCommand;
    u16 base=BARB_RUNTIME_BASE+BARB_BLOOD_OFFSET;
    BarbarianSpiReadBytes(base+((u16)bloodVariant*BARB_BLOOD_STRIDE+frame)*2,cmd,2);
    u16 a=base+BARB_BLOOD_INDEX_BYTES+cmd[0]+((u16)cmd[1]<<8);
    BarbarianSpiReadBytes(a++,cmd,1);u8 count=cmd[0];
    while(count){
        u8 batch=count>4?4:count;
        count-=batch;
        BarbarianSpiReadBytes(a,bloodCommand,batch*3);a+=batch*3;
        cmd=bloodCommand;
        for(u8 entry=0;entry<batch;entry++,cmd+=3){
        u8 width=cmd[2]&31,height=(cmd[2]>>5)+1;
        s16 x=(s8)cmd[0];if(bloodMirror)x=-x-width;
        x+=bloodX+26;s16 end=x+width;
        s16 limit=214;
        if(x<0)x=0;if(end>limit)end=limit;if(x>=end)continue;
        u8 lo=x>>1,n=(end-x)>>1;
        memset(barbLineBuf,0x33,n);
        for(u8 row=0;row<height;row++){
            /* Encoded source rows end before 118. Adding signed bloodY can
             * only wrap negative rows into [128,255], outside this window. */
            u8 y=cmd[1]+bloodY+row;
            if((u8)(y-24)>=94||(y>=110)!=floor)continue;
            u8 left=lo,right=lo+n;
            if(lo<20||right>88){
                u16 edge=BarbForegroundEdges(y);
                u8 l=((u8)edge+1)>>1,r=edge>>9;
                if(left<l)left=l;if(right>r)right=r;
                if(left>=right)continue;
            }
            if(floor)BackgroundWrite(y,left,right-left);
            else BarbarianSpiWriteBytes(back+(u16)y*BARB_FB_ROW_BYTES+left,barbLineBuf,right-left);
        }
        }
    }
}
static BARB_NOINLINE void BloodBake(void){
    while(bloodNext<=bloodFrame)BloodStamp(bloodNext++,1);
    if(bloodHeadX!=255){
        s16 x=bloodX;u8 variant=bloodVariant,mirror=bloodMirror;
        bloodX=bloodHeadX;bloodVariant=8;bloodMirror=0;
        BloodStamp(32,1);
        bloodX=x;bloodVariant=variant;bloodMirror=mirror;
    }
}
/* Attach small prepared masks to canonical wounds/head poses. No event queue,
 * gameplay RNG or snapshot changes: a replay selects the corrected mask. */
static BARB_NOINLINE u8 BloodEffects(const BarbarianState *g,u8 draw){
    s16 origin=bloodX;u8 variant=bloodVariant,mirror=bloodMirror,active=0;
    if(draw==2){bloodCommand[0]=107;bloodCommand[1]=0;bloodCommand[2]=128;bloodCommand[3]=0;}
    for(u8 i=0,mask=1;i<3;i++,mask<<=1){
        u8 frame;s16 y;
        if(!i){
            y=g->ySPRt-60;
            if((!BarbarianIsDecapHead(g->joueurA)&&!BarbarianIsDecapHead(g->joueurB))||
               g->xSPRt>=320||y<24||y>=110)continue;
            frame=12+((g->frame>>1)&7);
            if(g->numerotete==3)frame+=8;
            bloodX=((g->xSPRt-40)*2/3+4)&~1;bloodMirror=0;
        }else{
            s8 state=i==1?g->joueurA:g->joueurB;
            u16 age=g->temps-(i==1?g->reftemps:g->reftempsB);
            if((u8)(state-78)<4&&g->gnome&&!g->gnometire){
                frame=28+(((g->frame>>1)^g->xGNOME)&3);y=105;
                bloodX=BARB_DRAG_NECK_X(walkPosition[i-1]>>4,(g->sens==6?i==2:i==1))&~1;
            }else{
            if(!(i==1?g->sangA:g->sangB)||age>=24)continue;
            if(((u8)(state-69)&249)&&
               !((state==78||state==79)&&!g->gnome))continue;
            frame=age>>1;y=state==69||state==71?70:86;
            bloodX=(GridScreenX(i==1?g->xA:g->xB)+13)&~1;
            if(walkValid){
                bloodX=((walkPosition[i-1]>>4)+13)&~1;
                y=(state==69||state==71?38:54)+(motionY[i-1]>>4);
            }
            }
            bloodMirror=(g->sens==6)?i==2:i==1;
            if((u8)(state-78)<4)bloodMirror^=1;
        }
        active|=mask;
        if(draw==2){
            if(!i||frame>=12)continue;
            s16 x=(bloodX>>1)+13-(bloodMirror?12:0),end=x+12;
            s16 top=y-6,bottom=y+6;
            if(x<0)x=0;if(end>107)end=107;
            if(top<24)top=24;if(bottom>110)bottom=110;
            if(x>=end||top>=bottom)continue;
            u8 l=x,r=end,t=top,b=bottom;
            if(l<bloodCommand[0])bloodCommand[0]=l;
            if(r>bloodCommand[1])bloodCommand[1]=r;
            if(t<bloodCommand[2])bloodCommand[2]=t;
            if(b>bloodCommand[3])bloodCommand[3]=b;
        }else if(draw){bloodVariant=8;bloodY=y-64;BloodStamp(frame,0);}
    }
    bloodX=origin;bloodVariant=variant;bloodMirror=mirror;bloodY=0;
    return active;
}
static BARB_NOINLINE void BloodTrail(s16 x){
    if(!bloodVictim)return;
    if(x<0)x=0;if(x>158)x=158;
    u8 end=x>>1;
    if(bloodTrailColumn==255)bloodTrailStart=bloodTrailColumn=end;
    if(end<bloodTrailColumn){
        BloodRestore();BloodBake();
        bloodTrailColumn=bloodTrailStart;
        bloodVersion++;
    }
    while(bloodTrailColumn<end){
        u8 column=++bloodTrailColumn;
        /* Sample a pool twelve pixels behind the neck, below the smear and
         * ordinary drops. Streaks cannot feed themselves or extend forever. */
        BackgroundRead(115,column+7,1);
        u8 smear=barbLineBuf[0]==0x33;
        if(smear||((column^bloodVariant)&1)==0){
            s16 origin=bloodX;u8 variant=bloodVariant;
            bloodX=(u16)column*2;bloodVariant=smear?8:(variant^(column>>1))&7;
            BloodStamp(32,1);bloodX=origin;bloodVariant=variant;bloodVersion++;
        }
    }
}
static BARB_NOINLINE void BloodUpdate(const BarbarianState *g){
#if BARB_PUBLIC
    u8 victim=(u8)(g->joueurA-78)<2?1:(u8)(g->joueurB-78)<2?2:0;
    if(!victim){AudioHalt(4);if(bloodVictim)AudioHalt(5);}
    if(!(victim|bloodVictim))return;
    u8 fresh=victim!=bloodVictim;
    s16 ref=victim==1?g->reftemps:g->reftempsB;
    u16 age=fresh?(u16)(g->temps-ref):(u16)(g->frame-bloodRef);
    if(fresh){bloodVictim=victim;bloodRef=g->frame-age;}
    if(fresh&&victim&&age<5){
        AudioDeathBell(BARB_SFX_DEATHFALL);
        BarbarianMixPlayChannel(5,BARB_SFX_LAUGH,0);
    }
#else
    u8 effect=BloodEffects(g,0);
    if(effect|bloodEffect)bloodVersion++;
    bloodEffect=effect;
    u8 victim=(u8)(g->joueurA-78)<4?1:(u8)(g->joueurB-78)<4?2:0;
    /* Reconcile a predicted death undone by rollback, including a death
     * corrected before its first rendered frame. Other SFX keep their slots. */
    if(!victim){AudioHalt(4);if(bloodVictim)AudioHalt(5);}
    if(!(victim|bloodVictim))return;
    u8 ko=(victim==1?g->joueurA:g->joueurB)<80;
    u8 newVictim=victim!=bloodVictim;
    u8 fresh=newVictim||ko!=(bloodBaseNext>>5);
    s16 ref=victim==1?g->reftemps:g->reftempsB;
    /* The original gnome reuses both animation timers. Keep the cosmetic
     * death epoch on the canonical frame clock instead. */
    u16 age=fresh?(u16)(g->temps-ref):(u16)(g->frame-bloodRef);
    u8 frame=ko||age>=186?31:age/6;
    s16 x=(GridScreenX(victim==1?g->xA:g->xB)+13)&~1;
    if(fresh||(victim&&(frame<bloodFrame||(age<70&&x!=bloodX)))){
        if(bloodVictim)BloodRestore();
        bloodVictim=victim;bloodBaseNext=bloodNext=ko?32:0;bloodFrame=255;
        if(fresh){bloodRef=g->frame-age;bloodVariant=((u16)ref^(u8)g->xA^(u8)g->xB)&7;}
        bloodTrailColumn=255;
        bloodHeadX=255;
        bloodVersion++;
        bloodMirror=(g->sens==6)?victim==1:victim==2;
        bloodX=x;
    }
    if(!victim)return;
    /* Schedule from the final presented death, after rollback correction.
     * Audio events replayed in the simulation are intentionally suppressed. */
    if(fresh&&age<5){
        AudioDeathBell(ko?BARB_SFX_DEATHFALL:BARB_SFX_DECAPITATED);
        if(newVictim)BarbarianMixPlayChannel(5,BARB_SFX_LAUGH,0);
    }
    /* The source's settled head is y=164, then PostDisplay adds seven.
     * Keep its first resting position when the gnome later kicks it away. */
    if(!ko&&bloodHeadX==255&&g->ySPRt==171&&g->xSPRt>=34&&g->xSPRt<274){
        bloodHeadX=((g->xSPRt-40)*2/3+4)&~1;bloodVersion++;
    }
    if(frame!=bloodFrame){bloodFrame=frame;bloodVersion++;}
    BloodBake();
#endif
}
