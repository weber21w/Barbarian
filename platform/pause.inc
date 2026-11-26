/* Shared menu state: mode/owner plus previous canonical pads. Byte 3 is
 * display-only and is never restored by rollback. Original combat is frozen. */
static u8 pauseState[4]={0};
#ifdef __AVR__
extern u8 PauseStep(const BarbarianInput *input);
asm(".global barbPauseState\n.set barbPauseState,pauseState\n");
#else
static u8 PauseStep(const BarbarianInput *input){
    u8 a=input->p1,b=input->p2,state=pauseState[0];
    u8 ea=a&~pauseState[1],eb=b&~pauseState[2];
    pauseState[1]=a;pauseState[2]=b;
    if(!state){
        if((ea|eb)&32)pauseState[0]=1|((ea&32)?0:128);
        return pauseState[0]!=0;
    }
    u8 key=state&128?b:a,edge=state&128?eb:ea,mode=state&127;
    if(mode==3){if(!(key&112))pauseState[0]=0;}
    else if(mode<3){
        if(edge&3)state=(state&128)|(3-mode);
        if(edge&64)state=(state&128)|3;
        else if(edge&48)state=(state&128)|((state&127)==1?3:4);
        pauseState[0]=state;
    }
    return 1;
}
#endif
