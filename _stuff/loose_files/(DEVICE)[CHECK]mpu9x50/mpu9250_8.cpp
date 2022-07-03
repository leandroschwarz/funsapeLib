/*
 * mpu9250.c
 *
 * Created: 16/04/2018 13:43:32
 *  Author: Schwarz
 */

#include "mpu9250.h"


bool_t _mpu9250ActivateSensor(mpu9250_s * device, mpu9250SensorFlags_e sensorFlags)
{
	uint8_t buffer[2];
	uint8_t sensorStatus = device->activeSensorFlags;

	// Initial verifications
	if(sensorFlags == 0) {			// Nothing to do, just return
		return TRUE;
	}

	// Process compass flag
	if(sensorFlags & MPU9250_SENSOR_COMPASS) {					// Activate compass
		if(!(sensorStatus & MPU9250_SENSOR_COMPASS)) {			// AND compass isn't activated
			// XX Do stuff to activate compass
			sensorStatus |= MPU9250_SENSOR_COMPASS;
		}
	}

	// Process other flags
	if((sensorStatus | sensorFlags) != sensorStatus) {			// Activate other sensors
		sensorStatus |= sensorFlags;
		buffer[0] = MPU9250_REGISTER_PWR_MGMT_2;
		buffer[1] = sensorStatus & 0x3F;
		twiMasterSendData(device->slaveAddress, TWI_WRITE, buffer, 2);
	}

	// Update data in struct
	device->activeSensorFlags = sensorStatus;

	// Everything OK, just return
	return TRUE;
}

bool_t _mpu9250DectivateSensor(mpu9250_s * device, mpu9250SensorFlags_e sensorFlags)
{
	uint8_t buffer[2];
	uint8_t sensorStatus = device->activeSensorFlags;

	// Initial verifications
	if(sensorFlags == 0) {			// Nothing to do, just return
		return TRUE;
	}

	// Process compass flag
	if(sensorFlags & MPU9250_SENSOR_COMPASS) {					// Deactivate compass
		if(sensorStatus & MPU9250_SENSOR_COMPASS) {				// AND compass is activated
			// XX Do stuff to deactivate compass
			sensorStatus &= ~(MPU9250_SENSOR_COMPASS);
		}
	}

	// Process other flags
	if((sensorStatus & sensorFlags) != sensorStatus) {			// Deactivate other sensors
		sensorStatus &= ~sensorFlags;
		buffer[0] = MPU9250_REGISTER_PWR_MGMT_2;
		buffer[1] = sensorStatus & 0x3F;
		twiMasterSendData(device->slaveAddress, TWI_WRITE, buffer, 2);
	}

	// Update data in struct
	device->activeSensorFlags = sensorStatus;

	// Everything OK, just return
	return TRUE;
}

bool_t _mpu9250Init(mpu9250_s * device, mpu9250TwiSlaveAddress_e slaveAddress)
{
	uint8_t buffer[6];

	// Update struct variables
	device->slaveAddress = slaveAddress;
	device->fifoMode = FALSE;
	device->extSyncSet = MPU9250_EXT_SYNC_DISABLED;
	device->dlpfCfg = MPU9250_DLPF_CFG_250_Hz_NO_DLPF;
	device->activeSensorFlags = MPU9250_SENSOR_GYRO_XYZ | MPU9250_SENSOR_ACCEL_XYZ | MPU9250_SENSOR_COMPASS;
	device->gyroFsr = MPU9250_GYRO_FSR_250_DPS;
	device->accelFsr = MPU9250_ACCEL_FSR_2G;

	device->accelHalf = FALSE;
	device->dlpfCfg = 0xFF;
	device->sampleRate = 0xFFFF;
	device->fifoEnable = 0xFF;
	device->bypassMode = TRUE;
	device->compassSampleRate = 0xFFFF;
	device->clkSel = MPU9250_CLK_SEL_PLL;
	/* Handled in next call to mpu_set_bypass. */
	device->activeLowInt = TRUE;
	device->latchedInt = FALSE;
	device->intMotionOnly = FALSE;
	device->lpAccelMode = FALSE;
	device->cacheGyroFsr = 0;
	device->cacheAccelFsr = 0;
	device->cacheLpf = 0;
	device->cacheSampleRate = 0;
	device->cacheSensors = 0;
	device->cacheFifoSensors = 0;
	device->cacheDmpOn = 0;
	device->dmpOn = FALSE;
	device->dmpLoaded = 0;
	device->dmpSampleRate = 0;

	// Reset device
	buffer[0] = MPU9250_REGISTER_PWR_MGMT_1;
	buffer[1] = (1 << MPU9250_BIT_RESET);
	twiMasterSendData(device->slaveAddress, TWI_WRITE, buffer, 2);
	_delay_ms(100);

	// Wake up device
	device->sleep = FALSE;
	device->cycle = FALSE;
	device->gyroStandby = FALSE;
	device->pdPtat = FALSE;
	device->clkSel = MPU9250_CLK_SEL_INTERNAL;
	buffer[0] = MPU9250_REGISTER_PWR_MGMT_1;
	buffer[1] = 0x00;
	twiMasterSendData(device->slaveAddress, TWI_WRITE, buffer, 2);

	mpu9250SetGyroFsr(device, MPU9250_GYRO_FSR_2000_DPS);
	mpu9250SetAccelFsr(device, MPU9250_ACCEL_FSR_2G);
	mpu9250SetDlpf(device, MPU9250_DLPF_CFG_42_Hz);
	_mpu9250SetSampleRate(device, 50);
	mpu9250SetFifo(device, 0);
	// XX set interrupt callback reg_int_cb(int_param);
	mpu9250SetupCompass(device);
	mpu9250SetCompassSampleRate(device, 10);



	/*
	XX

	*/
	return TRUE;
}

bool_t _mpu9250LowPowerAccelMode(mpu9250_s * device, uint16_t sampleRate)
{
	uint8_t buffer[2];

	// Initial verifications
	if(sampleRate > 40) {						// Error 7 - Sample Rate too high for Low Power Mode
		device->error = MPU9250_ERROR_SAMPLE_RATE_TOO_HIGH;
		return FALSE;
	}

	if(sampleRate == 0) {						// Disables Low Power Mode
		mpu9250SetIntLatched(device, FALSE);
		buffer[0] = 0;
		tmp[1] = BIT_STBY_XYZG;
		if(i2c_write(st.hw->addr, st.reg->pwr_mgmt_1, 2, tmp)) {
			return -1;
		}
		st.chip_cfg.lp_accel_mode = 0;
		return 0;
	}

	if(!rate) {

	}
	* /
	/* For LP accel, we automatically configure the hardware to produce latched
	 * interrupts. In LP accel mode, the hardware cycles into sleep mode before
	 * it gets a chance to deassert the interrupt pin; therefore, we shift this
	 * responsibility over to the MCU.
	 *
	 * Any register read will clear the interrupt.
	 */
	/*
	mpu_set_int_latched(1);
	#if defined MPU6050
	tmp[0] = BIT_LPA_CYCLE;
	if(rate == 1) {
		tmp[1] = INV_LPA_1_25HZ;
		mpu_set_lpf(5);
	} else if(rate <= 5) {
		tmp[1] = INV_LPA_5HZ;
		mpu_set_lpf(5);
	} else if(rate <= 20) {
		tmp[1] = INV_LPA_20HZ;
		mpu_set_lpf(10);
	} else {
		tmp[1] = INV_LPA_40HZ;
		mpu_set_lpf(20);
	}
	tmp[1] = (tmp[1] << 6) | BIT_STBY_XYZG;
	if(i2c_write(st.hw->addr, st.reg->pwr_mgmt_1, 2, tmp)) {
		return -1;
	}
	#elif defined MPU6500
	// Set wake frequency.
	if(rate == 1) {
		tmp[0] = INV_LPA_1_25HZ;
	} else if(rate == 2) {
		tmp[0] = INV_LPA_2_5HZ;
	} else if(rate <= 5) {
		tmp[0] = INV_LPA_5HZ;
	} else if(rate <= 10) {
		tmp[0] = INV_LPA_10HZ;
	} else if(rate <= 20) {
		tmp[0] = INV_LPA_20HZ;
	} else if(rate <= 40) {
		tmp[0] = INV_LPA_40HZ;
	} else if(rate <= 80) {
		tmp[0] = INV_LPA_80HZ;
	} else if(rate <= 160) {
		tmp[0] = INV_LPA_160HZ;
	} else if(rate <= 320) {
		tmp[0] = INV_LPA_320HZ;
	} else {
		tmp[0] = INV_LPA_640HZ;
	}
	if(i2c_write(st.hw->addr, st.reg->lp_accel_odr, 1, tmp)) {
		return -1;
	}
	// Enable LP ACCEL mode, ACCEL_FCHOICE_B=1
	if(i2c_read(st.hw->addr, st.reg->accel_cfg2, 1, tmp)) {
		return -1;
	}

	tmp[0] = BIT_ACCL_FC_B | tmp[0];
	if(i2c_write(st.hw->addr, st.reg->accel_cfg2, 1, tmp)) {
		return -1;
	}
	tmp[0] = BIT_LPA_CYCLE;
	if(i2c_write(st.hw->addr, st.reg->pwr_mgmt_1, 1, tmp)) {
		return -1;
	}
	#endif
	st.chip_cfg.sensors = INV_XYZ_ACCEL;
	st.chip_cfg.clk_src = 0;
	st.chip_cfg.lp_accel_mode = 1;
	mpu_configure_fifo(0);

	return 0;
	*/
}

bool_t mpu9250SetAccelFsr(mpu9250_s * device, mpu9250AccelFsr_e accelFsr)
{
	uint8_t buffer[2];

	// Initial verifications
	if(!(device->activeSensorFlags)) {			// Error 1 - Sensors off
		device->error = MPU9250_ERROR_ALL_SENSORS_OFF;
		return FALSE;
	}
	if(device->accelFsr == accelFsr) {			// No need to process, just return
		return TRUE;
	}
	if(accelFsr > 3) {							// Error 3 - Invalid accel fsr value
		device->error = MPU9250_ERROR_INVALID_ACCEL_FSR_VALUE;
		return FALSE;
	}

	// Execute command
	device->accelFsr = accelFsr;
	buffer[0] = MPU9250_REGISTER_ACCEL_CONFIG;
	buffer[1] = accelFsr << MPU9250_BIT_ACCEL_FS_SEL;
	twiMasterSendData(device->slaveAddress, TWI_WRITE, buffer, 2);

	// Everything OK, just return
	return TRUE;
}

bool_t _mpu9250SetCompassSampleRate(mpu9250_s * device, uint16_t sampleRate)
{
	uint8_t div;

	// Initial verifications
	if(sampleRate == 0) {								// Error 6 - Sample Rate cannot be zero
		device->error = MPU9250_ERROR_SAMPLE_RATE_CANNOT_BE_ZERO;
		return FALSE;
	}
	if(sampleRate > device->sampleRate) {				// Error 7 - Sample Rate too high
		device->error = MPU9250_ERROR_SAMPLE_RATE_TOO_HIGH;
		return FALSE;
	}
	if(sampleRate > MPU9250_MAX_COMPASS_SAMPLE_RATE) {	// Error 7 - Sample Rate too high
		device->error = MPU9250_ERROR_SAMPLE_RATE_TOO_HIGH;
		return FALSE;
	}

	// Execute command
	div = (device->sampleRate / sampleRate) - 1;

	if(i2c_write(st.hw->addr, st.reg->s4_ctrl, 1, &div)) {
		return -1;
	}
	st.chip_cfg.compass_sample_rate = st.chip_cfg.sample_rate / (div + 1);
	return 0;
}

bool_t mpu9250SetDlpf(mpu9250_s * device, mpu9250DlpfCfg_e dlpfCfg)
{
	uint8_t buffer[2];

	// Initial verifications
	if(!(device->activeSensorFlags)) {			// Error 1 - Sensors off
		device->error = MPU9250_ERROR_ALL_SENSORS_OFF;
		return FALSE;
	}
	if(device->dlpfCfg == dlpfCfg) {			// No need to process, just return
		return TRUE;
	}
	if((dlpfCfg == 0) || (dlpfCfg > 6)) {		// Error 4 - Invalid dlpfCfg value
		device->error = MPU9250_ERROR_INVALID_DLPF_CFG_VALUE;
		return FALSE;
	}

	// Execute command
	device->dlpfCfg = dlpfCfg;
	buffer[0] = MPU9250_REGISTER_CONFIG;
	buffer[1] = (device->fifoMode << MPU9250_BIT_FIFO_MODE)
	            | (device->extSyncSet << MPU9250_BIT_EXT_SYNC_SET)
	            | (dlpfCfg << MPU9250_BIT_DLPF_CFG);
	twiMasterSendData(device->slaveAddress, TWI_WRITE, buffer, 2);

	// Everything OK, just return
	return TRUE;
}

bool_t mpu9250SetFifo(mpu9250_s * device, uint8_t fifo)
{

}

bool_t mpu9250SetGyroFsr(mpu9250_s * device, mpu9250GyroFsr_e gyroFsr)
{
	uint8_t buffer[2];

	// Initial verifications
	if(!(device->activeSensorFlags)) {			// Error 1 - Sensors off
		device->error = MPU9250_ERROR_ALL_SENSORS_OFF;
		return FALSE;
	}
	if(device->gyroFsr == gyroFsr) {			// No need to process, just return
		return TRUE;
	}
	if(gyroFsr > 3) {							// Error 2 - Invalid gyro fsr value
		device->error = MPU9250_ERROR_INVALID_GYRO_FSR_VALUE;
		return FALSE;
	}

	// Execute command
	device->gyroFsr = gyroFsr;
	device->fchoiceB = 0;
	buffer[0] = MPU9250_REGISTER_GYRO_CONFIG;
	buffer[1] = gyroFsr << MPU9250_BIT_GYRO_FS_SEL;
	twiMasterSendData(device->slaveAddress, TWI_WRITE, buffer, 2);

	// Everything OK, just return
	return TRUE;
}

bool_t _mpu9250SetIntLatched(mpu9250_s * device, bool_t enable)
{

}

bool_t _mpu9250SetSampleRate(mpu9250_s * device, uint16_t sampleRate)
{
	uint8_t buffer[2];

	// Initial verifications
	if(!(device->activeSensorFlags)) {			// Error 1 - Sensors off
		device->error = MPU9250_ERROR_ALL_SENSORS_OFF;
		return FALSE;
	}
	if(device->dmpOn) {							// Error 5 - DMP is turned on
		device->error = MPU9250_ERROR_CANNOT_BE_SET_WITH_DMP_ON;
		return FALSE;
	}
	if(sampleRate == 0) {						// Error 6 - Sample Rate cannot be zero
		device->error = MPU9250_ERROR_SAMPLE_RATE_CANNOT_BE_ZERO;
		return FALSE;
	}

	// Disables Low Power Mode, if necessary
	if(device->lpAccelMode) {
		if(sampleRate <= 40) {					// Just stay in low-power accel mode
			mpu9250LowPowerAccelMode(device, sampleRate);
			return TRUE;
		}
		// Requested rate exceeds the allowed frequencies in LP accel mode, switch back to full-power mode
		mpu9250LowPowerAccelMode(device, 0);
	}

	// Fix sampleRate limits
	sampleRate = (sampleRate < 4) ? 4 : (sampleRate > 1000) ? 1000 : sampleRate;

	// Execute command
	buffer[0] = MPU9250_REGISTER_SMPLRT_DIV;
	buffer[1] = (1000 / sampleRate) - 1;
	device->sampleRate = sampleRate;
	twiMasterSendData(device->slaveAddress, TWI_WRITE, buffer, 2);

	// Compass sample Rate
	mpu9250SetCompassSampleRate(device, 0/* min(st.chip_cfg.compass_sample_rate, MAX_COMPASS_SAMPLE_RATE) */);

	/* Automatically set LPF to 1/2 sampling rate. */
	// XX mpu9250SetDlpf(device, st.chip_cfg.sample_rate >> 1);

	return TRUE;
}

bool_t mpu9250SetupCompass(mpu9250_s * device)
{

}


