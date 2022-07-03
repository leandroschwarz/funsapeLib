/* =============================================================================
 * Project:		Acceleration Data Logger
 * File name:	main.cpp
 * Authors:		Leandro Schwarz
 * Date:		August 30, 2018
 * ========================================================================== */

// =============================================================================
// Include files
// =============================================================================

#include <avr/interrupt.h>
#include <globalDefines.hpp>
#include <stdio.h>
#include <string.h>
#include <usart0.hpp>
#include <timer.hpp>
#include <spi.hpp>
#include <mpu9250.hpp>

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define IMU0_CS_DIR		DDRD
#define IMU1_CS_DIR		DDRD
#define IMU2_CS_DIR		DDRD
#define IMU3_CS_DIR		DDRD
#define IMU0_CS_OUT		PORTD
#define IMU1_CS_OUT		PORTD
#define IMU2_CS_OUT		PORTD
#define IMU3_CS_OUT		PORTD
#define IMU0_CS_BIT		PD2
#define IMU1_CS_BIT		PD3
#define IMU2_CS_BIT		PD4
#define IMU3_CS_BIT		PD5

// =============================================================================
// Data Stored in Flash
// =============================================================================

// NONE

// =============================================================================
// File exclusive - New data types
// =============================================================================

// NONE

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

// NONE

// =============================================================================
// Global variables
// =============================================================================

Mpu9250 imu0;
Mpu9250 imu1;
Mpu9250 imu2;
Mpu9250 imu3;

// =============================================================================
// Function declarations
// =============================================================================

void imu0Select(void)
{
	clrBit(IMU0_CS_OUT, IMU0_CS_BIT);
}

void imu0Deselect(void)
{
	setBit(IMU0_CS_OUT, IMU0_CS_BIT);
}

void imu1Select(void)
{
	clrBit(IMU0_CS_OUT, IMU0_CS_BIT);
}

void imu1Deselect(void)
{
	setBit(IMU0_CS_OUT, IMU0_CS_BIT);
}

void imu2Select(void)
{
	clrBit(IMU0_CS_OUT, IMU0_CS_BIT);
}

void imu2Deselect(void)
{
	setBit(IMU0_CS_OUT, IMU0_CS_BIT);
}

void imu3Select(void)
{
	clrBit(IMU0_CS_OUT, IMU0_CS_BIT);
}

void imu3Deselect(void)
{
	setBit(IMU0_CS_OUT, IMU0_CS_BIT);
}

// NONE

// =============================================================================
// Main function
// =============================================================================

// =============================================================================
// Main function ==============================================================-

int main(void)
{
	// =========================================================================
	// General purpose variables
	// =========================================================================

	char auxStr[40];

	// =========================================================================
	// Variable initialization
	// =========================================================================

	// NONE

	setBit(IMU0_CS_OUT, IMU0_CS_BIT);
	setBit(IMU1_CS_OUT, IMU1_CS_BIT);
	setBit(IMU2_CS_OUT, IMU2_CS_BIT);
	setBit(IMU3_CS_OUT, IMU3_CS_BIT);
	setBit(IMU0_CS_DIR, IMU0_CS_BIT);
	setBit(IMU1_CS_DIR, IMU1_CS_BIT);
	setBit(IMU2_CS_DIR, IMU2_CS_BIT);
	setBit(IMU3_CS_DIR, IMU3_CS_BIT);


	// =========================================================================
	// Enable Global Interrupts
	// =========================================================================

	_delay_ms(3000);


	spi0.init();
	sei();

	// =========================================================================
	// USART configuration
	// =========================================================================

	Usart0 usart0;
	usart0.setBaudRate(USART_BAUD_57600);
	usart0.initialize();
	usart0.enableTransmitter();
	usart0.stdio();
	printf("Teste da USART\r\r");

	// =========================================================================
	// MPU92450 configuration
	// =========================================================================

	do {
		error_e error;

		// Check if object was created
		error = imu0.getLastError();
		if (error) {
			printf("IMU0 Error %d\r", error);
			systemHalt();
		} else {
			printf("IMU0 object created\r");
		}
		error = imu1.getLastError();
		if (error) {
			printf("IMU1 Error %d\r", error);
			systemHalt();
		} else {
			printf("IMU1 object created\r");
		}
		error = imu2.getLastError();
		if (error) {
			printf("IMU2 Error %d\r", error);
			systemHalt();
		} else {
			printf("IMU2 object created\r");
		}
		error = imu3.getLastError();
		if (error) {
			printf("IMU3 Error %d\r", error);
			systemHalt();
		} else {
			printf("IMU3 object created\r\r");
		}
	} while (0);

	// Initialize IMU
	if (!imu0.initSpi(&spi0, imu0Select, imu0Deselect)) {
		printf("IMU0 Error %d\r", imu0.getLastError());
		systemHalt();
	} else {
		printf("IMU0 initialized\r");
	}
	if (!imu1.initSpi(&spi0, imu1Select, imu1Deselect)) {
		printf("IMU1 Error %d\r", imu1.getLastError());
		systemHalt();
	} else {
		printf("IMU1 initialized\r");
	}
	if (!imu2.initSpi(&spi0, imu2Select, imu2Deselect)) {
		printf("IMU2 Error %d\r", imu2.getLastError());
		systemHalt();
	} else {
		printf("IMU2 initialized\r");
	}
	if (!imu3.initSpi(&spi0, imu3Select, imu3Deselect)) {
		printf("IMU3 Error %d\r", imu3.getLastError());
		systemHalt();
	} else {
		printf("IMU3 initialized\r\r");
	}








	// =========================================================================
	// TIMER configuration
	// =========================================================================

//	timer0.init(TIMER0_MODE_NORMAL, TIMER0_CLOCK_PRESCALER_1024);
//	timer0.activateOverflowInterrupt();

	// =========================================================================
	// SD Card configuration
	// =========================================================================

	while (1) {
	}

	return 0;
}

// =============================================================================
// Interruption handlers ======================================================-

ISR(TIMER0_OVF_vect)
{
}

// =============================================================================
// Function definitions ========================================================
