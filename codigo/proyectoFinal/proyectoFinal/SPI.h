#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>

void SPI_MasterInit(void);
void SPI_MasterTransmit(char cData);
char SPI_MasterReceive(void);  // la vamos a necesitar para la SD

#endif