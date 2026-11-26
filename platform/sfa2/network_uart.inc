/* Five constant-cycle pieces fitted into existing scanout SPI waits.
 * r16..r20 and Z are scratch; r1 must be zero. X and r24:r25 survive.
 * Video owns RX production and PA7/READY while service is active. Offsets are asserted in network.c against the shared 98-byte adapter. */
.macro NET_RX_A
    lds r16,_SFR_MEM_ADDR(UCSR0A)
    sbrs r16,RXC0
    rjmp .Lrx_none\@
    lds r17,_SFR_MEM_ADDR(UDR0)
    lds r18,un_uart+42
    lds r19,un_uart+43
    mov r20,r18
    inc r20
    andi r20,31
    rjmp .Lrx_a_done\@
.Lrx_none\@:
    clr r16
    WAIT r19,9
.Lrx_a_done\@:
.endm
.macro NET_RX_B
    sbrs r16,RXC0
    rjmp .Lrx_b_none\@
    cp r20,r19
    breq .Lrx_full\@
    mov ZL,r18
    ldi ZH,0
    subi ZL,lo8(-(un_uart+46))
    sbci ZH,hi8(-(un_uart+46))
    st Z,r17
    sts un_uart+42,r20
    nop
    rjmp .Lrx_b_done\@
.Lrx_full\@:
    ori r16,(1<<DOR0)
    WAIT r18,7
    rjmp .Lrx_b_done\@
.Lrx_b_none\@:
    WAIT r19,12
.Lrx_b_done\@:
.endm
.macro NET_RX_C
    sbrs r16,RXC0
    rjmp .Lrx_c_none\@
    sub r19,r20
    dec r19
    andi r19,31
    lds r18,un_uart+37
    cp r18,r19
    brlo .Lrx_room\@
    cbi _SFR_IO_ADDR(DDRA),7
    ldi r18,0
    sts un_uart+35,r18
    rjmp .Lrx_c_done\@
.Lrx_room\@:
    WAIT r18,4
    rjmp .Lrx_c_done\@
.Lrx_c_none\@:
    WAIT r19,13
.Lrx_c_done\@:
.endm
.macro NET_RX_ERROR
    /* r16 is dead after this piece; TX reloads it. Ten clocks on each path,
     * returning two clocks to the guarded scanout command/address bytes. */
    andi r16,(1<<FE0)|(1<<DOR0)|(1<<UPE0)
    brne .Lrx_bad\@
    WAIT r18,6
    rjmp .Lrx_err_done\@
.Lrx_bad\@:
    ldi r18,7
    sts un_uart+34,r18
    sts un_uart+35,r1
    cbi _SFR_IO_ADDR(DDRA),7
.Lrx_err_done\@:
.endm
.macro NET_TX scanout=0
    lds r16,_SFR_MEM_ADDR(UCSR0A)
.if \scanout
    and r16,r23
.endif
    sbrs r16,UDRE0
    rjmp .Ltx_busy\@
    lds r18,un_uart+44
    lds r19,un_uart+45
    cp r18,r19
    breq .Ltx_empty\@
    mov ZL,r19
.if \scanout
    /* Single-page TX ring is checked after link. Pays for the service mask. */
    subi ZL,lo8(-(un_uart+78))
    ldi ZH,hi8(un_uart+78)
.else
    ldi ZH,0
    subi ZL,lo8(-(un_uart+78))
    sbci ZH,hi8(-(un_uart+78))
.endif
    ld r17,Z
    sts _SFR_MEM_ADDR(UDR0),r17
    inc r19
    andi r19,7
    sts un_uart+45,r19
    rjmp .Ltx_done\@
.Ltx_empty\@:
    WAIT r19,11-\scanout
    rjmp .Ltx_done\@
.Ltx_busy\@:
    WAIT r19,19-\scanout
.Ltx_done\@:
.endm
