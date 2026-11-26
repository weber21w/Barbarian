#include "barbarian_actor.h"
/* Original-disk crouch/rise: one seven-tick phase precedes the held pose.
 * Transition order matters: completion precedes attack,
 * and some selected knee attacks fall through to their animation this pass.
 */
static BARB_NOINLINE void BarbarianHandleCrouch(BarbarianState *g,
        BarbarianLogicContext *ctx, u8 playerB){
    s8 *state = &BARB_ACTOR(g,joueurA,-1,playerB);
    s8 *busy = &BARB_ACTOR(g,Aoccupe,-1,playerB);
    s16 *reference = (s16 *)((u8 *)ctx+offsetof(BarbarianLogicContext,refA)+2*playerB);
    s8 j = *state;
    u8 mirrored, phase, mask;
    s8 *guardFlag=&BARB_ACTOR(g,protegeDA,1,playerB);
    s8 x = BARB_ACTOR(g,xA,1,playerB);
    if(ctx->jump != 0 || j < 26 || j > 31) return;
    phase=(u8)(j-26);
    mirrored=phase>=3;
    if(mirrored)phase-=3;
    if(phase == 0) mask = playerB == mirrored ? 8 : 7;
    else mask = playerB == mirrored ? 12 : 3;
    u8 guard=phase==2 && *guardFlag==2;
    if(guard)mask=playerB==mirrored?15:0;
    BarbarianBodyX(g, playerB, mask);
    s8 *sprite=&BARB_ACTOR(g,spriteA,-1,playerB);
    BARB_ACTOR(g,xAttA,-3,playerB)=x+(playerB==mirrored?0:4);
    *sprite=phase==1?10:guard?17:9;
    if(phase == 1){
        BARB_ACTOR(g,assisA,1,playerB)=1;
        if(!playerB)*busy=0;
    }
    if(phase == 2)BARB_ACTOR(g,yAttA,-2,playerB)=14;
    s16 age=(s16)((u16)ctx->time-(u16)*reference);
    if(phase != 1 && age >= 7){
        *state = phase == 0 ? 27 + 3*mirrored : mirrored;
        *busy=0;
        if(phase==0)BARB_ACTOR(g,assisA,1,playerB)=1;
        if(guard)*guardFlag=0;
        *sprite=phase==0?10:0;
        ctx->jump = playerB+1;
        return;
    }
    if((playerB ? g->attaqueB : g->attaqueA) == 1 && (playerB || g->demo == 0)){
        *state = 50 + mirrored;*busy = 1;*reference = ctx->time;
    }
    if(phase == 1){
        if(playerB){
            if(g->partie == 1){
                if(ctx->time > *reference + 20) *busy = 0;
                ctx->jump = 2;
            }else *busy = 0;
        }else if(!mirrored && g->demo == 1) ctx->jump = 1;
    }
}
