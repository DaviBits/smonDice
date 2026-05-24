#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include "ILI9341.h"
#include "FAT32.h"
#include "UART.h"
#include <stdio.h>

// =============================================
// FUENTE 5x7 básica (ASCII 32-126)
// =============================================
static const uint8_t font5x7[][5] = {
	{0x00,0x00,0x00,0x00,0x00}, // space
	{0x00,0x00,0x5F,0x00,0x00}, // !
	{0x00,0x07,0x00,0x07,0x00}, // "
	{0x14,0x7F,0x14,0x7F,0x14}, // #
	{0x24,0x2A,0x7F,0x2A,0x12}, // $
	{0x23,0x13,0x08,0x64,0x62}, // %
	{0x36,0x49,0x55,0x22,0x50}, // &
	{0x00,0x05,0x03,0x00,0x00}, // '
	{0x00,0x1C,0x22,0x41,0x00}, // (
	{0x00,0x41,0x22,0x1C,0x00}, // )
	{0x14,0x08,0x3E,0x08,0x14}, // *
	{0x08,0x08,0x3E,0x08,0x08}, // +
	{0x00,0x50,0x30,0x00,0x00}, // ,
	{0x08,0x08,0x08,0x08,0x08}, // -
	{0x00,0x60,0x60,0x00,0x00}, // .
	{0x20,0x10,0x08,0x04,0x02}, // /
	{0x3E,0x51,0x49,0x45,0x3E}, // 0
	{0x00,0x42,0x7F,0x40,0x00}, // 1
	{0x42,0x61,0x51,0x49,0x46}, // 2
	{0x21,0x41,0x45,0x4B,0x31}, // 3
	{0x18,0x14,0x12,0x7F,0x10}, // 4
	{0x27,0x45,0x45,0x45,0x39}, // 5
	{0x3C,0x4A,0x49,0x49,0x30}, // 6
	{0x01,0x71,0x09,0x05,0x03}, // 7
	{0x36,0x49,0x49,0x49,0x36}, // 8
	{0x06,0x49,0x49,0x29,0x1E}, // 9
	{0x00,0x36,0x36,0x00,0x00}, // :
	{0x00,0x56,0x36,0x00,0x00}, // ;
	{0x08,0x14,0x22,0x41,0x00}, //
	{0x14,0x14,0x14,0x14,0x14}, // =
	{0x00,0x41,0x22,0x14,0x08}, // >
	{0x02,0x01,0x51,0x09,0x06}, // ?
	{0x32,0x49,0x79,0x41,0x3E}, // @
	{0x7E,0x11,0x11,0x11,0x7E}, // A
	{0x7F,0x49,0x49,0x49,0x36}, // B
	{0x3E,0x41,0x41,0x41,0x22}, // C
	{0x7F,0x41,0x41,0x22,0x1C}, // D
	{0x7F,0x49,0x49,0x49,0x41}, // E
	{0x7F,0x09,0x09,0x09,0x01}, // F
	{0x3E,0x41,0x49,0x49,0x7A}, // G
	{0x7F,0x08,0x08,0x08,0x7F}, // H
	{0x00,0x41,0x7F,0x41,0x00}, // I
	{0x20,0x40,0x41,0x3F,0x01}, // J
	{0x7F,0x08,0x14,0x22,0x41}, // K
	{0x7F,0x40,0x40,0x40,0x40}, // L
	{0x7F,0x02,0x0C,0x02,0x7F}, // M
	{0x7F,0x04,0x08,0x10,0x7F}, // N
	{0x3E,0x41,0x41,0x41,0x3E}, // O
	{0x7F,0x09,0x09,0x09,0x06}, // P
	{0x3E,0x41,0x51,0x21,0x5E}, // Q
	{0x7F,0x09,0x19,0x29,0x46}, // R
	{0x46,0x49,0x49,0x49,0x31}, // S
	{0x01,0x01,0x7F,0x01,0x01}, // T
	{0x3F,0x40,0x40,0x40,0x3F}, // U
	{0x1F,0x20,0x40,0x20,0x1F}, // V
	{0x3F,0x40,0x38,0x40,0x3F}, // W
	{0x63,0x14,0x08,0x14,0x63}, // X
	{0x07,0x08,0x70,0x08,0x07}, // Y
	{0x61,0x51,0x49,0x45,0x43}, // Z
	{0x00,0x7F,0x41,0x41,0x00}, // [
	{0x02,0x04,0x08,0x10,0x20}, // backslash
	{0x00,0x41,0x41,0x7F,0x00}, // ]
	{0x04,0x02,0x01,0x02,0x04}, // ^
	{0x40,0x40,0x40,0x40,0x40}, // _
	{0x00,0x01,0x02,0x04,0x00}, // `
	{0x20,0x54,0x54,0x54,0x78}, // a
	{0x7F,0x48,0x44,0x44,0x38}, // b
	{0x38,0x44,0x44,0x44,0x20}, // c
	{0x38,0x44,0x44,0x48,0x7F}, // d
	{0x38,0x54,0x54,0x54,0x18}, // e
	{0x08,0x7E,0x09,0x01,0x02}, // f
	{0x0C,0x52,0x52,0x52,0x3E}, // g
	{0x7F,0x08,0x04,0x04,0x78}, // h
	{0x00,0x44,0x7D,0x40,0x00}, // i
	{0x20,0x40,0x44,0x3D,0x00}, // j
	{0x7F,0x10,0x28,0x44,0x00}, // k
	{0x00,0x41,0x7F,0x40,0x00}, // l
	{0x7C,0x04,0x18,0x04,0x78}, // m
	{0x7C,0x08,0x04,0x04,0x78}, // n
	{0x38,0x44,0x44,0x44,0x38}, // o
	{0x7C,0x14,0x14,0x14,0x08}, // p
	{0x08,0x14,0x14,0x18,0x7C}, // q
	{0x7C,0x08,0x04,0x04,0x08}, // r
	{0x48,0x54,0x54,0x54,0x20}, // s
	{0x04,0x3F,0x44,0x40,0x20}, // t
	{0x3C,0x40,0x40,0x40,0x7C}, // u
	{0x1C,0x20,0x40,0x20,0x1C}, // v
	{0x3C,0x40,0x30,0x40,0x3C}, // w
	{0x44,0x28,0x10,0x28,0x44}, // x
	{0x0C,0x50,0x50,0x50,0x3C}, // y
	{0x44,0x64,0x54,0x4C,0x44}, // z
};

// =============================================
// VARIABLES INTERNAS
// =============================================
static int16_t  _cursorX    = 0;
static int16_t  _cursorY    = 0;
static uint16_t _textColor  = TFT_WHITE;
static uint8_t  _textSize   = 1;

// =============================================
// FUNCIONES INTERNAS
// =============================================
static void tft_writeCommand(uint8_t cmd) {
	TFT_DC_LOW();
	TFT_CS_LOW();
	SPI_MasterTransmit(cmd);
	TFT_CS_HIGH();
}

static void tft_writeData(uint8_t data) {
	TFT_DC_HIGH();
	TFT_CS_LOW();
	SPI_MasterTransmit(data);
	TFT_CS_HIGH();
}

static void tft_writeData16(uint16_t data) {
	TFT_DC_HIGH();
	TFT_CS_LOW();
	SPI_MasterTransmit(data >> 8);
	SPI_MasterTransmit(data & 0xFF);
	TFT_CS_HIGH();
}

static void tft_setWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
	tft_writeCommand(ILI9341_CASET);
	tft_writeData16(x0);
	tft_writeData16(x1);
	tft_writeCommand(ILI9341_PASET);
	tft_writeData16(y0);
	tft_writeData16(y1);
	tft_writeCommand(ILI9341_RAMWR);
}

// =============================================
// INIT
// =============================================
void ILI9341_init(void) {
	// Configurar pines como salida
	TFT_CS_DDR  |= (1<<TFT_CS_PIN);
	TFT_DC_DDR  |= (1<<TFT_DC_PIN);
	TFT_RST_DDR |= (1<<TFT_RST_PIN);

	// Reset hardware
	TFT_RST_HIGH();
	_delay_ms(10);
	TFT_RST_LOW();
	_delay_ms(20);
	TFT_RST_HIGH();
	_delay_ms(150);

	// Secuencia de inicialización
	tft_writeCommand(ILI9341_SWRESET);  _delay_ms(100);
	tft_writeCommand(ILI9341_SLPOUT);   _delay_ms(100);

	tft_writeCommand(ILI9341_FRMCTR1);
	tft_writeData(0x00); tft_writeData(0x18);

	tft_writeCommand(ILI9341_DFUNCTR);
	tft_writeData(0x08); tft_writeData(0x82); tft_writeData(0x27);

	tft_writeCommand(ILI9341_PWCTR1);  tft_writeData(0x23);
	tft_writeCommand(ILI9341_PWCTR2);  tft_writeData(0x10);

	tft_writeCommand(ILI9341_VMCTR1);
	tft_writeData(0x3E); tft_writeData(0x28);

	tft_writeCommand(ILI9341_VMCTR2);  tft_writeData(0x86);

	tft_writeCommand(ILI9341_MADCTL);  tft_writeData(0x28); // landscape

	tft_writeCommand(ILI9341_COLMOD);  tft_writeData(0x55); // 16-bit color

	tft_writeCommand(ILI9341_INVOFF);
	tft_writeCommand(ILI9341_NORON);   _delay_ms(10);
	tft_writeCommand(ILI9341_DISPON);  _delay_ms(100);
}

// =============================================
// DIBUJAR PIXEL
// =============================================
void ILI9341_drawPixel(int16_t x, int16_t y, uint16_t color) {
	if (x < 0 || x >= TFT_WIDTH || y < 0 || y >= TFT_HEIGHT) return;
	tft_setWindow(x, y, x, y);
	tft_writeData16(color);
}

// =============================================
// RELLENAR RECTANGULO
// =============================================
void ILI9341_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
	if (x >= TFT_WIDTH || y >= TFT_HEIGHT) return;
	tft_setWindow(x, y, x+w-1, y+h-1);
	TFT_DC_HIGH();
	TFT_CS_LOW();
	for (int32_t i = (int32_t)w * h; i > 0; i--) {
		SPI_MasterTransmit(color >> 8);
		SPI_MasterTransmit(color & 0xFF);
	}
	TFT_CS_HIGH();
}

// =============================================
// RELLENAR PANTALLA
// =============================================
void ILI9341_fillScreen(uint16_t color) {
	ILI9341_fillRect(0, 0, TFT_WIDTH, TFT_HEIGHT, color);
}

// =============================================
// COLOR RGB ? RGB565
// =============================================
uint16_t ILI9341_color565(uint8_t r, uint8_t g, uint8_t b) {
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// =============================================
// TEXTO
// =============================================
void ILI9341_setCursor(int16_t x, int16_t y) {
	_cursorX = x;
	_cursorY = y;
}

void ILI9341_setTextColor(uint16_t color) {
	_textColor = color;
}

void ILI9341_setTextSize(uint8_t size) {
	_textSize = size ? size : 1;
}

static void tft_drawChar(int16_t x, int16_t y, char c, uint16_t color, uint8_t size) {
	if (c < 32 || c > 122) return;
	const uint8_t *glyph = font5x7[c - 32];

	for (uint8_t col = 0; col < 5; col++) {
		uint8_t line = glyph[col];
		for (uint8_t row = 0; row < 7; row++) {
			if (line & (1 << row)) {
				if (size == 1) {
					ILI9341_drawPixel(x + col, y + row, color);
					} else {
					ILI9341_fillRect(x + col*size, y + row*size, size, size, color);
				}
			}
		}
	}
}

void ILI9341_print(const char *str) {
	while (*str) {
		if (*str == '\n') {
			_cursorX  = 0;
			_cursorY += 8 * _textSize;
			} else {
			tft_drawChar(_cursorX, _cursorY, *str, _textColor, _textSize);
			_cursorX += 6 * _textSize;
			if (_cursorX > TFT_WIDTH - 6) {
				_cursorX  = 0;
				_cursorY += 8 * _textSize;
			}
		}
		str++;
	}
}

// =============================================
// DIBUJAR IMAGEN (array RGB565 en RAM/Flash)
// =============================================
void ILI9341_drawImage(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *img) {
	tft_setWindow(x, y, x+w-1, y+h-1);
	TFT_DC_HIGH();
	TFT_CS_LOW();
	for (int32_t i = 0; i < (int32_t)w*h; i++) {
		SPI_MasterTransmit(img[i] >> 8);
		SPI_MasterTransmit(img[i] & 0xFF);
	}
	TFT_CS_HIGH();
}



void ILI9341_drawBMP(const char *name83, int16_t x, int16_t y) {
	 uint32_t cluster = FAT32_findFile(name83);
	 if (cluster == 0) {
		 UART_string("Archivo no encontrado\r\n");
		 return;
	 }
	 UART_string("cluster ok\r\n");

	 static uint8_t buf[512];
	 uint32_t sector = FAT32_clusterToSector(cluster);
	 SD_readBlock(sector, buf);
	 UART_string("header leido\r\n");

	 if (buf[0] != 'B' || buf[1] != 'M') {
		 UART_string("No es BMP\r\n");
		 return;
	 }
	 UART_string("firma ok\r\n");

	 uint32_t dataOffset = *(uint32_t*)&buf[10];
	 int32_t  bmpWidth   = *(int32_t*)&buf[18];
	 int32_t  bmpHeight  = *(int32_t*)&buf[22];
	 uint16_t bmpDepth   = *(uint16_t*)&buf[28];

	 char tmp[40];
	 sprintf(tmp, "W:%ld H:%ld D:%d\r\n", bmpWidth, bmpHeight, bmpDepth);
	 UART_string(tmp);

	if (bmpDepth != 24) {
		UART_string("Solo BMP 24-bit\r\n");
		return;
	}

	uint8_t  flip    = (bmpHeight > 0);
	int32_t  h       = flip ? bmpHeight : -bmpHeight;
	int32_t  w       = bmpWidth;
	uint32_t rowSize = (w * 3 + 3) & ~3;

	// Dibuja fila por fila
	static uint8_t rowBuf[160 * 3]; // max 320px

	for (int32_t row = 0; row < h; row++) {
		int32_t  srcRow    = flip ? (h - 1 - row) : row;
		uint32_t bytePos   = dataOffset + srcRow * rowSize;
		uint32_t sectorNum = sector + (bytePos / 512);
		uint16_t offset    = bytePos % 512;

		// Lee los bytes necesarios para esta fila
		SD_readBlock(sectorNum, buf);
		uint16_t bytes_in_first = 512 - offset;

		if (bytes_in_first >= (uint16_t)(w * 3)) {
			memcpy(rowBuf, &buf[offset], w * 3);
			} else {
			memcpy(rowBuf, &buf[offset], bytes_in_first);
			SD_readBlock(sectorNum + 1, buf);
			memcpy(&rowBuf[bytes_in_first], buf, w * 3 - bytes_in_first);
		}

		// Manda fila a la pantalla
		tft_setWindow(x, y + row, x + w - 1, y + row);
		TFT_DC_HIGH();
		TFT_CS_LOW();
		for (int32_t col = 0; col < w; col++) {
			uint8_t b = rowBuf[col * 3];
			uint8_t g = rowBuf[col * 3 + 1];
			uint8_t r = rowBuf[col * 3 + 2];
			uint16_t color = ILI9341_color565(r, g, b);
			SPI_MasterTransmit(color >> 8);
			SPI_MasterTransmit(color & 0xFF);
		}
		TFT_CS_HIGH();
	}
	UART_string("BMP ok\r\n");
}