#ifndef BARB_FOREGROUND_EDGES
#define BARB_FOREGROUND_EDGES
#ifdef __AVR__
extern u16 BarbForegroundEdges(u8 y);
#else
static u16 BarbForegroundEdges(u8 y){
    u8 scene=barbScene-1,row=y-32;
    if(scene>=5||row>=80)return 214U<<8;
    u8 edge;
    BarbGraphicsRead(BARB_RUNTIME_BASE+BARB_FOREGROUND_OFFSET+(u16)scene*80+row,&edge,1);
    return (24+(edge>>4))|((u16)(176+(edge&15))<<8);
}
#endif
#endif
