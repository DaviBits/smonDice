#ifndef SD_H_
#define SD_H_

#include <avr/io.h>
#include "SPI.h"

extern uint8_t sd_isHC;

// =============================================
// PIN CS DE LA SD
// =============================================
#define SD_CS_DDR   DDRD
#define SD_CS_PORT  PORTD
#define SD_CS_PIN   PD2

#define SD_CS_LOW()  (SD_CS_PORT &= ~(1<<SD_CS_PIN))
#define SD_CS_HIGH() (SD_CS_PORT |=  (1<<SD_CS_PIN))

// =============================================
// RESPUESTAS SD
// =============================================
#define SD_OK        0x00
#define SD_IDLE      0x01
#define SD_ERROR     0xFF

// =============================================
// COMANDOS SD
// =============================================
#define CMD0   0   // GO_IDLE_STATE
#define CMD1   1   // SEND_OP_COND
#define CMD8   8   // SEND_IF_COND
#define CMD16  16  // SET_BLOCKLEN
#define CMD17  17  // READ_SINGLE_BLOCK
#define CMD41  41  // APP_SEND_OP_COND
#define CMD55  55  // APP_CMD
#define CMD58  58  // READ_OCR


// =============================================
// FUNCIONES
// =============================================
uint8_t SD_init(void);
uint8_t SD_readBlock(uint32_t addr, uint8_t *buf);

#endif