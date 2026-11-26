#ifndef UN_CANCEL_H
#define UN_CANCEL_H
#include "un_minimal.h"
/* One byte per game adapter, retained outside its resettable session storage.
 * A successful cancel keeps ESP PA7 mode and baud for the next join. The AVR
 * parser is parked until um_reconnect initializes the new local session. */
static uint8_t um_session_reusable;
static inline void um_reconnect(UnMinimal *s,uint16_t now,uint8_t tail,uint8_t hardware){
    uint8_t reuse=um_session_reusable;um_session_reusable=0;
    um_start(s,now,tail,hardware);
    if(reuse && tail && tail<=55 && (unsigned)tail+hardware<=253)s->phase=UM_READY;
}
/* Stop-time bounded pump; no extra persistent RX/TX buffers. Expressions refer
 * to the game's existing UART rings and millisecond clock. Finish a partial
 * encoded request/retry before borrowing the two-byte cancel buffer. */
#define UM_CANCEL_WAIT(serial,free_bytes,read_byte,millis,wait_frame) \
__extension__ ({ \
    UnMinimal *um_cs=(serial);uint8_t um_body[2],um_result=0,um_frames=0; \
    um_session_reusable=0; \
    while(um_cs->phase==UM_READY && um_frames++<210u){ \
        um_flow(um_cs,(free_bytes));um_poll(um_cs,(millis),32); \
        for(uint8_t um_budget=0;um_budget<64;um_budget++){ \
            int16_t um_value=(read_byte);if(um_value<0)break; \
            uint8_t um_n=um_byte(um_cs,(uint8_t)um_value); \
            if(um_n==2)um_result=um_cs->rx.data[1]; \
        } \
        if(um_result==2 || (um_result==1 && !um_cs->length))break; \
        if(um_result!=1 && !um_cs->length && (um_frames&7u)==1u)um_cancel(um_cs,um_body); \
        wait_frame; \
    } \
    um_flow(um_cs,0); \
    um_session_reusable=(um_result==1 && !um_cs->length); \
    um_abort(um_cs); \
    um_session_reusable; \
})
#ifdef UN_UART_AVR_H
static inline int16_t um_cancel_uart_read(UnUart *u){
    if(u->rx_head==u->rx_tail)return -1;
    uint8_t b=u->rx[u->rx_tail&(UM_UART_RX_SIZE-1)];u->rx_tail=(u->rx_tail+1)&(UM_UART_RX_SIZE-1);return b;
}
#define UM_CANCEL_UART(u) UM_CANCEL_WAIT(&(u)->serial, \
    (uint8_t)(((u)->rx_tail-(u)->rx_head-1)&(UM_UART_RX_SIZE-1)), \
    um_cancel_uart_read(u),(uint16_t)(GetVsyncCounter()*17u),WaitVsync(1))
#endif
#define UM_CANCEL_KERNEL(s,capacity) UM_CANCEL_WAIT(s, \
    (uint8_t)((capacity)-UartUnreadCount()),UartReadChar(), \
    (uint16_t)(GetVsyncCounter()*17u),WaitVsync(1))
#endif
