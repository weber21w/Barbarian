/* GPL-3.0-or-later. Packed SPI framebuffer, nine cycles per pixel. */
#include "assets.h"
#include "hud.h"
#if SFA2_NET
#include "network_uart.inc"
#endif
.section .bss
.balign 256
.global walk_palette
walk_palette: .space 16

.section .text
.global walk_video,walk_line,walk_end
walk_video:
.Lnormal_video:
    lds r16,walk_ready
    and r16,r16
    brne 1f
    ret
1:
    lds r16,walk_busy
    and r16,r16
    breq 2f
    .global walk_deadline_miss
walk_deadline_miss:
    lds r16,walk_missed
    inc r16
    sts walk_missed,r16
    ret
2:
    /* The 240-cycle mixer exceeds the stock kernel's 230-cycle entry
     * allowance by ten clocks. Compensate here, before the first HSYNC. */
#if SFA2_NET
    /* UART cannot change enable state during this non-nested render. */
    ldi r23,0xff
    lds r16,_SFR_MEM_ADDR(UCSR0B)
    sbrs r16,RXEN0
    ldi r23,0xdf
    WAIT r19,1228
#else
    WAIT r19,1233
#endif
    clr r2
    lds r16,render_lines_count
    mov r8,r16
    lds r6,barbScanBase
    lds r7,barbScanBase+1
    clr r3
    lds r9,barbScanCamera
    ldi r16,13
    sub r9,r16
    clr r11
    sbrc r9,7
    dec r11
    ldi r16,107
    mov r12,r16
    ldi r16,FRAME_LINES-BARB_HUD_ROWS+1
    mov r13,r16
    ldi r24,lo8(pm(walk_kernel_0))
    ldi r25,hi8(pm(walk_kernel_0))
    movw r14,r24
    clr r1
    ldi r16,3
walk_line:
    cbi _SFR_IO_ADDR(PORTA),4
    out _SFR_IO_ADDR(SPDR),r16
#if SFA2_NET
    NET_RX_A
#else
    WAIT r19,15
#endif
    nop
    nop /* Match the stock transfer's 18-clock OUT-to-OUT cadence. */
    out _SFR_IO_ADDR(SPDR),r3
#if SFA2_NET
    NET_RX_B
#else
    WAIT r19,15
#endif
    nop
    nop
    out _SFR_IO_ADDR(SPDR),r7
#if SFA2_NET
    NET_RX_C
#else
    WAIT r19,16
#endif
    nop
    out _SFR_IO_ADDR(SPDR),r6
    /* Seven clocks on either path. Use r14 directly for the kernel pointer,
     * leaving 18 clocks from the last address OUT to the first dummy OUT. */
    add r6,r12
    adc r7,r2
    cp r8,r13
    breq 1f
    nop
    rjmp 2f
1:  add r6,r9
    adc r7,r11
2:
#if SFA2_NET
    NET_RX_ERROR
#else
    WAIT r19,10
#endif
    out _SFR_IO_ADDR(SPDR),r2
    rcall hsync_pulse
    in r10,_SFR_IO_ADDR(SPDR)
    out _SFR_IO_ADDR(SPDR),r2
#if SFA2_NET
    NET_TX 1
#else
    WAIT r19,24
#endif
    movw ZL,r14
    ijmp

/* Unrolled packed-pixel palette lookup overlaps the next SPI byte transfer.
 * Its first DAC write follows six cycles of nibble lookup. */
#include "kernels.inc"

walk_end:
    /* Prepare the next command inside the existing three-clock tail. The
     * earlier command and RCALL absorb the longer headers without moving
     * pixel DAC writes or the HSYNC call entry. */
    ldi r16,3
    WAIT r19,2
    out VIDEO_PORT,r2
    sbi _SFR_IO_ADDR(PORTA),4
    dec r8
    breq walk_frame_end
    jmp walk_line
walk_frame_end:
    WAIT r19,76
    call hsync_pulse
walk_finish_flags:
    lds ZL,sync_flags
    ldi r20,SYNC_FLAG_FIELD
    eor ZL,r20
    ori ZL,SYNC_FLAG_VSYNC
    sts sync_flags,ZL
    ldi ZL,(1<<OCF1A)
    sts _SFR_MEM_ADDR(TIFR1),ZL
    clr r1
    ret

/* void WalkBlitRow(u8 *dst, const u8 *src, u8 bytes).
 * Nibble zero is transparent; all other indices replace the destination. */
.section .text.WalkBlitRow,"ax",@progbits
.global WalkBlitRow
WalkBlitRow:
    push r28
    push r29
    movw r28,r24
    movw r26,r22
1:  ld r18,X+
    ld r19,Y
    mov r21,r18
    andi r21,0xf0
    breq 2f
    andi r19,0x0f
    or r19,r21
2:  andi r18,0x0f
    breq 3f
    andi r19,0xf0
    or r19,r18
3:  st Y+,r19
    dec r20
    brne 1b
    pop r29
    pop r28
    ret

/* Read packed sprite bytes and blend during the next SPI transfer.
 * ABI: r24 bank, r23:r22 address, r21:r20 destination, r18 nonzero count.
 * The shortest (both nibbles transparent) path leaves 17 cycles between
 * starting a transfer and reading it. Interrupts may only lengthen this.
 * The final dummy read finishes before CS is released.
 */
.global WalkSpiBlit
WalkSpiBlit:
    call SpiRamSeqReadStart
    movw XL,r20
    WAIT r25,12
1:  in r22,_SFR_IO_ADDR(SPDR)
    out _SFR_IO_ADDR(SPDR),r25
    ld r19,X
    mov r23,r22
    andi r23,0xf0
    breq 2f
    andi r19,0x0f
    or r19,r23
2:  andi r22,0x0f
    breq 3f
    andi r19,0xf0
    or r19,r22
3:  st X+,r19
    nop
    nop
    dec r18
    brne 1b
    nop
    sbi _SFR_IO_ADDR(PORTA),4
    ret


.section .text
#if SFA2_NET
#include "network_uart.S"
#endif
