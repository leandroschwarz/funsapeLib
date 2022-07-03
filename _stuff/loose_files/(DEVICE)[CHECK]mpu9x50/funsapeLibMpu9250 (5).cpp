/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibMpu9250.cpp
 * Module:			MPU9250 Interface Module for FunSAPE++ Embedded Library
 *					project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// System file dependencies
// =============================================================================

#include "funsapeLibMpu9250.hpp"
#ifndef __FUNSAPE_LIB_MPU9250_HPP
#	error	[funsapeLibMpu9250.cpp] Error 1 - Header file (funsapeLibMpu9250.hpp) must be corrupted!
#elif __FUNSAPE_LIB_MPU9250_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibMpu9250.cpp] Error 9 - Build mismatch between source (funsapeLibMpu9250.cpp) and header (funsapeLibMpu9250.hpp) files!
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define MPU9250_SLAVE_ADDRESS_NORMAL			0x68
#define MPU9250_SLAVE_ADDRESS_ALTERNATE			0x69
#define MPU9250_WHO_AM_I						0x71

#define AXIS_Z_INDEX							2
#define AXIS_Y_INDEX							1
#define AXIS_X_INDEX							0

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

// TODO: Implement these registers
// #define MPU9250_REG_LP_ACCEL_ODR				0x1E
// #define MPU9250_REG_WOM_THR						0x1F
// #define MPU9250_REG_FIFO_EN						0x23
// #define MPU9250_REG_I2C_MST_CTRL				0x24
// #define MPU9250_REG_I2C_SLV4_DI					0x35
// #define MPU9250_REG_I2C_MST_STATUS				0x36
#define MPU9250_REG_INT_PIN_CFG					0x37
#	define MPU9250_BIT_INT_PIN_LEVEL			7
#	define MPU9250_BIT_INT_PIN_DRIVER			6
#	define MPU9250_BIT_INT_PIN_LATCH			5
#	define MPU9250_BIT_INT_PIN_AUTO_RESET		4
#	define MPU9250_BIT_FSYNC_LEVEL				3
#	define MPU9250_BIT_FSYNC_MODE_EN			2
#	define MPU9250_BIT_BYPASS_EN				1
#define MPU9250_REG_INT_ENABLE					0x38
#	define MPU9250_BIT_WAKE_ON_MOTION_EN		6
#	define MPU9250_BIT_FIFO_OVERFLOW_EN			4
#	define MPU9250_BIT_FSYNC_INT_EN				3
#	define MPU9250_BIT_RAW_DATA_READY_EN		0
#define MPU9250_REG_INT_STATUS					0x3A
#	define MPU9250_BIT_INT_STATUS_WOM			6
#	define MPU9250_BIT_INT_STATUS_FIFO_OVERFLOW	4
#	define MPU9250_BIT_INT_STATUS_FSYNC			3
#	define MPU9250_BIT_INT_STATUS_RAW_DATA_RDY	0
// #define MPU9250_REG_I2C_MST_DELAY_CTRL			0x67
// #define MPU9250_REG_FIFO_R_W					0x74

// =============================================================================
// File exclusive - New data types
// =============================================================================

// NONE

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

// NONE

// =============================================================================
// Global variables
// =============================================================================

// NONE

// =============================================================================
// Static functions declarations
// =============================================================================

// NONE

// =============================================================================
// Class constructors
// =============================================================================

Mpu9250::Mpu9250(void)
{
	// -------------------------------------------------------------------------
	// Reset data members
	// -------------------------------------------------------------------------

	// -------------------------------------------------------------------------
	// Interface ---------------------------------------------------------------

	this->_activateDevice						= NULL;
	this->_commPortSet							= false;
	this->_deactivateDevice						= NULL;
	this->_i2c7bitAddress						= 0;
	this->_i2cHandler							= NULL;
	this->_initialized							= false;
	this->_resetAccel							= false;
	this->_resetFifo							= false;
	this->_resetGyro							= false;
	this->_resetI2c								= false;
	this->_resetSignal							= false;
	this->_resetTemp							= false;
	this->_spiHandler							= NULL;
	this->_useSpi								= false;

	// -------------------------------------------------------------------------
	// Sensor Configuration ----------------------------------------------------

	this->_accelDisableX						= false;
	this->_accelDisableY						= false;
	this->_accelDisableZ						= false;
	this->_accelFChoiceB						= false;
	this->_accelXSelfTestEnabled				= false;
	this->_accelYSelfTestEnabled				= false;
	this->_accelZSelfTestEnabled				= false;
	this->_gyroDisableX							= false;
	this->_gyroDisableY							= false;
	this->_gyroDisableZ							= false;
	this->_gyroXSelfTestEnabled					= false;
	this->_gyroYSelfTestEnabled					= false;
	this->_gyroZSelfTestEnabled					= false;
	this->_accelDlpfCfg							= 0;
	this->_accelFullScale						= 0;
	this->_gyroDlpfCfg							= 0;
	this->_gyroFChoiceB							= 0;
	this->_gyroFullScale						= 0;
	this->_sampleRateDivider					= 0;
	for (uint8_t i = 0; i < 3; i++) {
		this->_accelSelfTest[i]					= 0;
		this->_gyroSelfTest[i]					= 0;
	}

	// -------------------------------------------------------------------------
	// Raw Data Management -----------------------------------------------------

	for (uint8_t i = 0; i < 3; i++) {
		this->_accelData[i]						= 0;
		this->_accelOffset[i]					= 0;
		this->_gyroData[i]						= 0;
		this->_gyroOffset[i]					= 0;
	}
	this->_tempData								= 0;

	// -------------------------------------------------------------------------
	// FIFO Configuration ------------------------------------------------------

	this->_fifoMode								= false;
	this->_fifoEnable							= false;
	this->_fifoLenght							= 0;
	this->_fifoTempStore						= false;
	this->_fifoGyroXStore						= false;
	this->_fifoGyroYStore						= false;
	this->_fifoGyroZStore						= false;
	this->_fifoAccelStore						= false;
	this->_fifoSlave3Store						= false;
	this->_fifoSlave2Store						= false;
	this->_fifoSlave1Store						= false;
	this->_fifoSlave0Store						= false;

	// -------------------------------------------------------------------------
	// I2C Master Bus Data Members ---------------------------------------------

	this->_i2cMultiMaster						= false;
	this->_i2cWaitForExtSensor					= false;
	this->_i2cStopBetweenTransmissions			= false;
	this->_i2cMasterClock						= false;
	this->_i2cSlave0ReadWrite					= false;
	this->_i2cSlave0Address						= 0;
	this->_i2cSlave1ReadWrite					= false;
	this->_i2cSlave1Address						= 0;
	this->_i2cSlave2ReadWrite					= false;
	this->_i2cSlave2Address						= 0;
	this->_i2cSlave3ReadWrite					= false;
	this->_i2cSlave3Address						= 0;
	this->_i2cSlave4ReadWrite					= false;
	this->_i2cSlave4Address						= 0;
	this->_i2cSlave0Register					= 0;
	this->_i2cSlave1Register					= 0;
	this->_i2cSlave2Register					= 0;
	this->_i2cSlave3Register					= 0;
	this->_i2cSlave4Register					= 0;
	this->_i2cSlave0DataOut						= 0;
	this->_i2cSlave1DataOut						= 0;
	this->_i2cSlave2DataOut						= 0;
	this->_i2cSlave3DataOut						= 0;
	this->_i2cSlave4DataOut						= 0;
	this->_i2cSlave0Enable						= false;
	this->_i2cSlave0SwapBytes					= false;
	this->_i2cSlave0RegisterDisable				= false;
	this->_i2cSlave0GroupingSkipFirst			= false;
	this->_i2cSlave0DataLength					= 0;
	this->_i2cSlave1Enable						= false;
	this->_i2cSlave1SwapBytes					= false;
	this->_i2cSlave1RegisterDisable				= false;
	this->_i2cSlave1GroupingSkipFirst			= false;
	this->_i2cSlave1DataLength					= 0;
	this->_i2cSlave2Enable						= false;
	this->_i2cSlave2SwapBytes					= false;
	this->_i2cSlave2RegisterDisable				= false;
	this->_i2cSlave2GroupingSkipFirst			= false;
	this->_i2cSlave2DataLength					= 0;
	this->_i2cSlave3Enable						= false;
	this->_i2cSlave3SwapBytes					= false;
	this->_i2cSlave3RegisterDisable				= false;
	this->_i2cSlave3GroupingSkipFirst			= false;
	this->_i2cSlave3DataLength					= 0;
	this->_i2cSlave4Enable						= false;
	this->_i2cSlave4IntEnable					= false;
	this->_i2cSlave4RegisterDisable				= false;
	this->_i2cSlave4Delay						= 0;
	this->_i2cBypassEnable						= false;
	this->_i2cExtSensorShadowDelayEnable		= false;
	this->_i2cSlave4DelayEnable					= false;
	this->_i2cSlave3DelayEnable					= false;
	this->_i2cSlave2DelayEnable					= false;
	this->_i2cSlave1DelayEnable					= false;
	this->_i2cSlave0DelayEnable					= false;
	this->_i2cMasterEnable						= false;

	// -------------------------------------------------------------------------
	// Interrupts --------------------------------------------------------------

	this->_intPinActiveLevel					= false;
	this->_intPinOpenDrain						= false;
	this->_intPinLatchEnable					= false;
	this->_intPinAutoClear						= false;
	this->_intPinFsyncActiveLevel				= false;
	this->_intFsyncModeEnable					= false;
	this->_intWakeOnMotionEnable				= false;
	this->_intFifoOverfowEnable					= false;
	this->_intFsyncEnable						= false;
	this->_intRawDataReadyEnable				= false;
	this->_intWakeOnMotionFlag					= false;
	this->_intFifoOverflowFlag					= false;
	this->_intFsyncFlag							= false;
	this->_intI2cDataReadyFlag					= false;

	// -------------------------------------------------------------------------
	// Motion Detection --------------------------------------------------------

	this->_wakeOnMotionThreshold				= 0;
	this->_wakeOnMotionMode						= false;
	this->_wakeOnMotionEnable					= false;

	// -------------------------------------------------------------------------
	// Power Management --------------------------------------------------------

	this->_cycleEnable							= false;
	this->_sleepEnable							= false;
	this->_gyroStandby							= false;
	this->_ptatDisable							= false;
	this->_clockSelection						= 0;
	this->_lowPowerOscClock						= 0;

	// -------------------------------------------------------------------------
	// External Sensor Data Member ---------------------------------------------

	for (uint8_t i = 0; i < 24; i++) {
		this->_extenalSensorData[i]				= 0;
	}
	this->_extSyncSet							= 0;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return;
}

Mpu9250::~Mpu9250(void)
{
	// TODO: Implement destructor
	return;
}

// =============================================================================
// Class public methods
// =============================================================================

// -----------------------------------------------------------------------------
// Interface
// -----------------------------------------------------------------------------

bool Mpu9250::initialize(I2c *i2cHndlr, bool useAlternateAddress)
{
	// Check for errors
	if (i2cHndlr == NULL) {						// Handler pointer null
		this->_lastError = ERROR_HANDLER_POINTER_NULL;
		return false;
	}

	// De-initialize if already initialized
	if (this->_initialized) {
		if (!this->_deinitialize()) {
			return false;
		}
	}

	// Configuring communication port
	this->_i2cHandler = i2cHndlr;
	if (useAlternateAddress) {
		this->_i2c7bitAddress = MPU9250_SLAVE_ADDRESS_ALTERNATE;
	} else {
		this->_i2c7bitAddress = MPU9250_SLAVE_ADDRESS_NORMAL;
	}
	this->_useSpi = false;
	this->_commPortSet = true;

	// Initialize
	if (!this->_initialize()) {
		return false;
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::initialize(Spi *spiHndlr, void (*actDevFuncCallback)(void), void (*deactDevFuncCallback)(void))
{
	// Check for errors
	if (spiHndlr == NULL) {						// Handler pointer null
		this->_lastError = ERROR_HANDLER_POINTER_NULL;
		return false;
	}
	if (actDevFuncCallback == NULL) {			// Function pointer null
		this->_lastError = ERROR_FUNCTION_POINTER_NULL;
		return false;
	}
	if (deactDevFuncCallback == NULL) {			// Function pointer null
		this->_lastError = ERROR_FUNCTION_POINTER_NULL;
		return false;
	}

	// De-initialize if already initialized
	if (this->_initialized) {
		if (!this->_deinitialize()) {
			return false;
		}
	}

	// Configuring communication port
	this->_spiHandler = spiHndlr;
	this->_useSpi = true;
	this->_commPortSet = true;
	this->_activateDevice = actDevFuncCallback;
	this->_deactivateDevice = deactDevFuncCallback;

	// Initialize
	if (!this->_initialize()) {
		return false;
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::restart(void)
{
	uint8_t aux8 = 0;

	// Send reset signal
	aux8 = (1 << MPU9250_BIT_PWR_MGMT_1_H_RESET);
	if (!this->_writeAtDevice(MPU9250_REG_PWR_MGMT_1, &aux8, 1)) {
		return false;
	}
	// Clear all data
	// TODO: Clear all data

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::reset(mpu9250ResetPath_f resetPaths)
{
	bool auxBool = false;

	// Signal path reset
	this->_resetGyro = (resetPaths & MPU9250_RESET_GYRO);
	this->_resetAccel = (resetPaths & MPU9250_RESET_ACCEL);
	this->_resetTemp = (resetPaths & MPU9250_RESET_TEMP);
	if (this->_resetGyro | this->_resetAccel | this->_resetTemp) {
		auxBool = this->_updateSignalPathReset();
		this->_resetGyro = false;
		this->_resetAccel = false;
		this->_resetTemp = false;
		if (!auxBool) {
			return false;
		}
	}

	// User control
	this->_resetFifo = (resetPaths & MPU9250_RESET_FIFO);
	this->_resetI2c = (resetPaths & MPU9250_RESET_I2C_MST);
	this->_resetSignal = (resetPaths & MPU9250_RESET_SIG_COND);
	if (this->_resetFifo | this->_resetI2c | this->_resetSignal) {
		auxBool = this->_updateUserControl();
		this->_resetFifo = false;
		this->_resetI2c = false;
		this->_resetSignal = false;
		if (!auxBool) {
			return false;
		}
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

error_e Mpu9250::getLastError(void)
{
	// Return last error
	return this->_lastError;
}

// -----------------------------------------------------------------------------
// Sensor configuration
// -----------------------------------------------------------------------------

bool Mpu9250::configAccel(axis_t axis, mpu9250AccelFullScale_e scale, mpu9250AccelSampleRate_e sampleRate,
	mpu9250AccelDigitalFilter_e filter)
{
	// Check for errors
	if (!this->_initialized) {					// Not initialized
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}
	if (filter == MPU9250_ACCEL_DLPF_1046_HZ) {	// Invalid selection
		if (sampleRate != MPU9250_ACCEL_SAMPLE_RATE_4_KHZ) {
			this->_lastError = ERROR_WRONG_CONFIGURATION;
			return false;
		}
	}
	if (sampleRate == MPU9250_ACCEL_SAMPLE_RATE_4_KHZ) {		// Invalid selection
		if (filter != MPU9250_ACCEL_DLPF_1046_HZ) {
			this->_lastError = ERROR_WRONG_CONFIGURATION;
			return false;
		}
	}

	// Update data members
	if (axis == AXIS_NONE) {					// Accelerometer disabled
		this->_accelDisableX = true;
		this->_accelDisableY = true;
		this->_accelDisableZ = true;
		this->_accelFChoiceB = true;
		this->_accelDlpfCfg = 0;
		this->_sampleRateDivider = 0;
	} else {									// Accelerometer enabled
		this->_accelDisableX = isBitClr(axis, AXIS_X_INDEX);
		this->_accelDisableY = isBitClr(axis, AXIS_Y_INDEX);
		this->_accelDisableZ = isBitClr(axis, AXIS_Z_INDEX);
		if (sampleRate == MPU9250_ACCEL_SAMPLE_RATE_4_KHZ) {
			this->_accelFChoiceB = true;
			this->_accelDlpfCfg = 0;
			this->_sampleRateDivider = 0;
		} else {
			this->_accelFChoiceB = true;
			this->_accelDlpfCfg = filter;
			this->_sampleRateDivider = sampleRate;
		}
	}
	this->_accelFullScale = scale;

	// Update data into device
	if (!this->_updateAccelConfig()) {
		return false;
	}
	if (!this->_updateAccelConfig2()) {
		return false;
	}
	if (!this->_updatePowerManagementControl2()) {
		return false;
	}
	if (!this->_updateSampleRateDivider()) {
		return false;
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::configGyro(axis_t axis, mpu9250GyroFullScale_e scale, mpu9250GyroSampleRate_e sampleRate,
	mpu9250GyroDigitalFilter_e filter)
{
	// Check for errors
	if (!this->_initialized) {					// Not initialized
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}

	// Auxiliary members
	uint8_t auxFChoiceB = 0;
	uint8_t auxDlpfCfg = 0;
	uint8_t auxSampleRate = 0;
	switch (filter) {
		case MPU9250_GYRO_DLPF_8800_HZ:
			if (sampleRate != MPU9250_GYRO_SAMPLE_RATE_32_KHZ) {
				this->_lastError = ERROR_WRONG_CONFIGURATION;
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
			this->_lastError = ERROR_WRONG_CONFIGURATION;
			return false;				// Invalid selection
		case MPU9250_GYRO_DLPF_250_HZ:
			auxFChoiceB = 0;
			auxDlpfCfg = 0;
			if (sampleRate == MPU9250_GYRO_SAMPLE_RATE_32_KHZ) {
				this->_lastError = ERROR_WRONG_CONFIGURATION;
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
				this->_lastError = ERROR_WRONG_CONFIGURATION;
				return false;			// Invalid selection
			}
			auxSampleRate = sampleRate - 256;
			break;
		default:
			auxFChoiceB = 0;
			auxDlpfCfg = filter;
			if (sampleRate > 256) {
				this->_lastError = ERROR_WRONG_CONFIGURATION;
				return false;			// Invalid selection
			}
			auxSampleRate = sampleRate;
			break;						// OK
	}

	// Update data members
	if (axis == AXIS_NONE) {					// Accelerometer disabled
		this->_gyroDisableX = true;
		this->_gyroDisableY = true;
		this->_gyroDisableZ = true;
		this->_gyroFChoiceB = 3;
		this->_gyroDlpfCfg = 0;
		this->_sampleRateDivider = 0;
	} else {									// Accelerometer enabled
		this->_gyroDisableX = isBitClr(axis, AXIS_X_INDEX);
		this->_gyroDisableY = isBitClr(axis, AXIS_Y_INDEX);
		this->_gyroDisableZ = isBitClr(axis, AXIS_Z_INDEX);
		this->_gyroFChoiceB = auxFChoiceB;
		this->_gyroDlpfCfg = auxDlpfCfg;
		this->_sampleRateDivider = auxSampleRate;
	}
	this->_gyroFullScale = scale;

	// Update data into device
	if (!this->_updateConfig()) {
		return false;
	}
	if (!this->_updateGyroConfig()) {
		return false;
	}
	if (!this->_updateSampleRateDivider()) {
		return false;
	}
	if (!this->_updatePowerManagementControl2()) {
		return false;
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::performSelfTest(axis_t accelAxis,
	axis_t gyroAxis)
{
	// bool_t axisSelected = false;

	// if (!this->_initialized) {
	// 	return MPU9250_ERROR_NOT_INITIALIZED;
	// }

	// // Update accelerometer axis
	// if (isBitSet(accelAxis, 0)) {
	// 	this->_accelXSelfTestEnabled = true;
	// 	axisSelected = true;
	// } else {
	// 	this->_accelXSelfTestEnabled = false;
	// }
	// if (isBitSet(accelAxis, 1)) {
	// 	this->_accelYSelfTestEnabled = true;
	// 	axisSelected = true;
	// } else {
	// 	this->_accelYSelfTestEnabled = false;
	// }
	// if (isBitSet(accelAxis, 2)) {
	// 	this->_accelZSelfTestEnabled = true;
	// 	axisSelected = true;
	// } else {
	// 	this->_accelZSelfTestEnabled = false;
	// }

	// // Update gyroscope axis
	// if (isBitSet(gyroAxis, 0)) {
	// 	this->_gyroXSelfTestEnabled = true;
	// 	axisSelected = true;
	// } else {
	// 	this->_gyroXSelfTestEnabled = false;
	// }
	// if (isBitSet(gyroAxis, 1)) {
	// 	this->_gyroYSelfTestEnabled = true;
	// 	axisSelected = true;
	// } else {
	// 	this->_gyroYSelfTestEnabled = false;
	// }
	// if (isBitSet(gyroAxis, 2)) {
	// 	this->_gyroZSelfTestEnabled = true;
	// 	axisSelected = true;
	// } else {
	// 	this->_gyroZSelfTestEnabled = false;
	// }

	// if (!axisSelected) {		// No axis was selected
	// 	return MPU9250_INVALID_AXIS;
	// }

	// // DO the rest of stuff
	// // (Update registers)

	// this->updateGyroConfig();
	// this->updateAccelConfig();

	// DO the rest of stuff

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

// -----------------------------------------------------------------------------
// Raw data management
// -----------------------------------------------------------------------------

bool Mpu9250::getAccelData(int16_t *accelBuff, axis_t axis)
{
	// Check for errors
	if (!this->_initialized) {					// Not initialized
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}
	if (accelBuff == NULL) {					// Null pointer
		this->_lastError = ERROR_BUFFER_POINTER_NULL;
		return false;
	}
	if (axis == AXIS_NONE) {					// No axis
		this->_lastError = ERROR_AXIS_INVALID;
		return false;
	}

	// Get data from device
	if (!this->_getAccelData()) {
		return false;
	}

	// Update data members
	uint8_t i = 0;
	if (isBitSet(axis, AXIS_X_INDEX)) {
		accelBuff[i++] = this->_accelData[AXIS_X_INDEX];
	}
	if (isBitSet(axis, AXIS_Y_INDEX)) {
		accelBuff[i++] = this->_accelData[AXIS_Y_INDEX];
	}
	if (isBitSet(axis, AXIS_Z_INDEX)) {
		accelBuff[i++] = this->_accelData[AXIS_Z_INDEX];
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::getAccelOffset(int16_t *accelBuff, axis_t axis)
{
	// Check for errors
	if (!this->_initialized) {					// Not initialized
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}
	if (accelBuff == NULL) {					// Null pointer
		this->_lastError = ERROR_BUFFER_POINTER_NULL;
		return false;
	}
	if (axis == AXIS_NONE) {					// No axis
		this->_lastError = ERROR_AXIS_INVALID;
		return false;
	}

	// Get data from device
	if (!this->_getAccelOffset()) {
		return false;
	}

	// Update data members
	uint8_t i = 0;
	if (isBitSet(axis, AXIS_X_INDEX)) {
		accelBuff[i++] = this->_accelOffset[AXIS_X_INDEX];
	}
	if (isBitSet(axis, AXIS_Y_INDEX)) {
		accelBuff[i++] = this->_accelOffset[AXIS_Y_INDEX];
	}
	if (isBitSet(axis, AXIS_Z_INDEX)) {
		accelBuff[i++] = this->_accelOffset[AXIS_Z_INDEX];
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::setAccelOffset(int16_t *accelBuff, axis_t axis)
{
	// Check for errors
	if (!this->_initialized) {					// Not initialized
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}
	if (accelBuff == NULL) {					// Null pointer
		this->_lastError = ERROR_BUFFER_POINTER_NULL;
		return false;
	}
	if (axis == AXIS_NONE) {					// No axis
		this->_lastError = ERROR_AXIS_INVALID;
		return false;
	}

	// Set data to device
	uint8_t i = 0;
	if (isBitSet(axis, AXIS_X_INDEX)) {
		this->_accelOffset[AXIS_X_INDEX] = accelBuff[i++];
	}
	if (isBitSet(axis, AXIS_Y_INDEX)) {
		this->_accelOffset[AXIS_Y_INDEX] = accelBuff[i++];
	}
	if (isBitSet(axis, AXIS_Z_INDEX)) {
		this->_accelOffset[AXIS_Z_INDEX] = accelBuff[i++];
	}
	if (!this->_updateAccelOffset()) {
		return false;
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::getGyroData(int16_t *gyroBuff, axis_t axis)
{
	// Check for errors
	if (!this->_initialized) {					// Not initialized
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}
	if (gyroBuff == NULL) {						// Null pointer
		this->_lastError = ERROR_BUFFER_POINTER_NULL;
		return false;
	}
	if (axis == AXIS_NONE) {					// No axis
		this->_lastError = ERROR_AXIS_INVALID;
		return false;
	}

	// Get data from device
	if (!this->_getGyroData()) {
		return false;
	}

	// Update data members
	uint8_t i = 0;
	if (isBitSet(axis, AXIS_X_INDEX)) {
		gyroBuff[i++] = this->_gyroData[AXIS_X_INDEX];
	}
	if (isBitSet(axis, AXIS_Y_INDEX)) {
		gyroBuff[i++] = this->_gyroData[AXIS_Y_INDEX];
	}
	if (isBitSet(axis, AXIS_Z_INDEX)) {
		gyroBuff[i++] = this->_gyroData[AXIS_Z_INDEX];
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::getGyroOffset(int16_t *gyroBuff,
	axis_t axis)
{
	// Check for errors
	if (!this->_initialized) {					// Not initialized
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}
	if (gyroBuff == NULL) {						// Null pointer
		this->_lastError = ERROR_BUFFER_POINTER_NULL;
		return false;
	}
	if (axis == AXIS_NONE) {					// No axis
		this->_lastError = ERROR_AXIS_INVALID;
		return false;
	}

	// Get data from device
	if (!this->_getGyroOffset()) {
		return false;
	}
	uint8_t i = 0;
	if (isBitSet(axis, AXIS_X_INDEX)) {
		gyroBuff[i++] = this->_gyroOffset[AXIS_X_INDEX];
	}
	if (isBitSet(axis, AXIS_Y_INDEX)) {
		gyroBuff[i++] = this->_gyroOffset[AXIS_Y_INDEX];
	}
	if (isBitSet(axis, AXIS_Z_INDEX)) {
		gyroBuff[i++] = this->_gyroOffset[AXIS_Z_INDEX];
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::setGyroOffset(int16_t *gyroBuff, axis_t axis)
{
	// Check for errors
	if (!this->_initialized) {					// Not initialized
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}
	if (gyroBuff == NULL) {						// Null pointer
		this->_lastError = ERROR_BUFFER_POINTER_NULL;
		return false;
	}
	if (axis == AXIS_NONE) {					// No axis
		this->_lastError = ERROR_AXIS_INVALID;
		return false;
	}

	// Set data to device
	uint8_t i = 0;
	if (isBitSet(axis, AXIS_X_INDEX)) {
		this->_gyroOffset[AXIS_X_INDEX] = gyroBuff[i++];
	}
	if (isBitSet(axis, AXIS_Y_INDEX)) {
		this->_gyroOffset[AXIS_Y_INDEX] = gyroBuff[i++];
	}
	if (isBitSet(axis, AXIS_Z_INDEX)) {
		this->_gyroOffset[AXIS_Z_INDEX] = gyroBuff[i++];
	}
	if (!this->_updateGyroOffset()) {
		return false;
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::getTempData(int16_t *tempValue)
{
	// Check for errors
	if (!this->_initialized) {					// Not initialized
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}
	if (tempValue == NULL) {					// Null pointer
		this->_lastError = ERROR_ARGUMENT_POINTER_NULL;
		return false;
	}

	// Get data from device
	if (!this->_getTempData()) {
		return false;
	}

	// Update data members
	*tempValue = this->_tempData;

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

// -----------------------------------------------------------------------------
// FIFO configuration
// -----------------------------------------------------------------------------

bool Mpu9250::configFifo(void)
{
	// TODO: Implement function
	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::configIntPin(logicLevel_t pinLevel, outputDriver_t pinDriver, bool pinLatch, bool pinAutoReset)
{
	// Check for errors
	if (!this->_initialized) {					// Not initialized
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}

	// Update data members
	this->_intPinActiveLevel = pinLevel;
	this->_intPinOpenDrain = pinDriver;
	this->_intPinLatchEnable = pinLatch;
	this->_intPinAutoClear = pinAutoReset;
	// Get data from device
	if (!this->_updateIntPinConfig()) {
		return false;
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return false;
}

bool Mpu9250::setIntDataReady(bool enabled)
{
	// Check for errors
	if (!this->_initialized) {					// Not initialized
		this->_lastError = ERROR_NOT_INITIALIZED;
		return false;
	}

	// Update data members
	this->_intRawDataReadyEnable = enabled;
	// Get data from device
	if (!this->_updateIntEnable()) {
		return false;
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return false;
}

// =============================================================================
// Class private methods
// =============================================================================

bool Mpu9250::_deinitialize(void)
{
	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_initialize(void)
{
	// Reset flags
	this->_initialized = false;

	// Device ID
	if (!this->_getWhoAmI()) {
		return false;
	}

	// Interruption
	if (!this->_getInterruptStatus()) {
		return false;
	}

	// Power management control
	if (!this->_updatePowerManagementControl1()) {
		return false;
	}

	// Return successfully
	this->_initialized = true;
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_getWhoAmI(void)
{
	// Local variables
	uint8_t whoAmI = 0x00;

	// Reads Device ID
	if (!this->_readFromDevice(MPU9250_REG_WHO_AM_I, &whoAmI, 1)) {
		return false;
	}

	// Checks response
	if (whoAmI != MPU9250_WHO_AM_I) {
		this->_lastError = ERROR_COMMUNICATION_DEVICE_ID_MATCH_FAILED;
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool inlined Mpu9250::_readFromDevice(uint8_t reg, uint8_t *buffer, uint8_t size)
{
	// Check for errors
	if (!this->_commPortSet) {
		this->_lastError = ERROR_COMMUNICATION_PORT_NOT_SET;
		return false;
	}

	// Communicates data
	if (this->_useSpi) {		// Using SPI
		if (!this->_spiHandler->setDefaultDevice(this->_activateDevice, this->_deactivateDevice)) {
			return false;
		}
		if (!this->_spiHandler->sendData((reg | SPI_READ), buffer, size)) {
			return false;
		}
	} else {					// Using I2C
//		if (!this->_i2cHandler->sendData(this->_i2c7bitAddress, TWI_READ, reg, buffer, size)) {
		return false;
//		}
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool inlined Mpu9250::_writeAtDevice(uint8_t reg, uint8_t *buffer, uint8_t size)
{
	// Check for errors
	if (!this->_commPortSet) {
		this->_lastError = ERROR_COMMUNICATION_PORT_NOT_SET;
		return false;
	}

	// Communicates data
	if (this->_useSpi) {		// Using SPI
		if (!this->_spiHandler->setDefaultDevice(this->_activateDevice, this->_deactivateDevice)) {
			return false;
		}
		if (!this->_spiHandler->sendData((reg | SPI_WRITE), buffer, size)) {
			return false;
		}
	} else {					// Using I2C
//		if (!this->_i2cHandler->sendData(this->_i2c7bitAddress, TWI_WRITE, reg, buffer, size)) {
		return false;
//		}
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_getAccelData(void)
{
	uint8_t buffer[6] = {0, 0, 0, 0, 0, 0};

	if (!this->_readFromDevice(MPU9250_REG_ACCEL_XOUT_H, buffer, 6)) {
		return false;
	}
	this->_accelData[0] = (int16_t)(((uint16_t)buffer[0] << 8) | buffer[1]);
	this->_accelData[1] = (int16_t)(((uint16_t)buffer[2] << 8) | buffer[3]);
	this->_accelData[2] = (int16_t)(((uint16_t)buffer[4] << 8) | buffer[5]);

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_getAccelSelfTest(void)
{
	uint8_t buffer[3] = {0, 0, 0};

	if (!this->_readFromDevice(MPU9250_REG_ACCEL_X_SELF_TEST, buffer, 3)) {
		return false;
	}
	this->_accelSelfTest[0] = buffer[0];
	this->_accelSelfTest[1] = buffer[1];
	this->_accelSelfTest[2] = buffer[2];

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_getAccelOffset(void)
{
	uint8_t buffer[6] = {0, 0, 0, 0, 0, 0};

	if (!this->_readFromDevice(MPU9250_REG_XA_OFFSET_H, buffer, 6)) {
		return false;
	}
	this->_accelOffset[0] = (int16_t)(((int16_t)buffer[0] << 8) | buffer[1]) >> 1;
	this->_accelOffset[1] = (int16_t)(((int16_t)buffer[2] << 8) | buffer[3]) >> 1;
	this->_accelOffset[2] = (int16_t)(((int16_t)buffer[4] << 8) | buffer[5]) >> 1;

	this->_lastError = ERROR_NONE;
	return true;
}


bool Mpu9250::_readExtSensData(uint8_t first, uint8_t total)
{
	uint8_t buffer[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	if (!this->_readFromDevice((MPU9250_REG_EXT_SENS_DATA_00 + first), buffer, (total - first))) {
		return false;
	}

	// Transfer data between buffers
	memcpy(&(this->_extenalSensorData[first]), buffer, total);

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_getFifoCount(void)
{
	uint8_t buffer[2] = {0, 0};

	if (!this->_readFromDevice(MPU9250_REG_FIFO_COUNT_H, buffer, 2)) {
		return false;
	}
	this->_fifoLenght = (int16_t)(((int16_t)buffer[0] << 8) | buffer[1]) >> 1;

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_getFifoReadWrite(void)
{
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_getGyroData(void)
{
	uint8_t buffer[6] = {0, 0, 0, 0, 0, 0};

	if (!this->_readFromDevice(MPU9250_REG_GYRO_XOUT_H, buffer, 6)) {
		return false;
	}
	this->_gyroData[0] = (int16_t)(((int16_t)buffer[0] << 8) | buffer[1]);
	this->_gyroData[1] = (int16_t)(((int16_t)buffer[2] << 8) | buffer[3]);
	this->_gyroData[2] = (int16_t)(((int16_t)buffer[4] << 8) | buffer[5]);

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_getGyroSelfTest(void)
{
	uint8_t buffer[3] = {0, 0, 0};

	if (!this->_readFromDevice(MPU9250_REG_GYRO_X_SELF_TEST, buffer, 3)) {
		return false;
	}
	this->_gyroSelfTest[0] = buffer[0];
	this->_gyroSelfTest[1] = buffer[1];
	this->_gyroSelfTest[2] = buffer[2];

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_getGyroOffset(void)
{
	uint8_t buffer[6] = {0, 0, 0, 0, 0, 0};

	if (!this->_readFromDevice(MPU9250_REG_GYRO_XH_OFFSET, buffer, 6)) {
		return false;
	}
	this->_gyroOffset[0] = (int16_t)(((int16_t)buffer[0] << 8) | buffer[1]);
	this->_gyroOffset[1] = (int16_t)(((int16_t)buffer[2] << 8) | buffer[3]);
	this->_gyroOffset[2] = (int16_t)(((int16_t)buffer[4] << 8) | buffer[5]);

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_getI2cMasterStatus(void)
{
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_getI2cSlave4DataIn(void)
{
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_getInterruptStatus(void)
{
	uint8_t aux8;

	if (!this->_readFromDevice(MPU9250_REG_INT_STATUS, &aux8, 1)) {
		return false;
	}
	this->_intWakeOnMotionFlag = isBitSet(aux8, MPU9250_BIT_INT_STATUS_WOM);
	this->_intFifoOverflowFlag = isBitSet(aux8, MPU9250_BIT_INT_STATUS_FIFO_OVERFLOW);
	this->_intFsyncFlag = isBitSet(aux8, MPU9250_BIT_INT_STATUS_FSYNC);
	this->_intI2cDataReadyFlag = isBitSet(aux8, MPU9250_BIT_INT_STATUS_RAW_DATA_RDY);

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_getTempData(void)
{
	uint8_t buffer[2] = {0, 0};

	if (!this->_readFromDevice(MPU9250_REG_TEMP_OUT_H, buffer, 2)) {
		return false;
	}
	this->_tempData = (int16_t)(((int16_t)buffer[0] << 8) | (int16_t)buffer[1]);

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateAccelConfig(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->_accelXSelfTestEnabled << MPU9250_BIT_ACCEL_CONFIG_X_ST_EN;
	aux8 |= this->_accelYSelfTestEnabled << MPU9250_BIT_ACCEL_CONFIG_Y_ST_EN;
	aux8 |= this->_accelZSelfTestEnabled << MPU9250_BIT_ACCEL_CONFIG_Z_ST_EN;
	aux8 |= this->_accelFullScale << MPU9250_BIT_ACCEL_CONFIG_FS_SEL0;
	if (!this->_writeAtDevice(MPU9250_REG_ACCEL_CONFIG, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateAccelConfig2(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->_accelFChoiceB << MPU9250_BIT_ACCEL_CONFIG2_FCHOICE_B0;
	aux8 |= this->_accelDlpfCfg << MPU9250_BIT_ACCEL_CONFIG2_DLPF_CFG0;
	if (!this->_writeAtDevice(MPU9250_REG_ACCEL_CONFIG2, &aux8, 1)) {
		return false;
	}
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateAccelOffset(void)
{
	uint8_t buffer[6];
	int16_t aux16;

	aux16 = this->_accelOffset[0] << 1;
	buffer[0] = (uint8_t)(aux16 >> 8);
	buffer[1] = (uint8_t)(aux16 & 0xFF);
	aux16 = this->_accelOffset[1] << 1;
	buffer[2] = (uint8_t)(aux16 >> 8);
	buffer[3] = (uint8_t)(aux16 & 0xFF);
	aux16 = this->_accelOffset[2] << 1;
	buffer[4] = (uint8_t)(aux16 >> 8);
	buffer[5] = (uint8_t)(aux16 & 0xFF);
	if (!this->_writeAtDevice(MPU9250_REG_XA_OFFSET_H, buffer, 6)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateConfig(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->_fifoMode << MPU9250_BIT_CONFIG_FIFO_MODE;
	aux8 |= this->_extSyncSet << MPU9250_BIT_CONFIG_EXT_SYNC_SET0;
	aux8 |= this->_gyroDlpfCfg << MPU9250_BIT_CONFIG_GYRO_DLPF_CFG0;
	if (!this->_writeAtDevice(MPU9250_REG_CONFIG, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateFifoEnable(void)
{
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateFifoReadWrite(void)
{
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateGyroConfig(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->_gyroZSelfTestEnabled << MPU9250_BIT_GYRO_CONFIG_Z_ST_EN;
	aux8 |= this->_gyroYSelfTestEnabled << MPU9250_BIT_GYRO_CONFIG_Y_ST_EN;
	aux8 |= this->_gyroXSelfTestEnabled << MPU9250_BIT_GYRO_CONFIG_X_ST_EN;
	aux8 |= this->_gyroFullScale << MPU9250_BIT_GYRO_CONFIG_FS_SEL0;
	aux8 |= (this->_gyroFChoiceB >> 3) << MPU9250_BIT_GYRO_CONFIG_FCHOICE_B0;
	if (!this->_writeAtDevice(MPU9250_REG_GYRO_CONFIG, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateGyroOffset(void)
{
	uint8_t buffer[6];

	buffer[0] = (uint8_t)(this->_gyroOffset[0] >> 8);
	buffer[1] = (uint8_t)(this->_gyroOffset[0] & 0xFF);
	buffer[2] = (uint8_t)(this->_gyroOffset[1] >> 8);
	buffer[3] = (uint8_t)(this->_gyroOffset[1] & 0xFF);
	buffer[4] = (uint8_t)(this->_gyroOffset[2] >> 8);
	buffer[5] = (uint8_t)(this->_gyroOffset[2] & 0xFF);
	if (!this->_writeAtDevice(MPU9250_REG_GYRO_XH_OFFSET, buffer, 6)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateI2cMasterControl(void)
{
	///
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateI2cMasterControlDelay(void)
{
	///
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateI2cSlave0Address(void)
{
	uint8_t aux8 = 0;

	aux8 = this->_i2cSlave0ReadWrite << 7;
	aux8 |= this->_i2cSlave0Address;
	if (!this->_writeAtDevice(MPU9250_REG_I2C_SLV0_ADDR, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateI2cSlave0Control(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->_fifoSlave0Store << MPU9250_BIT_I2C_SLV0_EN;
	aux8 |= this->_i2cSlave0SwapBytes << MPU9250_BIT_I2C_SLV0_BYTE_SW;
	aux8 |= this->_i2cSlave0RegisterDisable << MPU9250_BIT_I2C_SLV0_REG_DIS;
	aux8 |= this->_i2cSlave0GroupingSkipFirst << MPU9250_BIT_I2C_SLV0_GRP;
	aux8 |= this->_i2cSlave0DataLength << MPU9250_BIT_I2C_SLV0_LENG0;
	if (!this->_writeAtDevice(MPU9250_REG_I2C_SLV0_CTRL, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateI2cSlave0DataOut(void)
{
	uint8_t aux8 = 0;

	aux8 = this->_i2cSlave0DataOut;
	if (!this->_writeAtDevice(MPU9250_REG_I2C_SLV0_DO, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateI2cSlave0Register(void)
{
	uint8_t aux8 = 0;

	aux8 = this->_i2cSlave0Register;
	if (!this->_writeAtDevice(MPU9250_REG_I2C_SLV0_REG, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateI2cSlave1Address(void)
{
	uint8_t aux8 = 0;

	aux8 = this->_i2cSlave1ReadWrite << 7;
	aux8 |= this->_i2cSlave1Address;
	if (!this->_writeAtDevice(MPU9250_REG_I2C_SLV1_ADDR, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateI2cSlave1Control(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->_fifoSlave1Store << MPU9250_BIT_I2C_SLV1_EN;
	aux8 |= this->_i2cSlave1SwapBytes << MPU9250_BIT_I2C_SLV1_BYTE_SW;
	aux8 |= this->_i2cSlave1RegisterDisable << MPU9250_BIT_I2C_SLV1_REG_DIS;
	aux8 |= this->_i2cSlave1GroupingSkipFirst << MPU9250_BIT_I2C_SLV1_GRP;
	aux8 |= this->_i2cSlave1DataLength << MPU9250_BIT_I2C_SLV1_LENG0;
	if (!this->_writeAtDevice(MPU9250_REG_I2C_SLV1_CTRL, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateI2cSlave1DataOut(void)
{
	uint8_t aux8 = 0;

	aux8 = this->_i2cSlave1DataOut;
	if (!this->_writeAtDevice(MPU9250_REG_I2C_SLV1_DO, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateI2cSlave1Register(void)
{
	uint8_t aux8 = 0;

	aux8 = this->_i2cSlave1Register;
	if (!this->_writeAtDevice(MPU9250_REG_I2C_SLV1_REG, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateI2cSlave2Address(void)
{
	uint8_t aux8 = 0;

	aux8 = this->_i2cSlave2ReadWrite << 7;
	aux8 |= this->_i2cSlave2Address;
	if (!this->_writeAtDevice(MPU9250_REG_I2C_SLV2_ADDR, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateI2cSlave2Control(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->_fifoSlave2Store << MPU9250_BIT_I2C_SLV2_EN;
	aux8 |= this->_i2cSlave2SwapBytes << MPU9250_BIT_I2C_SLV2_BYTE_SW;
	aux8 |= this->_i2cSlave2RegisterDisable << MPU9250_BIT_I2C_SLV2_REG_DIS;
	aux8 |= this->_i2cSlave2GroupingSkipFirst << MPU9250_BIT_I2C_SLV2_GRP;
	aux8 |= this->_i2cSlave2DataLength << MPU9250_BIT_I2C_SLV2_LENG0;
	if (!this->_writeAtDevice(MPU9250_REG_I2C_SLV2_CTRL, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateI2cSlave2DataOut(void)
{
	uint8_t aux8 = 0;

	aux8 = this->_i2cSlave2DataOut;
	if (!this->_writeAtDevice(MPU9250_REG_I2C_SLV2_DO, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateI2cSlave2Register(void)
{
	uint8_t aux8 = 0;

	aux8 = this->_i2cSlave2Register;
	if (!this->_writeAtDevice(MPU9250_REG_I2C_SLV2_REG, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateI2cSlave3Address(void)
{
	uint8_t aux8 = 0;

	aux8 = this->_i2cSlave3ReadWrite << 7;
	aux8 |= this->_i2cSlave3Address;
	if (!this->_writeAtDevice(MPU9250_REG_I2C_SLV3_ADDR, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateI2cSlave3Control(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->_fifoSlave3Store << MPU9250_BIT_I2C_SLV3_EN;
	aux8 |= this->_i2cSlave3SwapBytes << MPU9250_BIT_I2C_SLV3_BYTE_SW;
	aux8 |= this->_i2cSlave3RegisterDisable << MPU9250_BIT_I2C_SLV3_REG_DIS;
	aux8 |= this->_i2cSlave3GroupingSkipFirst << MPU9250_BIT_I2C_SLV3_GRP;
	aux8 |= this->_i2cSlave3DataLength << MPU9250_BIT_I2C_SLV3_LENG0;
	if (!this->_writeAtDevice(MPU9250_REG_I2C_SLV3_CTRL, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateI2cSlave3DataOut(void)
{
	uint8_t aux8 = 0;

	aux8 = this->_i2cSlave3DataOut;
	if (!this->_writeAtDevice(MPU9250_REG_I2C_SLV3_DO, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateI2cSlave3Register(void)
{
	uint8_t aux8 = 0;

	aux8 = this->_i2cSlave3Register;
	if (!this->_writeAtDevice(MPU9250_REG_I2C_SLV3_REG, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateI2cSlave4Address(void)
{
	uint8_t aux8 = 0;

	aux8 = this->_i2cSlave4ReadWrite << 7;
	aux8 |= this->_i2cSlave4Address;
	if (!this->_writeAtDevice(MPU9250_REG_I2C_SLV4_ADDR, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateI2cSlave4Control(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->_i2cSlave4Enable << MPU9250_BIT_I2C_SLV4_EN;
	aux8 |= this->_i2cSlave4IntEnable << MPU9250_BIT_I2C_SLV4_INT_EN;
	aux8 |= this->_i2cSlave4RegisterDisable << MPU9250_BIT_I2C_SLV4_REG_DIS;
	aux8 |= this->_i2cSlave4Delay << MPU9250_BIT_I2C_SLV4_DELAY;
	if (!this->_writeAtDevice(MPU9250_REG_I2C_SLV4_CTRL, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateI2cSlave4DataOut(void)
{
	uint8_t aux8 = 0;

	aux8 = this->_i2cSlave4DataOut;
	if (!this->_writeAtDevice(MPU9250_REG_I2C_SLV4_DO, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateI2cSlave4Register(void)
{
	uint8_t aux8 = 0;

	aux8 = this->_i2cSlave4Register;
	if (!this->_writeAtDevice(MPU9250_REG_I2C_SLV4_REG, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateIntPinConfig(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->_intPinActiveLevel << MPU9250_BIT_INT_PIN_LEVEL;
	aux8 |= this->_intPinOpenDrain << MPU9250_BIT_INT_PIN_DRIVER;
	aux8 |= this->_intPinLatchEnable << MPU9250_BIT_INT_PIN_LATCH;
	aux8 |= this->_intPinAutoClear << MPU9250_BIT_INT_PIN_AUTO_RESET;
	aux8 |= this->_intPinFsyncActiveLevel << MPU9250_BIT_FSYNC_LEVEL;
	aux8 |= this->_intFsyncModeEnable << MPU9250_BIT_FSYNC_MODE_EN;
	aux8 |= this->_i2cBypassEnable << MPU9250_BIT_BYPASS_EN;
	if (!this->_writeAtDevice(MPU9250_REG_INT_PIN_CFG, &aux8, 1)) {
		return false;
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateIntEnable(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->_intWakeOnMotionEnable << MPU9250_BIT_WAKE_ON_MOTION_EN;
	aux8 |= this->_intFifoOverfowEnable << MPU9250_BIT_FIFO_OVERFLOW_EN;
	aux8 |= this->_intFsyncEnable << MPU9250_BIT_FSYNC_INT_EN;
	aux8 |= this->_intRawDataReadyEnable << MPU9250_BIT_RAW_DATA_READY_EN;
	if (!this->_writeAtDevice(MPU9250_REG_INT_ENABLE, &aux8, 1)) {
		return false;
	}

	// Return successfully
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateLowPowerAccelODR(void)
{
	// TODO: Implement function
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateMotionDetectControl(void)
{
	// TODO: Implement function
	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updatePowerManagementControl1(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->_sleepEnable << MPU9250_BIT_PWR_MGMT_1_SLEEP;
	aux8 |= this->_cycleEnable << MPU9250_BIT_PWR_MGMT_1_CYCLE;
	aux8 |= this->_gyroStandby << MPU9250_BIT_PWR_MGMT_1_GYRO_STANDBY;
	aux8 |= this->_ptatDisable << MPU9250_BIT_PWR_MGMT_1_PD_PTAT;
	aux8 |= this->_clockSelection << MPU9250_BIT_PWR_MGMT_1_CLKSEL0;
	if (!this->_writeAtDevice(MPU9250_REG_PWR_MGMT_1, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updatePowerManagementControl2(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->_accelDisableX << MPU9250_BIT_PWR_MGMT_2_DIS_XA;
	aux8 |= this->_accelDisableY << MPU9250_BIT_PWR_MGMT_2_DIS_YA;
	aux8 |= this->_accelDisableZ << MPU9250_BIT_PWR_MGMT_2_DIS_ZA;
	aux8 |= this->_gyroDisableX << MPU9250_BIT_PWR_MGMT_2_DIS_XG;
	aux8 |= this->_gyroDisableY << MPU9250_BIT_PWR_MGMT_2_DIS_YG;
	aux8 |= this->_gyroDisableZ << MPU9250_BIT_PWR_MGMT_2_DIS_ZG;
	if (!this->_writeAtDevice(MPU9250_REG_PWR_MGMT_2, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateSampleRateDivider(void)
{
	uint8_t aux8 = 0;

	aux8 = this->_sampleRateDivider;
	if (!this->_writeAtDevice(MPU9250_REG_SMPLRT_DIV, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateSignalPathReset(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->_resetGyro << MPU9250_BIT_SIG_PAT_RST_GYRO;
	aux8 |= this->_resetAccel << MPU9250_BIT_SIG_PAT_RST_ACCEL;
	aux8 |= this->_resetTemp << MPU9250_BIT_SIG_PAT_RST_TEMP;
	this->_resetGyro = false;
	this->_resetAccel = false;
	this->_resetTemp = false;
	if (!this->_writeAtDevice(MPU9250_REG_SIGNAL_PATH_RESET, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateUserControl(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->_fifoEnable << MPU9250_BIT_USR_CTRL_FIFO_EN;
	aux8 |= this->_i2cMasterEnable << MPU9250_BIT_USR_CTRL_I2C_MST_EN;
	aux8 |= this->_useSpi << MPU9250_BIT_USR_CTRL_I2C_IF_DIS;
	aux8 |= this->_resetFifo << MPU9250_BIT_USR_CTRL_FIFO_RST;
	aux8 |= this->_resetI2c << MPU9250_BIT_USR_CTRL_I2C_MST_RST;
	aux8 |= this->_resetSignal << MPU9250_BIT_USR_CTRL_SIG_COND_RST;
	if (!this->_writeAtDevice(MPU9250_REG_USER_CTRL, &aux8, 1)) {
		return false;
	}

	this->_lastError = ERROR_NONE;
	return true;
}

bool Mpu9250::_updateWakeUpMotionThreshold(void)
{
	// TODO: Implement function
	this->_lastError = ERROR_NONE;
	return true;
}

// =============================================================================
// Class protected methods
// =============================================================================

// NONE
