/*
 * mpu6050Twi.h
 *
 * Created: 30/03/2018 23:11:29
 *  Author: Schwarz
 */


#ifndef __MPU6050_TWI_H_
#define __MPU6050_TWI_H_ 1

#include "twiMaster.h"

typedef enum mpu6050GyrScale_t {
	MPU6050_GYR_RANGE_250_DPS	= 0,
	MPU6050_GYR_RANGE_500_DPS	= 1,
	MPU6050_GYR_RANGE_1000_DPS	= 2,
	MPU6050_GYR_RANGE_2000_DPS	= 3
} mpu6050GyrScale_t;

typedef enum mpu6050AccScale_t {
	MPU6050_ACC_RANGE_2_G		= 0,
	MPU6050_ACC_RANGE_4_G		= 1,
	MPU6050_ACC_RANGE_8_G		= 2,
	MPU6050_ACC_RANGE_1_6G		= 3
} mpu6050AccScale_t;

typedef enum mpu6050FIFOFlags_t {
	MPU6050_FIFO_TEMP		= 128,
	MPU6050_FIFO_XG			= 64,
	MPU6050_FIFO_YG			= 32,
	MPU6050_FIFO_ZG			= 16,
	MPU6050_FIFO_ACCEL		= 8,
	MPU6050_FIFO_SLV2		= 4,
	MPU6050_FIFO_SLV1		= 2,
	MPU6050_FIFO_SLV0		= 1
} mpu6050FIFOFlags_t;


void mpu6050AccConfig(mpu6050AccScale_t scale);
void mpu6050GyrConfig(mpu6050GyrScale_t scale);
void mpu6050FIFOConfig(mpu6050FIFOFlags_t flags);
void mpu6050FIFOOverflowInterrupt(bool_t enabled);
void mpu6050I2CMasterInterrupt(bool_t enabled);
void mpu6050DataReadyInterrupt(bool_t enabled);
void mpu6050InterruptStatus(bool_t enabled);

#endif /* MCP6050TWI_H_ */