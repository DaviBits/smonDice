#include "SPI.h"

void SPI_MasterInit(void) {
	DDRB |=  (1<<PINB2)|(1<<PINB3)|(1<<PINB5); // SS, MOSI, SCK como salida
	DDRB &= ~(1<<PINB4);                         // MISO como entrada
	SPCR  =  (1<<SPE)|(1<<MSTR);                // Enable SPI, Master
	SPSR  =  (1<<SPI2X);                         // fck/2
	// CPOL=0, CPHA=0, MSB first, SPR=0 — ya es el default
}

void SPI_MasterTransmit(char cData) {
	SPDR = cData;
	while(!(SPSR & (1<<SPIF)));
}

char SPI_MasterReceive(void) {
	SPDR = 0xFF;                    // manda dummy para generar clock
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}