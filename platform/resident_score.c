/* Native port of the three SID songs, with source-derived write timing.
 * Filter arithmetic is omitted because the existing PCM synth ignores it.
 * Audible parameters and their timing match the reference catalog. */
#include <stdint.h>
#include <stdlib.h>
struct ScoreState {uint8_t shadow[25],zero[32],voice[90],clocks[8],unused[2];};
struct ScoreState score;
uint8_t score_sequenced;
uint16_t score_cycles;
#define PS(base,last) do {uint8_t low=(uint8_t)(base);if(low>255-(last))elapsed+=low-(255-(last));} while(0)
#define P(base,index) (elapsed+=(((uint16_t)(uint8_t)(base)+(uint8_t)(index))>>8))
extern uint8_t ScoreRead(uint16_t);
extern void ScoreReadBlock(uint16_t,uint8_t*,uint8_t);
extern void ScoreEmit(uint8_t,uint8_t,uint16_t);
extern uint16_t ScoreReadWord(uint16_t);
static uint16_t get16(const uint8_t *p){return p[0]|(uint16_t)p[1]<<8;}
static void set16(uint8_t *p,uint16_t v){p[0]=v;p[1]=v>>8;}
static uint16_t advance_cost(uint16_t old,uint8_t count){uint16_t elapsed=0;
 /* All callers load immediate count then JSR $9b08. */
 elapsed+=16/*9b08..9b11*/;elapsed+=8;
 if((old&255)+count<=255){elapsed+=1/*taken 9b11*/;}else{elapsed+=7/*9b13..9b13*/;}
 elapsed+=6/*9b16..9b16*/;
return elapsed;}
static uint16_t cutoff(void){uint16_t elapsed=0;elapsed+=54/*9ca3..9cbc*/;return elapsed;}
static uint16_t silence(uint8_t v){uint16_t elapsed=0;
 uint8_t q=v*2,s=v*7;
 elapsed+=14/*9cbd..9cc4*/;if((*(score.zero+(14 + v)))<0x83){elapsed+=1/*taken 9cc4*/;}else{elapsed+=5/*9cc6..9cc8*/;elapsed+=1/*taken 9cc8*/;}
 elapsed+=76/*9ccf..9cfd*/;
 (*(score.voice+(81 + v)))=(*(score.zero+(14 + v)));(*(score.zero+(28)))=0;(*(score.zero+(14 + v)))=0;(*(score.voice+(57 + v)))=0;
 set16((score.voice+(24 + q)),0);set16((score.voice+(30 + q)),0);(*(score.shadow+(4 + s)))&=0xf6;

return elapsed;}
static uint16_t note(uint8_t v,uint8_t n){uint16_t elapsed=0;
 uint8_t q=v*2,s=v*7,t=v*3;
 elapsed+=152/*984a..98aa*/;P(0x9d73+(uint16_t)n*2,1);elapsed+=6/*98ac..98ac*/;
 (*(score.voice+(78 + v)))=n;set16((score.shadow+(0 + s)),ScoreReadWord(0x9d73 + (uint16_t)n*2));
 (*(score.voice+(60 + v)))=(*(score.voice+(72 + v)));(*(score.voice+(57 + v)))=(*(score.voice+(54 + v)));
 (*(score.voice+(69 + v)))=(*(score.voice+(0 + t)));(*(score.voice+(66 + v)))=(*(score.voice+(9 + t)));
 (*(score.voice+(75 + v)))=0;(*(score.voice+(63 + v)))=0;set16((score.voice+(24 + q)),get16((score.voice+(18 + q))));
 (*(score.zero+(26)))=(*(score.zero+(28)));(*(score.zero+(25)))=(*(score.voice+(87)));elapsed+=cutoff();
 elapsed+=36/*98af..98c5*/;
 set16((score.shadow+(2 + s)),get16((score.voice+(42 + q))));(*(score.zero+(29)))=0;(*(score.zero+(27)))=0;(*(score.shadow+(4 + s)))=((*(score.shadow+(4 + s)))&0xf6)|1;
return elapsed;}
static uint16_t instrument(uint8_t v,uint8_t id){uint16_t elapsed=0;
 uint16_t p=0xa23c+(uint16_t)id*12;uint8_t q=v*2,s=v*7,t=v*3,b[12];ScoreReadBlock(p,b,12);
 elapsed+=292/*9949..99e2*/;elapsed+=1/*taken 99e2*/;elapsed+=8/*99ec..99ee*/;elapsed+=1/*taken 99ee*/;
 PS(p,11);
 (*(score.shadow+(5 + s)))=b[0];(*(score.shadow+(6 + s)))=b[1];(*(score.shadow+(4 + s)))=b[2]&0xf0;
 (*(score.voice+(42 + q)))=b[3]^255;(*(score.voice+(43 + q)))=(b[2]&15)^15;
 (*(score.voice+(72 + v)))=b[4];(*(score.voice+(54 + v)))=b[5];
 for(uint8_t i=0;i<3;i++){(*(score.voice+(0 + t+i)))=b[6+i*2];(*(score.voice+(9 + t+i)))=b[7+i*2];}
 if((*(score.voice+(54 + v)))&6)abort();
return elapsed;}
static uint16_t filter(uint8_t v,uint8_t id){uint16_t elapsed=0;
 uint16_t p=ScoreReadWord(0x9f59+(uint8_t)(id*2));uint8_t b[11];ScoreReadBlock(p,b,11);
 elapsed+=89/*9a9f..9ad2*/;P(0x9f59,(uint8_t)(id*2));P(0x9f5a,(uint8_t)(id*2));
 (*(score.zero+(28)))=b[0];

 (*(score.voice+(50)))=b[7]*8;PS(p,10);
 for(uint8_t i=0;i<3;i++){elapsed+=94/*9ad4..9b00*/;if(i<2){elapsed+=2/*taken 9b00*/;}(*(score.voice+(87 + i)))=b[6+i*2];}
return elapsed;}
static uint16_t sequence(void){uint16_t elapsed=0;
 score_sequenced=1;elapsed+=5/*97d7..97d9*/;
 for(int8_t v=2;v>=0;v--){uint8_t q=v*2;
  elapsed+=6/*97db..97dd*/;
  if((*(score.zero+(14 + v)))&128){elapsed+=6/*97df..97df*/;elapsed+=silence(v);}else{elapsed+=1/*taken 97dd*/;}
  elapsed+=6/*97e2..97e5*/;
  if(!((*(score.voice+(81 + v)))&128)){elapsed+=2/*taken 97e5*/;goto voice_done;}
  elapsed+=29/*97e7..97f9*/;
  if(get16((score.voice+(24 + q)))){elapsed+=2/*taken 97f9*/;goto voice_done;}
  for(;;){uint8_t index=(*(score.voice+(81 + v)))*2;uint16_t p=ScoreReadWord(0xa029+index)+get16((score.voice+(30 + q)));uint8_t c=ScoreRead(p),a=0,count=0;
   elapsed+=49/*97fb..981b*/;P(0xa029,index);P(0xa02a,index);
   if(c>=0xf3&&c!=0xf8)a=ScoreRead(p+1);
   if(c<0xc0){elapsed+=16/*981d..9823*/;elapsed+=note(v,c*2);count=1;}
   else{
    elapsed+=1/*taken 981b*/;elapsed+=6/*983e..983e*/;elapsed+=6/*98cd..98d1*/;
    if(c<0xd0){elapsed+=32/*98d3..98e4*/;P(0x9ef3,(c&15)*2);P(0x9ef4,(c&15)*2);set16((score.voice+(18 + q)),ScoreReadWord(0x9ef3+(c&15)*2));count=1;}
    else{
     elapsed+=1/*taken 98d1*/;elapsed+=7/*98ed..98ef*/;
     if(c<0xf0||c>0xf9)abort();
     elapsed+=(uint8_t)(0xf9-c)*14;
     elapsed+=6/*98f1..98f4*/;elapsed+=27/*98f6..9904*/;
     switch(c){
      case 0xf0:elapsed+=26/*990b..991a*/;set16((score.voice+(24 + q)),get16((score.voice+(18 + q))));count=1;break;
      case 0xf3:elapsed+=instrument(v,a);count=2;break;
      case 0xf4:{
       elapsed+=65/*99fb..9a21*/;PS(p,2);
       P(0x9ef3,(a&15)*2);P(0x9ef4,(a&15)*2);
       uint8_t duration=ScoreRead(p+2);
       P(0x9ef3,(duration&15)*2);P(0x9ef4,(duration&15)*2);
       set16((score.voice+(18 + q)),ScoreReadWord(0x9ef3+(a&15)*2)+ScoreReadWord(0x9ef3+(duration&15)*2));count=3;break;}
      case 0xf5:elapsed+=48/*9a2a..9a46*/;P(p,1);(*(score.voice+(81 + v)))=a|128;set16((score.voice+(30 + q)),0);break;
      case 0xf6:elapsed+=17/*9a47..9a4c*/;elapsed+=32/*b4ef..b503*/;elapsed+=20/*9a4f..9a58*/;P(p,1);P(0x9f13,(uint8_t)(a*2));P(0x9f14,(uint8_t)(a*2));set16((score.clocks+(6)),ScoreReadWord(0x9f13+(uint8_t)(a*2)));set16((score.clocks+(2)),0);count=2;break;
      case 0xf7:elapsed+=24/*9a5f..9a6b*/;P(p,1);elapsed+=advance_cost(get16((score.voice+(30 + q))),2);elapsed+=36/*9a73..9a87*/;(*(score.voice+(84 + v)))=(*(score.voice+(81 + v)));(*(score.voice+(81 + v)))=a|128;set16((score.voice+(36 + q)),get16((score.voice+(30 + q)))+2);set16((score.voice+(30 + q)),0);break;
      case 0xf8:elapsed+=39/*9a88..9a9e*/;(*(score.voice+(81 + v)))=(*(score.voice+(84 + v)));set16((score.voice+(30 + q)),get16((score.voice+(36 + q))));break;
      case 0xf9:elapsed+=filter(v,a);P(p,1);count=2;break;
      default:abort();
     }
    }
   }
   if(count){elapsed+=advance_cost(get16((score.voice+(30 + q))),count);set16((score.voice+(30 + q)),get16((score.voice+(30 + q)))+count);elapsed+=6;}
   if(c<0xc0){elapsed+=3/*9826..9826*/;break;}
   elapsed+=8/*9841..9845*/;
   if(c==0xf0){elapsed+=1/*taken 9845*/;break;}
   elapsed+=3/*9847..9847*/;
  }
voice_done:
  elapsed+=4/*9828..9829*/;if(v){elapsed+=2/*taken 9829*/;}
 }
 elapsed+=6/*982b..982b*/;elapsed+=2/*9b17..9b17*/;
 for(int8_t v=2;v>=0;v--){elapsed+=16/*9b19..9b22*/;uint16_t d=get16((score.voice+(24 + v*2)));
  if(!d){elapsed+=1/*taken 9b22*/;}else{
   elapsed+=30/*9b24..9b38*/;set16((score.voice+(24 + v*2)),--d);
   if(d){elapsed+=1/*taken 9b38*/;}else{elapsed+=15/*9b3a..9b42*/;(*(score.shadow+(4 + v*7)))&=0xf6;}
  }
  elapsed+=4/*9b45..9b46*/;if(v){elapsed+=1/*taken 9b46*/;}
 }
 elapsed+=5/*9b48..9b4a*/;elapsed+=1/*taken 9b4a*/;elapsed+=6/*9b4e..9b4e*/;
 elapsed+=6/*982e..982e*/;elapsed+=10/*9cfe..9d04*/;elapsed+=1/*taken 9d04*/;elapsed+=5/*9d17..9d19*/;elapsed+=1/*taken 9d19*/;elapsed+=6/*9d25..9d25*/;
 if((*(score.zero+(20)))||(*(score.zero+(18)))&128||(*(score.zero+(19)))&128)abort();
 elapsed+=22/*9831..983d*/;(*(score.shadow+(24)))=15;
return elapsed;}
static uint16_t pitch(uint8_t v){uint16_t elapsed=0;
 elapsed+=8/*9bb6..9bbb*/;
 if(!((*(score.voice+(57 + v)))&64)){elapsed+=1/*taken 9bbb*/;elapsed+=6/*9bfd..9bfd*/;return elapsed;}
 elapsed+=14/*9bbd..9bc6*/;uint8_t n=(*(score.voice+(78 + v)))+(*(score.voice+(69 + v)));
 if(n<192){elapsed+=1/*taken 9bc6*/;}else{
  elapsed+=9/*9bc8..9bcc*/;
  if((*(score.voice+(69 + v)))&128){elapsed+=9/*9bce..9bd1*/;n-=63;}else{elapsed+=1/*taken 9bcc*/;elapsed+=6/*9bd4..9bd5*/;n-=192;}
 }
 elapsed+=76/*9bd7..9bfd*/;P(0x9d73+(uint16_t)n*2,1);(*(score.voice+(78 + v)))=n;set16((score.shadow+(0 + v*7)),ScoreReadWord(0x9d73+(uint16_t)n*2));
return elapsed;}
static uint16_t pulse(uint8_t v){uint16_t elapsed=0;
 elapsed+=8/*9bfe..9c03*/;
 if(!((*(score.voice+(57 + v)))&32)){elapsed+=1/*taken 9c03*/;elapsed+=6/*9c2d..9c2d*/;return elapsed;}
 elapsed+=14/*9c05..9c0e*/;for(uint8_t i=0;i<4;i++){elapsed+=14/*9c10..9c15*/;if(i<3){elapsed+=1/*taken 9c15*/;}}
 elapsed+=38/*9c17..9c2d*/;set16((score.shadow+(2 + v*7)),(get16((score.shadow+(2 + v*7)))+(uint16_t)(*(score.voice+(69 + v)))*16)&4095);
return elapsed;}
static uint16_t filter_tick(void){uint16_t elapsed=0;
 elapsed+=5/*9c58..9c5a*/;
 if(!(*(score.zero+(28)))){elapsed+=1/*taken 9c5a*/;elapsed+=6/*9c71..9c71*/;return elapsed;}
 elapsed+=5/*9c5c..9c5e*/;
 if((*(score.zero+(26)))){elapsed+=1/*taken 9c5e*/;elapsed+=6/*9c71..9c71*/;return elapsed;}
 for(;;){elapsed+=5/*9c60..9c62*/;
  if(!(*(score.zero+(25)))){elapsed+=1/*taken 9c62*/;}else{
   elapsed+=5/*9c64..9c66*/;
   if((*(score.zero+(25)))<(*(score.zero+(27)))){elapsed+=1/*taken 9c66*/;}else{
    elapsed+=6/*9c68..9c68*/;elapsed+=29/*9c8f..9ca1*/;elapsed+=cutoff();
    elapsed+=17/*9c6b..9c71*/;++(*(score.zero+(27)));(*(score.zero+(26)))=(*(score.zero+(28)));return elapsed;
   }
  }
  elapsed+=9/*9c72..9c77*/;uint8_t t=(*(score.zero+(29)))+1;
  if(t<3){elapsed+=1/*taken 9c77*/;}else{elapsed+=8/*9c79..9c7e*/;t=0;if(!(*(score.voice+(50)))){elapsed+=6/*9c80..9c80*/;return elapsed;}elapsed+=1/*taken 9c7e*/;}
  elapsed+=18/*9c81..9c8c*/;(*(score.zero+(29)))=t;(*(score.zero+(25)))=(*(score.voice+(87 + t)));(*(score.zero+(27)))=0;
 }
return elapsed;}
static uint16_t effects(uint16_t elapsed){
 elapsed+=7/*9d52..9d56*/;
 for(int8_t v=2;v>=0;v--){elapsed+=6/*9d58..9d5b*/;if((*(score.voice+(60 + v)))){elapsed+=7/*9d5d..9d5d*/;--(*(score.voice+(60 + v)));}else{elapsed+=1/*taken 9d5b*/;}elapsed+=4/*9d60..9d61*/;if(v){elapsed+=1/*taken 9d61*/;}}
 elapsed+=5/*9d63..9d65*/;if((*(score.zero+(26)))){elapsed+=5/*9d67..9d67*/;--(*(score.zero+(26)));}else{elapsed+=1/*taken 9d65*/;}
 elapsed+=6/*9d69..9d69*/;elapsed+=2/*9b4f..9b4f*/;
 for(int8_t v=2;v>=0;v--){
  for(;;){elapsed+=6/*9b51..9b54*/;if(!((*(score.voice+(57 + v)))&128)){elapsed+=1/*taken 9b54*/;break;}
   elapsed+=6/*9b56..9b59*/;if((*(score.voice+(60 + v)))){elapsed+=1/*taken 9b59*/;break;}
   elapsed+=8/*9b5b..9b60*/;uint8_t limit=(*(score.voice+(66 + v)))&127;
   if(!limit){elapsed+=1/*taken 9b60*/;}else{
    elapsed+=6/*9b62..9b65*/;
    if(limit<(*(score.voice+(63 + v)))){elapsed+=1/*taken 9b65*/;}else{
     elapsed+=6/*9b67..9b67*/;elapsed+=pitch(v);elapsed+=6/*9b6a..9b6a*/;elapsed+=pulse(v);
     elapsed+=6/*9b6d..9b6d*/;elapsed+=8/*9c2e..9c33*/;elapsed+=1/*taken 9c33*/;elapsed+=6/*9c57..9c57*/;if((*(score.voice+(57 + v)))&16)abort();
     elapsed+=16/*9b70..9b76*/;++(*(score.voice+(63 + v)));(*(score.voice+(60 + v)))=(*(score.voice+(72 + v)));break;
    }
   }
   elapsed+=10/*9b7d..9b83*/;uint8_t t=(*(score.voice+(75 + v)))+1;
   if(t<3){elapsed+=1/*taken 9b83*/;}else{
    elapsed+=8/*9b85..9b8a*/;
    if(!((*(score.voice+(57 + v)))&8)){elapsed+=14/*9b8c..9b94*/;(*(score.voice+(57 + v)))&=127;break;}
    elapsed+=1/*taken 9b8a*/;elapsed+=2/*9b97..9b97*/;t=0;
   }
   elapsed+=43/*9b99..9bb3*/;(*(score.voice+(75 + v)))=t;t+=v*3;(*(score.voice+(69 + v)))=(*(score.voice+(0 + t)));(*(score.voice+(66 + v)))=(*(score.voice+(9 + t)));(*(score.voice+(63 + v)))=0;
  }
  elapsed+=4/*9b79..9b7a*/;if(v){elapsed+=1/*taken 9b7a*/;}
 }
 elapsed+=6/*9b7c..9b7c*/;elapsed+=6/*9d6c..9d6c*/;elapsed+=filter_tick();elapsed+=6/*9d6f..9d6f*/;elapsed+=2/*9d47..9d47*/;
 for(int8_t i=24;i>=0;i--){elapsed+=9/*9d49..9d4b*/;ScoreEmit(i,(*(score.shadow+(0 + i))),elapsed);elapsed+=4/*9d4e..9d4f*/;if(i){elapsed+=1/*taken 9d4f*/;}}
 elapsed+=6/*9d51..9d51*/;elapsed+=6/*9d72..9d72*/;
return elapsed;}
void ScoreInit(uint8_t song){uint16_t elapsed=0;
  elapsed+=9/*b527..b528*/;elapsed+=4/*9d26..9d28*/;
  for(int8_t i=24;i>=0;i--){elapsed+=9/*9d2a..9d2c*/;(*(score.shadow+(0 + i)))=0;ScoreEmit(i,0,elapsed);elapsed+=4/*9d2f..9d30*/;if(i){elapsed+=1/*taken 9d30*/;}}
  elapsed+=4/*9d32..9d34*/;
  for(uint8_t i=0;i<90;i++){elapsed+=11/*9d35..9d3b*/;if(i<89){elapsed+=1/*taken 9d3b*/;}(*(score.voice+(0 + i)))=0;}
  elapsed+=2/*9d3d..9d3d*/;
  for(uint8_t i=0;i<32;i++){elapsed+=10/*9d3f..9d44*/;if(i<31){elapsed+=1/*taken 9d44*/;}(*(score.zero+(0 + i)))=0;}
  elapsed+=6/*9d46..9d46*/;elapsed+=33/*b52b..b53c*/;
  (*(score.zero+(14)))=ScoreRead(0xb550 + song);(*(score.zero+(15)))=ScoreRead(0xb560 + song);(*(score.zero+(16)))=ScoreRead(0xb570 + song);
  elapsed+=48/*b504..b526*/;set16((score.clocks+(0)),0);set16((score.clocks+(2)),0);set16((score.clocks+(6)),20833);
  elapsed+=28/*b53f..b54f*/;(*(&score.unused[0]))=0;(*(&score.unused[1]))=0;(*(score.zero+(19)))=0;(*(score.zero+(20)))=0;(*(score.zero+(17)))=15;(*(score.zero+(18)))=15;
score_cycles=elapsed;}
void ScoreStep(void){uint16_t elapsed=0;
  elapsed+=62/*b470..b49c*/;set16((score.clocks+(0)),get16((score.clocks+(0)))+2463);set16((score.clocks+(2)),get16((score.clocks+(2)))+2463);
  uint16_t clock=get16((score.clocks+(2))),tempo=get16((score.clocks+(6)));
  if((clock>>8)<(tempo>>8)){elapsed+=1/*taken b49c*/;}else{
   elapsed+=2/*b49e..b49e*/;
   if((clock>>8)!=(tempo>>8)){elapsed+=1/*taken b49e*/;}else{elapsed+=10/*b4a0..b4a6*/;if(clock<tempo){elapsed+=1/*taken b4a6*/;}}
   if(clock>=tempo){elapsed+=6/*b4a8..b4a8*/;elapsed+=32/*b4c1..b4d4*/;set16((score.clocks+(2)),clock-tempo);elapsed+=sequence();elapsed+=6/*b4d7..b4d7*/;}
  }
  elapsed+=10/*b4ab..b4b1*/;clock=get16((score.clocks+(0)));
  if((clock>>8)<39){elapsed+=1/*taken b4b1*/;}else{
   elapsed+=2/*b4b3..b4b3*/;
   if((clock>>8)!=39){elapsed+=1/*taken b4b3*/;}else{elapsed+=10/*b4b5..b4bb*/;if(clock<10000){elapsed+=1/*taken b4bb*/;}}
   if(clock>=10000){elapsed+=6/*b4bd..b4bd*/;elapsed+=32/*b4d8..b4eb*/;set16((score.clocks+(0)),clock-10000);elapsed=effects(elapsed);elapsed+=6/*b4ee..b4ee*/;}
  }
  elapsed+=6/*b4c0..b4c0*/;

score_cycles=elapsed;}
