#ifndef UN_UART_AVR_H
#define UN_UART_AVR_H
#include "un_minimal.h"
/* Single-console storage. START borrows 25 bytes from the game's setup
 * workspace; keep that workspace alive until pending==0 AND length==0.
 * Ordinary commands use <=8 argument bytes (four two-byte local seats).
 * Send outgoing chat in chunks of at most eight bytes. RX remains full size. */
#define UM_UART_RX_SIZE 32
#define UM_UART_TX_SIZE 8
#define UM_UART_ARGS 8
typedef struct {
    UnMinimal serial;
    volatile uint8_t rx_head,rx_tail,tx_head,tx_tail;
    volatile uint8_t rx[UM_UART_RX_SIZE],tx[UM_UART_TX_SIZE];
    uint8_t request[UM_UART_ARGS+4];
} UnUart;
#ifdef __AVR__
typedef char um_uart_98_bytes[sizeof(UnUart)==98?1:-1];
#endif
extern UnUart un_uart;
/* Call RX in the serialized producer context, once per physical byte. The
 * foreground flow decision masks interrupts only around its short check. */
void um_uart_rx(uint8_t byte);
void um_uart_flow(void);
/* Consume at most one queued byte. Positive result is a complete validated
 * record in serial.rx.data; process it before consuming any further byte. */
uint8_t um_uart_read(void);
/* args must contain n bytes; returns 0 without modifying the retained request
 * when busy or n>8. START uses um_request directly with borrowed storage. */
uint8_t um_uart_request(uint8_t opcode,const uint8_t *args,uint8_t n);
#endif
