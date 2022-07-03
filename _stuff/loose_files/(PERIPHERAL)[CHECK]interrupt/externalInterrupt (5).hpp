/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	externalInterrupt.hpp
 * Module:		External Interrupt Module Interface for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1002
 * Date:		July 04, 2018
 * ========================================================================== */

#ifndef __EXTERNAL_INTERRUPT_HPP
#define __EXTERNAL_INTERRUPT_HPP 1002

// =============================================================================
// Dependencies
// =============================================================================

// Global definitions file
#include "globalDefines.hpp"
#if __GLOBAL_DEFINES_HPP != 1002
#	error	[externalInterrupt.hpp] Error 101 - Global definitions file (globalDefines.hpp) must be build 1002.
#endif

// Header files
#include <avr/interrupt.h>

// =============================================================================
// Undefining previous definitions
// =============================================================================

/* NONE */

// =============================================================================
// Constants
// =============================================================================

/* NONE */

// =============================================================================
// Macro-functions
// =============================================================================

/* NONE */

// =============================================================================
// New data types
// =============================================================================

// -----------------------------------------------------------------------------
// IntX ------------------------------------------------------------------------

#if INT0_EXISTS || INT1_EXISTS || INT2_EXISTS || INT3_EXISTS || \
	INT4_EXISTS || INT5_EXISTS || INT6_EXISTS || INT7_EXISTS
typedef enum intSenseMode_e {
	INT_SENSE_NO_CHANGE			= 0,
	INT_SENSE_LOW_LEVEL			= 1,
	INT_SENSE_ANY_EDGE			= 2,
	INT_SENSE_FALLING_EDGE		= 3,
	INT_SENSE_RISING_EDGE		= 4
} intSenseMode_e;
#endif

// -----------------------------------------------------------------------------
// Pcint0 ----------------------------------------------------------------------

#if PCINT0_EXISTS  || PCINT1_EXISTS  || PCINT2_EXISTS  || PCINT3_EXISTS  || \
	PCINT4_EXISTS  || PCINT5_EXISTS  || PCINT6_EXISTS  || PCINT7_EXISTS
typedef enum pcint0Pins_f {
#	if PCINT7_EXISTS
	PCINT7_PIN		= (1 << 7),
#	endif
#	if PCINT6_EXISTS
	PCINT6_PIN		= (1 << 6),
#	endif
#	if PCINT5_EXISTS
	PCINT5_PIN		= (1 << 5),
#	endif
#	if PCINT4_EXISTS
	PCINT4_PIN		= (1 << 4),
#	endif
#	if PCINT3_EXISTS
	PCINT3_PIN		= (1 << 3),
#	endif
#	if PCINT2_EXISTS
	PCINT2_PIN		= (1 << 2),
#	endif
#	if PCINT1_EXISTS
	PCINT1_PIN		= (1 << 1),
#	endif
#	if PCINT0_EXISTS
	PCINT0_PIN		= (1 << 0),
#	endif
} pcint0Pins_f;

inline __attribute__((always_inline)) pcint0Pins_f operator |(pcint0Pins_f a, pcint0Pins_f b)
{
	return static_cast<pcint0Pins_f>(static_cast<int>(a) | static_cast<int>(b));
}

inline __attribute__((always_inline)) pcint0Pins_f operator &(pcint0Pins_f a, pcint0Pins_f b)
{
	return static_cast<pcint0Pins_f>(static_cast<int>(a) & static_cast<int>(b));
}

inline __attribute__((always_inline)) pcint0Pins_f operator ^(pcint0Pins_f a, pcint0Pins_f b)
{
	return static_cast<pcint0Pins_f>(static_cast<int>(a) ^ static_cast<int>(b));
}
#endif

// -----------------------------------------------------------------------------
// Pcint1 ----------------------------------------------------------------------

#if PCINT8_EXISTS  || PCINT9_EXISTS  || PCINT10_EXISTS || PCINT11_EXISTS || \
	PCINT12_EXISTS || PCINT13_EXISTS || PCINT14_EXISTS || PCINT15_EXISTS
typedef enum pcint1Pins_f {
#	if PCINT15_EXISTS
	PCINT15_PIN		= (1 << 7),
#	endif
#	if PCINT14_EXISTS
	PCINT14_PIN		= (1 << 6),
#	endif
#	if PCINT13_EXISTS
	PCINT13_PIN		= (1 << 5),
#	endif
#	if PCINT12_EXISTS
	PCINT12_PIN		= (1 << 4),
#	endif
#	if PCINT11_EXISTS
	PCINT11_PIN		= (1 << 3),
#	endif
#	if PCINT10_EXISTS
	PCINT10_PIN		= (1 << 2),
#	endif
#	if PCINT9_EXISTS
	PCINT9_PIN		= (1 << 1),
#	endif
#	if PCINT8_EXISTS
	PCINT8_PIN		= (1 << 0),
#	endif
} pcint1Pins_f;

inline __attribute__((always_inline)) pcint1Pins_f operator |(pcint1Pins_f a, pcint1Pins_f b)
{
	return static_cast<pcint1Pins_f>(static_cast<int>(a) | static_cast<int>(b));
}

inline __attribute__((always_inline)) pcint1Pins_f operator &(pcint1Pins_f a, pcint1Pins_f b)
{
	return static_cast<pcint1Pins_f>(static_cast<int>(a) & static_cast<int>(b));
}

inline __attribute__((always_inline)) pcint1Pins_f operator ^(pcint1Pins_f a, pcint1Pins_f b)
{
	return static_cast<pcint1Pins_f>(static_cast<int>(a) ^ static_cast<int>(b));
}
#endif

// -----------------------------------------------------------------------------
// Pcint2 ----------------------------------------------------------------------

#if PCINT16_EXISTS || PCINT17_EXISTS || PCINT18_EXISTS || PCINT19_EXISTS || \
	PCINT20_EXISTS || PCINT21_EXISTS || PCINT22_EXISTS || PCINT23_EXISTS
typedef enum pcint2Pins_f {
#	if PCINT23_EXISTS
	PCINT23_PIN		= (1 << 7),
#	endif
#	if PCINT22_EXISTS
	PCINT22_PIN		= (1 << 6),
#	endif
#	if PCINT21_EXISTS
	PCINT21_PIN		= (1 << 5),
#	endif
#	if PCINT20_EXISTS
	PCINT20_PIN		= (1 << 4),
#	endif
#	if PCINT19_EXISTS
	PCINT19_PIN		= (1 << 3),
#	endif
#	if PCINT18_EXISTS
	PCINT18_PIN		= (1 << 2),
#	endif
#	if PCINT17_EXISTS
	PCINT17_PIN		= (1 << 1),
#	endif
#	if PCINT16_EXISTS
	PCINT16_PIN		= (1 << 0),
#	endif
} pcint2Pins_f;

inline __attribute__((always_inline)) pcint2Pins_f operator |(pcint2Pins_f a, pcint2Pins_f b)
{
	return static_cast<pcint2Pins_f>(static_cast<int>(a) | static_cast<int>(b));
}

inline __attribute__((always_inline)) pcint2Pins_f operator &(pcint2Pins_f a, pcint2Pins_f b)
{
	return static_cast<pcint2Pins_f>(static_cast<int>(a) & static_cast<int>(b));
}

inline __attribute__((always_inline)) pcint2Pins_f operator ^(pcint2Pins_f a, pcint2Pins_f b)
{
	return static_cast<pcint2Pins_f>(static_cast<int>(a) ^ static_cast<int>(b));
}
#endif

// =============================================================================
// INT - Class declaration
// =============================================================================

// -----------------------------------------------------------------------------
// Int0 ------------------------------------------------------------------------

#if INT0_EXISTS
class Int0 {
	// Constructors
public:
	Int0(void);

	// Methods - Public
public:
	bool config(portMode_e outputMode = PORT_INPUT_PULL_UP, intSenseMode_e senseMode = INT_SENSE_RISING_EDGE);
	bool activate(void);
	bool deactivate(void);
	bool clearRequest(void);
	error_e getLastError(void);

	// Data members - Private
private:
	error_e	lastError_;
}; // class Int0
#endif

// -----------------------------------------------------------------------------
// Int1 ------------------------------------------------------------------------

#if INT1_EXISTS
class Int1 {
	// Constructors
public:
	Int1(void);

	// Methods - Public
public:
	bool config(portMode_e outputMode = PORT_INPUT_PULL_UP, intSenseMode_e senseMode = INT_SENSE_RISING_EDGE);
	bool activate(void);
	bool deactivate(void);
	bool clearRequest(void);
	error_e getLastError(void);

	// Data members - Private
private:
	error_e	lastError_;

}; // class Int1
#endif

// -----------------------------------------------------------------------------
// Int2 ------------------------------------------------------------------------

#if INT2_EXISTS
class Int2 {
	// Constructors
public:
	Int2(void);

	// Methods - Public
public:
	bool config(portMode_e outputMode = PORT_INPUT_PULL_UP, intSenseMode_e senseMode = INT_SENSE_RISING_EDGE);
	bool activate(void);
	bool deactivate(void);
	bool clearRequest(void);
	error_e getLastError(void);

	// Data members - Private
private:
	error_e	lastError_;

}; // class Int2
#endif

// -----------------------------------------------------------------------------
// Int3 ------------------------------------------------------------------------

#if INT3_EXISTS
class Int3 {
	// Constructors
public:
	Int3(void);

	// Methods - Public
public:
	bool config(portMode_e outputMode = PORT_INPUT_PULL_UP, intSenseMode_e senseMode = INT_SENSE_RISING_EDGE);
	bool activate(void);
	bool deactivate(void);
	bool clearRequest(void);
	error_e getLastError(void);

	// Data members - Private
private:
	error_e	lastError_;

}; // class Int3
#endif

// -----------------------------------------------------------------------------
// Int4 ------------------------------------------------------------------------

#if INT4_EXISTS
class Int4 {
	// Constructors
public:
	Int4(void);

	// Methods - Public
public:
	bool config(portMode_e outputMode = PORT_INPUT_PULL_UP, intSenseMode_e senseMode = INT_SENSE_RISING_EDGE);
	bool activate(void);
	bool deactivate(void);
	bool clearRequest(void);
	error_e getLastError(void);

	// Data members - Private
private:
	error_e	lastError_;

}; // class Int4
#endif

// -----------------------------------------------------------------------------
// Int5 ------------------------------------------------------------------------

#if INT5_EXISTS
class Int5 {
	// Constructors
public:
	Int5(void);

	// Methods - Public
public:
	bool config(portMode_e outputMode = PORT_INPUT_PULL_UP, intSenseMode_e senseMode = INT_SENSE_RISING_EDGE);
	bool activate(void);
	bool deactivate(void);
	bool clearRequest(void);
	error_e getLastError(void);

	// Data members - Private
private:
}; // class Int5
#endif

// -----------------------------------------------------------------------------
// Int6 ------------------------------------------------------------------------

#if INT6_EXISTS
class Int6 {
	// Constructors
public:
	Int6(void);

	// Methods - Public
public:
	bool config(portMode_e outputMode = PORT_INPUT_PULL_UP, intSenseMode_e senseMode = INT_SENSE_RISING_EDGE);
	bool activate(void);
	bool deactivate(void);
	bool clearRequest(void);
	error_e getLastError(void);

	// Data members - Private
private:
	error_e	lastError_;

}; // class Int6
#endif

// -----------------------------------------------------------------------------
// Int7 ------------------------------------------------------------------------

#if INT7_EXISTS
class Int7 {
	// Constructors
public:
	Int7(void);

	// Methods - Public
public:
	bool config(portMode_e outputMode = PORT_INPUT_PULL_UP, intSenseMode_e senseMode = INT_SENSE_RISING_EDGE);
	bool activate(void);
	bool deactivate(void);
	bool clearRequest(void);
	error_e getLastError(void);

	// Data members - Private
private:
	error_e	lastError_;

}; // class Int7
#endif

// =============================================================================
// PCINT - Class declaration
// =============================================================================

// -----------------------------------------------------------------------------
// Pcint0 ----------------------------------------------------------------------

#if PCINT0_EXISTS  || PCINT1_EXISTS  || PCINT2_EXISTS  || PCINT3_EXISTS  || \
	PCINT4_EXISTS  || PCINT5_EXISTS  || PCINT6_EXISTS  || PCINT7_EXISTS
class Pcint0 {
	// Constructors
public:
	Pcint0(void);

	// Methods - Public
public:
	bool activate(void);
	bool deactivate(void);
	bool clearRequest(void);
	bool enablePins(pcint0Pins_f pinsList, portMode_e outputMode = PORT_INPUT_PULL_UP);
	bool disablePins(pcint0Pins_f pinsList);
	error_e getLastError(void);

	// Data members - Private
private:
	error_e	lastError_;

protected:

}; // class Pcint0
#endif

// -----------------------------------------------------------------------------
// Pcint1 ----------------------------------------------------------------------

#if PCINT8_EXISTS  || PCINT9_EXISTS  || PCINT10_EXISTS || PCINT11_EXISTS || \
	PCINT12_EXISTS || PCINT13_EXISTS || PCINT14_EXISTS || PCINT15_EXISTS
class Pcint1 {
	// Constructors
public:
	Pcint1(void);

	// Methods - Public
public:
	bool activate(void);
	bool deactivate(void);
	bool clearRequest(void);
	bool enablePins(pcint1Pins_f pinsList, portMode_e outputMode = PORT_INPUT_PULL_UP);
	bool disablePins(pcint1Pins_f pinsList);
	error_e getLastError(void);

	// Data members - Private
private:
	error_e	lastError_;

protected:

}; // class Pcint1
#endif

// -----------------------------------------------------------------------------
// Pcint2 ----------------------------------------------------------------------

#if PCINT16_EXISTS || PCINT17_EXISTS || PCINT18_EXISTS || PCINT19_EXISTS || \
	PCINT20_EXISTS || PCINT21_EXISTS || PCINT22_EXISTS || PCINT23_EXISTS
class Pcint2 {
	// Constructors
public:
	Pcint2(void);

	// Methods - Public
public:
	bool activate(void);
	bool deactivate(void);
	bool clearRequest(void);
	bool enablePins(pcint2Pins_f pinsList, portMode_e outputMode = PORT_INPUT_PULL_UP);
	bool disablePins(pcint2Pins_f pinsList);
	error_e getLastError(void);

	// Data members - Private
private:
	error_e	lastError_;

protected:

}; // class Pcint2
#endif

// =============================================================================
// Int - Class inline function definitions
// =============================================================================

// -----------------------------------------------------------------------------
// Int0 ------------------------------------------------------------------------

#if INT0_EXISTS
bool inline __attribute__((always_inline)) Int0::activate(void)
{
	setBit(INT0_MASK_REG, INT0_MASK_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}

bool inline __attribute__((always_inline)) Int0::deactivate(void)
{
	clrBit(INT0_MASK_REG, INT0_MASK_BIT);
	this->lastError_ = ERROR_INTERRUPT_NOT_AVAILABLE;
	return true;
}

bool inline __attribute__((always_inline)) Int0::clearRequest(void)
{
	setBit(INT0_INT_FLAG_REG, INT0_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
#endif

// -----------------------------------------------------------------------------
// Int1 ------------------------------------------------------------------------

#if INT1_EXISTS
bool inline __attribute__((always_inline)) Int1::activate(void)
{
	setBit(INT1_MASK_REG, INT1_MASK_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}

bool inline __attribute__((always_inline)) Int1::deactivate(void)
{
	clrBit(INT1_MASK_REG, INT1_MASK_BIT);
	this->lastError_ = ERROR_INTERRUPT_NOT_AVAILABLE;
	return true;
}

bool inline __attribute__((always_inline)) Int1::clearRequest(void)
{
	setBit(INT1_INT_FLAG_REG, INT1_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
#endif

// -----------------------------------------------------------------------------
// Int2 ------------------------------------------------------------------------

#if INT2_EXISTS
bool inline __attribute__((always_inline)) Int2::activate(void)
{
	setBit(INT2_MASK_REG, INT2_MASK_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}

bool inline __attribute__((always_inline)) Int2::deactivate(void)
{
	clrBit(INT2_MASK_REG, INT2_MASK_BIT);
	this->lastError_ = ERROR_INTERRUPT_NOT_AVAILABLE;
	return true;
}

bool inline __attribute__((always_inline)) Int2::clearRequest(void)
{
	setBit(INT2_INT_FLAG_REG, INT2_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
#endif

// -----------------------------------------------------------------------------
// Int3 ------------------------------------------------------------------------

#if INT3_EXISTS
bool inline __attribute__((always_inline)) Int3::activate(void)
{
	setBit(INT3_MASK_REG, INT3_MASK_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}

bool inline __attribute__((always_inline)) Int3::deactivate(void)
{
	clrBit(INT3_MASK_REG, INT3_MASK_BIT);
	this->lastError_ = ERROR_INTERRUPT_NOT_AVAILABLE;
	return true;
}

bool inline __attribute__((always_inline)) Int3::clearRequest(void)
{
	setBit(INT3_INT_FLAG_REG, INT3_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
#endif

// -----------------------------------------------------------------------------
// Int4 ------------------------------------------------------------------------

#if INT4_EXISTS
bool inline __attribute__((always_inline)) Int4::activate(void)
{
	setBit(INT4_MASK_REG, INT4_MASK_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}

bool inline __attribute__((always_inline)) Int4::deactivate(void)
{
	clrBit(INT4_MASK_REG, INT4_MASK_BIT);
	this->lastError_ = ERROR_INTERRUPT_NOT_AVAILABLE;
	return true;
}

bool inline __attribute__((always_inline)) Int4::clearRequest(void)
{
	setBit(INT4_INT_FLAG_REG, INT4_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
#endif

// -----------------------------------------------------------------------------
// Int5 ------------------------------------------------------------------------

#if INT5_EXISTS
bool inline __attribute__((always_inline)) Int5::activate(void)
{
	setBit(INT5_MASK_REG, INT5_MASK_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}

bool inline __attribute__((always_inline)) Int5::deactivate(void)
{
	clrBit(INT5_MASK_REG, INT5_MASK_BIT);
	this->lastError_ = ERROR_INTERRUPT_NOT_AVAILABLE;
	return true;
}

bool inline __attribute__((always_inline)) Int5::clearRequest(void)
{
	setBit(INT5_INT_FLAG_REG, INT5_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
#endif

// -----------------------------------------------------------------------------
// Int6 ------------------------------------------------------------------------

#if INT6_EXISTS
bool inline __attribute__((always_inline)) Int6::activate(void)
{
	setBit(INT6_MASK_REG, INT6_MASK_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}

bool inline __attribute__((always_inline)) Int6::deactivate(void)
{
	clrBit(INT6_MASK_REG, INT6_MASK_BIT);
	this->lastError_ = ERROR_INTERRUPT_NOT_AVAILABLE;
	return true;
}

bool inline __attribute__((always_inline)) Int6::clearRequest(void)
{
	setBit(INT6_INT_FLAG_REG, INT6_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
#endif

// -----------------------------------------------------------------------------
// Int7 ------------------------------------------------------------------------

#if INT7_EXISTS
bool inline __attribute__((always_inline)) Int7::activate(void)
{
	setBit(INT7_MASK_REG, INT7_MASK_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}

bool inline __attribute__((always_inline)) Int7::deactivate(void)
{
	clrBit(INT7_MASK_REG, INT7_MASK_BIT);
	this->lastError_ = ERROR_INTERRUPT_NOT_AVAILABLE;
	return true;
}

bool inline __attribute__((always_inline)) Int7::clearRequest(void)
{
	setBit(INT7_INT_FLAG_REG, INT7_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
#endif

// =============================================================================
// Pcint - Class inline function definitions
// =============================================================================

// -----------------------------------------------------------------------------
// Pcint0 ----------------------------------------------------------------------

#if PCINT0_EXISTS  || PCINT1_EXISTS  || PCINT2_EXISTS  || PCINT3_EXISTS  || \
	PCINT4_EXISTS  || PCINT5_EXISTS  || PCINT6_EXISTS  || PCINT7_EXISTS
bool inline __attribute__((always_inline)) Pcint0::activate(void)
{
	setBit(PCINT0_CONTROL_REG, PCINT0_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}

bool inline __attribute__((always_inline)) Pcint0::deactivate(void)
{
	clrBit(PCINT0_CONTROL_REG, PCINT0_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}

bool inline __attribute__((always_inline)) Pcint0::clearRequest(void)
{
	setBit(PCINT0_INT_FLAG_REG, PCINT0_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
#endif

// -----------------------------------------------------------------------------
// Pcint1 ----------------------------------------------------------------------

#if PCINT8_EXISTS  || PCINT9_EXISTS  || PCINT10_EXISTS || PCINT11_EXISTS || \
	PCINT12_EXISTS || PCINT13_EXISTS || PCINT14_EXISTS || PCINT15_EXISTS
bool inline __attribute__((always_inline)) Pcint1::activate(void)
{
	setBit(PCINT1_CONTROL_REG, PCINT1_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}

bool inline __attribute__((always_inline)) Pcint1::deactivate(void)
{
	clrBit(PCINT1_CONTROL_REG, PCINT1_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}

bool inline __attribute__((always_inline)) Pcint1::clearRequest(void)
{
	setBit(PCINT1_INT_FLAG_REG, PCINT1_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
#endif

// -----------------------------------------------------------------------------
// Pcint2 ----------------------------------------------------------------------

#if PCINT16_EXISTS || PCINT17_EXISTS || PCINT18_EXISTS || PCINT19_EXISTS || \
	PCINT20_EXISTS || PCINT21_EXISTS || PCINT22_EXISTS || PCINT23_EXISTS
bool inline __attribute__((always_inline)) Pcint2::activate(void)
{
	setBit(PCINT2_CONTROL_REG, PCINT2_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}

bool inline __attribute__((always_inline)) Pcint2::deactivate(void)
{
	clrBit(PCINT2_CONTROL_REG, PCINT2_INT_ENABLE_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}

bool inline __attribute__((always_inline)) Pcint2::clearRequest(void)
{
	setBit(PCINT2_INT_FLAG_REG, PCINT2_INT_FLAG_BIT);
	this->lastError_ = ERROR_NONE;
	return true;
}
#endif

// =============================================================================
// Extern global variables
// *INDENT-OFF*
// =============================================================================

#if INT0_EXISTS
	extern Int0 int0;
#endif

#if INT1_EXISTS
	extern Int1 int1;
#endif

#if INT2_EXISTS
	extern Int2 int2;
#endif

#if INT3_EXISTS
	extern Int3 int3;
#endif

#if INT4_EXISTS
	extern Int4 int4;
#endif

#if INT5_EXISTS
	extern Int5 int5;
#endif

#if INT6_EXISTS
	extern Int6 int6;
#endif

#if INT7_EXISTS
	extern Int7 int7;
#endif

#if PCINT0_EXISTS  || PCINT1_EXISTS  || PCINT2_EXISTS  || PCINT3_EXISTS  || \
	PCINT4_EXISTS  || PCINT5_EXISTS  || PCINT6_EXISTS  || PCINT7_EXISTS
	extern Pcint0 pcint0;
#endif

#if PCINT8_EXISTS  || PCINT9_EXISTS  || PCINT10_EXISTS || PCINT11_EXISTS || \
	PCINT12_EXISTS || PCINT13_EXISTS || PCINT14_EXISTS || PCINT15_EXISTS
	extern Pcint1 pcint1;
#endif

#if PCINT16_EXISTS || PCINT17_EXISTS || PCINT18_EXISTS || PCINT19_EXISTS || \
	PCINT20_EXISTS || PCINT21_EXISTS || PCINT22_EXISTS || PCINT23_EXISTS
	extern Pcint2 pcint2;
#endif
// *INDENT-ON*

// =============================================================================
// General public functions declarations
// =============================================================================

/* NONE */

// =============================================================================
// General inline functions definitions
// =============================================================================

/* NONE */

#endif // __EXTERNAL_INTERRUPT_HPP
