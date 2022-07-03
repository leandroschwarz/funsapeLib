
static int set_int_enable(unsigned char enable);


struct gyro_reg_s {
	unsigned char who_am_i;
	unsigned char rate_div;
	unsigned char lpf;
	unsigned char prod_id;
	unsigned char user_ctrl;
	unsigned char fifo_en;
	unsigned char gyro_cfg;
	unsigned char accel_cfg;
	unsigned char accel_cfg2;
	unsigned char lp_accel_odr;
	unsigned char motion_thr;
	unsigned char motion_dur;
	unsigned char fifo_count_h;
	unsigned char fifo_r_w;
	unsigned char raw_gyro;
	unsigned char raw_accel;
	unsigned char temp;
	unsigned char int_enable;
	unsigned char dmp_int_status;
	unsigned char int_status;
	unsigned char accel_intel;
	unsigned char pwr_mgmt_1;
	unsigned char pwr_mgmt_2;
	unsigned char int_pin_cfg;
	unsigned char mem_r_w;
	unsigned char accel_offs;
	unsigned char i2c_mst;
	unsigned char bank_sel;
	unsigned char mem_start_addr;
	unsigned char prgm_start_h;
	unsigned char s0_addr;
	unsigned char s0_reg;
	unsigned char s0_ctrl;
	unsigned char s1_addr;
	unsigned char s1_reg;
	unsigned char s1_ctrl;
	unsigned char s4_ctrl;
	unsigned char s0_do;
	unsigned char s1_do;
	unsigned char i2c_delay_ctrl;
	unsigned char raw_compass;
	/* The I2C_MST_VDDIO bit is in this register. */
	unsigned char yg_offs_tc;
};

/* Information specific to a particular device. */
struct hw_s {
	unsigned char addr;
	unsigned short max_fifo;
	unsigned char num_reg;
	unsigned short temp_sens;
	short temp_offset;
	unsigned short bank_size;
	unsigned short compass_fsr;
};

/* When entering motion interrupt mode, the driver keeps track of the
 * previous state so that it can be restored at a later time.
 * TODO: This is tacky. Fix it.
 */
struct motion_int_cache_s {
	unsigned short gyro_fsr;
	unsigned char accel_fsr;
	unsigned short lpf;
	unsigned short sample_rate;
	unsigned char sensors_on;
	unsigned char fifo_sensors;
	unsigned char dmp_on;
};

/* Cached chip configuration data.
 * TODO: A lot of these can be handled with a bitmask.
 */
struct chip_cfg_s {
	/* Matches gyro_cfg >> 3 & 0x03 */
	unsigned char gyro_fsr;
	/* Matches accel_cfg >> 3 & 0x03 */
	unsigned char accel_fsr;
	/* Enabled sensors. Uses same masks as fifo_en, NOT pwr_mgmt_2. */
	unsigned char sensors;
	/* Matches config register. */
	unsigned char lpf;
	unsigned char clk_src;
	/* Sample rate, NOT rate divider. */
	unsigned short sample_rate;
	/* Matches fifo_en register. */
	unsigned char fifo_enable;
	/* Matches int enable register. */
	unsigned char int_enable;
	/* 1 if devices on auxiliary I2C bus appear on the primary. */
	unsigned char bypass_mode;
	/* 1 if half-sensitivity.
	 * NOTE: This doesn't belong here, but everything else in hw_s is const,
	 * and this allows us to save some precious RAM.
	 */
	unsigned char accel_half;
	/* 1 if device in low-power accel-only mode. */
	unsigned char lp_accel_mode;
	/* 1 if interrupts are only triggered on motion events. */
	unsigned char int_motion_only;
	struct motion_int_cache_s cache;
	/* 1 for active low interrupts. */
	unsigned char active_low_int;
	/* 1 for latched interrupts. */
	unsigned char latched_int;
	/* 1 if DMP is enabled. */
	unsigned char dmp_on;
	/* Ensures that DMP will only be loaded once. */
	unsigned char dmp_loaded;
	/* Sampling rate used when DMP is enabled. */
	unsigned short dmp_sample_rate;
	/* Compass sample rate. */
	unsigned short compass_sample_rate;
	unsigned char compass_addr;
	short mag_sens_adj[3];
};

/* Information for self-test. */
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

/* Gyro driver state variables. */
struct gyro_state_s {
	const struct gyro_reg_s *reg;
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

#define BIT_I2C_MST_VDDIO   (0x80)
#define BIT_FIFO_EN         (0x40)
#define BIT_DMP_EN          (0x80)
#define BIT_FIFO_RST        (0x04)
#define BIT_DMP_RST         (0x08)
#define BIT_FIFO_OVERFLOW   (0x10)
#define BIT_DATA_RDY_EN     (0x01)
#define BIT_DMP_INT_EN      (0x02)
#define BIT_MOT_INT_EN      (0x40)
#define BITS_FSR            (0x18)
#define BITS_LPF            (0x07)
#define BITS_HPF            (0x07)
#define BITS_CLK            (0x07)
#define BIT_FIFO_SIZE_1024  (0x40)
#define BIT_FIFO_SIZE_2048  (0x80)
#define BIT_FIFO_SIZE_4096  (0xC0)
#define BIT_RESET           (0x80)
#define BIT_SLEEP           (0x40)
#define BIT_S0_DELAY_EN     (0x01)
#define BIT_S2_DELAY_EN     (0x04)
#define BITS_SLAVE_LENGTH   (0x0F)
#define BIT_SLAVE_BYTE_SW   (0x40)
#define BIT_SLAVE_GROUP     (0x10)
#define BIT_SLAVE_EN        (0x80)
#define BIT_I2C_READ        (0x80)
#define BITS_I2C_MASTER_DLY (0x1F)
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
#define AKM_REG_ASAY        (0x11)
#define AKM_REG_ASAZ        (0x12)

#define AKM_DATA_READY      (0x01)
#define AKM_DATA_OVERRUN    (0x02)
#define AKM_OVERFLOW        (0x80)
#define AKM_DATA_ERROR      (0x40)

#define AKM_BIT_SELF_TEST   (0x40)

#define AKM_POWER_DOWN          (0x00 | SUPPORTS_AK89xx_HIGH_SENS)
#define AKM_SINGLE_MEASUREMENT  (0x01 | SUPPORTS_AK89xx_HIGH_SENS)
#define AKM_FUSE_ROM_ACCESS     (0x0F | SUPPORTS_AK89xx_HIGH_SENS)
#define AKM_MODE_SELF_TEST      (0x08 | SUPPORTS_AK89xx_HIGH_SENS)

#define AKM_WHOAMI      (0x48)

const struct gyro_reg_s reg = {
	.who_am_i       = 0x75,
	.rate_div       = 0x19,
	.lpf            = 0x1A,
	.prod_id        = 0x0C,
	.user_ctrl      = 0x6A,
	.fifo_en        = 0x23,
	.gyro_cfg       = 0x1B,
	.accel_cfg      = 0x1C,
	.accel_cfg2     = 0x1D,
	.lp_accel_odr   = 0x1E,
	.motion_thr     = 0x1F,
	.motion_dur     = 0x20,
	.fifo_count_h   = 0x72,
	.fifo_r_w       = 0x74,
	.raw_gyro       = 0x43,
	.raw_accel      = 0x3B,
	.temp           = 0x41,
	.int_enable     = 0x38,
	.dmp_int_status = 0x39,
	.int_status     = 0x3A,
	.accel_intel    = 0x69,
	.pwr_mgmt_1     = 0x6B,
	.pwr_mgmt_2     = 0x6C,
	.int_pin_cfg    = 0x37,
	.mem_r_w        = 0x6F,
	.accel_offs     = 0x77,
	.i2c_mst        = 0x24,
	.bank_sel       = 0x6D,
	.mem_start_addr = 0x6E,
	.prgm_start_h   = 0x70
	, .raw_compass   = 0x49,
	.s0_addr        = 0x25,
	.s0_reg         = 0x26,
	.s0_ctrl        = 0x27,
	.s1_addr        = 0x28,
	.s1_reg         = 0x29,
	.s1_ctrl        = 0x2A,
	.s4_ctrl        = 0x34,
	.s0_do          = 0x63,
	.s1_do          = 0x64,
	.i2c_delay_ctrl = 0x67
};
const struct hw_s hw = {
	.addr           = 0x68,
	.max_fifo       = 1024,
	.num_reg        = 128,
	.temp_sens      = 321,
	.temp_offset    = 0,
	.bank_size      = 256
	, .compass_fsr    = AK89xx_FSR
};

const struct test_s test = {
	.gyro_sens      = 32768 / 250,
	.accel_sens     = 32768 / 2, //FSR = +-2G = 16384 LSB/G
	.reg_rate_div   = 0,    /* 1kHz. */
	.reg_lpf        = 2,    /* 92Hz low pass filter*/
	.reg_gyro_fsr   = 0,    /* 250dps. */
	.reg_accel_fsr  = 0x0,  /* Accel FSR setting = 2g. */
	.wait_ms        = 200,   //200ms stabilization time
	.packet_thresh  = 200,    /* 200 samples */
	.min_dps        = 20.f,  //20 dps for Gyro Criteria C
	.max_dps        = 60.f, //Must exceed 60 dps threshold for Gyro Criteria B
	.max_gyro_var   = .5f, //Must exceed +50% variation for Gyro Criteria A
	.min_g          = .225f, //Accel must exceed Min 225 mg for Criteria B
	.max_g          = .675f, //Accel cannot exceed Max 675 mg for Criteria B
	.max_accel_var  = .5f,  //Accel must be within 50% variation for Criteria A
	.max_g_offset   = .5f,   //500 mg for Accel Criteria C
	.sample_wait_ms = 10    //10ms sample time wait
};

static struct gyro_state_s st = {
	.reg = &reg,
	.hw = &hw,
	.test = &test
};
