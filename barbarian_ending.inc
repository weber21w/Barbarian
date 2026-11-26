/* Original Amiga boss hit boxes and completion states (see source-ending audit). */
#include "barbarian_head_flight.inc"
static u8 BarbarianEndingMessage(const BarbarianState *g){
    return g->jeu==2?1:(g->jeu==0&&g->joueurA==84?2:0);
}
static void BarbarianBossDamage(BarbarianState *g,BarbarianLogicContext *ctx){
    if(g->xA<29&&((g->xAttB>g->xTA&&g->xAttB<=g->xTA+2&&g->yAttB==g->yTA)||
                 (g->xAttB>=g->xGA&&g->xAttB<=g->xGA+2&&g->yAttB==g->yGA))){
        g->gnome=0;
        if(g->jeu!=0){
            g->joueurA=82;g->Aoccupe=1;ctx->refA=ctx->time;g->sangA=0;
            g->joueurB=83;g->Boccupe=1;ctx->refB=ctx->time;g->sangB=0;g->jeu=0;
        }
        ctx->jump=26;
    }
    if(!ctx->jump){
        if(g->Aoccupe==1)ctx->jump=26;
        else{g->sangA=0;ctx->jump=22;}
    }
}
