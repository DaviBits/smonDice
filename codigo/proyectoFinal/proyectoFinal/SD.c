#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>
#include "SD.h"
#include "UART.h"
#include <stdio.h>
// En SD.c, variable global
uint8_t sd_isHC = 0;
// =============================================
// FUNCIONES INTERNAS
// =============================================
static uint8_t SD_sendCommand(uint8_t cmd, uint32_t arg) {
	uint8_t crc = 0xFF;
	if (cmd == CMD0) crc = 0x95;
	if (cmd == CMD8) crc = 0x87;

	SD_CS_LOW();
	SPI_MasterTransmit(0x40 | cmd);
	SPI_MasterTransmit(arg >> 24);
	SPI_MasterTransmit(arg >> 16);
	SPI_MasterTransmit(arg >> 8);
	SPI_MasterTransmit(arg);
	SPI_MasterTransmit(crc);

	// Espera respuesta max 8 intentos
	uint8_t res = 0xFF;
	for (uint8_t i = 0; i < 8; i++) {
		res = SPI_MasterReceive();
		if (res != 0xFF) break;
	}
	return res;
}

static void SD_release(void) {
	SD_CS_HIGH();
	SPI_MasterReceive(); // clock extra
}

// =============================================
// INIT
// =============================================
uint8_t SD_init(void) {
	SD_CS_DDR  |= (1<<SD_CS_PIN);
	SD_CS_HIGH();

	for (uint8_t i = 0; i < 20; i++) SPI_MasterTransmit(0xFF);

	uint8_t res;
	uint8_t intentos = 0;
	do {
		res = SD_sendCommand(CMD0, 0);
		SD_release();
		_delay_ms(1);
		if (++intentos > 50) {
			UART_string("CMD0 fallo\r\n");
			return SD_ERROR;
		}
	} while (res != SD_IDLE);
	UART_string("CMD0 ok\r\n");

	res = SD_sendCommand(CMD8, 0x000001AA);
	char tmp[20];
	sprintf(tmp, "CMD8: %02X\r\n", res);
	UART_string(tmp);
	SPI_MasterReceive();
	SPI_MasterReceive();
	SPI_MasterReceive();
	SPI_MasterReceive();
	SD_release();
	_delay_ms(10);

	intentos = 0;
	do {
		res = SD_sendCommand(CMD55, 0);
		SD_release();
		_delay_ms(5);
		res = SD_sendCommand(CMD41, 0x40000000);
		SD_release();
		_delay_ms(10);
		if (++intentos > 250) {
			char t[30];
			sprintf(t, "ACMD41 res=%02X\r\n", res);
			UART_string(t);
			return SD_ERROR;
		}
	} while (res != SD_OK);
	UART_string("ACMD41 ok\r\n");

	UART_string("SD init ok\r\n");
	
	res = SD_sendCommand(CMD58, 0);
	uint8_t ocr[4];
	ocr[0] = SPI_MasterReceive();
	ocr[1] = SPI_MasterReceive();
	ocr[2] = SPI_MasterReceive();
	ocr[3] = SPI_MasterReceive();
	SD_release();

	sd_isHC = (ocr[0] & 0x40) ? 1 : 0;
	char t2[20];
	sprintf(t2, "isHC=%d\r\n", sd_isHC);
	UART_string(t2);

	if (!sd_isHC) {
		res = SD_sendCommand(CMD16, 512);
		SD_release();
		char t3[20];
		sprintf(t3, "CMD16: %02X\r\n", res);
		UART_string(t3);
		if (res != SD_OK) return SD_ERROR;
	}

	
	return SD_OK;
}

// =============================================
// LEER BLOQUE DE 512 BYTES
// =============================================
uint8_t SD_readBlock(uint32_t addr, uint8_t *buf) {
	SD_CS_HIGH();
	SPI_MasterTransmit(0xFF);

	// SDSC usa dirección en bytes, SDHC en bloques
	uint32_t realAddr = sd_isHC ? addr : addr * 512;

	uint8_t res = SD_sendCommand(CMD17, realAddr);
	char tmp[20];
	sprintf(tmp, "CMD17: %02X\r\n", res);
	UART_string(tmp);
	if (res != SD_OK) { SD_release(); return SD_ERROR; }

	uint16_t timeout = 10000;
	uint8_t token = 0xFF;
	while (token != 0xFE && --timeout) token = SPI_MasterReceive();
	if (!timeout) { SD_release(); return SD_ERROR; }

	for (uint16_t i = 0; i < 512; i++) buf[i] = SPI_MasterReceive();

	SPI_MasterReceive();
	SPI_MasterReceive();
	SD_release();
	return SD_OK;
}