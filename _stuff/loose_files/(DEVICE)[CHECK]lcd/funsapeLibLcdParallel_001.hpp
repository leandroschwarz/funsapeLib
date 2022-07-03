/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibLcdParallel.hpp
 * Module:			LCD Parallel Interface Handler for FunSAPE++ Embedded
 * 					Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_LCD_PARALLEL_HPP
#define __FUNSAPE_LIB_LCD_PARALLEL_HPP				__BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#	error	[funsapeLibLcdParallel.hpp] Error 10 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibLcdParallel.hpp] Error 11 - Build mismatch between (funsapeLibLcdParallel.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif
// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "../util/funsapeLibHandler.hpp"
#ifndef __FUNSAPE_LIB_HANDLER_HPP
#	error	[funsapeLibLcdParallel.hpp] Error 12 - Required module (funsapeLibHandler.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_HANDLER_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibLcdParallel.hpp] Error 103 - Required module (funsapeLibHandler.hpp) must be build __BUILD_TO_REPLACE__.
#endif

#include "../util/funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#	error	[funsapeLibLcdParallel.hpp] Error 12 - Required module (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibLcdParallel.hpp] Error 103 - Required module (funsapeLibDebug.hpp) must be build __BUILD_TO_REPLACE__.
#endif

#if defined(_FUNSAPE_PLATFORM_ARM_STM32)
#	include "main.h"
#endif

// =============================================================================
// Undefining previous definitions
// =============================================================================

// NONE

// =============================================================================
// Constant definitions
// =============================================================================

// NONE

// =============================================================================
// Macro-function definitions
// *INDENT-OFF*
// =============================================================================

// NONE

// *INDENT-ON*

// =============================================================================
// New data types
// =============================================================================

// NONE

// =============================================================================
// Extern global variables
// =============================================================================

// NONE

// =============================================================================
// LcdParallel - Class declaration
// =============================================================================

class LcdParallel : public Handler
{
	// -------------------------------------------------------------------------
	// New data types ----------------------------------------------------------

public:
	typedef enum dataLength_b : bool {
		DATA_LENGTH_4_BITS					= false,
		DATA_LENGTH_8_BITS					= true
	} dataLength_b;

private:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Operators overloading ---------------------------------------------------

public:
	// NONE

private:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Constructors ------------------------------------------------------------

public:
	LcdParallel(void);
	~LcdParallel(void);

	// -------------------------------------------------------------------------
	// Methods - Inherited methods ---------------------------------------------

public:
	Handler::handlerType_e getHandlerType(void);
	error_e getLastError(void);
	bool sendData(uint8_t *buffData, uint16_t buffSize = 1);

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Methods - Class own methods ---------------------------------------------

public:
#if defined(_FUNSAPE_PLATFORM_AVR)
	bool setDataPort(Gpio *dataGpio_p, uint8_t dataFirst_p, dataLength_b dataLength_p);
	bool setControlPort(Gpio *controlGpio_p, uint8_t controlE_p, uint8_t controlRs_p);
	bool setControlPort(Gpio *controlGpio_p, uint8_t controlE_p, uint8_t controlRs_p, uint8_t controlRw_p);
#elif defined(_FUNSAPE_PLATFORM_ARM_STM32)
	bool setDataPort(GPIO_TypeDef *dataGpio_p, uint8_t dataFirst_p, dataLength_b dataLength_p);
	bool setControlPort(GPIO_TypeDef *controlGpio_p, uint8_t controlE_p, uint8_t controlRs_p);
	bool setControlPort(GPIO_TypeDef *controlGpio_p, uint8_t controlE_p, uint8_t controlRs_p, uint8_t controlRw_p);
#endif
	bool init(void);

private:
	bool _sendData4Bits(uint8_t data_p);
	bool _sendData8Bits(uint8_t data_p);
	bool _holdGpio(void);
	bool _releaseGpio(void);

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Properties --------------------------------------------------------------

public:
	// NONE

private:
#if defined(_FUNSAPE_PLATFORM_AVR)
	Gpio								*_controlGpio;
	Gpio								*_dataGpio;
#elif defined(_FUNSAPE_PLATFORM_ARM_STM32)
	GPIO_TypeDef						*_controlGpio;
	GPIO_TypeDef						*_dataGpio;
#endif
	uint8_t								_controlE		: 4;
	bool								_controlPortSet	: 1;
	uint8_t								_controlRs		: 4;
	uint8_t								_controlRw		: 4;
	uint8_t								_dataFirst		: 4;
	dataLength_b						_dataLength		: 1;
	bool								_dataPortSet	: 1;
	bool								_initialized	: 1;
	error_e								_lastError;
	bool								_useBusyFlag	: 1;

protected:
	//NONE

};	// class LcdParallel

// =============================================================================
// LcdParallel - Class overloading operators
// =============================================================================

// NONE

// =============================================================================
// Global variables
// =============================================================================

// -----------------------------------------------------------------------------
// Externally defined global variables -----------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Internally defined global variables -----------------------------------------

// NONE

// =============================================================================
// Lcd - Class inline function definitions
// =============================================================================

// NONE

// =============================================================================
// General public functions declarations
// =============================================================================

// NONE

// =============================================================================
// General inline functions definitions
// =============================================================================

// NONE

#endif	// __FUNSAPE_LIB_LCD_PARALLEL_HPP
