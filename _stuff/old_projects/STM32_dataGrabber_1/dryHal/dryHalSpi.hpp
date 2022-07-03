/* =============================================================================
 * Project:			Dry HAL Library
 * File name:		dryHalSpi.hpp
 * Module:			SPI Peripheral Module for Dry HAL Library project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __DRY_HAL_SPI_HPP
#define __DRY_HAL_SPI_HPP						__BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include <stddef.h>
#include "stm32f1xx.h"

// #include "../funsapeLibGlobalDefines.hpp"
// #ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
// #	error	[funsapeLibSpi.hpp] Error 9 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
// #elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
// #	error	[funsapeLibSpi.hpp] Error 10 - Build mismatch between (funsapeLibSpi.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
// #endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

// NONE

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
// General public functions declarations
// =============================================================================

HAL_StatusTypeDef dryHalSpiInit(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef dryHalSpiTransmitReceive(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout);

// =============================================================================
// General inline functions definitions
// =============================================================================

// NONE

#endif // __DRY_HAL_SPI_HPP
