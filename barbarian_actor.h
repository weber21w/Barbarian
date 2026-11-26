#ifndef BARBARIAN_ACTOR_H
#define BARBARIAN_ACTOR_H
#include <stddef.h>
/* Canonical byte pairs need address arithmetic, not repeated AVR branches.
 * The compile-time checks make these layout assumptions explicit. */
#define BARB_ACTOR(g,field,step,b) (*(s8 *)((u8 *)(g)+offsetof(BarbarianState,field)+(step)*(b)))
#define BARB_PAIR(a,b,d) (offsetof(BarbarianState,b)==offsetof(BarbarianState,a)+(d))
typedef char BarbActorPairsFit[
 BARB_PAIR(joueurA,joueurB,-1)&&BARB_PAIR(spriteA,spriteB,-1)&&
 BARB_PAIR(Aoccupe,Boccupe,-1)&&BARB_PAIR(xA,xB,1)&&
 BARB_PAIR(attaqueA,attaqueB,1)&&BARB_PAIR(levier1,levier2,-1)&&
 BARB_PAIR(xMA,xMB,-1)&&BARB_PAIR(xGA,xGB,1)&&
 BARB_PAIR(spritereculeA,spritereculeB,1)&&BARB_PAIR(spriteavanceA,spriteavanceB,1)&&BARB_PAIR(decapiteA,decapiteB,1)&&
 BARB_PAIR(xAttA,xAttB,-3)&&BARB_PAIR(yAttA,yAttB,-2)&&
 BARB_PAIR(yGA,yGB,4)&&BARB_PAIR(yMA,yMB,1)&&
 BARB_PAIR(assisA,assisB,1)&&BARB_PAIR(protegeDA,protegeDB,1)?1:-1];
#undef BARB_PAIR
#endif
