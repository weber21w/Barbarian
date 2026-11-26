/* Isolated resident score backend. Caller must own the foreground music
 * producer/reentry guard, and enter with the SPI bus released. Uses the existing
 * interrupt-enabled kernel SPI primitive; no SD or graphics scratch access.
 * Source addresses are build-audited against the fixed three-song score. */
#include <stdint.h>
#ifdef SCORE_IO_HOST
#define PROGMEM
#define BARB_SCORE_SPI_BASE 122640UL
#define pgm_read_word(p) (*(p))
#else
#include <avr/pgmspace.h>
#include "data/platform-layout.h"
#endif
extern volatile uint8_t walk_busy;
extern void AudioBusWait(uint16_t);
extern void SpiRamReadInto(uint8_t,uint16_t,void*,uint16_t);
#include "data/resident-score-map.inc"
static uint16_t dense(uint16_t address){
 uint8_t lo=0,hi=BARB_SCORE_RANGE_COUNT;
 while(lo<hi){uint8_t mid=(lo+hi)>>1;if(address<pgm_read_word(starts+mid))hi=mid;else lo=mid+1;}
 return address-pgm_read_word(biases+lo-1);
}
void ScoreReadBlock(uint16_t address,uint8_t *dest,uint8_t bytes){
 uint16_t offset=dense(address);
 AudioBusWait(bytes);walk_busy=1;
 SpiRamReadInto(BARB_SCORE_SPI_BASE>>16,(uint16_t)BARB_SCORE_SPI_BASE+offset,dest,bytes);
 walk_busy=0;
}
uint8_t ScoreRead(uint16_t address){uint8_t b;ScoreReadBlock(address,&b,1);return b;}
uint16_t ScoreReadWord(uint16_t address){uint8_t b[2];ScoreReadBlock(address,b,2);return b[0]|(uint16_t)b[1]<<8;}
