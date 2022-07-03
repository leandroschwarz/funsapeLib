/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			adxl345Twi.h
 * Module:			ADXL345 - 3-Axis Accelerometer in Two Wire Interface
 * Author:			Leandro Schwarz
 *					Hazael dos Santos Batista
 * Build:			1
 * Last edition:	October 26, 2017
 * -------------------------------------------------------------------------- */

/*!
 \file	adxl345Twi.h
 \author	Leandro Schwarz
 \author	Hazael dos Santos Batista
 \date		October 26, 2017
 \brief		ADXL345 - 3-Axis Accelerometer in Two Wire Interface

 The ADXL345 is a small, thin, ultralow power, 3-axis accelerometer with high resolution (13-bit) measurement at up to 16 g. Digital output data is formatted as 16-bit twos complement and is accessible through either a SPI (3-wire or 4-wire) or I2C digital interface. This module interfaces a ADXL345 in I2C mode.
*/

#ifndef __ADXL345TWI_H
#define __ADXL345TWI_H 1

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#include "twiMaster.h"
#if __GLOBALDEFINES_H != 1
	#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 1).
#endif

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

#define ADXL345_DEVICE_ID_VALUE	0xE5

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

/*!
	\brief ADXL345 register map enumeration.

	Register map of the ADXL345. When a register is written or read, the pointer is automatically incremented.
 */
typedef enum adxlRegisters_t{
	ADXL345_DEVID			= 0,	/*!< Fixed device ID code (0xE5). */
	ADXL345_THRESH_TAP		= 29,	/*!< Threshold tap (62.5 mg/LSB). If tap detection is in use, 0 in this register may result in undesirable behavior.*/
	ADXL345_OFSX			= 30,	/*!< X axis offset (15.6 mg/LSB). The value must be stored in twos complement format. */
	ADXL345_OFSY			= 31,	/*!< Y axis offset (15.6 mg/LSB). The value must be stored in twos complement format. */
	ADXL345_OFSZ			= 32,	/*!< Z axis offset (15.6 mg/LSB). The value must be stored in twos complement format. */
	ADXL345_DUR				= 33,	/*!< Maximum time that an event must be above tap threshold to qualify as a tap event (625us/LSB). A value of 0 disables the single tap/double tap functions. */
	ADXL345_LATENT			= 34,	/*!< Waiting time from the detection of a tap event to the start of the tap window (1.25ms/LSB). A value of 0 disables the double tap function. */
	ADXL345_WINDOW			= 35,	/*!< Amount of time after the expiration of the latency time during which a second valid tap can begin (1.25ms/LSB). A value of 0 disables the double tap function. */
	ADXL345_THRESH_ACT		= 36,	/*!< Threshold value for detecting activity (62.5 mg/LSB). A value of 0 may result in undesirable behavior if the activity interrupt is enabled. */
	ADXL345_THRESH_INACT	= 37,	/*!< Threshold value for detecting inactivity (62.5 mg/LSB). A value of 0 may result in undesirable behavior if the inactivity interrupt is enabled. */
	ADXL345_TIME_INACT		= 38,	/*!< Amount of time that acceleration must be less than THRESH_INACT for inactivity to be declared (1s/LSB). */
	ADXL345_ACT_INACT_CTL	= 39,	/*!< Control register of activity and inactivity detection funcions. */
	ADXL345_THRESH_FF		= 40,	/*!< Threshold for free-fall detection (62.5 mg/LSB). A value of 0 mg may result in undesirable behavior if the freefall interrupt is enabled. Values between 300 mg and 600 mg (0x05 to 0x09) are recommended. */
	ADXL345_TIME_FF			= 41,	/*!< Minimum time that the value of all axes must be less than THRESH_FF to generate a free-fall interrupt (5 ms/LSB). A value of 0 may result in undesirable behavior if the free-fall interrupt is enabled. Values between 100 ms and 350 ms (0x14 to 0x46) are recommended. */
	ADXL345_TAP_AXES		= 42,	/*!< Selection of the axes that are participating in the tap detection. */
	ADXL345_ACT_TAP_STATUS	= 43,	/*!< (READ ONLY) First axis involed in a tap or activity event. */
	ADXL345_BW_RATE			= 44,	/*!< Low Power mode configuration. */
	ADXL345_POWER_CTL		= 45,	/*!< Power mode control. */
	ADXL345_INT_ENABLE		= 46,	/*!< Interruption enable. */
	ADXL345_INT_MAP			= 47,	/*!< Interruption pin configuration. */
	ADXL345_INT_SOURCE		= 48,	/*!< (READ ONLY) Functions that have triggered an event. */
	ADXL345_DATA_FORMAT		= 49,	/*!< Controls the presentation of data. */
	ADXL345_DATAX0			= 50,	/*!< X axis acceleraton value (LSB). */
	ADXL345_DATAX1			= 51,	/*!< X axis acceleraton value (MSB). */
	ADXL345_DATAY0			= 52,	/*!< Y axis acceleraton value (LSB). */
	ADXL345_DATAY1			= 53,	/*!< Y axis acceleraton value (MSB). */
	ADXL345_DATAZ0			= 54,	/*!< Z axis acceleraton value (LSB). */
	ADXL345_DATAZ1			= 55,	/*!< Z axis acceleraton value (MSB). */
	ADXL345_FIFO_CTL		= 56,	/*!< Controls the FIFO mode. */
	ADXL345_FIFO_STATUS		= 57	/*!< (READ ONLY) Status fo the FIFO. */
} adxlRegisters_t;

/*!
	\brief ADXL345 range selection enumeration.

	Range selection enumeration. The sensibility values are overwritten if the FULL_SCALE bit is set.
 */
typedef enum adxl345Range_t{
	ADXL345_2G			= 0,	/*!< Acceleration range of 2 g (3.9 mg/LSB). */
	ADXL345_4G			= 1,	/*!< Acceleration range of 4 g (7.8 mg/LSB). */
	ADXL345_8G			= 2,	/*!< Acceleration range of 8 g (15.6 mg/LSB). */
	ADXL345_16G			= 3		/*!< Acceleration range of 16 g (31.2 mg/LSB). */
} adxl345Range_t;

/*!
	\brief ADXL345 TWI address enumeration.

	TWI address selection enumeration.
 */
typedef enum adxl345TwiAddress_t{
	ADXL345_ADDRESS_0x1D	= 0,	/*!< TWI address 0x1D. */
	ADXL345_ADDRESS_0x53	= 1		/*!< TWI address 0x52. */
} adxl345TwiAddress_t;

/*!
	\brief Axis used during calibration.

	Axis used during calibration. The sensibility values are overwritten if the FULL_SCALE bit is set.
 */
typedef enum adxl345GravityOn_t{
	ADXL345_GRAVITY_ON_ZP	= 0,	/*!< Indicates that the accelerometer has the positive Z axis pointed against gravity. */
	ADXL345_GRAVITY_ON_ZM	= 1,	/*!< Indicates that the accelerometer has the negative Z axis pointed against gravity. */
	ADXL345_GRAVITY_ON_YP	= 2,	/*!< Indicates that the accelerometer has the positive Y axis pointed against gravity. */
	ADXL345_GRAVITY_ON_YM	= 3,	/*!< Indicates that the accelerometer has the negative Y axis pointed against gravity. */
	ADXL345_GRAVITY_ON_XP	= 4,	/*!< Indicates that the accelerometer has the positive X axis pointed against gravity. */
	ADXL345_GRAVITY_ON_XM	= 5		/*!< Indicates that the accelerometer has the negative X axis pointed against gravity. */
} adxl345GravityOn_t;

/*!
	\brief Measurement unit enumeration.

	Unit to represent the acceleration.
 */
typedef enum adxl345Unit_t{
	ADXL345_RAW_DATA	= 0,	/*!< Acceleration in raw data. */
	ADXL345_G_UNITS		= 1		/*!< Acceleration in G units. */
} adxl345Unit_t;

/*!
	\brief Event pin enumeration.

	Interruption pin to use in event detection.
 */
typedef enum adxl345EventPin_t{
	ADXL345_INT_DISABLE	= 0,	/*!< Disables the event interruption detection. */
	ADXL345_INT_1	= 1,		/*!< Event detection on pin INT_1. */
	ADXL345_INT_2	= 2			/*!< Event detection on pin INT_2. */
} adxl345EventPin_t;

/*!
	\brief Low power rate enumeration.

	Measurement rate to be use during Low Power mode.
 */
typedef enum adxl345LowPowerRate_t{
	ADXL345_LP_6HZ		= 7,	/*!< 6.25 Hz measurement rate during Low Power mode. */
	ADXL345_LP_12HZ		= 8,	/*!< 12.5 Hz measurement rate during Low Power mode. */
	ADXL345_LP_25HZ		= 9,	/*!< 25 Hz measurement rate during Low Power mode. */
	ADXL345_LP_50HZ		= 10,	/*!< 50 Hz measurement rate during Low Power mode. */
	ADXL345_LP_100HZ	= 11,	/*!< 100 Hz measurement rate during Low Power mode. */
	ADXL345_LP_200HZ	= 12	/*!< 200 Hz measurement rate during Low Power mode. */
} adxl345LowPowerRate_t;

/*!
	\brief Power mode enumeration.

	Power mode of the ADXL345.
 */
typedef enum adxl345PowerMode_t{
	ADXL345_STAND_BY = 0,	/*!< Stand-by mode. */
	ADXL345_MEASURE			/*!< Measument mode. */
} adxl345PowerMode_t;

/*!
	\brief Tap axes enumeration.

	Axes that are active during tap detection. Must be used as flags.
 */
typedef enum adxl345TapAxes_t{
	ADXL345_TAP_Z	=	1,	/*!< Z axis. */
	ADXL345_TAP_Y	=	2,	/*!< Y axis. */
	ADXL345_TAP_X	=	4	/*!< X axis. */
} adxl345TapAxes_t;

/*!
	\brief Interruption sources enumeration.

	Sources of interruption of the ADXL345. Must be used as flags.
 */
typedef enum adxl345IntSources_t{
	ADXL345_SRC_OVERRUN		= 1,	/*!< New data has replaced unread data. */
	ADXL345_SRC_WATERMARK	= 2,	/*!< FIFO defined custom limit (watermark) has achieved. */
	ADXL345_SRC_FREE_FALL	= 4,	/*!< Free fall event has occurred. */
	ADXL345_SRC_INACTIVITY	= 8,	/*!< Inactivity event has occurred. */
	ADXL345_SRC_ACTIVITY	= 16,	/*!< Activity event has occurred. */
	ADXL345_SRC_DOUBLE_TAP	= 32,	/*!< Double tap event has occurred. */
	ADXL345_SRC_SINGLE_TAP	= 64,	/*!< Single tap event has occurred. */
	ADXL345_SRC_DATA_READY	= 128	/*!< New data is available. */
} adxl345IntSources_t;

/*!
	\brief ADXL345 configuration struct.

	Stores the ADXL345 configurations.
 */
typedef struct adxl345Configuration_t{
	uint8					deviceAddress;			/*!< I2C slave address. */
	bool_t					fullResolution	: 1;	/*!< Full resolution. */
	adxl345Range_t			range			: 2;	/*!< Sensibility range. */
	bool_t					measure			: 1;
	bool_t					link			: 1;
	bool_t					autoSleep		: 1;
	bool_t					sleep			: 1;
	bool_t					lowPower		: 1;
	adxl345LowPowerRate_t	lowPowerRate	: 4;
	uint8					wakeUp			: 2;
	uint8					unusedBits		: 2;
} adxl345Configuration_t;

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

/*! Global variable that stores the ADXL345 configuration. */
adxl345Configuration_t adxl345Configuration;

// -----------------------------------------------------------------------------
// Macrofunctions --------------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

/*! Initializes the ADXL345. */
bool_t adxl345Init(adxl345TwiAddress_t twiAddress);
/*! Disables the Low Power Mode. */
void adxl345LowPowerDisable(void);
/*! Enables the Low Power Mode. */
void adxl345LowPowerEnable(adxl345LowPowerRate_t rate);
/*! Sets the ADXL345 Power Mode. */
void adxl345PowerModeSet(adxl345PowerMode_t powerMode);
/*! Reads acceleration values. */
void adxl345DataGet(int16 * xValue, int16 * yValue, int16 * zValue, adxl345Unit_t unit);
/*! Sets the single tap and dual tap detection. */
void adxl345TapDetectionSet(adxl345EventPin_t singleTap, adxl345EventPin_t doubleTap, bool_t suppressDouble, adxl345TapAxes_t axesFlags, uint8 duration, uint8 latency, uint8 window, uint8 thres);
/*! Sets the ADXL345 sensibility and resolution. */
void adxl345RangeSet(adxl345Range_t range, bool_t fullResolution);
/*! Calibrates the ADXL345. */
void adxl345Calibrate(adxl345GravityOn_t gravityTowards);
/*! Returns the sources of last interruptions. */
adxl345IntSources_t adxl345IntSourceReturn(void);


void adxl345FreeFallDetectionSet(adxl345EventPin_t eventPin, uint8 thres, uint8 time);

#endif