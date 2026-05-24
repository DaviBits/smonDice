#include "i2c.h"

void i2c_init(void) {
	TWSR = 0x00;                          // prescaler = 1
	TWBR = ((F_CPU / SCL_CLOCK) - 16) / 2;
	TWCR = (1 << TWEN);
}

uint8_t i2c_start(uint8_t address) {
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != 0x08 && (TWSR & 0xF8) != 0x10) return 1;
	TWDR = address;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	uint8_t status = TWSR & 0xF8;
	if (status != 0x18 && status != 0x40) return 1;
	return 0;
}

void i2c_start_wait(uint8_t address) {
	while (i2c_start(address));
}

void i2c_stop(void) {
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	while (TWCR & (1<<TWSTO));
}

uint8_t i2c_write(uint8_t data) {
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	return (TWSR & 0xF8) != 0x28;
}

uint8_t i2c_read_ack(void) {
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}

uint8_t i2c_read_nack(void) {
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}