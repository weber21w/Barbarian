/* UART v4 transport for classic AVR with PA7 wired to ESP GPIO0.
 * Same state/API as un_minimal.c; compile exactly one implementation.
 * PA7 is always open drain. No interrupt masking, polling waits or extra
 * buffers. Only um_write() is a board hook in this implementation.
 * r1 remains zero; the only call into C preserves all live call-saved regs.
 * Layout is checked by un_minimal.h for every AVR C translation unit. */
#include <avr/io.h>
#ifndef UN_MINIMAL_STREAM_EXTENSIONS
#define UN_MINIMAL_STREAM_EXTENSIONS 0
#endif
#ifndef UN_MINIMAL_BRANCH_GUARDS
#define UN_MINIMAL_BRANCH_GUARDS 0
#endif
#if UN_MINIMAL_STREAM_EXTENSIONS || UN_MINIMAL_BRANCH_GUARDS
#define UM_RETRY_ZERO .Lretry_zero
#define UM_REPLY_ZERO .Lreply_zero
#else
#define UM_RETRY_ZERO .Lrequest_zero
#define UM_REPLY_ZERO .Lrx_zero
#endif
#define COUNT 26
#define FLAGS 27
#define TX 28
#define STARTED 30
#define SINCE 32
#define PHASE 34
#define READY 35
#define TAIL 36
#define RESERVE 37
#define TAG 38
#define PENDING 39
#define LENGTH 40
#define POS 41

.section .text.um_abort,"ax",@progbits
.global um_abort
.type um_abort,@function
um_abort:
    movw r30,r24
.Lfault:
    ldi r24,7
    std Z+PHASE,r24
    std Z+READY,r1
    clr r24
.Lrelease:
    cbi _SFR_IO_ADDR(DDRA),7
    cbi _SFR_IO_ADDR(PORTA),7
.Lreturn:
    ret
.size um_abort,.-um_abort

.section .text.um_start,"ax",@progbits
.global um_start
.type um_start,@function
um_start:
    movw r30,r24
    std Z+PHASE,r1             /* prevent ISR flow from reasserting PA7 */
    cbi _SFR_IO_ADDR(DDRA),7
    cbi _SFR_IO_ADDR(PORTA),7
    movw r26,r30
    ldi r24,42
1:  st X+,r1
    dec r24
    brne 1b
    std Z+STARTED,r22
    std Z+STARTED+1,r23
    std Z+SINCE,r22
    std Z+SINCE+1,r23
    mov r24,r20
    subi r24,1
    cpi r24,55
    brsh .Lfault
    add r18,r20
    brcs .Lfault
    cpi r18,254
    brsh .Lfault
    std Z+TAIL,r20
    std Z+RESERVE,r18
    ldi r24,1
    std Z+PHASE,r24
    ret
.size um_start,.-um_start

.section .text.um_flow,"ax",@progbits
.global um_flow
.type um_flow,@function
um_flow:
    movw r30,r24
    ldd r18,Z+PHASE
    subi r18,5
    cpi r18,2
    brsh .Lreturn
    ldd r18,Z+READY
    ldd r19,Z+RESERVE
    subi r18,1                 /* carry iff paused (ready was zero) */
    adc r19,r1
    cp r19,r22
    brsh 2f
    ldi r18,1
    std Z+READY,r18
.Lassert:
    cbi _SFR_IO_ADDR(PORTA),7
    sbi _SFR_IO_ADDR(DDRA),7
    ret
2:  std Z+READY,r1
    rjmp .Lrelease
.size um_flow,.-um_flow

/* Internal CRC: r24 accumulator, r22 byte; clobbers only r20/r21.
 * Both RX and TX share this loop. CRC-8/ATM, polynomial 0x07. */
.section .text.um_crc8,"ax",@progbits
.Lcrc8:
    eor r24,r22
    ldi r20,8
    ldi r21,7
1:  lsl r24
    brcc 2f
    eor r24,r21
2:  dec r20
    brne 1b
    ret
/* X -> body, r19 count; leaves X at CRC storage/end, r24 remainder. */
.Lcrc_span:
    clr r24
1:  ld r22,X+
    rcall .Lcrc8
    dec r19
    brne 1b
    ret

.section .text.um_request,"ax",@progbits
.global um_request
.type um_request,@function
um_request:
    movw r30,r24
    ldd r18,Z+PHASE
    cpi r18,6
    brne .Lrequest_zero
    ldd r18,Z+PENDING
    ldd r19,Z+LENGTH
    or r18,r19
    brne .Lrequest_zero
    cpi r20,23
    brsh .Lrequest_zero
    std Z+TX,r22
    std Z+TX+1,r23
    movw r26,r22
    ldi r18,0x30
    st X+,r18
    ldd r18,Z+TAG
    inc r18
    std Z+TAG,r18
    st X,r18
    movw r26,r22
    mov r19,r20
    subi r19,-4
    std Z+LENGTH,r19
    dec r19
    std Z+POS,r1
    ldi r18,1
    std Z+PENDING,r18
    rcall .Lcrc_span
    st X,r24
.Lrequest_success:
    ldi r24,1
    ret
.Lrequest_zero:
    clr r24
    ret
.size um_request,.-um_request

.section .text.um_snapshot,"ax",@progbits
.global um_snapshot
.type um_snapshot,@function
/* s=r24:r25, body=r22:r23, n=r20, epoch=r18:r19. */
um_snapshot:
    movw r30,r24
    ldd r24,Z+PHASE
    cpi r24,6
    brne .Lsnapshot_zero
    ldd r24,Z+PENDING
    ldd r25,Z+LENGTH
    or r24,r25
    brne .Lsnapshot_zero
    cpi r20,11
    breq 1f
    cpi r20,22
    brne .Lsnapshot_zero
1:  std Z+TX,r22
    std Z+TX+1,r23
    movw r26,r22
    ldi r24,0x31
    st X+,r24
    st X+,r18
    st X,r19
    movw r26,r22
    mov r19,r20
    subi r19,-4
    std Z+LENGTH,r19
    dec r19
    std Z+POS,r1
    rcall .Lcrc_span
    st X,r24
    ldi r24,1
    ret
.Lsnapshot_zero:
    clr r24
    ret
.size um_snapshot,.-um_snapshot

.section .text.um_datagram,"ax",@progbits
.global um_datagram
.type um_datagram,@function
um_datagram:
    movw r30,r24
    ldd r24,Z+PHASE
    cpi r24,6
    brne .Ldatagram_zero
    ldd r24,Z+PENDING
    ldd r25,Z+LENGTH
    or r24,r25
    brne .Ldatagram_zero
    cpi r20,25
    brsh .Ldatagram_zero
    movw r26,r22
    ld r24,X
    subi r24,0x32
    cpi r24,2
    brsh .Ldatagram_zero
    std Z+TX,r22
    std Z+TX+1,r23
    mov r19,r20
    subi r19,-2
    std Z+LENGTH,r19
    dec r19
    std Z+POS,r1
    rcall .Lcrc_span
    st X,r24
    ldi r24,1
    ret
.Ldatagram_zero:
    clr r24
    ret
.size um_datagram,.-um_datagram

.section .text.um_cancel,"ax",@progbits
.global um_cancel
.type um_cancel,@function
um_cancel:
    movw r30,r24
    ldd r18,Z+PHASE
    cpi r18,6
    brne .Lcancel_zero
    ldd r18,Z+LENGTH
    tst r18
    brne .Lcancel_zero
    std Z+TX,r22
    std Z+TX+1,r23
    movw r26,r22
    ldi r18,5
    st X+,r18
    ldi r18,0x1b
    st X,r18
    ldi r18,2
    std Z+LENGTH,r18
    std Z+PENDING,r18
    std Z+POS,r1
    ldi r24,1
    ret
.Lcancel_zero:
    clr r24
    ret
.size um_cancel,.-um_cancel

.section .text.um_retry,"ax",@progbits
.global um_retry
.type um_retry,@function
um_retry:
    movw r30,r24
    ldd r18,Z+PHASE
    cpi r18,6
    brne UM_RETRY_ZERO
    ldd r18,Z+PENDING
    tst r18
    breq UM_RETRY_ZERO
    ldd r18,Z+LENGTH
    tst r18
    brne UM_RETRY_ZERO
    ldd r18,Z+POS
    dec r18
    std Z+LENGTH,r18
    std Z+POS,r1
    rjmp .Lrequest_success
#if UN_MINIMAL_STREAM_EXTENSIONS || UN_MINIMAL_BRANCH_GUARDS
.Lretry_zero:
    clr r24
    ret
#endif
.size um_retry,.-um_retry

.section .text.um_poll,"ax",@progbits
.global um_poll
.type um_poll,@function
um_poll:
    movw r30,r24
    ldd r18,Z+PHASE
    cpi r18,6
    breq .Ltransmit
    tst r18
    breq .Lpoll_return
    cpi r18,6
    brsh .Lpoll_return
    movw r24,r22
    ldd r26,Z+STARTED
    ldd r27,Z+STARTED+1
    sub r24,r26
    sbc r25,r27
    subi r24,lo8(2000)
    sbci r25,hi8(2000)
    brlo 1f
    rjmp .Lfault
1:  cpi r18,3
    brlo .Ltimed
    breq .Lack_low
    cpi r18,4
    brne .Lpoll_return
    sbis _SFR_IO_ADDR(PINA),7
    ret
.Lnext_phase:
    inc r18
    std Z+PHASE,r18
    ret
.Lack_low:
    sbic _SFR_IO_ADDR(PINA),7
    ret
    rjmp .Lnext_phase
.Ltimed:
    movw r24,r22
    ldd r26,Z+SINCE
    ldd r27,Z+SINCE+1
    sub r24,r26
    sbc r25,r27
    cpi r18,2
    breq .Lpulse
    sbis _SFR_IO_ADDR(PINA),7
    rjmp .Lsince
    subi r24,100
    sbci r25,0
    brlo .Lpoll_return
    ldi r18,2
    std Z+PHASE,r18
    cbi _SFR_IO_ADDR(PORTA),7
    sbi _SFR_IO_ADDR(DDRA),7
.Lsince:
    std Z+SINCE,r22
    std Z+SINCE+1,r23
.Lpoll_return:
    ret
.Lpulse:
    sbiw r24,33
    brlo .Lpoll_return
    sbiw r24,58             /* >90 ms original elapsed: release on fault */
    brsh 1f
    ldi r18,3
    std Z+PHASE,r18
    rjmp .Lrelease
1:  rjmp .Lfault

.Ltransmit:
    push r16
    push r17
    push r28
    push r29
    movw r28,r30
    mov r17,r20
.Ltx_loop:
    tst r17
    breq .Ltx_done
    ldd r18,Y+PHASE
    cpi r18,6
    brne .Ltx_done
    ldd r18,Y+LENGTH
    tst r18
    breq .Ltx_done
    ldd r16,Y+POS
    mov r19,r16
    andi r19,127
    ldi r24,0x7e
    tst r19
    breq .Ltx_next
    cp r18,r19
    brlo .Ltx_end
    ldd r30,Y+TX
    ldd r31,Y+TX+1
    add r30,r19
    adc r31,r1
    sbiw r30,1
    ld r24,Z
    sbrc r16,7
    rjmp .Ltx_escaped
    cpi r24,0x7d
    breq .Ltx_escape
    cpi r24,0x7e
    brne .Ltx_next
.Ltx_escape:
    ori r16,128
    ldi r24,0x7d
    rjmp .Ltx_write
.Ltx_escaped:
    ldi r18,32
    eor r24,r18
.Ltx_next:
    mov r16,r19
    inc r16
    rjmp .Ltx_write
.Ltx_end:
    clr r16                 /* sentinel: retain final position for retry */
.Ltx_write:
    call um_write
    tst r24
    breq .Ltx_done
    tst r16
    breq .Ltx_clear
    std Y+POS,r16
    dec r17
    rjmp .Ltx_loop
.Ltx_clear:
    std Y+LENGTH,r1
.Ltx_done:
    pop r29
    pop r28
    pop r17
    pop r16
    ret
.size um_poll,.-um_poll

.section .text.um_byte,"ax",@progbits
.global um_byte
.type um_byte,@function
um_byte:
    movw r30,r24
    ldd r18,Z+PHASE
    subi r18,5
    cpi r18,2
    brsh .Lrx_zero
    ldd r18,Z+COUNT
    ldd r19,Z+FLAGS
    cpi r22,0x7e
    breq .Lrx_end
    sbrs r19,0
    rjmp .Lrx_zero
    sbrc r19,2
    rjmp .Lrx_zero
    sbrs r19,1
    rjmp .Lrx_plain
    andi r19,~2
    cpi r22,0x5d
    breq 2f
    cpi r22,0x5e
    brne .Lrx_bad
2:  ldi r24,32
    eor r22,r24
    rjmp .Lrx_store
.Lrx_plain:
    cpi r22,0x7d
    brne .Lrx_store
    ori r19,2
    rjmp .Lrx_flags
.Lrx_store:
    cpi r18,26
    breq .Lrx_bad
    movw r26,r30
    add r26,r18
    adc r27,r1
    st X,r22
    inc r18
    std Z+COUNT,r18
    rjmp .Lrx_flags
.Lrx_bad:
    ori r19,4
.Lrx_flags:
    std Z+FLAGS,r19
.Lrx_zero:
    clr r24
    ret
.Lrx_end:
    ldi r24,1
    std Z+FLAGS,r24
    std Z+COUNT,r1
    cpi r19,1
    brne .Lrx_zero
    cpi r18,2
    brlo .Lrx_zero
    movw r26,r30
    mov r19,r18
    rcall .Lcrc_span
    tst r24
    brne .Lrx_zero
    dec r18                 /* length excluding CRC */
    ld r19,Z
    cpi r19,0x80
#if UN_MINIMAL_STREAM_EXTENSIONS
    brne .Lnot_entered
    rjmp .Lentered
.Lnot_entered:
#else
    breq .Lentered
#endif
    ldd r24,Z+PHASE
    cpi r24,6
    brne .Lrx_zero
    cpi r19,0x85
    brne .Lnot_cancel_reply
    cpi r18,2
    brne .Lrx_zero
    ldd r24,Z+PENDING
    cpi r24,2
    brne .Lrx_zero
    ldd r24,Z+1
    cpi r24,3
    brsh .Lrx_zero
    cpi r24,1
    brne .Lrx_result
    std Z+PENDING,r1
    std Z+TAG,r1
    rjmp .Lrx_result
.Lnot_cancel_reply:
#if UN_MINIMAL_STREAM_EXTENSIONS
    cpi r19,0xb3
    brne .Lnot_input_event
    cpi r18,3
    breq .Lrx_result
    cpi r18,11
    brsh .Lrx_result
    rjmp .Lrx_zero
.Lnot_input_event:
    cpi r19,0xb4
    brne .Lnot_timed_event
    cpi r18,3
    breq .Lrx_result
    cpi r18,4
    breq .Lrx_result
    cpi r18,20
    breq .Lrx_result
    rjmp .Lrx_zero
.Lnot_timed_event:
#endif
    cpi r19,0xb0
    brne 2f
    cpi r18,5
    brsh .Lrx_result
2:  cpi r19,0xb2
    brne 3f
    cpi r18,3
    breq .Lrx_result
    cpi r18,4
    breq .Lrx_result
    cpi r18,14
    breq .Lrx_result
    cpi r18,25
    breq .Lrx_result
    rjmp .Lrx_zero
3:  cpi r19,0xb1
    brne UM_REPLY_ZERO
    cpi r18,3
    brlo UM_REPLY_ZERO
    ldd r24,Z+PENDING
    cpi r24,1
    brne UM_REPLY_ZERO
    ldd r24,Z+TAG
    ldd r19,Z+1
    cp r24,r19
    brne UM_REPLY_ZERO
    std Z+PENDING,r1
.Lrx_result:
    mov r24,r18
    ret
#if UN_MINIMAL_STREAM_EXTENSIONS || UN_MINIMAL_BRANCH_GUARDS
.Lreply_zero:
    rjmp .Lrx_zero
#endif
.Lentered:
    ldd r24,Z+PHASE
    cpi r24,5
    brne .Lentry_fault
    cpi r18,11
    brne .Lentry_fault
    ldd r24,Z+1
    cpi r24,'U'
    brne .Lentry_fault
    ldd r24,Z+2
    cpi r24,'N'
    brne .Lentry_fault
    ldd r24,Z+3
    cpi r24,4
    brne .Lentry_fault
    ldd r24,Z+4
    cpi r24,2
    brne .Lentry_fault
    ldd r24,Z+5
    andi r24,0xe5
    cpi r24,0xe5
    brne .Lentry_fault
    ldd r24,Z+9
    subi r24,1
    cpi r24,54
    brsh .Lentry_fault
    subi r24,-2
    ldd r19,Z+8
    cp r19,r24
    brne .Lentry_fault
    ldd r24,Z+TAIL
    cp r24,r19
    brlo .Lentry_fault
    ldd r24,Z+10
    cpi r24,40
    brlo .Lentry_fault
    ldi r24,6
    std Z+PHASE,r24
    rjmp .Lrx_zero
.Lentry_fault:
    rjmp .Lfault
.size um_byte,.-um_byte
