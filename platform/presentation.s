/* Presentation-only helpers, AVR-GCC ABI. Preserve the preceding AVR C's
 * signed 16-bit wrapping/truncation, smoothing thresholds and clipping.
 * Leaf routines: no stack, SPI, interrupt masking or gameplay state changes.
 * Host C implementations remain the readable reference. */
#include "compositor_layout.inc"

.section .text.GridScreenX,"ax",@progbits
.global GridScreenX
.type GridScreenX,@function
GridScreenX:
 sbiw r24,6
 .rept 4
 lsl r24
 rol r25
 .endr
 bst r25,7
 sbrs r25,7
 rjmp .Lgrid_magnitude
 com r24
 com r25
 adiw r24,1
.Lgrid_magnitude:
 /* floor(unsigned magnitude / 3) = (magnitude * 0xaaab) >> 17.
  * Keep only product bytes 1..3; all carries into the high word remain exact. */
 clr r19
 clr r21
 ldi r22,0xab
 mul r24,r22
 mov r18,r1
 mul r25,r22
 add r18,r0
 mov r20,r1
 adc r20,r19
 ldi r22,0xaa
 mul r24,r22
 add r18,r0
 adc r20,r1
 adc r21,r19
 mul r25,r22
 add r20,r0
 adc r21,r1
 clr r1
 movw r24,r20
 lsr r25
 ror r24
 brtc .Lgrid_done
 com r24
 com r25
 adiw r24,1
.Lgrid_done:
 ret
.size GridScreenX,.-GridScreenX

.section .text.SmoothAxis,"ax",@progbits
.global SmoothAxis
.type SmoothAxis,@function
SmoothAxis:
 movw r26,r24
 .rept 4
 lsl r22
 rol r23
 .endr
 lds r24,barbMotionValid
 tst r24
 brne 1f
 rjmp .Lmotion_snap
1:
 ldi r19,2
 rjmp .Lmotion_difference
.size SmoothAxis,.-SmoothAxis

.section .text.SmoothWalkX,"ax",@progbits
.global SmoothWalkX
.type SmoothWalkX,@function
SmoothWalkX:
 /* group = ((u8)(state-4))/5, exactly over all 256 byte values. */
 mov r21,r20
 subi r21,4
 ldi r25,205
 mul r21,r25
 mov r21,r1
 clr r1
 lsr r21
 lsr r21
 ldi r30,lo8(barbMotionGroup)
 ldi r31,hi8(barbMotionGroup)
 add r30,r24
 adc r31,r1
 lds r25,barbMotionValid
 tst r25
 breq 1f
 cpi r21,4
 brsh 2f
1:
 st Z,r21
2:
 ldi r26,lo8(barbMotionPosition)
 ldi r27,hi8(barbMotionPosition)
 lsl r24
 add r26,r24
 adc r27,r1
 .rept 4
 lsl r22
 rol r23
 .endr
 tst r25
 breq .Lmotion_snap
 cpi r20,78
 brge .Lmotion_snap
 ldi r24,32
 cpi r21,4
 brsh 1f
 ldi r24,12
1:
 /* Victory has no canonical translation: discard pending walk drift. */
 subi r20,64
 cpi r20,2
 brsh 1f
 clr r24
1:
 movw r20,r18
 mov r18,r24
 ldi r19,1
.Lmotion_difference:
 ld r24,X+
 ld r25,X
 sbiw r26,1
 movw r30,r22
 sub r30,r24
 sbc r31,r25
 /* (u16)(difference + threshold) <= 2*threshold; threshold is 256/512. */
 mov r0,r31
 add r0,r19
 lsl r19
 cp r0,r19
 brlo .Lmotion_step
 brne .Lmotion_snap
 tst r30
 brne .Lmotion_snap
.Lmotion_step:
 mov r19,r21
 mul r18,r20
 movw r20,r0
 mul r18,r19
 add r21,r0
 clr r1
 cp r20,r30
 cpc r21,r31
 brge 1f
 movw r30,r20
1:
 clr r18
 clr r19
 sub r18,r20
 sbc r19,r21
 cp r30,r18
 cpc r31,r19
 brge 1f
 movw r30,r18
1:
 add r24,r30
 adc r25,r31
 rjmp .Lmotion_store
.Lmotion_snap:
 movw r24,r22
.Lmotion_store:
 st X+,r24
 st X,r25
 .rept 4
 asr r25
 ror r24
 .endr
 ret
.size SmoothWalkX,.-SmoothWalkX

.section .text.BlitSprite,"ax",@progbits
.global BlitSprite
.type BlitSprite,@function
BlitSprite:
 lds r19,nextSprite
 cpi r19,4
 brsh .Lblit_done
 cpi r20,BARB_SPR_COUNT
 brsh .Lblit_done
 adiw r24,26
#if BARB_BG_CROP_TOP
 subi r22,lo8(BARB_BG_CROP_TOP)
 sbci r23,hi8(BARB_BG_CROP_TOP)
#endif
 ldi r30,lo8(barbAsmSpriteWidths)
 ldi r31,hi8(barbAsmSpriteWidths)
 add r30,r20
 adc r31,r1
 lpm r0,Z
 movw r26,r24
 add r26,r0
 adc r27,r1
 cpi r26,lo8(-11)
 ldi r21,hi8(-11)
 cpc r27,r21
 brlt .Lblit_done
 cpi r24,226
 cpc r25,r1
 brge .Lblit_done
 subi r30,lo8(-(BARB_SPR_COUNT))
 sbci r31,hi8(-(BARB_SPR_COUNT))
 lpm r0,Z
 movw r26,r22
 add r26,r0
 adc r27,r1
 cp r1,r26
 cpc r1,r27
 brge .Lblit_done
 /* BARB_VIEW_H is unsigned in the retained C: negative y is rejected too. */
 cpi r22,128
 cpc r23,r1
 brsh .Lblit_done
 inc r19
 sts nextSprite,r19
 dec r19
 ldi r21,6
 mul r19,r21
 movw r30,r0
 clr r1
 subi r30,lo8(-(spr))
 sbci r31,hi8(-(spr))
 st Z+,r24
 st Z+,r25
 st Z+,r22
 st Z+,r23
 st Z+,r20
 st Z,r18
.Lblit_done:
 ret
.size BlitSprite,.-BlitSprite
