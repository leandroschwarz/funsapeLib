/* -----------------------------------------------------------------------------
 * Project:     IMU Plotter
 * File:        imuPlotter.c
 * Author:      Kalman Filter for Multirotor Aircrafts
 * Created:		2017-11-03
 * Modified:	2017-11-17
 * Version:     1.5
 * Purpose:     Reads data from attitude sensors and send to PC using Bluetooth.
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// System definitions ----------------------------------------------------------

#define F_CPU 16000000UL

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#include "ATmega328.h"
#include "timer0.h"
#include "timer1.h"
#include "usart.h"
#include "twiMaster.h"
#include "adxl345Twi.h"
#include "l3g4200dTwi.h"
#include <stdlib.h>

// -----------------------------------------------------------------------------
// Project definitions ---------------------------------------------------------

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef union systemFlags_t{
	struct{
		uint8 getNewData	: 1;
		uint8 millis		: 6;
		uint8 unusedBits	: 1;
	};
	uint8 allFlags;
} systemFlags_t;

// -----------------------------------------------------------------------------
// Function declaration --------------------------------------------------------

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

volatile systemFlags_t systemFlags;

// -----------------------------------------------------------------------------
// Main function ---------------------------------------------------------------

int main(void)
{
	// Variable declaration
	l3g4200dDataStatusFlags_t dataStatus;
	int16 accX, accY, accZ;
	int16 gyrX, gyrY, gyrZ;
	uint16 tics;
	uint16 counter = 0;

	// Variable initialization
	systemFlags.allFlags = 0;

	// USART configuration
	usartInit(USART_MODE_ASYNCHRONOUS, USART_BAUD_9600, USART_DATA_BITS_8, USART_PARITY_NO_CHANGE, USART_STOP_BIT_SINGLE);
	usartEnableTransmitter();
	usartStdio();

	// TIMER1 configuration
	timer1Config(TIMER1_MODE_NORMAL, TIMER1_PRESCALER_64);

	// TIMER0 configuration
	timer0SetCompareAValue(250);
	timer0Config(TIMER0_MODE_CTC, TIMER0_PRESCALER_64);
	timer0ActivateCompareAInterrupt();

	// TWI configuration
	twiMasterInit(100000);

	// Enable Global Interrupts
	sei();

	// ADXL345 configuration
	adxl345Init(TRUE);
	adxl345PowerModeSet(ADXL345_MEASURE);
	adxl345RangeSet(ADXL345_4G, FALSE);

	// L3G4200D configuration
	l3g4200dInit(L3G4200D_ADDRESS_0x69);

	while(1){
		if(systemFlags.getNewData){
			// Reads acc e gyro
			adxl345DataGet(&accX, &accY, &accZ, ADXL345_RAW_DATA);
			l3g4200dDataGet(&gyrX, &gyrY, &gyrZ, &dataStatus);
			tics = timer1GetCounterValue();
			timer1SetCounterValue(0);
			printf("%u,%u,%d,%d,%d,%d,%d,%d\r", counter++, tics, accX, accY, accZ, gyrX, gyrY, gyrZ);
			systemFlags.getNewData = 0;
		}
	}

	return 0;
}

// -----------------------------------------------------------------------------
// Interruption handlers -------------------------------------------------------

ISR(TIMER0_COMPA_vect)
{
	systemFlags.millis++;
	if(systemFlags.millis == 40){
		systemFlags.millis = 0;
		systemFlags.getNewData = 1;
	}
}

// -----------------------------------------------------------------------------
// Function definitions --------------------------------------------------------
