#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>

// Dirección I2C (AD0 = GND ? 0x68)
#define MPU6050_ADDR        0x68

// Registros principales
#define MPU6050_SMPLRT_DIV  0x19
#define MPU6050_CONFIG      0x1A
#define MPU6050_GYRO_CONFIG 0x1B
#define MPU6050_ACCEL_CONFIG 0x1C
#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_TEMP_OUT_H  0x41
#define MPU6050_GYRO_XOUT_H 0x43
#define MPU6050_PWR_MGMT_1  0x6B

typedef struct {
	int16_t ax, ay, az;   // Acelerómetro (raw)
	int16_t gx, gy, gz;   // Giroscopio   (raw)
	float   temp;          // Temperatura  (°C)
} MPU6050_Data;

typedef enum {
	GESTO_NINGUNO,
	GESTO_AGITA,
	GESTO_INCLINA_DER,
	GESTO_INCLINA_IZQ,
} Gesto;

Gesto mpu6050_detectar_gesto(void);

void mpu6050_init(void);
void mpu6050_read(MPU6050_Data *data);
float getMagnitud();

// Conversión a unidades físicas
// Accel: dividir raw / 16384.0 ? g  (rango ±2g por defecto)
// Gyro:  dividir raw / 131.0   ? °/s (rango ±250°/s por defecto)

#endif