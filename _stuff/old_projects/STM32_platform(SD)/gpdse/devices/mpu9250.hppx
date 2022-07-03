/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	mpu9250.hpp
 * Module:		MPU9250 interface for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1002
 * Date:		June 06, 2018
 * ========================================================================== */

#ifndef __MPU9250_HPP
#define __MPU9250_HPP	1002

// =============================================================================
// Dependencies
// =============================================================================

// Global definitions file
#include "..\globalDefines.hpp"
#if __GLOBAL_DEFINES_HPP != 1002
#	error	[mpu9250.hpp] Error 101 - Global definitions file (globalDefines.hpp) must be build 1002.
#endif

// Header files
#include <math.h>

// =============================================================================
// Undefining previous definitions
// =============================================================================

/* NONE */

// =============================================================================
// Constants
// =============================================================================


/* NONE */

// =============================================================================
// Macro-functions
// =============================================================================

/* NONE */

// =============================================================================
// New data types
// =============================================================================

typedef enum mpu9250Error_e {
	MPU9250_ERROR_NONE				= 0,
	MPU9250_ERROR_WRONG_WHO_AM_I	= 1,
	MPU9250_ERROR_NOT_INITIALIZED	= 2,
	MPU9250_INVALID_AXIS			= 3,
} mpu9250Error_e;

// =============================================================================
// Mpu9250 - Class declaration
// =============================================================================

class Mpu9250
{
	// New data types
public:
	typedef enum gyroFullScale_e {
		GFS_250DPS	= 0,
		GFS_500DPS,
		GFS_1000DPS,
		GFS_2000DPS
	} gyroFullScale_e;
	typedef enum accelFullScale_e {
		AFS_2G = 0,
		AFS_4G,
		AFS_8G,
		AFS_16G
	} accelFullScale_e;
	typedef enum i2cMasterClock_e {
		I2C_MST_CLK_258_KHz	= 8,
		I2C_MST_CLK_267_KHz	= 7,
		I2C_MST_CLK_276_KHz	= 6,
		I2C_MST_CLK_286_KHz	= 5,
		I2C_MST_CLK_296_KHz	= 4,
		I2C_MST_CLK_308_KHz	= 3,
		I2C_MST_CLK_320_KHz	= 2,
		I2C_MST_CLK_333_KHz	= 1,
		I2C_MST_CLK_348_KHz	= 0,
		I2C_MST_CLK_364_KHz	= 15,
		I2C_MST_CLK_381_KHz	= 14,
		I2C_MST_CLK_400_KHz	= 13,
		I2C_MST_CLK_421_KHz	= 12,
		I2C_MST_CLK_444_KHz	= 11,
		I2C_MST_CLK_471_KHz	= 10,
		I2C_MST_CLK_500_KHz	= 9
	} i2cMasterClock_e;
	typedef enum externalSync_e {
		EXT_SYNC_DISABLED	= 0,
		EXT_SYNC_TEMP,
		EXT_SYNC_GYRO_X,
		EXT_SYNC_GYRO_Y,
		EXT_SYNC_GYRO_Z,
		EXT_SYNC_ACCEL_X,
		EXT_SYNC_ACCEL_Y,
		EXT_SYNC_ACCEL_Z
	} externalSync_e;
	typedef enum gyroSamplingConfig_e {
		GYRO_SAMP_32_KHZ_FILTER_8800_HZ		= 8,
		GYRO_SAMP_32_KHZ_FILTER_3600_HZ		= 16,
		GYRO_SAMP_8_KHZ_FILTER_250_HZ		= 0,
		GYRO_SAMP_1_KHZ_FILTER_184_HZ		= 1,
		GYRO_SAMP_1_KHZ_FILTER_92_HZ		= 2,
		GYRO_SAMP_1_KHZ_FILTER_41_HZ		= 3,
		GYRO_SAMP_1_KHZ_FILTER_20_HZ		= 4,
		GYRO_SAMP_1_KHZ_FILTER_10_HZ		= 5,
		GYRO_SAMP_1_KHZ_FILTER_5_HZ			= 6,
		GYRO_SAMP_8_KHZ_FILTER_3600_HZ		= 7,
	} gyroSamplingConfig_e;
	typedef enum accelSamplingConfig_e {
		ACCEL_SAMP_4_KHZ_FILTER_1046_HZ		= 0,
		ACCEL_SAMP_1_KHZ_FILTER_218_HZ		= 8,
		ACCEL_SAMP_1_KHZ_FILTER_99_HZ		= 10,
		ACCEL_SAMP_1_KHZ_FILTER_45_HZ		= 11,
		ACCEL_SAMP_1_KHZ_FILTER_21_HZ		= 12,
		ACCEL_SAMP_1_KHZ_FILTER_10_HZ		= 13,
		ACCEL_SAMP_1_KHZ_FILTER_5_HZ		= 14,
		ACCEL_SAMP_1_KHZ_FILTER_420_HZ		= 15,
	} accelSamplingConfig_e;
	typedef enum accelSamplingLowPower_e {
		ACCEL_SAMP_LOW_POWER_0P24_HZ		= 0,
		ACCEL_SAMP_LOW_POWER_0P49_HZ		= 1,
		ACCEL_SAMP_LOW_POWER_0P98_HZ		= 2,
		ACCEL_SAMP_LOW_POWER_1P95_HZ		= 3,
		ACCEL_SAMP_LOW_POWER_3P91_HZ		= 4,
		ACCEL_SAMP_LOW_POWER_7P81_HZ		= 5,
		ACCEL_SAMP_LOW_POWER_15P63_HZ		= 6,
		ACCEL_SAMP_LOW_POWER_31P25_HZ		= 7,
		ACCEL_SAMP_LOW_POWER_62P50_HZ		= 8,
		ACCEL_SAMP_LOW_POWER_125_HZ			= 9,
		ACCEL_SAMP_LOW_POWER_250_HZ			= 10,
		ACCEL_SAMP_LOW_POWER_500_HZ			= 11,
	} accelSamplingLowPower_e;
	typedef enum powerMode_e {
		POWER_MODE_NORMAL		= 0,
		POWER_MODE_SLEEP		= 1,
		POWER_MODE_CYCLE		= 2,
		POWER_MODE_STANDBY		= 4,
	} powerMode_e;
	typedef enum clockSelection_e {
		CLOCK_20_MHZ_OSCILLATOR		= 0,
		CLOCK_AUTO_SELECT_BEST		= 1,
		CLOCK_DISABLED				= 2,
	} clockSelection_e;

	// Constructors
public:
	Mpu9250(i2cHandler *i2cHand, bool alternateAddress = false);

	// Methods
public:

	mpu9250Error_e initialize(void);
	mpu9250Error_e setGyroScale(gyroFullScale_e scale);
	mpu9250Error_e setAccelScale(accelFullScale_e scale);
	mpu9250Error_e performSelfTest(axis_t accelAxis, axis_t gyroAxis);
	mpu9250Error_e getAccelData(int16_t *accelBuff, axis_t axis = AXIS_ALL);
	mpu9250Error_e getGyroData(int16_t *gyroBuff, axis_t axis = AXIS_ALL);
	mpu9250Error_e getTempData(int16_t *temp);
	mpu9250Error_e getAccelOffset(int16_t *accelBuff, axis_t axis = AXIS_ALL);
	mpu9250Error_e getGyroOffset(int16_t *gyroBuff, axis_t axis = AXIS_ALL);
	mpu9250Error_e setAccelOffset(int16_t *accelBuff, axis_t axis = AXIS_ALL);
	mpu9250Error_e setGyroOffset(int16_t *gyroBuff, axis_t axis = AXIS_ALL);
//	void readMag(int16_t * magBuff);
#if __DEBUG_MODE == 1
	void readAll(uint8_t *buffer, uint8_t *size);
#endif

private:
	void readAccelFactorySelfTest(void);
	void readGyroFactorySelfTest(void);
	void readAccelOffset(void);
	void readGyroOffset(void);
	void updateAccelOffset(void);
	void updateGyroOffset(void);
	void readAccelData(void);
	void readGyroData(void);
	void readTempData(void);
	void updateAccelConfig(void);
	void updateGyroConfig(void);
	void readIntStatus(void);
	void updatePowerManagementControl1(void);


protected:
	void updateSampleRateDivider(void);
	void updateConfig(void);
	void updateAccelConfig2(void);
	void updateLowPowerAccelODR(void);
	void updateWakeUpMotionThreshold(void);
	void updateFifoEnable(void);
	void updateI2cMasterControl(void);
	void updateI2cSlave0Address(void);
	void updateI2cSlave0Register(void);
	void updateI2cSlave0Control(void);
	void updateI2cSlave1Address(void);
	void updateI2cSlave1Register(void);
	void updateI2cSlave1Control(void);
	void updateI2cSlave2Address(void);
	void updateI2cSlave2Register(void);
	void updateI2cSlave2Control(void);
	void updateI2cSlave3Address(void);
	void updateI2cSlave3Register(void);
	void updateI2cSlave3Control(void);
	void updateI2cSlave4Address(void);
	void updateI2cSlave4Register(void);
	void updateI2cSlave4Control(void);
	void updateI2cSlave4DataOut(void);
	void readI2cSlave4DataIn(void);
	void readI2cMasterStatus(void);
	void updateIntPinConfig(void);
	void updateIntEnable(void);
	void readExtSensData(uint8_t first, uint8_t total);
	void updateI2cSlave0DataOut(void);
	void updateI2cSlave1DataOut(void);
	void updateI2cSlave2DataOut(void);
	void updateI2cSlave3DataOut(void);
	void updateI2cMasterControlDelay(void);
	void updateSignalPathReset(void);
	void updateMotionDetectControl(void);
	void updateUserControl(void);
	void updatePowerManagementControl2(void);
	void readFifoCount(void);
	void readFifoReadWrite(void);
	void updateFifoReadWrite(void);

// Data members
public:
private:
	// General data members
	i2cHandler				*i2cHandler_;
	uint8_t					i2cMyAddress_;
	bool					initialized_						: 1;
	powerMode_e				powerMode_							: 3;
	bool					powerDownMode_						: 1;
	clockSelection_e		clockSelection_						: 2;
	// Accelerometer data members
	bool					accelXSelfTestEnabled_				: 1;
	bool					accelYSelfTestEnabled_				: 1;
	bool					accelZSelfTestEnabled_				: 1;
	accelFullScale_e		accelFullScale_						: 2;
	uint8_t					accelFactorySelfTest_[3];
	int16_t					accelData_[3];
	uint16_t				accelOffset_[3];
	// Gyroscope data members
	uint16_t				gyroOffset_[3];
	int16_t					gyroData_[3];
	uint8_t					gyroFactorySelfTest_[3];
	bool					gyroXSelfTestEnabled_				: 1;
	bool					gyroYSelfTestEnabled_				: 1;
	bool					gyroZSelfTestEnabled_				: 1;
	gyroFullScale_e			gyroFullScale_						: 2;
	gyroSamplingConfig_e	gyroSamplingConfig_					: 5;
	// Temperature data members
	int16_t					tempData_;
	// External sensor data members
	// FIFO data members
	// I2C master bus data members
	// Interrupts
	bool					intWakeOnMotionFlag_				: 1;
	bool					intFifoOverflowFlag_				: 1;
	bool					intFsyncFlag_						: 1;
	bool					intI2cDataReadyFlag_				: 1;
	// Interrupt pin
	// Fsync pin
protected:

}; // class Template

// =============================================================================
// Template - Class inline function definitions
// =============================================================================

/* NONE */

// =============================================================================
// Extern global variables
// =============================================================================

/* NONE */

// =============================================================================
// General public functions declarations
// =============================================================================

/* NONE */

// =============================================================================
// General inline functions definitions
// =============================================================================

/* NONE */

#endif // __MPU9250_HPP
