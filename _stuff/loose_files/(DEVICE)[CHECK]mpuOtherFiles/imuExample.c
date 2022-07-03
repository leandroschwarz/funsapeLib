/* -----------------------------------------------------------------------------
 * Project:     IMU Storage
 * File:        imuStorage.c
 * Author:      Leandro Schwarz
 * Created:     2017-11-08
 * Modified:    2017-11-24
 * Version:     1.9
 * Purpose:     Reads data from attitude sensors and writes at the SD card.
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// System definitions ----------------------------------------------------------

#define F_CPU 16000000UL

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#include "ATmega328.h"
#include "timer1.h"
#include "timer0.h"
#include "twiMaster.h"
#include "adxl345Twi.h"
#include "l3g4200dTwi.h"
#include "ff.h"
#include "spi.h"
#include "usart.h"

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
	FATFS card;
	FIL file;
	char string[120];

	// Variable initialization
	systemFlags.allFlags = 0;

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

	// SPI configuration
	SPI_Init();

	// SD card mount
	f_mount(0, &card);
	f_open(&file, "data.txt", FA_WRITE | FA_CREATE_ALWAYS);

	while(1){
		if(systemFlags.getNewData){
			// Reads acc e gyro
			adxl345DataGet(&accX, &accY, &accZ, ADXL345_RAW_DATA);
			l3g4200dDataGet(&gyrX, &gyrY, &gyrZ, &dataStatus);
			tics = timer1GetCounterValue();
			timer1SetCounterValue(0);
			// Stores in SD card
			sprintf(string, "%u;%u;%d;%d;%d;%d;%d;%d\r\n", counter++, tics, accX, accY, accZ, gyrX, gyrY, gyrZ);
			f_puts(string, &file);
			f_sync(&file);
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
