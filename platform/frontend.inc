/* Submenus freeze combat; the title advances only the original title delay.
 * Connections use the normal UzeNet adapter; only RUN advances online play. */
#include "data/menu-font.inc"
#ifdef __AVR__
static const u8 rootLabels[4] __attribute__((section(".text.barbRootLabels"))) =
#else
static const u8 rootLabels[4] =
#endif
    {BARB_MT_SOLO,BARB_MT_LOCAL_VERSUS,BARB_MT_UZENET_ONLINE,BARB_MT_CONTROLS};
static u8 menuChoice,menuPhase;
static BARB_NOINLINE void FrontendTitle(void){barbFrontend=2;menuDirty=2;}
/* Share the release gate with network entry. Keep it with its UI callers so
 * the linker can retain short calls in this nearly full application. */
static BARB_NOINLINE void FrontendClockReset(BarbarianClock *clock){
    clock->phase=1;clock->pending1=clock->pending2=0;
}
static void MenuRead(u16 offset,u8 *dest,u8 n){
#ifdef __AVR__
    /* The resident menu lies wholly in SPI bank 1. */
    BarbarianSpiReadBytes(65536UL|(u16)(BARB_MENU_SPI_BASE+offset),dest,n);
#else
    memcpy(dest,menuData+offset,n);
#endif
}
#ifdef __AVR__
#define BARB_MENU_CODE __attribute__((aligned(2),section(".progmem.data.MenuCode")))
#else
#define BARB_MENU_CODE
#endif
static BARB_MENU_CODE void MenuText(u16 label,u8 y,u8 color){
    cachedPose[0]=255; /* Menu scratch temporarily replaces fighter A's cache. */
    MenuRead(0,workBuf,118);MenuRead(label,workBuf+118,32);
    const u8 *text=workBuf+118;
    u8 length=0;while(text[length])length++;
    u8 column=40-length,bytes=length*2;
    u8 *line=barbLineBuf+column;
    for(u8 row=0;row<5;row++){
        memset(line,0,bytes);u8 x=column*2;
        for(u8 c=0;c<length;c++,x+=4){
            u8 ch=text[c];u16 glyph=((const u16 *)workBuf)[ch-32];
            u8 bits=(glyph>>((4-row)*3))&7;
            for(u8 k=0;k<3;k++)if(bits&(4>>k)){
                u8 p=x+k;if(p&1)barbLineBuf[p>>1]|=color;
                else barbLineBuf[p>>1]|=color<<4;
            }
        }
        /* Write the label itself, preserving the title's side figures. */
        u16 address=back+(u16)(y+row)*(u16)BARB_FB_ROW_BYTES+camera+column;
        BarbarianSpiWriteBytes(address,line,bytes);
    }
}
static void FrontendDraw(void){
    u16 saved=back;
    if(barbFrontend==7)back=front;
    memset(barbLineBuf,0,BARB_FB_ROW_BYTES);
    if(barbFrontend!=2&&menuDirty!=3)for(u8 y=barbFrontend==7?0:52;y<128;y++)BarbarianSpiWriteBytes(back+(u16)y*BARB_FB_ROW_BYTES,barbLineBuf,BARB_FB_ROW_BYTES);
    if(barbFrontend==7){
        if(menuDirty!=3)MenuText(BARB_MT_PAUSED,37,7);
        MenuText(BARB_MT_BACK,61,menuChoice==0?3:7);
        MenuText(BARB_MT_QUIT,77,menuChoice==1?3:7);
    }else if(barbFrontend==2){
        for(u8 i=0,y=65;i<4;i++,y+=12){
#ifdef __AVR__
            u8 label=pgm_read_byte(rootLabels+i);
#else
            u8 label=rootLabels[i];
#endif
            MenuText(label,y,menuChoice==i?4:7);
        }
    }else if(barbFrontend==6){
        MenuText(BARB_MT_CONTROLS,57,4);
        MenuText(BARB_MT_D_PAD_MOVE,69,7);
        MenuText(BARB_MT_B_Y_ACTION,81,7);
        MenuText(BARB_MT_START_IN_GAME_MENU,93,7);
        MenuText(BARB_MT_START_SELECT_JOIN_LEAVE,105,7);
    }else{
        MenuText(BARB_MT_UZENET_ONLINE,57,4);
#if BARB_NET
        u16 message=barbFrontend==5?
            (barbNetwork.error==3?BARB_MT_CONNECTION_TIMEOUT:barbNetwork.error==2?BARB_MT_LINK_ERROR:BARB_MT_ROOM_ERROR):
            (barbNetwork.phase<=UMP_ENTER?BARB_MT_INITIALIZING_LINK:barbNetwork.phase==UMP_CONNECT?
             BARB_MT_CONNECTING:BARB_MT_WAITING_FOR_OPPONENT);
        MenuText(message,77,barbFrontend==5?3:7);
#else
        MenuText(BARB_MT_ONLINE_UNAVAILABLE,77,3);
#endif
        if(barbFrontend==5)MenuText(BARB_MT_RETRY,97,7);
    }
    if(barbFrontend==7)back=saved;else Present();menuDirty=0;
}
static void FrontendPause(BarbarianClock *clock){
    u8 mode=pauseState[0]&127;
    if(mode==4){
#if BARB_NET
        if(barbNetwork.phase)BarbarianNetworkClose();
#endif
        memset(pauseState,0,4);menuChoice=0;FrontendTitle();
    }else if(!mode){
        Present();composeEnding[back?1:0]=255;barbHudShown=255;
        pauseState[3]=barbFrontend=menuDirty=0;
        FrontendClockReset(clock);
    }else if(mode<3&&pauseState[3]!=pauseState[0]){
        menuDirty=pauseState[3]?3:1;
        if(!pauseState[3])Present();
        pauseState[3]=pauseState[0];menuChoice=mode-1;barbFrontend=7;
    }
}
static BARB_MENU_CODE void FrontendConnect(void){
#if BARB_NET
    /* The main loop draws first, then starts/polls the short entry pulse. */
    menuCancel=0;barbFrontend=4;menuDirty=2;menuPhase=UMP_ENTER;
#else
    barbFrontend=5;menuDirty=2;
#endif
}
static u8 FrontendUpdate(BarbarianInput *input,BarbarianClock *clock){
    u8 pad1=input->p1,pad2=input->p2;
    u16 raw=ReadJoypad(0),pressed=raw&~menuRaw;menuRaw=raw;
    u8 chord=(raw&(BTN_START|BTN_SELECT))==(BTN_START|BTN_SELECT);
    /* Canonical pads are zero-extended bytes. Only their low status bits
     * need clearing; avoid rewriting the already-zero high bytes. */
    if(!chord){input->p1&=0xff1f;input->p2&=0xff1f;}
    if(!barbFrontend||barbFrontend==7){
#if BARB_NET
        if(barbNetwork.phase)return 0;
#endif
        /* Any pad input leaves the original AI demonstration. Consume the
         * press before pause, network join or title selection can see it. */
        if(barb.demo){
            if(!(pad1|pad2))return 0;
            FrontendTitle();return 1;
        }
        /* Completed endings consume a fresh confirm before pause handling.
         * The original arret path returns GOTOU=36 to the mode menu. */
        if(!chord&&BarbarianEndingMessage(&barb)){
            if(pressed&(BTN_START|BTN_B|BTN_Y))clock->pending1|=BARB_PAD_START;
            return 0;
        }
        if(chord&&(pressed&(BTN_START|BTN_SELECT)))FrontendConnect();
        else{
            BarbarianInput menu;
            menu.p1=(pad1&63)|((raw&BTN_Y)?64:0);
            menu.p2=barb.partie==2?((pad2&63)|((ReadJoypad(1)&BTN_Y)?64:0)):0;
            if(!PauseStep(&menu)){
                return 0;
            }
            FrontendPause(clock);return 1;
        }
        /* Consume this press: entering a menu must not select a new game. */
        pressed=0;
    }
    if(barbFrontend==2){
        /* GOTOU 100 counts 1500 game ticks (30 seconds). Title
         * activity postpones it; submenu/pause/online screens remain frozen. */
        if(pad1|pad2)barb.temps=0;
        if(pressed&BTN_UP){menuChoice=(menuChoice+3)&3;menuDirty=1;}
        if(pressed&BTN_DOWN){menuChoice=(menuChoice+1)&3;menuDirty=1;}
        if(chord&&(pressed&(BTN_START|BTN_SELECT)))FrontendConnect();
        else if(pressed&(BTN_START|BTN_B|BTN_SELECT)){
            u8 choice=pressed&BTN_SELECT?1:menuChoice;
            if(choice==3){barbFrontend=6;menuDirty=1;}
            else if(choice==2)FrontendConnect();
            else{
                BarbarianInit(&barb);barb.demo=0;barb.IA=0;
                memset(pauseState,0,4);pauseState[1]=(raw&BTN_START)?32:0;
                barb.partie=choice+1;barb.GOTOU=16;barbFrontend=0;
                FrontendClockReset(clock);
                menuDirty=4;input->p1=input->p2=0;
                return 0;
            }
        }
    }else if(barbFrontend==6){
        if(pressed&(BTN_Y|BTN_START))FrontendTitle();
    }else if(barbFrontend==5){
        if(pressed&BTN_Y)FrontendTitle();
        else if(pressed&(BTN_B|BTN_START))FrontendConnect();
    }
#if BARB_NET
    if(barbFrontend==4){
        if(!barbNetwork.phase)FrontendClockReset(clock);
        if(menuCancel||(pressed&BTN_Y)){menuCancel=0;BarbarianNetworkClose();FrontendTitle();}
        else if(barbNetwork.phase==UMP_RUN||barbNetwork.phase>=BN_CHECK){
            barbFrontend=0;barbNetwork.chord=chord;return 0;
        }else{
            if(barbNetwork.phase&&menuPhase!=barbNetwork.phase){menuPhase=barbNetwork.phase;menuDirty=1;}
            input->p1=input->p2=0;return 0;
        }
    }
#endif
    /* Held title inputs suspend the original delay, including a consumed
     * join chord whose START bit would otherwise skip GOTOU 100. */
    return (barbFrontend&~2U)|pad1|pad2;
}
