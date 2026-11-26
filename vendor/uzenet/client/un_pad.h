#ifndef UN_PAD_H
#define UN_PAD_H
#include "un_minimal.h"
/* Optional three-byte policy wrapper. The raw um_* API remains available for
 * chat, prediction/replay and games choosing explicit WAIT polling.
 * This wrapper uses deferred replies and lockstep only. Never acknowledges
 * RUN until the application explicitly submits its next sample. */
enum { UMP_OFF,UMP_ENTER,UMP_CONNECT,UMP_WAIT,UMP_ROOM,UMP_RUN,UMP_FAILED };
typedef struct {uint8_t phase,local_bytes,all_bytes;} UnPad;
/* setup: 25 writable bytes, START arguments at offset 3, retained until ROOM
 * AND serial.length==0. No copying or permanent setup allocation. */
void ump_begin(UnMinimal *s,UnPad *p,uint8_t setup[25],uint16_t now,
               uint8_t tail,uint8_t hardware);
void ump_poll(UnMinimal *s,UnPad *p,uint16_t now,uint8_t budget);
/* Pass each nonzero um_byte result here. Do not consume further RX while
 * ROOM/RUN is held; rx.data contains the seed/masks or the granted pad data. */
void ump_accept(UnMinimal *s,UnPad *p,uint8_t n);
/* Call after processing ROOM, or completing exactly one RUN simulation step.
 * body needs local_bytes+4 writable bytes (<=12). Sample is copied now.
 * Returns zero without acknowledging anything if TX is still finishing. */
uint8_t ump_exchange(UnMinimal *s,UnPad *p,uint8_t *body,const uint8_t *pads);
#endif
