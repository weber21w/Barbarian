/* Shared source head arc. Preserve ordered deadlines and bounce sound gates.
 * Modes: right A/B, left A/B; source left B uses >5 instead of >8. */
static const u8 barbHeadArc[][4] PROGMEM={
    {56,164,16,3},{52,157,15,2},{48,159,14,1},
    {45,157,13,6},{44,157,13,6},{40,159,12,5},
    {36,159,11,4},{32,159,10,3},{29,159,9,2},
    {28,159,9,2},{24,139,8,1},{20,125,7,6},
    {16,125,6,5},{12,118,5,4},{8,111,4,3},
    {4,111,3,2},{0,111,2,1}
};
static BARB_NOINLINE void BarbarianHeadFlight(BarbarianState *g,BarbarianLogicContext *ctx,u8 mode){
    if(ctx->jump)return;
    if(ctx->time>ctx->refA+125){
        g->gnome=1;ctx->refA=ctx->time;g->xGNOME=3;ctx->jump=12;return;
    }
    for(u8 i=0;i<17;i++){
        u8 deadline=pgm_read_byte(&barbHeadArc[i][0]);
        if(mode==3&&i==14)deadline=5;
        if(i==16?ctx->time<ctx->refA:ctx->time<=ctx->refA+deadline)continue;
        if((i==4||i==9)&&g->xSPRt<273)BarbarianMixPlayChannel(2,BARB_SFX_HEAD1,0);
        s16 x=(mode&1)?g->xB:g->xA;
        s8 delta=pgm_read_byte(&barbHeadArc[i][2]);
        if(mode>=2)delta=(mode==3?2:4)-delta;
        g->xSPRt=x+delta;g->ySPRt=pgm_read_byte(&barbHeadArc[i][1]);
        g->numerotete=pgm_read_byte(&barbHeadArc[i][3]);ctx->jump=31;return;
    }
}
