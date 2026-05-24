/*
 * FAT32.h
 *
 * Created: 18/05/2026 09:19:17 p. m.
 *  Author: HP
 */ 


#ifndef FAT32_H_
#define FAT32_H_

#include <avr/io.h>
#include <string.h>
#include "SD.h"

typedef struct {
	uint32_t firstSector;    // sector inicial de datos
	uint32_t rootCluster;    // cluster del directorio raiz
	uint32_t fatStart;       // sector donde empieza la FAT
	uint16_t sectorsPerCluster;
	uint16_t bytesPerSector;
} FAT32_Info;

uint8_t  FAT32_init(void);
uint32_t FAT32_findFile(const char *name83); // nombre en formato 8.3 "1       BMP"
uint8_t  FAT32_readFile(uint32_t startCluster, uint8_t *buf, uint16_t size);
uint32_t FAT32_clusterToSector(uint32_t cluster);

extern FAT32_Info fat32;

#endif