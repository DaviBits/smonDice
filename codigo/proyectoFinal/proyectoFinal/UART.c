#include "UART.h"

void UART_init(void) {
	UCSR0A = 0;
	UCSR0B = (1<<TXEN0)|(1<<RXEN0);
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
	UBRR0  = 103; // 9600 baud a 16MHz
}

unsigned char UART_read(void) {
	if (UCSR0A & (1<<RXC0)) return UDR0;
	return 0;
}

void UART_write(unsigned char data) {
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}

void UART_string(char *string) {
	while(*string != 0) {
		UART_write(*string);
		string++;
	}
}