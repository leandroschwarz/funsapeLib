/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		mpu9250.hpp
 * Module:			MPU9250 module interface for FunSAPE++ Embedded Library
 * 					project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __MPU9250_HPP
#define __MPU9250_HPP							__BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#	error	[mpu9250.hpp] Error 9 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[mpu9250.hpp] Error 10 - Build mismatch between (mpu9250.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "../util/funsapeLibHandler.hpp"
#ifndef __FUNSAPE_LIB_HANDLER_HPP
#	error	[mpu9250.hpp] Error 1 - Header file (funsapeLibHandler.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_HANDLER_HPP != __BUILD_TO_REPLACE__
#	error	[mpu9250.hpp] Error 2 - Build mismatch between (mpu9250.hpp) and (funsapeLibHandler.hpp) header files!
#endif
#include "../util/funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#	error	[mpu9250.hpp] Error 1 - Header file (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_TO_REPLACE__
#	error	[mpu9250.hpp] Error 2 - Build mismatch between (mpu9250.hpp) and (funsapeLibDebug.hpp) header files!
#endif

// =============================================================================
// Undefining previous definitions
// =============================================================================

// NONE

// =============================================================================
// Constant definitions
// =============================================================================

// #define SPI_SPEED_STANDARD				100000
// #define SPI_SPEED_FAST					400000
// #define SPI_BUFFER_LENGTH				32

// =============================================================================
// Macro-function definitions
// *INDENT-OFF*
// =============================================================================

// NONE

// *INDENT-ON*

// =============================================================================
// New data types
// =============================================================================

// NONE

// =============================================================================
// Extern global variables
// =============================================================================

// NONE
void showError(uint8_t errorCode);

// =============================================================================
// Mpu9250 - Class declaration
// =============================================================================

class Mpu9250
{
	// -------------------------------------------------------------------------
	// New data types
	// -------------------------------------------------------------------------

public:
	//     //////////////     ACCELEROMETER FULL SCALE     //////////////     //
	enum class AccelFullScale {
		FULL_SCALE_2_G					= 0,
		FULL_SCALE_4_G					= 1,
		FULL_SCALE_8_G					= 2,
		FULL_SCALE_16_G					= 3,
	};

	//     //////////////    ACCELEROMETER SAMPLE RATE     //////////////     //
	enum class AccelSampleRate {
		SAMPLE_RATE_4_KHZ				= 256,
		SAMPLE_RATE_1_KHZ				= 0,
		SAMPLE_RATE_500_HZ				= 1,
		SAMPLE_RATE_250_HZ				= 3,
		SAMPLE_RATE_200_HZ				= 4,
		SAMPLE_RATE_125_HZ				= 7,
		SAMPLE_RATE_100_HZ				= 9,
		SAMPLE_RATE_50_HZ				= 19,
		SAMPLE_RATE_40_HZ				= 24,
		SAMPLE_RATE_25_HZ				= 39,
		SAMPLE_RATE_20_HZ				= 49,
		SAMPLE_RATE_12P5_HZ				= 79,
		SAMPLE_RATE_10_HZ				= 99,
		SAMPLE_RATE_8_HZ				= 124,
		SAMPLE_RATE_5_HZ				= 199,
		SAMPLE_RATE_4_HZ				= 249,
		SAMPLE_RATE_3P9_HZ				= 255,
	};

	//     ///////     ACCELEROMETER DIGITAL FILTER FREQUENCY     ///////     //
	enum class AccelDigitalFilter {
		FILTER_FREQ_218_HZ				= 1,
		FILTER_FREQ_99_HZ				= 2,
		FILTER_FREQ_45_HZ				= 3,
		FILTER_FREQ_21_HZ				= 4,
		FILTER_FREQ_10_HZ				= 5,
		FILTER_FREQ_5_HZ				= 6,
		FILTER_FREQ_420_HZ				= 7,
		FILTER_FREQ_1046_HZ				= 8,
	};

	//     ////////////////     GYROSCOPE FULL SCALE     ////////////////     //
	enum class GyroFullScale {
		FULL_SCALE_250_DPS				= 0,
		FULL_SCALE_500_DPS				= 1,
		FULL_SCALE_1000_DPS				= 2,
		FULL_SCALE_2000_DPS				= 3,
	};

	//     /////////     GYROSCOPE DIGITAL FILTER FREQUENCY     /////////     //
	enum class GyroDigitalFilter {
		FILTER_FREQ_250_HZ				= 0,
		FILTER_FREQ_184_HZ				= 1,
		FILTER_FREQ_92_HZ				= 2,
		FILTER_FREQ_41_HZ				= 3,
		FILTER_FREQ_20_HZ				= 4,
		FILTER_FREQ_10_HZ				= 5,
		FILTER_FREQ_5_HZ				= 6,
		FILTER_FREQ_3600_HZ				= 7,
		FILTER_FREQ_8800_HZ				= 8,
	};

	//     ////////////////    GYROSCOPE SAMPLE RATE     ////////////////     //
	enum class GyroSampleRate {
		SAMPLE_RATE_32_KHZ				= 512,
		SAMPLE_RATE_8_KHZ				= 256,
		SAMPLE_RATE_4_KHZ				= 257,
		SAMPLE_RATE_2_KHZ				= 259,
		SAMPLE_RATE_1P6_KHZ				= 260,
		SAMPLE_RATE_1_KHZ				= 0,
		SAMPLE_RATE_800_HZ				= 265,
		SAMPLE_RATE_500_HZ				= 1,
		SAMPLE_RATE_400_HZ				= 275,
		SAMPLE_RATE_320_HZ				= 281,
		SAMPLE_RATE_250_HZ				= 3,
		SAMPLE_RATE_200_HZ				= 4,
		SAMPLE_RATE_160_HZ				= 305,
		SAMPLE_RATE_125_HZ				= 7,
		SAMPLE_RATE_100_HZ				= 9,
		SAMPLE_RATE_80_HZ				= 355,
		SAMPLE_RATE_64_HZ				= 380,
		SAMPLE_RATE_50_HZ				= 19,
		SAMPLE_RATE_40_HZ				= 24,
		SAMPLE_RATE_32_HZ				= 505,
		SAMPLE_RATE_31P25_HZ			= 511,
		SAMPLE_RATE_25_HZ				= 39,
		SAMPLE_RATE_20_HZ				= 49,
		SAMPLE_RATE_12P5_HZ				= 79,
		SAMPLE_RATE_10_HZ				= 99,
		SAMPLE_RATE_8_HZ				= 124,
		SAMPLE_RATE_5_HZ				= 199,
		SAMPLE_RATE_4_HZ				= 249,
		SAMPLE_RATE_3P9_HZ				= 255,
	};

	//     //////////////////     I2C MASTER CLOCK     //////////////////     //
	enum class I2cMasterClock {
		I2C_MASTER_CLOCK_258_KHz		= 8,
		I2C_MASTER_CLOCK_267_KHz		= 7,
		I2C_MASTER_CLOCK_276_KHz		= 6,
		I2C_MASTER_CLOCK_286_KHz		= 5,
		I2C_MASTER_CLOCK_296_KHz		= 4,
		I2C_MASTER_CLOCK_308_KHz		= 3,
		I2C_MASTER_CLOCK_320_KHz		= 2,
		I2C_MASTER_CLOCK_333_KHz		= 1,
		I2C_MASTER_CLOCK_348_KHz		= 0,
		I2C_MASTER_CLOCK_364_KHz		= 15,
		I2C_MASTER_CLOCK_381_KHz		= 14,
		I2C_MASTER_CLOCK_400_KHz		= 13,
		I2C_MASTER_CLOCK_421_KHz		= 12,
		I2C_MASTER_CLOCK_444_KHz		= 11,
		I2C_MASTER_CLOCK_471_KHz		= 10,
		I2C_MASTER_CLOCK_500_KHz		= 9
	};

	//     ////////////////     EXTERNAL SYNCHRONISM     ////////////////     //
	enum class ExternalSync {
		EXTERNAL_SYNC_DISABLED			= 0,
		EXTERNAL_SYNC_TEMP				= 1,
		EXTERNAL_SYNC_GYRO_X			= 2,
		EXTERNAL_SYNC_GYRO_Y			= 3,
		EXTERNAL_SYNC_GYRO_Z			= 4,
		EXTERNAL_SYNC_ACCEL_X			= 5,
		EXTERNAL_SYNC_ACCEL_Y			= 6,
		EXTERNAL_SYNC_ACCEL_Z			= 7,
	};

	//     //////////////////     RESET PATH FLAGS     //////////////////     //
	enum class ResetFlags {
		RESET_GYRO						= 0x01,
		RESET_ACCEL						= 0x02,
		RESET_TEMP						= 0x04,
		RESET_FIFO						= 0x08,
		RESET_I2C_MST					= 0x10,
		RESET_SIG_COND					= 0x20,
		RESET_ALL						= 0x3F,
	};

	//     /////////    ACCELEROMETER LOW POWER SAMPLE RATE     /////////     //
	enum class AccelLowPowerSampleRate {
		SAMPLE_RATE_0P24_HZ				= 0,
		SAMPLE_RATE_0P49_HZ				= 1,
		SAMPLE_RATE_0P98_HZ				= 2,
		SAMPLE_RATE_1P95_HZ				= 3,
		SAMPLE_RATE_3P91_HZ				= 4,
		SAMPLE_RATE_7P81_HZ				= 5,
		SAMPLE_RATE_15P63_HZ			= 6,
		SAMPLE_RATE_31P25_HZ			= 7,
		SAMPLE_RATE_62P50_HZ			= 8,
		SAMPLE_RATE_125_HZ				= 9,
		SAMPLE_RATE_250_HZ				= 10,
		SAMPLE_RATE_500_HZ				= 11,
	};

	//     /////////////////////     POWER MODE     /////////////////////     //
	enum class PowerMode {
		UNDEFINED						= 0,
		SLEEP							= 1,
		STANDBY							= 2,
		LOW_POWER_ACCEL					= 3,
		LOW_NOISE_ACCEL					= 4,
		GYRO							= 5,
		MAG								= 6,
		ACCEL_GYRO						= 7,
		ACCEL_MAG						= 8,
		ACCEL_GYRO_MAG					= 9,
	};

	//     ///////////////////    CLOCK SELECTION     ///////////////////     //
	enum class ClockSelection {
		CLOCK_20_MHZ_OSCILLATOR			= 0,
		CLOCK_AUTO_SELECT_BEST			= 1,
		CLOCK_DISABLED					= 2,
	};

private:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Operators overloading
	// -------------------------------------------------------------------------

public:
	// NONE

private:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Constructors
	// -------------------------------------------------------------------------

public:
	Mpu9250(void);
	~Mpu9250(void);

	// -------------------------------------------------------------------------
	// Methods - Inherited methods
	// -------------------------------------------------------------------------

public:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Methods - Class own methods
	// -------------------------------------------------------------------------

public:
	//     ///////////////////     INITIALIZATION     ///////////////////     //
	bool init(
			Handler *twiHandler_p,								// Communication handler
			bool useAlternateAddress_p = false					// Use TWI alternate device address
	);
	bool init(
			Handler *spiHandler_p,								// Communication handler
			void (*actDevFuncCallback_p)(void),					// Funcion pointer to enable device in SPI mode
			void (*deactDevFuncCallback_p)(void)				// Funcion pointer to disable device in SPI mode
	);
	bool reset(
			ResetFlags reset_p									// TODO: Describe parameter
	);
	// bool restart(void);





private:
	//     ///////////////////////     STATUS     ///////////////////////     //
	void _clearData(void);
	// bool _isInitialized(void);
	bool _read(
			uint8_t regAddr_p,
			uint8_t *dataBuffer_p,
			uint16_t bytesToRead_p
	);
	bool _write(
			uint8_t regAddr_p,
			uint8_t *dataBuffer_p,
			uint16_t bytesToWrite_p
	);

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Properties
	// -------------------------------------------------------------------------

public:
	// NONE

private:
	//     /////////////////     PERIPHERAL HANDLER     /////////////////     //
	Handler		*_handler;
	uint8_t		_i2c7bitAddress					: 7;
	void	(* _activateDevice)(void);
	void	(* _deactivateDevice)(void);
	bool		_useSpi							: 1;
	bool		_commPortSet					: 1;

	//     /////////////////     CONTROL AND STATUS     /////////////////     //
	bool		_initialized					: 1;
	Error		_lastError;

	//     ////////////////     SENSOR CONFIGURATION     ////////////////     //
	bool		_accelDisableX					: 1;
	bool		_accelDisableY					: 1;
	bool		_accelDisableZ					: 1;
	AccelDigitalFilter	_accelDlpfCfg;
	bool		_accelFChoiceB					: 1;
	AccelFullScale	_accelFullScale;
	uint8_t		_sampleRateDivider;
	bool		_gyroDisableX					: 1;
	bool		_gyroDisableY					: 1;
	bool		_gyroDisableZ					: 1;
	GyroDigitalFilter		_gyroDlpfCfg;
	uint8_t		_gyroFChoiceB					: 2;
	GyroFullScale		_gyroFullScale;
	bool		_accelXSelfTestEnabled			: 1;	// TODO: Implement thgis parameter
	bool		_accelYSelfTestEnabled			: 1;	// TODO: Implement thgis parameter
	bool		_accelZSelfTestEnabled			: 1;	// TODO: Implement thgis parameter
	uint8_t		_accelSelfTest[3];						// TODO: Implement thgis parameter
	bool		_gyroXSelfTestEnabled			: 1;	// TODO: Implement thgis parameter
	bool		_gyroYSelfTestEnabled			: 1;	// TODO: Implement thgis parameter
	bool		_gyroZSelfTestEnabled			: 1;	// TODO: Implement thgis parameter
	uint8_t		_gyroSelfTest[3];						// TODO: Implement thgis parameter

	//     /////////////////    RAW DATA MANAGEMENT     /////////////////     //
	int16_t		_accelData[3];
	uint16_t	_accelOffset[3];
	int16_t		_gyroData[3];
	uint16_t	_gyroOffset[3];
	int16_t		_tempData;

	//     /////////////////     FIFO CONFIGURATION     /////////////////     //
	bool		_fifoMode						: 1;
	bool		_fifoEnable						: 1;
	uint16_t	_fifoLenght;
	bool		_fifoTempStore					: 1;
	bool		_fifoGyroXStore					: 1;
	bool		_fifoGyroYStore					: 1;
	bool		_fifoGyroZStore					: 1;
	bool		_fifoAccelStore					: 1;
	bool		_fifoSlave3Store				: 1;
	bool		_fifoSlave2Store				: 1;
	bool		_fifoSlave1Store				: 1;
	bool		_fifoSlave0Store				: 1;

	//     /////////////    I2C MASTER BUS DATA MEMBERS     /////////////     //
	bool		_i2cMultiMaster					: 1;
	bool		_i2cWaitForExtSensor			: 1;
	bool		_i2cStopBetweenTransmissions	: 1;
	bool		_i2cMasterClock					: 4;
	bool		_i2cSlave0ReadWrite				: 1;
	uint8_t		_i2cSlave0Address				: 7;
	bool		_i2cSlave1ReadWrite				: 1;
	uint8_t		_i2cSlave1Address				: 7;
	bool		_i2cSlave2ReadWrite				: 1;
	uint8_t		_i2cSlave2Address				: 7;
	bool		_i2cSlave3ReadWrite				: 1;
	uint8_t		_i2cSlave3Address				: 7;
	bool		_i2cSlave4ReadWrite				: 1;
	uint8_t		_i2cSlave4Address				: 7;
	uint8_t		_i2cSlave0Register;
	uint8_t		_i2cSlave1Register;
	uint8_t		_i2cSlave2Register;
	uint8_t		_i2cSlave3Register;
	uint8_t		_i2cSlave4Register;
	uint8_t		_i2cSlave0DataOut;
	uint8_t		_i2cSlave1DataOut;
	uint8_t		_i2cSlave2DataOut;
	uint8_t		_i2cSlave3DataOut;
	uint8_t		_i2cSlave4DataOut;
	bool		_i2cSlave0Enable				: 1;
	bool		_i2cSlave0SwapBytes				: 1;
	bool		_i2cSlave0RegisterDisable		: 1;
	bool		_i2cSlave0GroupingSkipFirst		: 1;
	uint8_t		_i2cSlave0DataLength			: 4;
	bool		_i2cSlave1Enable				: 1;
	bool		_i2cSlave1SwapBytes				: 1;
	bool		_i2cSlave1RegisterDisable		: 1;
	bool		_i2cSlave1GroupingSkipFirst		: 1;
	uint8_t		_i2cSlave1DataLength			: 4;
	bool		_i2cSlave2Enable				: 1;
	bool		_i2cSlave2SwapBytes				: 1;
	bool		_i2cSlave2RegisterDisable		: 1;
	bool		_i2cSlave2GroupingSkipFirst		: 1;
	uint8_t		_i2cSlave2DataLength			: 4;
	bool		_i2cSlave3Enable				: 1;
	bool		_i2cSlave3SwapBytes				: 1;
	bool		_i2cSlave3RegisterDisable		: 1;
	bool		_i2cSlave3GroupingSkipFirst		: 1;
	uint8_t		_i2cSlave3DataLength			: 4;
	bool		_i2cSlave4Enable				: 1;
	bool		_i2cSlave4IntEnable				: 1;
	bool		_i2cSlave4RegisterDisable		: 1;
	uint8_t		_i2cSlave4Delay					: 5;
	bool		_i2cBypassEnable				: 1;
	bool		_i2cExtSensorShadowDelayEnable	: 1;
	bool		_i2cSlave4DelayEnable			: 1;
	bool		_i2cSlave3DelayEnable			: 1;
	bool		_i2cSlave2DelayEnable			: 1;
	bool		_i2cSlave1DelayEnable			: 1;
	bool		_i2cSlave0DelayEnable			: 1;
	bool		_i2cMasterEnable				: 1;

	//     ///////////////    INTERRUPT CONFIGURATION     ///////////////     //
	bool		_intPinActiveLevel				: 1;
	bool		_intPinOpenDrain				: 1;
	bool		_intPinLatchEnable				: 1;
	bool		_intPinAutoClear				: 1;
	bool		_intPinFsyncActiveLevel			: 1;
	bool		_intFsyncModeEnable				: 1;
	bool		_intWakeOnMotionEnable			: 1;
	bool		_intFifoOverfowEnable			: 1;
	bool		_intFsyncEnable					: 1;
	bool		_intRawDataReadyEnable			: 1;

	//     //////////////////     INTERRUPT STATUS     //////////////////     //
	bool		_intWakeOnMotionFlag			: 1;
	bool		_intFifoOverflowFlag			: 1;
	bool		_intFsyncFlag					: 1;
	bool		_intI2cDataReadyFlag			: 1;

	//     //////////////////     MOTION DETECTION     //////////////////     //
	uint8_t		_wakeOnMotionThreshold;
	bool		_wakeOnMotionMode				: 1;
	bool		_wakeOnMotionEnable				: 1;

	//     //////////////////     POWER MANAGEMENT     //////////////////     //
	bool		_cycleEnable					: 1;
	bool		_sleepEnable					: 1;
	bool		_gyroStandby					: 1;
	bool		_ptatDisable					: 1;
	uint8_t		_clockSelection					: 3;
	uint8_t		_lowPowerOscClock				: 4;

	//     /////////////    EXTERNAL SENSOR DATA MEMBER     /////////////     //
	uint8_t		_extenalSensorData[24];
	uint8_t		_extSyncSet						: 3;

	//     /////////////////////    RESET FLAGS     /////////////////////     //
	bool		_resetGyro						: 1;
	bool		_resetAccel						: 1;
	bool		_resetTemp						: 1;
	bool		_resetFifo						: 1;
	bool		_resetI2c						: 1;
	bool		_resetSignal					: 1;

protected:
	// NONE

};


/********                      AQUI                      ********/
/********                      AQUI                      ********/
/********                      AQUI                      ********/
/********                      AQUI                      ********/
/********                      AQUI                      ********/
/********                      AQUI                      ********/

















/*



	// -------------------------------------------------------------------------
	// Methods - Class own methods
	// -------------------------------------------------------------------------

public:

	//     ////////////////////     CHECK STATUS     ////////////////////     //
	Error getLastError(void);

	//     ////////////////     DEVICE CONFIGURATION     ////////////////     //
	bool configAccel(
			Axis axis_p,										// TODO: Describe parameter
			AccelFullScale scale_p,								// TODO: Describe parameter
			AccelSampleRate sampleRate_p,						// TODO: Describe parameter
			AccelDigitalFilter filter_p							// TODO: Describe parameter
	);
	bool configGyro(
			Axis axis_p,										// TODO: Describe parameter
			GyroFullScale scale_p,								// TODO: Describe parameter
			GyroSampleRate sampleRate_p,						// TODO: Describe parameter
			GyroDigitalFilter filter_p							// TODO: Describe parameter
	);
	bool performSelfTest(
			Axis accelAxis_p,									// TODO: Describe parameter
			Axis gyroAxis_p										// TODO: Describe parameter
	);

	//     /////////////////    RAW DATA MANAGEMENT     /////////////////     //
	bool getAccelData(
			int16_t *accelBuff_p,								// TODO: Describe parameter
			Axis axis_p = Axis::ALL								// TODO: Describe parameter
	);
	bool getAccelOffset(
			int16_t *accelBuff_p,								// TODO: Describe parameter
			Axis axis_p = Axis::ALL								// TODO: Describe parameter
	);
	bool setAccelOffset(
			int16_t *accelBuff_p,								// TODO: Describe parameter
			Axis axis_p = Axis::ALL								// TODO: Describe parameter
	);
	bool getGyroData(
			int16_t *gyroBuff_p,								// TODO: Describe parameter
			Axis axis_p = Axis::ALL								// TODO: Describe parameter
	);
	bool getGyroOffset(
			int16_t *gyroBuff_p,								// TODO: Describe parameter
			Axis axis_p = Axis::ALL								// TODO: Describe parameter
	);
	bool setGyroOffset(
			int16_t *gyroBuff_p,								// TODO: Describe parameter
			Axis axis_p = Axis::ALL								// TODO: Describe parameter
	);
	bool getTempData(
			int16_t *temp_p										// TODO: Describe parameter
	);

	//     /////////////////     FIFO CONFIGURATION     /////////////////     //
	bool configFifo(void);										// TODO: Implement this function
	bool configIntPin(
			LogicLevel pinLevel_p = LogicLevel::LOW,			// TODO: Describe parameter
			DriverMode pinDriver_p = DriverMode::PUSH_PULL,		// TODO: Describe parameter
			bool pinLatch_p = false,							// TODO: Describe parameter
			bool pinAutoReset_p = false							// TODO: Describe parameter
	);
	bool setIntDataReady(
			bool enabled_p										// TODO: Describe parameter
	);

	//     ////////////////     OTHER TEST FUNCTIONS     ////////////////     //
	// void readAll(uint8_t *buffer, uint8_t *size);
	// void readMag(int16_t *magBuff);

private:





	//     //////////////////////    INTERFACE     //////////////////////     //
	bool _deinitialize(void);									// TODO: Implement this function
	bool _initialize(void);
	bool _getWhoAmI(void);

	//     ////////////////////    CONFIGURATION     ////////////////////     //
	bool _updateSampleRateDivider(void);						// TODO: Implement this function
	bool _updateAccelConfig(void);								// TODO: Implement this function
	bool _updateAccelConfig2(void);								// TODO: Implement this function
	bool _updatePowerManagementControl2(void);					// TODO: Implement this function
	bool _updateConfig(void);									// TODO: Implement this function
	bool _updateGyroConfig(void);								// TODO: Implement this function
	bool _getAccelSelfTest(void);								// TODO: Implement this function
	bool _getGyroSelfTest(void);								// TODO: Implement this function
	bool _updateUserControl(void);								// TODO: Implement this function

	//     /////////////////    RAW DATA MANAGEMENT     /////////////////     //
	bool _getAccelData(void);									// TODO: Implement this function
	bool _getAccelOffset(void);									// TODO: Implement this function
	bool _getGyroData(void);									// TODO: Implement this function
	bool _getGyroOffset(void);									// TODO: Implement this function
	bool _getTempData(void);									// TODO: Implement this function
	bool _updateAccelOffset(void);								// TODO: Implement this function
	bool _updateGyroOffset(void);								// TODO: Implement this function

	//     ////////////////////////     FIFO     ////////////////////////     //
	bool _updateFifoEnable(void);								// TODO: Implement this function
	bool _getFifoCount(void);									// TODO: Implement this function
	bool _getFifoReadWrite(void);								// TODO: Implement this function
	bool _updateFifoReadWrite(void);							// TODO: Implement this function

	//     ///////////////////     MOTION CONTROL     ///////////////////     //
	bool _updateWakeUpMotionThreshold(void);					// TODO: Implement this function
	bool _updateMotionDetectControl(void);						// TODO: Implement this function

	//     /////////////////////////    I2C     /////////////////////////     //
	bool _updateI2cMasterControl(void);							// TODO: Implement this function
	bool _updateI2cSlave0Address(void);							// TODO: Implement this function
	bool _updateI2cSlave0Control(void);							// TODO: Implement this function
	bool _updateI2cSlave0Register(void);						// TODO: Implement this function
	bool _updateI2cSlave1Address(void);							// TODO: Implement this function
	bool _updateI2cSlave1Control(void);							// TODO: Implement this function
	bool _updateI2cSlave1Register(void);						// TODO: Implement this function
	bool _updateI2cSlave2Address(void);							// TODO: Implement this function
	bool _updateI2cSlave2Control(void);							// TODO: Implement this function
	bool _updateI2cSlave2Register(void);						// TODO: Implement this function
	bool _updateI2cSlave3Address(void);							// TODO: Implement this function
	bool _updateI2cSlave3Control(void);							// TODO: Implement this function
	bool _updateI2cSlave3Register(void);						// TODO: Implement this function
	bool _updateI2cSlave4Address(void);							// TODO: Implement this function
	bool _updateI2cSlave4Control(void);							// TODO: Implement this function
	bool _updateI2cSlave4Register(void);						// TODO: Implement this function
	bool _updateI2cSlave4DataOut(void);							// TODO: Implement this function
	bool _getI2cSlave4DataIn(void);								// TODO: Implement this function
	bool _getI2cMasterStatus(void);								// TODO: Implement this function
	bool _updateI2cMasterControlDelay(void);					// TODO: Implement this function
	bool _updateI2cSlave0DataOut(void);							// TODO: Implement this function
	bool _updateI2cSlave1DataOut(void);							// TODO: Implement this function
	bool _updateI2cSlave2DataOut(void);							// TODO: Implement this function
	bool _updateI2cSlave3DataOut(void);							// TODO: Implement this function

	//     //////////////////////    INTERRUPT     //////////////////////     //
	bool _updateIntPinConfig(void);								// TODO: Implement this function
	bool _updateIntEnable(void);								// TODO: Implement this function
	bool _getInterruptStatus(void);								// TODO: Implement this function

	//     ///////////////////    EXTERNAL SENSOR     ///////////////////     //
	bool _readExtSensData(
			uint8_t first_p,									// TODO: Describe parameter
			uint8_t total_p										// TODO: Describe parameter
	);															// TODO: Implement this function

	//     ////////////////////////    RESET     ////////////////////////     //
	bool _updateSignalPathReset(void);							// TODO: Implement this function
	bool _updatePowerManagementControl1(void);					// TODO: Implement this function

	//     //////////////////////    LOW POWER     //////////////////////     //
	bool _updateLowPowerAccelODR(void);							// TODO: Implement this function

	//     ///////////////    LOW LEVEL COMMUNICATION     ///////////////     //
	bool inlined _readFromDevice(
			uint8_t reg_p,										// TODO: Describe parameter
			uint8_t *buffer_p,									// TODO: Describe parameter
			uint8_t size_p										// TODO: Describe parameter
	);															// TODO: Implement this function
	bool inlined _writeAtDevice(
			uint8_t reg_p,										// TODO: Describe parameter
			uint8_t *buffer_p,									// TODO: Describe parameter
			uint8_t size_p										// TODO: Describe parameter
	);															// TODO: Implement this function

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Properties
	// -------------------------------------------------------------------------

public:
	// NONE

private:

protected:
	// NONE

}; // class Mpu9250

// =============================================================================
// Mpu9250 - Class overloading operators
// =============================================================================

// NONE

// =============================================================================
// Global variables
// =============================================================================

// -----------------------------------------------------------------------------
// Externally defined global variables -----------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Internally defined global variables -----------------------------------------

// NONE

// =============================================================================
// Mpu9250 - Class inline function definitions
// =============================================================================

// NONE

// =============================================================================
// General public functions declarations
// =============================================================================

// NONE

// =============================================================================
// General inline functions definitions
// =============================================================================

// NONE

// =============================================================================
// External default objects
// =============================================================================
*/
// NONE

#endif // __MPU9250_HPP
