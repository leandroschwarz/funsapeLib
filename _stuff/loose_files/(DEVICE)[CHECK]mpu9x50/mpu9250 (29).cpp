

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "mpu9250.hpp"
#if __MPU9250_HPP != 1001
#error Error 101 - Build mismatch on header and source code files (mpu9250).
#endif

// -----------------------------------------------------------------------------
// File exclusive - Constant definitions ---------------------------------------

#define SLAVE_ADDRESS_NORMAL	0x68
#define SLAVE_ADDRESS_ALTERNATE	0x69
#define WHO_AM_I				0x71


#define REG_SELF_TEST_X_GYRO	0x00
#define REG_SELF_TEST_Y_GYRO	0x01
#define REG_SELF_TEST_Z_GYRO	0x02
#define REG_SELF_TEST_X_ACCEL	0x0D
#define REG_SELF_TEST_Y_ACCEL	0x0E
#define REG_SELF_TEST_Z_ACCEL	0x0F
#define REG_XG_OFFSET_H			0x13
#define REG_XG_OFFSET_L			0x14
#define REG_YG_OFFSET_H			0x15
#define REG_YG_OFFSET_L			0x16
#define REG_ZG_OFFSET_H			0x17
#define REG_ZG_OFFSET_L			0x18
#define REG_SMPLRT_DIV			0x19
#define REG_CONFIG				0x1A
#define REG_GYRO_CONFIG			0x1B
#define REG_ACCEL_CONFIG		0x1C
#define REG_ACCEL_CONFIG2		0x1D
#define REG_LP_ACCEL_ODR		0x1E
#define REG_WOM_THR				0x1F
#define REG_FIFO_EN				0x23
#define REG_I2C_MST_CTRL		0x24
#define REG_I2C_SLV0_ADDR		0x25
#define REG_I2C_SLV0_REG		0x26
#define REG_I2C_SLV0_CTRL		0x27
#define REG_I2C_SLV1_ADDR		0x28
#define REG_I2C_SLV1_REG		0x29
#define REG_I2C_SLV1_CTRL		0x2A
#define REG_I2C_SLV2_ADDR		0x2B
#define REG_I2C_SLV2_REG		0x2C
#define REG_I2C_SLV2_CTRL		0x2D
#define REG_I2C_SLV3_ADDR		0x2E
#define REG_I2C_SLV3_REG		0x2F
#define REG_I2C_SLV3_CTRL		0x30
#define REG_I2C_SLV4_ADDR		0x31
#define REG_I2C_SLV4_REG		0x32
#define REG_I2C_SLV4_DO			0x33
#define REG_I2C_SLV4_CTRL		0x34
#define REG_I2C_SLV4_DI			0x35
#define REG_I2C_MST_STATUS		0x36
#define REG_INT_PIN_CFG			0x37
#define REG_INT_ENABLE			0x38
#define REG_INT_STATUS			0x3A
#define REG_ACCEL_XOUT_H		0x3B
#define REG_ACCEL_XOUT_L		0x3C
#define REG_ACCEL_YOUT_H		0x3D
#define REG_ACCEL_YOUT_L		0x3E
#define REG_ACCEL_ZOUT_H		0x3F
#define REG_ACCEL_ZOUT_L		0x40
#define REG_TEMP_OUT_H			0x41
#define REG_TEMP_OUT_L			0x42
#define REG_GYRO_XOUT_H			0x43
#define REG_GYRO_XOUT_L			0x44
#define REG_GYRO_YOUT_H			0x45
#define REG_GYRO_YOUT_L			0x46
#define REG_GYRO_ZOUT_H			0x47
#define REG_GYRO_ZOUT_L			0x48
#define REG_EXT_SENS_DATA_00	0x49
#define REG_EXT_SENS_DATA_01	0x4A
#define REG_EXT_SENS_DATA_02	0x4B
#define REG_EXT_SENS_DATA_03	0x4C
#define REG_EXT_SENS_DATA_04	0x4D
#define REG_EXT_SENS_DATA_05	0x4E
#define REG_EXT_SENS_DATA_06	0x4F
#define REG_EXT_SENS_DATA_07	0x50
#define REG_EXT_SENS_DATA_08	0x51
#define REG_EXT_SENS_DATA_09	0x52
#define REG_EXT_SENS_DATA_10	0x53
#define REG_EXT_SENS_DATA_11	0x54
#define REG_EXT_SENS_DATA_12	0x55
#define REG_EXT_SENS_DATA_13	0x56
#define REG_EXT_SENS_DATA_14	0x57
#define REG_EXT_SENS_DATA_15	0x58
#define REG_EXT_SENS_DATA_16	0x59
#define REG_EXT_SENS_DATA_17	0x5A
#define REG_EXT_SENS_DATA_18	0x5B
#define REG_EXT_SENS_DATA_19	0x5C
#define REG_EXT_SENS_DATA_20	0x5D
#define REG_EXT_SENS_DATA_21	0x5E
#define REG_EXT_SENS_DATA_22	0x5F
#define REG_EXT_SENS_DATA_23	0x60
#define REG_I2C_SLV0_DO			0x63
#define REG_I2C_SLV1_DO			0x64
#define REG_I2C_SLV2_DO			0x65
#define REG_I2C_SLV3_DO			0x66
#define REG_I2C_MST_DELAY_CTRL	0x67
#define REG_SIGNAL_PATH_RESET	0x68
#define REG_MOT_DETECT_CTRL		0x69
#define REG_USER_CTRL			0x6A
#define REG_PWR_MGMT_1			0x6B
#define REG_PWR_MGMT_2			0x6C
#define REG_FIFO_COUNT_H		0x72
#define REG_FIFO_COUNT_L		0x73
#define REG_FIFO_R_W			0x74
#define REG_WHO_AM_I			0x75
#define REG_XA_OFFSET_H			0x77
#define REG_XA_OFFSET_L			0x78
#define REG_YA_OFFSET_H			0x7A
#define REG_YA_OFFSET_L			0x7B
#define REG_ZA_OFFSET_H			0x7D
#define REG_ZA_OFFSET_L			0x7E

#define BIT_GYRO_CONFIG_Z_ST_EN			7
#define BIT_GYRO_CONFIG_Y_ST_EN			6
#define BIT_GYRO_CONFIG_X_ST_EN			5
#define BIT_GYRO_CONFIG_FS_SEL0			2
#define BIT_GYRO_CONFIG_FCHOICE_B0		0

#define BIT_ACCEL_CONFIG_Z_ST_EN		7
#define BIT_ACCEL_CONFIG_Y_ST_EN		6
#define BIT_ACCEL_CONFIG_X_ST_EN		5
#define BIT_ACCEL_CONFIG_FS_SEL0		2

#define BIT_INT_STATUS_WOM				6
#define BIT_INT_STATUS_FIFO_OVERFLOW	4
#define BIT_INT_STATUS_FSYNC			3
#define BIT_INT_STATUS_RAW_DATA_RDY		0

#define BIT_PWR_MGMT_1_H_RESET			7
#define BIT_PWR_MGMT_1_SLEEP			6
#define BIT_PWR_MGMT_1_CYCLE			5
#define BIT_PWR_MGMT_1_GYRO_STANDBY		4
#define BIT_PWR_MGMT_1_PD_PTAT			3
#define BIT_PWR_MGMT_1_CLKSEL0			0

// -----------------------------------------------------------------------------
// File exclusive - New data types ---------------------------------------------

#define i2cDataRead(reg, buff, size)	HAL_I2C_Mem_Read(this->i2cHandler_, this->i2cMyAddress_, reg, 1, buff, size, 100)
#define i2cDataWrite(reg, buff, size)	HAL_I2C_Mem_Write(this->i2cHandler_, this->i2cMyAddress_, reg, 1, buff, size, 100)

// -----------------------------------------------------------------------------
// File exclusive - Macro-functions definitions --------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Global Variables ------------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Mpu9250 class - Constructors definitions ------------------------------------

Mpu9250::Mpu9250(I2C_HandleTypeDef *i2cHand, bool_t alternateAddress)
{
	uint8_t i;

	if (alternateAddress) {
		this->i2cMyAddress_ = (SLAVE_ADDRESS_ALTERNATE << 1);
	} else {
		this->i2cMyAddress_ = (SLAVE_ADDRESS_NORMAL << 1);
	}

	this->i2cHandler_ = i2cHand;

	// Clear data members
	for (i = 0; i < 3; i++) {
		this->accelFactorySelfTest_[i] = 0;
		this->accelData_[i] = 0;
		this->accelOffset_[i] = 0;
		this->gyroOffset_[i] = 0;
		this->gyroData_[i] = 0;
		this->gyroFactorySelfTest_[i] = 0;
	}
	this->initialized_ = FALSE;
	this->accelXSelfTestEnabled_ = FALSE;
	this->accelYSelfTestEnabled_ = FALSE;
	this->accelZSelfTestEnabled_ = FALSE;
	this->gyroXSelfTestEnabled_ = FALSE;
	this->gyroYSelfTestEnabled_ = FALSE;
	this->gyroZSelfTestEnabled_ = FALSE;
	this->accelFullScale_ = AFS_2G;
	this->gyroFullScale_ = GFS_250DPS;
	this->gyroSamplingConfig_ = GYRO_SAMP_8_KHZ_FILTER_250_HZ;
	this->tempData_ = 0;
	this->powerDownMode_ = FALSE;
	this->powerMode_ = POWER_MODE_NORMAL;
	this->clockSelection_ = CLOCK_AUTO_SELECT_BEST;
	this->intWakeOnMotionFlag_ = FALSE;
	this->intFifoOverflowFlag_ = FALSE;
	this->intFsyncFlag_ = FALSE;
	this->intI2cDataReadyFlag_ = FALSE;

	/*
		this->fifoStopWhenFull_ = FALSE;
		this->fifoTempRecordEnabled_ = FALSE;
		this->fifoGyroXRecordEnabled_ = FALSE;
		this->fifoGyroYRecordEnabled_ = FALSE;
		this->fifoGyroZRecordEnabled_ = FALSE;
		this->fifoAccelXYXRecordEnabled_ = FALSE;
		this->fifoSlave2RecordEnabled_ = FALSE;
		this->fifoSlave1RecordEnabled_ = FALSE;
		this->fifoSlave0RecordEnabled_ = FALSE;
		this->i2cMultiMasterEnabled_ = FALSE;
		this->i2cDataReadyIntWaitExternalSensors_ = FALSE;
		this->fifoSlave3RecordEnabled_ = FALSE;
		this->i2cSendStopBetweenSlaves_ = FALSE;
		this->i2cSlave0ReadMode_ = FALSE;
		this->i2cSlave0Enabled_ = FALSE;
		this->i2cSlave0WordBytesSwap_ = FALSE;
		this->i2cSlave0DoNotSendRegister_ = FALSE;
		this->i2cSlave0GroupEndingEven_ = FALSE;
		this->i2cSlave1ReadMode_ = FALSE;
		this->i2cSlave1Enabled_ = FALSE;
		this->i2cSlave1WordBytesSwap_ = FALSE;
		this->i2cSlave1DoNotSendRegister_ = FALSE;
		this->i2cSlave1GroupEndingEven_ = FALSE;
		this->i2cSlave2ReadMode_ = FALSE;
		this->i2cSlave2Enabled_ = FALSE;
		this->i2cSlave2WordBytesSwap_ = FALSE;
		this->i2cSlave2DoNotSendRegister_ = FALSE;
		this->i2cSlave2GroupEndingEven_ = FALSE;
		this->i2cSlave3ReadMode_ = FALSE;
		this->i2cSlave3Enabled_ = FALSE;
		this->i2cSlave3WordBytesSwap_ = FALSE;
		this->i2cSlave3DoNotSendRegister_ = FALSE;
		this->i2cSlave3GroupEndingEven_ = FALSE;
		this->i2cSlave4ReadMode_ = FALSE;
		this->i2cSlave4SingleTransferEnable_ = FALSE;
		this->i2cSlave4IntTransferReadyEnabled_ = FALSE;
		this->i2cSlave4DoNotSendRegister_ = FALSE;
		this->i2cStatusPassThroughFlag_ = FALSE;
		this->i2cStatusSlave4TransferDoneFlag_ = FALSE;
		this->i2cStatusArbitrationLost_ = FALSE;
		this->i2cStatusSlave4NackReceivedFlag_ = FALSE;
		this->i2cStatusSlave3NackReceivedFlag_ = FALSE;
		this->i2cStatusSlave2NackReceivedFlag_ = FALSE;
		this->i2cStatusSlave1NackReceivedFlag_ = FALSE;
		this->i2cStatusSlave0NackReceivedFlag_ = FALSE;
		this->intPinLatched_ = FALSE;
		this->intPinClearWhenRead_ = FALSE;
		this->fsyncPinTriggerInterrupt_ = FALSE;
		this->i2cBypassModeEnabled_ = FALSE;
		this->intWakeOnMotionEnabled_ = FALSE;
		this->intFifoOverflowEnabled_ = FALSE;
		this->intFsyncEnabled_ = FALSE;
		this->intI2cDataReadyEnabled_ = FALSE;
		this->i2cExtDataShadowingEnabled_ = FALSE;
		this->i2cSlave4DelayEnabled_ = FALSE;
		this->i2cSlave3DelayEnabled_ = FALSE;
		this->i2cSlave2DelayEnabled_ = FALSE;
		this->i2cSlave1DelayEnabled_ = FALSE;
		this->i2cSlave0DelayEnabled_ = FALSE;
		this->gyroSignalPathReset_ = FALSE;
		this->accelSignalPathReset_ = FALSE;
		this->tempSignalPathReset_ = FALSE;
		this->gyroZDisable_ = FALSE;
		this->fifoEnabled_ = FALSE;
		this->i2cMasterEnabled_ = FALSE;
		this->useSpiInterface_ = FALSE;
		this->accelXDisable_ = FALSE;
		this->accelYDisable_ = FALSE;
		this->accelZDisable_ = FALSE;
		this->gyroXDisable_ = FALSE;
		this->gyroYDisable_ = FALSE;
		this->intPinActivationLevel_ = LOW;
		this->intPinOutputDriver_ = PUSH_PULL;
		this->fsyncPinActivationLevel_ = LOW;
		this->i2cSlave0Address_ = 0;
		this->i2cSlave0Register_ = 0;
		this->i2cSlave0DataLenght_ = 0;
		this->i2cSlave1Address_ = 0;
		this->i2cSlave1Register_ = 0;
		this->i2cSlave1DataLenght_ = 0;
		this->i2cSlave2Address_ = 0;
		this->i2cSlave2Register_ = 0;
		this->i2cSlave2DataLenght_ = 0;
		this->i2cSlave3Address_ = 0;
		this->i2cSlave3Register_ = 0;
		this->i2cSlave3DataLenght_ = 0;
		this->i2cSlave4Address_ = 0;
		this->i2cSlave4Register_ = 0;
		this->i2cSlave4DataOut_ = 0;
		this->i2cSlave4MasterDelay_ = 0;
		this->i2cSlave4DataIn_ = 0;
		this->i2cSlave0DataOut_ = 0;
		this->i2cSlave1DataOut_ = 0;
		this->i2cSlave2DataOut_ = 0;
		this->i2cSlave3DataOut_ = 0;
		this->fifoCount_ = 0;
		this->fifoData_ = 0;
		this->accelWakeUpMotionThreshold_ = 0;
		for (i = 0;i < 24;i++) {
			this->extSensData_[i] = 0;
		}
		this->sampleRateDivider_ = 0;
		this->externalSync_ = EXT_SYNC_DISABLED;
		this->accelSamplingConfig_ = ACCEL_SAMP_4_KHZ_FILTER_1046_HZ;
		this->accelSamplingLowPower_ = ACCEL_SAMP_LOW_POWER_0P24_HZ;
		this->i2cMasterClock_ = I2C_MST_CLK_348_KHz;
	//	MOT_DETECT_CTRL			ACCEL_INTEL_EN
	//	MOT_DETECT_CTRL			ACCEL_INTEL_MODE
	//	USER_CTRL				FIFO_RST
	//	USER_CTRL				I2C_MST_RST
	//	USER_CTRL				SIG_COND_RST
	*/

	return;
}

// -----------------------------------------------------------------------------
// Mpu9250 class - Public functions definitions --------------------------------

mpu9250Error_e Mpu9250::initialize(void)
{
	// Local variables
	uint8_t whoAmI = 0x00;

	// Reads WHO_AM_I
	i2cDataRead(REG_WHO_AM_I, &whoAmI, 1);
	if (whoAmI != WHO_AM_I) {
		return MPU9250_ERROR_WRONG_WHO_AM_I;
	}
	this->readGyroFactorySelfTest();
	this->readAccelFactorySelfTest();
	this->readIntStatus();
	this->updatePowerManagementControl1(); // 1
	this->readAccelOffset();
	this->readGyroOffset();
	this->initialized_ = TRUE;

	return MPU9250_ERROR_NONE;
}

mpu9250Error_e Mpu9250::setGyroScale(gyroFullScale_e scale)
{
	if (!this->initialized_) {
		return MPU9250_ERROR_NOT_INITIALIZED;
	}

	this->gyroFullScale_ = scale;
	this->updateGyroConfig();

	return MPU9250_ERROR_NONE;
}

mpu9250Error_e Mpu9250::setAccelScale(accelFullScale_e scale)
{
	if (!this->initialized_) {
		return MPU9250_ERROR_NOT_INITIALIZED;
	}

	this->accelFullScale_ = scale;
	this->updateAccelConfig();

	return MPU9250_ERROR_NONE;
}

mpu9250Error_e Mpu9250::performSelfTest(axis_t accelAxis, axis_t gyroAxis)
{
	bool_t axisSelected = FALSE;

	if (!this->initialized_) {
		return MPU9250_ERROR_NOT_INITIALIZED;
	}

	// Update accelerometer axis
	if (isBitSet(accelAxis, 0)) {
		this->accelXSelfTestEnabled_ = TRUE;
		axisSelected = TRUE;
	} else {
		this->accelXSelfTestEnabled_ = FALSE;
	}
	if (isBitSet(accelAxis, 1)) {
		this->accelYSelfTestEnabled_ = TRUE;
		axisSelected = TRUE;
	} else {
		this->accelYSelfTestEnabled_ = FALSE;
	}
	if (isBitSet(accelAxis, 2)) {
		this->accelZSelfTestEnabled_ = TRUE;
		axisSelected = TRUE;
	} else {
		this->accelZSelfTestEnabled_ = FALSE;
	}

	// Update gyroscope axis
	if (isBitSet(gyroAxis, 0)) {
		this->gyroXSelfTestEnabled_ = TRUE;
		axisSelected = TRUE;
	} else {
		this->gyroXSelfTestEnabled_ = FALSE;
	}
	if (isBitSet(gyroAxis, 1)) {
		this->gyroYSelfTestEnabled_ = TRUE;
		axisSelected = TRUE;
	} else {
		this->gyroYSelfTestEnabled_ = FALSE;
	}
	if (isBitSet(gyroAxis, 2)) {
		this->gyroZSelfTestEnabled_ = TRUE;
		axisSelected = TRUE;
	} else {
		this->gyroZSelfTestEnabled_ = FALSE;
	}

	if (!axisSelected) {		// No axis was selected
		return MPU9250_INVALID_AXIS;
	}

	// DO the rest of stuff
	// (Update registers)

	this->updateGyroConfig();
	this->updateAccelConfig();

	// DO the rest of stuff

	return MPU9250_ERROR_NONE;
}

mpu9250Error_e Mpu9250::getAccelData(int16_t *accelBuff, axis_t axis)
{
	uint8_t i = 0;

	if (!this->initialized_) {
		return MPU9250_ERROR_NOT_INITIALIZED;
	}

	if (axis == AXIS_NONE) {
		return MPU9250_INVALID_AXIS;
	}

	this->readAccelData();

	if (isBitSet(axis, 0)) {
		accelBuff[i++] = this->accelData_[0];
	}
	if (isBitSet(axis, 1)) {
		accelBuff[i++] = this->accelData_[1];
	}
	if (isBitSet(axis, 2)) {
		accelBuff[i++] = this->accelData_[2];
	}

	return MPU9250_ERROR_NONE;
}

mpu9250Error_e Mpu9250::getGyroData(int16_t *gyroBuff, axis_t axis)
{
	uint8_t i = 0;

	if (!this->initialized_) {
		return MPU9250_ERROR_NOT_INITIALIZED;
	}

	if (axis == AXIS_NONE) {
		return MPU9250_INVALID_AXIS;
	}

	this->readGyroData();

	if (isBitSet(axis, 0)) {
		gyroBuff[i++] = this->gyroData_[0];
	}
	if (isBitSet(axis, 1)) {
		gyroBuff[i++] = this->gyroData_[1];
	}
	if (isBitSet(axis, 2)) {
		gyroBuff[i++] = this->gyroData_[2];
	}

	return MPU9250_ERROR_NONE;
}

mpu9250Error_e Mpu9250::getTempData(int16_t *temp)
{
	if (!this->initialized_) {
		return MPU9250_ERROR_NOT_INITIALIZED;
	}

	this->readTempData();

	*temp = this->tempData_;

	return MPU9250_ERROR_NONE;
}


mpu9250Error_e Mpu9250::getAccelOffset(int16_t *accelBuff, axis_t axis)
{
	uint8_t i = 0;

	if (!this->initialized_) {
		return MPU9250_ERROR_NOT_INITIALIZED;
	}

	if (axis == AXIS_NONE) {
		return MPU9250_INVALID_AXIS;
	}

	this->readAccelOffset();

	if (isBitSet(axis, 0)) {
		accelBuff[i++] = this->accelOffset_[0];
	}
	if (isBitSet(axis, 1)) {
		accelBuff[i++] = this->accelOffset_[1];
	}
	if (isBitSet(axis, 2)) {
		accelBuff[i++] = this->accelOffset_[2];
	}

	return MPU9250_ERROR_NONE;
}

mpu9250Error_e Mpu9250::getGyroOffset(int16_t *gyroBuff, axis_t axis)
{
	uint8_t i = 0;

	if (!this->initialized_) {
		return MPU9250_ERROR_NOT_INITIALIZED;
	}

	if (axis == AXIS_NONE) {
		return MPU9250_INVALID_AXIS;
	}

	this->readGyroOffset();

	if (isBitSet(axis, 0)) {
		gyroBuff[i++] = this->gyroOffset_[0];
	}
	if (isBitSet(axis, 1)) {
		gyroBuff[i++] = this->gyroOffset_[1];
	}
	if (isBitSet(axis, 2)) {
		gyroBuff[i++] = this->gyroOffset_[2];
	}

	return MPU9250_ERROR_NONE;
}

mpu9250Error_e Mpu9250::setAccelOffset(int16_t *accelBuff, axis_t axis)
{
	uint8_t i = 0;

	if (!this->initialized_) {
		return MPU9250_ERROR_NOT_INITIALIZED;
	}

	if (axis == AXIS_NONE) {
		return MPU9250_INVALID_AXIS;
	}

	if (isBitSet(axis, 0)) {
		this->accelOffset_[0] = accelBuff[i++];
	}
	if (isBitSet(axis, 1)) {
		this->accelOffset_[1] = accelBuff[i++];
	}
	if (isBitSet(axis, 2)) {
		this->accelOffset_[2] = accelBuff[i++];
	}
	this->updateAccelOffset();

	return MPU9250_ERROR_NONE;
}

mpu9250Error_e Mpu9250::setGyroOffset(int16_t *gyroBuff, axis_t axis)
{
	uint8_t i = 0;

	if (!this->initialized_) {
		return MPU9250_ERROR_NOT_INITIALIZED;
	}

	if (axis == AXIS_NONE) {
		return MPU9250_INVALID_AXIS;
	}

	if (isBitSet(axis, 0)) {
		this->gyroOffset_[0] = gyroBuff[i++];
	}
	if (isBitSet(axis, 1)) {
		this->gyroOffset_[1] = gyroBuff[i++];
	}
	if (isBitSet(axis, 2)) {
		this->gyroOffset_[2] = gyroBuff[i++];
	}
	this->updateGyroOffset();

	return MPU9250_ERROR_NONE;
}



// -----------------------------------------------------------------------------
// Mpu9250 class - Private functions definitions -------------------------------

void Mpu9250::updateGyroConfig(void)
{
	uint8_t gyroConfigValue = 0;

	gyroConfigValue |= this->gyroXSelfTestEnabled_ << BIT_GYRO_CONFIG_X_ST_EN;
	gyroConfigValue |= this->gyroYSelfTestEnabled_ << BIT_GYRO_CONFIG_Y_ST_EN;
	gyroConfigValue |= this->gyroZSelfTestEnabled_ << BIT_GYRO_CONFIG_Z_ST_EN;
	gyroConfigValue |= this->gyroFullScale_ << BIT_GYRO_CONFIG_FS_SEL0;
	gyroConfigValue |= (this->gyroSamplingConfig_ >> 3) << BIT_GYRO_CONFIG_FCHOICE_B0;

	i2cDataWrite(REG_GYRO_CONFIG, &gyroConfigValue, 1);

	return;
}

void Mpu9250::updateAccelConfig(void)
{
	uint8_t accelConfigValue = 0;

	accelConfigValue |= this->accelXSelfTestEnabled_ << BIT_ACCEL_CONFIG_X_ST_EN;
	accelConfigValue |= this->accelYSelfTestEnabled_ << BIT_ACCEL_CONFIG_Y_ST_EN;
	accelConfigValue |= this->accelZSelfTestEnabled_ << BIT_ACCEL_CONFIG_Z_ST_EN;
	accelConfigValue |= this->accelFullScale_ << BIT_ACCEL_CONFIG_FS_SEL0;

	i2cDataWrite(REG_ACCEL_CONFIG, &accelConfigValue, 1);

	return;
}

void Mpu9250::readAccelFactorySelfTest(void)
{
	uint8_t buffer[3] = {0, 0, 0};

	i2cDataRead(REG_SELF_TEST_X_ACCEL, buffer, 3);
	this->accelFactorySelfTest_[0] = buffer[0];
	this->accelFactorySelfTest_[1] = buffer[1];
	this->accelFactorySelfTest_[2] = buffer[2];

	return;
}

void Mpu9250::readGyroFactorySelfTest(void)
{
	uint8_t buffer[3] = {0, 0, 0};

	i2cDataRead(REG_SELF_TEST_X_GYRO, buffer, 3);
	this->gyroFactorySelfTest_[0] = buffer[0];
	this->gyroFactorySelfTest_[1] = buffer[1];
	this->gyroFactorySelfTest_[2] = buffer[2];

	return;
}

void Mpu9250::readAccelData(void)
{
	uint8_t buffer[6] = {0, 0, 0, 0, 0, 0};

	i2cDataRead(REG_ACCEL_XOUT_H, buffer, 6);
	this->accelData_[0] = (int16_t)(((int16_t)buffer[0] << 8) | buffer[1]);
	this->accelData_[1] = (int16_t)(((int16_t)buffer[2] << 8) | buffer[3]);
	this->accelData_[2] = (int16_t)(((int16_t)buffer[4] << 8) | buffer[5]);

	return;
}

void Mpu9250::readGyroData(void)
{
	uint8_t buffer[6] = {0, 0, 0, 0, 0, 0};

	i2cDataRead(REG_GYRO_XOUT_H, buffer, 6);
	this->gyroData_[0] = (int16_t)(((int16_t)buffer[0] << 8) | buffer[1]);
	this->gyroData_[1] = (int16_t)(((int16_t)buffer[2] << 8) | buffer[3]);
	this->gyroData_[2] = (int16_t)(((int16_t)buffer[4] << 8) | buffer[5]);

	return;
}

void Mpu9250::readTempData(void)
{
	uint8_t buffer[2] = {0, 0};

	i2cDataRead(REG_TEMP_OUT_H, buffer, 2);
	this->tempData_ = (int16_t)(((int16_t)buffer[0] << 8) | (int16_t)buffer[1]);

	return;
}

void Mpu9250::readAccelOffset(void)
{
	uint8_t buffer[6] = {0, 0, 0, 0, 0, 0};

	i2cDataRead(REG_XA_OFFSET_H, buffer, 6);
	this->accelOffset_[0] = (int16_t)(((int16_t)buffer[0] << 8) | buffer[1]) >> 1;
	this->accelOffset_[1] = (int16_t)(((int16_t)buffer[2] << 8) | buffer[3]) >> 1;
	this->accelOffset_[2] = (int16_t)(((int16_t)buffer[4] << 8) | buffer[5]) >> 1;

	return;
}

void Mpu9250::readGyroOffset(void)
{
	uint8_t buffer[6] = {0, 0, 0, 0, 0, 0};

	i2cDataRead(REG_XG_OFFSET_H, buffer, 6);
	this->gyroOffset_[0] = (int16_t)(((int16_t)buffer[0] << 8) | buffer[1]);
	this->gyroOffset_[1] = (int16_t)(((int16_t)buffer[2] << 8) | buffer[3]);
	this->gyroOffset_[2] = (int16_t)(((int16_t)buffer[4] << 8) | buffer[5]);

	return;
}

void Mpu9250::updateAccelOffset(void)
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
	i2cDataWrite(REG_XA_OFFSET_H, buffer, 6);

	return;
}

void Mpu9250::updateGyroOffset(void)
{
	uint8_t buffer[6];

	buffer[0] = (uint8_t)(this->gyroOffset_[0] >> 8);
	buffer[1] = (uint8_t)(this->gyroOffset_[0] & 0xFF);
	buffer[2] = (uint8_t)(this->gyroOffset_[1] >> 8);
	buffer[3] = (uint8_t)(this->gyroOffset_[1] & 0xFF);
	buffer[4] = (uint8_t)(this->gyroOffset_[2] >> 8);
	buffer[5] = (uint8_t)(this->gyroOffset_[2] & 0xFF);
	i2cDataWrite(REG_XG_OFFSET_H, buffer, 6);

	return;
}

void Mpu9250::readIntStatus(void)
{
	uint8_t aux8;

	i2cDataRead(REG_INT_STATUS, &aux8, 1);
	this->intWakeOnMotionFlag_ = isBitSet(aux8, BIT_INT_STATUS_WOM);
	this->intFifoOverflowFlag_ = isBitSet(aux8, BIT_INT_STATUS_FIFO_OVERFLOW);
	this->intFsyncFlag_ = isBitSet(aux8, BIT_INT_STATUS_FSYNC);
	this->intI2cDataReadyFlag_ = isBitSet(aux8, BIT_INT_STATUS_RAW_DATA_RDY);

	return;
}

void Mpu9250::updatePowerManagementControl1(void)
{
	uint8_t aux8 = 0;

	aux8 |= this->powerMode_ << BIT_PWR_MGMT_1_GYRO_STANDBY;
	aux8 |= this->powerDownMode_ << BIT_PWR_MGMT_1_PD_PTAT;
	aux8 |= this->clockSelection_ << BIT_PWR_MGMT_1_CLKSEL0;
	i2cDataWrite(REG_PWR_MGMT_1, &aux8, 1);

	return;
}




// -----------------------------------------------------------------------------
// Mpu9250 class - Protected functions definitions -----------------------------

/* NONE */

// -----------------------------------------------------------------------------
// General public functions definitions ----------------------------------------

/* NONE */

void Mpu9250::readAll(uint8_t *buffer, uint8_t *size)
{
	i2cDataRead(0x00, &buffer[0], 3);
	i2cDataRead(0x0D, &buffer[3], 3);
	i2cDataRead(0x13, &buffer[6], 13);
	i2cDataRead(0x23, &buffer[19], 22);
	i2cDataRead(0x3A, &buffer[41], 39);
	i2cDataRead(0x63, &buffer[80], 10);
	i2cDataRead(0x72, &buffer[90], 4);
	i2cDataRead(0x77, &buffer[94], 2);
	i2cDataRead(0x7A, &buffer[96], 2);
	i2cDataRead(0x7D, &buffer[98], 2);
	*size = 100;

	return;
}
