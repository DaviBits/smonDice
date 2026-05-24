#ifndef ILI9341_H_
#define ILI9341_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "SPI.h"
#include <stdarg.h>

// =============================================
// PINES DE CONTROL
// =============================================
#define TFT_CS_DDR   DDRB
#define TFT_CS_PORT  PORTB
#define TFT_CS_PIN   PB2

#define TFT_DC_DDR   DDRD
#define TFT_DC_PORT  PORTD
#define TFT_DC_PIN   PD7

#define TFT_RST_DDR  DDRB
#define TFT_RST_PORT PORTB
#define TFT_RST_PIN  PB0

// =============================================
// MACROS DE CONTROL
// =============================================
#define TFT_CS_LOW()   (TFT_CS_PORT  &= ~(1<<TFT_CS_PIN))
#define TFT_CS_HIGH()  (TFT_CS_PORT  |=  (1<<TFT_CS_PIN))
#define TFT_DC_LOW()   (TFT_DC_PORT  &= ~(1<<TFT_DC_PIN))
#define TFT_DC_HIGH()  (TFT_DC_PORT  |=  (1<<TFT_DC_PIN))
#define TFT_RST_LOW()  (TFT_RST_PORT &= ~(1<<TFT_RST_PIN))
#define TFT_RST_HIGH() (TFT_RST_PORT |=  (1<<TFT_RST_PIN))

// =============================================
// DIMENSIONES
// =============================================
#define TFT_WIDTH  320
#define TFT_HEIGHT 240

// =============================================
// COLORES (RGB565)
// =============================================
#define TFT_BLACK   0x0000
#define TFT_WHITE   0xFFFF
#define TFT_RED     0xF800
#define TFT_GREEN   0x07E0
#define TFT_BLUE    0x001F
#define TFT_YELLOW  0xFFE0
#define TFT_CYAN    0x07FF
#define TFT_MAGENTA 0xF81F
#define TFT_ORANGE  0xFD20
#define TFT_PINK    0xF812
// =============================================
// COMANDOS ILI9341
// =============================================
#define ILI9341_SWRESET  0x01
#define ILI9341_SLPOUT   0x11
#define ILI9341_NORON    0x13
#define ILI9341_INVOFF   0x20
#define ILI9341_DISPON   0x29
#define ILI9341_CASET    0x2A
#define ILI9341_PASET    0x2B
#define ILI9341_RAMWR    0x2C
#define ILI9341_MADCTL   0x36
#define ILI9341_COLMOD   0x3A
#define ILI9341_FRMCTR1  0xB1
#define ILI9341_DFUNCTR  0xB6
#define ILI9341_PWCTR1   0xC0
#define ILI9341_PWCTR2   0xC1
#define ILI9341_VMCTR1   0xC5
#define ILI9341_VMCTR2   0xC7
#define ILI9341_GMCTRP1  0xE0
#define ILI9341_GMCTRN1  0xE1

// =============================================
// FUNCIONES
// =============================================
void TFT_display(int16_t x, int16_t y, uint16_t color, uint16_t bg, uint8_t size, const char *fmt, ...);
void ILI9341_init(void);
void ILI9341_fillScreen(uint16_t color);
void ILI9341_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void ILI9341_drawPixel(int16_t x, int16_t y, uint16_t color);
void ILI9341_setCursor(int16_t x, int16_t y);
void ILI9341_setTextColor(uint16_t color);
void ILI9341_setTextSize(uint8_t size);
void ILI9341_print(const char *str);
void ILI9341_drawImage(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *img);
uint16_t ILI9341_color565(uint8_t r, uint8_t g, uint8_t b);
void ILI9341_drawBMP(const char *name83, int16_t x, int16_t y);

#endif