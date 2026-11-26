/* UzeNet v4 adapter following SFA2's four-frame prediction/repair protocol.
 * SPI snapshots contain only deterministic game state and desired scene.
 * UART is serviced by the video interrupt, including scanout.
 */
#include "network_compat.h"
#include <string.h>
#include <stddef.h>
#include <util/atomic.h>

#define BARB_NET_SPI_BASE BARB_ROLLBACK_BASE
#define BARB_NET_SLOT_BYTES 256U
typedef char BarbNetSnapshotFits[(sizeof(BarbarianState)+6U<=BARB_NET_SLOT_BYTES)?1:-1];
typedef char BarbNetAudioDoesNotOverlap[(BARB_SPIROFF_SFX_DATA +
    BARB_AUDIO_DATA_BYTES<=BARB_NET_SPI_BASE)?1:-1];
typedef char BarbNetSpiFits[(BARB_NET_SPI_BASE+8UL*BARB_NET_SLOT_BYTES<=131072UL)?1:-1];
typedef char BarbNetUartLayout[(offsetof(UnUart,rx_head)==42 &&
    offsetof(UnUart,rx_tail)==43 && offsetof(UnUart,tx_head)==44 &&
    offsetof(UnUart,tx_tail)==45 && offsetof(UnUart,rx)==46 &&
    offsetof(UnUart,tx)==78 && UM_UART_RX_SIZE==32 && UM_UART_TX_SIZE==8)?1:-1];

#if BARB_SFA_VIDEO
/* Foreground publishes requests; video owns READY and PA7 with RX enabled. */
volatile u8 barbUartPause;
#endif

static const u8 barbNetConfig[25] PROGMEM={
    0,0,UM_START,'B','A','R','B','N','E','T','1',BARB_NET_COMPAT_BYTES,
    0,0,0,0,1,2,0xc2,UM_AUTO_DELAY|4,4,0};
#if BARB_SFA_VIDEO
/* Joining suspends gameplay composition. Borrow the large small-actor cache
 * until START's reply and TX have completed; scene loads invalidate its IDs
 * before gameplay resumes. Menus, audio and SD never touch this cache. */
#define barbNetSetup poseCacheLarge
#else
#define barbNetSetup barbNetwork.setup
#endif

static u16 BarbarianNetworkNow(void){return barbNetwork.clock+GetVsyncCounter()*17U;}

static void BarbarianNetworkCapture(void){
#if BARB_SFA_VIDEO
    if(barbFrontend==4&&(ReadJoypad(0)&BTN_Y))menuCancel=1;
#endif
    if(barbNetwork.phase && barbNetwork.phase!=UMP_FAILED){
        u16 raw=ReadJoypad(0);u8 pad=BarbarianPadFromJoy(raw)&63U;
#if BARB_SFA_VIDEO
        if(raw&BTN_Y)pad|=64;
        if((raw&(BTN_START|BTN_SELECT))==(BTN_START|BTN_SELECT))pad&=31;
#else
        pad&=31;
#endif
        barbNetwork.sample|=pad;
    }
}

static void BarbarianNetworkSpiWait(void){
#if BARB_SFA_VIDEO
    AudioBusWait(32);
#else
    extern volatile u8 sync_phase,sync_pulse;
    /* Reserve four whole scanlines before Mode 748 owns SPI. Snapshot bursts
     * are <=32 bytes; foreground cannot run while active scanout is executing. */
    while((m74_config&M74_CFG_ENABLE) && sync_phase &&
          sync_pulse>=252U-BARB_M748_RENDER_START-BARB_M748_RENDER_LINES &&
          sync_pulse<=252U-BARB_M748_RENDER_START+4U);
#endif
}

static void BarbarianNetworkSnapshot(u8 frame,u8 restore){
    u32 address=BARB_NET_SPI_BASE+(u16)(frame&7U)*BARB_NET_SLOT_BYTES;
    u8 *bytes=(u8 *)&barb;
    u16 left=sizeof(barb);
    while(left){
        u8 n=left>32U?32U:left;
        BarbarianNetworkSpiWait();
        if(restore)BarbarianSpiReadBytes(address,bytes,n);
        else BarbarianSpiWriteBytes(address,bytes,n);
        address+=n;bytes+=n;left-=n;
    }
    BarbarianNetworkSpiWait();
    if(restore)BarbarianSpiReadBytes(address,&barbNetwork.scene,1);
    else BarbarianSpiWriteBytes(address,&barbNetwork.scene,1);
    address++;
#if BARB_BLADE_BLOOD
    if(restore)BarbarianSpiReadBytes(address,bladeLevel,2);
    else BarbarianSpiWriteBytes(address,bladeLevel,2);
    address+=2;
#endif
#if BARB_SFA_VIDEO
    BarbarianNetworkSpiWait();
    if(restore)BarbarianSpiReadBytes(address,pauseState,3);
    else BarbarianSpiWriteBytes(address,pauseState,3);
#endif
}

#if defined(__AVR__) && BARB_SFA_VIDEO
extern u32 BarbarianNetworkHash(void);
typedef char BarbPauseCoreBytes[sizeof(BarbarianState)==133?1:-1];
typedef char BarbPauseSceneOffset[offsetof(BarbNetwork,scene)==7?1:-1];
asm(".global barbPauseCore\n.set barbPauseCore,barb\n"
    ".global barbPauseScene\n.set barbPauseScene,barbNetwork+7\n");
#else
static BARB_NOINLINE u32 BarbarianNetworkHash(void){
    u32 crc=0xffffffffUL;
    const u8 *p=(const u8 *)&barb;
    u16 n=sizeof(barb);
    do{
        crc^=*p++;
        for(u8 bit=0;bit<8;bit++)crc=(crc>>1)^((crc&1U)?0xedb88320UL:0);
    }while(--n);
    for(u8 i=0;i<
#if BARB_SFA_VIDEO
        4
#else
        1
#endif
        ;i++){
#if BARB_SFA_VIDEO
        crc^=i?pauseState[i-1]:barbNetwork.scene;
#else
        crc^=barbNetwork.scene;
#endif
        for(u8 bit=0;bit<8;bit++)crc=(crc>>1)^((crc&1U)?0xedb88320UL:0);
    }
    return ~crc;
}
#endif

static void BarbarianNetworkStep(const u8 *pads,u8 replay){
    BarbarianInput input;
    input.p1=(pads[0]|((u16)pads[1]<<8))&127U;
    input.p2=(pads[2]|((u16)pads[3]<<8))&127U;
    BarbarianNetworkSnapshot(barbNetwork.frame,0);
    barbNetSimulating=1;barbNetReplaying=replay;
#if BARB_SFA_VIDEO
    if(!PauseStep(&input))
#endif
    {input.p1&=31;input.p2&=31;BarbarianTick(&barb,&input);}
    barbNetReplaying=barbNetSimulating=0;
    barbNetwork.frame++;
}

static u8 BarbarianNetworkRequest(u8 phase,u8 bytes){
    if(!um_request(&un_uart.serial,un_uart.request,bytes))return 0;
    barbNetwork.phase=phase;return 1;
}

static void BarbarianNetworkPoll(void){
    if(!barbNetwork.phase || barbNetwork.phase==UMP_FAILED)return;
#if BARB_SFA_VIDEO
    AudioRefill();
#endif
    u8 chord=(ReadJoypad(0)&(BTN_START|BTN_SELECT))==(BTN_START|BTN_SELECT);
#if BARB_SFA_VIDEO
    if(barbFrontend==4){
        if(menuCancel||(ReadJoypad(0)&BTN_Y)){menuCancel=0;barbUartPause=1;barbNetwork.phase=UMP_FAILED;return;}
    }else{
#endif
    if(!chord)barbNetwork.chord=0;
    else if(!barbNetwork.chord){
#if BARB_SFA_VIDEO
        barbUartPause=1;
#endif
        barbNetwork.chord=1;barbNetwork.phase=UMP_FAILED;return;
    }
#if BARB_SFA_VIDEO
    }
#endif
    u16 now=BarbarianNetworkNow();
    um_poll(&un_uart.serial,now,7);
    if(un_uart.serial.phase==UM_FAULT){barbNetwork.error=2;barbNetwork.phase=UMP_FAILED;return;}
    if(un_uart.serial.phase>=UM_ENTRY && un_uart.serial.phase<=UM_READY)
        UCSR0B=(1<<RXEN0)|(1<<TXEN0);
    if(un_uart.serial.phase==UM_READY){
        if(barbNetwork.phase==UMP_ENTER){
            um_request(&un_uart.serial,barbNetSetup,21);barbNetwork.phase=UMP_CONNECT;
        }
        if(!un_uart.serial.pending)un_uart.serial.since=now;
        else if((u16)(now-un_uart.serial.since)>=250U){
            um_retry(&un_uart.serial);un_uart.serial.since=now;
        }
    }
    u8 budget=32;
    while(budget-- && (barbNetwork.phase==UMP_ENTER||barbNetwork.phase==UMP_CONNECT||barbNetwork.phase==UMP_WAIT||
          barbNetwork.phase==BN_CHECK||barbNetwork.phase==BN_SETTLE||
          barbNetwork.phase==BN_REPLAY||barbNetwork.phase==BN_FENCE)){
        u8 n=um_uart_read();if(!n)continue;
        u8 *v=un_uart.serial.rx.data;
        if(v[0]!=UM_REPLY)continue;
        if(n<3){barbNetwork.error=1;barbNetwork.phase=UMP_FAILED;break;}
        barbNetwork.last=now;
        if((barbNetwork.phase==UMP_WAIT||barbNetwork.phase==BN_SETTLE) &&
           v[2]==UM_REPAIR && n==5 && v[4] && v[4]<=4 &&
           (u8)(barbNetwork.frame-v[3])==v[4]){
            barbNetwork.resume=barbNetwork.phase;barbNetwork.phase=BN_REPAIR;break;
        }
        if((v[2]&(UM_OK|UM_ERROR|UM_REPAIR))!=UM_OK){
            if(!v[2] && barbNetwork.phase==BN_CHECK){
                barbNetwork.phase=BN_CHECK_DONE;break; /* Retry same check below. */
            }
            barbNetwork.error=n==4?v[3]:1;barbNetwork.phase=UMP_FAILED;break;
        }
        if(barbNetwork.phase==UMP_CONNECT && n==10 && (v[3]==1||v[3]==2) && v[4]==3 && v[5]==2){
            barbNetwork.seat=v[3]-1;barbNetwork.phase=UMP_ROOM;
        }else if(barbNetwork.phase==UMP_WAIT && n==7)barbNetwork.phase=UMP_RUN;
        else if(barbNetwork.phase==BN_REPLAY && n==8 && v[3]==barbNetwork.frame)
            barbNetwork.phase=BN_REPLAY_RUN;
        else if(n==3 && (barbNetwork.phase==BN_SETTLE||barbNetwork.phase==BN_REPLAY||barbNetwork.phase==BN_FENCE))
            barbNetwork.phase++;
        else if(n==3 && barbNetwork.phase==BN_CHECK)barbNetwork.phase=BN_NEXT;
        else{barbNetwork.error=1;barbNetwork.phase=UMP_FAILED;}
    }
    if((u16)(now-barbNetwork.last)>((barbNetwork.phase==UMP_CONNECT)?30000U:10000U)){
        barbNetwork.error=3;barbNetwork.phase=UMP_FAILED;
    }
#if !BARB_SFA_VIDEO
    um_flow(&un_uart.serial,(un_uart.rx_tail-un_uart.rx_head-1)&(UM_UART_RX_SIZE-1));
#endif
}

static void BarbarianNetworkTransaction(u8 phase,u8 opcode,u8 bytes){
    while(un_uart.serial.length && barbNetwork.phase!=UMP_FAILED)BarbarianNetworkPoll();
    if(barbNetwork.phase==UMP_FAILED)return;
    un_uart.request[2]=opcode;
    if(!BarbarianNetworkRequest(phase,bytes)){barbNetwork.phase=UMP_FAILED;return;}
    do{BarbarianNetworkPoll();}while(barbNetwork.phase==phase);
}

static void BarbarianNetworkRepair(void){
    u8 end=barbNetwork.frame;
    barbNetwork.frame=un_uart.serial.rx.data[3];
    BarbarianNetworkSnapshot(barbNetwork.frame,1);
    BarbarianNetworkTransaction(BN_REPLAY,UM_REPLAY_READ,0);
    while(barbNetwork.phase==BN_REPLAY_RUN){
        if(barbNetwork.frame==end){barbNetwork.phase=UMP_FAILED;return;}
        BarbarianNetworkStep(un_uart.serial.rx.data+4,1);
        BarbarianNetworkTransaction(BN_REPLAY,UM_REPLAY_NEXT,0);
    }
    if(barbNetwork.phase!=BN_REPLAY_DONE||barbNetwork.frame!=end){barbNetwork.phase=UMP_FAILED;return;}
    barbNetwork.phase=barbNetwork.resume==UMP_WAIT?BN_NEXT:UMP_RUN;
}

static void BarbarianNetworkSettle(void){
    do{
        BarbarianNetworkTransaction(BN_SETTLE,UM_SETTLE,0);
        if(barbNetwork.phase==BN_REPAIR)BarbarianNetworkRepair();
    }while(barbNetwork.phase==UMP_RUN);
    if(barbNetwork.phase==BN_SETTLED)barbNetwork.phase=UMP_RUN;
}

static void BarbarianNetworkFence(u8 label){
    while(un_uart.serial.length && barbNetwork.phase!=UMP_FAILED)BarbarianNetworkPoll();
    if(barbNetwork.phase==UMP_FAILED)return;
    un_uart.request[3]=label;
    BarbarianNetworkTransaction(BN_FENCE,UM_BARRIER,1);
    if(barbNetwork.phase==BN_FENCE_DONE)barbNetwork.phase=UMP_RUN;
}

static void BarbarianNetworkScene(u8 scene){barbNetwork.scene=scene;}

static void BarbarianNetworkPresent(void){
    if(barbNetwork.scene==barbNetwork.loadedScene)return;
    BarbarianNetworkSettle();
    if(barbNetwork.phase==UMP_FAILED||barbNetwork.scene==barbNetwork.loadedScene)return;
    BarbarianNetworkFence(0x40U|barbNetwork.scene);
    if(barbNetwork.phase==UMP_FAILED)return;
    LoadBG(barbNetwork.scene);
    barbNetwork.loadedScene=barbNetwork.scene;
    BarbarianNetworkFence(0x80U|barbNetwork.scene);
}

static void BarbarianNetworkNext(void){
    barbNetwork.phase=BN_NEXT; /* Release the consumed grant even if TX is busy. */
    if(un_uart.serial.pending||un_uart.serial.length)return;
    if(barbNetwork.checkDue){
        barbNetwork.checkDue=0;
        BarbarianNetworkSettle();if(barbNetwork.phase==UMP_FAILED)return;
        u32 hash=BarbarianNetworkHash();
        memcpy(un_uart.request+3,&hash,4);un_uart.request[2]=UM_CHECK;
        BarbarianNetworkRequest(BN_CHECK,4);return;
    }
    u8 sample;
#ifdef __AVR__
    BarbSyncSafe();
#endif
    sample=barbNetwork.sample;barbNetwork.sample=0;
    un_uart.request[2]=UM_EXCHANGE;un_uart.request[3]=sample;un_uart.request[4]=0;
    BarbarianNetworkRequest(UMP_WAIT,2);
}

static void BarbarianNetworkBegin(void){
#if BARB_SFA_VIDEO
    memset(pauseState,0,4);
#endif
    UCSR0B=0;
#if BARB_SFA_VIDEO
    barbUartPause=0;
#endif
    memset(&un_uart,0,sizeof(un_uart));
    barbNetwork.phase=UMP_ENTER;barbNetwork.count=barbNetwork.error=0;
    barbNetwork.frame=barbNetwork.sample=0;barbNetwork.loadedScene=255;
    barbNetwork.checkDue=0;
    memcpy_P(barbNetSetup,barbNetConfig,sizeof(barbNetConfig));
    UCSR0A=(1<<U2X0);UBRR0H=0;UBRR0L=30;
    UCSR0C=(1<<UCSZ01)|(1<<UCSZ00);
    barbNetwork.last=BarbarianNetworkNow();
    um_reconnect(&un_uart.serial,barbNetwork.last,13,2);
    BarbarianMixHaltChannel(-1);
}

#if BARB_SFA_VIDEO
static __attribute__((aligned(2),section(".progmem.data.MenuCode"))) void NetworkWaitFrame(void){
    ClearVsyncFlag();while(!GetVsyncFlag())AudioRefill();
}
static __attribute__((noinline)) u8 BarbCancelUart(void){
    UnMinimal *s=&un_uart.serial;
    u8 result=0,frames=0;
    /* A cancel reply needs RX enabled and the normal reserve/hysteresis. */
    barbUartPause=0;
    while(s->phase==UM_READY && frames++<210u){
        um_poll(s,(u16)(GetVsyncCounter()*17u),32);
        for(u8 budget=64;budget;--budget){
            int16_t byte=um_cancel_uart_read(&un_uart);
            if(byte<0)break;
            if(um_byte(s,(u8)byte)==2)result=s->rx.data[1];
        }
        if(result==2 || (result==1 && !s->length))break;
        /* The old request is fully encoded. Cancel can reuse its buffer;
         * no stack frame or stack-pointer IRQ masking is needed. */
        if(result!=1 && !s->length && (frames&7u)==1u)um_cancel(s,un_uart.request);
        NetworkWaitFrame();
    }
    UCSR0B=0; /* Transfer ownership before abort, without masking IRQs. */
    um_session_reusable=(result==1 && !s->length);
    um_abort(s);
    return um_session_reusable;
}
#define BARB_CANCEL_UART(u) BarbCancelUart()
#else
#define BARB_CANCEL_UART(u) UM_CANCEL_UART(u)
#endif
static void BarbarianNetworkClose(void){
#if BARB_SFA_VIDEO
    if((pauseState[0]&127)==4)barbNetwork.error=0;
    memset(pauseState,0,4);
#endif
    if(!BARB_CANCEL_UART(&un_uart)){
#if !BARB_SFA_VIDEO
        UCSR0B=0;um_abort(&un_uart.serial);
#endif
        PORTD&=~(1<<PD3);DDRD|=(1<<PD3);
#if BARB_SFA_VIDEO
        NetworkWaitFrame();NetworkWaitFrame();
#else
        WaitVsync(2);
#endif
        DDRD&=~(1<<PD3);
    }
#if !BARB_SFA_VIDEO
    UCSR0B=0;
#endif
    barbNetwork.phase=UMP_OFF;barbNetwork.sample=0;
    BarbarianInit(&barb);
#if BARB_SFA_VIDEO
    barbFrontend=barbNetwork.error?5:2;menuDirty=2;
    menuRaw=ReadJoypad(0); /* Consume the key that cancelled a blocking poll. */
#else
    LoadBG((u8)barb.decorS);
#endif
}

static void BarbarianNetworkStatus(void){
#if !BARB_DEBUG_FRAME_VSYNC_COUNTER && !BARB_SFA_VIDEO
    /* Small status text in Mode 748's existing SRAM margin. */
    static const u8 letters[7][5] PROGMEM={
        {5,7,7,7,5}, /* N */ {7,4,6,4,7}, /* E */ {7,2,2,2,2}, /* T */
        {6,5,6,5,5}, /* R */ {5,5,7,7,5}, /* W */ {2,5,7,5,5}, /* A */
        {7,2,2,2,7}}; /* I */
    u8 active=barbNetwork.phase||barbNetwork.error;
    u8 joining=barbNetwork.phase==UMP_ENTER||barbNetwork.phase==UMP_CONNECT;
    for(u8 y=0;y<5;y++)for(u8 x=0;x<16;x++){
        u8 glyph=barbNetwork.error?(x<4?1:3):joining?(x<4?4:x<8?5:x<12?6:2):x<4?0:x<8?1:2;
        u8 on=active && (joining||x<12) && (x&3U)<3 && (pgm_read_byte(&letters[glyph][y])&(4U>>(x&3U)));
        u8 *p=&vramrow[0].data[(u16)y*BARB_M74_SRAM_ROW_BYTES+(x>>1)];
        u8 color=on?15:0;
        if(x&1)*p=(*p&0xf0)|color;else *p=(*p&15)|(color<<4);
    }
#endif
}

/* Called every display iteration; a RUN grant is consumed at most once.
 * Physical pad capture continues in the VSYNC callback while waiting. */
static u8 BarbarianNetworkFrame(BarbarianInput *input,BarbarianClock *clock){
    BarbarianNetworkStatus();
    u8 chord=(input->p1&(BARB_PAD_START|BARB_PAD_SELECT))==(BARB_PAD_START|BARB_PAD_SELECT);
    if(chord && !barbNetwork.chord){
        barbNetwork.chord=1;
#if BARB_SFA_VIDEO && defined(__AVR__)
        FrontendClockReset(clock);
#else
        clock->phase=1;clock->pending1=clock->pending2=0;
#endif
        if(!barbNetwork.phase)BarbarianNetworkBegin();
        else BarbarianNetworkClose();
        return 1;
    }
    if(!chord)barbNetwork.chord=0;
    if(chord && !barbNetwork.phase)return 1;
    if(!barbNetwork.phase)return 0;
    /* GPIO0's entry pulse/ack has a short deadline. Rendering may span several
     * fields, so keep servicing entry here until the byte protocol is ready. */
    do{BarbarianNetworkPoll();}while(barbNetwork.phase==UMP_ENTER);
    if(barbNetwork.phase==UMP_FAILED){
        BarbarianNetworkClose();return 1;
    }
    if(barbNetwork.phase==UMP_ROOM && !un_uart.serial.length){
        const u8 *seed=un_uart.serial.rx.data+6;
        u16 random=seed[0]|((u16)seed[1]<<8);
        random^=seed[2]|((u16)seed[3]<<8);
        BarbarianSetOriginalDefaults(&barb);
        barb.randSeed=random?random:1;
        barb.partie=2;barb.demo=0;barb.IA=0;barb.GOTOU=16;
        barbNetwork.scene=2;barbNetwork.phase=UMP_RUN;
        BarbarianNetworkFence(0x20);
        BarbarianNetworkPresent();
        if(barbNetwork.phase!=UMP_FAILED){barbNetwork.phase=BN_NEXT;BarbarianNetworkNext();}
    }
    if(barbNetwork.phase==BN_REPAIR)BarbarianNetworkRepair();
    if(barbNetwork.phase==BN_CHECK_DONE){
        if(!un_uart.serial.pending && !un_uart.serial.length){
            un_uart.request[2]=UM_CHECK;BarbarianNetworkRequest(BN_CHECK,4);
        }
    }else if(barbNetwork.phase==BN_NEXT)BarbarianNetworkNext();
    else if(barbNetwork.phase==UMP_RUN){
        input->p1=input->p2=0;
        if(!BarbarianClockPoll(clock,&input->p1,&input->p2))return 1;
        BarbarianNetworkStep(un_uart.serial.rx.data+3,0);
#if BARB_SFA_VIDEO
        if(pauseState[0]||pauseState[3])BarbarianNetworkSettle();
#endif
        if(!(++barbNetwork.count&63U))barbNetwork.checkDue=1;
        BarbarianNetworkPresent();
        if(barbNetwork.phase!=UMP_FAILED)BarbarianNetworkNext();
    }
    return 1;
}
