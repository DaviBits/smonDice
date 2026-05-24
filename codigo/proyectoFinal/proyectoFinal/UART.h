#ifndef UART_H_
#define UART_H_

#include <avr/io.h>

#define UART_PARITY_NONE 0
#define UART_PARITY_EVEN 2
#define UART_PARITY_ODD  3
#define UART_STOP_1      0
#define UART_STOP_2      1
#define UART_DATA_5      0
#define UART_DATA_6      1
#define UART_DATA_7      2
#define UART_DATA_8      3

void          UART_init(void);
unsigned char UART_read(void);
void          UART_write(unsigned char data);
void          UART_string(char *string);

#endif