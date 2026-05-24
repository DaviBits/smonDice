/*
 * proyectoFinal.c
 *
 * Created: 13/05/2026 03:35:40 p. m.
 * Author : HP
 */ 

/*
 * proyectoFinal.c
 *
 * Created: 13/05/2026 03:35:40 p. m.
 * Author : HP
 */
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "UART.h"
#include "EEPROM.h"
#include "SPI.h"
#include "ILI9341.h"
#include "I2C.h"
#include "mpu6050.h"
#include "daviGPIO.h"
#include <string.h>
#include <stdio.h>
#define TIEMPO_INICIAL 5000;//5seg
#define TIEMPO_FINAL 1500;

//==VARIABLES PARA EL JUEGO DEL POT	
uint16_t adcVal=0;
uint16_t nv1Min=0;
uint16_t nv1Max=292;
uint16_t nv2Min=293;
uint16_t nv2Max= 620;
uint16_t nv3Min=621;
uint16_t nv3Max=1023;
//==VARIABLES PARA LOS BOTONES L, R Y START
int pinStart=4;
uint8_t iniciarJuego = 0;
int pinL=5;
int pinR=6;
typedef enum{
	nivel1, 
	nivel2,
	nivel3
	}nivel;

#define UART_println(s) do { UART_string(s); UART_string("\r\n"); } while(0)
	
void ADC_init(){
	ADMUX  |= (1 << REFS0);                              // Ref 5V
	ADCSRA |= (1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);  // Prescaler 128
	ADCSRA |= (1 << ADEN);
}

uint16_t readADC(uint8_t channel) {
	ADMUX =(ADMUX& 0xF0)|(channel&0x07);//configuramos el registro admux dejando la parte alta en 0s y la parte baja configurada al canal que queremos leer
	ADCSRA |= (1 << ADSC);//inicamos la conversion
	while (ADCSRA & (1 << ADSC));//esperamos a que acabe la conversion
	return ADC;//devolvemos el resultado
}


nivel getNivelPot(){
	if(adcVal>=nv1Min && adcVal<=nv1Max)return nivel1;
	if(adcVal>=nv2Min && adcVal<=nv2Max)return nivel2;
	if(adcVal>=nv3Min && adcVal<=nv3Max)return nivel3;
	return nivel1;
}

uint16_t generarSemilla(){
	return readADC(1);
}

void rutinaEncendido() {
	// 1. Fondo blanco -> negro
	ILI9341_fillScreen(TFT_WHITE); _delay_ms(400);
	ILI9341_fillScreen(TFT_BLACK); _delay_ms(200);

	// 2. "GameBopIt" entra desde izquierda en rojo, centrado
	for (int16_t x = -200; x <= 79; x += 8) {
		ILI9341_fillRect(0, 100, 320, 40, TFT_BLACK);
		ILI9341_setCursor(x, 108);
		ILI9341_setTextSize(3);
		ILI9341_setTextColor(TFT_RED);
		ILI9341_print("GameBopIt");
		_delay_ms(18);
	}
	_delay_ms(300);

	// 3. Barrido arcoiris
	uint16_t arcoiris[] = {
		TFT_RED, TFT_ORANGE, TFT_YELLOW, TFT_GREEN,
		TFT_CYAN, TFT_BLUE, TFT_MAGENTA, TFT_PINK, TFT_WHITE
	};
	const char letras[] = "GameBopIt";
	uint8_t nColores = 9;
	uint8_t nLetras  = 9;

	for (uint8_t sweep = 0; sweep < nColores + nLetras; sweep++) {
		ILI9341_fillRect(0, 100, 320, 40, TFT_BLACK);
		int16_t xPos = 79;
		for (uint8_t i = 0; i < nLetras; i++) {
			int8_t ci = (int8_t)sweep - (int8_t)i;
			uint16_t color;
			if      (ci < 0)        color = TFT_RED;
			else if (ci < nColores) color = arcoiris[ci];
			else                    color = arcoiris[nColores - 1];

			char tmp[2] = {letras[i], 0};
			ILI9341_setCursor(xPos, 108);
			ILI9341_setTextSize(3);
			ILI9341_setTextColor(color);
			ILI9341_print(tmp);
			xPos += 6 * 3;
		}
		_delay_ms(1);
	}

	// 4. Puchale play
	_delay_ms(300);
	ILI9341_setCursor(35, 200);
	ILI9341_setTextSize(2);
	ILI9341_setTextColor(TFT_WHITE);
	ILI9341_print("->PUCHALE PLAY<-");
}

void setUp() {
	UART_init();
	i2c_init();
	mpu6050_init();
	SPI_MasterInit();
	ILI9341_init();
	
	ADC_init();
	pinMode(pinStart, INPUT);
	pinMode(pinL, INPUT);
	pinMode(pinR, INPUT);
	srand(generarSemilla());
}

int main(void) {
	setUp();

	UART_println("UART LISTO");
	UART_println("I2C LISTO");
	UART_println("MPU LISTO");
	UART_println("-------------------");
	rutinaEncendido();
	
	while (digitalRead(pinStart) == LOW);

	_delay_ms(50);

	char buffer[10];
	Gesto gesto_actual  = GESTO_NINGUNO;
	Gesto gesto_anterior = GESTO_NINGUNO;
	uint16_t antADC = adcVal;

	// Dibuja layout base
	ILI9341_fillScreen(TFT_WHITE);

	// Zona boton (arriba)
	ILI9341_setCursor(10, 20);
	ILI9341_setTextSize(3);
	ILI9341_setTextColor(TFT_GREEN);
	ILI9341_print("---");

	// Zona gesto (medio)
	ILI9341_setCursor(10, 110);
	ILI9341_setTextSize(2);
	ILI9341_setTextColor(TFT_BLACK);
	ILI9341_print("---");

	// Zona ADC (esquina inferior derecha)
	ILI9341_setCursor(220, 220);
	ILI9341_setTextSize(1);
	ILI9341_setTextColor(TFT_YELLOW);
	ILI9341_print("ADC: ---");
	
	while(1) {
		// ---- BOTONES ----
		if (digitalRead(pinR) == HIGH) {
			TFT_display(10, 20, TFT_GREEN, TFT_BLACK, 3, "BOTON R");
		}
		if (digitalRead(pinL) == HIGH) {
			TFT_display(10, 20, TFT_CYAN, TFT_BLACK, 3, "BOTON L");
		}
		if (digitalRead(pinStart) == HIGH) {
			TFT_display(10, 20, TFT_YELLOW, TFT_BLACK, 3, "START");
		}

		// ---- GESTO ----
		gesto_actual = mpu6050_detectar_gesto();
		if (gesto_actual != gesto_anterior) {
			switch (gesto_actual) {
				case GESTO_AGITA:       TFT_display(10, 100, TFT_BLACK, TFT_WHITE, 2, "~(>_<)~ AGITA");   break;
				case GESTO_INCLINA_DER: TFT_display(10, 100, TFT_BLACK, TFT_WHITE, 2, "(>_) DERECHA");    break;
				case GESTO_INCLINA_IZQ: TFT_display(10, 100, TFT_BLACK, TFT_WHITE, 2, "(_<) IZQUIERDA");  break;
				case GESTO_NINGUNO:     TFT_display(10, 100, TFT_BLACK, TFT_WHITE, 2, "(-_-) REPOSO");    break;
				default: break;
			}

			gesto_anterior = gesto_actual;

			float magnitud = getMagnitud();
			dtostrf(magnitud, 5, 3, buffer);
			UART_println(buffer);
		}

		// ---- ADC esquina ----
		adcVal = readADC(0);
		if (adcVal != antADC) {
			TFT_display(200, 215, TFT_YELLOW, TFT_BLACK, 1, "ADC:%d", adcVal);
			antADC = adcVal;
		}
	}
	
}