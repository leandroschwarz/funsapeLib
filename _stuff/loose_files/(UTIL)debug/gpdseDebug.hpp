/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	gpdseDebug.hpp
 * Module:		Debug module for GPDSE++ Library project
 * Author :		Leandro Schwarz
 * Build :		1002
 * Date :		June 25, 2018
 * ========================================================================== */

#ifndef __GPDSE_DEBUG_HPP
#define __GPDSE_DEBUG_HPP 1002

// =============================================================================
// Dependencies
// =============================================================================

// Global definitions file
#include <globalDefines.hpp>
#if __GLOBAL_DEFINES_HPP != 1002
#	error	[gpdseDebug.hpp] Error 101 - Global definitions file (globalDefines.hpp) must be build 1002.
#endif

// Header files
#include <avr/pgmspace.h>

// =============================================================================
// Undefining previous definitions
// =============================================================================

// NONE

// =============================================================================
// Constants
// =============================================================================

// NONE

// =============================================================================
// Macro-functions
// =============================================================================

// NONE

// =============================================================================
// New data types
// =============================================================================

typedef enum dbgFunctionName_e {		// 16 bit variable

	// System functions
	DBG_SYS_MAIN_CONFIG					= 0,
	DBG_SYS_MAIN_LOOP					= 1,

	// DemiFatFs
	DBG_DFF_CONSTRUCTOR					= 2,
	DBG_DFF_DESTRUCTOR					= 3,
	DBG_DFF_MOUNT						= 4,
	DBG_DFF_OPEN_DIR					= 5,
	DBG_DFF_CLOSE_DIR					= 6,
	DBG_DFF_READ_DIR					= 7,
	DBG_DFF_F_MOUNT						= 8,
	DBG_DFF_F_OPEN_DIR					= 9,
	DBG_DFF_F_CLOSE_DIR					= 10,
	DBG_DFF_F_READ_DIR					= 11,
	DBG_DFF_LD_WORD						= 12,
	DBG_DFF_LD_DWORD					= 13,
	DBG_DFF_ST_WORD						= 14,
	DBG_DFF_ST_DWORD					= 15,
	DBG_DFF_MEM_CPY						= 16,
	DBG_DFF_MEM_SET						= 17,
	DBG_DFF_MEM_CMP						= 18,
	DBG_DFF_CHK_CHR						= 19,
	DBG_DFF_DBC_1ST						= 20,
	DBG_DFF_DBC_2ND						= 21,
	DBG_DFF_SYNC_WINDOW					= 22,
	DBG_DFF_MOVE_WINDOW					= 23,
	DBG_DFF_SYNC_FS						= 24,
	DBG_DFF_CLST2SECT					= 25,
	DBG_DFF_GET_FAT						= 26,
	DBG_DFF_PUT_FAT						= 27,
	DBG_DFF_REMOVE_CHAIN				= 28,
	DBG_DFF_CREATE_CHAIN				= 29,
	DBG_DFF_DIR_CLEAR					= 30,
	DBG_DFF_DIR_SDI						= 31,
	DBG_DFF_DIR_NEXT					= 32,
	DBG_DFF_DIR_ALLOC					= 33,
	DBG_DFF_LD_CLUST					= 34,
	DBG_DFF_ST_CLUST					= 35,
	DBG_DFF_DIR_READ					= 36,
	DBG_DFF_DIR_FIND					= 37,
	DBG_DFF_DIR_REGISTER				= 38,
	DBG_DFF_DIR_REMOVE					= 39,
	DBG_DFF_GET_FILE_INFO				= 40,
	DBG_DFF_CREATE_NAME					= 41,
	DBG_DFF_FOLLOW_PATH					= 42,
	DBG_DFF_GET_LD_NUMBER				= 43,
	DBG_DFF_CHECK_FS					= 44,
	DBG_DFF_FIND_VOLUME					= 45,
	DBG_DFF_VALIDATE					= 46,

	// Circular Buffer 8 functions

	// Circular Buffer 16 functions

	// Circular Buffer 32 functions

	// Circular Buffer 64 functions

	// Date Time functions

	// Package API functions

	DBG_VECTOR_SIZE
} dbgFunctionName_e;

// =============================================================================
// Extern global variables
// =============================================================================


// =============================================================================
// General public functions declarations
// =============================================================================

void dbgFunctionEntering(dbgFunctionName_e funcName);
void dbgFunctionReturning(dbgFunctionName_e funcName, uint16_t returnCode);
void dbgFunctionResuming(dbgFunctionName_e funcName);
void dbgPointReaching(uint32_t pointCode);

#endif // __GPDSE_DEBUG_HPP
