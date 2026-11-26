/* Sword-hit callback, AVR-GCC ABI: state pointer r24, victim 0/1 in r22.
 * Death state 80/81 immediately zeros life and selects that player's HUD slot.
 * Nonfatal staining and kick/headbutt exclusion retain the C oracle exactly.
 * Leaf: no stack, SPI or interrupt masking. */
#include "compositor_layout.inc"
.section .text.BladeHit,"ax",@progbits
.global BladeHit
.type BladeHit,@function
BladeHit:
    movw r30,r24
    sub r30,r22
    sbc r31,r1
    ldd r18,Z+BARB_STATE_JOUEURA
    cpi r18,72
    breq .Lhit_done
    cpi r18,74
    breq .Lhit_done
    /* State A/B are reversed in the packed state; blood/life are forward. */
    add r30,r22
    adc r31,r1
    add r30,r22
    adc r31,r1
    ldi r26,lo8(barbBladeLevel+1)
    ldi r27,hi8(barbBladeLevel+1)
    sub r26,r22
    sbc r27,r1
    subi r18,80
    cpi r18,2
    brlo .Lhit_fatal
    ldd r18,Z+BARB_STATE_SANGA
    tst r18
    breq .Lhit_done
    ld r18,X
    cpi r18,3
    brsh .Lhit_done
    inc r18
    st X,r18
.Lhit_done:
    ret
.Lhit_fatal:
    subi r30,lo8(-BARB_STATE_VIEA)
    sbci r31,hi8(-BARB_STATE_VIEA)
    st Z,r1
    mov r18,r22
    lsl r18
    sts barbHudPhase,r18
    ldi r18,3
    st X,r18
    ret
.size BladeHit,.-BladeHit
