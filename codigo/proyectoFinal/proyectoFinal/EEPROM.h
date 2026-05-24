#ifndef EEPROM_H_
#define EEPROM_H_

#include <avr/io.h>

unsigned char EEPROM_read(unsigned int uiAddress);
void EEPROM_write(unsigned int uiAddress, unsigned char ucData);
void EEPROM_update(unsigned int uiAddress, unsigned char ucData);

unsigned char EEPROM_read(unsigned int uiAddress)
{
	/* Esperar a que termine cualquier escritura previa */
	while(EECR & (1<<EEPE));
	/* Configurar dirección */
	EEAR = uiAddress;
	/* Iniciar lectura */
	EECR |= (1<<EERE);
	/* Retornar dato */
	return EEDR;
}

void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
	/* Esperar a que termine cualquier escritura previa */
	while(EECR & (1<<EEPE));
	/* Configurar dirección y dato */
	EEAR = uiAddress;
	EEDR = ucData;
	/* Escribir un uno lógico en EEMPE (Master Write Enable) */
	EECR |= (1<<EEMPE);
	/* Iniciar escritura configurando EEPE */
	EECR |= (1<<EEPE);
}

void EEPROM_update(unsigned int uiAddress, unsigned char ucData)
{
	/* Leemos el valor actual en esa dirección */
	unsigned char valorActual = EEPROM_read(uiAddress);
	
	/* Solo escribimos si el valor nuevo es diferente al que ya existe */
	if (valorActual != ucData)
	{
		EEPROM_write(uiAddress, ucData);
	}
}

#endif /* EEPROM_H_ */