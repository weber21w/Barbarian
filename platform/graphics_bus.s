/* Graphics-only ABI: u32 address r25:r22, buffer r21:r20, u8 count r18.
 * The compositor services audio once per bounded world row, not per transfer.
 * Preserve its arguments across the exact original raster exclusion guard.
 */
#include <avr/io.h>
#include "defines.h"
.macro BARB_RASTER_GUARD
 lds r19,walk_ready
 tst r19
 breq .Lguard_done\@
.Lguard_again\@:
 lds r19,sync_phase
 tst r19
 breq .Lguard_done\@
 lds r19,sync_pulse
 cpi r19,252-FIRST_RENDER_LINE-FRAME_LINES
 brlo .Lguard_done\@
 mov r25,r27
 subi r25,lo8(-(252+2-FIRST_RENDER_LINE))
 cp r25,r19
 brsh .Lguard_again\@
.Lguard_done\@:
.endm
.macro BARB_GRAPHICS_GUARD
 mov r27,r18
 swap r27
 lsr r27
 lsr r27
 andi r27,3
 BARB_RASTER_GUARD
.endm
.section .text.BarbSyncSafe,"ax",@progbits
.global BarbSyncSafe
/* Leave 192 clocks before Timer1's next enabled compare. Compound foreground
 * counter/input updates keep interrupts enabled.
 * Check compare B too: it raises SYNC during the equalization interval.
 * No SREG changes; r18..r21 are caller-clobbered. */
BarbSyncSafe:
 lds r18,_SFR_MEM_ADDR(TCNT1L)
 lds r19,_SFR_MEM_ADDR(TCNT1H)
 subi r18,lo8(-192)
 sbci r19,hi8(-192)
 lds r20,_SFR_MEM_ADDR(OCR1AL)
 lds r21,_SFR_MEM_ADDR(OCR1AH)
 cp r18,r20
 cpc r19,r21
 brsh BarbSyncSafe
 lds r20,_SFR_MEM_ADDR(TIMSK1)
 sbrs r20,OCIE1B
 rjmp .Lsync_safe
 lds r20,_SFR_MEM_ADDR(OCR1BL)
 lds r21,_SFR_MEM_ADDR(OCR1BH)
 cp r18,r20
 cpc r19,r21
 brsh BarbSyncSafe
.Lsync_safe:
 ret
.section .text.BarbarianSpiReadBytes,"ax",@progbits
.global BarbarianSpiReadBytes
BarbarianSpiReadBytes:
 rcall BarbRefillPreserve
 rjmp BarbGraphicsRead
.section .text.BarbarianSpiWriteBytes,"ax",@progbits
.global BarbarianSpiWriteBytes
BarbarianSpiWriteBytes:
 rcall BarbRefillPreserve
 rjmp BarbGraphicsWrite
.section .text.BarbRefillPreserve,"ax",@progbits
BarbRefillPreserve:
 push r18
 push r20
 push r21
 push r22
 push r23
 push r24
 call AudioRefill
 pop r24
 pop r23
 pop r22
 pop r21
 pop r20
 pop r18
 ret
.section .text.BarbGraphicsRead,"ax",@progbits
.global BarbGraphicsRead
BarbGraphicsRead:
 BARB_GRAPHICS_GUARD
 /* The count is nonzero, so it also serves as the busy flag. */
 sts walk_busy,r18
 /* Same bank/address/pointer ABI; widen the nonzero u8 count to u16.
  * These stock assembly transfers pass the physical hardware comparison. */
 clr r19
 jmp SpiRamReadInto
.section .text.BarbGraphicsWrite,"ax",@progbits
.global BarbGraphicsWrite
BarbGraphicsWrite:
 BARB_GRAPHICS_GUARD
 sts walk_busy,r18
 clr r19
 jmp SpiRamWriteFrom
.section .text.AudioBusWait,"ax",@progbits
.global AudioBusWait
AudioBusWait:
 mov r27,r24
 swap r27
 lsr r27
 lsr r27
 andi r27,3
 lsl r25
 lsl r25
 or r27,r25
 BARB_RASTER_GUARD
 ret
