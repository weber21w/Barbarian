/* Original ordered head-football deadlines, compacted without changing timing. */
static const u8 barbHeadFootball[][3] PROGMEM={
    {81,0,0},
    {80,153,5},
    {76,0,0},
    {75,151,6},
    {71,0,0},
    {70,153,1},
    {66,0,0},
    {65,157,2},
    {61,0,0},
    {60,159,3},
    {56,0,0},
    {55,157,4},
    {51,0,0},
    {50,153,5},
    {46,0,0},
    {45,151,6},
    {41,0,0},
    {40,153,1},
    {36,0,0},
    {35,157,2},
    {31,0,0},
    {30,159,3},
    {26,0,0},
    {25,157,4},
    {21,0,0},
    {20,153,5},
    {16,0,0},
    {15,151,6},
    {11,0,0},
    {10,153,1},
    {6,0,0},
    {5,157,2},
    {2,0,0},
    {1,0,0},
};
static void BarbarianHeadFootball(BarbarianState *g){
    if(g->GOTOU||g->football!=1)return;
    for(u8 i=0;i<sizeof barbHeadFootball/3;i++){
        if(g->temps<=g->reftempsB+pgm_read_byte(&barbHeadFootball[i][0]))continue;
        u8 y=pgm_read_byte(&barbHeadFootball[i][1]);
        if(y){g->ySPRt=y;g->xSPRt+=16;g->numerotete=pgm_read_byte(&barbHeadFootball[i][2]);}
        g->GOTOU=19;return;
    }
}
