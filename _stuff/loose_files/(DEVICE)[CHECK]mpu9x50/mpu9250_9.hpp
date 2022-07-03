/**
	\file		mpu9250Spi.h
	\author		Hazael dos Santos Batista
	\author		Leandro Schwarz
	\date		April 3, 2018
	\version	1
	\brief		MPU9250 Interface Driver Using SPI
*/

#ifndef __MPU9250_SPI_H
#define __MPU9250_SPI_H 1

#include "globalDefines.h"
#include "spiMaster.h"

typedef enum mpu9250GyrScale_t {
	MPU9250_GYR_RANGE_250_DPS	= 0,
	MPU9250_GYR_RANGE_500_DPS	= 1,
	MPU9250_GYR_RANGE_1000_DPS	= 2,
	MPU9250_GYR_RANGE_2000_DPS	= 3
} mpu9250GyrScale_t;

typedef enum mpu9250AccScale_t {
	MPU9250_ACC_RANGE_2_G		= 0,
	MPU9250_ACC_RANGE_4_G		= 1,
	MPU9250_ACC_RANGE_8_G		= 2,
	MPU9250_ACC_RANGE_1_6G		= 3
} mpu9250AccScale_t;

typedef struct mpu9250_t {
	bool_t		reset		: 1;
	bool_t		cycle		: 1;
	bool_t		sleep		: 1;
	uint8_t		gyrScale	: 2;
	uint8_t		accScale	: 2;
	bool_t		disableAccX	: 1;
	bool_t		disableAccY	: 1;
	bool_t		disableAccZ	: 1;
	bool_t		disableGyrX	: 1;
	bool_t		disableGyrY	: 1;
	bool_t		disableGyrZ	: 1;

	uint8_t		unusedBits	: 4;
} mpu9250_t;

typedef enum mpu9250Response_t {
	MPU9250_OK	= 0
} mpu9250Response_t;

/*



typedef enum mpu9250FIFOFlags_t {
	MPU9250_FIFO_TEMP		= 128,
	MPU9250_FIFO_XG			= 64,
	MPU9250_FIFO_YG			= 32,
	MPU9250_FIFO_ZG			= 16,
	MPU9250_FIFO_ACCEL		= 8,
	MPU9250_FIFO_SLV2		= 4,
	MPU9250_FIFO_SLV1		= 2,
	MPU9250_FIFO_SLV0		= 1
} mpu9250FIFOFlags_t;


void mpu9250AccConfig(mpu9250AccScale_t scale);
void mpu9250GyrConfig(mpu9250GyrScale_t scale);
void mpu9250FIFOConfig(mpu9250FIFOFlags_t flags);
void mpu9250FIFOOverflowInterrupt(bool_t enabled);
void mpu9250I2CMasterInterrupt(bool_t enabled);
void mpu9250DataReadyInterrupt(bool_t enabled);
void mpu9250InterruptStatus(bool_t enabled);

*/

#endif