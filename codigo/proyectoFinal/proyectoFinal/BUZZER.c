#include "BUZZER.h"

// =============================================
// Genera un tono en Hz por ms milisegundos
// =============================================
static void tono(uint16_t hz, uint16_t ms) {
	if (hz == 0) {
		// Delay variable con loop
		for (uint16_t i = 0; i < ms; i++) _delay_ms(1);
		return;
	}

	BUZZER_DDR |= (1 << BUZZER_PIN);
	TCCR2A = (1 << COM2B0) | (1 << WGM21);
	TCCR2B = 0;

	uint32_t ocr;
	uint8_t  cs;

	ocr = (F_CPU / (2UL * 8 * hz)) - 1;
	if (ocr <= 255) {
		cs = (1 << CS21);
		} else {
		ocr = (F_CPU / (2UL * 64 * hz)) - 1;
		if (ocr <= 255) {
			cs = (1 << CS21) | (1 << CS20);
			} else {
			ocr = (F_CPU / (2UL * 256 * hz)) - 1;
			cs = (1 << CS22);
		}
	}

	OCR2A  = (uint8_t)ocr;
	TCCR2B = cs;

	// Delay variable con loop
	for (uint16_t i = 0; i < ms; i++) _delay_ms(1);

	TCCR2A = 0;
	TCCR2B = 0;
	BUZZER_PORT &= ~(1 << BUZZER_PIN);
}

// =============================================
// INIT
// =============================================
void BUZZER_init(void) {
	BUZZER_DDR  |= (1 << BUZZER_PIN);
	BUZZER_PORT &= ~(1 << BUZZER_PIN);
	TCCR2A = 0;
	TCCR2B = 0;
}

// =============================================
// SONIDO DE INICIO — melodía ascendente alegre
// =============================================
void startSound(void) {
	tono(262, 100); // DO
	tono(330, 100); // MI
	tono(392, 100); // SOL
	tono(523, 200); // DO alto
	tono(0,   50);
	tono(523, 100);
	tono(659, 300); // MI alto
}

// =============================================
// SONIDO OK — dos bips positivos
// =============================================
void okSound(void) {
	tono(523, 80);  // DO
	tono(0,   30);
	tono(659, 150); // MI
}

// =============================================
// SONIDO ERROR — bip grave descendente
// =============================================
void errorSound(void) {
	tono(330, 100); // MI
	tono(0,   20);
	tono(262, 100); // DO
	tono(0,   20);
	tono(196, 300); // SOL bajo
}

// =============================================
// SONIDO VICTORIA — jingle épico
// =============================================
void victorySound(void) {
	tono(523, 100); // DO
	tono(523, 100); // DO
	tono(523, 100); // DO
	tono(415, 75);  // SOL#
	tono(0,   25);
	tono(523, 400); // DO largo
	tono(0,   50);
	tono(494, 100); // SI
	tono(523, 100); // DO
	tono(587, 100); // RE
	tono(659, 400); // MI largo
}