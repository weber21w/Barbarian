#include <stddef.h>
#include "barbarian_actor.h"
typedef char BarbContactPicturesAdjacent[(offsetof(BarbarianState,spriteA)==offsetof(BarbarianState,spriteB)+1)?1:-1];
typedef char BarbContactOffsetsFit[(sizeof(BarbarianState)<=256)?1:-1];
/* Shared legacy contact ordering, plus original standing/crouch/guard roll rules.
 * Tables live after mixer alignment; they must not move the ISR LUT pages.
 * Results below64 preserve a preparation phase and request a clash in caller;
 * zero misses, and the existing damage/block entry states are all above63. */
static BARB_NOINLINE u8 BarbarianContact(BarbarianState *g,u8 victim){
#define OFF(n) offsetof(BarbarianState,n)
    static const u8 boxes[2][4][2] __attribute__((section(".text.barbContactBoxes")))={
        {{OFF(xFA),OFF(yFA)},{OFF(xTA),OFF(yTA)},{OFF(xMA),OFF(yMA)},{OFF(xGA),OFF(yGA)}},
        {{OFF(xFB),OFF(yFB)},{OFF(xTB),OFF(yTB)},{OFF(xMB),OFF(yMB)},{OFF(xGB),OFF(yGB)}}};
    /* Two bits per published seven-tick phase: immune/first/normal/wide. */
    static const u16 attackRoll[8] __attribute__((section(".text.barbContactRoll")))={
        0x28,0x216,0x2aa,0xaa,0x8c,0x26,0x22aa,0x2aa};
    /* Two bits for first-high, held-high, held-middle, other-guard.
     * Even storage size keeps following AVR instructions word aligned. */
    static const u8 guardTrim[6] __attribute__((section(".text.barbGuardTrim")))={0x41,0x45,0x85,0x55,0x55,0};
    static const u8 counters[2][3] __attribute__((section(".text.barbContactCounters")))={
        {OFF(infoAdegatF),OFF(infoAdegatT),OFF(infoAdegatG)},
        {OFF(infoBdegatF),OFF(infoBdegatT),OFF(infoBdegatG)}};
#undef OFF
    /* Bit1 restricts outgoing B checks to the corrected roll targets. */
    u8 rollOnly=victim&2;
    victim&=1;
    u8 mirror=g->sens==9;
    if(!mirror && g->sens!=6)return 0;
    u8 own=BARB_ACTOR(g,joueurA,-1,victim);
    u8 enemy=BARB_ACTOR(g,joueurB,1,victim);
    if(enemy==32 || enemy==34){
        u8 picture=*((u8 *)g+offsetof(BarbarianState,spriteA)-victim);
        u8 exitGuard=(own==28 || own==31) && BARB_ACTOR(g,protegeDA,1,victim)==2;
        /* Selector63/64 is roll-immune, unlike high guard with the same art. */
        if(picture==17 && (own==42 || own==44 || exitGuard))return 0;
        u8 target=picture<4;
        if((u8)(own-26)<6 && (picture==9 || picture==10))target=1;
        if((u8)(own-38)<4 && (picture==6 || picture==17))target=1;
        if((own==43 || own==45) && picture==18)target=1;
        u8 pose=BARB_ACTOR(g,spriteB,1,victim);
        u8 range=pose==13,attack=(u8)(own-46)<16;
        if(attack && picture>=4){
            u16 age=(u16)(g->temps-(victim?g->reftempsB:g->reftemps));
            /* Incoming B still shows the previous tick; outgoing sees updated A. */
            if(victim && !rollOnly)age--;
            u16 rules=pgm_read_word(&attackRoll[(own-46)>>1]);
            while(age>=7 && rules){rules>>=2;age-=7;}
            u8 rule=rules&3;
            if(!rule || (rule==1 && pose!=12))return 0;
            range=rule==3 || pose==13;
        }
        if(target || attack){
    s8 x=BARB_ACTOR(g,xA,1,victim),ox=BARB_ACTOR(g,xB,-1,victim);
    s8 weapon=BARB_ACTOR(g,xAttB,3,victim);
    u8 right=victim==mirror;
            s16 gap=right?ox-x:x-ox;
            if((u8)(pose-12)>2 || (u16)gap>range ||
                    weapon!=(s8)(ox+(right?-1:5)))return 0;
            g->positionSANG=BARB_ACTOR(g,yGA,4,victim);
            return 72+2*mirror;
        }
    }
    if(rollOnly)return 0;
    s8 x=BARB_ACTOR(g,xA,1,victim),ox=BARB_ACTOR(g,xB,-1,victim);
    s8 weapon=BARB_ACTOR(g,xAttB,3,victim);
    u8 right=victim==mirror;
    /* A neutral weapon cannot hit even when body boxes overlap. */
    if((u8)(enemy-46)<16 && weapon==(s8)(ox+(right?4:0)))return 0;
    if(right?x>=ox:x<=ox)return 0;
    s8 height=BARB_ACTOR(g,yAttB,2,victim);
    if((u8)height<18){
        u8 pose=*((volatile u8 *)g+offsetof(BarbarianState,spriteA)-victim);
        if(pose==10 || (pose&254)==24)return 0;
        /* Amiga profile5: high-first reach1, held-middle reach3; the
         * other guard art has a sound-only outer band, unlike held-high. */
        if(enemy==58+mirror && height==16){
            if(pose==6 || pose==18){
                if((pose==6)==right)weapon++;else weapon--;
            }
            if(pose==17 && own!=39+2*mirror && (u8)(right?weapon-x-4:x-weapon)<2)return own;
        }
        if(pose==17){
            if((u8)(enemy-56)<2)return 0;
            if(enemy==60+mirror){
                /* Original high-guard gaps1..3, relative to headbutt gap4. */
                if((u8)(own-38)>3 || (u8)((right?ox-x:x-ox)-5)>2)return 0;
                /* Sound a preparation block without replacing its phase. */
                if(!(own&1))return own;
            }
        }
    }
    /* Normalize a local tip; never move canonical actors or hurtboxes. */
    {u8 pose=*((volatile u8 *)g+offsetof(BarbarianState,spriteA)-victim);
    if((pose==6 || pose==17 || pose==18) && (u8)(enemy-46)<10 && (enemy&1)==mirror){
        u8 trim=pgm_read_byte(&guardTrim[(enemy-46)>>1]);
        if(pose==18)trim>>=4;
        else if(pose==17){if(own==39+2*mirror)trim>>=2;else trim>>=6;}
        trim&=3;
        if(enemy==52+mirror && pose==6 && BARB_ACTOR(g,spriteB,1,victim)==29)trim=0;
        weapon+=right?trim:-trim;
    }}
    for(u8 point=0;point<4;point++){
        const u8 *box=boxes[victim][point];
        s8 bx=*((s8 *)g+pgm_read_byte(box));
        s8 by=*((s8 *)g+pgm_read_byte(box+1));
        if(height!=by || (right?weapon>bx:weapon<bx))continue;
        if(weapon==bx){
            u8 pose=*((volatile u8 *)g+offsetof(BarbarianState,spriteA)-victim);
            u8 move=*((volatile u8 *)g+offsetof(BarbarianState,joueurB)+victim);
            if((pose<4 || ((pose==6 || pose==18) && (move&254)==60)) && (u8)(move-46)<16 && (move&254)!=56 && (move&254)!=58)continue;
        }
        u8 hurt=68+2*mirror,fall=72+2*mirror,reaction=hurt;
        if(point==0){
            if(own==39+2*mirror){reaction=77;goto no_count;}
            reaction=fall;
        }else if(point==1){
            if(enemy==56+mirror){reaction=fall;goto no_count;}
        }else if(point==2){
            if(own==43+2*mirror)reaction=76;
            goto no_count;
        }else{
            if(enemy==52+mirror || enemy==32+2*mirror){reaction=fall;goto no_count;}
            if(own==43+2*mirror){
                if(enemy==54+mirror)return 0;
                reaction=enemy==50+mirror?hurt:76;goto no_count;
            }
            if(enemy==54+mirror)reaction=fall;
            point=2;
        }
        g->positionSANG=by;
        (*(s8 *)((u8 *)g+pgm_read_byte(&counters[victim][point])))++;
        return reaction;
    no_count:
        g->positionSANG=by;
        return reaction;
    }
    return 0;
}
