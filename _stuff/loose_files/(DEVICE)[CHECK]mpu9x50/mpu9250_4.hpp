/*
 * mpu9250.h
 *
 * Created: 16/04/2018 13:44:10
 *  Author: Schwarz
 */

#ifndef __MPU9250_H_
#define __MPU9250_H_ 1

#include "globalDefines.h"
#include "twiMaster.h"

#define MPU9250_MAX_COMPASS_SAMPLE_RATE		100

typedef enum mpu9250Bits_e {
	// Config register				0x1A
	MPU9250_BIT_FIFO_MODE		= 7,
	MPU9250_BIT_EXT_SYNC_SET	= 3,
	MPU9250_BIT_DLPF_CFG		= 0,
	// Accel Config register		0x1C
	MPU9250_BIT_ACCEL_FS_SEL	= 3,
	// Power Management 2 register	0x6C
	MPU9250_BIT_DISABLE_XA		= 5,
	MPU9250_BIT_DISABLE_YA		= 4,
	MPU9250_BIT_DISABLE_ZA		= 3,
	MPU9250_BIT_DISABLE_XG		= 2,
	MPU9250_BIT_DISABLE_YG		= 1,
	MPU9250_BIT_DISABLE_ZG		= 0,

	MPU9250_OTHER				= 0
} mpu9250Bits_e;


typedef enum mpu9250DlpfCfg_e {
	MPU9250_DLPF_CFG_250_Hz_NO_DLPF		= 0,
	MPU9250_DLPF_CFG_188_Hz				= 1,
	MPU9250_DLPF_CFG_98_Hz				= 2,
	MPU9250_DLPF_CFG_42_Hz				= 3,
	MPU9250_DLPF_CFG_20_Hz				= 4,
	MPU9250_DLPF_CFG_10_Hz				= 5,
	MPU9250_DLPF_CFG_5_Hz				= 6,
	MPU9250_DLPF_CFG_4000_Hz_NO_DLPF	= 7
} mpu9250DlpfCfg_e;

typedef enum mpu9250ErrorCodes_e {
	MPU9250_ERROR_NONE							= 0,
	MPU9250_ERROR_ALL_SENSORS_OFF				= 1,	// mpu9250SetGyroFsr(), mpu9250SetAccelFsr(), mpu9250SetLpf(), mpu9250SetSampleRate()
	MPU9250_ERROR_INVALID_GYRO_FSR_VALUE		= 2,	// mpu9250SetGyroFsr()
	MPU9250_ERROR_INVALID_ACCEL_FSR_VALUE		= 3,	// mpu9250SetAccelFsr()
	MPU9250_ERROR_INVALID_DLPF_CFG_VALUE		= 4,	// mpu9250SetDlpf()
	MPU9250_ERROR_CANNOT_BE_SET_WITH_DMP_ON		= 5,	// mpu9250SetSampleRate()
	MPU9250_ERROR_SAMPLE_RATE_CANNOT_BE_ZERO	= 6,	// mpu9250SetSampleRate()
	MPU9250_ERROR_SAMPLE_RATE_TOO_HIGH			= 7,	// mpu9250LowPowerAccelMode(), mpu9250SetCompassSampleRate()
} mpu9250ErrorCodes_e;

typedef enum mpu9250ExtSyncSet_e {
	MPU9250_EXT_SYNC_DISABLED		= 0,
	MPU9250_EXT_SYNC_TEMP_OUT_L		= 1,
	MPU9250_EXT_SYNC_GYRO_XOUT_L	= 2,
	MPU9250_EXT_SYNC_GYRO_YOUT_L	= 3,
	MPU9250_EXT_SYNC_GYRO_ZOUT_L	= 4,
	MPU9250_EXT_SYNC_ACCEL_XOUT_L	= 5,
	MPU9250_EXT_SYNC_ACCEL_YOUT_L	= 6,
	MPU9250_EXT_SYNC_ACCEL_ZOUT_L	= 7
} mpu9250ExtSyncSet_e;


// typedef enum mpu9250Registers_e {
	// General configuration
	// MPU9250_REGISTER_SMPLRT_DIV				= 0x19,
	// MPU9250_REGISTER_CONFIG					= 0x1A,
	// MPU9250_REGISTER_GYRO_CONFIG			= 0x1B,
	// MPU9250_REGISTER_ACCEL_CONFIG			= 0x1C,
	// MPU9250_REGISTER_PWR_MGMT_1				= 0x6B,
	// MPU9250_REGISTER_PWR_MGMT_2				= 0x6C,

	// MPU9250_REGISTER_ACCEL_CONFIG_2			= 0x1D,

	// To do register list
	// MPU9250_REGISTER_SELF_TEST_X_GYRO		= 0x00,
	// MPU9250_REGISTER_SELF_TEST_Y_GYRO		= 0x01,
	// MPU9250_REGISTER_SELF_TEST_Z_GYRO		= 0x02,
	// MPU9250_REGISTER_SELF_TEST_X_ACCEL		= 0x0D,
	// MPU9250_REGISTER_SELF_TEST_Y_ACCEL		= 0x0E,
	// MPU9250_REGISTER_SELF_TEST_Z_ACCEL		= 0x0F,
	// MPU9250_REGISTER_XG_OFFSET_H			= 0x13,
	// MPU9250_REGISTER_XG_OFFSET_L			= 0x14,
	// MPU9250_REGISTER_YG_OFFSET_H			= 0x15,
	// MPU9250_REGISTER_YG_OFFSET_L			= 0x16,
	// MPU9250_REGISTER_ZG_OFFSET_H			= 0x17,
	// MPU9250_REGISTER_ZG_OFFSET_L			= 0x18,
	// MPU9250_REGISTER_LP_ACCEL_ODR			= 0x1E,
	// MPU9250_REGISTER_WOM_THR				= 0x1F,
	// MPU9250_REGISTER_FIFO_EN				= 0x23,
	// MPU9250_REGISTER_I2C_MST_CTRL			= 0x24,
	// MPU9250_REGISTER_I2C_SLV0_ADDR			= 0x25,
	// MPU9250_REGISTER_I2C_SLV0_REG			= 0x26,
	// MPU9250_REGISTER_I2C_SLV0_CTRL			= 0x27,
	// MPU9250_REGISTER_I2C_SLV1_ADDR			= 0x28,
	// MPU9250_REGISTER_I2C_SLV1_REG			= 0x29,
	// MPU9250_REGISTER_I2C_SLV1_CTRL			= 0x2A,
	// MPU9250_REGISTER_I2C_SLV2_ADDR			= 0x2B,
	// MPU9250_REGISTER_I2C_SLV2_REG			= 0x2C,
	// MPU9250_REGISTER_I2C_SLV2_CTRL			= 0x2D,
	// MPU9250_REGISTER_I2C_SLV3_ADDR			= 0x2E,
	// MPU9250_REGISTER_I2C_SLV3_REG			= 0x2F,
	// MPU9250_REGISTER_I2C_SLV3_CTRL			= 0x30,
	// MPU9250_REGISTER_I2C_SLV4_ADDR			= 0x31,
	// MPU9250_REGISTER_I2C_SLV4_REG			= 0x32,
	// MPU9250_REGISTER_I2C_SLV4_DO			= 0x33,
	// MPU9250_REGISTER_I2C_SLV4_CTRL			= 0x34,
	// MPU9250_REGISTER_I2C_SLV4_DI			= 0x35,
	// MPU9250_REGISTER_I2C_MST_STATUS			= 0x36,
	// MPU9250_REGISTER_INT_PIN_CFG			= 0x37,
	// MPU9250_REGISTER_INT_ENABLE				= 0x38,
	// MPU9250_REGISTER_INT_STATUS				= 0x3A,
	// MPU9250_REGISTER_ACCEL_XOUT_H			= 0x3B,
	// MPU9250_REGISTER_ACCEL_XOUT_L			= 0x2C,
	// MPU9250_REGISTER_ACCEL_YOUT_H			= 0x3D,
	// MPU9250_REGISTER_ACCEL_YOUT_L			= 0x3E,
	// MPU9250_REGISTER_ACCEL_ZOUT_H			= 0x3F,
	// MPU9250_REGISTER_ACCEL_ZOUT_L			= 0x40,
	// MPU9250_REGISTER_TEMP_OUT_H				= 0x41,
	// MPU9250_REGISTER_TEMP_OUT_L				= 0x42,
	// MPU9250_REGISTER_GYRO_XOUT_H			= 0x43,
	// MPU9250_REGISTER_GYRO_XOUT_L			= 0x44,
	// MPU9250_REGISTER_GYRO_YOUT_H			= 0x45,
	// MPU9250_REGISTER_GYRO_YOUT_L			= 0x46,
	// MPU9250_REGISTER_GYRO_ZOUT_H			= 0x47,
	// MPU9250_REGISTER_GYRO_ZOUT_L			= 0x48,
	// MPU9250_REGISTER_EXT_SENS_DATA_00		= 0x49,
	// MPU9250_REGISTER_EXT_SENS_DATA_01		= 0x4A,
	// MPU9250_REGISTER_EXT_SENS_DATA_02		= 0x4B,
	// MPU9250_REGISTER_EXT_SENS_DATA_03		= 0x4C,
	// MPU9250_REGISTER_EXT_SENS_DATA_04		= 0x4D,
	// MPU9250_REGISTER_EXT_SENS_DATA_05		= 0x4E,
	// MPU9250_REGISTER_EXT_SENS_DATA_06		= 0x4F,
	// MPU9250_REGISTER_EXT_SENS_DATA_07		= 0x50,
	// MPU9250_REGISTER_EXT_SENS_DATA_08		= 0x51,
	// MPU9250_REGISTER_EXT_SENS_DATA_09		= 0x52,
	// MPU9250_REGISTER_EXT_SENS_DATA_10		= 0x53,
	// MPU9250_REGISTER_EXT_SENS_DATA_11		= 0x54,
	// MPU9250_REGISTER_EXT_SENS_DATA_12		= 0x55,
	// MPU9250_REGISTER_EXT_SENS_DATA_13		= 0x56,
	// MPU9250_REGISTER_EXT_SENS_DATA_14		= 0x57,
	// MPU9250_REGISTER_EXT_SENS_DATA_15		= 0x58,
	// MPU9250_REGISTER_EXT_SENS_DATA_16		= 0x59,
	// MPU9250_REGISTER_EXT_SENS_DATA_17		= 0x5A,
	// MPU9250_REGISTER_EXT_SENS_DATA_18		= 0x5B,
	// MPU9250_REGISTER_EXT_SENS_DATA_19		= 0x5C,
	// MPU9250_REGISTER_EXT_SENS_DATA_20		= 0x5D,
	// MPU9250_REGISTER_EXT_SENS_DATA_21		= 0x5E,
	// MPU9250_REGISTER_EXT_SENS_DATA_22		= 0x5F,
	// MPU9250_REGISTER_EXT_SENS_DATA_23		= 0x60,
	// MPU9250_REGISTER_I2C_SLV0_DO			= 0x63,
	// MPU9250_REGISTER_I2C_SLV1_DO			= 0x64,
	// MPU9250_REGISTER_I2C_SLV2_DO			= 0x65,
	// MPU9250_REGISTER_I2C_SLV3_DO			= 0x66,
	// MPU9250_REGISTER_I2C_MST_DELAY_CTRL		= 0x67,
	// MPU9250_REGISTER_SIGNAL_PATH_RESET		= 0x68,
	// MPU9250_REGISTER_MOT_DETECT_CTRL		= 0x69,
	// MPU9250_REGISTER_USER_CTRL				= 0x6A,
	// MPU9250_REGISTER_FIFO_COUNT_H			= 0x72,
	// MPU9250_REGISTER_FIFO_COUNT_L			= 0x73,
	// MPU9250_REGISTER_FIFO_R_W				= 0x74,
	// MPU9250_REGISTER_WHO_AM_I				= 0x75,
	// MPU9250_REGISTER_XA_OFFSET_H			= 0x77,
	// MPU9250_REGISTER_XA_OFFSET_L			= 0x78,
	// MPU9250_REGISTER_YA_OFFSET_H			= 0x7A,
	// MPU9250_REGISTER_YA_OFFSET_L			= 0x7B,
	// MPU9250_REGISTER_ZA_OFFSET_H			= 0x7D,
	// MPU9250_REGISTER_ZA_OFFSET_L			= 0x7E
// } mpu9250Registers_e;

typedef enum mpu9250SensorFlags_e {
	MPU9250_SENSOR_GYRO_Z		= 0x01,
	MPU9250_SENSOR_GYRO_Y		= 0x02,
	MPU9250_SENSOR_GYRO_X		= 0x04,
	MPU9250_SENSOR_GYRO_XYZ		= 0x07,
	MPU9250_SENSOR_ACCEL_Z		= 0x08,
	MPU9250_SENSOR_ACCEL_Y		= 0x10,
	MPU9250_SENSOR_ACCEL_X		= 0x20,
	MPU9250_SENSOR_ACCEL_XYZ	= 0x38,
	MPU9250_SENSOR_COMPASS		= 0x40
} mpu9250SensorFlags_e;



typedef struct mpu9250_s {
	// Config register				0x1A
	bool_t						fifoMode				: 1;	// 1 -> Stop placing data when FIFO is full, 0 -> overwrite oldest data
	mpu9250ExtSyncSet_e			extSyncSet				: 3;	// Enables the FSYNC pin data to be sampled
	mpu9250DlpfCfg_e			dlpfCfg					: 3;	// DLPF config
	// Power Management 1 register	0x6B
	uint16_t					sampleRate;						/* Sample rate, NOT rate divider. */
	// Power Management 2 register	0x6C + Compass
	uint8_t						activeSensorFlags		: 7;	// Opposite of the Power Management sensor




	// To be confirmed
	uint16_t					maxFifo;
	uint8_t						numReg;
	uint16_t					tempSens;
	int16_t						tempOffset;
	uint16_t					bankSize;
	uint16_t					compassFsr;
	uint8_t						fifoEnable;						/* Matches fifo_en register. */
	uint8_t						intEnable;						/* Matches int enable register. */
	bool_t						bypassMode				: 1;	/* 1 if devices on auxiliary I2C bus appear on the primary. */
	bool_t						accelHalf				: 1;	/* 1 if half-sensitivity. */
	bool_t						lpAccelMode				: 1;	/* 1 if device in low-power accel-only mode. */
	bool_t						intMotionOnly			: 1;	/* 1 if interrupts are only triggered on motion events. */
	mpu9250GyroFsr_e			cacheGyroFsr			: 2;	/* -------------------------------------------------- */
	mpu9250AccelFsr_e			cacheAccelFsr			: 2;	/* When entering motion interrupt mode, the driver keeps track of the */
	mpu9250DlpfCfg_e			cacheLpf				: 3;	/* previous state so that it can be restored at a later time. */
	uint16_t					cacheSampleRate;				/* TODO: This is tacky. Fix it */
	uint8_t						cacheSensors;					/* - */
	uint8_t						cacheFifoSensors;				/* PS: The never fixed it! */
	bool_t						cacheDmpOn				: 1;	/* -------------------------------------------------- */
	bool_t						activeLowInt			: 1;	/* 1 for active low interrupts. */
	bool_t						latchedInt				: 1;	/* 1 for latched interrupts. */
	bool_t						dmpOn					: 1;	/* 1 if DMP is enabled. */
	uint8_t						dmpLoaded;						/* Ensures that DMP will only be loaded once. */
	uint16_t					dmpSampleRate;					/* Sampling rate used when DMP is enabled. */
	uint16_t					compassSampleRate;				/* Compass sample rate. */
	uint8_t						compassAddress;
	int16_t						magSensAdj[3];
	uint32_t					selfTestGyroSens;
	uint32_t					selfTestAccelSens;
	uint8_t						selfTestRegRateDiv;
	uint8_t						selfTestRegLpf;
	uint8_t						selfTestRegGyroFsr;
	uint8_t						selfTestRegAccelFsr;
	uint16_t					selfTestWaitMs;
	uint8_t						selfTestPacketThresh;
	float						selfTestMinDps;
	float						selfTestMaxDps;
	float						selfTestMaxGyroVar;
	float						selfTestMinG;
	float						selfTestMaxG;
	float						selfTestMaxAccelVar;
	mpu9250ErrorCodes_e			error					: 5;
	uint8_t						unusedBits				: 4;
} mpu9250_s;

extern mpu9250_s * device;
/*
bool_t _mpu9250ActivateSensor(mpu9250_s * device, mpu9250SensorFlags_e sensorFlags);
bool_t _mpu9250DectivateSensor(mpu9250_s * device, mpu9250SensorFlags_e sensorFlags);
bool_t _mpu9250Init(mpu9250_s * device, mpu9250TwiSlaveAddress_e slaveAddress);
bool_t _mpu9250LowPowerAccelMode(mpu9250_s * device, uint16_t sampleRate);
bool_t mpu9250SetAccelFsr(mpu9250_s * device, mpu9250AccelFsr_e accelFsr);
bool_t _mpu9250SetCompassSampleRate(mpu9250_s * device, uint16_t sampleRate);
bool_t mpu9250SetDlpf(mpu9250_s * device, mpu9250DlpfCfg_e dlfpCfg);
bool_t mpu9250SetFifo(mpu9250_s * device, uint8_t fifo);
bool_t mpu9250SetGyroFsr(mpu9250_s * device, mpu9250GyroFsr_e gyroFsr);
bool_t _mpu9250SetIntLatched(mpu9250_s * device, bool_t enable);
bool_t _mpu9250SetSampleRate(mpu9250_s * device, uint16_t sampleRate);
bool_t mpu9250SetupCompass(mpu9250_s * device);
*/

#endif
