/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			mpu9250.h
 * Module:			MPU9250 9-axis IMU
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	April 18, 2018
 * Purpose:			Driver interface for MPU9250.
 * ---------------------------------------------------------------------------*/

#ifndef __MPU9250_H_
#define __MPU9250_H_ 1

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#if __GLOBAL_DEFINES_H != 1
	#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 1).
#endif
#include <string.h>
#include "twiMaster.h"

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

#define truncateToLimits(variable, minimum, maximum) (variable > maximum) ? maximum : (variable < minimum) ? minimum : variable
#define returnMinimum(variable, minimum) (variable > minimum) ? minimum : variable
#define returnMaximum(variable, maximum) (variable < maximum) ? maximum : variable

#define INV_X_GYRO      (0x40)
#define INV_Y_GYRO      (0x20)
#define INV_Z_GYRO      (0x10)
#define INV_XYZ_GYRO    (INV_X_GYRO | INV_Y_GYRO | INV_Z_GYRO)
#define INV_XYZ_ACCEL   (0x08)
#define INV_XYZ_COMPASS (0x01)
//
// #define MPU_INT_STATUS_DATA_READY       (0x0001)
// #define MPU_INT_STATUS_DMP              (0x0002)
// #define MPU_INT_STATUS_PLL_READY        (0x0004)
// #define MPU_INT_STATUS_I2C_MST          (0x0008)
// #define MPU_INT_STATUS_FIFO_OVERFLOW    (0x0010)
// #define MPU_INT_STATUS_ZMOT             (0x0020)
// #define MPU_INT_STATUS_MOT              (0x0040)
// #define MPU_INT_STATUS_FREE_FALL        (0x0080)
// #define MPU_INT_STATUS_DMP_0            (0x0100)
// #define MPU_INT_STATUS_DMP_1            (0x0200)
// #define MPU_INT_STATUS_DMP_2            (0x0400)
// #define MPU_INT_STATUS_DMP_3            (0x0800)
// #define MPU_INT_STATUS_DMP_4            (0x1000)
// #define MPU_INT_STATUS_DMP_5            (0x2000)
//
#define INT_PORT_P1     (0x100)
// #define INT_PIN_P10     (0x102)
// #define INT_PIN_P11     (0x104)
// #define INT_PIN_P12     (0x106)
// #define INT_PIN_P13     (0x108)
// #define INT_PIN_P14     (0x10A)
// #define INT_PIN_P15     (0x10C)
// #define INT_PIN_P16     (0x10E)
// #define INT_PIN_P17     (0x110)
//
#define INT_PORT_P2     (0x200)
// #define INT_PIN_P20     (0x202)
// #define INT_PIN_P21     (0x204)
// #define INT_PIN_P22     (0x206)
// #define INT_PIN_P23     (0x208)
// #define INT_PIN_P24     (0x20A)
// #define INT_PIN_P25     (0x20C)
// #define INT_PIN_P26     (0x20E)
// #define INT_PIN_P27     (0x210)
//
// #define BIT_I2C_MST_VDDIO   (0x80)
#define BIT_FIFO_EN         (0x40)
#define BIT_DMP_EN          (0x80)
#define BIT_FIFO_RST        (0x04)
#define BIT_DMP_RST         (0x08)
#define BIT_FIFO_OVERFLOW   (0x10)
#define BIT_DATA_RDY_EN     (0x01)
#define BIT_DMP_INT_EN      (0x02)
#define BIT_MOT_INT_EN      (0x40)
// #define BITS_FSR            (0x18)
// #define BITS_LPF            (0x07)
// #define BITS_HPF            (0x07)
// #define BITS_CLK            (0x07)
#define BIT_FIFO_SIZE_1024  (0x40)
#define BIT_FIFO_SIZE_2048  (0x80)
#define BIT_FIFO_SIZE_4096  (0xC0)
#define BIT_RESET           (0x80)
#define BIT_SLEEP           (0x40)
// #define BIT_S0_DELAY_EN     (0x01)
// #define BIT_S2_DELAY_EN     (0x04)
// #define BITS_SLAVE_LENGTH   (0x0F)
// #define BIT_SLAVE_BYTE_SW   (0x40)
// #define BIT_SLAVE_GROUP     (0x10)
#define BIT_SLAVE_EN        (0x80)
#define BIT_I2C_READ        (0x80)
// #define BITS_I2C_MASTER_DLY (0x1F)
#define BIT_AUX_IF_EN       (0x20)
#define BIT_ACTL            (0x80)
#define BIT_LATCH_EN        (0x20)
#define BIT_ANY_RD_CLR      (0x10)
#define BIT_BYPASS_EN       (0x02)
#define BITS_WOM_EN         (0xC0)
#define BIT_LPA_CYCLE       (0x20)
#define BIT_STBY_XA         (0x20)
#define BIT_STBY_YA         (0x10)
#define BIT_STBY_ZA         (0x08)
#define BIT_STBY_XG         (0x04)
#define BIT_STBY_YG         (0x02)
#define BIT_STBY_ZG         (0x01)
#define BIT_STBY_XYZA       (BIT_STBY_XA | BIT_STBY_YA | BIT_STBY_ZA)
#define BIT_STBY_XYZG       (BIT_STBY_XG | BIT_STBY_YG | BIT_STBY_ZG)
#define BIT_ACCL_FC_B		(0x08)

#define SUPPORTS_AK89xx_HIGH_SENS   (0x10)
#define AK89xx_FSR                  (4915)
#define AKM_REG_WHOAMI      (0x00)
#define AKM_REG_ST1         (0x02)
#define AKM_REG_HXL         (0x03)
#define AKM_REG_ST2         (0x09)
#define AKM_REG_CNTL        (0x0A)
#define AKM_REG_ASTC        (0x0C)
#define AKM_REG_ASAX        (0x10)
// #define AKM_REG_ASAY        (0x11)
// #define AKM_REG_ASAZ        (0x12)
#define AKM_DATA_READY      (0x01)
#define AKM_DATA_OVERRUN    (0x02)
#define AKM_OVERFLOW        (0x80)
// #define AKM_DATA_ERROR      (0x40)
#define AKM_BIT_SELF_TEST   (0x40)
#define AKM_POWER_DOWN          (0x00 | SUPPORTS_AK89xx_HIGH_SENS)
#define AKM_SINGLE_MEASUREMENT  (0x01 | SUPPORTS_AK89xx_HIGH_SENS)
#define AKM_FUSE_ROM_ACCESS     (0x0F | SUPPORTS_AK89xx_HIGH_SENS)
#define AKM_MODE_SELF_TEST      (0x08 | SUPPORTS_AK89xx_HIGH_SENS)
#define AKM_WHOAMI      (0x48)

#define MAX_PACKET_LENGTH (12)
#define HWST_MAX_PACKET_LENGTH (512)
#define MAX_COMPASS_SAMPLE_RATE (100)
#define REG_6500_XG_ST_DATA     0x0
#define REG_6500_XA_ST_DATA     0xD

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

/* OK */

typedef enum mpu9250AccelFullScale_e {
	MPU9250_ACCEL_FULL_SCALE_2_G	= 0,
	MPU9250_ACCEL_FULL_SCALE_4_G	= 1,
	MPU9250_ACCEL_FULL_SCALE_8_G	= 2,
	MPU9250_ACCEL_FULL_SCALE_16_G	= 3
} mpu9250AccelFullScale_e;

typedef enum mpu9250ClkSel_e {
	MPU9250_CLK_SEL_INTERNAL	= 0,
	MPU9250_CLK_SEL_PLL			= 1,
	MPU9250_CLK_SEL_DISABLED	= 7
} mpu9250ClkSel_e;

typedef enum mpu9250CommInterface_e {
	MPU9250_COMM_INTERFACE_TWI		= 0,
	MPU9250_COMM_INTERFACE_SPI		= 1
} mpu9250CommInterface_e;

typedef enum mpu9250GyroFullScale_e {
	MPU9250_GYRO_FULL_SCALE_250_DPS		= 0,
	MPU9250_GYRO_FULL_SCALE_500_DPS		= 1,
	MPU9250_GYRO_FULL_SCALE_1000_DPS	= 2,
	MPU9250_GYRO_FULL_SCALE_2000_DPS	= 3
} mpu9250GyroFullScale_e;

typedef enum mpu9250TwiSlaveAddress_e {
	MPU9250_TWI_SLAVE_ADDRESS_0x68	= 0x68,
	MPU9250_TWI_SLAVE_ADDRESS_0x69	= 0x69
} mpu9250TwiSlaveAddress_e;




typedef struct mpu9250_s {
	mpu9250CommInterface_e		commInterface		: 1;
	mpu9250TwiSlaveAddress_e	slaveAddress		: 7;
	uint8_t						csBit				: 3;
	vuint8_t *					csDdr;
	vuint8_t *					csPort;
	bool_t						initialized			: 1;
	bool_t						accelLowPowerMode	: 1;
	uint16_t					sampleRate			: 10;
	uint8_t						compassSampleRate	: 7;
	// Gyro Config register		0x1B
	mpu9250GyroFullScale_e		gyroFullScale		: 2;
	uint8_t						fchoiceB			: 2;
	// Accel Config register	0x1C
	mpu9250AccelFullScale_e		accelFullScale		: 2;
	// Power Management 1		0x6B
	bool_t						sleep				: 1;
	bool_t						cycle				: 1;
	bool_t						gyroStandby			: 1;
	bool_t						pdPtat				: 1;
	mpu9250ClkSel_e 			clkSel				: 3;
	// Communication buffer
	uint8_t				bufferData[2];
	uint8_t				bufferLength;

	uint8_t				unusedBits					: 5;
} mpu9250_s;

/* FUNCTION PENDING */

typedef int inv_error_t;

struct int_param_s {
	void (*cb)(void);
	unsigned short pin;
	unsigned char lp_exit;
	unsigned char active_low;
};

struct hw_s {
	unsigned char addr;
	unsigned short max_fifo;
	unsigned char num_reg;
	unsigned short temp_sens;
	short temp_offset;
	unsigned short bank_size;
	unsigned short compass_fsr;
};

struct motion_int_cache_s {
	unsigned short gyro_fsr;
	unsigned char accel_fsr;
	unsigned short lpf;
	unsigned short sample_rate;
	unsigned char sensors_on;
	unsigned char fifo_sensors;
	unsigned char dmp_on;
};

struct chip_cfg_s {
	unsigned char gyro_fsr;				/* Matches gyro_cfg >> 3 & 0x03 */
	unsigned char accel_fsr;			/* Matches accel_cfg >> 3 & 0x03 */
	unsigned char sensors;				/* Enabled sensors. Uses same masks as fifo_en, NOT pwr_mgmt_2. */
	unsigned char lpf;					/* Matches config register. */
	unsigned char clk_src;
	unsigned short sample_rate;			/* Sample rate, NOT rate divider. */
	unsigned char fifo_enable;			/* Matches fifo_en register. */
	unsigned char int_enable;			/* Matches int enable register. */
	unsigned char bypass_mode;			/* 1 if devices on auxiliary I2C bus appear on the primary. */
	unsigned char accel_half;			/* 1 if half-sensitivity. */
	unsigned char lp_accel_mode;		/* 1 if device in low-power accel-only mode. */
	unsigned char int_motion_only;		/* 1 if interrupts are only triggered on motion events. */
	struct motion_int_cache_s cache;
	unsigned char active_low_int;		/* 1 for active low interrupts. */
	unsigned char latched_int;			/* 1 for latched interrupts. */
	unsigned char dmp_on;				/* 1 if DMP is enabled. */
	unsigned char dmp_loaded;			/* Ensures that DMP will only be loaded once. */
	unsigned short dmp_sample_rate;		/* Sampling rate used when DMP is enabled. */
	unsigned short compass_sample_rate;	/* Compass sample rate. */
	unsigned char compass_addr;
	short mag_sens_adj[3];
};

struct test_s {
	unsigned long gyro_sens;
	unsigned long accel_sens;
	unsigned char reg_rate_div;
	unsigned char reg_lpf;
	unsigned char reg_gyro_fsr;
	unsigned char reg_accel_fsr;
	unsigned short wait_ms;
	unsigned char packet_thresh;
	float min_dps;
	float max_dps;
	float max_gyro_var;
	float min_g;
	float max_g;
	float max_accel_var;
	float max_g_offset;
	unsigned short sample_wait_ms;
};

struct gyro_state_s {
	const struct hw_s *hw;
	struct chip_cfg_s chip_cfg;
	const struct test_s *test;
};

/* Filter configurations. */
enum lpf_e {
	INV_FILTER_256HZ_NOLPF2 = 0,
	INV_FILTER_188HZ,
	INV_FILTER_98HZ,
	INV_FILTER_42HZ,
	INV_FILTER_20HZ,
	INV_FILTER_10HZ,
	INV_FILTER_5HZ,
	INV_FILTER_2100HZ_NOLPF,
	NUM_FILTER
};

/* Full scale ranges. */
enum gyro_fsr_e {
	INV_FSR_250DPS = 0,
	INV_FSR_500DPS,
	INV_FSR_1000DPS,
	INV_FSR_2000DPS,
	NUM_GYRO_FSR
};

/* Full scale ranges. */
enum accel_fsr_e {
	INV_FSR_2G = 0,
	INV_FSR_4G,
	INV_FSR_8G,
	INV_FSR_16G,
	NUM_ACCEL_FSR
};

/* Clock sources. */
enum clock_sel_e {
	INV_CLK_INTERNAL = 0,
	INV_CLK_PLL,
	NUM_CLK
};

/* Low-power accel wakeup rates. */
enum lp_accel_rate_e {
	INV_LPA_0_3125HZ,
	INV_LPA_0_625HZ,
	INV_LPA_1_25HZ,
	INV_LPA_2_5HZ,
	INV_LPA_5HZ,
	INV_LPA_10HZ,
	INV_LPA_20HZ,
	INV_LPA_40HZ,
	INV_LPA_80HZ,
	INV_LPA_160HZ,
	INV_LPA_320HZ,
	INV_LPA_640HZ
};



// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Macrofunctions --------------------------------------------------------------

#define min(a,b) ((a<b)?a:b)
#define log_i(...)     do {} while (0)
#define log_e(...)     do {} while (0)

// -----------------------------------------------------------------------------
// Public functions declaration ------------------------------------------------

/* Set up APIs */
int mpu_init(struct int_param_s *int_param);
int mpu_init_slave(void);
int mpu_set_bypass(unsigned char bypass_on);

/* Configuration APIs */
int mpu_lp_accel_mode(unsigned short rate);
int mpu_lp_motion_interrupt(unsigned short thresh, unsigned char time,
                            unsigned short lpa_freq);
int mpu_set_int_level(unsigned char active_low);
int mpu_set_int_latched(unsigned char enable);

int mpu_set_dmp_state(unsigned char enable);
int mpu_get_dmp_state(unsigned char *enabled);

int mpu_get_lpf(unsigned short *lpf);
int mpu_set_lpf(unsigned short lpf);

int mpu_get_gyro_fsr(unsigned short *fsr);
int mpu_set_gyro_fsr(unsigned short fsr);

int mpu_get_accel_fsr(unsigned char *fsr);
int mpu_set_accel_fsr(unsigned char fsr);

int mpu_get_compass_fsr(unsigned short *fsr);

int mpu_get_gyro_sens(float *sens);
int mpu_get_accel_sens(unsigned short *sens);

int mpu_get_sample_rate(unsigned short *rate);
int mpu_set_sample_rate(unsigned short rate);
int mpu_get_compass_sample_rate(unsigned short *rate);
int mpu_set_compass_sample_rate(unsigned short rate);

int mpu_get_fifo_config(unsigned char *sensors);
int mpu_configure_fifo(unsigned char sensors);

int mpu_get_power_state(unsigned char *power_on);
int mpu_set_sensors(unsigned char sensors);

int mpu_read_6500_accel_bias(long *accel_bias);
int mpu_set_gyro_bias_reg(long * gyro_bias);
int mpu_set_accel_bias_6500_reg(const long *accel_bias);
int mpu_read_6050_accel_bias(long *accel_bias);
int mpu_set_accel_bias_6050_reg(const long *accel_bias);

/* Data getter/setter APIs */
int mpu_get_gyro_reg(short *data, unsigned long *timestamp);
int mpu_get_accel_reg(short *data, unsigned long *timestamp);
int mpu_get_compass_reg(short *data, unsigned long *timestamp);
int mpu_get_temperature(long *data, unsigned long *timestamp);

int mpu_get_int_status(short *status);
int mpu_read_fifo(short *gyro, short *accel, unsigned long *timestamp,
                  unsigned char *sensors, unsigned char *more);
int mpu_read_fifo_stream(unsigned short length, unsigned char *data,
                         unsigned char *more);
int mpu_reset_fifo(void);

int mpu_write_mem(unsigned short mem_addr, unsigned short length,
                  unsigned char *data);
int mpu_read_mem(unsigned short mem_addr, unsigned short length,
                 unsigned char *data);
int mpu_load_firmware(unsigned short length, const unsigned char *firmware,
                      unsigned short start_addr, unsigned short sample_rate);

int mpu_reg_dump(void);
int mpu_read_reg(unsigned char reg, unsigned char *data);
int mpu_run_self_test(long *gyro, long *accel);
int mpu_run_6500_self_test(long *gyro, long *accel, unsigned char debug);
int mpu_register_tap_cb(void (*func)(unsigned char, unsigned char));

// STATIC FUNCTIONS
int set_int_enable(unsigned char enable);
int setup_compass(void);
// END - STATIC FUNCTIONS


// EXTERNAL FUNCTIONS
bool_t i2c_write(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const *data);
bool_t mpu9250Write(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const *data);
bool_t i2c_read(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char *data);
void delay_ms(unsigned long num_ms);
uint32_t get_ms(unsigned long *count);
uint32_t labs(long x);
float absf(float x);

bool_t mpu9250TwiInit(mpu9250_s * device, mpu9250TwiSlaveAddress_e slaveAddress);
void mpu9250CreateDevice(mpu9250_s * device, mpu9250CommInterface_e commInterface);
bool_t mpu9250WriteData(mpu9250_s * device);
bool_t mpu9250SetGyroFullScale(mpu9250_s * device, mpu9250GyroFullScale_e gyroFs);
bool_t mpu9250SetAccelFullScale(mpu9250_s * device, mpu9250AccelFullScale_e accelFs);

bool_t mpu9250SetAccelLowPowerMode(mpu9250_s * device, bool_t enabled);


#endif

