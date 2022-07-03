/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			adxl345Twi.c
 * Module:			ADXL345 - 3-Axis Accelerometer in Two Wire Interface
 * Author:			Leandro Schwarz
 *					Hazael dos Santos Batista
 * Build:			1
 * Last edition:	October 26, 2017
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "adxl345Twi.h"
#if __ADXL345TWI_H != 1
#error Error 101 - Build mismatch on header and source code files (adxl345Twi).
#endif

// -----------------------------------------------------------------------------
// Private function declarations -----------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Private functions -----------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Public functions ------------------------------------------------------------

/* -----------------------------------------------------------------------------
 * Function:	adxl345Init
 * Purpose:		Initializes the adxl345 in TWI communication
 * Arguments:	twiAddress		adxl345TwiAddress_t enumeration
 * Returns:		TRUE / FALSE
 * Notes:		The TWI module must be configurated before calling this function
 * -------------------------------------------------------------------------- */

bool_t adxl345Init(adxl345TwiAddress_t twiAddress)
{
    uint8 data[2];

    // Select between addresses
    if (twiAddress == ADXL345_ADDRESS_0x53) {
        adxl345Configuration.deviceAddress = 0x53;
    } else {
        adxl345Configuration.deviceAddress = 0x1D;
    }

    // Look for ADXL345
    data[0] = ADXL345_DEVID;
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_WRITE, data, 1);
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_READ, data, 1);
    if (data[0] != ADXL345_DEVICE_ID_VALUE) {
        return FALSE;
    }

    // Reset configuration
    data[0] = ADXL345_DATA_FORMAT;
    data[1] = 0;
    adxl345Configuration.fullResolution = FALSE;
    adxl345Configuration.range = ADXL345_2G;

    return TRUE;
}

/* -----------------------------------------------------------------------------
 * Function:	adxl345DataGet
 * Purpose:		Gets the acceleration values
 * Arguments:	xValue	pointer where to store X-axis acceleration
 *				yValue	pointer where to store Y-axis acceleration
 *				zValue	pointer where to store Z-axis acceleration
 *				unit	ADXL345_RAW	->	returns raw values
 *						ADXL345_G	->	converts units to G force data
 * Returns:		none
 * -------------------------------------------------------------------------- */

void adxl345DataGet(int16 *xValue, int16 *yValue, int16 *zValue, adxl345Unit_t unit)
{
    uint8 data[6];
    int32 x32 = 0;
    int32 y32 = 0;
    int32 z32 = 0;

    // Gets acceleration values
    data[0] = ADXL345_DATAX0;
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_WRITE, data, 1);
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_READ, data, 6);
    *xValue = ((int16)data[1]) << 8;
    *xValue |= (int16)data[0];
    *yValue = ((int16)data[3]) << 8;
    *yValue |= (int16)data[2];
    *zValue = ((int16)data[5]) << 8;
    *zValue |= (int16)data[4];

    // If raw data was desired, just returns
    if (unit == ADXL345_RAW_DATA) {
        return;
    }

    // Converts data into mg
    x32 = (int32)(*xValue) * 1000;
    y32 = (int32)(*yValue) * 1000;
    z32 = (int32)(*zValue) * 1000;
    if (!adxl345Configuration.fullResolution) {
        x32 *= (1 << adxl345Configuration.range);
        y32 *= (1 << adxl345Configuration.range);
        z32 *= (1 << adxl345Configuration.range);
    }
    *xValue = (int16)(x32 / 256);
    *yValue = (int16)(y32 / 256);
    *zValue = (int16)(z32 / 256);

    return;
}

/* -----------------------------------------------------------------------------
 * Function:	adxl345RangeSet
 * Purpose:		Sets the scale range
 * Arguments:	range			scale range
 *				fullResolution	sets full resolution
 * Returns:		none
 * -------------------------------------------------------------------------- */

void adxl345RangeSet(adxl345Range_t range, bool_t fullResolution)
{
    uint8 aux8 = 0;
    uint8 data[2];

    // Retrieves old register value
    data[0] = ADXL345_DATA_FORMAT;
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_WRITE, data, 1);
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_READ, data, 1);
    aux8 = data[0];

    // Configures new register value
    clrMaskOffset(aux8, 0x0B, 0);
    adxl345Configuration.fullResolution = FALSE;
    if (fullResolution) {
        setBit(aux8, 3);
        adxl345Configuration.fullResolution = TRUE;
    }
    setMaskOffset(aux8, (range & 0x03), 0);
    adxl345Configuration.range = range;

    // Pushes value back to register
    data[0] = ADXL345_DATA_FORMAT;
    data[1] = aux8;
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_WRITE, data, 2);

    return;
}

/* -----------------------------------------------------------------------------
 * Function:	adxl345Calibrate
 * Purpose:		Calibrates the device
 * Arguments:	gravityTowards		indicates where gravity is pointed out
 * Returns:		none
 * -------------------------------------------------------------------------- */

void adxl345Calibrate(adxl345GravityOn_t gravityTowards)
{
    int16 xCal = 0;
    int16 yCal = 0;
    int16 zCal = 0;
    int16 xVal = 0;
    int16 yVal = 0;
    int16 zVal = 0;
    uint8 i = 0;
    int16 gValue = 0;
    int16 factor = 0;
    uint8 data[4];

    // Resets old offset
    data[0] = ADXL345_OFSX;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_WRITE, data, 4);

    // Performs 10 acquisitions
    for (i = 0; i < 10; i++) {
        adxl345DataGet(&xVal, &yVal, &zVal, ADXL345_RAW_DATA);
        xCal -= xVal;
        yCal -= yVal;
        zCal -= zVal;
        _delay_ms(10);
    }

    // Scale balance factor
    if (adxl345Configuration.fullResolution) {
        factor = 40;
        gValue = 256;
    } else {
        factor = 40 / (1 << adxl345Configuration.range);
        gValue = 256 / (1 << adxl345Configuration.range);
    }

    // Calibration value
    data[1] = (uint8)(xCal / factor);
    data[2] = (uint8)(yCal / factor);
    data[3] = (uint8)(zCal / factor);
    switch (gravityTowards) {
    case ADXL345_GRAVITY_ON_ZP:
        data[3] = (uint8)((zCal / factor) + gValue);
        break;
    case ADXL345_GRAVITY_ON_ZM:
        data[3] = (uint8)((zCal / factor) - gValue);
        break;
    case ADXL345_GRAVITY_ON_YP:
        data[2] = (uint8)((yCal / factor) + gValue);
        break;
    case ADXL345_GRAVITY_ON_YM:
        data[2] = (uint8)((yCal / factor) - gValue);
        break;
    case ADXL345_GRAVITY_ON_XP:
        data[1] = (uint8)((xCal / factor) + gValue);
        break;
    case ADXL345_GRAVITY_ON_XM:
        data[1] = (uint8)((xCal / factor) - gValue);
        break;
    }
    data[0] = ADXL345_OFSX;
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_WRITE, data, 4);

    return;
}

/* -----------------------------------------------------------------------------
 * Function:	adxl345LowPowerDisable
 * Purpose:		Disables the low power mode
 * Arguments:	none
 * Returns:		none
 * -------------------------------------------------------------------------- */

void adxl345LowPowerDisable(void)
{
    uint8 data[2];

    // Disables low power mode
    data[0] = ADXL345_BW_RATE;
    data[1] = 0;
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_WRITE, data, 2);
    adxl345Configuration.lowPower = FALSE;
    adxl345Configuration.lowPowerRate = ADXL345_LP_6HZ;

    return;
}

/* -----------------------------------------------------------------------------
 * Function:	adxl345LowPowerEnable
 * Purpose:		Enables the low power mode
 * Arguments:	rate	define the measurement rate in low power mode
 * Returns:		none
 * -------------------------------------------------------------------------- */

void adxl345LowPowerEnable(adxl345LowPowerRate_t rate)
{
    uint8 data[2];

    // Enables low power mode
    data[0] = ADXL345_BW_RATE;
    data[1] = (1 << 4) | rate;
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_WRITE, data, 2);
    adxl345Configuration.lowPower = TRUE;
    adxl345Configuration.lowPowerRate = rate;

    return;
}

/* -----------------------------------------------------------------------------
 * Function:	adxl345PowerModeSet
 * Purpose:		Sets the ADXL345 power mode
 * Arguments:	powerMode	switches between standby and measurement modes
 * Returns:		none
 * -------------------------------------------------------------------------- */

void adxl345PowerModeSet(adxl345PowerMode_t powerMode)
{
    uint8 data[2];

    // Retrieves old register value
    data[0] = ADXL345_POWER_CTL;
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_WRITE, data, 1);
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_READ, data, 1);
    data[1] = clrBit(data[0], 3);

    // Configures power mode
    if (powerMode) {
        setBit(data[1], 3);
        adxl345Configuration.measure = TRUE;
    } else {
        adxl345Configuration.measure = FALSE;
    }

    // Pushes value back to register
    data[0] = ADXL345_POWER_CTL;
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_WRITE, data, 2);

    return;
}

/* -----------------------------------------------------------------------------
 * Function:	adxl345TapDetectionSet
 * Purpose:		Sets the ADXL345 power mode
 * Arguments:	singleTap		single tap interrupt configuration
 *				doubleTap		double tap interrupt configuration
 *				suppressDouble	suppress double tap if threshold met during latency
 *				axesFlags		flags of the axes actives during tap detection
 *				duration		max duration of the tap
 *				latency			minimum time between taps
 *				window			interval after latency, where a second tap must be present
 *				thres			tap detection threshold
 * Returns:		none
 * -------------------------------------------------------------------------- */

void adxl345TapDetectionSet(adxl345EventPin_t singleTap, adxl345EventPin_t doubleTap, bool_t suppressDouble,
        adxl345TapAxes_t axesFlags, uint8 duration, uint8 latency, uint8 window, uint8 thres)
{
    uint8 data[4];

    // Sets duration, latency and window
    data[0] = ADXL345_DUR;
    data[1] = duration;
    data[2] = latency;
    data[3] = window;
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_WRITE, data, 4);

    // Sets threshold
    data[0] = ADXL345_THRESH_TAP;
    data[1] = thres;
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_WRITE, data, 2);

    // Sets axes and suppress double
    data[0] = ADXL345_TAP_AXES;
    data[1] = (suppressDouble << 3);
    data[1] |= axesFlags;
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_WRITE, data, 2);

    // Sets single and double tap
    data[0] = ADXL345_INT_ENABLE;
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_WRITE, data, 1);
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_READ, data, 2);
    data[2] = clrMaskOffset(data[1], 0x03, 5);	// INT_MAP
    data[1] = clrMaskOffset(data[0], 0x03, 5);	// INT_ENABLE
    switch (singleTap) {
    case ADXL345_INT_1:			setBit(data[1], 6);	break;
    case ADXL345_INT_2:			setBit(data[1], 6);	setBit(data[2], 6);	break;
    case ADXL345_INT_DISABLE:	break;
    }
    switch (doubleTap) {
    case ADXL345_INT_1:			setBit(data[1], 5);	break;
    case ADXL345_INT_2:			setBit(data[1], 5);	setBit(data[2], 5);	break;
    case ADXL345_INT_DISABLE:	break;
    }
    data[0] = ADXL345_INT_ENABLE;
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_WRITE, data, 3);

    return;
}

adxl345IntSources_t	adxl345IntSourceReturn(void)
{
    uint8 data[2];

    data[0] = ADXL345_INT_SOURCE;
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_WRITE, data, 1);
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_READ, data, 1);
    return data[0];
}

void adxl345FreeFallDetectionSet(adxl345EventPin_t eventPin, uint8 thres, uint8 time)
{
    uint8 data[3];

    data[0] = ADXL345_THRESH_FF;
    data[1] = thres;
    data[2] = time;
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_WRITE, data, 3);
    data[0] = ADXL345_INT_ENABLE;
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_WRITE, data, 1);
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_READ, data, 2);
    data[2] = clrBit(data[1], 2);	// INT_MAP
    data[1] = clrBit(data[0], 2);	// INT_ENABLE
    switch (eventPin) {
    case ADXL345_INT_1:			setBit(data[1], 2);	break;
    case ADXL345_INT_2:			setBit(data[1], 2);	setBit(data[2], 2);	break;
    case ADXL345_INT_DISABLE:	break;
    }
    data[0] = ADXL345_INT_ENABLE;
    twiMasterSendData(adxl345Configuration.deviceAddress, TWI_MASTER_WRITE, data, 3);

    return;
}



// Activation and Inactivation
// THRESH_ACT
// THRESH_INACT
// TIME_INACT
// ACT_INACT_CTL
// ACT_TAP_STATUS (READ)
// POWER_CTL (link, AUTO_SLEEP)
// INT_ENABLE (Activity, Inactivity)
// INT_MAP (Activity, Inactivity)

// // FIFO
// FIFO_CTL
// FIFO_STATUS (READ)
//
// POWER_CTL (sleep, wakeup)
// INT_ENABLE (DATA_READY, Watermark, Overrun)
// INT_MAP (DATA_READY, Watermark, Overrun)
// DATA_FORMAT (SELF_TEST, SPI, INT_INVERT, Justify)
