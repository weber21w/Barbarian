/* The four hurtbox columns share one of two x offsets for each pose.
 * Mask bits 0..3 select x+4 for head/front, throat, middle and leg.
 * Keep this separate from attack coordinates, which often change later.
 */
static BARB_NOINLINE void BarbarianBodyX(BarbarianState *g, u8 playerB, u8 mask){
    s8 x = playerB ? g->xB : g->xA;
    s8 f = x + ((mask & 1U) ? 4 : 0);
    s8 t = x + ((mask & 2U) ? 4 : 0);
    s8 m = x + ((mask & 4U) ? 4 : 0);
    s8 leg = x + ((mask & 8U) ? 4 : 0);
    if(playerB){g->xFB=f;g->xTB=t;g->xMB=m;g->xGB=leg;}
    else{g->xFA=f;g->xTA=t;g->xMA=m;g->xGA=leg;}
}

static BARB_NOINLINE void BarbarianBodyY(BarbarianState *g, u8 playerB){
    if(playerB){g->yFB=15;g->yTB=16;g->yMB=18;g->yGB=20;}
    else{g->yFA=15;g->yTA=16;g->yMA=18;g->yGA=20;}
}
