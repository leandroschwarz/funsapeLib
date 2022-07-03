/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			mma7455Spi.c
 * Module:			SPI interface for MMA7455 accelerometer
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 21, 2018
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "mma7455Spi.h"
#if __MMA7455_SPI_H != 1
	#error Error 101 - Build mismatch on header and source code files (mma7455Spi).
#endif

// -----------------------------------------------------------------------------
// New data types (used only in this file) -------------------------------------

typedef enum mma7455SpiRegister_t {
	MMA7455_XOUTL_REG	= 0x00,
	MMA7455_XOUTH_REG	= 0x01,
	MMA7455_YOUTL_REG	= 0x02,
	MMA7455_YOUTH_REG	= 0x03,
	MMA7455_ZOUTL_REG	= 0x04,
	MMA7455_ZOUTH_REG	= 0x05,
	MMA7455_XOUT8_REG	= 0x06,
	MMA7455_YOUT8_REG	= 0x07,
	MMA7455_ZOUT8_REG	= 0x08,
	MMA7455_STATUS_REG	= 0x09,
	MMA7455_DETSRC_REG	= 0x0A,
	MMA7455_TOUT_REG	= 0x0B,
	MMA7455_I2CAD_REG	= 0x0D,
	MMA7455_USRINF_REG	= 0x0E,
	MMA7455_WHOAMI_REG	= 0x0F,
	MMA7455_XOFFL_REG	= 0x10,
	MMA7455_XOFFH_REG	= 0x11,
	MMA7455_YOFFL_REG	= 0x12,
	MMA7455_YOFFH_REG	= 0x13,
	MMA7455_ZOFFL_REG	= 0x14,
	MMA7455_ZOFFH_REG	= 0x15,
	MMA7455_MCTL_REG	= 0x16,
	MMA7455_INTRST_REG	= 0x17,
	MMA7455_CTL1_REG	= 0x18,
	MMA7455_CTL2_REG	= 0x19,
	MMA7455_LDTH_REG	= 0x1A,
	MMA7455_PDTH_REG	= 0x1B,
	MMA7455_PW_REG		= 0x1C,
	MMA7455_LT_REG		= 0x1D,
	MMA7455_TW_REG		= 0x1E
} mma7455SpiRegister_t;

// STATUS Register Bits
#define MMA7455_STATUS_DRDY_BIT		0
#define MMA7455_STATUS_DOVR_BIT		1
#define MMA7455_STATUS_PERR_BIT		2
// DETSCR Register Bits
#define MMA7455_DETSRC_INT1_BIT		0
#define MMA7455_DETSRC_INT2_BIT		1
#define MMA7455_DETSRC_PDZ_BIT		2
#define MMA7455_DETSRC_PDY_BIT		3
#define MMA7455_DETSRC_PDX_BIT		4
#define MMA7455_DETSRC_LDZ_BIT		5
#define MMA7455_DETSRC_LDY_BIT		6
#define MMA7455_DETSRC_LDX_BIT		7
// I2CDA Register Bits
#define MMA7455_I2CDA_DVAD_BIT		0
#define MMA7455_I2CDA_I2CDIS_BIT	7
// MCTL Register Bits
#define MMA7455_MCTL_MODE_BIT		0
#define MMA7455_MCTL_GLVL_BIT		2
#define MMA7455_MCTL_STON_BIT		4
#define MMA7455_MCTL_SPI3W_BIT		5
#define MMA7455_MCTL_DRPD_BIT		6
// INTRST Register Bits
#define MMA7455_INTRST_CLR_INT1_BIT	0
#define MMA7455_INTRST_CLR_INT2_BIT	1
// CTL1 Register Bits
#define MMA7455_CTL1_INTPIN_BIT		0
#define MMA7455_CTL1_INTREG_BIT		1
#define MMA7455_CTL1_XDA_BIT		3
#define MMA7455_CTL1_YDA_BIT		4
#define MMA7455_CTL1_ZDA_BIT		5
#define MMA7455_CTL1_THOPT			6
#define MMA7455_CTL1_DFBW			7
// CTL2 Register Bits
#define MMA7455_CTL2_LDPL_BIT		0
#define MMA7455_CTL2_PDPL_BIT		1
#define MMA7455_CTL2_DRVO_BIT		3
// PDTH Register Bits
#define MMA7455_PDTH_PDTH_BIT		0
#define MMA7455_PDTH_XPDTH_BIT		7



