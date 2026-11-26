/* Four source phases, with the source pointer advanced once per byte.
 * Private ABI: dst r24, source r22, table low byte r20, config r18:r19.
 * The table page is fixed by barbAsmComposePairs; all four tables fit it.
 * Y is clobbered
 * and returns the last pair's source-byte cursor. ComposeSpan saves Y once
 * around the whole span and reloads the source only for a trailing pixel.
 * T holds direction. Ready takes count 1..127 with T pre-established;
 * Raw retains the packed direction/count ABI. No C callers may use either.
 * Table must be 32-byte aligned. Y retains the source and ZH the table page;
 * the alignment guarantees that entry+mask never carries into another page.
 * Interrupts stay enabled; the raster/audio ISR preserves SREG, including T. */
.section .text.BarbBlendPairsRaw,"ax",@progbits
.global BarbBlendPairsRaw
.global BarbBlendPairsReady
.macro BLEND id
    /* Zero pairs use short out-of-line handlers, so the non-final exit
     * can fall directly into its phase continuation. */
    breq .Ltransparent\id
    lsl r19
    mov r30,r20
    or r30,r19
    ld r19,Z+
    ld r18,Z
    cpse r18,r1
    rcall .Lpartial_pair
    st X+,r19
.Lblend_step\id:
    dec r23
    breq .Lpair_done
.endm
BarbBlendPairsRaw:
    bst r19,7
    andi r19,127
/* Count r19 is 1..127, without a direction bit. ComposeSpan establishes T
 * while preparing its source phase, so its production entry skips the two
 * packed-config instructions. Raw retains the original private ABI. */
BarbBlendPairsReady:
    movw r26,r24
    movw r28,r22
    ldi r31,hi8(barbAsmComposePairs)
    mov r23,r19
    mov r19,r18
    andi r19,3
    lsr r18
    lsr r18
    add r28,r18
    adc r29,r1
    tst r19
    breq .Lphase0
    cpi r19,2
    breq .Lphase2
    cpi r19,1
    breq .Lphase1
    rjmp .Lphase3
.Lphase0:
    ld r22,Y
.Lphase0_cached:
    mov r19,r22
    swap r19
    andi r19,15
    BLEND 0
    brtc .Lphase2_cached
    sbiw r28,1
    rjmp .Lphase2
.Lphase2:
    ld r22,Y
.Lphase2_cached:
    mov r19,r22
    andi r19,15
    BLEND 2
    brts .Lphase0_cached
    adiw r28,1
    rjmp .Lphase0
/* Center these handlers so all four conditional branches stay in range.
 * Returning and transparent advancement are shared, while every continuing
 * pair falls straight through to its own source-phase transition. */
.Lpair_done:
    ret
.Ltransparent0:
    adiw r26,1
    rjmp .Lblend_step0
.Ltransparent2:
    adiw r26,1
    rjmp .Lblend_step2
.Ltransparent1:
    adiw r26,1
    rjmp .Lblend_step1
.Ltransparent3:
    adiw r26,1
    rjmp .Lblend_step3
.Lphase1:
    ld r19,Y
    lsr r19
    lsr r19
    andi r19,15
    BLEND 1
    brtc .Lphase3
    sbiw r28,1
    rjmp .Lphase3
.Lphase3:
    ld r19,Y
    ldd r18,Y+1
    /* Shift the two adjacent source bytes together. The low nibble is
     * exactly the pair straddling their boundary, in either direction. */
    lsl r18
    rol r19
    lsl r18
    rol r19
    andi r19,15
    BLEND 3
    brts 2f
    adiw r28,1
2:  rjmp .Lphase1
.Lpartial_pair:
    ld r30,X
    and r30,r18
    or r19,r30
    ret
