#include <avr/io.h>
#include <avr/interrupt.h>
#include "un_uart_avr.h"
UnUart un_uart;
uint8_t um_write(uint8_t byte){
    uint8_t head=un_uart.tx_head,next=(head+1)&(UM_UART_TX_SIZE-1);
    if(next==un_uart.tx_tail)return 0;
    un_uart.tx[head]=byte;un_uart.tx_head=next;return 1;
}
void um_uart_rx(uint8_t byte){
    uint8_t head=un_uart.rx_head,tail=un_uart.rx_tail;
    uint8_t next=(head+1)&(UM_UART_RX_SIZE-1);
    if(next==tail){um_abort(&un_uart.serial);return;}
    un_uart.rx[head]=byte;un_uart.rx_head=next;
    um_flow(&un_uart.serial,(tail-next-1)&(UM_UART_RX_SIZE-1));
}
void um_uart_flow(void){
    uint8_t saved=SREG;cli();
    um_flow(&un_uart.serial,(un_uart.rx_tail-un_uart.rx_head-1)&(UM_UART_RX_SIZE-1));
    SREG=saved;
}
uint8_t um_uart_read(void){
    uint8_t tail=un_uart.rx_tail;
    if(tail==un_uart.rx_head)return 0;
    uint8_t b=un_uart.rx[tail];un_uart.rx_tail=(tail+1)&(UM_UART_RX_SIZE-1);
    return um_byte(&un_uart.serial,b);
}
uint8_t um_uart_request(uint8_t opcode,const uint8_t *args,uint8_t n){
    if(n>UM_UART_ARGS || (un_uart.serial.pending | un_uart.serial.length))return 0;
    un_uart.request[2]=opcode;
    uint8_t *out=un_uart.request+3;
    for(uint8_t i=n;i;i--)*out++=*args++;
    return um_request(&un_uart.serial,un_uart.request,n);
}
