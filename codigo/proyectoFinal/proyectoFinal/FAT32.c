/*
 * FAT32.c
 *
 * Created: 18/05/2026 09:19:40 p. m.
 *  Author: HP
 */ 
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include "FAT32.h"
#include "UART.h"
#include <stdio.h>
#include <util/delay.h>

FAT32_Info fat32;
static uint8_t sector_buf[512];

uint8_t FAT32_init(void) {
	
	SD_CS_HIGH();
	for (uint8_t i = 0; i < 10; i++) SPI_MasterTransmit(0xFF);
	_delay_ms(10);

	if (SD_readBlock(0, sector_buf) != SD_OK) {
		UART_string("error leyendo s0\r\n");
		return 0xFF;
	}
	
	char tmp[50];
	sprintf(tmp, "s0[0]=%02X s0[510]=%02X s0[511]=%02X\r\n",
	sector_buf[0], sector_buf[510], sector_buf[511]);
	UART_string(tmp);

	uint32_t partStart = 0;
	if (sector_buf[510] == 0x55 && sector_buf[511] == 0xAA &&
	(sector_buf[0] == 0xEB || sector_buf[0] == 0xE9)) {
		partStart = 0;
		UART_string("VBR directo\r\n");
		} else {
		partStart = *(uint32_t*)&sector_buf[454];
		sprintf(tmp, "MBR partStart=%lu\r\n", partStart);
		UART_string(tmp);
		if (SD_readBlock(partStart, sector_buf) != SD_OK) return 0xFF;
		sprintf(tmp, "VBR[0]=%02X VBR[510]=%02X VBR[511]=%02X\r\n",
		sector_buf[0], sector_buf[510], sector_buf[511]);
		UART_string(tmp);
	}

	fat32.bytesPerSector    = *(uint16_t*)&sector_buf[11];
	fat32.sectorsPerCluster = sector_buf[13];
	uint16_t reservedSectors = *(uint16_t*)&sector_buf[14];
	uint8_t  numFATs         = sector_buf[16];
	uint32_t sectorsPerFAT   = *(uint32_t*)&sector_buf[36];
	fat32.rootCluster        = *(uint32_t*)&sector_buf[44];
	fat32.fatStart    = partStart + reservedSectors;
	fat32.firstSector = partStart + reservedSectors + (numFATs * sectorsPerFAT);

	sprintf(tmp, "bps=%d spc=%d root=%lu\r\n",
	fat32.bytesPerSector, fat32.sectorsPerCluster, fat32.rootCluster);
	UART_string(tmp);

	return SD_OK;
}

 uint32_t FAT32_clusterToSector(uint32_t cluster) {
	return fat32.firstSector + (cluster - 2) * fat32.sectorsPerCluster;
}

static uint32_t FAT32_nextCluster(uint32_t cluster) {
	uint32_t fatOffset  = cluster * 4;
	uint32_t fatSector  = fat32.fatStart + (fatOffset / 512);
	uint16_t fatIndex   = fatOffset % 512;

	SD_readBlock(fatSector, sector_buf);
	return *(uint32_t*)&sector_buf[fatIndex] & 0x0FFFFFFF;
}

uint32_t FAT32_findFile(const char *name83) {
	uint32_t cluster = fat32.rootCluster;
	uint16_t maxClusters = 1000; // límite de seguridad

	while (cluster < 0x0FFFFFF8 && maxClusters--) {  // ? agrega maxClusters
		uint32_t sector = FAT32_clusterToSector(cluster);

		for (uint8_t s = 0; s < fat32.sectorsPerCluster; s++) {
			SD_readBlock(sector + s, sector_buf);

			for (uint16_t i = 0; i < 512; i += 32) {
				if (sector_buf[i] == 0x00) return 0;
				if (sector_buf[i] == 0xE5) continue;
				if (sector_buf[i+11] & 0x08) continue;
				if (sector_buf[i+11] & 0x10) continue;

				if (memcmp(&sector_buf[i], name83, 11) == 0) {
					uint32_t startCluster = ((uint32_t)sector_buf[i+21] << 24) |
					((uint32_t)sector_buf[i+20] << 16) |
					((uint32_t)sector_buf[i+27] << 8)  |
					sector_buf[i+26];
					return startCluster;
				}
			}
		}
		cluster = FAT32_nextCluster(cluster);
	}
	UART_string("archivo no encontrado\r\n");
	return 0;
}
