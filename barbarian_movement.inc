#include "barbarian_actor.h"
static BARB_NOINLINE void BarbarianBodyX(BarbarianState *,u8,u8);
/* Shared walking pass. Seven-tick phases follow the original disk. Attack
 * gates and pose/box update order retain the audited source translation.
 * State groups: 4..8 forward, 9..13 mirrored forward, 14..18 backward,
 * 19..23 mirrored backward. Each group has an entry plus four timed phases.
 */
static BARB_NOINLINE u8 BarbarianHandleMove(BarbarianState *g, s8 *gotou,
        s16 temps, s16 *reftempsp, u8 playerB){
    s8 *state = &BARB_ACTOR(g,joueurA,-1,playerB);
    s8 *sprite = &BARB_ACTOR(g,spriteA,-1,playerB);
    s8 *x = &BARB_ACTOR(g,xA,1,playerB);
    s8 j = *state;
    u8 group, phase, backward, mirrored, attack;
    s8 weapon, direction;
    s8 *counter;

    /* Clear a completed jump latch on any different action/reaction.
     * A held Up enters the jump handler before reaching this point. */
    if(*gotou==0 && j!=24 && j!=25){
        s8 *repeat=&BARB_ACTOR(g,spriteavanceA,1,playerB);
        if(*repeat==5)*repeat=0;
    }
    if(*gotou != 0 || j < 4 || j > 23)
        return 0U;
    group=(u8)(j-4)/5U;
    phase=(u8)(j-4)%5U;
    backward = group >= 2U;
    mirrored = group & 1U;
    attack = (playerB ? g->attaqueB : g->attaqueA) == 1;
    if(!playerB && g->demo != 0) attack = 0;
    if(group == 0 && g->entree != 0) attack = 0;
    if(group == 2 && (playerB ? g->sortieB : g->sortieA) != 0) attack = 0;

    /* The source publishes boxes/held pose before testing attack input. */
    {
        weapon = *x + ((playerB == mirrored) ? 0 : 4);
        BarbarianBodyX(g,playerB,playerB==mirrored?15:0);
        BARB_ACTOR(g,xAttA,-3,playerB)=weapon;
        if(phase >= 2U)
            *sprite = phase == 4U ? 0 : (backward ? 7 - phase : 3 + phase);
    }
    if(attack){
        *state = (backward ? 58 : 48) + mirrored;
        if(playerB) g->Boccupe = 1; else g->Aoccupe = 1;
        *reftempsp = temps;
        *gotou = playerB ? 24 : 26;
        return 1U;
    }

    counter = backward ? &BARB_ACTOR(g,spritereculeA,1,playerB)
                       : &BARB_ACTOR(g,spriteavanceA,1,playerB);
    direction = (playerB == mirrored) ? 1 : -1;
    if(backward) direction = -direction;
    if(phase == 0U){
        *sprite = backward ? 6 : 4;
        *counter = 1;
        *x += direction;
        *gotou = playerB ? 2 : 1;
    }else if(temps >= *reftempsp + (s16)phase * 7){
        if(phase == 4U){
            *state = mirrored;
            *reftempsp = temps;
            *counter = 0;
        }else{
            *x += direction;
            *sprite = phase == 3U ? 0 : (backward ? 6 - phase : 4 + phase);
            *counter = phase + 1U;
        }
        *gotou = playerB ? 2 : 1;
    }
    if(backward) *gotou = playerB ? 2 : 1;
    return 1U;
}

static u8 BarbarianHandleMoveA(BarbarianState *g, s8 *gotou, s16 temps, s16 *ref){
    return BarbarianHandleMove(g, gotou, temps, ref, 0U);
}
static u8 BarbarianHandleMoveB(BarbarianState *g, s8 *gotou, s16 temps, s16 *ref){
    return BarbarianHandleMove(g, gotou, temps, ref, 1U);
}
