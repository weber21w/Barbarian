/* SFA2's compressed FIFO contract. Decoding/mixing is in sfa2/mixer/three.s. */
#include <uzebox.h>
#include <spiram.h>
#include <avr/pgmspace.h>
#include <string.h>
#include "audio.h"
#include "data/platform-layout.h"
#include "sfx_ids.h"
#include "data/sfx-huff-dir.inc"
volatile u8 audio_music_ring[BARB_MUSIC_FIFO_BYTES],audio_sfx_ring[2][96];
volatile u8 audio_music_head,audio_music_tail,audio_sfx_head[2],audio_sfx_tail[2];
volatile u8 audio_music_underruns,audio_sfx_underruns[2];
volatile u8 audio_music_eof,audio_sfx_eof[2],audio_music_reset_at,audio_music_reset_pending;
volatile u8 spiram_adpcm_music_active,spiram_adpcm_music_volume,spiram_adpcm_music_loop;
volatile u8 spiram_adpcm_music_accumulator=128,spiram_adpcm_music_adpcm_state;
volatile u8 spiram_adpcm_music_boundary_pending;
volatile u8 spiram_adpcm_sfx_active[2],spiram_adpcm_sfx_volume[2],spiram_adpcm_sfx_pending_volume[2];
volatile u8 spiram_adpcm_sfx_accumulator[2]={128,128},spiram_adpcm_sfx_adpcm_state[2],audio_sfx_packed1;
/* Shared with the assembly producer; external linkage makes its reads visible
 * to the C optimizer when AudioPlay/AudioHalt publish this state. */
struct {
    u16 positions[2],remaining[2],starts[2],lengths[2];
    u8 repeats[2];
} barbAudioProducer={0};
#define barbAudioPositions barbAudioProducer.positions
#define barbAudioRemaining barbAudioProducer.remaining
#define barbAudioStarts barbAudioProducer.starts
#define barbAudioLengths barbAudioProducer.lengths
#define barbAudioRepeats barbAudioProducer.repeats
/* Compressed-source state; 16-byte foreground-only compressed input cache. */
extern volatile u8 walk_busy;
u8 audioHuffLeft[4],audioHuffCache[16];
#define audioHuffByte (audioHuffLeft+2)
#ifndef __AVR__
static struct {u16 position;u8 byte,left,cap,at;u8 *base;} huff;
static __attribute__((noinline)) u8 AudioHuffBit(void){
    if(!huff.left){
        if(huff.at==huff.cap){
            AudioBusWait(huff.cap);walk_busy=1;
            SpiRamReadInto(1,huff.position,huff.base,huff.cap);walk_busy=0;
            huff.at=0;
        }
        huff.byte=huff.base[huff.at++];huff.position++;huff.left=8;
    }
    u8 bit=huff.byte&1;huff.byte>>=1;huff.left--;return bit;
}
void AudioReadSfx(u8 lane,u8 head,u8 count){
    huff.base=audioHuffCache;huff.cap=16;huff.at=16;
    huff.position=barbAudioPositions[lane];huff.byte=audioHuffByte[lane];huff.left=audioHuffLeft[lane];
    do {
        u8 packed=0;
        for(u8 j=0;j<4;j++){
            u8 code=2;
            if(AudioHuffBit()){code=1;if(AudioHuffBit())code=AudioHuffBit()?3:0;}
            packed=(packed>>2)|(code<<6);
        }
        audio_sfx_ring[lane][head++]=packed;
    }while(--count);
    barbAudioPositions[lane]=huff.position;audioHuffByte[lane]=huff.byte;audioHuffLeft[lane]=huff.left;
}
#endif
static s8 channels[2];
static u8 priorities[2];
u8 barbDeathBellFields;
u8 barbDeathBellRings;
#if BARB_SFA_VIDEO
extern u8 barbPauseState[4];
#endif
extern volatile u8 walk_busy;
#if BARB_SID
#include "sid.inc"
#endif
#ifdef __AVR__
static const u8 samplePriorities[24] __attribute__((section(".text.barbSamplePriorities"))) =
#else
static const u8 samplePriorities[24] =
#endif
    {4,3,3,3,5,5,1,5,4,3,1,1,4,4,4,4,2,1,1,1,1,2,1,1};
static u8 Priority(u8 s){
#ifdef __AVR__
    return pgm_read_byte(samplePriorities+s);
#else
    return samplePriorities[s];
#endif
}
static void Cancel(u8 i){
    spiram_adpcm_sfx_active[i]=0;spiram_adpcm_sfx_volume[i]=0;
    audio_sfx_head[i]=audio_sfx_tail[i];barbAudioRemaining[i]=0;audioHuffLeft[i]=0;
}
void AudioHalt(s8 channel){
    if(channel<0||channel==4)barbDeathBellFields=barbDeathBellRings=0;
    for(u8 i=0;i<2;i++)if(channel<0||channels[i]==channel)Cancel(i);
}
#ifdef __AVR__
/* Foreground-only countdown; fields are bounded to six by the main loop.
 * Keep the pause gate here: zero-countdown calls return before reading it.
 * The shorter valid-ring selector fits this gate in mixer-alignment padding. */
asm(".pushsection .progmem.data.BellCode,\"ax\",@progbits\n"
    ".balign 2\n.global AudioAdvance\n.type AudioAdvance,@function\n"
    "AudioAdvance:\nlds r18,barbDeathBellFields\ntst r18\nbreq 1f\n"
#if BARB_SFA_VIDEO
    "lds r19,barbPauseState\ntst r19\nbrne 1f\n"
#endif
    "sub r18,r24\nbrcs 2f\nbreq 2f\nsts barbDeathBellFields,r18\n1: ret\n"
    "2: lds r19,barbDeathBellRings\ndec r19\nsts barbDeathBellRings,r19\nbreq 3f\n"
    "ldi r20,86\ncpi r19,6\nbrlo 5f\nldi r20,83\nbreq 5f\nldi r20,77\n"
    "5: add r18,r20\nrjmp 6f\n3: clr r18\n6: sts barbDeathBellFields,r18\n"
    "ldi r24,4\nclr r22\nclr r20\njmp AudioPlay\n"
    ".size AudioAdvance,.-AudioAdvance\n.popsection\n");
#else
void AudioAdvance(u8 fields){
    if(!barbDeathBellFields)return;
#if BARB_SFA_VIDEO
    if(barbPauseState[0])return;
#endif
    if(fields>=barbDeathBellFields){
        u8 left=--barbDeathBellRings;
        if(left)barbDeathBellFields+=(left==7?77:left==6?83:86);
        else{barbDeathBellFields=0;fields=0;}
        AudioPlay(4,BARB_SFX_BELL,0);
    }
    barbDeathBellFields-=fields;
}
#endif
#ifndef __AVR__
/* Independent host reference for the assembly FIFO producer. */
void AudioRefill(void){
    for(u8 i=0;i<2;i++){
        u8 head=audio_sfx_head[i],tail=audio_sfx_tail[i];
        u8 free=tail>head?tail-head-1:96+tail-head-1;
        if(free<32)continue; /* ~17 ms remain at this low-water mark. */
        while(free&&barbAudioRemaining[i]){
            u8 n=96-head;if(n>free)n=free;if(n>barbAudioRemaining[i])n=barbAudioRemaining[i];
            if(n>16)n=16;
            AudioReadSfx(i,head,n);
            head+=n;if(head==96)head=0;audio_sfx_head[i]=head;
            barbAudioRemaining[i]-=n;free-=n;
            if(!barbAudioRemaining[i]){
                if(barbAudioRepeats[i]){audioHuffLeft[i]=0;barbAudioPositions[i]=barbAudioStarts[i];barbAudioRemaining[i]=barbAudioLengths[i];}
                else audio_sfx_eof[i]=1;
            }
        }
    }
#if BARB_SID
    SidRefill();
#endif
}
#else
typedef char AudioProducerLayout[sizeof(barbAudioProducer)==18&&
    __builtin_offsetof(__typeof__(barbAudioProducer),remaining)==4&&
    __builtin_offsetof(__typeof__(barbAudioProducer),starts)==8&&
    __builtin_offsetof(__typeof__(barbAudioProducer),lengths)==12&&
    __builtin_offsetof(__typeof__(barbAudioProducer),repeats)==16?1:-1];
asm(".global barbAudioPositions\n.set barbAudioPositions,barbAudioProducer\n"
    ".global barbAudioRemaining\n.set barbAudioRemaining,barbAudioProducer+4\n"
    ".global barbAudioStarts\n.set barbAudioStarts,barbAudioProducer+8\n"
    ".global barbAudioLengths\n.set barbAudioLengths,barbAudioProducer+12\n"
    ".global barbAudioRepeats\n.set barbAudioRepeats,barbAudioProducer+16\n");
#if BARB_SID
asm(".global barbSidEnabled\n.set barbSidEnabled,sidEnabled\n"
    ".global barbSidBusy\n.set barbSidBusy,sidBusy\n"
    ".global barbSidFault\n.set barbSidFault,sidFault\n"
    ".global barbSidAheadHead\n.set barbSidAheadHead,sidAheadHead\n"
    ".global barbSidAheadTail\n.set barbSidAheadTail,sidAheadTail\n"
    ".global barbSidGenerate\n.set barbSidGenerate,SidGenerate\n");
extern void AudioRefillMusic(void);
#endif
#endif
void AudioPlay(s8 channel,u8 sample,s8 loops){
    if(sample>=BARB_SFX_COUNT)return;
    u8 slot=255,priority=Priority(sample);
    for(u8 i=0;i<2;i++)if(channel>=0&&channels[i]==channel&&spiram_adpcm_sfx_active[i])slot=i;
    if(slot==255)for(u8 i=0;i<2;i++)if(!spiram_adpcm_sfx_active[i]){slot=i;break;}
    if(slot==255){slot=priorities[0]<=priorities[1]?0:1;if(priority<priorities[slot])return;}
    Cancel(slot);
    barbAudioLengths[slot]=pgm_read_word(&sfx_dir[(u16)sample*2+1]);
    barbAudioStarts[slot]=pgm_read_word(&sfx_dir[(u16)sample*2]);
    barbAudioPositions[slot]=barbAudioStarts[slot];barbAudioRemaining[slot]=barbAudioLengths[slot];
    channels[slot]=channel;priorities[slot]=priority;barbAudioRepeats[slot]=loops<0;
    audio_sfx_eof[slot]=audio_sfx_underruns[slot]=0;
    AudioRefill();spiram_adpcm_sfx_pending_volume[slot]=BARB_AUDIO_SFX_GAIN;
    spiram_adpcm_sfx_active[slot]=2;
}
