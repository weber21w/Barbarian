#include <avr/io.h>
/* Kernel has already saved r1, Z and SREG. Do not use from ordinary C. */
.section .text.NetworkUartBlank,"ax",@progbits
.global NetworkUartBlank
NetworkUartBlank:
    lds ZL,_SFR_MEM_ADDR(UCSR0B)
    sbrs ZL,RXEN0
    ret
    /* Coarse cutoff leaves at least 256 clocks to the earlier compare.
     * B is armed only on equalization half-lines, where it precedes A.
     * Reading the low byte first latches each Timer1 high byte. No wait. */
    lds ZL,_SFR_MEM_ADDR(TCNT1L)
    lds ZL,_SFR_MEM_ADDR(TCNT1H)
    lds ZH,_SFR_MEM_ADDR(TIMSK1)
    sbrs ZH,OCIE1B
    rjmp .Luart_compare_a
    lds ZH,_SFR_MEM_ADDR(OCR1BL)
    lds ZH,_SFR_MEM_ADDR(OCR1BH)
    rjmp .Luart_check
.Luart_compare_a:
    lds ZH,_SFR_MEM_ADDR(OCR1AL)
    lds ZH,_SFR_MEM_ADDR(OCR1AH)
.Luart_check:
    dec ZH
    cp ZL,ZH
    brlo .Luart_work
    ret
.Luart_work:
    push r16
    push r17
    push r18
    push r19
    push r20
    clr r1
    NET_RX_A
    NET_RX_B
    NET_RX_C
    NET_RX_ERROR
    NET_TX
    /* Foreground may drain RX or change the request without an RX event. */
    lds r18,un_uart+34
    subi r18,5
    cpi r18,2
    brsh .Luart_pause
    lds r18,barbUartPause
    tst r18
    brne .Luart_pause
    lds r18,un_uart+43
    lds r19,un_uart+42
    sub r18,r19
    dec r18
    andi r18,31
    lds r19,un_uart+37
    lds r20,un_uart+35
    subi r20,1
    adc r19,r1
    cp r19,r18
    brsh .Luart_pause
    sbi _SFR_IO_ADDR(DDRA),7
    ldi r18,1
    rjmp .Luart_publish
.Luart_pause:
    cbi _SFR_IO_ADDR(DDRA),7
    clr r18
.Luart_publish:
    sts un_uart+35,r18
    pop r20
    pop r19
    pop r18
    pop r17
    pop r16
    ret
