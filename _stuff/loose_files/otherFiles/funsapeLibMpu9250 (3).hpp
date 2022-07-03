/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		funsapeLibMpu9250.hpp
 * Module:			MPU-9250 interface module for FunSAPE++ Embedded Library
 * 					project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __FUNSAPE_LIB_MPU9250_HPP
#define __FUNSAPE_LIB_MPU9250_HPP				__BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#	error	[funsapeLibMpu9250.hpp] Error 10 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibMpu9250.hpp] Error 11 - Build mismatch between (funsapeLibMpu9250.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "../util/funsapeLibHandler.hpp"
#ifndef __FUNSAPE_LIB_HANDLER_HPP
#	error	[funsapeLibMpu9250.hpp] Error 12 - Required module (funsapeLibHandler.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_HANDLER_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibMpu9250.hpp] Error 103 - Required module (funsapeLibHandler.hpp) must be build __BUILD_TO_REPLACE__.
#endif
#include "../funsapeLibSpi.hpp"
#ifndef __FUNSAPE_LIB_SPI_HPP
#	error	[funsapeLibMpu9250.hpp] Error 12 - Required module (funsapeLibSpi.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_SPI_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibMpu9250.hpp] Error 103 - Required module (funsapeLibSpi.hpp) must be build __BUILD_TO_REPLACE__.
#endif
#include "util/funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#	error	[funsapeLibMpu9250.hpp] Error 12 - Required module (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibMpu9250.hpp] Error 103 - Required module (funsapeLibDebug.hpp) must be build __BUILD_TO_REPLACE__.
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
// Mpu9250 - Class declaration
// =============================================================================

class Mpu9250
{
	// -------------------------------------------------------------------------
	// New data types ----------------------------------------------------------

public:
	// NONE

private:
	typedef enum interfaceMode_b : bool {
		INTERFACE_MODE_TWI				= false,
		INTERFACE_MODE_SPI				= true
	} interfaceMode_b;

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
	Mpu9250(void);
	Mpu9250(Handler *hdlr_p);
	~Mpu9250(void);

	// -------------------------------------------------------------------------
	// Methods - Class own methods ---------------------------------------------

public:
	bool configAccel(void);				// TODO: Implement this function
	bool configFifo(void);				// TODO: Implement this function
	bool configGyro(void);				// TODO: Implement this function
	bool configInterrupts(void);		// TODO: Implement this function
	error_e getLastError(void);
	bool init(void);
	bool setDevice(void (* selDevFunc_p)(void), void (* deselDevFunc_p)(void));
	bool setHandler(Handler *hdlr_p);

	bool getAccelRawData(int16_t *accelData_p, bool updateRawData_p);
	bool getGyroRawData(int16_t *gyroData_p, bool updateRawData_p);
	bool getTempRawData(int16_t *tempData_p, bool updateRawData_p);

	// TODO: Add magnetometer implementation

private:
//	bool _runSelfTest(void);
//	bool _writeData(void);
//	bool _readData(void);
	bool _deviceReady(void);
	void _resetDataMembers(void);
	bool _updateData(void);

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Properties  -------------------------------------------------------------

public:
	// NONE

private:
	//     /////////////////    PERIPHERAL HANDLER     //////////////////     //
	Handler		*_handler;

	//     /////////////////    CONTROL AND STATUS     //////////////////     //
	bool				_initialized			: 1;
	bool				_spiSelFuncReady		: 1;
	interfaceMode_b		_interfaceMode			: 1;
	error_e				_lastError;
	void	(* _spiSelectDevice)(void);
	void	(* _spiDeselectDevice)(void);

	//     //////////////////////     RAW DATA     //////////////////////     //
	int16_t			_accelRawData[3];
	int16_t			_gyroRawData[3];
	int16_t			_tempRawData;

	//     ///////////////////     PROCESSED DATA     ///////////////////     //
	float				_tempData;



protected:
	// NONE

};	// class Mpu9250

// =============================================================================
// Mpu9250 - Class overloading operators
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
// Mpu9250 - Class inline function definitions
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

#endif	// __FUNSAPE_LIB_MPU9250_HPP
