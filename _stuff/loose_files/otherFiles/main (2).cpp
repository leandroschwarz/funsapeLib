/* =============================================================================
 * Project:		Accelerometer Data Logger Using AVR
 * File name:	main.cpp
 * Author:		Leandro Schwarz
 * Date:		October 02, 2018
 * Purpose:		Acquires signal from 4 3-channel accelerometers and 1 ECG
 * 				analog channel, sends the data under Bluetooth protocol and
 *				stores in a SD card.
 * ========================================================================== */

// =============================================================================
// Include files
// =============================================================================

#include <gpdseAvrGlobalDefines.hpp>
#include <gpdseAvrUsart.hpp>
#include <gpdseAvrSpi.hpp>
#include <gpdseAvrMpu9250.hpp>
#include <gpdseAvrTimer.hpp>

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define		TICKS_SAMPLE_RATE			1000
#define		MPU9250_DEVICE_0_BIT		PD2
#define		MPU9250_DEVICE_0_OUT		PORTD
#define		MPU9250_DEVICE_0_DDR		DDRD
#define		MPU9250_DEVICE_1_BIT		PD3
#define		MPU9250_DEVICE_1_OUT		PORTD
#define		MPU9250_DEVICE_1_DDR		DDRD
#define		MPU9250_DEVICE_2_BIT		PD4
#define		MPU9250_DEVICE_2_OUT		PORTD
#define		MPU9250_DEVICE_2_DDR		DDRD
#define		MPU9250_DEVICE_3_BIT		PD5
#define		MPU9250_DEVICE_3_OUT		PORTD
#define		MPU9250_DEVICE_3_DDR		DDRD

// =============================================================================
// Data Stored in Flash
// =============================================================================

// NONE

// =============================================================================
// File exclusive - New data types
// =============================================================================

typedef union systemFlags_f {
	struct {
		bool		accDataReady		: 1;
		bool		adcDataReady		: 1;
		bool		sampleReady			: 1;
		bool		packageReady		: 1;
		uint32_t	ticks				: 28;
	};
	uint32_t all;
} systemFlags_f;

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

#undef ledEnable
#undef ledToggle
#undef ledTurnOn
#undef ledTurnOff
#define ledEnable()								setBit(DDRC, PC3)
#define ledToggle()								cplBit(PORTC, PC3)
#define ledTurnOn()								setBit(PORTC, PC3)
#define ledTurnOff()							clrBit(PORTC, PC3)

#define timer0Config()							do{OCR0A = 250;TCCR0A = 2;} while (0)
#define timer0Start()							do{TCNT0 = 0;TCCR0B = 3;}while(0)
#define timer0Resume()							do{TCCR0B = 3;}while(0)
#define timer0Pause()							do{TCCR0B = 0;}while(0)
#define timer0Stop()							do{TCNT0 = 0;TCCR0B = 0;}while(0)
#define timer0ActivateCompareAInterrupt()		do{setBit(TIMSK0, OCIE0A);}while(0)
#define timer0DeactivateCompareAInterrupt()		do{clrBit(TIMSK0, OCIE0A);}while(0)
#define timer0ClearCompareAInterruptRequest()	do{setBit(TIFR0, OCF0A);}while(0)

// =============================================================================
// Function declarations
// =============================================================================

void mpu9250ActivateDevice0(void);
void mpu9250ActivateDevice1(void);
void mpu9250ActivateDevice2(void);
void mpu9250ActivateDevice3(void);
void mpu9250DeactivateDevice0(void);
void mpu9250DeactivateDevice1(void);
void mpu9250DeactivateDevice2(void);
void mpu9250DeactivateDevice3(void);

// =============================================================================
// Global variables
// =============================================================================

//     /////////////////////////    MPU9250     /////////////////////////     //
Mpu9250 mpu0;
Mpu9250 mpu1;
Mpu9250 mpu2;
Mpu9250 mpu3;

uint8_t		txFrame[285];
vuint16_t	txIndex = 0;
vuint16_t	txSize = 0;

volatile systemFlags_f	systemFlags;


// =============================================================================
// Main function
// =============================================================================

int main(void)
{
	// =========================================================================
	// General purpose variables
	// =========================================================================

	// =========================================================================
	// Variable initialization
	// =========================================================================

	ledEnable();
	ledTurnOff();

	// =========================================================================
	// Peripheral configuration
	// =========================================================================

	//     ///////////////////////    USART 0     ///////////////////////     //
	if (usart0.getLastError()) {				// Check object integrity
	}
	usart0.setTransmitter(ACTIVATION_ON);		// Enable transmitter
//	usart0.setReceiver(ACTIVATION_ON);			// Enable receiver
	if (!usart0.initialize()) {					// Initialization routine
	}
	usart0.remapStdio();
	printf("Teste da USART\r");

	//     ////////////////////////    SPI 0     ////////////////////////     //
	if (spi0.getLastError()) {					// Check object integrity
		printf("SPI Object Error - %d\r", spi0.getLastError());
		systemHalt();
	}

	//     ///////////////////////    TIMER 0     ///////////////////////     //
	timer0Config();
	Timer0 timer0;
	uint32_t aux32;

	//     /////////////////     MPU9250 - DEVICE 0     /////////////////     //
	if (mpu0.getLastError()) {					// Check object integrity
		printf("MPU0 Object Error - %d\r", mpu0.getLastError());
		systemHalt();
	}
	if (!mpu0.initialize(&spi0, mpu9250ActivateDevice0, mpu9250DeactivateDevice0)) {
		printf("MPU0 Initialization Error - %d\r", mpu0.getLastError());
		systemHalt();
	}

	//     /////////////////     MPU9250 - DEVICE 1     /////////////////     //
	if (mpu1.getLastError()) {					// Check object integrity
		printf("MPU1 Object Error - %d\r", mpu1.getLastError());
		systemHalt();
	}
	if (!mpu1.initialize(&spi0, mpu9250ActivateDevice1, mpu9250DeactivateDevice1)) {
		printf("MPU1 Initialization Error - %d\r", mpu1.getLastError());
		systemHalt();
	}

	//     /////////////////     MPU9250 - DEVICE 2     /////////////////     //
	if (mpu2.getLastError()) {					// Check object integrity
		printf("MPU2 Object Error - %d\r", mpu2.getLastError());
		systemHalt();
	}
	if (!mpu2.initialize(&spi0, mpu9250ActivateDevice2, mpu9250DeactivateDevice2)) {
		printf("MPU2 Initialization Error - %d\r", mpu2.getLastError());
		systemHalt();
	}

	//     /////////////////     MPU9250 - DEVICE 3     /////////////////     //
	if (mpu3.getLastError()) {					// Check object integrity
		printf("MPU3 Object Error - %d\r", mpu3.getLastError());
		systemHalt();
	}
	if (!mpu3.initialize(&spi0, mpu9250ActivateDevice3, mpu9250DeactivateDevice3)) {
		printf("MPU3 Initialization Error - %d\r", mpu3.getLastError());
		systemHalt();
	}

	// =========================================================================
	// Interrupts
	// =========================================================================

	//     ///////////////////////    USART 0     ///////////////////////     //
	usart0.flushDataReceptionBuffer();				// Flush USART data buffer
	usart0.setInterruptReceptionComplete(false);	// Enable USART RX interrupt
	usart0.setInterruptDataBufferEmpty(false);		// Enable USART TX interrupt

	//     ///////////////////////    TIMER 0     ///////////////////////     //
	timer0ClearCompareAInterruptRequest();
	timer0ActivateCompareAInterrupt();
	timer0Start();

	//     ///////////////////////     GLOBAL     ///////////////////////     //
	sei();

	// -------------------------------------------------------------------------
	// Main Loop ---------------------------------------------------------------

	while (1) {

	}

	return 0;							// Never reached return
}

// =============================================================================
// Interruption Handlers
// =============================================================================

ISR(USART_RX_vect)
{

}

ISR(USART_TX_vect)
{

}

ISR(TIMER0_COMPA_vect)
{
	systemFlags.ticks++;
	if (systemFlags.ticks == TICKS_SAMPLE_RATE) {
		systemFlags.ticks = 0;
		/////////////////////////////////// Start acquisition
		ledToggle();
	}
}

// =============================================================================
// Function Definitions
// =============================================================================


void mpu9250ActivateDevice0(void)
{
	clrBit(MPU9250_DEVICE_0_OUT, MPU9250_DEVICE_0_BIT);
}

void mpu9250ActivateDevice1(void)
{
	clrBit(MPU9250_DEVICE_1_OUT, MPU9250_DEVICE_1_BIT);
}

void mpu9250ActivateDevice2(void)
{
	clrBit(MPU9250_DEVICE_2_OUT, MPU9250_DEVICE_2_BIT);
}

void mpu9250ActivateDevice3(void)
{
	clrBit(MPU9250_DEVICE_3_OUT, MPU9250_DEVICE_3_BIT);
}

void mpu9250DeactivateDevice0(void)
{
	setBit(MPU9250_DEVICE_0_OUT, MPU9250_DEVICE_0_BIT);
}

void mpu9250DeactivateDevice1(void)
{
	setBit(MPU9250_DEVICE_1_OUT, MPU9250_DEVICE_1_BIT);
}

void mpu9250DeactivateDevice2(void)
{
	setBit(MPU9250_DEVICE_2_OUT, MPU9250_DEVICE_2_BIT);
}

void mpu9250DeactivateDevice3(void)
{
	setBit(MPU9250_DEVICE_3_OUT, MPU9250_DEVICE_3_BIT);
}
