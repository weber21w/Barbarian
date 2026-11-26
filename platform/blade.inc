/* Sword-hit callback: cosmetic attacker stains, plus requested fatal health. */
#define BARB_BLADE_BLOOD 1
static u8 bladeLevel[2];
extern u8 barbHudPhase;
#if BARB_PUBLIC
static BARB_NOINLINE void BladeHit(BarbarianState *g,u8 victim){
    s8 state=victim?g->joueurB:g->joueurA;
    if((u8)(state-78)<2)barbHudPhase=victim<<1;
}
#elif defined(__AVR__)
extern void BladeHit(BarbarianState *g,u8 victim);
asm(".global barbBladeLevel\n.set barbBladeLevel,bladeLevel\n");
#else
static BARB_NOINLINE void BladeHit(BarbarianState *g,u8 victim){
    s8 state=victim?g->joueurB:g->joueurA;
    if(state!=72&&state!=74&&((victim?g->sangB:g->sangA)||(u8)(state-80)<2)){
        u8 attacker=victim^1;
        if((u8)(state-80)<2){
            if(victim)g->vieB=0;else g->vieA=0;
            barbHudPhase=victim<<1; /* Paint the dead gauge on this draw. */
            bladeLevel[attacker]=3;
        }else if(bladeLevel[attacker]<3)bladeLevel[attacker]++;
    }
}
#endif
#define BARB_BLADE_HIT(player) BladeHit(g,player)
