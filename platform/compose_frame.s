/* Frame compositor orchestration, AVR-GCC ABI. The C implementation remains
 * the host oracle. Row preparation, sorting/merging, painting and transfers
 * retain their public ABIs and exact call order. Use the same GCC shared
 * stack prologue as the old C, with 42 local bytes instead of 53.
 * Local layout (Y-relative): rows 1..20, left 21..25, right 26..30,
 * cursors 31..38, dirty 39..42.
 * Persistent: changed r2; flags r3 (scroll/bloodDirty/fullBlood bits 0/1/2);
 * last r4; y r5; left r7:r6; right r9:r8; buffer r10; count r12;
 * dirty r15:r14; cursors r17:r16. No extra interrupt exclusions. */
#include "compositor_layout.inc"
#include "hud.h"
.section .text.Compose,"ax",@progbits
.global Compose
.type Compose,@function
Compose:
 ldi r26,42
 clr r27
 ldi r30,lo8(pm(.Lframe_enter))
 ldi r31,hi8(pm(.Lframe_enter))
 jmp __prologue_saves__
.Lframe_enter:
 clr r10
 lds r18,barbAsmBack
 lds r19,barbAsmBack+1
 or r18,r19
 breq 1f
 inc r10
1:
 lds r18,nextSprite
 cpi r18,5
 brlo 1f
 ldi r18,4
 sts nextSprite,r18
1:
 clr r3
 ldi r30,lo8(barbAsmComposeEnding)
 ldi r31,hi8(barbAsmComposeEnding)
 add r30,r10
 adc r31,r1
 ld r18,Z
 lds r19,barbAsmEndingWanted
 cp r18,r19
 breq 1f
 inc r3
1:
 call BarbRetainFront
 tst r24
 breq 1f
 clr r24
 rjmp .Lframe_return
1:
 movw r16,r28
 subi r16,lo8(-31)
 sbci r17,hi8(-31)
 movw r30,r16
 ldi r18,8
1:
 st Z+,r1
 dec r18
 brne 1b
 movw r30,r28
 subi r30,lo8(-39)
 sbci r31,hi8(-39)
 movw r14,r30
 clr r20
 lds r18,nextSprite
 cpi r18,4
 brlo .Lframe_swap_ready
 lds r30,spr+22
 clr r31
 lsl r30
 rol r31
 subi r30,lo8(-(barbAsmSpriteBytes))
 sbci r31,hi8(-(barbAsmSpriteBytes))
 lpm r18,Z+
 lpm r19,Z
 andi r19,1
 cpi r18,BARB_SMALL_SPRITE_CACHE_BYTES+1
 cpc r19,r1
 brlo .Lframe_swap_ready
 inc r20
.Lframe_swap_ready:
 lds r18,barbAsmSmallSwap
 cp r18,r20
 breq 1f
 sts barbAsmSmallSwap,r20
 ldi r18,255
 sts barbAsmCachedPose+2,r18
 sts barbAsmCachedPose+3,r18
1:
 ldi r30,lo8(barbAsmCount)
 ldi r31,hi8(barbAsmCount)
 add r30,r10
 adc r31,r1
 ld r12,Z
 lds r18,nextSprite
 cp r12,r18
 brsh 1f
 mov r12,r18
1:
 ldi r30,lo8(barbAsmBloodDrawn)
 ldi r31,hi8(barbAsmBloodDrawn)
 add r30,r10
 adc r31,r1
 ld r18,Z
 lds r19,barbAsmBloodVersion
 cp r18,r19
 breq 1f
 ldi r18,2
 or r3,r18
1:
 mov r24,r10
 movw r22,r14
 mov r20,r12
 call BarbComposeActors
 mov r2,r24
 ldi r30,lo8(barbAsmComposeCamera)
 ldi r31,hi8(barbAsmComposeCamera)
 add r30,r10
 adc r31,r1
 lds r18,barbAsmCamera
 st Z,r18
 tst r2
 brne .Lframe_changed
 tst r3
 brne .Lframe_changed
 rjmp .Lframe_success
.Lframe_changed:
 call BladeBegin
 sbrs r3,1
 rjmp .Lframe_range
 lds r18,barbAsmBloodVictim
 tst r18
 brne .Lframe_full_blood
 ldi r30,lo8(barbAsmBloodDrawnFrame)
 ldi r31,hi8(barbAsmBloodDrawnFrame)
 add r30,r10
 adc r31,r1
 ld r18,Z
 cpi r18,255
 breq .Lframe_range
.Lframe_full_blood:
 ldi r18,4
 or r3,r18
.Lframe_range:
 clr r5
 ldi r18,128
 mov r4,r18
 sbrc r3,0
 rjmp .Lframe_rows_setup
 mov r24,r10
 call BarbRowRange
 sbrs r3,2
 rjmp .Lframe_range_set
 cpi r24,25
 brlo 1f
 ldi r24,24
1:
 cpi r25,118
 brsh .Lframe_range_set
 ldi r25,118
.Lframe_range_set:
 mov r5,r24
 mov r4,r25
.Lframe_rows_setup:
 movw r6,r28
 ldi r18,21
 add r6,r18
 adc r7,r1
 movw r8,r6
 ldi r18,5
 add r8,r18
 adc r9,r1
.Lframe_row:
 cp r5,r4
 brlo 1f
 rjmp .Lframe_remember
1:
 /* Count/buffer/dirty/cursor arguments remain in their ABI registers. */
 mov r24,r5
 movw r22,r28
 subi r22,lo8(-1)
 sbci r23,hi8(-1)
 movw r20,r6
 movw r18,r8
 call BarbPrepareRows
 lds r18,barbAsmEndingWanted
 tst r18
 brne 1f
 ldi r18,BARB_HUD_ROWS
 cp r5,r18
 brlo .Lframe_next
1:
 mov r18,r5
 andi r18,3
 brne 1f
 call AudioRefill
1:
 sbrc r3,0
 rjmp .Lframe_full_row
 sbrs r3,2
 rjmp .Lframe_partial
 mov r18,r5
 subi r18,24
 cpi r18,94
 brlo .Lframe_full_row
 rjmp .Lframe_spans
.Lframe_partial:
 sbrs r3,1
 rjmp .Lframe_spans
 mov r24,r5
 mov r22,r10
 call BarbBloodRow
 movw r30,r6
 add r30,r12
 adc r31,r1
 st Z,r24
 movw r30,r8
 add r30,r12
 adc r31,r1
 st Z,r25
 mov r22,r12
 inc r22
 rjmp .Lframe_paint_spans
.Lframe_spans:
 mov r22,r12
 tst r22
 breq .Lframe_next
.Lframe_paint_spans:
 mov r24,r5
 movw r20,r6
 movw r18,r8
 movw r16,r28
 subi r16,lo8(-1)
 sbci r17,hi8(-1)
 call BarbComposeSpans
 /* Span dispatch's row pointer overlaps PrepareRows' cursor argument. */
 subi r16,lo8(-30)
 sbci r17,hi8(-30)
 rjmp .Lframe_next
.Lframe_full_row:
 mov r24,r5
 clr r22
 ldi r20,BARB_FB_ROW_BYTES
 movw r18,r28
 subi r18,lo8(-1)
 sbci r19,hi8(-1)
 call ComposeSpan
.Lframe_next:
 inc r5
 rjmp .Lframe_row
.Lframe_remember:
 sbrs r2,1
 rjmp .Lframe_history
 mov r24,r10
 call BarbRememberBounds
.Lframe_history:
 ldi r26,lo8(barbAsmHistory)
 ldi r27,hi8(barbAsmHistory)
 tst r10
 breq 1f
 adiw r26,24
1:
 ldi r30,lo8(spr)
 ldi r31,hi8(spr)
 lds r18,nextSprite
 ldi r19,6
 mul r18,r19
 mov r19,r0
 clr r1
 tst r19
 breq 2f
1:
 ld r0,Z+
 st X+,r0
 dec r19
 brne 1b
2:
 ldi r30,lo8(barbAsmCount)
 ldi r31,hi8(barbAsmCount)
 add r30,r10
 adc r31,r1
 st Z,r18
 ldi r26,lo8(barbAsmBloodBounds)
 ldi r27,hi8(barbAsmBloodBounds)
 tst r10
 breq 1f
 adiw r26,4
1:
 ldi r30,lo8(barbAsmBloodCurrent)
 ldi r31,hi8(barbAsmBloodCurrent)
 ldi r18,4
1:
 ld r0,Z+
 st X+,r0
 dec r18
 brne 1b
.Lframe_success:
 ldi r24,1
.Lframe_return:
 adiw r28,42
 ldi r30,18
 jmp __epilogue_restores__
.size Compose,.-Compose
