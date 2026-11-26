/* Compressed effect FIFO producer, AVR-GCC ABI. The interrupt owns tails;
 * foreground publishes each filled chunk by updating its head last. Keep the
 * original 32-byte low-water threshold, repeat and EOF ordering. Compressed
 * input and decoded output chunks are limited to 16 bytes.
 * Raster exclusion remains in AudioBusWait; interrupts stay enabled here. */
#include "audio.h"
.section .text.AudioRefill,"ax",@progbits
.global AudioRefill
.type AudioRefill,@function
AudioRefill:
    /* No pending effects: go straight to the music gate without saving any
     * registers. Active effects retain their per-lane low-water checks below. */
    lds r18,barbAudioRemaining
    lds r19,barbAudioRemaining+1
    or r18,r19
    lds r19,barbAudioRemaining+2
    or r18,r19
    lds r19,barbAudioRemaining+3
    or r18,r19
    breq .Lrefill_music
    rjmp .Lrefill_effects
.Lrefill_music:
#if BARB_SID
    lds r18,barbSidEnabled
    tst r18
    breq .Lrefill_return
    lds r18,barbSidBusy
    tst r18
    brne .Lrefill_return
    lds r18,barbSidFault
    tst r18
    brne .Lrefill_return
    lds r18,spiram_adpcm_music_active
    tst r18
    breq .Lrefill_synth
    lds r18,audio_music_head
    lds r19,audio_music_tail
    sub r18,r19
    brcc 1f
    subi r18,lo8(-BARB_MUSIC_FIFO_BYTES)
1:  cpi r18,160
    brsh .Lrefill_return
.Lrefill_synth:
    jmp AudioRefillMusic
#endif
.Lrefill_return:
    ret
.Lrefill_effects:
    /* slot:r2, head:r3, free:r4, chunk:r5, position:r7:r6,
     * remaining:r9:r8, producer positions pointer:Y. No local stack frame.
     * The four word arrays share one explicit 18-byte producer structure. */
    push r2
    push r3
    push r4
    push r5
    push r6
    push r7
    push r8
    push r9
    push r28
    push r29
    clr r2
.Lrefill_lane:
    mov r18,r2
    lsl r18
    ldi r28,lo8(barbAudioPositions)
    ldi r29,hi8(barbAudioPositions)
    add r28,r18
    adc r29,r1
    ldd r8,Y+4
    ldd r9,Y+5
    mov r18,r8
    or r18,r9
    brne 1f
    rjmp .Lrefill_next
1:  ldi r30,lo8(audio_sfx_head)
    ldi r31,hi8(audio_sfx_head)
    add r30,r2
    adc r31,r1
    ld r3,Z
    ldi r30,lo8(audio_sfx_tail)
    ldi r31,hi8(audio_sfx_tail)
    add r30,r2
    adc r31,r1
    ld r18,Z
    mov r19,r3
    sub r19,r18
    brcc 1f
    subi r19,lo8(-96)
1:  ldi r18,95
    sub r18,r19
    cpi r18,32
    brsh 1f
    rjmp .Lrefill_next
1:  mov r4,r18
    ld r6,Y
    ldd r7,Y+1
.Lrefill_chunk:
    ldi r18,96
    sub r18,r3
    cp r4,r18
    brsh 1f
    mov r18,r4
1:  tst r9
    brne 1f
    cp r8,r18
    brsh 1f
    mov r18,r8
1:  cpi r18,17
    brlo 1f
    ldi r18,16
1:  mov r5,r18
    call AudioReadSfx
    ldd r8,Y+4
    ldd r9,Y+5
    mov r18,r3
    cpi r18,96
    brne 1f
    clr r3
1:  ldi r30,lo8(audio_sfx_head)
    ldi r31,hi8(audio_sfx_head)
    add r30,r2
    adc r31,r1
    st Z,r3
    st Y,r6
    std Y+1,r7
    sub r8,r5
    sbc r9,r1
    std Y+4,r8
    std Y+5,r9
    sub r4,r5
    mov r18,r8
    or r18,r9
    brne .Lrefill_more
    ldi r30,lo8(barbAudioRepeats)
    ldi r31,hi8(barbAudioRepeats)
    add r30,r2
    adc r31,r1
    ld r18,Z
    tst r18
    breq .Lrefill_eof
    ldi r30,lo8(audioHuffLeft)
    ldi r31,hi8(audioHuffLeft)
    add r30,r2
    adc r31,r1
    st Z,r1
    ldd r6,Y+8
    ldd r7,Y+9
    st Y,r6
    std Y+1,r7
    ldd r8,Y+12
    ldd r9,Y+13
    std Y+4,r8
    std Y+5,r9
    rjmp .Lrefill_more
.Lrefill_eof:
    ldi r30,lo8(audio_sfx_eof)
    ldi r31,hi8(audio_sfx_eof)
    add r30,r2
    adc r31,r1
    ldi r18,1
    st Z,r18
.Lrefill_more:
    tst r4
    breq .Lrefill_next
    mov r18,r8
    or r18,r9
    breq .Lrefill_next
    rjmp .Lrefill_chunk
.Lrefill_next:
    inc r2
    mov r18,r2
    cpi r18,2
    breq 1f
    rjmp .Lrefill_lane
1:  pop r29
    pop r28
    pop r9
    pop r8
    pop r7
    pop r6
    pop r5
    pop r4
    pop r3
    pop r2
    rjmp .Lrefill_music
.size AudioRefill,.-AudioRefill

#if BARB_SID
#include "compositor_layout.inc"
.if BARB_MUSIC_AHEAD_BYTES != 128
.error "SidPrepare requires 128-byte ahead queue"
.endif
/* Shallow idle-only producer. Check gates/room before touching the stack;
 * only the produced byte count survives the ABI-clobbering helper calls.
 * Graphics scratch belongs to this caller only in the main VSYNC wait. */
.section .text.SidPrepare,"ax",@progbits
.global SidPrepare
.type SidPrepare,@function
SidPrepare:
    lds r18,barbSidEnabled
    tst r18
    breq .Lprepare_return
    lds r18,barbSidBusy
    tst r18
    brne .Lprepare_return
    lds r18,barbSidFault
    tst r18
    brne .Lprepare_return
    lds r22,barbSidAheadTail
    lds r18,barbSidAheadHead
    subi r22,1
    sub r22,r18
    andi r22,127
    breq .Lprepare_return
    ldi r19,128
    sub r19,r18
    cp r19,r22
    brsh 1f
    mov r22,r19
1:  push r16
    ldi r18,1
    sts barbSidBusy,r18
    ldi r24,lo8(barbAsmLineBuf)
    ldi r25,hi8(barbAsmLineBuf)
    call barbSidGenerate
    tst r24
    breq .Lprepare_done
    mov r16,r24
    clr r25
    call AudioBusWait
    ldi r18,1
    sts walk_busy,r18
    lds r22,barbSidAheadHead
    ldi r23,0
    subi r22,lo8(-BARB_MUSIC_AHEAD_BASE)
    sbci r23,hi8(-BARB_MUSIC_AHEAD_BASE)
    mov r18,r16
    clr r19
    ldi r20,lo8(barbAsmLineBuf)
    ldi r21,hi8(barbAsmLineBuf)
    ldi r24,0
    call SpiRamWriteFrom
    lds r18,barbSidAheadHead
    add r18,r16
    andi r18,127
    sts barbSidAheadHead,r18
.Lprepare_done:
    sts barbSidBusy,r1
    pop r16
.Lprepare_return:
    ret
.size SidPrepare,.-SidPrepare
#endif

/* Private effect-producer ABI: slot r2, output head r3, chunk r5,
 * compressed position r7:r6. Return updated head/position; r8:r9 scratch. */
.section .text.AudioReadSfx,"ax",@progbits
.global AudioReadSfx
AudioReadSfx:
 .irp reg,10,11,13,16,17
 push r\reg
 .endr
 mov r8,r5
 ldi r19,16
 mov r11,r19
 mov r10,r11
 ldi ZL,lo8(audioHuffLeft)
 ldi ZH,hi8(audioHuffLeft)
 add ZL,r2
 adc ZH,r1
 ld r17,Z
 adiw ZL,2
 ld r16,Z
.Lhuff_output:
 clr r13
 ldi r18,4
 mov r9,r18
/* Select already-positioned symbols: avoid three shifts per symbol. */
.Lhuff_symbol:
 rcall .Lhuff_bit
 brcc .Lhuff_two
 rcall .Lhuff_bit
 brcc .Lhuff_one
 rcall .Lhuff_bit
 ldi r18,0
 brcc .Lhuff_pack
 ldi r18,192
 rjmp .Lhuff_pack
.Lhuff_two:
 ldi r18,128
 rjmp .Lhuff_pack
.Lhuff_one:
 ldi r18,64
.Lhuff_pack:
 lsr r13
 lsr r13
 or r13,r18
 dec r9
 brne .Lhuff_symbol
 mov r18,r3
 rcall .Lhuff_ring
 st Z,r13
 inc r3
 dec r8
 brne .Lhuff_output
 ldi ZL,lo8(audioHuffLeft)
 ldi ZH,hi8(audioHuffLeft)
 add ZL,r2
 adc ZH,r1
 st Z,r17
 adiw ZL,2
 st Z,r16
 .irp reg,17,16,13,11,10
 pop r\reg
 .endr
 ret
.Lhuff_bit:
 tst r17
 brne .Lhuff_ready
 cp r10,r11
 brlo .Lhuff_cached
 mov r24,r11
 clr r25
 call AudioBusWait
 ldi r18,1
 sts walk_busy,r18
 ldi r20,lo8(audioHuffCache)
 ldi r21,hi8(audioHuffCache)
 movw r22,r6
 mov r18,r11
 clr r19
 ldi r24,1
 call SpiRamReadInto
 clr r10
.Lhuff_cached:
 ldi ZL,lo8(audioHuffCache)
 ldi ZH,hi8(audioHuffCache)
 add ZL,r10
 adc ZH,r1
 ld r16,Z
 inc r10
 ldi r18,1
 add r6,r18
 adc r7,r1
 ldi r17,8
.Lhuff_ready:
 lsr r16
 dec r17
 ret
.Lhuff_ring:
 ldi ZL,lo8(audio_sfx_ring)
 ldi ZH,hi8(audio_sfx_ring)
 tst r2
 breq 1f
 subi ZL,lo8(-96)
 sbci ZH,hi8(-96)
1: add ZL,r18
 adc ZH,r1
 ret
