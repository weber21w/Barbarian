/* Presentation only: approach canonical cells in fractional screen pixels.
 * 12/16 px per 50 Hz tick tracks four grid steps per 29-tick walk.
 * Combat/idle ease at two pixels per tick; vertical changes at four pixels.
 * Death, scene changes, teleports and rewinds retain their canonical anchors.
 * None of this state belongs in rollback snapshots or collision calculations. */
#ifdef __AVR__
/* External linkage makes assembly reads/writes visible to C optimization.
 * Explicit zero initializers retain the existing .bss declaration order. */
s16 walkPosition[2]={0};
#else
static s16 walkPosition[2];
#endif
static s16 motionY[2],gnomePosition=-16384;
static u16 walkFrame;
#ifdef __AVR__
u8 walkValid=0,walkGroup[2]={0};
#else
static u8 walkValid,walkGroup[2];
#endif
static u8 walkFacing;
#ifdef __AVR__
extern s16 SmoothWalkX(u8 player,s16 target,s8 state,u16 ticks);
extern s16 SmoothAxis(s16 *position,s16 target,u16 ticks,u8 rate);
asm(".global barbMotionValid\n.set barbMotionValid,walkValid\n"
    ".global barbMotionPosition\n.set barbMotionPosition,walkPosition\n"
    ".global barbMotionGroup\n.set barbMotionGroup,walkGroup\n");
#else
static s16 SmoothWalkX(u8 player,s16 target,s8 state,u16 ticks){
    target*=16;
    s16 difference=target-walkPosition[player];
    u8 group=(u8)(state-4)/5;
    s16 step=ticks*((u8)(state-64)<2?0:group<4?12:32);
    if(!walkValid||state>=78||(u16)(difference+256)>512)
        walkPosition[player]=target;
    else{
        if(difference>step)difference=step;
        if(difference< -step)difference= -step;
        walkPosition[player]+=difference;
    }
    if(!walkValid||group<4)walkGroup[player]=group;
    return walkPosition[player]>>4;
}
static s16 SmoothAxis(s16 *position,s16 target,u16 ticks,u8 rate){
    target*=16;s16 difference=target-*position,step=ticks*rate;
    if(!walkValid||(u16)(difference+512)>1024)*position=target;
    else{
        if(difference>step)difference=step;
        if(difference< -step)difference= -step;
        *position+=difference;
    }
    return *position>>4;
}
#endif
