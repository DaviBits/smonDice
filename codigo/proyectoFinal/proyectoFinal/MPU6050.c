#include "MPU6050.h"
#include "I2C.h"
#include <math.h>

static void mpu6050_write_reg(uint8_t reg, uint8_t val) {
	i2c_start_wait((MPU6050_ADDR << 1) | 0);
	i2c_write(reg);
	i2c_write(val);
	i2c_stop();
}
Gesto mpu6050_detectar_gesto(void) {
	MPU6050_Data data;
	mpu6050_read(&data);

	float ax = data.ax / 16384.0f;
	float ay = data.ay / 16384.0f;
	float az = data.az / 16384.0f;

	float magnitud = sqrtf(ax*ax + ay*ay + az*az);

	if (magnitud > 1.7f)  return GESTO_AGITA;
	//if (az < -0.8f)       return GESTO_SUBE;instruccion descartada
	//if (az >  1.8f)       return GESTO_BAJA;instruccion descartada
	if (ax >  0.7f)       return GESTO_INCLINA_DER;
	if (ax < -0.7f)       return GESTO_INCLINA_IZQ;

	return GESTO_NINGUNO;
}

float getMagnitud(){
	MPU6050_Data data;
	mpu6050_read(&data);
	float ax = data.ax / 16384.0f;
	float ay = data.ay / 16384.0f;
	float az = data.az / 16384.0f;

	return sqrtf(ax*ax + ay*ay + az*az);
}
void mpu6050_init(void) {
	i2c_init();
	mpu6050_write_reg(MPU6050_PWR_MGMT_1,  0x00); // Despertar chip
	mpu6050_write_reg(MPU6050_SMPLRT_DIV,  0x07); // Sample rate = 1kHz / (1+7) = 125 Hz
	mpu6050_write_reg(MPU6050_CONFIG,       0x00); // Sin filtro DLPF
	mpu6050_write_reg(MPU6050_GYRO_CONFIG,  0x00); // ±250 °/s
	mpu6050_write_reg(MPU6050_ACCEL_CONFIG, 0x00); // ±2 g
}

void mpu6050_read(MPU6050_Data *data) {
	// Apuntar al primer registro de datos
	i2c_start_wait((MPU6050_ADDR << 1) | 0);
	i2c_write(MPU6050_ACCEL_XOUT_H);
	i2c_stop();

	// Lectura en ráfaga: 14 bytes (accel + temp + gyro)
	i2c_start_wait((MPU6050_ADDR << 1) | 1);

	data->ax = (int16_t)((i2c_read_ack() << 8) | i2c_read_ack());
	data->ay = (int16_t)((i2c_read_ack() << 8) | i2c_read_ack());
	data->az = (int16_t)((i2c_read_ack() << 8) | i2c_read_ack());

	int16_t raw_temp = (int16_t)((i2c_read_ack() << 8) | i2c_read_ack());
	data->temp = (raw_temp / 340.0f) + 36.53f;  // Fórmula del datasheet

	data->gx = (int16_t)((i2c_read_ack() << 8) | i2c_read_ack());
	data->gy = (int16_t)((i2c_read_ack() << 8) | i2c_read_ack());
	data->gz = (int16_t)((i2c_read_ack() << 8) | i2c_read_nack()); // último ? NACK
	i2c_stop();
}