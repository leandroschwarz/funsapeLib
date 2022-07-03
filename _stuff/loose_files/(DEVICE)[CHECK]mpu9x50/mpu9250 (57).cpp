/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	mpu9250.cpp
 * Module:		MPU9250 module interface for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1002
 * Date:		June 20, 2018
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "mpu9250.hpp"
#if __MPU9250_HPP != 1002
#	error [mpu9250.cpp] Error 102 - Build mismatch on header and source code files.
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define MPU9250_SLAVE_ADDRESS_NORMAL	0x68
#define MPU9250_SLAVE_ADDRESS_ALTERNATE	0x69
#define MPU9250_WHO_AM_I				0x71

#define AXIS_Z_INDEX					2
#define AXIS_Y_INDEX					1
#define AXIS_X_INDEX					0

// -----------------------------------------------------------------------------
// MPU9250 internal registers and bit names
//
// In compliance with InvenSense official documentation:
// MPU-9250 Register Map and Description - Revision 1.6 (01/07/2015)
// -----------------------------------------------------------------------------

// Gyroscope Self-Test Registers
//	Operations:		Read/Write
//	Reset Value:	0x00
#define MPU9250_REG_GYRO_X_SELF_TEST			0x00
#define MPU9250_REG_GYRO_Y_SELF_TEST			0x01
#define MPU9250_REG_GYRO_Z_SELF_TEST			0x02

// Accelerometer Self-Test Registers
//	Operations:		Read/Write
//	Reset Value:	0x00
#define MPU9250_REG_ACCEL_X_SELF_TEST			0x0D
#define MPU9250_REG_ACCEL_Y_SELF_TEST			0x0E
#define MPU9250_REG_ACCEL_Z_SELF_TEST			0x0F

// Gyro Offset Registers
//	Operations:		Read/Write
//	Reset Value:	0x00
#define MPU9250_REG_GYRO_XH_OFFSET				0x13
#define MPU9250_REG_GYRO_XL_OFFSET				0x14
#define MPU9250_REG_GYRO_YH_OFFSET				0x15
#define MPU9250_REG_GYRO_YL_OFFSET				0x16
#define MPU9250_REG_GYRO_ZH_OFFSET				0x17
#define MPU9250_REG_GYRO_ZL_OFFSET				0x18

// Sample Rate Divider
//	Operations:		Read/Write
//	Reset Value:	0x00
#define MPU9250_REG_SMPLRT_DIV					0x19

// Configuration
//	Operations:		Read/Write
//	Reset Value:	0x00
#define MPU9250_REG_CONFIG						0x1A
#	define MPU9250_BIT_CONFIG_FIFO_MODE			6
#	define MPU9250_BIT_CONFIG_EXT_SYNC_SET0		3
#	define MPU9250_BIT_CONFIG_GYRO_DLPF_CFG0	0


#define MPU9250_REG_GYRO_CONFIG					0x1B
#	define MPU9250_BIT_GYRO_CONFIG_Z_ST_EN		7
#	define MPU9250_BIT_GYRO_CONFIG_Y_ST_EN		6
#	define MPU9250_BIT_GYRO_CONFIG_X_ST_EN		5
#	define MPU9250_BIT_GYRO_CONFIG_FS_SEL0		2
#	define MPU9250_BIT_GYRO_CONFIG_FCHOICE_B0	0
#define MPU9250_REG_ACCEL_CONFIG				0x1C
#	define MPU9250_BIT_ACCEL_CONFIG_Z_ST_EN		7
#	define MPU9250_BIT_ACCEL_CONFIG_Y_ST_EN		6
#	define MPU9250_BIT_ACCEL_CONFIG_X_ST_EN		5
#	define MPU9250_BIT_ACCEL_CONFIG_FS_SEL0		3
#define MPU9250_REG_ACCEL_CONFIG2				0x1D
#	define MPU9250_BIT_ACCEL_CONFIG2_FCHOICE_B0	2
#	define MPU9250_BIT_ACCEL_CONFIG2_DLPF_CFG0	0
#define MPU9250_REG_I2C_SLV0_ADDR				0x25
#define MPU9250_REG_I2C_SLV0_REG				0x26
#define MPU9250_REG_I2C_SLV0_CTRL				0x27
#	define MPU9250_BIT_I2C_SLV0_EN				7
#	define MPU9250_BIT_I2C_SLV0_BYTE_SW			6
#	define MPU9250_BIT_I2C_SLV0_REG_DIS			5
#	define MPU9250_BIT_I2C_SLV0_GRP				4
#	define MPU9250_BIT_I2C_SLV0_LENG0			0
#define MPU9250_REG_I2C_SLV1_ADDR				0x28
#define MPU9250_REG_I2C_SLV1_REG				0x29
#define MPU9250_REG_I2C_SLV1_CTRL				0x2A
#	define MPU9250_BIT_I2C_SLV1_EN				7
#	define MPU9250_BIT_I2C_SLV1_BYTE_SW			6
#	define MPU9250_BIT_I2C_SLV1_REG_DIS			5
#	define MPU9250_BIT_I2C_SLV1_GRP				4
#	define MPU9250_BIT_I2C_SLV1_LENG0			0
#define MPU9250_REG_I2C_SLV2_ADDR				0x2B
#define MPU9250_REG_I2C_SLV2_REG				0x2C
#define MPU9250_REG_I2C_SLV2_CTRL				0x2D
#	define MPU9250_BIT_I2C_SLV2_EN				7
#	define MPU9250_BIT_I2C_SLV2_BYTE_SW			6
#	define MPU9250_BIT_I2C_SLV2_REG_DIS			5
#	define MPU9250_BIT_I2C_SLV2_GRP				4
#	define MPU9250_BIT_I2C_SLV2_LENG0			0
#define MPU9250_REG_I2C_SLV3_ADDR				0x2E
#define MPU9250_REG_I2C_SLV3_REG				0x2F
#define MPU9250_REG_I2C_SLV3_CTRL				0x30
#	define MPU9250_BIT_I2C_SLV3_EN				7
#	define MPU9250_BIT_I2C_SLV3_BYTE_SW			6
#	define MPU9250_BIT_I2C_SLV3_REG_DIS			5
#	define MPU9250_BIT_I2C_SLV3_GRP				4
#	define MPU9250_BIT_I2C_SLV3_LENG0			0
#define MPU9250_REG_I2C_SLV4_ADDR				0x31
#define MPU9250_REG_I2C_SLV4_REG				0x32
#define MPU9250_REG_I2C_SLV4_DO					0x33
#define MPU9250_REG_I2C_SLV4_CTRL				0x34
#	define MPU9250_BIT_I2C_SLV4_EN				7
#	define MPU9250_BIT_I2C_SLV4_INT_EN			6
#	define MPU9250_BIT_I2C_SLV4_REG_DIS			5
#	define MPU9250_BIT_I2C_SLV4_DELAY			0
#define MPU9250_REG_ACCEL_XOUT_H				0x3B
#define MPU9250_REG_ACCEL_XOUT_L				0x3C
#define MPU9250_REG_ACCEL_YOUT_H				0x3D
#define MPU9250_REG_ACCEL_YOUT_L				0x3E
#define MPU9250_REG_ACCEL_ZOUT_H				0x3F
#define MPU9250_REG_ACCEL_ZOUT_L				0x40
#define MPU9250_REG_TEMP_OUT_H					0x41
#define MPU9250_REG_TEMP_OUT_L					0x42
#define MPU9250_REG_GYRO_XOUT_H					0x43
#define MPU9250_REG_GYRO_XOUT_L					0x44
#define MPU9250_REG_GYRO_YOUT_H					0x45
#define MPU9250_REG_GYRO_YOUT_L					0x46
#define MPU9250_REG_GYRO_ZOUT_H					0x47
#define MPU9250_REG_GYRO_ZOUT_L					0x48
#define MPU9250_REG_EXT_SENS_DATA_00			0x49
#define MPU9250_REG_EXT_SENS_DATA_01			0x4A
#define MPU9250_REG_EXT_SENS_DATA_02			0x4B
#define MPU9250_REG_EXT_SENS_DATA_03			0x4C
#define MPU9250_REG_EXT_SENS_DATA_04			0x4D
#define MPU9250_REG_EXT_SENS_DATA_05			0x4E
#define MPU9250_REG_EXT_SENS_DATA_06			0x4F
#define MPU9250_REG_EXT_SENS_DATA_07			0x50
#define MPU9250_REG_EXT_SENS_DATA_08			0x51
#define MPU9250_REG_EXT_SENS_DATA_09			0x52
#define MPU9250_REG_EXT_SENS_DATA_10			0x53
#define MPU9250_REG_EXT_SENS_DATA_11			0x54
#define MPU9250_REG_EXT_SENS_DATA_12			0x55
#define MPU9250_REG_EXT_SENS_DATA_13			0x56
#define MPU9250_REG_EXT_SENS_DATA_14			0x57
#define MPU9250_REG_EXT_SENS_DATA_15			0x58
#define MPU9250_REG_EXT_SENS_DATA_16			0x59
#define MPU9250_REG_EXT_SENS_DATA_17			0x5A
#define MPU9250_REG_EXT_SENS_DATA_18			0x5B
#define MPU9250_REG_EXT_SENS_DATA_19			0x5C
#define MPU9250_REG_EXT_SENS_DATA_20			0x5D
#define MPU9250_REG_EXT_SENS_DATA_21			0x5E
#define MPU9250_REG_EXT_SENS_DATA_22			0x5F
#define MPU9250_REG_EXT_SENS_DATA_23			0x60
#define MPU9250_REG_I2C_SLV0_DO					0x63
#define MPU9250_REG_I2C_SLV1_DO					0x64
#define MPU9250_REG_I2C_SLV2_DO					0x65
#define MPU9250_REG_I2C_SLV3_DO					0x66
#define MPU9250_REG_SIGNAL_PATH_RESET			0x68
#	define MPU9250_BIT_SIG_PAT_RST_GYRO			2
#	define MPU9250_BIT_SIG_PAT_RST_ACCEL		1
#	define MPU9250_BIT_SIG_PAT_RST_TEMP			0
#define MPU9250_REG_MOT_DETECT_CTRL				0x69
#define MPU9250_REG_USER_CTRL					0x6A
#	define MPU9250_BIT_USR_CTRL_FIFO_EN			6
#	define MPU9250_BIT_USR_CTRL_I2C_MST_EN		5
#	define MPU9250_BIT_USR_CTRL_I2C_IF_DIS		4
#	define MPU9250_BIT_USR_CTRL_FIFO_RST		2
#	define MPU9250_BIT_USR_CTRL_I2C_MST_RST		1
#	define MPU9250_BIT_USR_CTRL_SIG_COND_RST	0
#define MPU9250_REG_PWR_MGMT_1					0x6B
#	define MPU9250_BIT_PWR_MGMT_1_H_RESET		7
#	define MPU9250_BIT_PWR_MGMT_1_SLEEP			6
#	define MPU9250_BIT_PWR_MGMT_1_CYCLE			5
#	define MPU9250_BIT_PWR_MGMT_1_GYRO_STANDBY	4
#	define MPU9250_BIT_PWR_MGMT_1_PD_PTAT		3
#	define MPU9250_BIT_PWR_MGMT_1_CLKSEL0		0
#define MPU9250_REG_PWR_MGMT_2					0x6C
#	define MPU9250_BIT_PWR_MGMT_2_DIS_XA		5
#	define MPU9250_BIT_PWR_MGMT_2_DIS_YA		4
#	define MPU9250_BIT_PWR_MGMT_2_DIS_ZA		3
#	define MPU9250_BIT_PWR_MGMT_2_DIS_XG		2
#	define MPU9250_BIT_PWR_MGMT_2_DIS_YG		1
#	define MPU9250_BIT_PWR_MGMT_2_DIS_ZG		0
#define MPU9250_REG_FIFO_COUNT_H				0x72
#define MPU9250_REG_FIFO_COUNT_L				0x73
#define MPU9250_REG_WHO_AM_I					0x75
#define MPU9250_REG_XA_OFFSET_H					0x77
#define MPU9250_REG_XA_OFFSET_L					0x78
#define MPU9250_REG_YA_OFFSET_H					0x7A
#define MPU9250_REG_YA_OFFSET_L					0x7B
#define MPU9250_REG_ZA_OFFSET_H					0x7D
#define MPU9250_REG_ZA_OFFSET_L					0x7E

/// To do
#define MPU9250_REG_LP_ACCEL_ODR		0x1E
#define MPU9250_REG_WOM_THR				0x1F
#define MPU9250_REG_FIFO_EN				0x23
#define MPU9250_REG_I2C_MST_CTRL		0x24
#define MPU9250_REG_I2C_SLV4_DI			0x35
#define MPU9250_REG_I2C_MST_STATUS		0x36
#define MPU9250_REG_INT_PIN_CFG			0x37
#	define MPU9250_BIT_INT_PIN_LEVEL			7
#	define MPU9250_BIT_INT_PIN_DRIVER			6
#	define MPU9250_BIT_INT_PIN_LATCH			5
#	define MPU9250_BIT_INT_PIN_AUTO_RESET		4
#	define MPU9250_BIT_FSYNC_LEVEL				3
#	define MPU9250_BIT_FSYNC_MODE_EN			2
#	define MPU9250_BIT_BYPASS_EN				1
#define MPU9250_REG_INT_ENABLE			0x38
#	define MPU9250_BIT_WAKE_ON_MOTION_EN		6
#	define MPU9250_BIT_FIFO_OVERFLOW_EN			4
#	define MPU9250_BIT_FSYNC_INT_EN				3
#	define MPU9250_BIT_RAW_DATA_READY_EN		0
#define MPU9250_REG_INT_STATUS			0x3A
#	define MPU9250_BIT_INT_STATUS_WOM			6
#	define MPU9250_BIT_INT_STATUS_FIFO_OVERFLOW	4
#	define MPU9250_BIT_INT_STATUS_FSYNC			3
#	define MPU9250_BIT_INT_STATUS_RAW_DATA_RDY	0
#define MPU9250_REG_I2C_MST_DELAY_CTRL	0x67
#define MPU9250_REG_FIFO_R_W			0x74

// =============================================================================
// File exclusive - New data types
// =============================================================================

/* NONE */

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

/* NONE */

// =============================================================================
// Global variables
// =============================================================================

/* NONE */

// =============================================================================
// Class constructors
// =============================================================================

Mpu9250::Mpu9250(void)
{
	// Reset data members	this->lastError_ = ERROR_NONE;
	this->initialized_ = false;
	this->i2cHandler_ = NULL;
	this->i2c7bitAddress_ = 0;
	this->useSpi_ = false;
	this->commPortSet_ = false;
	this->spiHandler_ = NULL;
	this->activateDevice_ = NULL;
	this->deactivateDevice_ = NULL;
	this->resetGyro_ = false;
	this->resetAccel_ = false;
	this->resetTemp_ = false;
	this->resetFifo_ = false;
	this->resetI2c_ = false;
	this->resetSignal_ = false;
	this->accelDisableX_ = false;
	this->accelDisableY_ = false;
	this->accelDisableZ_ = false;
	this->accelDlpfCfg_ = MPU9250_ACCEL_DLPF_218_HZ;
	this->accelFChoiceB_ = false;
	this->accelFullScale_ = MPU9250_AFS_2G;
	this->sampleRateDivider_ = 0;
	this->gyroDisableX_ = false;
	this->gyroDisableY_ = false;
	this->gyroDisableZ_ = false;
	this->gyroDlpfCfg_ = MPU9250_GYRO_DLPF_250_HZ;
	this->gyroFChoiceB_ = 0;
	this->gyroFullScale_ = MPU9250_GFS_250DPS;
	this->accelXSelfTestEnabled_ = false;
	this->accelYSelfTestEnabled_ = false;
	this->accelZSelfTestEnabled_ = false;
	this->gyroXSelfTestEnabled_ = false;
	this->gyroYSelfTestEnabled_ = false;
	this->gyroZSelfTestEnabled_ = false;
	for (uint8_t i = 0; i < 3; i++) {
		this->accelSelfTest_[0] = 0;
		this->gyroSelfTest_[0] = 0;
		this->accelData_[0] = 0;
		this->accelOffset_[0] = 0;
		this->gyroData_[0] = 0;
		this->gyroOffset_[0] = 0;
	}
	this->tempData_ = 0;
	this->fifoMode_ = false;
	this->fifoEnable_ = false;
	this->fifoLenght_ = 0;
	this->fifoTempStore_ = false;
	this->fifoGyroXStore_ = false;
	this->fifoGyroyStore_ = false;
	this->fifoGyrozStore_ = false;
	this->fifoAccelStore_ = false;
	this->fifoSlave3Store_ = false;
	this->fifoSlave2Store_ = false;
	this->fifoSlave1Store_ = false;
	this->fifoSlave0Store_ = false;
	this->i2cMultiMaster_ = false;
	this->i2cWaitForExtSensor_ = false;
	this->i2cStopBetweenTransmissions_ = false;
	this->i2cMasterClock_ = 0;
	this->i2cSlave0ReadWrite_ = false;
	this->i2cSlave0Address_ = 0;
	this->i2cSlave1ReadWrite_ = false;
	this->i2cSlave1Address_ = 0;
	this->i2cSlave2ReadWrite_ = false;
	this->i2cSlave2Address_ = 0;
	this->i2cSlave3ReadWrite_ = false;
	this->i2cSlave3Address_ = 0;
	this->i2cSlave4ReadWrite_ = false;
	this->i2cSlave4Address_ = 0;
	this->i2cSlave0Register_ = 0;
	this->i2cSlave1Register_ = 0;
	this->i2cSlave2Register_ = 0;
	this->i2cSlave3Register_ = 0;
	this->i2cSlave4Register_ = 0;
	this->i2cSlave0DataOut_ = 0;
	this->i2cSlave1DataOut_ = 0;
	this->i2cSlave2DataOut_ = 0;
	this->i2cSlave3DataOut_ = 0;
	this->i2cSlave4DataOut_ = 0;
	this->i2cSlave0Enable_ = false;
	this->i2cSlave0SwapBytes_ = false;
	this->i2cSlave0RegisterDisable_ = false;
	this->i2cSlave0GroupingSkipFirst_ = false;
	this->i2cSlave0DataLength_ = 0;
	this->i2cSlave1Enable_ = false;
	this->i2cSlave1SwapBytes_ = false;
	this->i2cSlave1RegisterDisable_ = false;
	this->i2cSlave1GroupingSkipFirst_ = false;
	this->i2cSlave1DataLength_ = 0;
	this->i2cSlave2Enable_ = false;
	this->i2cSlave2SwapBytes_ = false;
	this->i2cSlave2RegisterDisable_ = false;
	this->i2cSlave2GroupingSkipFirst_ = false;
	this->i2cSlave2DataLength_ = 0;
	this->i2cSlave3Enable_ = false;
	this->i2cSlave3SwapBytes_ = false;
	this->i2cSlave3RegisterDisable_ = false;
	this->i2cSlave3GroupingSkipFirst_ = false;
	this->i2cSlave3DataLength_ = 0;
	this->i2cSlave4Enable_ = false;
	this->i2cSlave4IntEnable_ = false;
	this->i2cSlave4RegisterDisable_ = false;
	this->i2cSlave4Delay_ = 0;
	this->i2cBypassEnable_ = false;
	this->i2cExtSensorShadowDelayEnable_ = false;
	this->i2cSlave4DelayEnable_ = false;
	this->i2cSlave3DelayEnable_ = false;
	this->i2cSlave2DelayEnable_ = false;
	this->i2cSlave1DelayEnable_ = false;
	this->i2cSlave0DelayEnable_ = false;
	this->i2cMasterEnable_ = false;
	this->intPinActiveLevel_ = false;
	this->intPinOpenDrain_ = false;
	this->intPinLatchEnable_ = false;
	this->intPinAutoClear_ = false;
	this->intPinFsyncActiveLevel_ = false;
	this->intFsyncModeEnable_ = false;
	this->intWakeOnMotionEnable_ = false;
	this->intFifoOverfowEnable_ = false;
	this->intFsyncEnable_ = false;
	this->intRawDataReadyEnable_ = false;
	this->wakeOnMotionThreshold_ = 0;
	this->wakeOnMotionMode_ = false;
	this->wakeOnMotionEnable_ = false;
	this->cycleEnable_ = false;
	this->sleepEnable_ = false;
	this->gyroStandby_ = false;
	this->ptatDisable_ = false;
	this->clockSelection_ = 0;
	this->lowPowerOscClock_ = 0;
	for (uint8_t i = 0; i < 3; i++) {
		this->extenalSensorData_[i] = 0;
	}
	this->extSyncSet_ = 0;
	this->intWakeOnMotionFlag_ = false;
	this->intFifoOverflowFlag_ = false;
	this->intFsyncFlag_ = false;
	this->intI2cDataReadyFlag_ = false;

	this->lastError_ = ERROR_NONE;
	return;
}

Mpu9250::~Mpu9250(void)
{
	return;
}

// =============================================================================
// Class public methods
// =============================================================================

// -----------------------------------------------------------------------------
// Interface
// -----------------------------------------------------------------------------

bool Mpu9250::initI2c(		Twi0 *i2cHndlr,
							bool useAlternateAddress)
{
	// Check for errors
	if (i2cHndlr == NULL) {						// Handler pointer null
		this->lastError_ = ERROR_HANDLER_POINTER_NULL;
		return false;
	}

	// De-initialize if already initialized
	if (this->initialized_) {
		if (!this->deinitialize_()) {
			return false;
		}
	}

	// Configuring communication port
	this->i2cHandler_ = i2cHndlr;
	if (useAlternateAddress) {
		this->i2c7bitAddress_ = MPU9250_SLAVE_ADDRESS_ALTERNATE;
	} else {
		this->i2c7bitAddress_ = MPU9250_SLAVE_ADDRESS_NORMAL;
	}
	this->useSpi_ = false;
	this->commPortSet_ = true;

	// Initialize
	if (!this->initialize_()) {
		return false;
	}

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::initSpi(		Spi0 *spiHndlr,
							void (*actDevFuncCallback)(void),
							void (*deactDevFuncCallback)(void))
{
	// Check for errors
	if (spiHndlr == NULL) {						// Handler pointer null
		this->lastError_ = ERROR_HANDLER_POINTER_NULL;
		return false;
	}
	if (actDevFuncCallback == NULL) {			// Function pointer null
		this->lastError_ = ERROR_FUNCTION_POINTER_NULL;
		return false;
	}
	if (deactDevFuncCallback == NULL) {			// Function pointer null
		this->lastError_ = ERROR_FUNCTION_POINTER_NULL;
		return false;
	}

	// De-initialize if already initialized
	if (this->initialized_) {
		if (!this->deinitialize_()) {
			return false;
		}
	}

	// Configuring communication port
	this->spiHandler_ = spiHndlr;
	this->useSpi_ = true;
	this->commPortSet_ = true;
	this->activateDevice_ = actDevFuncCallback;
	this->deactivateDevice_ = deactDevFuncCallback;

	// Initialize
	if (!this->initialize_()) {
		return false;
	}

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::restart(void)
{
	uint8_t aux8 = 0;

	// Send reset signal
	aux8 = (1 << MPU9250_BIT_PWR_MGMT_1_H_RESET);
	if (!this->writeAtDevice_(MPU9250_REG_PWR_MGMT_1, &aux8, 1)) {
		return false;
	}
	// Clear all data
	/// TO DO

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::reset(mpu9250ResetPath_f resetPaths)
{
	bool auxBool = false;

	// Signal path reset
	this->resetGyro_ = (resetPaths & MPU9250_RESET_GYRO);
	this->resetAccel_ = (resetPaths & MPU9250_RESET_ACCEL);
	this->resetTemp_ = (resetPaths & MPU9250_RESET_TEMP);
	if (this->resetGyro_ | this->resetAccel_ | this->resetTemp_) {
		auxBool = this->updateSignalPathReset_();
		this->resetGyro_ = false;
		this->resetAccel_ = false;
		this->resetTemp_ = false;
		if (!auxBool) {
			return false;
		}
	}

	// User control
	this->resetFifo_ = (resetPaths & MPU9250_RESET_FIFO);
	this->resetI2c_ = (resetPaths & MPU9250_RESET_I2C_MST);
	this->resetSignal_ = (resetPaths & MPU9250_RESET_SIG_COND);
	if (this->resetFifo_ | this->resetI2c_ | this->resetSignal_) {
		auxBool = this->updateUserControl_();
		this->resetFifo_ = false;
		this->resetI2c_ = false;
		this->resetSignal_ = false;
		if (!auxBool) {
			return false;
		}
	}

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

error_e Mpu9250::getLastError(void)
{
	return this->lastError_;
}

// -----------------------------------------------------------------------------
// Sensor configuration
// -----------------------------------------------------------------------------

bool Mpu9250::configAccel(	axis_t axis,
							mpu9250AccelFullScale_e scale,
							mpu9250AccelSampleRate_e sampleRate,
							mpu9250AccelDigitalFilter_e filter)
{
	// Check for errors
	if (!this->initialized_) {					// Not initialized
		this->lastError_ = ERROR_NOT_INITIALIZED;
		return false;
	}
	if (filter == MPU9250_ACCEL_DLPF_1046_HZ) {	// Invalid selection
		if (sampleRate != MPU9250_ACCEL_SAMPLE_RATE_4_KHZ) {
			this->lastError_ = ERROR_WRONG_CONFIGURATION;
			return false;
		}
	}
	if (sampleRate == MPU9250_ACCEL_SAMPLE_RATE_4_KHZ) {		// Invalid selection
		if (filter != MPU9250_ACCEL_DLPF_1046_HZ) {
			this->lastError_ = ERROR_WRONG_CONFIGURATION;
			return false;
		}
	}

	// Update data members
	if (axis == AXIS_NONE) {					// Accelerometer disabled
		this->accelDisableX_ = true;
		this->accelDisableY_ = true;
		this->accelDisableZ_ = true;
		this->accelFChoiceB_ = true;
		this->accelDlpfCfg_ = 0;
		this->sampleRateDivider_ = 0;
	} else {									// Accelerometer enabled
		this->accelDisableX_ = isBitClr(axis, AXIS_X_INDEX);
		this->accelDisableY_ = isBitClr(axis, AXIS_Y_INDEX);
		this->accelDisableZ_ = isBitClr(axis, AXIS_Z_INDEX);
		if (sampleRate == MPU9250_ACCEL_SAMPLE_RATE_4_KHZ) {
			this->accelFChoiceB_ = true;
			this->accelDlpfCfg_ = 0;
			this->sampleRateDivider_ = 0;
		} else {
			this->accelFChoiceB_ = true;
			this->accelDlpfCfg_ = filter;
			this->sampleRateDivider_ = sampleRate;
		}
	}
	this->accelFullScale_ = scale;

	// Update data into device
	if (!this->updateAccelConfig_()) {
		return false;
	}
	if (!this->updateAccelConfig2_()) {
		return false;
	}
	if (!this->updatePowerManagementControl2_()) {
		return false;
	}
	if (!this->updateSampleRateDivider_()) {
		return false;
	}

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::configGyro(	axis_t axis,
							mpu9250GyroFullScale_e scale,
							mpu9250GyroSampleRate_e sampleRate,
							mpu9250GyroDigitalFilter_e filter)
{
	// Check for errors
	if (!this->initialized_) {					// Not initialized
		this->lastError_ = ERROR_NOT_INITIALIZED;
		return false;
	}

	// Auxiliary members
	uint8_t auxFChoiceB = 0;
	uint8_t auxDlpfCfg = 0;
	uint8_t auxSampleRate = 0;
	switch (filter) {
	case MPU9250_GYRO_DLPF_8800_HZ:
		if (sampleRate != MPU9250_GYRO_SAMPLE_RATE_32_KHZ) {
			this->lastError_ = ERROR_WRONG_CONFIGURATION;
			return false;			// Invalid selection
		}
		auxFChoiceB = 3;
		auxDlpfCfg = 0;
		auxSampleRate = 0;
		break;						// OK
	case MPU9250_GYRO_DLPF_3600_HZ:
		auxSampleRate = 0;
		if (sampleRate == MPU9250_GYRO_SAMPLE_RATE_32_KHZ) {
			auxFChoiceB = 2;
			auxDlpfCfg = 0;
			break;					// OK
		}
		if (sampleRate == MPU9250_GYRO_SAMPLE_RATE_8_KHZ) {
			auxFChoiceB = 0;
			auxDlpfCfg = 7;
			break;					// OK
		}
		this->lastError_ = ERROR_WRONG_CONFIGURATION;
		return false;				// Invalid selection
	case MPU9250_GYRO_DLPF_250_HZ:
		auxFChoiceB = 0;
		auxDlpfCfg = 0;
		if (sampleRate == MPU9250_GYRO_SAMPLE_RATE_32_KHZ) {
			this->lastError_ = ERROR_WRONG_CONFIGURATION;
			return false;			// Invalid selection
		}
		if (sampleRate == MPU9250_GYRO_SAMPLE_RATE_1_KHZ) {
			auxSampleRate = MPU9250_GYRO_SAMPLE_RATE_1_KHZ;
			break;					// OK
		}
		if (sampleRate == MPU9250_GYRO_SAMPLE_RATE_200_HZ) {
			auxSampleRate = MPU9250_GYRO_SAMPLE_RATE_200_HZ;
			break;					// OK
		}
		if (sampleRate == MPU9250_GYRO_SAMPLE_RATE_100_HZ) {
			auxSampleRate = MPU9250_GYRO_SAMPLE_RATE_100_HZ;
			break;					// OK
		}
		if (sampleRate == MPU9250_GYRO_SAMPLE_RATE_40_HZ) {
			auxSampleRate = MPU9250_GYRO_SAMPLE_RATE_40_HZ;
			break;					// OK
		}
		if (sampleRate < 256) {
			this->lastError_ = ERROR_WRONG_CONFIGURATION;
			return false;			// Invalid selection
		}
		auxSampleRate = sampleRate - 256;
		break;
	default:
		auxFChoiceB = 0;
		auxDlpfCfg = filter;
		if (sampleRate > 256) {
			this->lastError_ = ERROR_WRONG_CONFIGURATION;
			return false;			// Invalid selection
		}
		auxSampleRate = sampleRate;
		break;						// OK
	}

	// Update data members
	if (axis == AXIS_NONE) {					// Accelerometer disabled
		this->gyroDisableX_ = true;
		this->gyroDisableY_ = true;
		this->gyroDisableZ_ = true;
		this->gyroFChoiceB_ = 3;
		this->gyroDlpfCfg_ = 0;
		this->sampleRateDivider_ = 0;
	} else {									// Accelerometer enabled
		this->gyroDisableX_ = isBitClr(axis, AXIS_X_INDEX);
		this->gyroDisableY_ = isBitClr(axis, AXIS_Y_INDEX);
		this->gyroDisableZ_ = isBitClr(axis, AXIS_Z_INDEX);
		this->gyroFChoiceB_ = auxFChoiceB;
		this->gyroDlpfCfg_ = auxDlpfCfg;
		this->sampleRateDivider_ = auxSampleRate;
	}
	this->gyroFullScale_ = scale;

	// Update data into device
	if (!this->updateConfig_()) {
		return false;
	}
	if (!this->updateGyroConfig_()) {
		return false;
	}
	if (!this->updateSampleRateDivider_()) {
		return false;
	}
	if (!this->updatePowerManagementControl2_()) {
		return false;
	}

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::performSelfTest(	axis_t accelAxis,
								axis_t gyroAxis)
{
	/*
	bool_t axisSelected = false;

	if (!this->initialized_) {
		return MPU9250_ERROR_NOT_INITIALIZED;
	}

	// Update accelerometer axis
	if (isBitSet(accelAxis, 0)) {
		this->accelXSelfTestEnabled_ = true;
		axisSelected = true;
	} else {
		this->accelXSelfTestEnabled_ = false;
	}
	if (isBitSet(accelAxis, 1)) {
		this->accelYSelfTestEnabled_ = true;
		axisSelected = true;
	} else {
		this->accelYSelfTestEnabled_ = false;
	}
	if (isBitSet(accelAxis, 2)) {
		this->accelZSelfTestEnabled_ = true;
		axisSelected = true;
	} else {
		this->accelZSelfTestEnabled_ = false;
	}

	// Update gyroscope axis
	if (isBitSet(gyroAxis, 0)) {
		this->gyroXSelfTestEnabled_ = true;
		axisSelected = true;
	} else {
		this->gyroXSelfTestEnabled_ = false;
	}
	if (isBitSet(gyroAxis, 1)) {
		this->gyroYSelfTestEnabled_ = true;
		axisSelected = true;
	} else {
		this->gyroYSelfTestEnabled_ = false;
	}
	if (isBitSet(gyroAxis, 2)) {
		this->gyroZSelfTestEnabled_ = true;
		axisSelected = true;
	} else {
		this->gyroZSelfTestEnabled_ = false;
	}

	if (!axisSelected) {		// No axis was selected
		return MPU9250_INVALID_AXIS;
	}

	// DO the rest of stuff
	// (Update registers)

	this->updateGyroConfig();
	this->updateAccelConfig();

	// DO the rest of stuff
	*/

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

// -----------------------------------------------------------------------------
// Raw data management
// -----------------------------------------------------------------------------

bool Mpu9250::getAccelData(	int16_t *accelBuff,
							axis_t axis)
{
	// Check for errors
	if (!this->initialized_) {					// Not initialized
		this->lastError_ = ERROR_NOT_INITIALIZED;
		return false;
	}
	if (accelBuff == NULL) {					// Null pointer
		this->lastError_ = ERROR_BUFFER_POINTER_NULL;
		return false;
	}
	if (axis == AXIS_NONE) {					// No axis
		this->lastError_ = ERROR_INVALID_AXIS;
		return false;
	}

	// Get data from device
	if (!this->getAccelData_()) {
		return false;
	}

	// Update data members
	uint8_t i = 0;
	if (isBitSet(axis, AXIS_X_INDEX)) {
		accelBuff[i++] = this->accelData_[AXIS_X_INDEX];
	}
	if (isBitSet(axis, AXIS_Y_INDEX)) {
		accelBuff[i++] = this->accelData_[AXIS_Y_INDEX];
	}
	if (isBitSet(axis, AXIS_Z_INDEX)) {
		accelBuff[i++] = this->accelData_[AXIS_Z_INDEX];
	}

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::getAccelOffset(	int16_t *accelBuff,
								axis_t axis)
{
	// Check for errors
	if (!this->initialized_) {					// Not initialized
		this->lastError_ = ERROR_NOT_INITIALIZED;
		return false;
	}
	if (accelBuff == NULL) {					// Null pointer
		this->lastError_ = ERROR_BUFFER_POINTER_NULL;
		return false;
	}
	if (axis == AXIS_NONE) {					// No axis
		this->lastError_ = ERROR_INVALID_AXIS;
		return false;
	}

	// Get data from device
	if (!this->getAccelOffset_()) {
		return false;
	}

	// Update data members
	uint8_t i = 0;
	if (isBitSet(axis, AXIS_X_INDEX)) {
		accelBuff[i++] = this->accelOffset_[AXIS_X_INDEX];
	}
	if (isBitSet(axis, AXIS_Y_INDEX)) {
		accelBuff[i++] = this->accelOffset_[AXIS_Y_INDEX];
	}
	if (isBitSet(axis, AXIS_Z_INDEX)) {
		accelBuff[i++] = this->accelOffset_[AXIS_Z_INDEX];
	}

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::setAccelOffset(	int16_t *accelBuff,
								axis_t axis)
{
	// Check for errors
	if (!this->initialized_) {					// Not initialized
		this->lastError_ = ERROR_NOT_INITIALIZED;
		return false;
	}
	if (accelBuff == NULL) {					// Null pointer
		this->lastError_ = ERROR_BUFFER_POINTER_NULL;
		return false;
	}
	if (axis == AXIS_NONE) {					// No axis
		this->lastError_ = ERROR_INVALID_AXIS;
		return false;
	}

	// Set data to device
	uint8_t i = 0;
	if (isBitSet(axis, AXIS_X_INDEX)) {
		this->accelOffset_[AXIS_X_INDEX] = accelBuff[i++];
	}
	if (isBitSet(axis, AXIS_Y_INDEX)) {
		this->accelOffset_[AXIS_Y_INDEX] = accelBuff[i++];
	}
	if (isBitSet(axis, AXIS_Z_INDEX)) {
		this->accelOffset_[AXIS_Z_INDEX] = accelBuff[i++];
	}
	if (!this->updateAccelOffset_()) {
		return false;
	}

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::getGyroData(	int16_t *gyroBuff,
							axis_t axis)
{
	// Check for errors
	if (!this->initialized_) {					// Not initialized
		this->lastError_ = ERROR_NOT_INITIALIZED;
		return false;
	}
	if (gyroBuff == NULL) {						// Null pointer
		this->lastError_ = ERROR_BUFFER_POINTER_NULL;
		return false;
	}
	if (axis == AXIS_NONE) {					// No axis
		this->lastError_ = ERROR_INVALID_AXIS;
		return false;
	}

	// Get data from device
	if (!this->getGyroData_()) {
		return false;
	}

	// Update data members
	uint8_t i = 0;
	if (isBitSet(axis, AXIS_X_INDEX)) {
		gyroBuff[i++] = this->gyroData_[AXIS_X_INDEX];
	}
	if (isBitSet(axis, AXIS_Y_INDEX)) {
		gyroBuff[i++] = this->gyroData_[AXIS_Y_INDEX];
	}
	if (isBitSet(axis, AXIS_Z_INDEX)) {
		gyroBuff[i++] = this->gyroData_[AXIS_Z_INDEX];
	}

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::getGyroOffset(	int16_t *gyroBuff,
								axis_t axis)
{
	// Check for errors
	if (!this->initialized_) {					// Not initialized
		this->lastError_ = ERROR_NOT_INITIALIZED;
		return false;
	}
	if (gyroBuff == NULL) {						// Null pointer
		this->lastError_ = ERROR_BUFFER_POINTER_NULL;
		return false;
	}
	if (axis == AXIS_NONE) {					// No axis
		this->lastError_ = ERROR_INVALID_AXIS;
		return false;
	}

	// Get data from device
	if (!this->getGyroOffset_()) {
		return false;
	}
	uint8_t i = 0;
	if (isBitSet(axis, AXIS_X_INDEX)) {
		gyroBuff[i++] = this->gyroOffset_[AXIS_X_INDEX];
	}
	if (isBitSet(axis, AXIS_Y_INDEX)) {
		gyroBuff[i++] = this->gyroOffset_[AXIS_Y_INDEX];
	}
	if (isBitSet(axis, AXIS_Z_INDEX)) {
		gyroBuff[i++] = this->gyroOffset_[AXIS_Z_INDEX];
	}

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::setGyroOffset(	int16_t *gyroBuff,
								axis_t axis)
{
	// Check for errors
	if (!this->initialized_) {					// Not initialized
		this->lastError_ = ERROR_NOT_INITIALIZED;
		return false;
	}
	if (gyroBuff == NULL) {						// Null pointer
		this->lastError_ = ERROR_BUFFER_POINTER_NULL;
		return false;
	}
	if (axis == AXIS_NONE) {					// No axis
		this->lastError_ = ERROR_INVALID_AXIS;
		return false;
	}

	// Set data to device
	uint8_t i = 0;
	if (isBitSet(axis, AXIS_X_INDEX)) {
		this->gyroOffset_[AXIS_X_INDEX] = gyroBuff[i++];
	}
	if (isBitSet(axis, AXIS_Y_INDEX)) {
		this->gyroOffset_[AXIS_Y_INDEX] = gyroBuff[i++];
	}
	if (isBitSet(axis, AXIS_Z_INDEX)) {
		this->gyroOffset_[AXIS_Z_INDEX] = gyroBuff[i++];
	}
	if (!this->updateGyroOffset_()) {
		return false;
	}

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::getTempData(int16_t *tempValue)
{
	// Check for errors
	if (!this->initialized_) {					// Not initialized
		this->lastError_ = ERROR_NOT_INITIALIZED;
		return false;
	}
	if (tempValue == NULL) {					// Null pointer
		this->lastError_ = ERROR_ARGUMENT_POINTER_NULL;
		return false;
	}

	// Get data from device
	if (!this->getTempData_()) {
		return false;
	}

	// Update data members
	*tempValue = this->tempData_;

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

// -----------------------------------------------------------------------------
// FIFO configuration
// -----------------------------------------------------------------------------

bool Mpu9250::configFifo(	void)
{
	/// To do
	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}






bool Mpu9250::configIntPin(	logicLevel_t pinLevel,
							outputDriver_t pinDriver,
							bool pinLatch,
							bool pinAutoReset)
{
	// Check for errors
	if (!this->initialized_) {					// Not initialized
		this->lastError_ = ERROR_NOT_INITIALIZED;
		return false;
	}

	// Update data members
	this->intPinActiveLevel_ = pinLevel;
	this->intPinOpenDrain_ = pinDriver;
	this->intPinLatchEnable_ = pinLatch;
	this->intPinAutoClear_ = pinAutoReset;
	// Get data from device
	if (!this->updateIntPinConfig_()) {
		return false;
	}

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return false;
}

bool Mpu9250::setIntDataReady(bool enabled)
{
	// Check for errors
	if (!this->initialized_) {					// Not initialized
		this->lastError_ = ERROR_NOT_INITIALIZED;
		return false;
	}

	// Update data members
	this->intRawDataReadyEnable_ = enabled;
	// Get data from device
	if (!this->updateIntEnable_()) {
		return false;
	}

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return false;
}


// =============================================================================
// Class private methods
// =============================================================================

bool Mpu9250::deinitialize_(void)
{
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::initialize_(void)
{
	// Reset flags
	this->initialized_ = false;

	// Device ID
	if (!this->getWhoAmI_()) {
		return false;
	}

	// Interruption
	if (!this->getInterruptStatus_()) {
		return false;
	}

	// Power management control
	if (!this->updatePowerManagementControl1_()) {
		return false;
	}

	// Return successfully
	this->initialized_ = true;
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::getAccelData_(void)
{
	uint8_t buffer[6] = {0, 0, 0, 0, 0, 0};

	if (!this->readFromDevice_(MPU9250_REG_ACCEL_XOUT_H, buffer, 6)) {
		return false;
	}
	this->accelData_[0] = (int16_t)(((uint16_t)buffer[0] << 8) | buffer[1]);
	this->accelData_[1] = (int16_t)(((uint16_t)buffer[2] << 8) | buffer[3]);
	this->accelData_[2] = (int16_t)(((uint16_t)buffer[4] << 8) | buffer[5]);

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::getAccelSelfTest_(void)
{
	uint8_t buffer[3] = {0, 0, 0};

	if (!this->readFromDevice_(MPU9250_REG_ACCEL_X_SELF_TEST, buffer, 3)) {
		return false;
	}
	this->accelSelfTest_[0] = buffer[0];
	this->accelSelfTest_[1] = buffer[1];
	this->accelSelfTest_[2] = buffer[2];

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::getAccelOffset_(void)
{
	uint8_t buffer[6] = {0, 0, 0, 0, 0, 0};

	if (!this->readFromDevice_(MPU9250_REG_XA_OFFSET_H, buffer, 6)) {
		return false;
	}
	this->accelOffset_[0] = (int16_t)(((int16_t)buffer[0] << 8) | buffer[1]) >> 1;
	this->accelOffset_[1] = (int16_t)(((int16_t)buffer[2] << 8) | buffer[3]) >> 1;
	this->accelOffset_[2] = (int16_t)(((int16_t)buffer[4] << 8) | buffer[5]) >> 1;

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::getWhoAmI_(void)
{
	// Local variables
	uint8_t whoAmI = 0x00;

	// Reads Device ID
	if (!this->readFromDevice_(MPU9250_REG_WHO_AM_I, &whoAmI, 1)) {
		return false;
	}

	// Checks response
	if (whoAmI != MPU9250_WHO_AM_I) {
		this->lastError_ = ERROR_DEVICE_ID_MATCH_FAILED;
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::readExtSensData(uint8_t first, uint8_t total)
{
	uint8_t buffer[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	if (!this->readFromDevice_((MPU9250_REG_EXT_SENS_DATA_00 + first), buffer, (total - first))) {
		return false;
	}

	// Transfer data between buffers
	memcpy(&(this->extenalSensorData_[first]), buffer, total);

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::getFifoCount_(void)
{
	uint8_t buffer[2] = {0, 0};

	if (!this->readFromDevice_(MPU9250_REG_FIFO_COUNT_H, buffer, 2)) {
		return false;
	}
	this->fifoLenght_ = (int16_t)(((int16_t)buffer[0] << 8) | buffer[1]) >> 1;

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::getFifoReadWrite_(void)
{
	///
	this->lastError_ = ERROR_NONE;
	return true;
}

bool inline __attribute__((always_inline)) Mpu9250::readFromDevice_(uint8_t reg, uint8_t *buffer, uint8_t size)
{
	// Check for errors
	if (!this->commPortSet_) {
		this->lastError_ = ERROR_COMMUNICATION_PORT_NOT_SET;
		return false;
	}

	// Communicates data
	if (this->useSpi_) {		// Using SPI
		if (!this->spiHandler_->setDefaultDevice(this->activateDevice_, this->deactivateDevice_)) {
			return false;
		}
		if (!this->spiHandler_->sendData((reg | SPI_READ), buffer, size)) {
			return false;
		}
	} else {					// Using I2C
		if (!this->i2cHandler_->sendData(this->i2c7bitAddress_, TWI_READ, reg, buffer, size)) {
			return false;
		}
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::getGyroData_(void)
{
	uint8_t buffer[6] = {0, 0, 0, 0, 0, 0};

	if (!this->readFromDevice_(MPU9250_REG_GYRO_XOUT_H, buffer, 6)) {
		return false;
	}
	this->gyroData_[0] = (int16_t)(((int16_t)buffer[0] << 8) | buffer[1]);
	this->gyroData_[1] = (int16_t)(((int16_t)buffer[2] << 8) | buffer[3]);
	this->gyroData_[2] = (int16_t)(((int16_t)buffer[4] << 8) | buffer[5]);

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::getGyroSelfTest_(void)
{
	uint8_t buffer[3] = {0, 0, 0};

	if (!this->readFromDevice_(MPU9250_REG_GYRO_X_SELF_TEST, buffer, 3)) {
		return false;
	}
	this->gyroSelfTest_[0] = buffer[0];
	this->gyroSelfTest_[1] = buffer[1];
	this->gyroSelfTest_[2] = buffer[2];

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::getGyroOffset_(void)
{
	uint8_t buffer[6] = {0, 0, 0, 0, 0, 0};

	if (!this->readFromDevice_(MPU9250_REG_GYRO_XH_OFFSET, buffer, 6)) {
		return false;
	}
	this->gyroOffset_[0] = (int16_t)(((int16_t)buffer[0] << 8) | buffer[1]);
	this->gyroOffset_[1] = (int16_t)(((int16_t)buffer[2] << 8) | buffer[3]);
	this->gyroOffset_[2] = (int16_t)(((int16_t)buffer[4] << 8) | buffer[5]);

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::getI2cMasterStatus_(void)
{
	///
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::getI2cSlave4DataIn_(void)
{
	///
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::getInterruptStatus_(void)
{
	uint8_t aux8;

	if (!this->readFromDevice_(MPU9250_REG_INT_STATUS, &aux8, 1)) {
		return false;
	}
	this->intWakeOnMotionFlag_ = isBitSet(aux8, MPU9250_BIT_INT_STATUS_WOM);
	this->intFifoOverflowFlag_ = isBitSet(aux8, MPU9250_BIT_INT_STATUS_FIFO_OVERFLOW);
	this->intFsyncFlag_ = isBitSet(aux8, MPU9250_BIT_INT_STATUS_FSYNC);
	this->intI2cDataReadyFlag_ = isBitSet(aux8, MPU9250_BIT_INT_STATUS_RAW_DATA_RDY);

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::getTempData_(void)
{
	uint8_t buffer[2] = {0, 0};

	if (!this->readFromDevice_(MPU9250_REG_TEMP_OUT_H, buffer, 2)) {
		return false;
	}
	this->tempData_ = (int16_t)(((int16_t)buffer[0] << 8) | (int16_t)buffer[1]);

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateAccelConfig_(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->accelXSelfTestEnabled_ << MPU9250_BIT_ACCEL_CONFIG_X_ST_EN;
	aux8 |= this->accelYSelfTestEnabled_ << MPU9250_BIT_ACCEL_CONFIG_Y_ST_EN;
	aux8 |= this->accelZSelfTestEnabled_ << MPU9250_BIT_ACCEL_CONFIG_Z_ST_EN;
	aux8 |= this->accelFullScale_ << MPU9250_BIT_ACCEL_CONFIG_FS_SEL0;
	if (!this->writeAtDevice_(MPU9250_REG_ACCEL_CONFIG, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateAccelConfig2_(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->accelFChoiceB_ << MPU9250_BIT_ACCEL_CONFIG2_FCHOICE_B0;
	aux8 |= this->accelDlpfCfg_ << MPU9250_BIT_ACCEL_CONFIG2_DLPF_CFG0;
	if (!this->writeAtDevice_(MPU9250_REG_ACCEL_CONFIG2, &aux8, 1)) {
		return false;
	}
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateAccelOffset_(void)
{
	uint8_t buffer[6];
	int16_t aux16;

	aux16 = this->accelOffset_[0] << 1;
	buffer[0] = (uint8_t)(aux16 >> 8);
	buffer[1] = (uint8_t)(aux16 & 0xFF);
	aux16 = this->accelOffset_[1] << 1;
	buffer[2] = (uint8_t)(aux16 >> 8);
	buffer[3] = (uint8_t)(aux16 & 0xFF);
	aux16 = this->accelOffset_[2] << 1;
	buffer[4] = (uint8_t)(aux16 >> 8);
	buffer[5] = (uint8_t)(aux16 & 0xFF);
	if (!this->writeAtDevice_(MPU9250_REG_XA_OFFSET_H, buffer, 6)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateConfig_(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->fifoMode_ << MPU9250_BIT_CONFIG_FIFO_MODE;
	aux8 |= this->extSyncSet_ << MPU9250_BIT_CONFIG_EXT_SYNC_SET0;
	aux8 |= this->gyroDlpfCfg_ << MPU9250_BIT_CONFIG_GYRO_DLPF_CFG0;
	if (!this->writeAtDevice_(MPU9250_REG_CONFIG, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateFifoEnable_(void)
{


	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateFifoReadWrite_(void)
{
	///
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateGyroConfig_(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->gyroZSelfTestEnabled_ << MPU9250_BIT_GYRO_CONFIG_Z_ST_EN;
	aux8 |= this->gyroYSelfTestEnabled_ << MPU9250_BIT_GYRO_CONFIG_Y_ST_EN;
	aux8 |= this->gyroXSelfTestEnabled_ << MPU9250_BIT_GYRO_CONFIG_X_ST_EN;
	aux8 |= this->gyroFullScale_ << MPU9250_BIT_GYRO_CONFIG_FS_SEL0;
	aux8 |= (this->gyroFChoiceB_ >> 3) << MPU9250_BIT_GYRO_CONFIG_FCHOICE_B0;
	if (!this->writeAtDevice_(MPU9250_REG_GYRO_CONFIG, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateGyroOffset_(void)
{
	uint8_t buffer[6];

	buffer[0] = (uint8_t)(this->gyroOffset_[0] >> 8);
	buffer[1] = (uint8_t)(this->gyroOffset_[0] & 0xFF);
	buffer[2] = (uint8_t)(this->gyroOffset_[1] >> 8);
	buffer[3] = (uint8_t)(this->gyroOffset_[1] & 0xFF);
	buffer[4] = (uint8_t)(this->gyroOffset_[2] >> 8);
	buffer[5] = (uint8_t)(this->gyroOffset_[2] & 0xFF);
	if (!this->writeAtDevice_(MPU9250_REG_GYRO_XH_OFFSET, buffer, 6)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateI2cMasterControl_(void)
{
	///
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateI2cMasterControlDelay_(void)
{
	///
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateI2cSlave0Address_(void)
{
	uint8_t aux8 = 0;

	aux8 = this->i2cSlave0ReadWrite_ << 7;
	aux8 |= this->i2cSlave0Address_;
	if (!this->writeAtDevice_(MPU9250_REG_I2C_SLV0_ADDR, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateI2cSlave0Control_(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->fifoSlave0Store_ << MPU9250_BIT_I2C_SLV0_EN;
	aux8 |= this->i2cSlave0SwapBytes_ << MPU9250_BIT_I2C_SLV0_BYTE_SW;
	aux8 |= this->i2cSlave0RegisterDisable_ << MPU9250_BIT_I2C_SLV0_REG_DIS;
	aux8 |= this->i2cSlave0GroupingSkipFirst_ << MPU9250_BIT_I2C_SLV0_GRP;
	aux8 |= this->i2cSlave0DataLength_ << MPU9250_BIT_I2C_SLV0_LENG0;
	if (!this->writeAtDevice_(MPU9250_REG_I2C_SLV0_CTRL, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateI2cSlave0DataOut_(void)
{
	uint8_t aux8 = 0;

	aux8 = this->i2cSlave0DataOut_;
	if (!this->writeAtDevice_(MPU9250_REG_I2C_SLV0_DO, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateI2cSlave0Register_(void)
{
	uint8_t aux8 = 0;

	aux8 = this->i2cSlave0Register_;
	if (!this->writeAtDevice_(MPU9250_REG_I2C_SLV0_REG, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateI2cSlave1Address_(void)
{
	uint8_t aux8 = 0;

	aux8 = this->i2cSlave1ReadWrite_ << 7;
	aux8 |= this->i2cSlave1Address_;
	if (!this->writeAtDevice_(MPU9250_REG_I2C_SLV1_ADDR, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateI2cSlave1Control_(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->fifoSlave1Store_ << MPU9250_BIT_I2C_SLV1_EN;
	aux8 |= this->i2cSlave1SwapBytes_ << MPU9250_BIT_I2C_SLV1_BYTE_SW;
	aux8 |= this->i2cSlave1RegisterDisable_ << MPU9250_BIT_I2C_SLV1_REG_DIS;
	aux8 |= this->i2cSlave1GroupingSkipFirst_ << MPU9250_BIT_I2C_SLV1_GRP;
	aux8 |= this->i2cSlave1DataLength_ << MPU9250_BIT_I2C_SLV1_LENG0;
	if (!this->writeAtDevice_(MPU9250_REG_I2C_SLV1_CTRL, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateI2cSlave1DataOut_(void)
{
	uint8_t aux8 = 0;

	aux8 = this->i2cSlave1DataOut_;
	if (!this->writeAtDevice_(MPU9250_REG_I2C_SLV1_DO, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateI2cSlave1Register_(void)
{
	uint8_t aux8 = 0;

	aux8 = this->i2cSlave1Register_;
	if (!this->writeAtDevice_(MPU9250_REG_I2C_SLV1_REG, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateI2cSlave2Address_(void)
{
	uint8_t aux8 = 0;

	aux8 = this->i2cSlave2ReadWrite_ << 7;
	aux8 |= this->i2cSlave2Address_;
	if (!this->writeAtDevice_(MPU9250_REG_I2C_SLV2_ADDR, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateI2cSlave2Control_(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->fifoSlave2Store_ << MPU9250_BIT_I2C_SLV2_EN;
	aux8 |= this->i2cSlave2SwapBytes_ << MPU9250_BIT_I2C_SLV2_BYTE_SW;
	aux8 |= this->i2cSlave2RegisterDisable_ << MPU9250_BIT_I2C_SLV2_REG_DIS;
	aux8 |= this->i2cSlave2GroupingSkipFirst_ << MPU9250_BIT_I2C_SLV2_GRP;
	aux8 |= this->i2cSlave2DataLength_ << MPU9250_BIT_I2C_SLV2_LENG0;
	if (!this->writeAtDevice_(MPU9250_REG_I2C_SLV2_CTRL, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateI2cSlave2DataOut_(void)
{
	uint8_t aux8 = 0;

	aux8 = this->i2cSlave2DataOut_;
	if (!this->writeAtDevice_(MPU9250_REG_I2C_SLV2_DO, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateI2cSlave2Register_(void)
{
	uint8_t aux8 = 0;

	aux8 = this->i2cSlave2Register_;
	if (!this->writeAtDevice_(MPU9250_REG_I2C_SLV2_REG, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateI2cSlave3Address_(void)
{
	uint8_t aux8 = 0;

	aux8 = this->i2cSlave3ReadWrite_ << 7;
	aux8 |= this->i2cSlave3Address_;
	if (!this->writeAtDevice_(MPU9250_REG_I2C_SLV3_ADDR, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateI2cSlave3Control_(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->fifoSlave3Store_ << MPU9250_BIT_I2C_SLV3_EN;
	aux8 |= this->i2cSlave3SwapBytes_ << MPU9250_BIT_I2C_SLV3_BYTE_SW;
	aux8 |= this->i2cSlave3RegisterDisable_ << MPU9250_BIT_I2C_SLV3_REG_DIS;
	aux8 |= this->i2cSlave3GroupingSkipFirst_ << MPU9250_BIT_I2C_SLV3_GRP;
	aux8 |= this->i2cSlave3DataLength_ << MPU9250_BIT_I2C_SLV3_LENG0;
	if (!this->writeAtDevice_(MPU9250_REG_I2C_SLV3_CTRL, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateI2cSlave3DataOut_(void)
{
	uint8_t aux8 = 0;

	aux8 = this->i2cSlave3DataOut_;
	if (!this->writeAtDevice_(MPU9250_REG_I2C_SLV3_DO, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateI2cSlave3Register_(void)
{
	uint8_t aux8 = 0;

	aux8 = this->i2cSlave3Register_;
	if (!this->writeAtDevice_(MPU9250_REG_I2C_SLV3_REG, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateI2cSlave4Address_(void)
{
	uint8_t aux8 = 0;

	aux8 = this->i2cSlave4ReadWrite_ << 7;
	aux8 |= this->i2cSlave4Address_;
	if (!this->writeAtDevice_(MPU9250_REG_I2C_SLV4_ADDR, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateI2cSlave4Control_(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->i2cSlave4Enable_ << MPU9250_BIT_I2C_SLV4_EN;
	aux8 |= this->i2cSlave4IntEnable_ << MPU9250_BIT_I2C_SLV4_INT_EN;
	aux8 |= this->i2cSlave4RegisterDisable_ << MPU9250_BIT_I2C_SLV4_REG_DIS;
	aux8 |= this->i2cSlave4Delay_ << MPU9250_BIT_I2C_SLV4_DELAY;
	if (!this->writeAtDevice_(MPU9250_REG_I2C_SLV4_CTRL, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateI2cSlave4DataOut_(void)
{
	uint8_t aux8 = 0;

	aux8 = this->i2cSlave4DataOut_;
	if (!this->writeAtDevice_(MPU9250_REG_I2C_SLV4_DO, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateI2cSlave4Register_(void)
{
	uint8_t aux8 = 0;

	aux8 = this->i2cSlave4Register_;
	if (!this->writeAtDevice_(MPU9250_REG_I2C_SLV4_REG, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateIntPinConfig_(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->intPinActiveLevel_ << MPU9250_BIT_INT_PIN_LEVEL;
	aux8 |= this->intPinOpenDrain_ << MPU9250_BIT_INT_PIN_DRIVER;
	aux8 |= this->intPinLatchEnable_ << MPU9250_BIT_INT_PIN_LATCH;
	aux8 |= this->intPinAutoClear_ << MPU9250_BIT_INT_PIN_AUTO_RESET;
	aux8 |= this->intPinFsyncActiveLevel_ << MPU9250_BIT_FSYNC_LEVEL;
	aux8 |= this->intFsyncModeEnable_ << MPU9250_BIT_FSYNC_MODE_EN;
	aux8 |= this->i2cBypassEnable_ << MPU9250_BIT_BYPASS_EN;
	if (!this->writeAtDevice_(MPU9250_REG_INT_PIN_CFG, &aux8, 1)) {
		return false;
	}

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateIntEnable_(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->intWakeOnMotionEnable_ << MPU9250_BIT_WAKE_ON_MOTION_EN;
	aux8 |= this->intFifoOverfowEnable_ << MPU9250_BIT_FIFO_OVERFLOW_EN;
	aux8 |= this->intFsyncEnable_ << MPU9250_BIT_FSYNC_INT_EN;
	aux8 |= this->intRawDataReadyEnable_ << MPU9250_BIT_RAW_DATA_READY_EN;
	if (!this->writeAtDevice_(MPU9250_REG_INT_ENABLE, &aux8, 1)) {
		return false;
	}

	// Return successfully
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateLowPowerAccelODR_(void)
{
	///
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateMotionDetectControl_(void)
{
	///
	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updatePowerManagementControl1_(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->sleepEnable_ << MPU9250_BIT_PWR_MGMT_1_SLEEP;
	aux8 |= this->cycleEnable_ << MPU9250_BIT_PWR_MGMT_1_CYCLE;
	aux8 |= this->gyroStandby_ << MPU9250_BIT_PWR_MGMT_1_GYRO_STANDBY;
	aux8 |= this->ptatDisable_ << MPU9250_BIT_PWR_MGMT_1_PD_PTAT;
	aux8 |= this->clockSelection_ << MPU9250_BIT_PWR_MGMT_1_CLKSEL0;
	if (!this->writeAtDevice_(MPU9250_REG_PWR_MGMT_1, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updatePowerManagementControl2_(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->accelDisableX_ << MPU9250_BIT_PWR_MGMT_2_DIS_XA;
	aux8 |= this->accelDisableY_ << MPU9250_BIT_PWR_MGMT_2_DIS_YA;
	aux8 |= this->accelDisableZ_ << MPU9250_BIT_PWR_MGMT_2_DIS_ZA;
	aux8 |= this->gyroDisableX_ << MPU9250_BIT_PWR_MGMT_2_DIS_XG;
	aux8 |= this->gyroDisableY_ << MPU9250_BIT_PWR_MGMT_2_DIS_YG;
	aux8 |= this->gyroDisableZ_ << MPU9250_BIT_PWR_MGMT_2_DIS_ZG;
	if (!this->writeAtDevice_(MPU9250_REG_PWR_MGMT_2, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateSampleRateDivider_(void)
{
	uint8_t aux8 = 0;

	aux8 = this->sampleRateDivider_;
	if (!this->writeAtDevice_(MPU9250_REG_SMPLRT_DIV, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateSignalPathReset_(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->resetGyro_ << MPU9250_BIT_SIG_PAT_RST_GYRO;
	aux8 |= this->resetAccel_ << MPU9250_BIT_SIG_PAT_RST_ACCEL;
	aux8 |= this->resetTemp_ << MPU9250_BIT_SIG_PAT_RST_TEMP;
	this->resetGyro_ = false;
	this->resetAccel_ = false;
	this->resetTemp_ = false;
	if (!this->writeAtDevice_(MPU9250_REG_SIGNAL_PATH_RESET, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateUserControl_(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->fifoEnable_ << MPU9250_BIT_USR_CTRL_FIFO_EN;
	aux8 |= this->i2cMasterEnable_ << MPU9250_BIT_USR_CTRL_I2C_MST_EN;
	aux8 |= this->useSpi_ << MPU9250_BIT_USR_CTRL_I2C_IF_DIS;
	aux8 |= this->resetFifo_ << MPU9250_BIT_USR_CTRL_FIFO_RST;
	aux8 |= this->resetI2c_ << MPU9250_BIT_USR_CTRL_I2C_MST_RST;
	aux8 |= this->resetSignal_ << MPU9250_BIT_USR_CTRL_SIG_COND_RST;
	if (!this->writeAtDevice_(MPU9250_REG_USER_CTRL, &aux8, 1)) {
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Mpu9250::updateWakeUpMotionThreshold_(void)
{
	///
	this->lastError_ = ERROR_NONE;
	return true;
}

bool inline __attribute__((always_inline)) Mpu9250::writeAtDevice_(uint8_t reg, uint8_t *buffer, uint8_t size)
{
	// Check for errors
	if (!this->commPortSet_) {
		this->lastError_ = ERROR_COMMUNICATION_PORT_NOT_SET;
		return false;
	}

	// Communicates data
	if (this->useSpi_) {		// Using SPI
		if (!this->spiHandler_->setDefaultDevice(this->activateDevice_, this->deactivateDevice_)) {
			return false;
		}
		if (!this->spiHandler_->sendData((reg | SPI_WRITE), buffer, size)) {
			return false;
		}
	} else {					// Using I2C
		if (!this->i2cHandler_->sendData(this->i2c7bitAddress_, TWI_WRITE, reg, buffer, size)) {
			return false;
		}
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

// =============================================================================
// Class protected methods
// =============================================================================

/* NONE */
