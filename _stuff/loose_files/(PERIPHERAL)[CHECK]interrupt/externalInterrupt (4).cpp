/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	lInterrupt.cpp
 * Module:		l Interrupt Module Interface for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1002
 * Date:		July 04, 2018
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "externalInterrupt.hpp"
#if __EXTERNAL_INTERRUPT_HPP != 1002
#	error [externalInterrupt.cpp] Error 102 - Build mismatch on header and source code files.
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

/* NONE */

// =============================================================================
// File exclusive - New data types
// =============================================================================

/* NONE */

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

/* NONE */

// =============================================================================
// Global variables
// *INDENT-OFF*
// =============================================================================

#if INT0_EXISTS
	Int0 int0;
#endif
#if INT1_EXISTS
	Int1 int1;
#endif
#if INT2_EXISTS
	Int2 int2;
#endif
#if INT3_EXISTS
	Int3 int3;
#endif
#if INT4_EXISTS
	Int4 int4;
#endif
#if INT5_EXISTS
	Int5 int5;
#endif
#if INT6_EXISTS
	Int6 int6;
#endif
#if INT7_EXISTS
	Int7 int7;
#endif

#if PCINT0_EXISTS  || PCINT1_EXISTS  || PCINT2_EXISTS  || PCINT3_EXISTS  || \
	PCINT4_EXISTS  || PCINT5_EXISTS  || PCINT6_EXISTS  || PCINT7_EXISTS
	Pcint0 pcint0;
#endif
#if PCINT8_EXISTS  || PCINT9_EXISTS  || PCINT10_EXISTS || PCINT11_EXISTS || \
	PCINT12_EXISTS || PCINT13_EXISTS || PCINT14_EXISTS || PCINT15_EXISTS
	Pcint1 pcint1;
#endif
#if	 PCINT16_EXISTS || PCINT17_EXISTS || PCINT18_EXISTS || PCINT19_EXISTS || \
	PCINT20_EXISTS || PCINT21_EXISTS || PCINT22_EXISTS || PCINT23_EXISTS
	Pcint2 pcint2;
#endif
// *INDENT-ON*

// =============================================================================
// Class constructors
// =============================================================================

// -----------------------------------------------------------------------------
// INT0 ------------------------------------------------------------------------

#if INT0_EXISTS
Int0::Int0(void)
{
	this->lastError_ = ERROR_NONE;
}
#endif

// -----------------------------------------------------------------------------
// INT1 ------------------------------------------------------------------------

#if INT1_EXISTS
Int1::Int1(void)
{
	this->lastError_ = ERROR_NONE;
}
#endif

// -----------------------------------------------------------------------------
// INT2 ------------------------------------------------------------------------

#if INT2_EXISTS
Int2::Int2(void)
{
	this->lastError_ = ERROR_NONE;
}
#endif

// -----------------------------------------------------------------------------
// INT3 ------------------------------------------------------------------------

#if INT3_EXISTS
Int3::Int3(void)
{
	this->lastError_ = ERROR_NONE;
}
#endif

// -----------------------------------------------------------------------------
// INT4 ------------------------------------------------------------------------

#if INT4_EXISTS
Int4::Int4(void)
{
	this->lastError_ = ERROR_NONE;
}
#endif

// -----------------------------------------------------------------------------
// INT5 ------------------------------------------------------------------------

#if INT5_EXISTS
Int5::Int5(void)
{
	this->lastError_ = ERROR_NONE;
}
#endif

// -----------------------------------------------------------------------------
// INT6 ------------------------------------------------------------------------

#if INT6_EXISTS
Int6::Int6(void)
{
	this->lastError_ = ERROR_NONE;
}
#endif

// -----------------------------------------------------------------------------
// INT7 ------------------------------------------------------------------------

#if INT7_EXISTS
Int7::Int7(void)
{
	this->lastError_ = ERROR_NONE;
}
#endif

// -----------------------------------------------------------------------------
// PCINT0 ----------------------------------------------------------------------

#if PCINT0_EXISTS  || PCINT1_EXISTS  || PCINT2_EXISTS  || PCINT3_EXISTS  || \
	PCINT4_EXISTS  || PCINT5_EXISTS  || PCINT6_EXISTS  || PCINT7_EXISTS
Pcint0::Pcint0(void)
{
	this->lastError_ = ERROR_NONE;
}
#endif

// -----------------------------------------------------------------------------
// PCINT1 ----------------------------------------------------------------------

#if PCINT8_EXISTS  || PCINT9_EXISTS  || PCINT10_EXISTS || PCINT11_EXISTS || \
	PCINT12_EXISTS || PCINT13_EXISTS || PCINT14_EXISTS || PCINT15_EXISTS
Pcint1::Pcint1(void)
{
	this->lastError_ = ERROR_NONE;
}
#endif

// -----------------------------------------------------------------------------
// PCINT2 ----------------------------------------------------------------------

#if PCINT16_EXISTS || PCINT17_EXISTS || PCINT18_EXISTS || PCINT19_EXISTS || \
	PCINT20_EXISTS || PCINT21_EXISTS || PCINT22_EXISTS || PCINT23_EXISTS
Pcint2::Pcint2(void)
{
	this->lastError_ = ERROR_NONE;
}
#endif

// =============================================================================
// Class public methods
// =============================================================================

// -----------------------------------------------------------------------------
// INT0 ------------------------------------------------------------------------

#if INT0_EXISTS
bool Int0::config(portMode_e outputMode, intSenseMode_e senseMode)
{
	uint8_t reg = INT0_SENSE_REG;

	switch (outputMode) {
	case PORT_OUTPUT_LOW:
		clrBit(INT0_PORT_OUT, INT0_PORT_BIT);
		setBit(INT0_PORT_DIR, INT0_PORT_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(INT0_PORT_OUT, INT0_PORT_BIT);
		setBit(INT0_PORT_DIR, INT0_PORT_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(INT0_PORT_OUT, INT0_PORT_BIT);
		clrBit(INT0_PORT_DIR, INT0_PORT_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(INT0_PORT_OUT, INT0_PORT_BIT);
		clrBit(INT0_PORT_DIR, INT0_PORT_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		this->lastError_ = ERROR_PORT_OUTPUT_MODE_UNSUPPORTED;
		return false;
	}

	if (senseMode != INT_SENSE_NO_CHANGE) {
		clrMask(reg, 0x03, INT0_SENSE_BIT_0);
		switch (senseMode) {
		case INT_SENSE_LOW_LEVEL:
			break;
		case INT_SENSE_ANY_EDGE:
			setBit(reg, INT0_SENSE_BIT_0);
			break;
		case INT_SENSE_FALLING_EDGE:
			setBit(reg, INT0_SENSE_BIT_1);
			break;
		case INT_SENSE_RISING_EDGE:
			setBit(reg, INT0_SENSE_BIT_1);
			setBit(reg, INT0_SENSE_BIT_0);
			break;
		default:
			this->lastError_ = ERROR_SENSE_MODE_UNSUPPORTED;
			return false;
		}
		INT0_SENSE_REG = reg;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

error_e Int0::getLastError(void)
{
	return this->lastError_;
}
#endif

// -----------------------------------------------------------------------------
// INT1 ------------------------------------------------------------------------

#if INT1_EXISTS
bool Int1::config(portMode_e outputMode, intSenseMode_e senseMode)
{
	uint8_t reg = INT1_SENSE_REG;

	switch (outputMode) {
	case PORT_OUTPUT_LOW:
		clrBit(INT1_PORT_OUT, INT1_PORT_BIT);
		setBit(INT1_PORT_DIR, INT1_PORT_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(INT1_PORT_OUT, INT1_PORT_BIT);
		setBit(INT1_PORT_DIR, INT1_PORT_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(INT1_PORT_OUT, INT1_PORT_BIT);
		clrBit(INT1_PORT_DIR, INT1_PORT_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(INT1_PORT_OUT, INT1_PORT_BIT);
		clrBit(INT1_PORT_DIR, INT1_PORT_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		this->lastError_ = ERROR_PORT_OUTPUT_MODE_UNSUPPORTED;
		return false;
	}

	if (senseMode != INT_SENSE_NO_CHANGE) {
		clrMask(reg, 0x03, INT1_SENSE_BIT_0);
		switch (senseMode) {
		case INT_SENSE_LOW_LEVEL:
			break;
		case INT_SENSE_ANY_EDGE:
			setBit(reg, INT1_SENSE_BIT_0);
			break;
		case INT_SENSE_FALLING_EDGE:
			setBit(reg, INT1_SENSE_BIT_1);
			break;
		case INT_SENSE_RISING_EDGE:
			setBit(reg, INT1_SENSE_BIT_1);
			setBit(reg, INT1_SENSE_BIT_0);
			break;
		default:
			this->lastError_ = ERROR_SENSE_MODE_UNSUPPORTED;
			return false;
		}
		INT1_SENSE_REG = reg;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

error_e Int1::getLastError(void)
{
	return this->lastError_;
}
#endif

// -----------------------------------------------------------------------------
// INT2 ------------------------------------------------------------------------

#if INT2_EXISTS
bool Int2::config(portMode_e outputMode, intSenseMode_e senseMode)
{
	uint8_t reg = INT2_SENSE_REG;

	switch (outputMode) {
	case PORT_OUTPUT_LOW:
		clrBit(INT2_PORT_OUT, INT2_PORT_BIT);
		setBit(INT2_PORT_DIR, INT2_PORT_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(INT2_PORT_OUT, INT2_PORT_BIT);
		setBit(INT2_PORT_DIR, INT2_PORT_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(INT2_PORT_OUT, INT2_PORT_BIT);
		clrBit(INT2_PORT_DIR, INT2_PORT_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(INT2_PORT_OUT, INT2_PORT_BIT);
		clrBit(INT2_PORT_DIR, INT2_PORT_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		this->lastError_ = ERROR_PORT_OUTPUT_MODE_UNSUPPORTED;
		return false;
	}

	if (senseMode != INT_SENSE_NO_CHANGE) {
		clrMask(reg, 0x03, INT2_SENSE_BIT_0);
		switch (senseMode) {
		case INT_SENSE_LOW_LEVEL:
			break;
		case INT_SENSE_ANY_EDGE:
			setBit(reg, INT2_SENSE_BIT_0);
			break;
		case INT_SENSE_FALLING_EDGE:
			setBit(reg, INT2_SENSE_BIT_1);
			break;
		case INT_SENSE_RISING_EDGE:
			setBit(reg, INT2_SENSE_BIT_1);
			setBit(reg, INT2_SENSE_BIT_0);
			break;
		default:
			this->lastError_ = ERROR_SENSE_MODE_UNSUPPORTED;
			return false;
		}
		INT2_SENSE_REG = reg;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

error_e Int2::getLastError(void)
{
	return this->lastError_;
}
#endif

// -----------------------------------------------------------------------------
// INT3 ------------------------------------------------------------------------

#if INT3_EXISTS
bool Int3::config(portMode_e outputMode, intSenseMode_e senseMode)
{
	uint8_t reg = INT3_SENSE_REG;

	switch (outputMode) {
	case PORT_OUTPUT_LOW:
		clrBit(INT3_PORT_OUT, INT3_PORT_BIT);
		setBit(INT3_PORT_DIR, INT3_PORT_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(INT3_PORT_OUT, INT3_PORT_BIT);
		setBit(INT3_PORT_DIR, INT3_PORT_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(INT3_PORT_OUT, INT3_PORT_BIT);
		clrBit(INT3_PORT_DIR, INT3_PORT_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(INT3_PORT_OUT, INT3_PORT_BIT);
		clrBit(INT3_PORT_DIR, INT3_PORT_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		this->lastError_ = ERROR_PORT_OUTPUT_MODE_UNSUPPORTED;
		return false;
	}

	if (senseMode != INT_SENSE_NO_CHANGE) {
		clrMask(reg, 0x03, INT3_SENSE_BIT_0);
		switch (senseMode) {
		case INT_SENSE_LOW_LEVEL:
			break;
		case INT_SENSE_ANY_EDGE:
			setBit(reg, INT3_SENSE_BIT_0);
			break;
		case INT_SENSE_FALLING_EDGE:
			setBit(reg, INT3_SENSE_BIT_1);
			break;
		case INT_SENSE_RISING_EDGE:
			setBit(reg, INT3_SENSE_BIT_1);
			setBit(reg, INT3_SENSE_BIT_0);
			break;
		default:
			this->lastError_ = ERROR_SENSE_MODE_UNSUPPORTED;
			return false;
		}
		INT3_SENSE_REG = reg;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

error_e Int3::getLastError(void)
{
	return this->lastError_;
}
#endif

// -----------------------------------------------------------------------------
// INT4 ------------------------------------------------------------------------

#if INT4_EXISTS
bool Int4::config(portMode_e outputMode, intSenseMode_e senseMode)
{
	uint8_t reg = INT4_SENSE_REG;

	switch (outputMode) {
	case PORT_OUTPUT_LOW:
		clrBit(INT4_PORT_OUT, INT4_PORT_BIT);
		setBit(INT4_PORT_DIR, INT4_PORT_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(INT4_PORT_OUT, INT4_PORT_BIT);
		setBit(INT4_PORT_DIR, INT4_PORT_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(INT4_PORT_OUT, INT4_PORT_BIT);
		clrBit(INT4_PORT_DIR, INT4_PORT_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(INT4_PORT_OUT, INT4_PORT_BIT);
		clrBit(INT4_PORT_DIR, INT4_PORT_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		this->lastError_ = ERROR_PORT_OUTPUT_MODE_UNSUPPORTED;
		return false;
	}

	if (senseMode != INT_SENSE_NO_CHANGE) {
		clrMask(reg, 0x03, INT4_SENSE_BIT_0);
		switch (senseMode) {
		case INT_SENSE_LOW_LEVEL:
			break;
		case INT_SENSE_ANY_EDGE:
			setBit(reg, INT4_SENSE_BIT_0);
			break;
		case INT_SENSE_FALLING_EDGE:
			setBit(reg, INT4_SENSE_BIT_1);
			break;
		case INT_SENSE_RISING_EDGE:
			setBit(reg, INT4_SENSE_BIT_1);
			setBit(reg, INT4_SENSE_BIT_0);
			break;
		default:
			this->lastError_ = ERROR_SENSE_MODE_UNSUPPORTED;
			return false;
		}
		INT4_SENSE_REG = reg;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

error_e Int4::getLastError(void)
{
	return this->lastError_;
}
#endif

// -----------------------------------------------------------------------------
// INT5 ------------------------------------------------------------------------

#if INT5_EXISTS
bool Int5::config(portMode_e outputMode, intSenseMode_e senseMode)
{
	uint8_t reg = INT5_SENSE_REG;

	switch (outputMode) {
	case PORT_OUTPUT_LOW:
		clrBit(INT5_PORT_OUT, INT5_PORT_BIT);
		setBit(INT5_PORT_DIR, INT5_PORT_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(INT5_PORT_OUT, INT5_PORT_BIT);
		setBit(INT5_PORT_DIR, INT5_PORT_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(INT5_PORT_OUT, INT5_PORT_BIT);
		clrBit(INT5_PORT_DIR, INT5_PORT_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(INT5_PORT_OUT, INT5_PORT_BIT);
		clrBit(INT5_PORT_DIR, INT5_PORT_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		this->lastError_ = ERROR_PORT_OUTPUT_MODE_UNSUPPORTED;
		return false;
	}

	if (senseMode != INT_SENSE_NO_CHANGE) {
		clrMask(reg, 0x03, INT5_SENSE_BIT_0);
		switch (senseMode) {
		case INT_SENSE_LOW_LEVEL:
			break;
		case INT_SENSE_ANY_EDGE:
			setBit(reg, INT5_SENSE_BIT_0);
			break;
		case INT_SENSE_FALLING_EDGE:
			setBit(reg, INT5_SENSE_BIT_1);
			break;
		case INT_SENSE_RISING_EDGE:
			setBit(reg, INT5_SENSE_BIT_1);
			setBit(reg, INT5_SENSE_BIT_0);
			break;
		default:
			this->lastError_ = ERROR_SENSE_MODE_UNSUPPORTED;
			return false;
		}
		INT5_SENSE_REG = reg;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

error_e Int5::getLastError(void)
{
	return this->lastError_;
}
#endif

// -----------------------------------------------------------------------------
// INT6 ------------------------------------------------------------------------

#if INT6_EXISTS
bool Int6::config(portMode_e outputMode, intSenseMode_e senseMode)
{
	uint8_t reg = INT6_SENSE_REG;

	switch (outputMode) {
	case PORT_OUTPUT_LOW:
		clrBit(INT6_PORT_OUT, INT6_PORT_BIT);
		setBit(INT6_PORT_DIR, INT6_PORT_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(INT6_PORT_OUT, INT6_PORT_BIT);
		setBit(INT6_PORT_DIR, INT6_PORT_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(INT6_PORT_OUT, INT6_PORT_BIT);
		clrBit(INT6_PORT_DIR, INT6_PORT_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(INT6_PORT_OUT, INT6_PORT_BIT);
		clrBit(INT6_PORT_DIR, INT6_PORT_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		this->lastError_ = ERROR_PORT_OUTPUT_MODE_UNSUPPORTED;
		return false;
	}

	if (senseMode != INT_SENSE_NO_CHANGE) {
		clrMask(reg, 0x03, INT6_SENSE_BIT_0);
		switch (senseMode) {
		case INT_SENSE_LOW_LEVEL:
			break;
		case INT_SENSE_ANY_EDGE:
			setBit(reg, INT6_SENSE_BIT_0);
			break;
		case INT_SENSE_FALLING_EDGE:
			setBit(reg, INT6_SENSE_BIT_1);
			break;
		case INT_SENSE_RISING_EDGE:
			setBit(reg, INT6_SENSE_BIT_1);
			setBit(reg, INT6_SENSE_BIT_0);
			break;
		default:
			this->lastError_ = ERROR_SENSE_MODE_UNSUPPORTED;
			return false;
		}
		INT6_SENSE_REG = reg;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

error_e Int6::getLastError(void)
{
	return this->lastError_;
}
#endif

// -----------------------------------------------------------------------------
// INT7 ------------------------------------------------------------------------

#if INT7_EXISTS
bool Int7::config(portMode_e outputMode, intSenseMode_e senseMode)
{
	uint8_t reg = INT7_SENSE_REG;

	switch (outputMode) {
	case PORT_OUTPUT_LOW:
		clrBit(INT7_PORT_OUT, INT7_PORT_BIT);
		setBit(INT7_PORT_DIR, INT7_PORT_BIT);
		break;
	case PORT_OUTPUT_HIGH:
		setBit(INT7_PORT_OUT, INT7_PORT_BIT);
		setBit(INT7_PORT_DIR, INT7_PORT_BIT);
		break;
	case PORT_INPUT_TRISTATE:
		clrBit(INT7_PORT_OUT, INT7_PORT_BIT);
		clrBit(INT7_PORT_DIR, INT7_PORT_BIT);
		break;
	case PORT_INPUT_PULL_UP:
		setBit(INT7_PORT_OUT, INT7_PORT_BIT);
		clrBit(INT7_PORT_DIR, INT7_PORT_BIT);
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		this->lastError_ = ERROR_PORT_OUTPUT_MODE_UNSUPPORTED;
		return false;
	}

	if (senseMode != INT_SENSE_NO_CHANGE) {
		clrMask(reg, 0x03, INT7_SENSE_BIT_0);
		switch (senseMode) {
		case INT_SENSE_LOW_LEVEL:
			break;
		case INT_SENSE_ANY_EDGE:
			setBit(reg, INT7_SENSE_BIT_0);
			break;
		case INT_SENSE_FALLING_EDGE:
			setBit(reg, INT7_SENSE_BIT_1);
			break;
		case INT_SENSE_RISING_EDGE:
			setBit(reg, INT7_SENSE_BIT_1);
			setBit(reg, INT7_SENSE_BIT_0);
			break;
		default:
			this->lastError_ = ERROR_SENSE_MODE_UNSUPPORTED;
			return false;
		}
		INT7_SENSE_REG = reg;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

error_e Int7::getLastError(void)
{
	return this->lastError_;
}
#endif

// -----------------------------------------------------------------------------
// PCINT0 ----------------------------------------------------------------------

#if PCINT0_EXISTS || PCINT1_EXISTS || PCINT2_EXISTS || PCINT3_EXISTS || \
	PCINT4_EXISTS || PCINT5_EXISTS || PCINT6_EXISTS || PCINT7_EXISTS
bool Pcint0::enablePins(pcint0Pins_f pinsList, portMode_e outputMode)
{
	switch (outputMode) {
	case PORT_OUTPUT_LOW:
#if		PCINT7_EXISTS
		if (pinsList & PCINT7_PIN) {
			clrBit(PCINT7_PORT_OUT, PCINT7_PORT_BIT);
			setBit(PCINT7_PORT_DIR, PCINT7_PORT_BIT);
		}
#endif
#if		PCINT6_EXISTS
		if (pinsList & PCINT6_PIN) {
			clrBit(PCINT6_PORT_OUT, PCINT6_PORT_BIT);
			setBit(PCINT6_PORT_DIR, PCINT6_PORT_BIT);
		}
#endif
#if		PCINT5_EXISTS
		if (pinsList & PCINT5_PIN) {
			clrBit(PCINT5_PORT_OUT, PCINT5_PORT_BIT);
			setBit(PCINT5_PORT_DIR, PCINT5_PORT_BIT);
		}
#endif
#if		PCINT4_EXISTS
		if (pinsList & PCINT4_PIN) {
			clrBit(PCINT4_PORT_OUT, PCINT4_PORT_BIT);
			setBit(PCINT4_PORT_DIR, PCINT4_PORT_BIT);
		}
#endif
#if		PCINT3_EXISTS
		if (pinsList & PCINT3_PIN) {
			clrBit(PCINT3_PORT_OUT, PCINT3_PORT_BIT);
			setBit(PCINT3_PORT_DIR, PCINT3_PORT_BIT);
		}
#endif
#if		PCINT2_EXISTS
		if (pinsList & PCINT2_PIN) {
			clrBit(PCINT2_PORT_OUT, PCINT2_PORT_BIT);
			setBit(PCINT2_PORT_DIR, PCINT2_PORT_BIT);
		}
#endif
#if		PCINT1_EXISTS
		if (pinsList & PCINT1_PIN) {
			clrBit(PCINT1_PORT_OUT, PCINT1_PORT_BIT);
			setBit(PCINT1_PORT_DIR, PCINT1_PORT_BIT);
		}
#endif
#if		PCINT0_EXISTS
		if (pinsList & PCINT0_PIN) {
			clrBit(PCINT0_PORT_OUT, PCINT0_PORT_BIT);
			setBit(PCINT0_PORT_DIR, PCINT0_PORT_BIT);
		}
#endif
		break;
	case PORT_OUTPUT_HIGH:
#if		PCINT7_EXISTS
		if (pinsList & PCINT7_PIN) {
			setBit(PCINT7_PORT_OUT, PCINT7_PORT_BIT);
			setBit(PCINT7_PORT_DIR, PCINT7_PORT_BIT);
		}
#endif
#if		PCINT6_EXISTS
		if (pinsList & PCINT6_PIN) {
			setBit(PCINT6_PORT_OUT, PCINT6_PORT_BIT);
			setBit(PCINT6_PORT_DIR, PCINT6_PORT_BIT);
		}
#endif
#if		PCINT5_EXISTS
		if (pinsList & PCINT5_PIN) {
			setBit(PCINT5_PORT_OUT, PCINT5_PORT_BIT);
			setBit(PCINT5_PORT_DIR, PCINT5_PORT_BIT);
		}
#endif
#if		PCINT4_EXISTS
		if (pinsList & PCINT4_PIN) {
			setBit(PCINT4_PORT_OUT, PCINT4_PORT_BIT);
			setBit(PCINT4_PORT_DIR, PCINT4_PORT_BIT);
		}
#endif
#if		PCINT3_EXISTS
		if (pinsList & PCINT3_PIN) {
			setBit(PCINT3_PORT_OUT, PCINT3_PORT_BIT);
			setBit(PCINT3_PORT_DIR, PCINT3_PORT_BIT);
		}
#endif
#if		PCINT2_EXISTS
		if (pinsList & PCINT2_PIN) {
			setBit(PCINT2_PORT_OUT, PCINT2_PORT_BIT);
			setBit(PCINT2_PORT_DIR, PCINT2_PORT_BIT);
		}
#endif
#if		PCINT1_EXISTS
		if (pinsList & PCINT1_PIN) {
			setBit(PCINT1_PORT_OUT, PCINT1_PORT_BIT);
			setBit(PCINT1_PORT_DIR, PCINT1_PORT_BIT);
		}
#endif
#if		PCINT0_EXISTS
		if (pinsList & PCINT0_PIN) {
			setBit(PCINT0_PORT_OUT, PCINT0_PORT_BIT);
			setBit(PCINT0_PORT_DIR, PCINT0_PORT_BIT);
		}
#endif
		break;
	case PORT_INPUT_TRISTATE:
#if		PCINT7_EXISTS
		if (pinsList & PCINT7_PIN) {
			clrBit(PCINT7_PORT_OUT, PCINT7_PORT_BIT);
			clrBit(PCINT7_PORT_DIR, PCINT7_PORT_BIT);
		}
#endif
#if		PCINT6_EXISTS
		if (pinsList & PCINT6_PIN) {
			clrBit(PCINT6_PORT_OUT, PCINT6_PORT_BIT);
			clrBit(PCINT6_PORT_DIR, PCINT6_PORT_BIT);
		}
#endif
#if		PCINT5_EXISTS
		if (pinsList & PCINT5_PIN) {
			clrBit(PCINT5_PORT_OUT, PCINT5_PORT_BIT);
			clrBit(PCINT5_PORT_DIR, PCINT5_PORT_BIT);
		}
#endif
#if		PCINT4_EXISTS
		if (pinsList & PCINT4_PIN) {
			clrBit(PCINT4_PORT_OUT, PCINT4_PORT_BIT);
			clrBit(PCINT4_PORT_DIR, PCINT4_PORT_BIT);
		}
#endif
#if		PCINT3_EXISTS
		if (pinsList & PCINT3_PIN) {
			clrBit(PCINT3_PORT_OUT, PCINT3_PORT_BIT);
			clrBit(PCINT3_PORT_DIR, PCINT3_PORT_BIT);
		}
#endif
#if		PCINT2_EXISTS
		if (pinsList & PCINT2_PIN) {
			clrBit(PCINT2_PORT_OUT, PCINT2_PORT_BIT);
			clrBit(PCINT2_PORT_DIR, PCINT2_PORT_BIT);
		}
#endif
#if		PCINT1_EXISTS
		if (pinsList & PCINT1_PIN) {
			clrBit(PCINT1_PORT_OUT, PCINT1_PORT_BIT);
			clrBit(PCINT1_PORT_DIR, PCINT1_PORT_BIT);
		}
#endif
#if		PCINT0_EXISTS
		if (pinsList & PCINT0_PIN) {
			clrBit(PCINT0_PORT_OUT, PCINT0_PORT_BIT);
			clrBit(PCINT0_PORT_DIR, PCINT0_PORT_BIT);
		}
#endif
		break;
	case PORT_INPUT_PULL_UP:
#if		PCINT7_EXISTS
		if (pinsList & PCINT7_PIN) {
			setBit(PCINT7_PORT_OUT, PCINT7_PORT_BIT);
			clrBit(PCINT7_PORT_DIR, PCINT7_PORT_BIT);
		}
#endif
#if		PCINT6_EXISTS
		if (pinsList & PCINT6_PIN) {
			setBit(PCINT6_PORT_OUT, PCINT6_PORT_BIT);
			clrBit(PCINT6_PORT_DIR, PCINT6_PORT_BIT);
		}
#endif
#if		PCINT5_EXISTS
		if (pinsList & PCINT5_PIN) {
			setBit(PCINT5_PORT_OUT, PCINT5_PORT_BIT);
			clrBit(PCINT5_PORT_DIR, PCINT5_PORT_BIT);
		}
#endif
#if		PCINT4_EXISTS
		if (pinsList & PCINT4_PIN) {
			setBit(PCINT4_PORT_OUT, PCINT4_PORT_BIT);
			clrBit(PCINT4_PORT_DIR, PCINT4_PORT_BIT);
		}
#endif
#if		PCINT3_EXISTS
		if (pinsList & PCINT3_PIN) {
			setBit(PCINT3_PORT_OUT, PCINT3_PORT_BIT);
			clrBit(PCINT3_PORT_DIR, PCINT3_PORT_BIT);
		}
#endif
#if		PCINT2_EXISTS
		if (pinsList & PCINT2_PIN) {
			setBit(PCINT2_PORT_OUT, PCINT2_PORT_BIT);
			clrBit(PCINT2_PORT_DIR, PCINT2_PORT_BIT);
		}
#endif
#if		PCINT1_EXISTS
		if (pinsList & PCINT1_PIN) {
			setBit(PCINT1_PORT_OUT, PCINT1_PORT_BIT);
			clrBit(PCINT1_PORT_DIR, PCINT1_PORT_BIT);
		}
#endif
#if		PCINT0_EXISTS
		if (pinsList & PCINT0_PIN) {
			setBit(PCINT0_PORT_OUT, PCINT0_PORT_BIT);
			clrBit(PCINT0_PORT_DIR, PCINT0_PORT_BIT);
		}
#endif
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		this->lastError_ = ERROR_PORT_OUTPUT_MODE_UNSUPPORTED;
		return 	false;
	}

#if		PCINT7_EXISTS
	if (pinsList & PCINT7_PIN) {
		setBit(PCINT0_MASK_REG, PCINT0_MASK_7_BIT);
	}
#endif
#if		PCINT6_EXISTS
	if (pinsList & PCINT6_PIN) {
		setBit(PCINT0_MASK_REG, PCINT0_MASK_6_BIT);
	}
#endif
#if		PCINT5_EXISTS
	if (pinsList & PCINT5_PIN) {
		setBit(PCINT0_MASK_REG, PCINT0_MASK_5_BIT);
	}
#endif
#if		PCINT4_EXISTS
	if (pinsList & PCINT4_PIN) {
		setBit(PCINT0_MASK_REG, PCINT0_MASK_4_BIT);
	}
#endif
#if		PCINT3_EXISTS
	if (pinsList & PCINT3_PIN) {
		setBit(PCINT0_MASK_REG, PCINT0_MASK_3_BIT);
	}
#endif
#if		PCINT2_EXISTS
	if (pinsList & PCINT2_PIN) {
		setBit(PCINT0_MASK_REG, PCINT0_MASK_2_BIT);
	}
#endif
#if		PCINT1_EXISTS
	if (pinsList & PCINT1_PIN) {
		setBit(PCINT0_MASK_REG, PCINT0_MASK_1_BIT);
	}
#endif
#if		PCINT0_EXISTS
	if (pinsList & PCINT0_PIN) {
		setBit(PCINT0_MASK_REG, PCINT0_MASK_0_BIT);
	}
#endif

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Pcint0::disablePins(pcint0Pins_f pinsList)
{
#if		PCINT7_EXISTS
	if (pinsList & PCINT7_PIN) {
		clrBit(PCINT0_MASK_REG, PCINT0_MASK_7_BIT);
	}
#endif
#if		PCINT6_EXISTS
	if (pinsList & PCINT6_PIN) {
		clrBit(PCINT0_MASK_REG, PCINT0_MASK_6_BIT);
	}
#endif
#if		PCINT5_EXISTS
	if (pinsList & PCINT5_PIN) {
		clrBit(PCINT0_MASK_REG, PCINT0_MASK_5_BIT);
	}
#endif
#if		PCINT4_EXISTS
	if (pinsList & PCINT4_PIN) {
		clrBit(PCINT0_MASK_REG, PCINT0_MASK_4_BIT);
	}
#endif
#if		PCINT3_EXISTS
	if (pinsList & PCINT3_PIN) {
		clrBit(PCINT0_MASK_REG, PCINT0_MASK_3_BIT);
	}
#endif
#if		PCINT2_EXISTS
	if (pinsList & PCINT2_PIN) {
		clrBit(PCINT0_MASK_REG, PCINT0_MASK_2_BIT);
	}
#endif
#if		PCINT1_EXISTS
	if (pinsList & PCINT1_PIN) {
		clrBit(PCINT0_MASK_REG, PCINT0_MASK_1_BIT);
	}
#endif
#if		PCINT0_EXISTS
	if (pinsList & PCINT0_PIN) {
		clrBit(PCINT0_MASK_REG, PCINT0_MASK_0_BIT);
	}
#endif

	this->lastError_ = ERROR_NONE;
	return true;
}

error_e Pcint0::getLastError(void)
{
	return this->lastError_;
}
#endif

// -----------------------------------------------------------------------------
// PCINT1 ----------------------------------------------------------------------

#if PCINT8_EXISTS  || PCINT9_EXISTS  || PCINT10_EXISTS || PCINT11_EXISTS || \
	PCINT12_EXISTS || PCINT13_EXISTS || PCINT14_EXISTS || PCINT15_EXISTS
bool Pcint1::enablePins(pcint1Pins_f pinsList, portMode_e outputMode)
{
	switch (outputMode) {
	case PORT_OUTPUT_LOW:
#if		PCINT15_EXISTS
		if (pinsList & PCINT15_PIN) {
			clrBit(PCINT15_OUT, PCINT15_BIT);
			setBit(PCINT15_DIR, PCINT15_BIT);
		}
#endif
#if		PCINT14_EXISTS
		if (pinsList & PCINT14_PIN) {
			clrBit(PCINT14_OUT, PCINT14_BIT);
			setBit(PCINT14_DIR, PCINT14_BIT);
		}
#endif
#if		PCINT13_EXISTS
		if (pinsList & PCINT13_PIN) {
			clrBit(PCINT13_OUT, PCINT13_BIT);
			setBit(PCINT13_DIR, PCINT13_BIT);
		}
#endif
#if		PCINT12_EXISTS
		if (pinsList & PCINT12_PIN) {
			clrBit(PCINT12_OUT, PCINT12_BIT);
			setBit(PCINT12_DIR, PCINT12_BIT);
		}
#endif
#if		PCINT11_EXISTS
		if (pinsList & PCINT11_PIN) {
			clrBit(PCINT11_OUT, PCINT11_BIT);
			setBit(PCINT11_DIR, PCINT11_BIT);
		}
#endif
#if		PCINT10_EXISTS
		if (pinsList & PCINT10_PIN) {
			clrBit(PCINT10_OUT, PCINT10_BIT);
			setBit(PCINT10_DIR, PCINT10_BIT);
		}
#endif
#if		PCINT9_EXISTS
		if (pinsList & PCINT9_PIN) {
			clrBit(PCINT9_OUT, PCINT9_BIT);
			setBit(PCINT9_DIR, PCINT9_BIT);
		}
#endif
#if		PCINT8_EXISTS
		if (pinsList & PCINT8_PIN) {
			clrBit(PCINT8_OUT, PCINT8_BIT);
			setBit(PCINT8_DIR, PCINT8_BIT);
		}
#endif
		break;
	case PORT_OUTPUT_HIGH:
#if		PCINT15_EXISTS
		if (pinsList & PCINT15_PIN) {
			setBit(PCINT15_OUT, PCINT15_BIT);
			setBit(PCINT15_DIR, PCINT15_BIT);
		}
#endif
#if		PCINT14_EXISTS
		if (pinsList & PCINT14_PIN) {
			setBit(PCINT14_OUT, PCINT14_BIT);
			setBit(PCINT14_DIR, PCINT14_BIT);
		}
#endif
#if		PCINT13_EXISTS
		if (pinsList & PCINT13_PIN) {
			setBit(PCINT13_OUT, PCINT13_BIT);
			setBit(PCINT13_DIR, PCINT13_BIT);
		}
#endif
#if		PCINT12_EXISTS
		if (pinsList & PCINT12_PIN) {
			setBit(PCINT12_OUT, PCINT12_BIT);
			setBit(PCINT12_DIR, PCINT12_BIT);
		}
#endif
#if		PCINT11_EXISTS
		if (pinsList & PCINT11_PIN) {
			setBit(PCINT11_OUT, PCINT11_BIT);
			setBit(PCINT11_DIR, PCINT11_BIT);
		}
#endif
#if		PCINT10_EXISTS
		if (pinsList & PCINT10_PIN) {
			setBit(PCINT10_OUT, PCINT10_BIT);
			setBit(PCINT10_DIR, PCINT10_BIT);
		}
#endif
#if		PCINT9_EXISTS
		if (pinsList & PCINT9_PIN) {
			setBit(PCINT9_OUT, PCINT9_BIT);
			setBit(PCINT9_DIR, PCINT9_BIT);
		}
#endif
#if		PCINT8_EXISTS
		if (pinsList & PCINT8_PIN) {
			setBit(PCINT8_OUT, PCINT8_BIT);
			setBit(PCINT8_DIR, PCINT8_BIT);
		}
#endif
		break;
	case PORT_INPUT_TRISTATE:
#if		PCINT15_EXISTS
		if (pinsList & PCINT15_PIN) {
			clrBit(PCINT15_OUT, PCINT15_BIT);
			clrBit(PCINT15_DIR, PCINT15_BIT);
		}
#endif
#if		PCINT14_EXISTS
		if (pinsList & PCINT14_PIN) {
			clrBit(PCINT14_OUT, PCINT14_BIT);
			clrBit(PCINT14_DIR, PCINT14_BIT);
		}
#endif
#if		PCINT13_EXISTS
		if (pinsList & PCINT13_PIN) {
			clrBit(PCINT13_OUT, PCINT13_BIT);
			clrBit(PCINT13_DIR, PCINT13_BIT);
		}
#endif
#if		PCINT12_EXISTS
		if (pinsList & PCINT12_PIN) {
			clrBit(PCINT12_OUT, PCINT12_BIT);
			clrBit(PCINT12_DIR, PCINT12_BIT);
		}
#endif
#if		PCINT11_EXISTS
		if (pinsList & PCINT11_PIN) {
			clrBit(PCINT11_OUT, PCINT11_BIT);
			clrBit(PCINT11_DIR, PCINT11_BIT);
		}
#endif
#if		PCINT10_EXISTS
		if (pinsList & PCINT10_PIN) {
			clrBit(PCINT10_OUT, PCINT10_BIT);
			clrBit(PCINT10_DIR, PCINT10_BIT);
		}
#endif
#if		PCINT9_EXISTS
		if (pinsList & PCINT9_PIN) {
			clrBit(PCINT9_OUT, PCINT9_BIT);
			clrBit(PCINT9_DIR, PCINT9_BIT);
		}
#endif
#if		PCINT8_EXISTS
		if (pinsList & PCINT8_PIN) {
			clrBit(PCINT8_OUT, PCINT8_BIT);
			clrBit(PCINT8_DIR, PCINT8_BIT);
		}
#endif
		break;
	case PORT_INPUT_PULL_UP:
#if		PCINT15_EXISTS
		if (pinsList & PCINT15_PIN) {
			setBit(PCINT15_OUT, PCINT15_BIT);
			clrBit(PCINT15_DIR, PCINT15_BIT);
		}
#endif
#if		PCINT14_EXISTS
		if (pinsList & PCINT14_PIN) {
			setBit(PCINT14_OUT, PCINT14_BIT);
			clrBit(PCINT14_DIR, PCINT14_BIT);
		}
#endif
#if		PCINT13_EXISTS
		if (pinsList & PCINT13_PIN) {
			setBit(PCINT13_OUT, PCINT13_BIT);
			clrBit(PCINT13_DIR, PCINT13_BIT);
		}
#endif
#if		PCINT12_EXISTS
		if (pinsList & PCINT12_PIN) {
			setBit(PCINT12_OUT, PCINT12_BIT);
			clrBit(PCINT12_DIR, PCINT12_BIT);
		}
#endif
#if		PCINT11_EXISTS
		if (pinsList & PCINT11_PIN) {
			setBit(PCINT11_OUT, PCINT11_BIT);
			clrBit(PCINT11_DIR, PCINT11_BIT);
		}
#endif
#if		PCINT10_EXISTS
		if (pinsList & PCINT10_PIN) {
			setBit(PCINT10_OUT, PCINT10_BIT);
			clrBit(PCINT10_DIR, PCINT10_BIT);
		}
#endif
#if		PCINT9_EXISTS
		if (pinsList & PCINT9_PIN) {
			setBit(PCINT9_OUT, PCINT9_BIT);
			clrBit(PCINT9_DIR, PCINT9_BIT);
		}
#endif
#if		PCINT8_EXISTS
		if (pinsList & PCINT8_PIN) {
			setBit(PCINT8_OUT, PCINT8_BIT);
			clrBit(PCINT8_DIR, PCINT8_BIT);
		}
#endif
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		this->lastError_ = ERROR_PORT_OUTPUT_MODE_UNSUPPORTED;
		return 	false;
	}

#if		PCINT15_EXISTS
	if (pinsList & PCINT15_PIN) {
		setBit(PCINT1_MASK_REG, PCINT1_MASK_15_BIT);
	}
#endif
#if		PCINT14_EXISTS
	if (pinsList & PCINT14_PIN) {
		setBit(PCINT1_MASK_REG, PCINT1_MASK_14_BIT);
	}
#endif
#if		PCINT13_EXISTS
	if (pinsList & PCINT13_PIN) {
		setBit(PCINT1_MASK_REG, PCINT1_MASK_13_BIT);
	}
#endif
#if		PCINT12_EXISTS
	if (pinsList & PCINT12_PIN) {
		setBit(PCINT1_MASK_REG, PCINT1_MASK_12_BIT);
	}
#endif
#if		PCINT11_EXISTS
	if (pinsList & PCINT11_PIN) {
		setBit(PCINT1_MASK_REG, PCINT1_MASK_11_BIT);
	}
#endif
#if		PCINT10_EXISTS
	if (pinsList & PCINT10_PIN) {
		setBit(PCINT1_MASK_REG, PCINT1_MASK_10_BIT);
	}
#endif
#if		PCINT9_EXISTS
	if (pinsList & PCINT9_PIN) {
		setBit(PCINT1_MASK_REG, PCINT1_MASK_9_BIT);
	}
#endif
#if		PCINT8_EXISTS
	if (pinsList & PCINT8_PIN) {
		setBit(PCINT1_MASK_REG, PCINT1_MASK_8_BIT);
	}
#endif

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Pcint1::disablePins(pcint1Pins_f pinsList)
{
#if		PCINT15_EXISTS
	if (pinsList & PCINT15_PIN) {
		clrBit(PCINT1_MASK_REG, PCINT1_MASK_15_BIT);
	}
#endif
#if		PCINT14_EXISTS
	if (pinsList & PCINT14_PIN) {
		clrBit(PCINT1_MASK_REG, PCINT1_MASK_14_BIT);
	}
#endif
#if		PCINT13_EXISTS
	if (pinsList & PCINT13_PIN) {
		clrBit(PCINT1_MASK_REG, PCINT1_MASK_13_BIT);
	}
#endif
#if		PCINT12_EXISTS
	if (pinsList & PCINT12_PIN) {
		clrBit(PCINT1_MASK_REG, PCINT1_MASK_12_BIT);
	}
#endif
#if		PCINT11_EXISTS
	if (pinsList & PCINT11_PIN) {
		clrBit(PCINT1_MASK_REG, PCINT1_MASK_11_BIT);
	}
#endif
#if		PCINT10_EXISTS
	if (pinsList & PCINT10_PIN) {
		clrBit(PCINT1_MASK_REG, PCINT1_MASK_10_BIT);
	}
#endif
#if		PCINT9_EXISTS
	if (pinsList & PCINT9_PIN) {
		clrBit(PCINT1_MASK_REG, PCINT1_MASK_9_BIT);
	}
#endif
#if		PCINT8_EXISTS
	if (pinsList & PCINT8_PIN) {
		clrBit(PCINT1_MASK_REG, PCINT1_MASK_8_BIT);
	}
#endif

	this->lastError_ = ERROR_NONE;
	return true;
}

error_e Pcint1::getLastError(void)
{
	return this->lastError_;
}
#endif

// -----------------------------------------------------------------------------
// PCINT2 ----------------------------------------------------------------------

#if PCINT16_EXISTS || PCINT17_EXISTS || PCINT18_EXISTS || PCINT19_EXISTS || \
	PCINT20_EXISTS || PCINT21_EXISTS || PCINT22_EXISTS || PCINT23_EXISTS
bool Pcint2::enablePins(pcint2Pins_f pinsList, portMode_e outputMode)
{
	switch (outputMode) {
	case PORT_OUTPUT_LOW:
#if		PCINT23_EXISTS
		if (pinsList & PCINT23_PIN) {
			clrBit(PCINT23_OUT, PCINT23_BIT);
			setBit(PCINT23_DIR, PCINT23_BIT);
		}
#endif
#if		PCINT22_EXISTS
		if (pinsList & PCINT22_PIN) {
			clrBit(PCINT22_OUT, PCINT22_BIT);
			setBit(PCINT22_DIR, PCINT22_BIT);
		}
#endif
#if		PCINT21_EXISTS
		if (pinsList & PCINT21_PIN) {
			clrBit(PCINT21_OUT, PCINT21_BIT);
			setBit(PCINT21_DIR, PCINT21_BIT);
		}
#endif
#if		PCINT20_EXISTS
		if (pinsList & PCINT20_PIN) {
			clrBit(PCINT20_OUT, PCINT20_BIT);
			setBit(PCINT20_DIR, PCINT20_BIT);
		}
#endif
#if		PCINT19_EXISTS
		if (pinsList & PCINT19_PIN) {
			clrBit(PCINT19_OUT, PCINT19_BIT);
			setBit(PCINT19_DIR, PCINT19_BIT);
		}
#endif
#if		PCINT18_EXISTS
		if (pinsList & PCINT18_PIN) {
			clrBit(PCINT18_OUT, PCINT18_BIT);
			setBit(PCINT18_DIR, PCINT18_BIT);
		}
#endif
#if		PCINT17_EXISTS
		if (pinsList & PCINT17_PIN) {
			clrBit(PCINT17_OUT, PCINT17_BIT);
			setBit(PCINT17_DIR, PCINT17_BIT);
		}
#endif
#if		PCINT16_EXISTS
		if (pinsList & PCINT16_PIN) {
			clrBit(PCINT16_OUT, PCINT16_BIT);
			setBit(PCINT16_DIR, PCINT16_BIT);
		}
#endif
		break;
	case PORT_OUTPUT_HIGH:
#if		PCINT23_EXISTS
		if (pinsList & PCINT23_PIN) {
			setBit(PCINT23_OUT, PCINT23_BIT);
			setBit(PCINT23_DIR, PCINT23_BIT);
		}
#endif
#if		PCINT22_EXISTS
		if (pinsList & PCINT22_PIN) {
			setBit(PCINT22_OUT, PCINT22_BIT);
			setBit(PCINT22_DIR, PCINT22_BIT);
		}
#endif
#if		PCINT21_EXISTS
		if (pinsList & PCINT21_PIN) {
			setBit(PCINT21_OUT, PCINT21_BIT);
			setBit(PCINT21_DIR, PCINT21_BIT);
		}
#endif
#if		PCINT20_EXISTS
		if (pinsList & PCINT20_PIN) {
			setBit(PCINT20_OUT, PCINT20_BIT);
			setBit(PCINT20_DIR, PCINT20_BIT);
		}
#endif
#if		PCINT19_EXISTS
		if (pinsList & PCINT19_PIN) {
			setBit(PCINT19_OUT, PCINT19_BIT);
			setBit(PCINT19_DIR, PCINT19_BIT);
		}
#endif
#if		PCINT18_EXISTS
		if (pinsList & PCINT18_PIN) {
			setBit(PCINT18_OUT, PCINT18_BIT);
			setBit(PCINT18_DIR, PCINT18_BIT);
		}
#endif
#if		PCINT17_EXISTS
		if (pinsList & PCINT17_PIN) {
			setBit(PCINT17_OUT, PCINT17_BIT);
			setBit(PCINT17_DIR, PCINT17_BIT);
		}
#endif
#if		PCINT16_EXISTS
		if (pinsList & PCINT16_PIN) {
			setBit(PCINT16_OUT, PCINT16_BIT);
			setBit(PCINT16_DIR, PCINT16_BIT);
		}
#endif
		break;
	case PORT_INPUT_TRISTATE:
#if		PCINT23_EXISTS
		if (pinsList & PCINT23_PIN) {
			clrBit(PCINT23_OUT, PCINT23_BIT);
			clrBit(PCINT23_DIR, PCINT23_BIT);
		}
#endif
#if		PCINT22_EXISTS
		if (pinsList & PCINT22_PIN) {
			clrBit(PCINT22_OUT, PCINT22_BIT);
			clrBit(PCINT22_DIR, PCINT22_BIT);
		}
#endif
#if		PCINT21_EXISTS
		if (pinsList & PCINT21_PIN) {
			clrBit(PCINT21_OUT, PCINT21_BIT);
			clrBit(PCINT21_DIR, PCINT21_BIT);
		}
#endif
#if		PCINT20_EXISTS
		if (pinsList & PCINT20_PIN) {
			clrBit(PCINT20_OUT, PCINT20_BIT);
			clrBit(PCINT20_DIR, PCINT20_BIT);
		}
#endif
#if		PCINT19_EXISTS
		if (pinsList & PCINT19_PIN) {
			clrBit(PCINT19_OUT, PCINT19_BIT);
			clrBit(PCINT19_DIR, PCINT19_BIT);
		}
#endif
#if		PCINT18_EXISTS
		if (pinsList & PCINT18_PIN) {
			clrBit(PCINT18_OUT, PCINT18_BIT);
			clrBit(PCINT18_DIR, PCINT18_BIT);
		}
#endif
#if		PCINT17_EXISTS
		if (pinsList & PCINT17_PIN) {
			clrBit(PCINT17_OUT, PCINT17_BIT);
			clrBit(PCINT17_DIR, PCINT17_BIT);
		}
#endif
#if		PCINT16_EXISTS
		if (pinsList & PCINT16_PIN) {
			clrBit(PCINT16_OUT, PCINT16_BIT);
			clrBit(PCINT16_DIR, PCINT16_BIT);
		}
#endif
		break;
	case PORT_INPUT_PULL_UP:
#if		PCINT23_EXISTS
		if (pinsList & PCINT23_PIN) {
			setBit(PCINT23_OUT, PCINT23_BIT);
			clrBit(PCINT23_DIR, PCINT23_BIT);
		}
#endif
#if		PCINT22_EXISTS
		if (pinsList & PCINT22_PIN) {
			setBit(PCINT22_OUT, PCINT22_BIT);
			clrBit(PCINT22_DIR, PCINT22_BIT);
		}
#endif
#if		PCINT21_EXISTS
		if (pinsList & PCINT21_PIN) {
			setBit(PCINT21_OUT, PCINT21_BIT);
			clrBit(PCINT21_DIR, PCINT21_BIT);
		}
#endif
#if		PCINT20_EXISTS
		if (pinsList & PCINT20_PIN) {
			setBit(PCINT20_OUT, PCINT20_BIT);
			clrBit(PCINT20_DIR, PCINT20_BIT);
		}
#endif
#if		PCINT19_EXISTS
		if (pinsList & PCINT19_PIN) {
			setBit(PCINT19_OUT, PCINT19_BIT);
			clrBit(PCINT19_DIR, PCINT19_BIT);
		}
#endif
#if		PCINT18_EXISTS
		if (pinsList & PCINT18_PIN) {
			setBit(PCINT18_OUT, PCINT18_BIT);
			clrBit(PCINT18_DIR, PCINT18_BIT);
		}
#endif
#if		PCINT17_EXISTS
		if (pinsList & PCINT17_PIN) {
			setBit(PCINT17_OUT, PCINT17_BIT);
			clrBit(PCINT17_DIR, PCINT17_BIT);
		}
#endif
#if		PCINT16_EXISTS
		if (pinsList & PCINT16_PIN) {
			setBit(PCINT16_OUT, PCINT16_BIT);
			clrBit(PCINT16_DIR, PCINT16_BIT);
		}
#endif
		break;
	case PORT_NO_CHANGE:
		break;
	default:
		this->lastError_ = ERROR_PORT_OUTPUT_MODE_UNSUPPORTED;
		return 	false;
	}

#if		PCINT23_EXISTS
	if (pinsList & PCINT23_PIN) {
		setBit(PCINT2_MASK_REG, PCINT2_MASK_23_BIT);
	}
#endif
#if		PCINT22_EXISTS
	if (pinsList & PCINT22_PIN) {
		setBit(PCINT2_MASK_REG, PCINT2_MASK_22_BIT);
	}
#endif
#if		PCINT21_EXISTS
	if (pinsList & PCINT21_PIN) {
		setBit(PCINT2_MASK_REG, PCINT2_MASK_21_BIT);
	}
#endif
#if		PCINT20_EXISTS
	if (pinsList & PCINT20_PIN) {
		setBit(PCINT2_MASK_REG, PCINT2_MASK_20_BIT);
	}
#endif
#if		PCINT19_EXISTS
	if (pinsList & PCINT19_PIN) {
		setBit(PCINT2_MASK_REG, PCINT2_MASK_19_BIT);
	}
#endif
#if		PCINT18_EXISTS
	if (pinsList & PCINT18_PIN) {
		setBit(PCINT2_MASK_REG, PCINT2_MASK_18_BIT);
	}
#endif
#if		PCINT17_EXISTS
	if (pinsList & PCINT17_PIN) {
		setBit(PCINT2_MASK_REG, PCINT2_MASK_17_BIT);
	}
#endif
#if		PCINT16_EXISTS
	if (pinsList & PCINT16_PIN) {
		setBit(PCINT2_MASK_REG, PCINT2_MASK_16_BIT);
	}
#endif

	this->lastError_ = ERROR_NONE;
	return true;
}

bool Pcint2::disablePins(pcint2Pins_f pinsList)
{
#if		PCINT23_EXISTS
	if (pinsList & PCINT23_PIN) {
		clrBit(PCINT2_MASK_REG, PCINT2_MASK_23_BIT);
	}
#endif
#if		PCINT22_EXISTS
	if (pinsList & PCINT22_PIN) {
		clrBit(PCINT2_MASK_REG, PCINT2_MASK_22_BIT);
	}
#endif
#if		PCINT21_EXISTS
	if (pinsList & PCINT21_PIN) {
		clrBit(PCINT2_MASK_REG, PCINT2_MASK_21_BIT);
	}
#endif
#if		PCINT20_EXISTS
	if (pinsList & PCINT20_PIN) {
		clrBit(PCINT2_MASK_REG, PCINT2_MASK_20_BIT);
	}
#endif
#if		PCINT19_EXISTS
	if (pinsList & PCINT19_PIN) {
		clrBit(PCINT2_MASK_REG, PCINT2_MASK_19_BIT);
	}
#endif
#if		PCINT18_EXISTS
	if (pinsList & PCINT18_PIN) {
		clrBit(PCINT2_MASK_REG, PCINT2_MASK_18_BIT);
	}
#endif
#if		PCINT17_EXISTS
	if (pinsList & PCINT17_PIN) {
		clrBit(PCINT2_MASK_REG, PCINT2_MASK_17_BIT);
	}
#endif
#if		PCINT16_EXISTS
	if (pinsList & PCINT16_PIN) {
		clrBit(PCINT2_MASK_REG, PCINT2_MASK_16_BIT);
	}
#endif

	this->lastError_ = ERROR_NONE;
	return true;
}

error_e Pcint2::getLastError(void)
{
	return this->lastError_;
}
#endif

// =============================================================================
// Class private methods
// =============================================================================

/* NONE */

// =============================================================================
// Class protected methods
// =============================================================================

/* NONE */

// =============================================================================
// General public functions definitions
// =============================================================================

/* NONE */

// =============================================================================
// Public functions definitions
// =============================================================================

/* NONE */

// =============================================================================
// Interruption handlers
// =============================================================================

/* NONE */
