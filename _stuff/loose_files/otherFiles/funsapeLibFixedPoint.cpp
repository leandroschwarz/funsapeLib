/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibFixedPoint.cpp
 * Module:			Fixed point arithmetic using Q types
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */


/*


// =============================================================================
// Dependencies
// =============================================================================

#include "funsapeLibFixedPoint.hpp"
#ifndef __FUNSAPE_LIB_FIXED_POINT_HPP
#	error	[funsapeLibFixedPoint.cpp] Error 1 - Header file (funsapeLibFixedPoint.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_FIXED_POINT_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibFixedPoint.cpp] Error 13 - Build mismatch between source (funsapeLibFixedPoint.cpp) and header (funsapeLibFixedPoint.hpp) files!
#endif

int16q4_t::int16q4_t(void)
{
	this->_value = 0;
}

int16q4_t::int16q4_t(float value_p)
{
	this->_value = 0;

	if(value_p < 0) {
		value_p = -value_p;
		this->_value |= (1 << 15);
	}
	this->_value |= int(value_p * 16) & ~(1 << 15);
}

uint16_t int16q4_t::getRawValue()
{
	return this->_value;
}
*/
