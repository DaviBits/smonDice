#ifndef I2C_H
#define I2C_H

#include <avr/io.h>

#define F_CPU 16000000UL
#define SCL_CLOCK 400000L

void     i2c_init(void);
uint8_t  i2c_start(uint8_t address);
void     i2c_start_wait(uint8_t address);
uint8_t  i2c_rep_start(uint8_t address);
void     i2c_stop(void);
uint8_t  i2c_write(uint8_t data);
uint8_t  i2c_read_ack(void);
uint8_t  i2c_read_nack(void);

#endif