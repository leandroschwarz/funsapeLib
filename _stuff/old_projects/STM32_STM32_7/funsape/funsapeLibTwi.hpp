/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibTwi.hpp
 * Module:			TWI Peripheral module for FunSAPE++ Embedded Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_TWI_HPP
#define __FUNSAPE_LIB_TWI_HPP					__BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#	error	[funsapeLibTwi.hpp] Error 12 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibTwi.hpp] Error 13 - Build mismatch between global definitions file (funsapeLibGlobalDefines.hpp) and current header file (funsapeLibTwi.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "util/funsapeLibBus.hpp"
#ifndef __FUNSAPE_LIB_BUS_HPP
#	error	[funsapeLibTwi.hpp] Error 14 - Required module (funsapeLibBus.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_BUS_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibTwi.hpp] Error 15 - Build mismatch between required module (funsapeLibBus.hpp) and current header file (funsapeLibTwi.hpp)!
#endif

#include "util/funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#	error	[funsapeLibTwi.hpp] Error 14 - Required module (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibTwi.hpp] Error 15 - Build mismatch between required module (funsapeLibDebug.hpp) and current header file (funsapeLibTwi.hpp)!
#endif

// =============================================================================
// Platform verification
// =============================================================================

#if !defined(_FUNSAPE_PLATFORM_ARM_STM32)
#	error [funsapeLibTwi.hpp] Error 18 - Feature is unsupported or was not implemented for selected platform!
#elif !defined(HAL_I2C_MODULE_ENABLED)
#	error [funsapeLibTwi.hpp] Error 19 - Module was not enabled in platform HAL files!
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
// Twi - Class declaration
// =============================================================================

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
class Twi : public Bus
{
	// -------------------------------------------------------------------------
	// New data types ----------------------------------------------------------

public:
	// NONE

private:
	/**
	 * @brief
	 *
	 * Detailed description of this element.
	 *
	 */
	enum class TwiOperation : bool {
		READ				= 0,
		WRITE				= 1,
	};

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Operator overloading ----------------------------------------------------

public:
	// NONE

private:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Constructors ------------------------------------------------------------

public:
	Twi(
			void
	);
	~Twi(
			void
	);

	// -------------------------------------------------------------------------
	// Methods - Inherited methods ---------------------------------------------

public:
	//     ////////////////////    DATA TRANSFER     ////////////////////     //
	bool readReg(
			uint8_t reg_p,
			uint8_t *buffData_p,
			uint16_t buffSize_p = 1
	);
	bool writeReg(
			cuint8_t reg_p,
			cuint8_t *buffData_p,
			cuint16_t buffSize_p = 1
	);

	//     //////////////////    PROTOCOL SPECIFIC     //////////////////     //
	bool setDevice(
			cuint16_t address_p,
			cbool useLongAddress_p = false
	);

	//     /////////////////     CONTROL AND STATUS     /////////////////     //
	Bus::BusType getBusType(
			void
	);

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Methods - Class own methods ---------------------------------------------

public:
	//     /////////////////     CONTROL AND STATUS     /////////////////     //
	Error getLastError(
			void
	);
	bool init(
			I2C_HandleTypeDef *busHandler_p,
			cuint16_t bufSize_p = 20
	);
	bool setTimeout(
			cuint16_t timeout_p
	);

private:
	//     /////////////////     CONTROL AND STATUS     /////////////////     //
	void _clearData(
			void
	);
	void _deinitialize(
			void
	);
	bool _initialize(
			void
	);
	bool _isInitialized(
			void
	);
	bool _transmit(
			void
	);

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Properties --------------------------------------------------------------

public:
	// NONE

private:
	//     /////////////////    PERIPHERAL HANDLER     //////////////////     //
	I2C_HandleTypeDef	*_busHandler;

	//     /////////////////    CONTROL AND STATUS     //////////////////     //
	bool				_busy					: 1;
	uint16_t			_devAddress				: 10;
	bool				_devAddressSet			: 1;
	bool				_initialized			: 1;
	Error				_lastError;
	TwiOperation		_operation;
	uint8_t				_regAddress;
	uint16_t			_timeout;

	//     /////////////     HAL DATA STRUCT VARIABLES     //////////////     //
	uint32_t			_clockSpeed				: 22;
	uint16_t			_ownAddress1			: 10;
	uint16_t			_ownAddress2			: 10;
	bool				_useDualAddress			: 1;
	bool				_useDutyCycle16_9		: 1;
	bool				_useGeneralCall			: 1;
	bool				_useLongAddress			: 1;
	bool				_useNoStretchMode		: 1;

	//     ////////////////////    DATA BUFFERS      ////////////////////     //
	uint8_t				*_bufferData;
	uint16_t			_bufferIndex;
	uint16_t			_bufferLength;
	uint16_t			_bufferMaxSize;

protected:
	// NONE

}; // class Twi

// =============================================================================
// Twi - Class overloading operators
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
// Twi - Class inline function definitions
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

// =============================================================================
// External default objects
// =============================================================================

// NONE

#endif // __FUNSAPE_LIB_TWI_HPP

// =============================================================================
// END OF FILE
// =============================================================================
