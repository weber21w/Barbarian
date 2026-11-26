/* Foreground pulse/triangle/saw synth; levels 0..15, signed sum -45..45.
 * Hoist the constant negative level sum into batch setup. T retains half-byte
 * packing state; all voice arithmetic and the raster/audio ISR preserve it.
 * ZL holds voice 1's pulse threshold; r0 is the waveform/multiply scratch.
 * This leaves callee-saved r14 untouched. Interrupts remain enabled throughout.
 * No IO, SPI or SD in this kernel. */
.section .text.SidRender,"ax",@progbits
.global SidRender
SidRender:
 .irp reg,2,3,4,5,6,7,8,9,10,11,12,13,15,16,17
 push r\reg
 .endr
 movw r26,r24
 lds r2,sidPhase
 lds r3,sidPhase+1
 lds r4,sidPhase+2
 lds r5,sidPhase+3
 lds r6,sidPhase+4
 lds r7,sidPhase+5
 lds r8,sidParameters
 lds r9,sidParameters+1
 lds r10,sidParameters+5
 lds r11,sidParameters+6
 lds r12,sidParameters+10
 lds r13,sidParameters+11
 lds r30,sidParameters+2
 lds r15,sidParameters+7
 lds r16,sidParameters+12
 lds r17,sidParameters+4
 lds r18,sidParameters+9
 lds r19,sidParameters+14
 lds r24,sidParameters+3
 lds r25,sidParameters+8
 lds r31,sidParameters+13
 lds r23,sidHalf
 bst r23,0
 clr r23
 sub r23,r17
 sub r23,r18
 sub r23,r19
 lds r21,sidLow
 .macro VOICE LO,HI,SL,SH,PULSE,LEVEL,CONTROL
 add \LO,\SL
 adc \HI,\SH
 sbrc \CONTROL,4
 rjmp .Ltriangle\@
 sbrc \CONTROL,5
 rjmp .Lsaw\@
 cp \HI,\PULSE
 brlo .Lvoice_done\@
 add r20,\LEVEL
 add r20,\LEVEL
 /* Pulse is the common path. Cold waveform work returns here explicitly. */
.Lvoice_done\@:
 .subsection 1
.Ltriangle\@:
 mov r0,\HI
 sbrc r0,7
 com r0
 lsl r0
 rjmp .Lscale\@
.Lsaw\@:
 mov r0,\HI
.Lscale\@:
 mul r0,\LEVEL
 lsl r1
 add r20,r1
 clr r1
 rjmp .Lvoice_done\@
 .subsection 0
 .endm
.Lsample:
 mov r20,r23
 VOICE r2,r3,r8,r9,r30,r17,r24
 VOICE r4,r5,r10,r11,r15,r18,r25
 VOICE r6,r7,r12,r13,r16,r19,r31
 /* sum+64 is nonnegative (19..109). Its bits 3..6 are the sample.
  * The high-nibble path shifts those bits directly into position. */
 subi r20,-64
 brts .Lpack
 lsr r20
 lsr r20
 lsr r20
 mov r21,r20
 set
 rjmp .Lpacked
.Lpack:
 lsl r20
 andi r20,0xf0
 or r20,r21
 st X+,r20
 clt
.Lpacked:
 dec r22
 brne .Lsample
.Lfinished:
 sts sidPhase,r2
 sts sidPhase+1,r3
 sts sidPhase+2,r4
 sts sidPhase+3,r5
 sts sidPhase+4,r6
 sts sidPhase+5,r7
 clr r20
 bld r20,0
 sts sidHalf,r20
 sts sidLow,r21
 .irp reg,17,16,15,13,12,11,10,9,8,7,6,5,4,3,2
 pop r\reg
 .endr
 ret

/* SID control bytes: an SRAM hit is a leaf with no register saves. On a
 * miss, retain count/tail in their existing state across ABI-clobbering bus
 * calls. Only foreground owns this ring and SidRefill holds its busy gate.
 * Bound reads to 32 bytes and the exact ring end; publish tail after SPI.
 * No SD access and no interrupt masking. The C implementation is the oracle. */
#include "compositor_layout.inc"
.if BARB_SID_RING_BYTES != 1024
.error "SID reader requires the 1024-byte control ring"
.endif
.if (BARB_SID_SPI_A >> 16) != ((BARB_SID_SPI_A+BARB_SID_RING_BYTES-1) >> 16)
.error "SID control ring must fit one SPI bank"
.endif
.section .text.SidRead,"ax",@progbits
.global SidRead
.type SidRead,@function
SidRead:
 lds r18,barbSidAt
 lds r19,barbSidCount
 cp r18,r19
 breq .Lsid_read_refill
.Lsid_read_byte:
 mov r30,r18
 clr r31
 subi r30,lo8(-(barbSidBuffer))
 sbci r31,hi8(-(barbSidBuffer))
 ld r24,Z
 inc r18
 sts barbSidAt,r18
 ret
.Lsid_read_refill:
 lds r22,barbSidTail
 lds r23,barbSidTail+1
 lds r24,barbSidHead
 lds r25,barbSidHead+1
 sub r24,r22
 sbc r25,r23
 andi r25,3
 mov r18,r24
 or r18,r25
 brne .Lsid_read_ready
 ldi r24,1
 sts barbSidFault,r24
 sts audio_music_underruns,r24
 sts spiram_adpcm_music_active,r1
 sts spiram_adpcm_music_volume,r1
 clr r24
 ret
.Lsid_read_ready:
 ldi r18,32
 tst r25
 brne 1f
 cp r24,r18
 brsh 1f
 mov r18,r24
1:
 /* Room to the end of the ring, not merely the next SRAM/SPI page. */
 clr r20
 ldi r21,4
 sub r20,r22
 sbc r21,r23
 tst r21
 brne 1f
 cp r20,r18
 brsh 1f
 mov r18,r20
1:
 sts barbSidCount,r18
 sts barbSidAt,r1
 mov r24,r18
 clr r25
 call AudioBusWait
 ldi r18,1
 sts walk_busy,r18
 lds r22,barbSidTail
 lds r23,barbSidTail+1
 subi r22,lo8(-BARB_SID_SPI_A)
 sbci r23,hi8(-BARB_SID_SPI_A)
 ldi r24,(BARB_SID_SPI_A >> 16)
 ldi r20,lo8(barbSidBuffer)
 ldi r21,hi8(barbSidBuffer)
 lds r18,barbSidCount
 clr r19
 call SpiRamReadInto
 /* The private kernel releases walk_busy after raising chip select. */
 lds r24,barbSidTail
 lds r25,barbSidTail+1
 lds r18,barbSidCount
 add r24,r18
 adc r25,r1
 andi r25,3
 sts barbSidTail,r24
 sts barbSidTail+1,r25
 clr r18
 rjmp .Lsid_read_byte
.size SidRead,.-SidRead
