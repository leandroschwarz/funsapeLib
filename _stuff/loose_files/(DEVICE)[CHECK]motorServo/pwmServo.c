/* -----------------------------------------------------------------------------
 * Project:			PWM Servo Controller
 * File:			pwmServo.c
 * Author:			Leandro Schwarz
 * Version:			1.0
 * Last edition:	March 18, 2018
 * Purpose:			This project implements a windshield wiper using a PWM servo
 *					motor. The PWM servo motor (controlled by TIMER1 in FAST PWM
 *					mode) has total angle of 120 degrees, PWM Frequency of 50
 *					Hz, minimum Control Pulse of 1ms, and Maximum Control Pulse
 *					of 2ms. The wiper is moved each 1 second (counting the
 *					overflows of TIMER1). The MCU Clock frequency is 16 MHz, and
 *					the device is an ATmega328P.
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// System definitions ----------------------------------------------------------

#define F_CPU 16000000UL

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include <globalDefines.h>
#include <timer1.h>

// -----------------------------------------------------------------------------
// Project definitions ---------------------------------------------------------

#define SERVO_DDR	DDRB
#define SERVO_PORT	PORTB
#define SERVO_BIT	PB2

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

typedef volatile union {
	struct {
		bool_t	changePosition	: 1;
		uint8_t	servoPosition	: 1;
		uint8_t	timer1Overflows	: 6;
	};
	uint8_t allFlags;
} systemFlags_t;

// -----------------------------------------------------------------------------
// Function declaration --------------------------------------------------------

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

systemFlags_t systemFlags;

// -----------------------------------------------------------------------------
// Main function ---------------------------------------------------------------

int main(void)
{
	// Variable declaration

	// Variable initialization
	systemFlags.allFlags = 0;

	// Servo configuration
	setBit(SERVO_DDR, SERVO_BIT);

	// TIMER1 configuration
	timer1Config(TIMER1_MODE_FAST_PWM_OCRA, TIMER1_PRESCALER_8);
	timer1SetCompareAValue(40000);
	timer1SetCompareBValue(2000);
	timer1OutputConfig(TIMER_PORT_NO_CHANGE, TIMER_PORT_NON_INVERTING_MODE);
	timer1ActivateCompareAInterrupt();

	// Enable Global Interrupts
	sei();

	while(1){
		if(systemFlags.changePosition){
			if(systemFlags.servoPosition){
				timer1SetCompareBValue(2000);
			}else{
				timer1SetCompareBValue(4000);
			}
			systemFlags.servoPosition = !systemFlags.servoPosition;
			systemFlags.changePosition = FALSE;
		}
	}

	return 0;
}

// -----------------------------------------------------------------------------
// Interruption handlers -------------------------------------------------------

ISR(TIMER1_COMPA_vect)
{
	if(systemFlags.timer1Overflows == 49){
		systemFlags.changePosition = TRUE;
		systemFlags.timer1Overflows = 0;
	}else{
		systemFlags.timer1Overflows++;
	}
}

// -----------------------------------------------------------------------------
// Function definitions --------------------------------------------------------
