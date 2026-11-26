#include <uzebox.h>
#include <spiram.h>
#include <petitfatfs/pff.h>
#include <avr/pgmspace.h>
#include "Barbarian.h"
#include "barbarian_clock.h"
#include "audio.h"
#include "hud.h"
#if BARB_SID
#include "data/sid-music.h"
#endif
#include "network/barbarian_net.h"
#include <string.h>
#include <util/atomic.h>
#define BARB_NOINLINE __attribute__((noinline))
unsigned char workBuf[BARB_WORKBUF_BYTES];
BarbSprite spr[BARB_NUM_SPRITES];
u8 nextSprite;
static BarbarianState barb;
static u8 barbFrontend=2;
static u8 menuDirty;
static u16 menuRaw;
static volatile u8 menuCancel;
static u8 barbScene;
/* Camera is a byte offset (two pixels); world coordinates stay canonical. */
static u8 camera=13,composeCamera[2]={13,13};
#include "data/spi-timelines.inc"
static u8 barbHeldPoseA,barbHeldPoseB;
extern u8 walk_palette[16];
volatile u8 walk_ready,walk_busy,walk_missed;
/* Linear scanout latches the visible buffer and camera at presentation. */
volatile u16 barbScanBase;
volatile u8 barbScanCamera;
static u16 front,back=BARB_FB_BYTES;
static u8 barbLineBuf[BARB_FB_ROW_BYTES];
static u32 sdSector;
static u8 barbUzeFirstSector; /* Open-success flag in the SFA loader. */
static FATFS filesystem;
static u8 sdBusy;
static volatile u8 disk_error;
static u16 sdBytePos;
static u8 sdCacheValid;
#define barbSpriteCache workBuf
#if BARB_PUBLIC
#define BARB_UZE_FILENAME "BARBCENS.UZE"
static const char barbUzeFilename[] PROGMEM="BARBCENSUZE";
#else
#define BARB_UZE_FILENAME "BARBARIN.UZE"
static const char barbUzeFilename[] PROGMEM="BARBARINUZE";
#endif
extern volatile u8 sync_phase,sync_pulse,render_lines_count,first_render_line;
extern void BarbarianSpiReadBytes(u32 a,u8 *p,u8 n);
extern void BarbarianSpiWriteBytes(u32 a,const u8 *p,u8 n);
extern void BarbSyncSafe(void);
static void BarbarianMemClear(u8 *p,u16 n){memset(p,0,n);}
static void BarbarianMixPlayChannel(s8 c,u8 s,s8 l){
#if BARB_NET
    if(barbNetReplaying)return;
#endif
    AudioPlay(c,s,l);
}
/* Fit this small existing gate into the pre-mixer alignment gap. */
static __attribute__((aligned(2),section(".progmem.data.AudioHaltGate"))) void BarbarianMixHaltChannel(s8 c){
#if BARB_NET
    if(barbNetReplaying)return;
#endif
    AudioHalt(c);
}
void PlaySfx(u8 s){BarbarianMixPlayChannel(-1,s,0);}
#include "blade.inc"
#include "data/runtime-init.inc"
/* Keep the unchanged input adapter in the pre-mixer alignment gap. */
void BarbarianReadInput(BarbarianInput *in)
    __attribute__((aligned(2),section(".progmem.data.PadRead")));
#include "game.inc"
#include "pause.inc"
static void BarbarianSdLoadSector(void){
    WORD bytes=0;
    sdBusy=1;disk_error=pf_lseek(sdSector<<9);
    if(!disk_error)disk_error=pf_read(workBuf,512,&bytes);
    sdBusy=0;
    if(bytes<512)memset(workBuf+bytes,0,512-bytes);
    sdCacheValid=1;
}
static void BarbarianSdStart(u32 pos){
    u32 sector=pos>>9;
    if(sdSector!=sector)sdCacheValid=0;
    sdSector=sector;sdBytePos=pos&511;
}
#if BARB_SID
static void SidSceneCue(void){
    u8 song=2;
    if(barb.jeu==2)song=1;
    else if(barb.sorcier||barb.joueurA==82||barb.joueurB==82||barb.spriteA==55||barb.spriteB==55||barb.spriteA==56||barb.spriteB==56)song=3;
    SidSelect(song);
}
#endif
static u8 BarbarianSdReadU8(void){
    if(sdBytePos==512){sdSector++;sdBytePos=0;sdCacheValid=0;}
    if(!sdCacheValid)BarbarianSdLoadSector();return workBuf[sdBytePos++];
}
static void CopyDisk(u32 source,u32 address,u32 length){
    BarbarianSdStart(source);
    while(length){u8 n=length>sizeof barbLineBuf?sizeof barbLineBuf:length;
        for(u8 i=0;i<n;i++)barbLineBuf[i]=BarbarianSdReadU8();
        BarbarianSpiWriteBytes(address,barbLineBuf,n);address+=n;length-=n;
    }
}
void LoadPalette(const unsigned char *p){for(u8 i=0;i<16;i++)walk_palette[i]=pgm_read_byte(p+i);}
/* Contiguous world rows; scanout selects the visible camera window. */
#ifdef __AVR__
extern void BarbGraphicsRead(u32 a,u8 *p,u8 n);
extern void BarbGraphicsWrite(u32 a,const u8 *p,u8 n);
#else
#define BarbGraphicsRead BarbarianSpiReadBytes
#define BarbGraphicsWrite BarbarianSpiWriteBytes
#endif
static void BackgroundRead(u8 y,u8 start,u8 n){
    BarbGraphicsRead(BARB_SPIVRAM_BG_CLEAN+(u16)y*BARB_FB_ROW_BYTES+start,barbLineBuf,n);
}
static void BackgroundWrite(u8 y,u8 start,u8 n){
    BarbarianSpiWriteBytes(BARB_SPIVRAM_BG_CLEAN+(u16)y*BARB_FB_ROW_BYTES+start,barbLineBuf,n);
}
void LoadBG(u8 decor){
#if BARB_NET
    if(barbNetSimulating){BarbarianNetworkScene(decor);return;}
#endif
    u8 enabled=walk_ready;walk_ready=0;AudioHalt(-1);
    barbScene=decor;camera=13;composeCamera[0]=composeCamera[1]=255;
    extern void InvalidateComposition(void);
    InvalidateComposition();
    u8 bg=!decor?BG_TITLE:decor==255?BG_MENU:decor==5?BG_BOSS:decor==1?BG_PLAINE:decor==3?BG_THRONE:decor==4?BG_ARENA:BG_FOREST;
    BarbarianSpiReadBytes((u16)(BARB_RUNTIME_BASE+BARB_BG_PALETTES_OFFSET+(u16)bg*16),walk_palette,16);
    BarbarianSdStart(SDOFF_GFX_DATA+(u32)bg*13696UL);
    for(u8 y=0;y<BARB_VIEW_H;y++){
        for(u8 x=0;x<BARB_FB_ROW_BYTES;x++)barbLineBuf[x]=BarbarianSdReadU8();
        u16 row=(u16)y*BARB_FB_ROW_BYTES;
        BarbarianSpiWriteBytes(BARB_SPIVRAM_BG_CLEAN+row,barbLineBuf,BARB_FB_ROW_BYTES);
        if(y>=110&&y<118)BarbarianSpiWriteBytes(BARB_BG_FLOOR_LEFT+(u16)(y-110)*BARB_FB_ROW_BYTES,barbLineBuf,BARB_FB_ROW_BYTES);
        BarbarianSpiWriteBytes(front+row,barbLineBuf,BARB_FB_ROW_BYTES);
        BarbarianSpiWriteBytes(back+row,barbLineBuf,BARB_FB_ROW_BYTES);
    }
    /* The kernel may call walk_video on every visible line while blanked.
     * Resume in VSYNC so a reload cannot start a whole raster frame late. */
    if(enabled)while(sync_phase)AudioRefill();
    walk_ready=enabled;
}
/* Assets are prepared at final display resolution; no runtime resampling. */
static s16 ScaleX(s16 x){return x;}
static void BarbarianRestoreActiveDirty(void){}
static void BarbarianSpiFrameBudgetReset(void){}
void ResetSprites(void){nextSprite=0;}
void CleanupSprite(u8 n){(void)n;}
volatile u16 barbPresentations,barbComposeRows,barbRetained;
#ifdef __AVR__
extern void Present(void);
asm(".global barbAsmFront\n.set barbAsmFront,front\n"
    ".global barbAsmCamera\n.set barbAsmCamera,camera\n");
#else
static void Present(void){
    AudioRefill();AudioBusWait(3);walk_busy=1;
    u16 old=front;front=back;back=old;
    barbScanBase=front+13;barbScanCamera=camera;
    walk_busy=0;
    barbPresentations++;
}
#endif
#ifdef __AVR__
extern void BlitSprite(s16 x,s16 y,u8 f,u8 flags);
#else
BARB_NOINLINE void BlitSprite(s16 x,s16 y,u8 f,u8 flags){
    x+=26;
    y-=BARB_BG_CROP_TOP;
    /* RenderFrame submits at most two fighters, followed only by small actors. */
    if(nextSprite>=4||f>=BARB_SPR_COUNT)return;
    s16 x0=ScaleX(x),x1=ScaleX(x+pgm_read_byte(&sprite_widths[f]));
    s16 y1=y+pgm_read_byte(&sprite_heights[f]);
    /* Keep near-edge actors in history: wound spray may still be visible. */
    if(x1<=-12||x0>=226||y1<=0||y>=BARB_VIEW_H)return;
    if(x0<0)x0=0;if(x1>214)x1=214;
    x0&=~1;x1=(x1+1)&~1;
    spr[nextSprite++]=(BarbSprite){x,y,f,flags};
}
#endif
static void DrawEndingText(u8 ending){
    u8 n=ending==1?40:48,column=ending==1?20:16;
    u16 a=BARB_RUNTIME_BASE+BARB_ENDING_BITMAP_OFFSET;
    if(ending==2)a+=320;
    for(u8 y=0;y<8;y++){
        u8 start=(y<BARB_HUD_ROWS?13:camera)+column;
        BackgroundRead(y,start,n);
        BarbarianSpiReadBytes(a,barbLineBuf+48,n);
        for(u8 i=0;i<n;i++){
            u8 ink=barbLineBuf[48+i],mask=ink&0x88;
            mask|=mask>>1;mask|=mask>>2;
            barbLineBuf[i]=(barbLineBuf[i]&mask)|(ink&0x77);
        }
        BarbarianSpiWriteBytes(back+(u16)y*BARB_FB_ROW_BYTES+start,barbLineBuf,n);a+=n;
    }
}
#include "walk_motion.inc"
#ifdef __AVR__
extern s16 GridScreenX(s16 x);
#else
static s16 GridScreenX(s16 x);
#endif
#include "blood.inc"
#include "compositor.inc"
#include "render.inc"
void BarbarianAudioVsync(void){AudioRefill();}
static void FrontendClockReset(BarbarianClock *clock);
#if BARB_NET
#include "network/barbarian_net.inc"
#endif
#include "frontend.inc"
int main(void){
    BarbarianInput input;BarbarianClock clock={1,0,0};
    GPIOR0=GPIOR1=GPIOR2=0;
    disk_error=pf_mount(&filesystem);SpiRamInit();
    if(!disk_error)disk_error=pf_open(BARB_UZE_FILENAME);
    barbUzeFirstSector=!disk_error;
    SetRenderingParameters(FIRST_RENDER_LINE,FRAME_LINES);
    if(barbUzeFirstSector){
        CopyDisk(SDOFF_SPRITE_DATA,BARB_SPIVRAM_SPRITES,BARB_SPR_BANK0_BYTES);
#if BARB_SPR_BANK0_BYTES < BARB_SPR_RESIDENT_BYTES
        CopyDisk(SDOFF_SPRITE_DATA+BARB_SPR_BANK0_BYTES,BARB_SPR_BANK1_BASE,BARB_SPR_RESIDENT_BYTES-BARB_SPR_BANK0_BYTES);
#endif
        CopyDisk(SDOFF_SPRITE_DATA+BARB_SPR_RESIDENT_BYTES,BARB_RUNTIME_BASE,BARB_RUNTIME_BYTES);
        CopyDisk(SDOFF_SFX_DATA,BARB_SPIROFF_SFX_DATA,BARB_AUDIO_DATA_BYTES);
        CopyDisk(SDOFF_SPRITE_DATA+BARB_MENU_FILE_OFFSET,BARB_MENU_SPI_BASE,BARB_MENU_BYTES);
#if BARB_SID
        CopyDisk(BARB_SCORE_FILE_OFFSET,BARB_SCORE_SPI_BASE,BARB_SCORE_BYTES);
#endif
        LoadBG(0);
    }
    FrontendDraw();BarbarianInit(&barb);
#if BARB_SID
    if(barbUzeFirstSector)SidStart();
#endif
#if BARB_NET
    SetUserPostVsyncCallback(BarbarianNetworkCapture);
#endif
    walk_ready=1;ClearVsyncCounter();
    while(1){
        /* Drawing can cross VSYNC. Process already-pending fields immediately;
         * waiting for a fresh flag here adds a whole field of input latency. */
        while(!GetVsyncCounter()){
            AudioRefill();
#if BARB_SID
            if(!GetVsyncCounter())SidPrepare();
#endif
        }
        u8 elapsed;
        BarbSyncSafe();
        elapsed=GetVsyncCounter();ClearVsyncCounter();
#if BARB_NET
        barbNetwork.clock+=elapsed*17U;
#endif
        if(elapsed>6)elapsed=6;
        AudioRefill();BarbarianReadInput(&input);
        u8 blocked=FrontendUpdate(&input,&clock);
        AudioAdvance(elapsed);
        /* SD loads run at this shallow call depth, keeping stack room for
         * the raster/audio interrupts. Draw status before beginning entry. */
        if(menuDirty==4){LoadBG(2);menuDirty=0;ClearVsyncCounter();}
        else if(menuDirty){
            if(menuDirty==2){
                LoadBG(255);
                if(barbFrontend==2)BarbarianInit(&barb);
#if BARB_SID
                SidSelect(2);
#endif
            }
            FrontendDraw();
        }
#if BARB_NET
        if(barbFrontend==4&&!barbNetwork.phase)BarbarianNetworkBegin();
#endif
        if(blocked){ClearVsyncCounter();continue;}
        BarbarianInput sampled=input;
        /* Like SFA2 Advance(), simulation follows elapsed video time. Bound
         * catch-up after a scene load; never let rendering redefine game Hz. */
        while(elapsed--){
        /* NetworkFrame replaces pads with canonical online inputs. Each
         * elapsed field must start with physical controls again, especially
         * the join/cancel chord latch. */
        input=sampled;
        if(
#if BARB_NET
           !BarbarianNetworkFrame(&input,&clock)&&
#endif
           BarbarianClockPoll(&clock,&input.p1,&input.p2)){
            BarbarianTick(&barb,&input);
        }
        }
#if BARB_NET
        if(barbNetwork.phase&&(pauseState[0]||pauseState[3])){
            FrontendPause(&clock);
            if(menuDirty)FrontendDraw();
        }
#endif
        /* Canonical flow labels range from 0 through 101. */
        u8 jump=barb.GOTOU;
        if(barbFrontend){
            if(barbFrontend!=2||jump==100)continue;
            barbFrontend=0;
        }
        /* Original demo rounds return through GOTOU 100. Reuse the title
         * delay between demonstrations instead of leaving a stale arena. */
        if(jump==36||(barb.demo&&jump==100)){
            FrontendTitle();
            ClearVsyncCounter();continue;
        }
#if BARB_SID
        SidSceneCue();
#endif
        if(barb.sorcier&&barbScene!=5)LoadBG(5);
        BarbarianDisplayFrame(&barb);
    }
}
