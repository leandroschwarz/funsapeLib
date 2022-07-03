/**
	\file		mpu9250Spi.c
	\author		Hazael dos Santos Batista
	\author		Leandro Schwarz
	\date		April 3, 2018
	\version	1
	\brief		MPU9250 Interface Driver Using SPI
*/

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "mpu9250Spi.h"
#if __MPU9250_SPI_H != 1
	#error Error 101 - Build mismatch on header and source code files (mpu9250Spi).
#endif

// -----------------------------------------------------------------------------
// Constant definitions  (used only in this file) ------------------------------

#define csLow()			while(0)
#define csHigh()		while(0)
#define waitForMs(n)	_delay_ms(n)

#define MPU9250_MAG_SLAVE_ADDRESS		0x0C
#define MPU9250_RESET_OFFSET			7
#define MPU9250_SLEEP_OFFSET			6
#define MPU9250_CYCLE_OFFSET			5
#define MPU9250_GYRO_STANDBY_OFFSET		4
#define MPU9250_PD_PDAT_OFFSET			3
#define MPU9250_CLKSEL_OFFSET			2
#define MPU9250_GYR_RES_SIG_PAT_OFFSET	2
#define MPU9250_ACC_RES_SIG_PAT_OFFSET	1
#define MPU9250_TEMP_RES_SIG_PAT_OFFSET	0
#define MPU9250_GYR_SCALE_OFFSET		3
#define MPU9250_ACC_SCALE_OFFSET		3

// -----------------------------------------------------------------------------
// New data types  (used only in this file) ------------------------------------

/**
	\brief	Register Adresses of the MPU9250

	This data type is used to rename the MPU9250 internal register addresses to
	a more friendly name.

*/
typedef enum mpu9250Register_t {
	// GENERAL
	MPU9250_SMPLRT_DIV_REG			= 0x19,
	MPU9250_CONFIG_REG				= 0x1A,
	MPU9250_PWR_MGMT_1_REG			= 0x6B,
	MPU9250_PWR_MGMT_2_REG			= 0x6C,
	MPU9250_WHO_AM_I_REG			= 0x75,	/**< MPU9250 WHO_AM_I. When read, must be 0x68 */
	// GYR
	MPU9250_GYR_X_SELFTEST_REG		= 0x00,	/**< Self test register for gyroscope X axis */
	MPU9250_GYR_Y_SELFTEST_REG		= 0x01,	/**< Self test register for gyroscope Y axis */
	MPU9250_GYR_Z_SELFTEST_REG		= 0x02,	/**< Self test register for gyroscope Z axis */
	MPU9250_GYR_X_OFFSET_H_REG		= 0x13,
	MPU9250_GYR_X_OFFSET_L_REG		= 0x14,
	MPU9250_GYR_Y_OFFSET_H_REG		= 0x15,
	MPU9250_GYR_Y_OFFSET_L_REG		= 0x16,
	MPU9250_GYR_Z_OFFSET_H_REG		= 0x17,
	MPU9250_GYR_Z_OFFSET_L_REG		= 0x18,
	MPU9250_GYR_CONFIG_REG			= 0x1B,
	MPU9250_GYR_OUT_X_H_REG			= 0x43,
	MPU9250_GYR_OUT_X_L_REG			= 0x44,
	MPU9250_GYR_OUT_Y_H_REG			= 0x45,
	MPU9250_GYR_OUT_Y_L_REG			= 0x46,
	MPU9250_GYR_OUT_Z_H_REG			= 0x47,
	MPU9250_GYR_OUT_Z_L_REG			= 0x48,
	// ACC
	MPU9250_ACC_X_SELFTEST_REG		= 0x0D,
	MPU9250_ACC_Y_SELFTEST_REG		= 0x0E,
	MPU9250_ACC_Z_SELFTEST_REG		= 0x0F,
	MPU9250_ACC_X_OFFSET_H_REG		= 0x77,
	MPU9250_ACC_X_OFFSET_L_REG		= 0x78,
	MPU9250_ACC_Y_OFFSET_H_REG		= 0x7A,
	MPU9250_ACC_Y_OFFSET_L_REG		= 0x7B,
	MPU9250_ACC_Z_OFFSET_H_REG		= 0x7D,
	MPU9250_ACC_Z_OFFSET_L_REG		= 0x7E,
	MPU9250_ACC_CONFIG_1_REG		= 0x1C,
	MPU9250_ACC_CONFIG_2_REG		= 0x1D,
	MPU9250_ACC_LP_ODR_REG			= 0x1E,
	MPU9250_ACC_OUT_X_H_REG			= 0x3B,
	MPU9250_ACC_OUT_X_L_REG			= 0x3C,
	MPU9250_ACC_OUT_Y_H_REG			= 0x3D,
	MPU9250_ACC_OUT_Y_L_REG			= 0x3E,
	MPU9250_ACC_OUT_Z_H_REG			= 0x3F,
	MPU9250_ACC_OUT_Z_L_REG			= 0x40,
	// TEMP
	MPU9250_TEMP_OUT_H_REG			= 0x41,
	MPU9250_TEMP_OUT_L_REG			= 0x42,
	// FIFO
	MPU9250_FIFO_EN_REG				= 0x23,
	MPU9250_FIFO_COUNT_H_REG		= 0x72,
	MPU9250_FIFO_COUNT_L_REG		= 0x73,
	MPU9250_FIFO_R_W_REG			= 0x74,
	// I2C
	MPU9250_I2C_MST_CTRL_REG		= 0x24,
	MPU9250_I2C_SLV0_ADDR_REG		= 0x25,
	MPU9250_I2C_SLV0_REG			= 0x26,
	MPU9250_I2C_SLV0_CTRL_REG		= 0x27,
	MPU9250_I2C_SLV1_ADDR_REG		= 0x28,
	MPU9250_I2C_SLV1_REG			= 0x29,
	MPU9250_I2C_SLV1_CTRL_REG		= 0x2A,
	MPU9250_I2C_SLV2_ADDR_REG		= 0x2B,
	MPU9250_I2C_SLV2_REG			= 0x2C,
	MPU9250_I2C_SLV2_CTRL_REG		= 0x2D,
	MPU9250_I2C_SLV3_ADDR_REG		= 0x2E,
	MPU9250_I2C_SLV3_REG			= 0x2F,
	MPU9250_I2C_SLV3_CTRL_REG		= 0x30,
	MPU9250_I2C_SLV4_ADDR_REG		= 0x31,
	MPU9250_I2C_SLV4_REG			= 0x32,
	MPU9250_I2C_SLV4_DO_REG			= 0x33,
	MPU9250_I2C_SLV4_CTRL_REG		= 0x34,
	MPU9250_I2C_SLV4_DI_REG			= 0x35,
	MPU9250_I2C_MST_STATUS_REG		= 0x36,
	MPU9250_I2C_SLV0_DO_REG			= 0x63,
	MPU9250_I2C_SLV1_DO_REG			= 0x64,
	MPU9250_I2C_SLV2_DO_REG			= 0x65,
	MPU9250_I2C_SLV3_DO_REG			= 0x66,
	MPU9250_I2C_MST_DELAY_CTRL_REG	= 0x67,
	// INTERRUPT
	MPU9250_INT_PIN_CFG_REG			= 0x37,
	MPU9250_INT_ENABLE_REG			= 0x38,
	MPU9250_INT_STATUS_REG			= 0x3A,
	// EXTERNAL
	MPU9250_EXT_SENS_DATA_00_REG	= 0x49,
	MPU9250_EXT_SENS_DATA_01_REG	= 0x4A,
	MPU9250_EXT_SENS_DATA_02_REG	= 0x4B,
	MPU9250_EXT_SENS_DATA_03_REG	= 0x4C,
	MPU9250_EXT_SENS_DATA_04_REG	= 0x4D,
	MPU9250_EXT_SENS_DATA_05_REG	= 0x4E,
	MPU9250_EXT_SENS_DATA_06_REG	= 0x4F,
	MPU9250_EXT_SENS_DATA_07_REG	= 0x50,
	MPU9250_EXT_SENS_DATA_08_REG	= 0x51,
	MPU9250_EXT_SENS_DATA_09_REG	= 0x52,
	MPU9250_EXT_SENS_DATA_10_REG	= 0x53,
	MPU9250_EXT_SENS_DATA_11_REG	= 0x54,
	MPU9250_EXT_SENS_DATA_12_REG	= 0x55,
	MPU9250_EXT_SENS_DATA_13_REG	= 0x56,
	MPU9250_EXT_SENS_DATA_14_REG	= 0x57,
	MPU9250_EXT_SENS_DATA_15_REG	= 0x58,
	MPU9250_EXT_SENS_DATA_16_REG	= 0x59,
	MPU9250_EXT_SENS_DATA_17_REG	= 0x5A,
	MPU9250_EXT_SENS_DATA_18_REG	= 0x5B,
	MPU9250_EXT_SENS_DATA_19_REG	= 0x5C,
	MPU9250_EXT_SENS_DATA_20_REG	= 0x5D,
	MPU9250_EXT_SENS_DATA_21_REG	= 0x5E,
	MPU9250_EXT_SENS_DATA_22_REG	= 0x5F,
	MPU9250_EXT_SENS_DATA_23_REG	= 0x60,
	// DONT KNOW
	MPU9250_WOM_THR_REG				= 0x1F,
	MPU9250_SIGNAL_PATH_RESET_REG	= 0x68,
	MPU9250_MOT_DETECT_CTRL_REG		= 0x69,
	MPU9250_USER_CTRL_REG			= 0x6A,
	// MAG
	MPU9250_MAG_WHOAMI_REG			= 0x00,
	MPU9250_MAG_INFO_REG			= 0x01,
	MPU9250_MAG_ST_1_REG			= 0x02,
	MPU9250_MAG_OUT_X_L_REG			= 0x03,
	MPU9250_MAG_OUT_X_H_REG			= 0x04,
	MPU9250_MAG_OUT_Y_L_REG			= 0x05,
	MPU9250_MAG_OUT_Y_H_REG			= 0x06,
	MPU9250_MAG_OUT_Z_L_REG			= 0x07,
	MPU9250_MAG_OUT_Z_H_REG			= 0x08,
	MPU9250_MAG_ST_2_REG			= 0x09,
	MPU9250_MAG_CNTL_REG			= 0x0A,
	MPU9250_MAG_SELFTEST_REG		= 0x0C,
	MPU9250_MAG_TS_1_REG			= 0x0D,
	MPU9250_MAG_TS_2_REG			= 0x0E,
	MPU9250_MAG_I2C_DIS_REG			= 0x0F,
	MPU9250_MAG_SENS_X_REG			= 0x10,
	MPU9250_MAG_SENS_Y_REG			= 0x11,
	MPU9250_MAG_SENS_Z_REG			= 0x12
} mpu9250Register_t;

// -----------------------------------------------------------------------------
// Static function declarations ------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Public function definitions -------------------------------------------------


void mpu9250Init(mpu9250_t * device)
{
	uint8_t data[2];

	// Reset device
	data[0] = MPU9250_PWR_MGMT_1_REG;
	device->reset = TRUE;
	data[1] = 1 << MPU9250_RESET_OFFSET;
	csLow();
	spiMasterSendReceiveData(data, data, 2);
	csHigh();
	waitForMs(100);

	// Reset signal path
	data[0] = MPU9250_SIGNAL_PATH_RESET_REG;
	data[1] = (1 << MPU9250_GYR_RES_SIG_PAT_OFFSET) | (1 << MPU9250_ACC_RES_SIG_PAT_OFFSET) | (1 << MPU9250_TEMP_RES_SIG_PAT_OFFSET);
	csLow();
	spiMasterSendReceiveData(data, data, 2);
	csHigh();
	waitForMs(1);

	// Wake up device
	data[0] = MPU9250_PWR_MGMT_1_REG;
	device->reset = FALSE;
	data[1] = 0x01;		// (Clock source)
	csLow();
	spiMasterSendReceiveData(data, data, 2);
	csHigh();

	return;
}

void mpu9250SetSampleRateDefault(mpu9250_t * device)
{


	return;
}

void mpu9250GyrConfig(mpu9250_t * device, mpu9250GyrScale_t scale)
{
	uint8_t data[2];

	data[0] = MPU9250_GYR_CONFIG_REG;
	device->gyrScale = scale;
	data[1] = scale << MPU9250_GYR_SCALE_OFFSET;
	csLow();
	spiMasterSendReceiveData(data, data, 2);
	csHigh();

	return;
}

void mpu9250AccConfig(mpu9250_t * device, mpu9250AccScale_t scale)
{
	uint8_t data[2];

	data[0] = MPU9250_ACC_CONFIG_1_REG;
	device->accScale = scale;
	data[1] = scale << MPU9250_ACC_SCALE_OFFSET;
	csLow();
	spiMasterSendReceiveData(data, data, 2);
	csHigh();

	return;
}









































/**
 *  @brief      Enable/disable data ready interrupt.
 *  If the DMP is on, the DMP interrupt is enabled. Otherwise, the data ready
 *  interrupt is used.
 *  @param[in]  enable      TRUE to enable interrupt.
 *  @return     MPU9250_OK if successful.
 */
static mpu9250Response_t mpu9250EnableInterrupt(bool_t enable)
{
	uint8_t aux8;

	if(st.chip_cfg.dmp_on) {
		if(enable) {
			tmp = BIT_DMP_INT_EN;
		} else {
			tmp = 0x00;
		}
		if(i2c_write(st.hw->addr, st.reg->int_enable, 1, &tmp)) {
			return -1;
		}
		st.chip_cfg.int_enable = tmp;
	} else {
		if(!st.chip_cfg.sensors) {
			return -1;
		}
		if(enable && st.chip_cfg.int_enable) {
			return 0;
		}
		if(enable) {
			tmp = BIT_DATA_RDY_EN;
		} else {
			tmp = 0x00;
		}
		if(i2c_write(st.hw->addr, st.reg->int_enable, 1, &tmp)) {
			return -1;
		}
		st.chip_cfg.int_enable = tmp;
	}
	return 0;
}
