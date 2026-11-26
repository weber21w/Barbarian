/* The 66-byte leaf fits the unreachable tail of the auxiliary-next LUT. */
.global BarbBuildPairs
.type BarbBuildPairs,@function
/* table:r25:r24, four SRAM palette bytes:r23:r22, flags:r20.
 * Reuse each high nibble for four low nibbles; swapping both result bytes
 * implements facing without rebuilding two source indices on every pair.
 * Leaf, no saved registers or SPI. Interrupts preserve SREG's facing bit. */
BarbBuildPairs:
    bst r20,7
    movw r30,r24
    clr r24
.Lbuild_pairs_outer:
    movw r26,r22
    add r26,r24
    adc r27,r1
    ld r18,X
    swap r18
    ldi r19,0
    tst r24
    brne 1f
    ldi r19,240
1:  movw r26,r22
    clr r25
.Lbuild_pairs_inner:
    ld r20,X+
    mov r21,r18
    or r21,r20
    mov r20,r19
    tst r25
    brne 1f
    ori r20,15
1:  brtc 2f
    swap r21
    swap r20
2:  st Z+,r21
    st Z+,r20
    inc r25
    cpi r25,4
    brlo .Lbuild_pairs_inner
    inc r24
    cpi r24,4
    brlo .Lbuild_pairs_outer
    ret
.size BarbBuildPairs,.-BarbBuildPairs
