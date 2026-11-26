/* Resident score -> PCM4. Idle preparation owns graphics scratch only while
 * the main loop is waiting; normal refill never borrows compositor scratch. */
#if BARB_MUSIC_AHEAD_BYTES != 128
#error Music-ahead byte indices require a 128-byte SPI queue
#endif
u16 sidPhase[3];
extern u8 sidParameters[15],barbAsmLineBuf[];
u8 sidHalf,sidLow;
static u16 sidRemaining;
static u8 sidEnabled,sidBusy,sidFault,sidSong=255,sidAheadHead,sidAheadTail;
volatile u16 sidLoops,sidPackets;
extern volatile u8 walk_busy;
void ResidentMusicInit(u8 song);
u8 ResidentMusicFrame(void);
extern void SidRender(u8 *dest,u8 count);
void SidSelect(u8 song){
 if(song==sidSong||song<1||song>3)return;
 sidBusy=1;sidSong=song;sidEnabled=1;sidFault=sidHalf=0;
 spiram_adpcm_music_active=spiram_adpcm_music_volume=0;
 audio_music_head=audio_music_tail;audio_music_eof=0;sidRemaining=0;
 sidAheadHead=sidAheadTail=0;
 memset(sidPhase,0,sizeof sidPhase);ResidentMusicInit(song-1);sidBusy=0;
}
void SidStart(void){SidSelect(2);}
/* Produce at most 32 samples, bounded by complete-byte output capacity. */
static __attribute__((noinline,used)) u8 SidGenerate(u8 *dest,u8 room){
 if(!sidRemaining){sidRemaining=ResidentMusicFrame();sidPackets++;}
 u8 n=sidRemaining>32?32:sidRemaining;
 if(room<17&&n>room*2-sidHalf)n=room*2-sidHalf;
 u8 bytes=(n+sidHalf)>>1;
 SidRender(dest,n);sidRemaining-=n;return bytes;
}
/* One bounded piece per idle poll. The queued PCM precedes generator state. */
#ifndef __AVR__
/* Retain the C implementation as the independent host oracle. */
void SidPrepare(void){
 if(!sidEnabled||sidBusy||sidFault)return;
 u8 room=(sidAheadTail-sidAheadHead-1)&127,end=128-sidAheadHead;
 if(!room)return;
 if(room>end)room=end;
 sidBusy=1;u8 n=SidGenerate(barbAsmLineBuf,room);
 if(n){AudioBusWait(n);walk_busy=1;
  SpiRamWriteFrom(0,BARB_MUSIC_AHEAD_BASE+sidAheadHead,barbAsmLineBuf,n);
  sidAheadHead=(sidAheadHead+n)&127;
 }
 sidBusy=0;
}
#endif
void AudioRefillMusic(void){
 if(!sidEnabled||sidBusy||sidFault)return;
 if(spiram_adpcm_music_active){u8 h=audio_music_head,t=audio_music_tail,ready=h-t;if(h<t)ready+=BARB_MUSIC_FIFO_BYTES;if(ready>=160)return;}
 sidBusy=1;
 u8 head=audio_music_head,tail=audio_music_tail,free=tail>head?tail-head-1:BARB_MUSIC_FIFO_BYTES+tail-head-1;
 while(free){
  u8 room=BARB_MUSIC_FIFO_BYTES-head;if(room>free)room=free;
  u8 n=(sidAheadHead-sidAheadTail)&127;
  if(n){
   if(n>room)n=room;
   u8 end=128-sidAheadTail;
   if(n>end)n=end;
   if(n>32)n=32;
   AudioBusWait(n);walk_busy=1;
   SpiRamReadInto(0,BARB_MUSIC_AHEAD_BASE+sidAheadTail,(u8 *)audio_music_ring+head,n);
   sidAheadTail=(sidAheadTail+n)&127;
  }else n=SidGenerate((u8 *)audio_music_ring+head,room);
  head+=n;if(head==BARB_MUSIC_FIFO_BYTES)head=0;audio_music_head=head;free-=n;
 }
 if(!spiram_adpcm_music_active){GPIOR0=0x88;spiram_adpcm_music_accumulator=128;spiram_adpcm_music_volume=BARB_AUDIO_MUSIC_GAIN;spiram_adpcm_music_active=1;}
 sidBusy=0;
}
