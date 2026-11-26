/* The existing streamed catalog's software ADSR and parameter conversion.
 * Ordered writes use source-driver cycle offsets; all IO is foreground. */
#include <stdint.h>
#include <string.h>
#ifdef SCORE_IO_HOST
#define PROGMEM
#define pgm_read_word(p) (*(p))
#else
#include <avr/pgmspace.h>
#endif
extern struct ScoreState {uint8_t shadow[25],zero[32],voice[90],clocks[8],unused[2];} score;
extern uint16_t score_cycles;
void ScoreInit(uint8_t);void ScoreStep(void);
uint8_t sidParameters[15];
static struct Envelope {uint32_t clock,rate;uint8_t level,stage;} envelopes[3];
static uint8_t adsr[6],volume,initializing,wrote;
/* Defer clock-only spans until an envelope event or parameter change. The
 * limit is the earliest rate crossing among the three stored voice clocks. */
static uint16_t spanClocks;
static uint8_t spanLimit;
static uint16_t at,sample_phase;
static const uint16_t rates[16] PROGMEM={9,32,63,95,149,220,267,313,392,977,1954,3126,3907,11720,19532,31251};
static void update_rate(uint8_t v){
 struct Envelope *e=envelopes+v;uint8_t ad=adsr[v*2],sr=adsr[v*2+1];
 uint32_t rate=pgm_read_word(rates+(e->stage==0?ad>>4:e->stage==1?ad&15:sr&15));
 if(e->stage){uint8_t n=e->level;if(n>93){}else if(n>54)rate*=2;else if(n>26)rate*=4;else if(n>14)rate*=8;else if(n>6)rate*=16;else rate*=30;}
 e->rate=rate;
}
static void advance(uint16_t clocks){
 /* Cap the wait, not the SID rate; an early check preserves exact timing. */
 spanLimit=255;
 for(uint8_t v=0;v<3;v++){
  /* Rate changes inspect level/stage, so retain the clock in registers until
   * this voice is finished rather than storing/reloading on every step. */
  struct Envelope *e=envelopes+v;uint32_t clock=e->clock+clocks;
  while(clock>=e->rate){
   clock-=e->rate;
   if(e->stage==0){if(e->level<255)++e->level;if(e->level==255){e->stage=1;update_rate(v);}}
   else if(e->stage==2||e->level>(adsr[v*2+1]>>4)*17){
    if(e->level){--e->level;uint8_t n=e->level;if(n==93||n==54||n==26||n==14||n==6)update_rate(v);}
   }else{clock=0;break;}
  }
  e->clock=clock;
  uint32_t next=e->rate-clock;
  if(next<spanLimit)spanLimit=next;
 }
}
static void flush_span(void){advance(spanClocks);spanClocks=0;}
/* Share the three change barriers rather than duplicating their clock check. */
static __attribute__((noinline)) void finish_span(void){if(spanClocks)flush_span();}
void ScoreEmit(uint8_t reg,uint8_t value,uint16_t when){
 if(!initializing){when=when<2464?when:2464;spanClocks+=when>at?when-at:0;if(spanClocks>=spanLimit)flush_span();at=when;wrote=1;}
 if(reg==24)volume=value&15;
 if(reg<21){uint8_t v=0,r=reg;
  if(r>=14){v=2;r-=14;}else if(r>=7){v=1;r-=7;}
  if(r==4){uint8_t *control=sidParameters+v*5+3;if((value^*control)&1){finish_span();envelopes[v].stage=value&1?0:2;envelopes[v].clock=0;update_rate(v);spanLimit=0;}*control=value;}
  if(r==5&&adsr[v*2]!=value){finish_span();adsr[v*2]=value;update_rate(v);spanLimit=0;}
  if(r==6&&adsr[v*2+1]!=value){finish_span();adsr[v*2+1]=value;update_rate(v);spanLimit=0;}
 }
}
void ResidentMusicInit(uint8_t song){
 memset(envelopes,0,sizeof envelopes);memset(adsr,0,sizeof adsr);memset(sidParameters,0,sizeof sidParameters);
 for(uint8_t v=0;v<3;v++){envelopes[v].stage=2;update_rate(v);}
 /* ScoreInit writes volume first; each music frame resets at/wrote before
  * reading them. Avoid duplicate stores to these private scratch fields. */
 sample_phase=466;spanClocks=spanLimit=0;initializing=1;ScoreInit(song);initializing=0;
}
static __attribute__((noinline)) uint16_t pitch_step(uint16_t freq){
 /* 985248/(256*15720) = 10263/41920. A lower fixed-point ratio gives the nearest
  * step or one below it; the small exact residual restores ties-to-even.
  * Its signed value fits in 16 bits, so low-word products suffice. */
 uint16_t upper=((uint32_t)freq*32089UL)>>16;
 uint16_t step=(upper+1)>>1;
 int16_t remain=(uint16_t)((uint16_t)(freq*10263U)-(uint16_t)(step*41920U));
 if(remain>20960||(remain==20960&&(step&1)))++step;
 return step;
}
uint8_t ResidentMusicFrame(void){
 at=0;wrote=0;ScoreStep();spanClocks+=2464-at;flush_span();
 for(uint8_t v=0;v<3;v++){
  uint8_t *p=sidParameters+v*5,*r=score.shadow+v*7;
  if(wrote){
   uint16_t freq=r[0]|(uint16_t)r[1]<<8;
   uint16_t step=pitch_step(freq);
   p[0]=step;p[1]=step>>8;p[2]=(r[2]|(uint16_t)(r[3]&15)<<8)>>4;
  }
  uint16_t amplitude=(uint16_t)envelopes[v].level*volume+127;
  /* Exact division by 255 for 0..3952, without an AVR division loop. */
  p[4]=(p[3]&0xfe)?(amplitude+1+(amplitude>>8))>>8:0;
 }
 /* 2464 source clocks at nominal 15720 Hz = 39 + 293/933 samples. */
 sample_phase+=293;
 if(sample_phase>=933){sample_phase-=933;return 40;}
 return 39;
}
