#ifndef BUZZER_H_
#define BUZZER_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

// Pin OC2B = D3 = PD3
#define BUZZER_DDR  DDRD
#define BUZZER_PORT PORTD
#define BUZZER_PIN  PD3

void BUZZER_init(void);
void startSound(void);
void okSound(void);
void errorSound(void);
void victorySound(void);

#endif