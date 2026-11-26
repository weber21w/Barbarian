#include "barbarian_actor.h"
/* Original disk jump phases at 0/7/14/21, completed at28 ticks.
 * spriteavance==5 is the canonical completed-jump latch; walk uses1..4.
 * An airborne attack hands off to the next player/collision pass; it does not
 * execute the newly selected attack animation in the same pass.
 */
static BARB_NOINLINE void BarbarianHandleJump(BarbarianState *g, s8 *jump,
        s16 time, s16 *reference, u8 playerB){
    s8 *state = &BARB_ACTOR(g,joueurA,-1,playerB);
    s8 *sprite = &BARB_ACTOR(g,spriteA,-1,playerB);
    s8 *middle = &BARB_ACTOR(g,xMA,-1,playerB);
    s8 *leg = &BARB_ACTOR(g,xGA,1,playerB);
    s8 *repeat = &BARB_ACTOR(g,spriteavanceA,1,playerB);
    s8 *busy = &BARB_ACTOR(g,Aoccupe,-1,playerB);
    s8 x = BARB_ACTOR(g,xA,1,playerB);
    u8 mirrored = *state == 25;
    s8 body = x + (playerB == mirrored ? 4 : 0);
    s8 raised = x + (playerB == mirrored ? 1 : 3);
    if(*jump != 0 || (*state != 24 && *state != 25)) return;
    s16 age=(s16)((u16)time-(u16)*reference);
    /* Original descending selector56: roll profiles9/11 hurt;10 trips.
     * Resolve during this actor's jump update, then fall through to damage.
     * Coordinates share the original eight-pixel world grid (scaled to16/3).
     */
    if((u16)(age-14)<7){
        u8 other=playerB^1;
        u8 pose=BARB_ACTOR(g,spriteA,-1,other);
        s8 ox=BARB_ACTOR(g,xA,1,other);
        u8 right=playerB==mirrored;
        u8 gap=right?ox-x:x-ox;
        u8 reach=2-(pose==13);
        /* Only forward rolls publish these pictures with an active weapon. */
        if((u8)(pose-12)<3 && gap<=reach &&
                BARB_ACTOR(g,xAttA,-3,other)==(s8)(ox+(right?-1:5))){
            *state=68+2*mirrored;
            if(pose==13)*state+=4;
            return;
        }
    }
    BarbarianBodyX(g, playerB, playerB == mirrored ? 15 : 0);
    BARB_ACTOR(g,xAttA,-3,playerB)=x+(playerB==mirrored?0:4);
    BARB_ACTOR(g,yAttA,-2,playerB)=14;
    u8 attack=BARB_ACTOR(g,attaqueA,1,playerB)==1;
    u8 grounded=0;
    *jump = playerB+1;
    if(attack && (playerB || g->demo==0)){
        *repeat=0;*state=46+mirrored;*busy=1;*reference=time;
    }else if(*repeat==5 && !g->demo && (!playerB || g->partie==2) &&
            BARB_ACTOR(g,levier1,-1,playerB)!=8){
        *state=mirrored;*busy=0;*repeat=0;
        goto landed;
    }else if(age >= 21){
        if(age>=28){*state=mirrored;*busy=0;*repeat=5;}
landed:
        *sprite=0;*middle=body;*leg=body;
        grounded=1;
    }else if(age >= 14){
        *sprite=7;*middle=body;*leg=x+1;
    }else if(age >= 7){
        *sprite=8;*middle=raised;*leg=raised;
    }else{
        *sprite=7;*middle=body;*leg=raised;
    }
    BARB_ACTOR(g,yGA,4,playerB)=grounded?20:BARB_ACTOR(g,yMA,1,playerB);
    BARB_ACTOR(g,decapiteA,1,playerB)=grounded;
}
