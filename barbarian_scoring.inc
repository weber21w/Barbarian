/* Original point awards: reaction0=ordinary,1=decapitation,2=knockdown.
 * Rolls score100 for an ordinary air hit and0 for a knockdown. */
#include "barbarian_actor.h"
typedef char BarbScorePair[(offsetof(BarbarianState,scoreB)==offsetof(BarbarianState,scoreA)+4)?1:-1];
static BARB_NOINLINE void BarbarianAwardHit(BarbarianState *g,u8 victim,u8 reaction){
    static const u8 awards[] PROGMEM={125,100,100,100,125,200,125,125};
    u8 move=BARB_ACTOR(g,joueurB,1,victim);
    u8 index=(u8)(move-46)/2;
    u16 points=reaction==1?5000:index<8?pgm_read_byte(awards+index):100;
    if(reaction==2 && (move==32||move==34))points=0;
    s32 *score=(s32 *)((u8 *)g+offsetof(BarbarianState,scoreB)-4*victim);
    *score+=points;
}
