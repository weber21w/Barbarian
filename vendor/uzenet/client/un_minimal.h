#ifndef UN_MINIMAL_H
#define UN_MINIMAL_H
#include "un_wire.h"
#ifndef UN_MINIMAL_STREAM_EXTENSIONS
#define UN_MINIMAL_STREAM_EXTENSIONS 0
#endif
/* ESP service opcodes; no network headers needed on the AVR. */
#define UM_REQUEST 0x30
#define UM_REPLY 0xb1
enum { UM_START=1,UM_POLL,UM_EXCHANGE,UM_CHAT_BEGIN,UM_CHAT_WRITE,
       UM_CHAT_READ,UM_CHAT_NEXT,UM_CHAT_STATUS,UM_CREDIT,UM_REPLAY_READ,UM_REPLAY_NEXT,
       UM_SERVICE_INFO,UM_DELAY_POLICY,UM_NET_STATUS,UM_PEER_STATUS,
       UM_NAME_SET,UM_NAME_GET,UM_CHECK,UM_CHECK_STATUS,UM_PROFILE_SET,UM_PROFILE_GET,
       UM_BARRIER,UM_END,UM_LOBBY,UM_SNAPSHOT,UM_JOIN,UM_SETTLE,
       UM_NAME_FORMAT,UM_NAME_MAP,UM_DISPLAY_NAME,UM_COMPACT_STATUS,
       UM_INPUT_SEND,UM_INPUT_READ,UM_LIVE,UM_LAST_FAILURE,UM_BURST,UM_BURST_ACK,
       UM_SNAPSHOT_STREAM,UM_PERFORMANCE,UM_INPUT_STREAM,UM_CAPTURE_TIME, UM_SCORES };
enum { UM_BURST_REPLAY=1,UM_BURST_INPUT,UM_BURST_LIVE,UM_BURST_SNAPSHOT };
#define UM_NAME_ASCII 0
#define UM_NAME_UPPERCASE 1
#define UM_NAME_CUSTOM_MAP 2
enum { UM_STATUS_IDLE=0,UM_STATUS_CONNECTING,UM_STATUS_BINDING,
       UM_STATUS_WAIT_PLAYERS,UM_STATUS_WAIT_READY,UM_STATUS_SYNC,
       UM_STATUS_RUNNING,UM_STATUS_WAIT_INPUT,UM_STATUS_RECONNECTING,
       UM_STATUS_ENDING,UM_STATUS_FAILED };
#define UM_JOINING 8
#define UM_OK 0x80
#define UM_OOB 0x40
#define UM_ERROR 0x10
#define UM_REPAIR 0x20
#define UM_DEFER 0x40 /* START width flag; ESP holds WAIT replies. */
#define UM_AUTO_DELAY 0x80 /* START delay byte: this bit plus maximum 0..8. */
enum {UM_IDLE,UM_HIGH,UM_PULSE,UM_LOW_ACK,UM_HIGH_ACK,UM_ENTRY,UM_READY,UM_FAULT};
typedef struct {
    UnRx rx;
    uint8_t *tx;
    uint16_t started,since;
    volatile uint8_t phase,ready;
    uint8_t tail,reserve,tag,pending,length,pos;
} UnMinimal;
#ifdef __AVR__
#include <stddef.h>
/* Fail the build rather than silently using stale assembly offsets. */
typedef char um_avr_layout_check[(sizeof(UnMinimal)==42 &&
    offsetof(UnMinimal,rx.count)==26 && offsetof(UnMinimal,rx.flags)==27 &&
    offsetof(UnMinimal,tx)==28 && offsetof(UnMinimal,started)==30 &&
    offsetof(UnMinimal,since)==32 && offsetof(UnMinimal,phase)==34 &&
    offsetof(UnMinimal,ready)==35 && offsetof(UnMinimal,tail)==36 &&
    offsetof(UnMinimal,reserve)==37 && offsetof(UnMinimal,tag)==38 &&
    offsetof(UnMinimal,pending)==39 && offsetof(UnMinimal,length)==40 &&
    offsetof(UnMinimal,pos)==41)?1:-1];
typedef char um_avr_protocol_check[(UN_PROTOCOL_VERSION==4 && UN_BODY_MAX==26 &&
    UN_DELIM==0x7e && UN_ESCAPE==0x7d && UM_REQUEST==0x30 &&
    UM_REPLY==0xb1 && UN_FRAME==0xb0 && UM_IDLE==0 && UM_HIGH==1 &&
    UM_PULSE==2 && UM_LOW_ACK==3 && UM_HIGH_ACK==4 && UM_ENTRY==5 &&
    UM_READY==6 && UM_FAULT==7)?1:-1];
#endif
/* Board hooks never block. drive(0) releases PA7 WITHOUT a pull-up.
 * un_minimal_avr.S binds PA7 directly and needs only um_write(). */
void um_drive(uint8_t low);
uint8_t um_pin(void);
uint8_t um_write(uint8_t byte);
void um_start(UnMinimal *s,uint16_t now,uint8_t tail,uint8_t hardware);
void um_flow(UnMinimal *s,uint8_t free);
void um_poll(UnMinimal *s,uint16_t now,uint8_t budget);
/* Caller supplies n+4 writable bytes. Put opcode at body[2], arguments at
 * body[3]. n is argument count (0..22). Borrow until response AND TX complete.
 * Request buffer is prepared once; retries merely rewind its cursor. */
uint8_t um_request(UnMinimal *s,uint8_t *body,uint8_t n);
/* After enabling UM_SNAPSHOT_STREAM: body[3..] holds 11/22 compact state
 * bytes. Borrow n+4 bytes only until TX finishes. No pending reply or tag. */
uint8_t um_snapshot(UnMinimal *s,uint8_t *body,uint8_t n,uint16_t epoch);
/* Optional input/timed-state upload. body[0] is UN_INPUT_WRITE or
 * UN_TIMED_WRITE; body[1..n] is payload. Borrow n+2 bytes until TX finishes. */
uint8_t um_datagram(UnMinimal *s,uint8_t *body,uint8_t n);
uint8_t um_retry(UnMinimal *s);
uint8_t um_cancel(UnMinimal *s,uint8_t body[2]);
/* Positive = validated response length; payload status is rx.data[2].
 * A new request is refused until an in-flight retry finishes encoding. */
uint8_t um_byte(UnMinimal *s,uint8_t byte);
void um_abort(UnMinimal *s);
#endif
