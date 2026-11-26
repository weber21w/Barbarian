/* Damaged world-row spans, AVR-GCC ABI. Interrupts remain enabled.
 * ComposeRow is {left,right,data_lo,data_hi,sourceLeft}, BarbSprite is six
 * bytes. The caller services audio once per row; every SPI operation retains the
 * graphics bus raster guard. C implementations remain the host oracle.
 */
#include <avr/io.h>
#include "compositor_layout.inc"
#include "hud.h"

.section .text.Present,"ax",@progbits
.global Present
.type Present,@function
/* Latch a linear framebuffer and camera in the existing raster exclusion
 * window. The scanout derives each row in registers; no address table or
 * long busy interval. The three-byte update is bounded by the same guard. */
Present:
    call AudioRefill
    ldi r24,3
    clr r25
    call AudioBusWait
    ldi r18,1
    sts walk_busy,r18
    lds r24,barbAsmFront
    lds r25,barbAsmFront+1
    lds r22,barbAsmBack
    lds r23,barbAsmBack+1
    sts barbAsmFront,r22
    sts barbAsmFront+1,r23
    sts barbAsmBack,r24
    sts barbAsmBack+1,r25
    subi r22,lo8(-13)
    sbci r23,hi8(-13)
    sts barbScanBase,r22
    sts barbScanBase+1,r23
    lds r18,barbAsmCamera
    sts barbScanCamera,r18
    sts walk_busy,r1
    lds r24,barbPresentations
    lds r25,barbPresentations+1
    adiw r24,1
    sts barbPresentations,r24
    sts barbPresentations+1,r25
    ret
.size Present,.-Present

.section .text.BarbRetainFront,"ax",@progbits
.global BarbRetainFront
.type BarbRetainFront,@function
/* Leaf equality check; histories contain at most four six-byte actors. */
BarbRetainFront:
    clr r18
    lds r19,barbAsmFront+1
    cpse r19,r1
    ldi r18,1
    ldi r30,lo8(barbAsmComposeCamera)
    ldi r31,hi8(barbAsmComposeCamera)
    add r30,r18
    adc r31,r1
    ld r19,Z
    lds r20,barbAsmCamera
    cp r19,r20
    brne .Lretain_no
    ldi r30,lo8(barbAsmBloodDrawn)
    ldi r31,hi8(barbAsmBloodDrawn)
    add r30,r18
    adc r31,r1
    ld r19,Z
    lds r20,barbAsmBloodVersion
    cp r19,r20
    brne .Lretain_no
    ldi r30,lo8(barbAsmComposeEnding)
    ldi r31,hi8(barbAsmComposeEnding)
    add r30,r18
    adc r31,r1
    ld r19,Z
    lds r20,barbAsmEndingWanted
    cp r19,r20
    brne .Lretain_no
    ldi r30,lo8(barbAsmCount)
    ldi r31,hi8(barbAsmCount)
    add r30,r18
    adc r31,r1
    ld r19,Z
    lds r20,nextSprite
    cp r19,r20
    brne .Lretain_no
    ldi r21,24
    mul r18,r21
    movw r30,r0
    clr r1
    subi r30,lo8(-(barbAsmHistory))
    sbci r31,hi8(-(barbAsmHistory))
    ldi r21,6
    mul r20,r21
    mov r20,r0
    clr r1
    ldi r26,lo8(spr)
    ldi r27,hi8(spr)
    tst r20
    breq .Lretain_yes
1:  ld r18,X+
    ld r19,Z+
    cp r18,r19
    brne .Lretain_no
    dec r20
    brne 1b
.Lretain_yes:
    lds r24,barbRetained
    lds r25,barbRetained+1
    adiw r24,1
    sts barbRetained,r24
    sts barbRetained+1,r25
    ldi r24,1
    ret
.Lretain_no:
    clr r24
    ret
.size BarbRetainFront,.-BarbRetainFront

.section .text.BladeBegin,"ax",@progbits
.global BladeBegin
.type BladeBegin,@function
/* The pose cache tail holds two five-byte blade cursors. */
BladeBegin:
    lds r21,nextSprite
    clr r20
    ldi r26,lo8(workBuf+BARB_SPRITE_CACHE_BYTES)
    ldi r27,hi8(workBuf+BARB_SPRITE_CACHE_BYTES)
    ldi r22,lo8(workBuf)
    ldi r23,hi8(workBuf)
1:  cp r20,r21
    brsh 4f
    cpi r20,2
    brsh 4f
    ldi r30,lo8(spr+4)
    ldi r31,hi8(spr+4)
    tst r20
    breq 2f
    adiw r30,6
2:  ld r18,Z
    ldi r30,lo8(barbAsmSpriteBytes)
    ldi r31,hi8(barbAsmSpriteBytes)
    lsl r18
    add r30,r18
    adc r31,r1
    lpm r18,Z+
    lpm r19,Z
    andi r19,1
    add r22,r18
    adc r23,r19
    movw r30,r22
    sbiw r30,1
    ld r18,Z
    sbiw r30,2
    ld r19,Z
    sub r30,r18
    sbc r31,r1
    st X+,r30
    st X+,r31
    st X+,r18
    st X+,r19
    st X+,r1
    inc r20
    ldi r22,lo8(barbAsmPoseB)
    ldi r23,hi8(barbAsmPoseB)
    rjmp 1b
4:  ret
.size BladeBegin,.-BladeBegin

.section .text.BarbCachePose,"ax",@progbits
.global BarbCachePose
.type BarbCachePose,@function
/* i:r24, pose:r22. Palette work uses the ten-byte free cache tail only
 * after this load finishes. Retain bounded transfers and audio refill. */
BarbCachePose:
    ldi r30,lo8(barbAsmCachedPose)
    ldi r31,hi8(barbAsmCachedPose)
    add r30,r24
    adc r31,r1
    ld r18,Z
    cp r18,r22
    brne 1f
    ret
1:  push r7
    push r8
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    push r16
    push r17
    push r28
    push r29
    mov r10,r24
    mov r11,r22
    /* Stored offsets live with resident runtime data; decoded sizes
     * stay in flash because row/blade consumers retain their old cache ABI. */
    mov r22,r11
    lsl r22
    clr r23
    subi r22,lo8(-(BARB_RUNTIME_BASE+BARB_POSE_DIRECTORY_OFFSET))
    sbci r23,hi8(-(BARB_RUNTIME_BASE+BARB_POSE_DIRECTORY_OFFSET))
    clr r24
    clr r25
    ldi r20,lo8(barbAsmLineBuf)
    ldi r21,hi8(barbAsmLineBuf)
    ldi r18,2
    call BarbarianSpiReadBytes
    lds r14,barbAsmLineBuf
    lds r15,barbAsmLineBuf+1
    mov r30,r11
    clr r31
    lsl r30
    rol r31
    subi r30,lo8(-(barbAsmSpriteBytes))
    sbci r31,hi8(-(barbAsmSpriteBytes))
    lpm r12,Z+
    lpm r18,Z
    andi r18,1
    mov r13,r18
    mov r24,r10
    ldi r28,lo8(workBuf)
    ldi r29,hi8(workBuf)
    tst r24
    breq 4f
    ldi r28,lo8(barbAsmPoseB)
    ldi r29,hi8(barbAsmPoseB)
    cpi r24,1
    breq 4f
    lds r18,barbAsmSmallSwap
    cpi r24,3
    brne 3f
    ldi r19,1
    eor r18,r19
3:  ldi r28,lo8(barbAsmPoseLarge)
    ldi r29,hi8(barbAsmPoseLarge)
    tst r18
    breq 4f
    ldi r28,lo8(barbAsmPoseSmall)
    ldi r29,hi8(barbAsmPoseSmall)
4:  clr r16
    ldi r18,lo8(BARB_SPR_BANK0_BYTES)
    ldi r19,hi8(BARB_SPR_BANK0_BYTES)
    cp r14,r18
    cpc r15,r19
    brsh 5f
    ldi r18,lo8(BARB_SPIVRAM_SPRITES)
    ldi r19,hi8(BARB_SPIVRAM_SPRITES)
    rjmp 6f
5:  ldi r18,lo8(BARB_SPR_BANK1_BASE-BARB_SPR_BANK0_BYTES)
    ldi r19,hi8(BARB_SPR_BANK1_BASE-BARB_SPR_BANK0_BYTES)
    ldi r16,hh8(BARB_SPR_BANK1_BASE-BARB_SPR_BANK0_BYTES)
6:  add r14,r18
    adc r15,r19
    adc r16,r1
    rjmp .Lcache_decode
.Lcache_done:
    ldi r30,lo8(barbAsmCachedPose)
    ldi r31,hi8(barbAsmCachedPose)
    add r30,r10
    adc r31,r1
    st Z,r11
    pop r29
    pop r28
    pop r17
    pop r16
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r8
    pop r7
    ret
.size BarbCachePose,.-BarbCachePose

/* Cache loading precedes span pixel work. The line buffer is scratch here,
 * including across guarded reads; audio refill never uses that buffer. */
.section .text.BarbPoseDecode,"ax",@progbits
.Lcache_decode:
    ldi r18,32
    mov r8,r18
 .Lcache_token:
    call BarbPoseByte
    mov r17,r24
    sbrc r24,7
    rjmp .Lcache_match
    inc r17
    mov r7,r17
.Lcache_literals:
    /* Copy directly within each prefetched run instead of calling the byte
     * reader for every literal. Refill preserves the literal count. */
    ldi r18,32
    cp r8,r18
    brne 1f
    call BarbPoseRefill
1:  ldi r30,lo8(barbAsmLineBuf)
    ldi r31,hi8(barbAsmLineBuf)
    add r30,r8
    adc r31,r1
2:  ld r18,Z+
    st Y+,r18
    inc r8
    dec r7
    breq .Lcache_token_done
    ldi r18,32
    cp r8,r18
    brne 2b
    rjmp .Lcache_literals
.Lcache_match:
    call BarbPoseByte
    clr r18
    sbrc r17,6
    inc r18
    movw r26,r28
    sub r26,r24
    sbc r27,r18
    sbiw r26,1
    andi r17,63
    subi r17,-3
    mov r24,r17
1:  ld r18,X+
    st Y+,r18
    dec r24
    brne 1b
.Lcache_token_done:
    sub r12,r17
    sbc r13,r1
    mov r18,r12
    or r18,r13
    breq 2f
    rjmp .Lcache_token
2:  jmp .Lcache_done

.section .text.BarbPoseRefill,"ax",@progbits
BarbPoseRefill:
    movw r22,r14
    mov r24,r16
    clr r25
    ldi r20,lo8(barbAsmLineBuf)
    ldi r21,hi8(barbAsmLineBuf)
    ldi r18,32
    call BarbarianSpiReadBytes
    ldi r18,32
    add r14,r18
    adc r15,r1
    /* A final prefetch may wrap, but no stored pose crosses a bank. */
    clr r8
    ret

/* Keep calls close to the decoder so the linker can relax them. */
.section .text.BarbPoseByte,"ax",@progbits
.global BarbPoseByte
.type BarbPoseByte,@function
BarbPoseByte:
    ldi r18,32
    cp r8,r18
    brne 1f
    call BarbPoseRefill
1:  ldi r30,lo8(barbAsmLineBuf)
    ldi r31,hi8(barbAsmLineBuf)
    add r30,r8
    adc r31,r1
    ld r24,Z
    inc r8
    ret
.size BarbPoseByte,.-BarbPoseByte

.section .text.BarbComposeActors,"ax",@progbits
.global BarbComposeActors
.type BarbComposeActors,@function
/* buffer:r24, dirty:r22, count:r20; return OR of dirty flags. The retained
 * C loop is the host oracle. Once per actor, compare histories, load a pose
 * and build its palette. No per-row frame/pose decisions are repeated. */
BarbComposeActors:
    /* Resolve the selected history bank and each fighter's validity once.
     * Row preparation consumes a null pointer for an unavailable history. */
    ldi r30,lo8(barbAsmBoundsValid)
    ldi r31,hi8(barbAsmBoundsValid)
    add r30,r24
    adc r31,r1
    ld r18,Z
    ldi r26,lo8(barbAsmRowBounds)
    ldi r27,hi8(barbAsmRowBounds)
    tst r24
    breq 1f
    subi r26,lo8(-160)
    sbci r27,hi8(-160)
1:  ldi r30,lo8(barbAsmOldBounds)
    ldi r31,hi8(barbAsmOldBounds)
    ldi r19,2
2:  sbrs r18,0
    rjmp 3f
    st Z+,r26
    st Z+,r27
    rjmp 4f
3:  st Z+,r1
    st Z+,r1
4:  subi r26,lo8(-80)
    sbci r27,hi8(-80)
    lsr r18
    dec r19
    brne 2b
    push r2
    push r3
    push r4
    push r5
    push r6
    push r7
    push r8
    push r9
    push r10
    push r11
    push r12
    push r28
    push r29
    clr r2
    mov r3,r20
    movw r4,r22
    clr r8
    lds r11,nextSprite
    ldi r30,lo8(barbAsmCount)
    ldi r31,hi8(barbAsmCount)
    add r30,r24
    adc r31,r1
    ld r12,Z
    ldi r18,24
    mul r24,r18
    movw r6,r0
    clr r1
    ldi r18,lo8(barbAsmHistory)
    ldi r19,hi8(barbAsmHistory)
    add r6,r18
    adc r7,r19
    ldi r28,lo8(spr)
    ldi r29,hi8(spr)
.Lactors_loop:
    cp r2,r3
    brlo 1f
    rjmp .Lactors_done
1:  ldd r10,Y+4
    ldd r9,Y+5
    ldi r24,3
    cp r2,r11
    brsh .Lactors_dirty
    cp r2,r12
    brsh .Lactors_dirty
    movw r30,r6
    ldd r18,Z+4
    cp r18,r10
    brne .Lactors_dirty
    ldd r18,Z+2
    ldd r19,Z+3
    ldd r20,Y+2
    ldd r21,Y+3
    cp r18,r20
    cpc r19,r21
    brne .Lactors_dirty
    ldi r24,1
    ld r18,Z
    ldd r19,Z+1
    ld r20,Y
    ldd r21,Y+1
    cp r18,r20
    cpc r19,r21
    brne .Lactors_dirty
    ldd r18,Z+5
    cp r18,r9
    brne .Lactors_dirty
    clr r24
.Lactors_dirty:
    movw r26,r4
    st X+,r24
    movw r4,r26
    or r8,r24
    cp r2,r11
    brlo 1f
    rjmp .Lactors_next
1:  ldi r30,lo8(barbAsmCachedPose)
    ldi r31,hi8(barbAsmCachedPose)
    add r30,r2
    adc r31,r1
    ld r18,Z
    ldi r30,lo8(barbAsmCachedFlags)
    ldi r31,hi8(barbAsmCachedFlags)
    add r30,r2
    adc r31,r1
    ld r19,Z
    mov r20,r9
    andi r20,129
    cp r18,r10
    brne 1f
    cp r19,r20
    brne 1f
    rjmp .Lactors_next
1:  st Z,r20
    mov r24,r2
    mov r22,r10
    call BarbCachePose
    mov r30,r10
    clr r31
    lsl r30
    rol r31
    subi r30,lo8(-(barbAsmSpriteBytes+1))
    sbci r31,hi8(-(barbAsmSpriteBytes+1))
    lpm r18,Z
    lsr r18
    mov r19,r18
    lsl r18
    add r18,r19
    ldi r30,lo8(barbAsmSpritePalettes)
    ldi r31,hi8(barbAsmSpritePalettes)
    add r30,r18
    adc r31,r1
    ldi r26,lo8(workBuf+BARB_SPRITE_CACHE_BYTES)
    ldi r27,hi8(workBuf+BARB_SPRITE_CACHE_BYTES)
    st X+,r1
    ldi r20,3
1:  lpm r18,Z+
    sbrs r9,0
    rjmp 3f
    cpi r18,1
    brne 2f
    ldi r18,6
    rjmp 3f
2:  cpi r18,2
    brne 3f
    ldi r18,9
3:  st X+,r18
    dec r20
    brne 1b
    mov r24,r2
    ldi r25,32
    mul r24,r25
    movw r24,r0
    clr r1
    subi r24,lo8(-(barbAsmComposePairs))
    sbci r25,hi8(-(barbAsmComposePairs))
    ldi r22,lo8(workBuf+BARB_SPRITE_CACHE_BYTES)
    ldi r23,hi8(workBuf+BARB_SPRITE_CACHE_BYTES)
    mov r20,r9
    call BarbBuildPairs
.Lactors_next:
    adiw r28,6
    ldi r18,6
    add r6,r18
    adc r7,r1
    inc r2
    rjmp .Lactors_loop
.Lactors_done:
    mov r24,r8
    pop r29
    pop r28
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop r7
    pop r6
    pop r5
    pop r4
    pop r3
    pop r2
    ret
.size BarbComposeActors,.-BarbComposeActors

.section .text.BarbRememberBounds,"ax",@progbits
.global BarbRememberBounds
.type BarbRememberBounds,@function
/* buffer:r24. Commit only after all old bounds have been consumed, so vertical
 * movement cannot overwrite a source row still needed later in this frame.
 * Two-pixel left/right margins clamp outward at 30; width <=60 makes 0xff
 * an unambiguous empty row. All original source and blade pixels are covered. */
BarbRememberBounds:
    push r28
    push r29
    ldi r18,160
    mul r24,r18
    movw r26,r0
    clr r1
    subi r26,lo8(-(barbAsmRowBounds))
    sbci r27,hi8(-(barbAsmRowBounds))
    ldi r30,lo8(barbAsmBoundsValid)
    ldi r31,hi8(barbAsmBoundsValid)
    add r30,r24
    adc r31,r1
    lds r23,nextSprite
    mov r18,r23
    cpi r18,2
    brlo 1f
    ldi r18,3
1:  st Z,r18
    clr r22
    ldi r28,lo8(workBuf)
    ldi r29,hi8(workBuf)
.Lremember_actor:
    cp r22,r23
    brsh .Lremember_done
    cpi r22,2
    brsh .Lremember_done
    ldi r30,lo8(spr+4)
    ldi r31,hi8(spr+4)
    tst r22
    breq 1f
    adiw r30,6
1:  ld r18,Z
    ldi r30,lo8(barbAsmSpriteHeights)
    ldi r31,hi8(barbAsmSpriteHeights)
    add r30,r18
    adc r31,r1
    lpm r25,Z
    /* The build checks that widths immediately precede heights. */
    subi r30,lo8(BARB_SPR_COUNT)
    sbci r31,hi8(BARB_SPR_COUNT)
    lpm r0,Z
    clr r24
.Lremember_row:
    ldi r18,255
    cp r24,r25
    brsh .Lremember_store
    ld r18,Y+
    ld r19,Y+
    cpi r18,255
    breq .Lremember_store
    mov r20,r19
    sub r20,r18
    subi r20,lo8(-3)
    lsr r20
    lsr r20
    add r28,r20
    adc r29,r1
    lsr r18
    sbrc r18,4
    ldi r18,15
    swap r18
    mov r20,r0
    sub r20,r19
    lsr r20
    sbrc r20,4
    ldi r20,15
    or r18,r20
.Lremember_store:
    st X+,r18
    inc r24
    cpi r24,80
    brlo .Lremember_row
    inc r22
    ldi r28,lo8(barbAsmPoseB)
    ldi r29,hi8(barbAsmPoseB)
    rjmp .Lremember_actor
.Lremember_done:
    pop r29
    pop r28
    ret
.size BarbRememberBounds,.-BarbRememberBounds

.section .text.DrawHealth,"ax",@progbits
.global DrawHealth
.type DrawHealth,@function
/* Packed clamped life:r24, elapsed-field/heavy flag:r22. Phase is presentation-local: P1, rest, P2, rest.
 * No pending change exits without saving registers or touching SPI. */
DrawHealth:
    lds r18,barbHudPhase
    mov r19,r18
    inc r19
    sts barbHudPhase,r19
    andi r18,3
    lds r19,barbHudShown
    cpi r19,255
    brne .Lhud_active
    rjmp .Lhud_init
.Lhud_active:
    sbrc r18,0
.Lhud_return:
    ret
1:  tst r18
    breq 1f
    swap r24
    swap r19
1:  mov r20,r24
    andi r20,15
    mov r21,r19
    andi r21,15
    cp r20,r21
    breq .Lhud_return
    mov r23,r21
    /* Empty gauges are urgent. Otherwise retry this eligible slot at most
     * twice when foreground work has already consumed a video field. */
    tst r22
    breq .Lhud_update
    tst r20
    breq .Lhud_update
    lds r21,barbHudDeferred
    cpi r21,2
    brsh .Lhud_update
    inc r21
    sts barbHudDeferred,r21
    lds r21,barbHudPhase
    dec r21
    sts barbHudPhase,r21
    ret
.Lhud_update:
1:  andi r19,240
    or r19,r20
    tst r18
    breq 1f
    swap r19
1:  sts barbHudShown,r19
    /* Only the segment between old/new gauge ends changed. */
    lsl r20
    lsl r23
    mov r22,r20
    sub r22,r23
    ldi r19,BARB_HUD_BLOOD
    brsh 1f
    neg r22
    ldi r19,BARB_HUD_STEEL
1:  mov r21,r20
    cp r20,r23
    brlo 1f
    mov r21,r23
1:  ldi r24,15
    tst r18
    breq 1f
    /* Mirrored P2: 91 - max(old,new), rather than 15 + min. */
    add r20,r23
    sub r20,r21
    ldi r24,91
    sub r24,r20
    rjmp 2f
1:  add r24,r21
2:  ldi r26,lo8(barbAsmLineBuf)
    ldi r27,hi8(barbAsmLineBuf)
    mov r18,r19
    mov r19,r22
    rcall .Lhud_fill
    rjmp .Lhud_commit
.Lhud_init:
    sts barbHudShown,r24
    mov r23,r24
    mov r20,r24
    andi r20,15
    lsl r20
    ldi r21,2
    ldi r22,80
    ldi r24,13
.Lhud_buffer:
    ldi r26,lo8(barbAsmLineBuf)
    ldi r27,hi8(barbAsmLineBuf)
    ldi r18,BARB_HUD_STEEL
    mov r19,r22
    rcall .Lhud_fill
    ldi r26,lo8(barbAsmLineBuf)
    ldi r27,hi8(barbAsmLineBuf)
    add r26,r21
    adc r27,r1
    ldi r18,BARB_HUD_BLOOD
    mov r19,r20
    rcall .Lhud_fill
    swap r23
    andi r23,15
    lsl r23
    ldi r21,78
    sub r21,r23
    ldi r26,lo8(barbAsmLineBuf)
    ldi r27,hi8(barbAsmLineBuf)
    add r26,r21
    adc r27,r1
    mov r19,r23
    rcall .Lhud_fill
    /* Only the two 24-byte gauges have steel behind them. Black the two
     * outer margins and the centre gap once, in the initial shared row. */
    ldi r26,lo8(barbAsmLineBuf)
    ldi r27,hi8(barbAsmLineBuf)
    st X+,r1
    st X,r1
    adiw r26,25
    clr r18
    ldi r19,28
    rcall .Lhud_fill
    adiw r26,24
    st X+,r1
    st X,r1
.Lhud_commit:
    sts barbHudDeferred,r1
    push r24
    push r22
    call AudioRefill
    pop r22
    pop r24
    jmp BarbPaintHud
.Lhud_fill:
    tst r19
    breq 2f
1:  st X+,r18
    dec r19
    brne 1b
2:  ret
.size DrawHealth,.-DrawHealth

.section .text.BarbPaintHud,"ax",@progbits
.global BarbPaintHud
.type BarbPaintHud,@function
/* column:r24, count:r22. Both fixed framebuffers retain identical HUD pixels.
 * Caller refills audio once; each short write retains the raster exclusion. */
BarbPaintHud:
    push r16
    push r17
    push r28
    push r29
    mov r16,r22
    ldi r17,BARB_HUD_ROWS
    mov r28,r24
    clr r29
1:  movw r22,r28
    rcall .Lhud_write
    movw r22,r28
    subi r22,lo8(-(BARB_FB_ROW_BYTES*128))
    sbci r23,hi8(-(BARB_FB_ROW_BYTES*128))
    rcall .Lhud_write
    subi r28,lo8(-BARB_FB_ROW_BYTES)
    sbci r29,hi8(-BARB_FB_ROW_BYTES)
    dec r17
    brne 1b
    pop r29
    pop r28
    pop r17
    pop r16
    ret
.Lhud_write:
    clr r24
    clr r25
    ldi r20,lo8(barbAsmLineBuf)
    ldi r21,hi8(barbAsmLineBuf)
    mov r18,r16
    jmp BarbGraphicsWrite
.size BarbPaintHud,.-BarbPaintHud

.section .text.BarbBloodRow,"ax",@progbits
.global BarbBloodRow
.type BarbBloodRow,@function
/* y:r24, buffer:r22; return byte bounds r24:r25. No SPI or saved registers. */
BarbBloodRow:
    mov r18,r24
    lsl r22
    lsl r22
    ldi r26,lo8(barbAsmBloodBounds)
    ldi r27,hi8(barbAsmBloodBounds)
    add r26,r22
    adc r27,r1
    ldi r30,lo8(barbAsmBloodCurrent)
    ldi r31,hi8(barbAsmBloodCurrent)
    ldi r24,107
    clr r25
    rcall .Lblood_box
    movw r30,r26
.Lblood_box:
    ld r19,Z+
    ld r20,Z+
    ld r21,Z+
    ld r22,Z
    cp r18,r21
    brlo 1f
    cp r18,r22
    brsh 1f
    cp r19,r24
    brsh 2f
    mov r24,r19
2:  cp r25,r20
    brsh 1f
    mov r25,r20
1:  ret
.size BarbBloodRow,.-BarbBloodRow

.section .text.BarbComposeSpans,"ax",@progbits
.global BarbComposeSpans
.type BarbComposeSpans,@function
/* y:r24, count:r22 (0..5), left:r20, right:r18, rows:r16.
 * Preserve the C selection-sort order and six-byte merge threshold, including
 * empty damage entries. No SPI here: ComposeSpan retains every bus guard.
 * Across calls: y:r2, remaining:r3, lo/hi:r4/r5, right cursor:r6,
 * pending lo/hi:r8/r9, rows:r16 (unchanged argument), left cursor:Y. */
BarbComposeSpans:
    tst r22
    brne 1f
    ret
1:  push r2
    push r3
    push r4
    push r5
    push r6
    push r7
    push r8
    push r9
    push r28
    push r29
    mov r2,r24
    mov r3,r22
    movw r6,r20
    movw r8,r18
    movw r26,r6
    movw r28,r8
    mov r24,r3
    dec r24
    breq .Lspans_sorted
    /* Two fighters normally already have left-to-right damage order. */
    cpi r24,1
    brne 1f
    ld r18,X+
    ld r19,X
    cp r19,r18
    brsh .Lspans_sorted
    sbiw r26,1
1:  clr r18
.Lspans_sort_i:
    ld r20,X
    ld r21,Y
    mov r19,r18
    inc r19
.Lspans_sort_j:
    movw r30,r6
    add r30,r19
    adc r31,r1
    ld r22,Z
    cp r22,r20
    brsh .Lspans_sort_next
    st Z,r20
    movw r30,r8
    add r30,r19
    adc r31,r1
    ld r23,Z
    st Z,r21
    mov r20,r22
    mov r21,r23
    st X,r20
    st Y,r21
.Lspans_sort_next:
    inc r19
    cp r19,r3
    brlo .Lspans_sort_j
    adiw r26,1
    adiw r28,1
    inc r18
    cp r18,r24
    brlo .Lspans_sort_i
.Lspans_sorted:
    movw r28,r6
    movw r30,r8
    ld r4,Y+
    ld r5,Z+
    movw r6,r30
    dec r3
    breq .Lspans_last
.Lspans_merge:
    ld r18,Y+
    movw r30,r6
    ld r19,Z+
    movw r6,r30
    cp r18,r19
    brsh .Lspans_merge_next
    mov r20,r5
    subi r20,lo8(-6)
    cp r20,r18
    brlo .Lspans_disjoint
    cp r5,r19
    brsh .Lspans_merge_next
    mov r5,r19
    rjmp .Lspans_merge_next
.Lspans_disjoint:
    mov r8,r18
    mov r9,r19
    cp r4,r5
    brsh 1f
    rcall .Lspans_emit
1:  mov r4,r8
    mov r5,r9
.Lspans_merge_next:
    dec r3
    brne .Lspans_merge
.Lspans_last:
    cp r4,r5
    brsh .Lspans_done
    rcall .Lspans_emit
.Lspans_done:
    pop r29
    pop r28
    rjmp .Lcompose_restore9
.Lspans_emit:
    mov r24,r2
    mov r22,r4
    mov r20,r5
    movw r18,r16
    jmp ComposeSpan
.size BarbComposeSpans,.-BarbComposeSpans

.section .text.ComposeSpan,"ax",@progbits
.global ComposeSpan
.type ComposeSpan,@function
/* y:r24, start:r22, end:r20, rows:r19:r18. Persistent registers:
 * r2 y, r3 start, r4 length, r5/r6 clip, r7 actor index,
 * r9:r8 row, r17 pair LUT low byte, r14 sx, r15 lo,
 * r16 hi, T facing, Y source. All four LUTs fit the build-checked SRAM page.
 * Row byte 4 packs the source offset at
 * its clipped left edge (bits 0..5) and horizontal flip (bit 7).
 */
ComposeSpan:
    push r2
    push r3
    push r4
    push r5
    push r6
    push r7
    push r8
    push r9
    push r14
    push r15
    push r16
    push r17
    push r28
    push r29
    mov r2,r24
    mov r3,r22
    mov r4,r20
    sub r4,r22
    mov r5,r22
    lsl r5
    mov r6,r20
    lsl r6
    movw r8,r18
    /* All packed column edges are in [24,39] and [176,191].
     * Interior spans cannot intersect either column; avoid a guarded SPI
     * transaction for the one-byte edge mask on each such span. */
    mov r18,r5
    cpi r18,39
    brlo .Lspan_foreground
    mov r18,r6
    cpi r18,177
    brlo .Lspan_background
.Lspan_foreground:
    mov r24,r2
    call BarbForegroundEdges
    cp r5,r24
    brsh 1f
    mov r5,r24
1:  cp r25,r6
    brsh .Lspan_background
    mov r6,r25
.Lspan_background:
    /* The row address has one consumer now. Inline it and multiply the
     * saved y directly: no helper return address or redundant register move. */
    ldi r23,BARB_FB_ROW_BYTES
    mul r2,r23
    movw r22,r0
    clr r1
    add r22,r3
    adc r23,r1
    /* Keep the clean-background offset for the matching framebuffer write.
     * SPI and actor painting own all scratch registers in between. */
    push r22
    push r23
    subi r22,lo8(-BARB_SPIVRAM_BG_CLEAN)
    sbci r23,hi8(-BARB_SPIVRAM_BG_CLEAN)
    clr r24
    clr r25
    ldi r20,lo8(barbAsmLineBuf)
    ldi r21,hi8(barbAsmLineBuf)
    mov r18,r4
    call BarbGraphicsRead
    ldi r17,lo8(barbAsmComposePairs)
    clr r7
.Lspan_actor:
    lds r18,nextSprite
    cp r7,r18
    brlo 1f
    rjmp .Lspan_write
1:  movw r30,r8
    ld r18,Z+
    mov r15,r18
    ld r16,Z+
    cp r15,r5
    brsh 1f
    mov r15,r5
1:  cp r6,r16
    brsh 1f
    mov r16,r6
1:  cp r15,r16
    brlo 1f
    rjmp .Lspan_next
.Lspan_no_pairs:
    /* LSR left C set exactly when one unpaired pixel remains. */
    brcs .Lspan_tail_single
    rjmp .Lspan_painted
1:  ld r28,Z+
    ld r29,Z+
    ld r14,Z
    sub r18,r15
    bst r14,7
    mov r19,r14
    andi r19,63
    brts 2f
    neg r18
2:
    add r19,r18
    mov r14,r19
    sbrc r15,0
    rcall .Lspan_single
    mov r19,r16
    sub r19,r15
    lsr r19
    breq .Lspan_no_pairs
    mov r18,r14
    brtc 2f
    dec r18
2:
    mov r20,r17
    movw r22,r28
    mov r24,r15
    lsr r24
    sub r24,r3
    clr r25
    subi r24,lo8(-(barbAsmLineBuf))
    sbci r25,hi8(-(barbAsmLineBuf))
    call BarbBlendPairsReady
.Lspan_after_pairs:
    /* Leading singles leave lo even. An even exclusive edge has no tail;
     * neither lo nor source phase is needed again before the next actor. */
    sbrs r16,0
    rjmp .Lspan_painted
    mov r18,r16
    sub r18,r15
    andi r18,254
    add r15,r18
    brtc 1f
    neg r18
1:  add r14,r18
.Lspan_tail_single:
    /* The pair kernel owns Y. Only an unpaired final pixel needs the original
     * source-row base again; all complete pairs avoid per-call Y saves. */
    movw r30,r8
    ldd r28,Z+2
    ldd r29,Z+3
    rcall .Lspan_single
.Lspan_painted:
    mov r24,r7
    mov r22,r2
    mov r20,r3
    mov r18,r5
    mov r16,r6
    call BarbBladeRow
.Lspan_next:
    ldi r18,5
    add r8,r18
    adc r9,r1
    subi r17,lo8(-32)
    inc r7
    rjmp .Lspan_actor
.Lspan_write:
    pop r23
    pop r22
    lds r18,barbAsmBack
    lds r19,barbAsmBack+1
    add r22,r18
    adc r23,r19
    clr r24
    clr r25
    ldi r20,lo8(barbAsmLineBuf)
    ldi r21,hi8(barbAsmLineBuf)
    mov r18,r4
    call BarbGraphicsWrite
#ifdef BARB_DEBUG_COMPOSE_ROWS
    lds r24,barbComposeRows
    lds r25,barbComposeRows+1
    adiw r24,1
    sts barbComposeRows,r24
    sts barbComposeRows+1,r25
#endif
.Lspan_restore:
    pop r29
    pop r28
    pop r17
    pop r16
    pop r15
    pop r14
    rjmp .Lcompose_restore9
.Lcompose_restore:
    pop r29
    pop r28
    pop r17
    pop r16
.Lcompose_restore15:
    pop r15
    pop r14
    pop r13
    pop r12
.Lcompose_restore11:
    pop r11
    pop r10
.Lcompose_restore9:
    pop r9
    pop r8
    pop r7
    pop r6
    pop r5
    pop r4
    pop r3
    pop r2
    ret
/* One odd leading or trailing pixel. Source, position and flip are retained
 * in callee-saved registers; all scratch is call-clobbered. */
.Lspan_single:
    mov r18,r14
    lsr r18
    lsr r18
    movw r30,r28
    add r30,r18
    adc r31,r1
    ld r19,Z
    /* Packed 2bpp phases: select the nibble, then its high/low pair.
     * SWAP avoids shifting through all three preceding pixel pairs. */
    sbrs r14,1
    swap r19
    sbrc r14,0
    rjmp 2f
    lsr r19
    lsr r19
2:  andi r19,3
    breq .Lsingle_done
    ldi r18,10
    mul r19,r18
    /* code <=3 keeps r1 zero; this entry also stays inside the LUT page. */
    mov r30,r17
    add r30,r0
    ldi r31,hi8(barbAsmComposePairs)
    ld r19,Z
    andi r19,15
    mov r26,r15
    lsr r26
    sub r26,r3
    clr r27
    subi r26,lo8(-(barbAsmLineBuf))
    sbci r27,hi8(-(barbAsmLineBuf))
    ld r18,X
    sbrs r15,0
    rjmp 1f
    andi r18,240
    rjmp 2f
1:  andi r18,15
    swap r19
2:  or r18,r19
    st X,r18
.Lsingle_done:
    brtc 1f
    dec r14
    rjmp 2f
1:  inc r14
2:  inc r15
    ret
.size ComposeSpan,.-ComposeSpan

.section .text.BarbForegroundEdges,"ax",@progbits
.global BarbForegroundEdges
.type BarbForegroundEdges,@function
/* y:r24 -> first visible pixel:r24, exclusive right pixel:r25. Shared by
 * sprite composition and blood. The final row-buffer byte is scratch here;
 * blood rectangles occupy at most the first fifteen bytes. */
BarbForegroundEdges:
    subi r24,32
    cpi r24,80
    brsh .Lforeground_none
    lds r18,barbAsmScene
    subi r18,1
    cpi r18,5
    brsh .Lforeground_none
    ldi r19,80
    mul r18,r19
    movw r22,r0
    clr r1
    add r22,r24
    adc r23,r1
    subi r22,lo8(-(BARB_RUNTIME_BASE+BARB_FOREGROUND_OFFSET))
    sbci r23,hi8(-(BARB_RUNTIME_BASE+BARB_FOREGROUND_OFFSET))
    clr r24
    clr r25
    ldi r20,lo8(barbAsmLineBuf+106)
    ldi r21,hi8(barbAsmLineBuf+106)
    ldi r18,1
    call BarbGraphicsRead
    lds r24,barbAsmLineBuf+106
    mov r25,r24
    swap r24
    andi r24,15
    subi r24,lo8(-24)
    andi r25,15
    subi r25,lo8(-176)
    ret
.Lforeground_none:
    clr r24
    ldi r25,214
    ret
.size BarbForegroundEdges,.-BarbForegroundEdges

.section .text.BarbBladeRow,"ax",@progbits
.global BarbBladeRow
.type BarbBladeRow,@function
/* i:r24,y:r22,start:r20,left:r18,right:r16. Cursor storage is the ten
 * unused bytes after the maximum cached fighter pose, as in the C path.
 * Persistent: sy:r2, flags:r3, x:r5:r4, width:r6, count:r7, row:r8,
 * source x:r9, cursor:r11:r10, data:Y. Left/start/grade use r21/r22/r23;
 * the right-edge argument stays in r16. Only ten low registers need saving. */
BarbBladeRow:
    cpi r24,2
    brlo 1f
    ret
1:  ldi r19,5
    mul r24,r19
    movw r30,r0
    clr r1
    subi r30,lo8(-(workBuf+BARB_SPRITE_CACHE_BYTES))
    sbci r31,hi8(-(workBuf+BARB_SPRITE_CACHE_BYTES))
    ldd r19,Z+2
    tst r19
    brne 1f
    ret
1:  /* Probe the next command before saving registers. A future-row command
     * leaves the cursor unchanged in the decoder, including absolute rows. */
    lds r21,spr+2
    tst r24
    breq 2f
    lds r21,spr+8
2:  mov r23,r22
    sub r23,r21
    ldd r21,Z+3
    ld r26,Z
    ldd r27,Z+1
    ld r19,X+
    andi r19,63
    cpi r19,48
    brlo 2f
    inc r21
    cpi r19,62
    brne 2f
    ld r21,X
2:  cp r23,r21
    brsh 1f
    ret
1:
    push r2
    push r3
    push r4
    push r5
    push r6
    push r7
    push r8
    push r9
    push r10
    push r11
    push r28
    push r29
    mov r2,r23
    mov r21,r18
    mov r22,r20
    movw r10,r30
    ld r28,Z+
    ld r29,Z+
    ld r7,Z+
    ld r8,Z+
    ld r9,Z
    ldi r18,6
    mul r24,r18
    movw r30,r0
    clr r1
    subi r30,lo8(-(spr))
    sbci r31,hi8(-(spr))
    ld r4,Z+
    ld r5,Z+
    adiw r30,2
    ld r18,Z+
    ld r3,Z
    mov r19,r3
    lsr r19
    lsr r19
    andi r19,3
    mov r23,r19
    sbrs r3,7
    rjmp .Lblade_loop
    ldi r30,lo8(barbAsmSpriteWidths)
    ldi r31,hi8(barbAsmSpriteWidths)
    add r30,r18
    adc r31,r1
    lpm r18,Z
    dec r18
    mov r6,r18
.Lblade_loop:
    tst r7
    brne 1f
    rjmp .Lblade_done
1:  dec r7
    ld r19,Y+
    mov r18,r19
    andi r18,63
    cpi r18,63
    breq .Lblade_next_row
    cpi r18,62
    breq .Lblade_absolute
    cpi r18,48
    brlo 1f
    inc r8
    subi r18,49
    add r9,r18
    rjmp .Lblade_compare_point
1:  mov r9,r18
    rjmp .Lblade_compare_point
.Lblade_next_row:
    inc r8
    rjmp .Lblade_compare_command
.Lblade_absolute:
    ld r8,Y+
    dec r7
.Lblade_compare_command:
    cp r2,r8
    brlo .Lblade_done
    cp r8,r2
    brlo .Lblade_advance
    rjmp .Lblade_loop
.Lblade_compare_point:
    cp r2,r8
    brlo .Lblade_done
    cp r8,r2
    brlo .Lblade_advance
    swap r19
    lsr r19
    lsr r19
    andi r19,3
    ldi r20,7
    tst r19
    breq 1f
    cp r23,r19
    brlo 1f
    ldi r20,3
1:  mov r24,r9
    clr r25
    sbrs r3,7
    rjmp 1f
    mov r18,r6
    sub r18,r24
    sbc r25,r1
    mov r24,r18
1:  add r24,r4
    adc r25,r5
    tst r25
    brne .Lblade_loop
    cp r24,r21
    brlo .Lblade_loop
    cp r24,r16
    brsh .Lblade_loop
    mov r26,r24
    lsr r26
    sub r26,r22
    clr r27
    subi r26,lo8(-(barbAsmLineBuf))
    sbci r27,hi8(-(barbAsmLineBuf))
    ld r18,X
    sbrs r24,0
    rjmp 1f
    andi r18,240
    rjmp 2f
1:  andi r18,15
    swap r20
2:  or r18,r20
    st X,r18
    rjmp .Lblade_loop
.Lblade_advance:
    movw r30,r10
    st Z+,r28
    st Z+,r29
    st Z+,r7
    st Z+,r8
    st Z,r9
    rjmp .Lblade_loop
.Lblade_done:
    pop r29
    pop r28
    rjmp .Lcompose_restore11
.size BarbBladeRow,.-BarbBladeRow

.section .text.BarbPrepareRows,"ax",@progbits
.global BarbPrepareRows
.type BarbPrepareRows,@function
/* y:r24, rows:r22, left:r20, right:r18, cursor:r16, dirty:r14,
 * count:r12, buffer:r10. No SPI or audio calls. Blood repairs are separate.
 * Persistent: y:r2, i:r3, count:r4, current/old counts:r5/r6,
 * header-ready:r7, rows:r8, left:r10, right:r12, cursor:r14, dirty:r16,
 * dirty bytes: 0 unchanged, 1 same pose/y, 3 changed pose/y or actor count.
 * old actor:Y. All structures are AVR byte-aligned.
 */
BarbPrepareRows:
    push r2
    push r3
    push r4
    push r5
    push r6
    push r7
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    push r16
    push r17
    push r28
    push r29
    mov r2,r24
    mov r4,r12
    mov r25,r10
    movw r8,r22
    movw r12,r18
    movw r10,r20
    movw r18,r14
    movw r14,r16
    movw r16,r18
    ldi r30,lo8(barbAsmCount)
    ldi r31,hi8(barbAsmCount)
    add r30,r25
    adc r31,r1
    ld r6,Z
    ldi r18,24
    mul r25,r18
    movw r28,r0
    clr r1
    subi r28,lo8(-(barbAsmHistory))
    sbci r29,hi8(-(barbAsmHistory))
    lds r5,nextSprite
    clr r3
.Lprepare_actor:
    cp r3,r4
    brlo 1f
    rjmp .Lprepare_done
1:  clr r7
    movw r30,r8
    st Z+,r1
    st Z,r1
    movw r30,r10
    ldi r18,BARB_FB_ROW_BYTES
    st Z,r18
    movw r30,r12
    st Z,r1
    movw r30,r16
    ld r20,Z
    cp r3,r5
    brlo 1f
    rjmp .Lprepare_old
1:  ldi r18,6
    mul r3,r18
    movw r30,r0
    clr r1
    subi r30,lo8(-(spr))
    sbci r31,hi8(-(spr))
    ld r22,Z+
    ld r23,Z+
    ld r18,Z+
    ld r19,Z+
    mov r24,r2
    clr r25
    sub r24,r18
    sbc r25,r19
    ld r21,Z+
    ld r19,Z
    tst r25
    breq 1f
    rjmp .Lprepare_old
1:  ldi r30,lo8(barbAsmSpriteHeights)
    ldi r31,hi8(barbAsmSpriteHeights)
    add r30,r21
    adc r31,r1
    lpm r18,Z
    cp r24,r18
    brlo 1f
    rjmp .Lprepare_old
1:  movw r30,r14
    ld r26,Z+
    ld r27,Z
    /* Cache bases are fixed during Compose(). After the first row, retain
     * the next header address instead of reconstructing base plus offset. */
    mov r18,r26
    or r18,r27
    breq .Lprepare_first_header
    /* Cached rows enter the decoder directly; only an uncached leading
     * row visits the out-of-line cache-base/header-skip setup below. */
    movw r30,r26
.Lprepare_header:
    ld r18,Z+
    ld r24,Z+
    movw r26,r30
    cpi r18,255
    breq 1f
    mov r25,r24
    sub r25,r18
    subi r25,lo8(-3)
    lsr r25
    lsr r25
    add r26,r25
    adc r27,r1
1:  /* Commit the next header address, retaining this row's data in Z. */
    mov r0,r26
    mov r25,r27
    movw r26,r14
    st X+,r0
    st X,r25
    movw r26,r8
    adiw r26,2
    st X+,r30
    st X+,r31
    st X,r18
    inc r7
    cpi r18,255
    brne .Lprepare_header_nonempty
    rjmp .Lprepare_old
.Lprepare_first_header:
    tst r3
    brne 1f
    ldi r30,lo8(workBuf)
    ldi r31,hi8(workBuf)
    rjmp .Lprepare_pose
1:  mov r18,r3
    cpi r18,1
    brne 1f
    ldi r30,lo8(barbAsmPoseB)
    ldi r31,hi8(barbAsmPoseB)
    rjmp .Lprepare_pose
1:  lds r18,barbAsmSmallSwap
    mov r25,r3
    cpi r25,3
    brne 1f
    ldi r25,1
    eor r18,r25
1:  tst r18
    breq 1f
    ldi r30,lo8(barbAsmPoseSmall)
    ldi r31,hi8(barbAsmPoseSmall)
    rjmp .Lprepare_pose
1:  ldi r30,lo8(barbAsmPoseLarge)
    ldi r31,hi8(barbAsmPoseLarge)
.Lprepare_pose:
    tst r24
    breq .Lprepare_header
.Lprepare_skip:
    ld r18,Z+
    ld r25,Z+
    cpi r18,255
    breq 1f
    sub r25,r18
    subi r25,lo8(-3)
    lsr r25
    lsr r25
    add r30,r25
    adc r31,r1
1:  dec r24
    brne .Lprepare_skip
    rjmp .Lprepare_header
.Lprepare_header_nonempty:
    bst r19,7
    brtc .Lprepare_forward
    mov r0,r24
    sub r0,r18
    dec r0
    ldi r30,lo8(barbAsmSpriteWidths)
    ldi r31,hi8(barbAsmSpriteWidths)
    add r30,r21
    adc r31,r1
    lpm r21,Z
    add r22,r21
    adc r23,r1
    movw r26,r22
    sub r26,r24
    sbc r27,r1
    sub r22,r18
    sbc r23,r1
    rjmp .Lprepare_clip_current
.Lprepare_forward:
    clr r0
    movw r26,r22
    add r26,r18
    adc r27,r1
    add r22,r24
    adc r23,r1
.Lprepare_clip_current:
    /* Most rows need no left clipping. For negative left bounds adjust the
     * prepared source offset before the shared signed world clamp. */
    sbrs r27,7
    rjmp 2f
    brtc 1f
    add r0,r26
    rjmp 2f
1:  sub r0,r26
2:
    call .Lprepare_clip
    brlt .Lprepare_current_valid
    rjmp .Lprepare_old
.Lprepare_current_valid:
    movw r30,r8
    st Z+,r26
    st Z,r22
    bld r0,7
    std Z+3,r0
    tst r20
    breq .Lprepare_old
    lsr r26
    inc r22
    lsr r22
    movw r30,r10
    st Z,r26
    movw r30,r12
    st Z,r22
.Lprepare_old:
    tst r20
    brne 1f
    rjmp .Lprepare_next
1:  cp r3,r6
    brlo 1f
    rjmp .Lprepare_next
1:  ldd r22,Y+0
    ldd r23,Y+1
    ldd r21,Y+4
.Lprepare_try_header:
    sbrc r20,1
    rjmp .Lprepare_canvas_bounds
    /* Same source pose and y: its cached row header also describes the old
     * silhouette. The ready flag rejects out-of-height rows before reading
     * their uninitialized header. Empty headers also reject the transparent
     * rows outside the pose's flash Y bounds, avoiding another bounds lookup.
     * Use raw source bounds, never the clipped current bounds. */
    tst r7
    brne 1f
    rjmp .Lprepare_next
1:
    movw r30,r8
    ldd r26,Z+2
    ldd r27,Z+3
    ld r18,-X
    ld r19,-X
    cpi r19,255
    brne 1f
    rjmp .Lprepare_next
1:
.Lprepare_old_source:
    ldd r0,Y+5
    sbrs r0,7
    rjmp .Lprepare_old_forward
    ldi r30,lo8(barbAsmSpriteWidths)
    ldi r31,hi8(barbAsmSpriteWidths)
    add r30,r21
    adc r31,r1
    lpm r24,Z
.Lprepare_old_width:
    add r22,r24
    adc r23,r1
    movw r26,r22
    sub r26,r18
    sbc r27,r1
    sub r22,r19
    sbc r23,r1
    rjmp .Lprepare_old_clip
.Lprepare_old_forward:
    movw r26,r22
    add r26,r19
    adc r27,r1
    add r22,r18
    adc r23,r1
    rjmp .Lprepare_old_clip
.Lprepare_canvas_bounds:
    ldd r18,Y+2
    ldd r19,Y+3
    mov r24,r2
    clr r25
    sub r24,r18
    sbc r25,r19
    tst r25
    breq 1f
    rjmp .Lprepare_next
1:  mov r18,r3
    cpi r18,2
    brsh .Lprepare_canvas_range
    cpi r24,80
    brsh .Lprepare_canvas_range
    /* Wound-band repair retains its original conservative canvas. */
    ldd r18,Y+5
    sbrs r18,1
    rjmp 1f
    cpi r24,32
    brlo 1f
    cpi r24,60
    brlo .Lprepare_canvas_range
1:  ldi r30,lo8(barbAsmOldBounds)
    ldi r31,hi8(barbAsmOldBounds)
    tst r3
    breq 1f
    adiw r30,2
1:  ld r18,Z+
    ld r31,Z
    mov r30,r18
    or r18,r31
    brne 1f
    rjmp .Lprepare_canvas_range
1:  add r30,r24
    adc r31,r1
    ld r19,Z
    cpi r19,255
    brne 1f
    rjmp .Lprepare_next
1:  mov r18,r19
    andi r18,15
    lsl r18
    swap r19
    andi r19,15
    lsl r19
    ldi r30,lo8(barbAsmSpriteWidths)
    ldi r31,hi8(barbAsmSpriteWidths)
    add r30,r21
    adc r31,r1
    lpm r24,Z
    neg r18
    add r18,r24
    /* Width is already decoded; a mirrored cached row must not reload it. */
    ldd r0,Y+5
    sbrc r0,7
    rjmp .Lprepare_old_width
    rjmp .Lprepare_old_forward
.Lprepare_canvas_range:
    mov r30,r21
    clr r0
    call .Lrange_bounds
    cp r24,r0
    brsh 1f
    rjmp .Lprepare_next
1:  add r30,r0
    cp r24,r30
    brlo 1f
    rjmp .Lprepare_next
1:
    ldi r30,lo8(barbAsmSpriteWidths)
    ldi r31,hi8(barbAsmSpriteWidths)
    add r30,r21
    adc r31,r1
    lpm r18,Z
    movw r26,r22
    add r22,r18
    adc r23,r1
.Lprepare_old_clip:
    call .Lprepare_clip
    brge .Lprepare_next
    lsr r26
    inc r22
    lsr r22
    movw r30,r10
    ld r18,Z
    cp r26,r18
    brsh 1f
    st Z,r26
1:  movw r30,r12
    ld r18,Z
    cp r18,r22
    brsh .Lprepare_next
    st Z,r22
.Lprepare_next:
    ldi r18,5
    add r8,r18
    adc r9,r1
    ldi r18,1
    add r10,r18
    adc r11,r1
    add r12,r18
    adc r13,r1
    add r16,r18
    adc r17,r1
    ldi r18,2
    add r14,r18
    adc r15,r1
    adiw r28,6
    inc r3
    rjmp .Lprepare_actor
.Lprepare_done:
    rjmp .Lcompose_restore
.size BarbPrepareRows,.-BarbPrepareRows

/* Fit this leaf in the unused alignment tail before the mixer LUT planes.
 * build_sfa.py links this object's .progmem input before the aligned mixer. */
.section .progmem.data.BarbRowRange,"ax",@progbits
.global BarbRowRange
.type BarbRowRange,@function
/* buffer:r24; return first:r24, exclusive last:r25. Only calls the private
 * flash-bound reader below; no SPI/audio. Include current and old visible
 * bounds, including the sword-blood band for wound owners. First row preparation
 * advances any unconsumed leading source headers. Signed y is clipped.
 */
BarbRowRange:
    ldi r30,lo8(barbAsmCount)
    ldi r31,hi8(barbAsmCount)
    add r30,r24
    adc r31,r1
    ld r19,Z
    ldi r18,24
    mul r24,r18
    movw r24,r0
    clr r1
    subi r24,lo8(-(barbAsmHistory))
    sbci r25,hi8(-(barbAsmHistory))
    lds r18,nextSprite
    ldi r26,lo8(spr)
    ldi r27,hi8(spr)
    ldi r20,128
    clr r21
.Lrange_next:
    tst r18
    brne .Lrange_actor
    tst r19
    breq .Lrange_done
    movw r26,r24
    mov r18,r19
    clr r19
    rjmp .Lrange_next
.Lrange_actor:
    dec r18
    adiw r26,2
    ld r22,X+
    ld r23,X+
    ld r30,X+
    ld r0,X+
    call .Lrange_bounds
    tst r23
    brmi .Lrange_top
    brne .Lrange_bottom
    cpi r22,128
    brsh .Lrange_bottom
.Lrange_top:
    add r22,r0
    adc r23,r1
    tst r23
    brmi .Lrange_negative
    brne .Lrange_bottom
    cp r22,r20
    brsh 1f
    mov r20,r22
1:  rjmp .Lrange_add
.Lrange_negative:
    clr r20
.Lrange_add:
    add r22,r30
    adc r23,r1
    tst r23
    brmi .Lrange_next
    brne .Lrange_bottom
    cpi r22,128
    brlo 1f
.Lrange_bottom:
    ldi r22,128
1:  cp r21,r22
    brsh .Lrange_next
    mov r21,r22
    rjmp .Lrange_next
.Lrange_done:
    movw r24,r20
    ret
.size BarbRowRange,.-BarbRowRange

.section .text.BarbRowBounds,"ax",@progbits
/* Private ABI: f:r30, flags:r0 -> top:r0, height:r30. Other registers,
 * including r1, stay intact. Keep this reader outside the alignment gap. */
.Lrange_bounds:
    bst r0,1
    ldi r31,0
    lsl r30
    subi r30,lo8(-(barbAsmSpriteYBounds))
    sbci r31,hi8(-(barbAsmSpriteYBounds))
    lpm r0,Z+
    lpm r30,Z
    brtc .Lrange_bounds_done
    cpi r30,60
    brsh 1f
    ldi r30,60
1:  mov r31,r0
    cpi r31,32
    brlo .Lrange_bounds_done
    ldi r31,32
    mov r0,r31
.Lrange_bounds_done:
    sub r30,r0
    ret

/* Fill the existing pre-mixer alignment tail with this 30-byte leaf. */
.section .progmem.data.BarbPrepareClip,"ax",@progbits
/* Signed [lo:X, hi:r23:r22), clamp to the 214-pixel world. Return signed
 * compare flags without destroying the bound bytes. */
.Lprepare_clip:
    sbrs r27,7
    rjmp 1f
    clr r26
    clr r27
1:  cpi r22,214
    cpc r23,r1
    brlt 1f
    ldi r22,214
    clr r23
1:  cp r26,r22
    cpc r27,r23
    ret
