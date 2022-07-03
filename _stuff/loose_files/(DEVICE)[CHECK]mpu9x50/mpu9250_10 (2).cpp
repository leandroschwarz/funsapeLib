
#include "mpu9250.h"

typedef enum mpu9250Register_e {
	MPU9250_REGISTER_INT_ENABLE				= 0x38,
	MPU9250_REGISTER_FIFO_R_W				= 0x74,
	MPU9250_REGISTER_PWR_MGMT_1				= 0x6B,
	MPU9250_REGISTER_PWR_MGMT_2				= 0x6C,
	MPU9250_REGISTER_ACCEL_CONFIG_2			= 0x1D,
	MPU9250_REGISTER_LP_ACCEL_ODR			= 0x1E,
	MPU9250_REGISTER_GYRO_XOUT_H			= 0x43,
	MPU9250_REGISTER_GYRO_XOUT_L			= 0x44,
	MPU9250_REGISTER_GYRO_YOUT_H			= 0x45,
	MPU9250_REGISTER_GYRO_YOUT_L			= 0x46,
	MPU9250_REGISTER_GYRO_ZOUT_H			= 0x47,
	MPU9250_REGISTER_GYRO_ZOUT_L			= 0x48,
	MPU9250_REGISTER_ACCEL_XOUT_H			= 0x3B,
	MPU9250_REGISTER_ACCEL_XOUT_L			= 0x2C,
	MPU9250_REGISTER_ACCEL_YOUT_H			= 0x3D,
	MPU9250_REGISTER_ACCEL_YOUT_L			= 0x3E,
	MPU9250_REGISTER_ACCEL_ZOUT_H			= 0x3F,
	MPU9250_REGISTER_ACCEL_ZOUT_L			= 0x40,
	MPU9250_REGISTER_TEMP_OUT_H				= 0x41,
	MPU9250_REGISTER_TEMP_OUT_L				= 0x42,
	MPU9250_REGISTER_USER_CTRL				= 0x6A,
	MPU9250_REGISTER_FIFO_EN				= 0x23,
	MPU9250_REGISTER_ACCEL_CONFIG			= 0x1C,
	MPU9250_REGISTER_GYRO_CONFIG			= 0x1B,
	MPU9250_REGISTER_CONFIG					= 0x1A,
	MPU9250_REGISTER_SMPLRT_DIV				= 0x19,
	MPU9250_REGISTER_I2C_SLV4_CTRL			= 0x34,
	MPU9250_REGISTER_I2C_SLV0_DO			= 0x63,
	MPU9250_REGISTER_INT_STATUS				= 0x3A,
	MPU9250_REGISTER_FIFO_COUNT_H			= 0x72,
	MPU9250_REGISTER_FIFO_COUNT_L			= 0x73,
	MPU9250_REGISTER_INT_PIN_CFG			= 0x37,
	MPU9250_REGISTER_I2C_MST_STATUS			= 0x36,
	MPU9250_REGISTER_I2C_SLV0_ADDR			= 0x25,
	MPU9250_REGISTER_I2C_SLV0_REG			= 0x26,
	MPU9250_REGISTER_I2C_SLV0_CTRL			= 0x27,
	MPU9250_REGISTER_I2C_SLV1_ADDR			= 0x28,
	MPU9250_REGISTER_I2C_SLV1_REG			= 0x29,
	MPU9250_REGISTER_I2C_SLV1_CTRL			= 0x2A,
	MPU9250_REGISTER_I2C_MST_DELAY_CTRL		= 0x67,
	MPU9250_REGISTER_EXT_SENS_DATA_00		= 0x49,
	MPU9250_REGISTER_MOT_DETECT_CTRL		= 0x69,
	MPU9250_REGISTER_WOM_THR				= 0x1F,

	// 	MPU9250_REGISTER_WHO_AM_I				= 0x75,
	// 	MPU9250_REGISTER_XA_OFFSET_H			= 0x77,
	//	.motion_dur     = 0x20,
	// 	MPU9250_REGISTER_I2C_MST_CTRL			= 0x24,
	// 	MPU9250_REGISTER_I2C_SLV2_ADDR			= 0x2B,
	// 	MPU9250_REGISTER_I2C_SLV2_REG			= 0x2C,
	// 	MPU9250_REGISTER_I2C_SLV2_CTRL			= 0x2D,
	// 	MPU9250_REGISTER_I2C_SLV3_ADDR			= 0x2E,
	// 	MPU9250_REGISTER_I2C_SLV3_REG			= 0x2F,
	// 	MPU9250_REGISTER_I2C_SLV3_CTRL			= 0x30,
	// 	MPU9250_REGISTER_I2C_SLV4_ADDR			= 0x31,
	// 	MPU9250_REGISTER_I2C_SLV4_REG			= 0x32,
	// 	MPU9250_REGISTER_I2C_SLV4_DO			= 0x33,
	// 	MPU9250_REGISTER_I2C_SLV4_DI			= 0x35,
	// 	MPU9250_REGISTER_I2C_SLV1_DO			= 0x64,
	//	.prod_id        = 0x0C,
	// 	.mem_r_w        = 0x6F,
	// 	.bank_sel       = 0x6D,
	// 	.mem_start_addr = 0x6E,
	// 	.prgm_start_h   = 0x70,
	// RAW COMPASS = 0x49
	// To do register list
	// 	MPU9250_REGISTER_SELF_TEST_X_GYRO		= 0x00,
	// 	MPU9250_REGISTER_SELF_TEST_Y_GYRO		= 0x01,
	// 	MPU9250_REGISTER_SELF_TEST_Z_GYRO		= 0x02,
	// 	MPU9250_REGISTER_SELF_TEST_X_ACCEL		= 0x0D,
	// 	MPU9250_REGISTER_SELF_TEST_Y_ACCEL		= 0x0E,
	// 	MPU9250_REGISTER_SELF_TEST_Z_ACCEL		= 0x0F,
	// 	MPU9250_REGISTER_XG_OFFSET_H			= 0x13,
	// 	MPU9250_REGISTER_XG_OFFSET_L			= 0x14,
	// 	MPU9250_REGISTER_YG_OFFSET_H			= 0x15,
	// 	MPU9250_REGISTER_YG_OFFSET_L			= 0x16,
	// 	MPU9250_REGISTER_ZG_OFFSET_H			= 0x17,
	// 	MPU9250_REGISTER_ZG_OFFSET_L			= 0x18,
	// 	MPU9250_REGISTER_EXT_SENS_DATA_01		= 0x4A,
	// 	MPU9250_REGISTER_EXT_SENS_DATA_02		= 0x4B,
	// 	MPU9250_REGISTER_EXT_SENS_DATA_03		= 0x4C,
	// 	MPU9250_REGISTER_EXT_SENS_DATA_04		= 0x4D,
	// 	MPU9250_REGISTER_EXT_SENS_DATA_05		= 0x4E,
	// 	MPU9250_REGISTER_EXT_SENS_DATA_06		= 0x4F,
	// 	MPU9250_REGISTER_EXT_SENS_DATA_07		= 0x50,
	// 	MPU9250_REGISTER_EXT_SENS_DATA_08		= 0x51,
	// 	MPU9250_REGISTER_EXT_SENS_DATA_09		= 0x52,
	// 	MPU9250_REGISTER_EXT_SENS_DATA_10		= 0x53,
	// 	MPU9250_REGISTER_EXT_SENS_DATA_11		= 0x54,
	// 	MPU9250_REGISTER_EXT_SENS_DATA_12		= 0x55,
	// 	MPU9250_REGISTER_EXT_SENS_DATA_13		= 0x56,
	// 	MPU9250_REGISTER_EXT_SENS_DATA_14		= 0x57,
	// 	MPU9250_REGISTER_EXT_SENS_DATA_15		= 0x58,
	// 	MPU9250_REGISTER_EXT_SENS_DATA_16		= 0x59,
	// 	MPU9250_REGISTER_EXT_SENS_DATA_17		= 0x5A,
	// 	MPU9250_REGISTER_EXT_SENS_DATA_18		= 0x5B,
	// 	MPU9250_REGISTER_EXT_SENS_DATA_19		= 0x5C,
	// 	MPU9250_REGISTER_EXT_SENS_DATA_20		= 0x5D,
	// 	MPU9250_REGISTER_EXT_SENS_DATA_21		= 0x5E,
	// 	MPU9250_REGISTER_EXT_SENS_DATA_22		= 0x5F,
	// 	MPU9250_REGISTER_EXT_SENS_DATA_23		= 0x60,
	// 	MPU9250_REGISTER_I2C_SLV2_DO			= 0x65,
	// 	MPU9250_REGISTER_I2C_SLV3_DO			= 0x66,
	// 	MPU9250_REGISTER_SIGNAL_PATH_RESET		= 0x68,
	// 	MPU9250_REGISTER_XA_OFFSET_L			= 0x78,
	// 	MPU9250_REGISTER_YA_OFFSET_H			= 0x7A,
	// 	MPU9250_REGISTER_YA_OFFSET_L			= 0x7B,
	// 	MPU9250_REGISTER_ZA_OFFSET_H			= 0x7D,
	// 	MPU9250_REGISTER_ZA_OFFSET_L			= 0x7E
} mpu9250Register_e;

typedef enum mpu9250Bits_e {
	// Gyro Config register			0x1B
	MPU9250_BIT_GYRO_X_ST_EN			= 7,
	MPU9250_BIT_GYRO_Y_ST_EN			= 6,
	MPU9250_BIT_GYRO_Z_ST_EN			= 5,
	MPU9250_BIT_GYRO_FULL_SCALE_SEL		= 3,
	MPU9250_BIT_FCHOICE_B				= 0,
	// Accel Config register		0x1C
	MPU9250_BIT_ACCEL_X_ST_EN			= 7,
	MPU9250_BIT_ACCEL_Y_ST_EN			= 6,
	MPU9250_BIT_ACCEL_Z_ST_EN			= 5,
	MPU9250_BIT_ACCEL_FULL_SCALE_SEL	= 3,
	// Power Management 1 register	0x6B
	MPU9250_BIT_RESET					= 7,
	MPU9250_BIT_SLEEP					= 6,
	MPU9250_BIT_CYCLE					= 5,
	MPU9250_BIT_GYRO_STANDBY			= 4,
	MPU9250_BIT_PD_PTAT					= 3,
	MPU9250_BIT_CLK_SEL					= 0,

} mpu9250Bits_e;

void mpu9250CreateDevice(mpu9250_s * device, mpu9250CommInterface_e commInterface)
{
	device->initialized = FALSE;
	device->commInterface = commInterface;
	device->slaveAddress = MPU9250_TWI_SLAVE_ADDRESS_0x68;
	device->csBit = 0;
	device->csDdr = NULL;
	device->csPort = NULL;
	device->sleep = FALSE;
	device->cycle = FALSE;
	device->gyroStandby = FALSE;
	device->pdPtat = FALSE;
	device->clkSel = MPU9250_CLK_SEL_INTERNAL;
	device->gyroFullScale = MPU9250_GYRO_FULL_SCALE_250_DPS;
	device->fchoiceB = 0;
	device->accelFullScale = MPU9250_ACCEL_FULL_SCALE_2_G;
	device->accelLowPowerMode = FALSE;
	device->sampleRate = 1000;
	device->compassSampleRate = 100;

	device->unusedBits = 0;
}

bool_t mpu9250TwiInit(mpu9250_s * device, mpu9250TwiSlaveAddress_e slaveAddress)
{
	// Update struct
	device->bufferData[0] = MPU9250_REGISTER_PWR_MGMT_1;
	device->bufferData[1] = 1 << MPU9250_BIT_RESET;
	device->bufferLength = 2;
	// Reseting device
	mpu9250WriteData(device);
	_delay_ms(10);

	// Update struct
	device->bufferData[1] = 0x00;
	device->bufferLength = 2;
	// Waking up chip
	mpu9250WriteData(device);

	return TRUE;
}

bool_t mpu9250SetGyroFullScale(mpu9250_s * device, mpu9250GyroFullScale_e gyroFs)
{
	if(device->gyroFullScale == gyroFs) {
		// Nothing to do, just return
		return TRUE;
	}

	// Update struct
	device->gyroFullScale = gyroFs;
	device->bufferData[0] = MPU9250_REGISTER_GYRO_CONFIG;
	device->bufferData[1] = (gyroFs				<< MPU9250_BIT_GYRO_FULL_SCALE_SEL) |
	                        (device->fchoiceB	<< MPU9250_BIT_FCHOICE_B);
	device->bufferLength = 2;
	// Changing gyroscope full scale
	mpu9250WriteData(device);

	return TRUE;
}

bool_t mpu9250SetAccelFullScale(mpu9250_s * device, mpu9250AccelFullScale_e accelFs)
{
	if(device->accelFullScale == accelFs) {
		// Nothing to do, just return
		return TRUE;
	}

	// Update struct
	device->accelFullScale = accelFs;
	device->bufferData[0] = MPU9250_REGISTER_ACCEL_CONFIG;
	device->bufferData[1] = (accelFs << MPU9250_BIT_ACCEL_FULL_SCALE_SEL);
	device->bufferLength = 2;
	// Changing accelerometer full scale
	mpu9250WriteData(device);

	return TRUE;
}

bool_t mpu9250SetSampleRate(mpu9250_s * device, uint16_t sampleRate)
{
	if(sampleRate == 0) {
		// Sample rate cannot be zero
		return FALSE;
	}
	if((device->accelLowPowerMode) && (sampleRate > 40)) {
		// Exit from low power mode
		mpu9250SetAccelLowPowerMode(device, FALSE);
	}

	// Verify sampleRate limits
	sampleRate = truncateToLimits(sampleRate, 4, 1000);

	// Change Sample Rate
	device->sampleRate = (1000 / sampleRate) - 1;
	device->bufferData[0] = MPU9250_REGISTER_SMPLRT_DIV;
	device->bufferData[1] = device->sampleRate;
	device->bufferLength = 2;
	mpu9250WriteData(device);

	return TRUE;
}

bool_t mpu9250SetDlpf(mpu9250_s * device, mpu9250AccelFullScale_e accelFs)
{
	/*
		if(lpf >= 188) {
			data = INV_FILTER_188HZ;
		} else if(lpf >= 98) {
			data = INV_FILTER_98HZ;
		} else if(lpf >= 42) {
			data = INV_FILTER_42HZ;
		} else if(lpf >= 20) {
			data = INV_FILTER_20HZ;
		} else if(lpf >= 10) {
			data = INV_FILTER_10HZ;
		} else {
			data = INV_FILTER_5HZ;
		}

		if(st.chip_cfg.lpf == data) {
			return 0;
		}
		if(i2c_write(st.hw->addr, MPU9250_REGISTER_CONFIG, 1, &data)) {
			return -1;
		}
		data = BIT_FIFO_SIZE_1024 | data;

		if(i2c_write(st.hw->addr, MPU9250_REGISTER_ACCEL_CONFIG_2, 1, &data)) {
			return -1;
		}
		st.chip_cfg.lpf = data;
	*/
	return 0;
}

/*

// Set to invalid values to ensure no I2C writes are skipped. //
st.chip_cfg.sensors = 0xFF;
st.chip_cfg.gyro_fsr = 0xFF;
st.chip_cfg.accel_fsr = 0xFF;
st.chip_cfg.lpf = 0xFF;
st.chip_cfg.sample_rate = 0xFFFF;
st.chip_cfg.fifo_enable = 0xFF;
st.chip_cfg.bypass_mode = 0xFF;
st.chip_cfg.compass_sample_rate = 0xFFFF;
// mpu_set_sensors always preserves this setting. //
st.chip_cfg.clk_src = INV_CLK_PLL;
// Handled in next call to mpu_set_bypass. //
st.chip_cfg.active_low_int = 1;
st.chip_cfg.latched_int = 0;
st.chip_cfg.int_motion_only = 0;
st.chip_cfg.lp_accel_mode = 0;
memset(&st.chip_cfg.cache, 0, sizeof(st.chip_cfg.cache));
st.chip_cfg.dmp_on = 0;
st.chip_cfg.dmp_loaded = 0;
st.chip_cfg.dmp_sample_rate = 0;

if(mpu_set_lpf(42))
{
	return -1;
}

if(mpu_set_sample_rate(50))
{
	return -1;
}

if(mpu_configure_fifo(0))
{
	return -1;
}

if(int_param)
{
	reg_int_cb(int_param);
}

setup_compass();

if(mpu_set_compass_sample_rate(10))
{
	return -1;
}

mpu_set_sensors(0);
*/

bool_t mpu9250WriteData(mpu9250_s * device)
{
	if(device->commInterface == MPU9250_COMM_INTERFACE_TWI) {
		twiMasterSendData(device->slaveAddress, TWI_WRITE, device->bufferData, device->bufferLength);
	} else if(MPU9250_COMM_INTERFACE_SPI) {
		//
	}

	return TRUE;
}
