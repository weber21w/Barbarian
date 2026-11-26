/* Repeated combat sound selection. Keep the counter update and event order. */
/* Fill the existing pre-mixer alignment gap with this small helper. */
static BARB_NOINLINE __attribute__((section(".progmem.data.BarbHitSound")))
void BarbarianHitSound(BarbarianState *g,u8 channel){
    g->choixTOUCHE++;
    if(g->choixTOUCHE>3)g->choixTOUCHE=1;
    if(g->choixTOUCHE>=1&&g->choixTOUCHE<=3)
        BarbarianMixPlayChannel(channel,BARB_SFX_HIT1+g->choixTOUCHE-1,0);
}
static BARB_NOINLINE void BarbarianClashSound(BarbarianState *g){
    g->choixCLING++;
    if(g->choixCLING > 3) g->choixCLING = 1;
    if(g->choixCLING>=1 && g->choixCLING<=3)
        BarbarianMixPlayChannel(3, BARB_SFX_BLOCK1+g->choixCLING-1, 0);
}

static BARB_NOINLINE void BarbarianGruntSound(BarbarianState *g){
    g->songrogne++;
    if(g->songrogne > 6) g->songrogne = 0;
    if(g->songrogne == 3 || g->songrogne == 6)
        BarbarianMixPlayChannel(3, BARB_SFX_PAIN1+(g->songrogne==6), 0);
}

/* Retain the remake fall-effect timing and its headbutt suppression. */
static BARB_NOINLINE void BarbarianFallSound(BarbarianState *g,u8 playerB){
    u8 channel=playerB+1;
    s8 attacker=playerB?g->joueurA:g->joueurB;
    BarbarianMixPlayChannel(channel,BARB_SFX_FALL,0);
    if(attacker==56 || attacker==57)BarbarianMixHaltChannel(channel);
}
