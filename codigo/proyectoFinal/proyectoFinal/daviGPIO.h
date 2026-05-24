#ifndef daviGPIO
#define daviGPIO
#define F_CPU 16000000UL
#include <avr/io.h>

#define LOW  0
#define HIGH 1

#define INPUT  0
#define OUTPUT 1

void pinMode(int pin, int mode);
void portMode(char port, int mode);
uint8_t digitalRead(int pin);
void digitalWrite(int pin, int state);
uint8_t portRead(char port);
void portWrite(char port, int state);

#endif /* daviGPIO */