/*!
	\file		externalInterrupt.h
	\brief		External Interrupts interface for the GPDSE AVR8 Integrated Library
	\author		Hazael dos Santos Batista
	\author		Leandro Schwarz
	\date		May 15, 2018
*/

#ifndef __EXTERNAL_INTERRUPT_HPP
#ifndef __DOXYGEN__
	#define __EXTERNAL_INTERRUPT_HPP 1000
#endif	// #ifndef __DOXYGEN__

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.hpp"
#if __GLOBAL_DEFINES_HPP != 1000
	#error Error 100 - globalDefines.hpp - wrong build (globalDefines must be build 2).
#endif	// #if __GLOBAL_DEFINES_H != 2

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

/*!
	\enum			intSenseMode_e
		\details	Sensibility modes associated with external INT0 and INT1.
		\warning	The value associate with the symbolic names may change between builds and doesn't reflect the register/bits real values. In order to ensure compatibility between builds, always refer to the SYMBOLIC NAME, instead of the VALUE.
*/
typedef enum intSenseMode_e {
	INT_SENSE_NO_CHANGE			= 0,	//!< Keep last value configured
	INT_SENSE_LOW_LEVEL			= 1,	//!< Triggers interrupt constantly when in LOW logic level
	INT_SENSE_ANY_EDGE			= 2,	//!< Triggers interrupt at any logical change
	INT_SENSE_FALLING_EDGE		= 3,	//!< Triggers interrupt when a falling edge occurs
	INT_SENSE_RISING_EDGE		= 4		//!< Triggers interrupt when a rising edge occurs
} intSenseMode_e;

/*!
	\enum			pcint2Pins_f
		\details	Pins list flags associated with external PCINT2. The flags may be used with bitwise-or operator.
		\warning	The value associate with the symbolic names may change between builds and doesn't reflect the register/bits real values. In order to ensure compatibility between builds, always refer to the SYMBOLIC NAME, instead of the VALUE.
*/
typedef enum pcint2Pins_f {
	PCINT23_PIN		= (1 << 7),
	PCINT22_PIN		= (1 << 6),
	PCINT21_PIN		= (1 << 5),
	PCINT20_PIN		= (1 << 4),
	PCINT19_PIN		= (1 << 3),
	PCINT18_PIN		= (1 << 2),
	PCINT17_PIN		= (1 << 1),
	PCINT16_PIN		= (1 << 0),
} pcint2Pins_f;

/*!
	\enum			pcint1Pins_f
		\details	Pins list flags associated with external PCINT1. The flags may be used with bitwise-or operator.
		\warning	The value associate with the symbolic names may change between builds and doesn't reflect the register/bits real values. In order to ensure compatibility between builds, always refer to the SYMBOLIC NAME, instead of the VALUE.
*/
typedef enum pcint1Pins_f {
	PCINT14_PIN		= (1 << 6),
	PCINT13_PIN		= (1 << 5),
	PCINT12_PIN		= (1 << 4),
	PCINT11_PIN		= (1 << 3),
	PCINT10_PIN		= (1 << 2),
	PCINT9_PIN		= (1 << 1),
	PCINT8_PIN		= (1 << 0),
} pcint1Pins_f;

/*!
	\enum			pcint0Pins_f
		\details	Pins list flags associated with external PCINT0. The flags may be used with bitwise-or operator.
		\warning	The value associate with the symbolic names may change between builds and doesn't reflect the register/bits real values. In order to ensure compatibility between builds, always refer to the SYMBOLIC NAME, instead of the VALUE.
*/
typedef enum pcint0Pins_f {
	PCINT7_PIN		= (1 << 7),
	PCINT6_PIN		= (1 << 6),
	PCINT5_PIN		= (1 << 5),
	PCINT4_PIN		= (1 << 4),
	PCINT3_PIN		= (1 << 3),
	PCINT2_PIN		= (1 << 2),
	PCINT1_PIN		= (1 << 1),
	PCINT0_PIN		= (1 << 0),
} pcint0Pins_f;

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

// INTs

error_e		int7Config(portMode_e outputMode, intSenseMode_e sense);
error_e		int6Config(portMode_e outputMode, intSenseMode_e sense);
error_e		int5Config(portMode_e outputMode, intSenseMode_e sense);
error_e		int4Config(portMode_e outputMode, intSenseMode_e sense);
error_e		int3Config(portMode_e outputMode, intSenseMode_e sense);
error_e		int2Config(portMode_e outputMode, intSenseMode_e sense);

/*!
	\fn		int1Config(portMode_e outputMode, intSenseMode_e sense)
		\details	Configures the INT1 module
		\param		outputMode	desired behavior of the pin
		\param		sense		desired sensibility to trigger interrupt
		\return					error_e enumeration
*/
error_e		int1Config(portMode_e outputMode, intSenseMode_e sense);

/*!
	\fn		int0Config(portMode_e outputMode, intSenseMode_e sense)
		\details	Configures the INT0 module
		\param		outputMode	desired behavior of the pin
		\param		sense		desired sensibility to trigger interrupt
		\return					error_e enumeration
*/
error_e		int0Config(portMode_e outputMode, intSenseMode_e sense);

// PCINTs

/*!
	\fn		pcint2ActivatePins(pcint2Pins_f pinsList, portMode_e outputMode)
		\details	Activates PCINT2 Port Interrupts pins in an output mode
		\param		pinsList	bitwise-or pins list
		\param		outputMode	desired behavior of the listed pins
		\return					error_e enumeration
*/
error_e		pcint2ActivatePins(pcint2Pins_f pinsList, portMode_e outputMode);

/*!
	\fn		pcint1ActivatePins(pcint1Pins_f pinsList, portMode_e outputMode)
		\details	Activates PCINT1 Port Interrupts pins in an output mode
		\param		pinsList	bitwise-or pins list
		\param		outputMode	desired behavior of the listed pins
		\return					error_e enumeration
*/
error_e		pcint1ActivatePins(pcint1Pins_f pinsList, portMode_e outputMode);

/*!
	\fn		pcint0ActivatePins(pcint0Pins_f pinsList, portMode_e outputMode)
		\details	Activates PCINT0 Port Interrupts pins in an output mode
		\param		pinsList		bitwise-or pins list
		\param		outputMode	desired behavior of the listed pins
		\return					error_e enumeration
*/
error_e		pcint0ActivatePins(pcint0Pins_f pinsList, portMode_e outputMode);

/*!
	\fn		pcint2DeactivatePins(pcint2Pins_f pinsList)
		\details	Deactivates PCINT2 Port Interrupts pins
		\param		pinsList	bitwise-or pins list
		\return					error_e enumeration
*/
error_e		pcint2DeactivatePins(pcint2Pins_f pinsList);

/*!
	\fn		pcint1DeactivatePins(pcint1Pins_f pinsList)
		\details	Deactivates PCINT1 Port Interrupts pins
		\param		pinsList	bitwise-or pins list
		\return					error_e enumeration
*/
error_e		pcint1DeactivatePins(pcint1Pins_f pinsList);

/*!
	\fn		pcint0DeactivatePins(pcint0Pins_f pinsList)
		\details	Deactivates PCINT0 Port Interrupts pins
		\param		pinsList	bitwise-or pins list
		\return					error_e enumeration
*/
error_e		pcint0DeactivatePins(pcint0Pins_f pinsList);

// -----------------------------------------------------------------------------
// Inline function definitions -------------------------------------------------

// Doxygen description
#ifdef __DOXYGEN__

	// INTs

	void		int7ActivateInterrupt(void);
	void		int6ActivateInterrupt(void);
	void		int5ActivateInterrupt(void);
	void		int4ActivateInterrupt(void);
	void		int3ActivateInterrupt(void);
	void		int2ActivateInterrupt(void);

	/*!
	\fn			void int1ActivateInterrupt(void);
	\details	Activates INT1 Interrupt.
	*/
	void		int1ActivateInterrupt(void);

	/*!
	\fn			void int0ActivateInterrupt(void);
	\details	Activates INT0 Interrupt.
	*/
	void		int0ActivateInterrupt(void);



	void		int7DeactivateInterrupt(void);
	void		int6DeactivateInterrupt(void);
	void		int5DeactivateInterrupt(void);
	void		int4DeactivateInterrupt(void);
	void		int3DeactivateInterrupt(void);
	void		int2DeactivateInterrupt(void);

	/*!
	\fn			void int1DeactivateInterrupt(void);
	\details	Deactivates INT1 Interrupt.
	*/
	void		int1DeactivateInterrupt(void);

	/*!
	\fn			void int0DeactivateInterrupt(void);
	\details	Deactivates INT0 Interrupt.
	*/
	void		int0DeactivateInterrupt(void);



	void		int7ClearInterruptRequest(void);
	void		int6ClearInterruptRequest(void);
	void		int5ClearInterruptRequest(void);
	void		int4ClearInterruptRequest(void);
	void		int3ClearInterruptRequest(void);
	void		int2ClearInterruptRequest(void);

	/*!
	\fn			void int1ClearInterruptRequest(void);
	\details	Clears INT1 Interrupt Requests.
	*/
	void		int1ClearInterruptRequest(void);

	/*!
	\fn			void int0ClearInterruptRequest(void);
	\details	Clears INT0 Interrupt Requests.
	*/
	void		int0ClearInterruptRequest(void);

	// PCINTs

	/*!
	\fn			void pcint2Enable(void);
	\details	Enables PCINT2 Port Interrupt.
	*/
	void		pcint2Enable(void);

	/*!
	\fn			void pcint1Enable(void);
	\details	Enables PCINT1 Port Interrupt.
	*/
	void		pcint1Enable(void);

	/*!
	\fn			void pcint0Enable(void);
	\details	Enables PCINT0 Port Interrupt.
	*/
	void		pcint0Enable(void);

	/*!
	\fn			void pcint2Disable(void);
	\details	Disables PCINT2 Port Interrupt.
	*/
	void		pcint2Disable(void);

	/*!
	\fn			void pcint1Disable(void);
	\details	Disables PCINT1 Port Interrupt.
	*/
	void		pcint1Disable(void);

	/*!
	\fn			void pcint0Disable(void);
	\details	Disables PCINT0 Port Interrupt.
	*/
	void		pcint0Disable(void);

	/*!
	\fn			void pcint2ClearInterruptRequest(void);
	\details	Clears PCINT2 Port Interrupt Requests.
	*/
	void		pcint2ClearInterruptRequest(void);

	/*!
	\fn			void pcint1ClearInterruptRequest(void);
	\details	Clears PCINT1 Port Interrupt Requests.
	*/
	void		pcint1ClearInterruptRequest(void);

	/*!
	\fn			void pcint0ClearInterruptRequest(void);
	\details	Clears PCINT0 Port Interrupt Requests.
	*/
	void		pcint0ClearInterruptRequest(void);

#endif	// #ifdef __DOXYGEN__

// Real implementation
#ifndef __DOXYGEN__

void inline __attribute__((always_inline)) int7ActivateInterrupt(void)
{
	setBit(EIMSK, INT7);
	return;
}

void inline __attribute__((always_inline)) int6ActivateInterrupt(void)
{
	setBit(EIMSK, INT6);
	return;
}

void inline __attribute__((always_inline)) int5ActivateInterrupt(void)
{
	setBit(EIMSK, INT5);
	return;
}

void inline __attribute__((always_inline)) int4ActivateInterrupt(void)
{
	setBit(EIMSK, INT4);
	return;
}

void inline __attribute__((always_inline)) int3ActivateInterrupt(void)
{
	setBit(EIMSK, INT3);
	return;
}

void inline __attribute__((always_inline)) int2ActivateInterrupt(void)
{
	setBit(EIMSK, INT2);
	return;
}

void inline __attribute__((always_inline)) int1ActivateInterrupt(void)
{
	setBit(EIMSK, INT1);
	return;
}

void inline __attribute__((always_inline)) int0ActivateInterrupt(void)
{
	setBit(EIMSK, INT0);
	return;
}

void inline __attribute__((always_inline)) int7DeactivateInterrupt(void)
{
	clrBit(EIMSK, INT7);
	return;
}

void inline __attribute__((always_inline)) int6DeactivateInterrupt(void)
{
	clrBit(EIMSK, INT6);
	return;
}

void inline __attribute__((always_inline)) int5DeactivateInterrupt(void)
{
	clrBit(EIMSK, INT5);
	return;
}

void inline __attribute__((always_inline)) int4DeactivateInterrupt(void)
{
	clrBit(EIMSK, INT4);
	return;
}

void inline __attribute__((always_inline)) int3DeactivateInterrupt(void)
{
	clrBit(EIMSK, INT3);
	return;
}

void inline __attribute__((always_inline)) int2DeactivateInterrupt(void)
{
	clrBit(EIMSK, INT2);
	return;
}

void inline __attribute__((always_inline)) int1DeactivateInterrupt(void)
{
	clrBit(EIMSK, INT1);
	return;
}

void inline __attribute__((always_inline)) int0DeactivateInterrupt(void)
{
	clrBit(EIMSK, INT0);
	return;
}

void inline __attribute__((always_inline)) int7ClearInterruptRequest(void)
{
	setBit(EIFR, INTF7);
	return;
}

void inline __attribute__((always_inline)) int6ClearInterruptRequest(void)
{
	setBit(EIFR, INTF6);
	return;
}

void inline __attribute__((always_inline)) int5ClearInterruptRequest(void)
{
	setBit(EIFR, INTF5);
	return;
}

void inline __attribute__((always_inline)) int4ClearInterruptRequest(void)
{
	setBit(EIFR, INTF4);
	return;
}

void inline __attribute__((always_inline)) int3ClearInterruptRequest(void)
{
	setBit(EIFR, INTF3);
	return;
}

void inline __attribute__((always_inline)) int2ClearInterruptRequest(void)
{
	setBit(EIFR, INTF2);
	return;
}

void inline __attribute__((always_inline)) int1ClearInterruptRequest(void)
{
	setBit(EIFR, INTF1);
	return;
}

void inline __attribute__((always_inline)) int0ClearInterruptRequest(void)
{
	setBit(EIFR, INTF0);
	return;
}

// PCINTs

void inline __attribute__((always_inline)) pcint2Enable(void)
{
	setBit(PCICR, PCIE2);
	return;
}

void inline __attribute__((always_inline)) pcint1Enable(void)
{
	setBit(PCICR, PCIE1);
	return;
}

void inline __attribute__((always_inline)) pcint0Enable(void)
{
	setBit(PCICR, PCIE0);
	return;
}

void inline __attribute__((always_inline)) pcint2Disable(void)
{
	clrBit(PCICR, PCIE2);
	return;
}

void inline __attribute__((always_inline)) pcint1Disable(void)
{
	clrBit(PCICR, PCIE1);
	return;
}

void inline __attribute__((always_inline)) pcint0Disable(void)
{
	clrBit(PCICR, PCIE0);
	return;
}

void inline __attribute__((always_inline)) pcint2ClearInterruptRequest(void)
{
	setBit(PCIFR, PCIF2);
	return;
}

void inline __attribute__((always_inline)) pcint1ClearInterruptRequest(void)
{
	setBit(PCIFR, PCIF1);
	return;
}

void inline __attribute__((always_inline)) pcint0ClearInterruptRequest(void)
{
	setBit(PCIFR, PCIF0);
	return;
}

#endif	// #ifndef __DOXYGEN__

#endif	// #ifndef __EXTERNAL_INTERRUPT_H
