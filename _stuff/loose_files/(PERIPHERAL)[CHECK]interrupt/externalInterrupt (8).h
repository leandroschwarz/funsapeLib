/*!
	\file		globalDefines.h
	\brief		Global definitions for the GPDSE AVR8 Integrated Library
	\author		Hazael dos Santos Batista
	\author		Leandro Schwarz
	\date		May 5, 2018
*/

#ifndef __EXTERNAL_INTERRUPT_H
#ifndef __DOXYGEN__
	#define __EXTERNAL_INTERRUPT_H 2
#endif

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalDefines.h"
#if __GLOBAL_DEFINES_H != 2
	#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 2).
#endif

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

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

// INTs

/*!
	\fn			int1Config(portMode_e port, intSenseMode_e sense)
		\details	Configures the INT1 module
		\param		port		desired behavior of the pin
		\param		sense		desired sensibility to trigger interrupt
		\return					error_e enumeration
*/
error_e		int1Config(portMode_e port, intSenseMode_e sense);

/*!
	\fn			int0Config(portMode_e port, intSenseMode_e sense)
		\details	Configures the INT0 module
		\param		port		desired behavior of the pin
		\param		sense		desired sensibility to trigger interrupt
		\return					error_e enumeration
*/
error_e		int0Config(portMode_e port, intSenseMode_e sense);

// PCINTs

/*!
	\fn			pcint23ActivateInterrupt(portMode_e port)
		\details	Configures the PCINT23 pin
		\param		port		desired behavior of the pin
		\return					error_e enumeration
*/
error_e		pcint23ActivateInterrupt(portMode_e port);

/*!
	\fn			pcint22ActivateInterrupt(portMode_e port)
		\details	Configures the PCINT22 pin
		\param		port		desired behavior of the pin
		\return					error_e enumeration
*/
error_e		pcint22ActivateInterrupt(portMode_e port);

/*!
	\fn			pcint21ActivateInterrupt(portMode_e port)
		\details	Configures the PCINT21 pin
		\param		port		desired behavior of the pin
		\return					error_e enumeration
*/
error_e		pcint21ActivateInterrupt(portMode_e port);

/*!
	\fn			pcint20ActivateInterrupt(portMode_e port)
		\details	Configures the PCINT20 pin
		\param		port		desired behavior of the pin
		\return					error_e enumeration
*/
error_e		pcint20ActivateInterrupt(portMode_e port);

/*!
	\fn			pcint19ActivateInterrupt(portMode_e port)
		\details	Configures the PCINT19 pin
		\param		port		desired behavior of the pin
		\return					error_e enumeration
*/
error_e		pcint19ActivateInterrupt(portMode_e port);

/*!
	\fn			pcint18ActivateInterrupt(portMode_e port)
		\details	Configures the PCINT18 pin
		\param		port		desired behavior of the pin
		\return					error_e enumeration
*/
error_e		pcint18ActivateInterrupt(portMode_e port);

/*!
	\fn			pcint17ActivateInterrupt(portMode_e port)
		\details	Configures the PCINT17 pin
		\param		port		desired behavior of the pin
		\return					error_e enumeration
*/
error_e		pcint17ActivateInterrupt(portMode_e port);

/*!
	\fn			pcint16ActivateInterrupt(portMode_e port)
		\details	Configures the PCINT16 pin
		\param		port		desired behavior of the pin
		\return					error_e enumeration
*/
error_e		pcint16ActivateInterrupt(portMode_e port);

/*!
	\fn			pcint14ActivateInterrupt(portMode_e port)
		\details	Configures the PCINT14 pin
		\param		port		desired behavior of the pin
		\return					error_e enumeration
*/
error_e		pcint14ActivateInterrupt(portMode_e port);

/*!
	\fn			pcint13ActivateInterrupt(portMode_e port)
		\details	Configures the PCINT13 pin
		\param		port		desired behavior of the pin
		\return					error_e enumeration
*/
error_e		pcint13ActivateInterrupt(portMode_e port);

/*!
	\fn			pcint12ActivateInterrupt(portMode_e port)
		\details	Configures the PCINT12 pin
		\param		port		desired behavior of the pin
		\return					error_e enumeration
*/
error_e		pcint12ActivateInterrupt(portMode_e port);

/*!
	\fn			pcint11ActivateInterrupt(portMode_e port)
		\details	Configures the PCINT11 pin
		\param		port		desired behavior of the pin
		\return					error_e enumeration
*/
error_e		pcint11ActivateInterrupt(portMode_e port);

/*!
	\fn			pcint10ActivateInterrupt(portMode_e port)
		\details	Configures the PCINT10 pin
		\param		port		desired behavior of the pin
		\return					error_e enumeration
*/
error_e		pcint10ActivateInterrupt(portMode_e port);

/*!
	\fn			pcint9ActivateInterrupt(portMode_e port)
		\details	Configures the PCINT9 pin
		\param		port		desired behavior of the pin
		\return					error_e enumeration
*/
error_e		pcint9ActivateInterrupt(portMode_e port);

/*!
	\fn			pcint8ActivateInterrupt(portMode_e port)
		\details	Configures the PCINT8 pin
		\param		port		desired behavior of the pin
		\return					error_e enumeration
*/
error_e		pcint8ActivateInterrupt(portMode_e port);

/*!
	\fn			pcint7ActivateInterrupt(portMode_e port)
		\details	Configures the PCINT7 pin
		\param		port		desired behavior of the pin
		\return					error_e enumeration
*/
error_e		pcint7ActivateInterrupt(portMode_e port);

/*!
	\fn			pcint6ActivateInterrupt(portMode_e port)
		\details	Configures the PCINT6 pin
		\param		port		desired behavior of the pin
		\return					error_e enumeration
*/
error_e		pcint6ActivateInterrupt(portMode_e port);

/*!
	\fn			pcint5ActivateInterrupt(portMode_e port)
		\details	Configures the PCINT5 pin
		\param		port		desired behavior of the pin
		\return					error_e enumeration
*/
error_e		pcint5ActivateInterrupt(portMode_e port);

/*!
	\fn			pcint4ActivateInterrupt(portMode_e port)
		\details	Configures the PCINT4 pin
		\param		port		desired behavior of the pin
		\return					error_e enumeration
*/
error_e		pcint4ActivateInterrupt(portMode_e port);

/*!
	\fn			pcint3ActivateInterrupt(portMode_e port)
		\details	Configures the PCINT3 pin
		\param		port		desired behavior of the pin
		\return					error_e enumeration
*/
error_e		pcint3ActivateInterrupt(portMode_e port);

/*!
	\fn			pcint2ActivateInterrupt(portMode_e port)
		\details	Configures the PCINT2 pin
		\param		port		desired behavior of the pin
		\return					error_e enumeration
*/
error_e		pcint2ActivateInterrupt(portMode_e port);

/*!
	\fn			pcint1ctivateInterrupt(portMode_e port)
		\details	Configures the PCINT1 pin
		\param		port		desired behavior of the pin
		\return					error_e enumeration
*/
error_e		pcint1ActivateInterrupt(portMode_e port);

/*!
	\fn			pcint0ActivateInterrupt(portMode_e port)
		\details	Configures the PCINT0 pin
		\param		port		desired behavior of the pin
		\return					error_e enumeration
*/
error_e		pcint0ActivateInterrupt(portMode_e port);

// -----------------------------------------------------------------------------
// Inline function definitions -------------------------------------------------

// Doxygen description
#ifdef __DOXYGEN__

	// INTs

	/*!
	\fn			void timer0ActivateOverflowInterrupt(void);
	\details	Activates TIMER0 Overflow Interrupt.
	*/
	void		int1ActivateInterrupt(void);
	void		int0ActivateInterrupt(void);
	void		int1DeactivateInterrupt(void);
	void		int0DeactivateInterrupt(void);
	void		int1ClearInterruptRequest(void);
	void		int0ClearInterruptRequest(void);

	// PCINTs

	void			pcint23_16Enable(void);
	void			pcint14_8Enable(void);
	void			pcint7_0Enable(void);
	void			pcint23_16Disable(void);
	void			pcint14_8Disable(void);
	void			pcint7_0Disable(void);
	void			pcint23_16ClearInterruptRequest(void);
	void			pcint14_8ClearInterruptRequest(void);
	void			pcint7_0ClearInterruptRequest(void);
	void			pcint23DeactivateInterrupt(void);
	void			pcint22DeactivateInterrupt(void);
	void			pcint21DeactivateInterrupt(void);
	void			pcint20DeactivateInterrupt(void);
	void			pcint19DeactivateInterrupt(void);
	void			pcint18DeactivateInterrupt(void);
	void			pcint17DeactivateInterrupt(void);
	void			pcint16DeactivateInterrupt(void);
	void			pcint14DeactivateInterrupt(void);
	void			pcint13DeactivateInterrupt(void);
	void			pcint12DeactivateInterrupt(void);
	void			pcint11DeactivateInterrupt(void);
	void			pcint10DeactivateInterrupt(void);
	void			pcint9DeactivateInterrupt(void);
	void			pcint8DeactivateInterrupt(void);
	void			pcint7DeactivateInterrupt(void);
	void			pcint6DeactivateInterrupt(void);
	void			pcint5DeactivateInterrupt(void);
	void			pcint4DeactivateInterrupt(void);
	void			pcint3DeactivateInterrupt(void);
	void			pcint2DeactivateInterrupt(void);
	void			pcint1DeactivateInterrupt(void);
	void			pcint0DeactivateInterrupt(void);

#endif

// Real implementation
#ifndef __DOXYGEN__

void inline __attribute__((always_inline)) timer0ActivateOverflowInterrupt(void)
{
	setBit(TIMSK0, TOIE0);
	return;
}




#endif







#endif
