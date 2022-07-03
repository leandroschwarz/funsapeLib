/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	packageApi.hpp
 * Module:		API package parser for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1002
 * Date:		June 06, 2018
 * ========================================================================== */

#ifndef __PACKAGE_API_HPP
#define __PACKAGE_API_HPP	1002

// =============================================================================
// Dependencies
// =============================================================================

// Global definitions file
#include "..\globalDefines.hpp"
#if __GLOBAL_DEFINES_HPP != 1002
#	error	[packageApi.hpp] Error 101 - Global definitions file (globalDefines.hpp) must be build 1002.
#endif

// Header files
/* NONE */

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

/* NONE */

// =============================================================================
// PackageApi - Class declaration
// =============================================================================

class PackageApi
{
	// New data types
public:

	// Constructors
public:
	PackageApi(uint16_t packageSize = 100);

	// Methods
public:
	bool createNewPackage(uint8_t id, uint8_t destAddr, uint8_t sourAddr, uint8_t * payload, uint8_t payloadSize);
	uint8_t * getPackageData(void);
	uint16_t getPackageSize(void);
	bool setEscapeCharacters(bool enable, uint8_t * charList = NULL, uint8_t listSize = 0);
	error_e getLastError(void);

private:
	bool checkInitilized(void);
	bool checkUnlocked(void);
	bool checkReady(void);
	bool pushPackageData(uint8_t data);

protected:

	// Data members
public:

private:
	// Package data
	uint16_t	index_				: 12;
	uint8_t		* data_;
	uint16_t	size_				: 12;
	// Package status
	error_e		lastError_;
	bool		initialized_		: 1;
	bool		locked_				: 1;
	bool		ready_				: 1;
	// Package configuration
	uint16_t	maxSize_			: 12;
	uint8_t		startDelimiter_;
	// Escape character configuration
	bool		escapeEnabled_		: 1;
	uint8_t		escapeCharacter_;
	uint8_t		escapeXorValue_;
	uint8_t		escapeCharSize_;
	uint8_t		* escapeCharData_;

protected:

}; // class PackageApi

// =============================================================================
// PackageApi - Class inline function definitions
// =============================================================================

/* NONE */

// =============================================================================
// Extern global variables
// =============================================================================

/* NONE */

// =============================================================================
// General public functions declarations
// =============================================================================

/* NONE */

// =============================================================================
// General inline functions definitions
// =============================================================================

/* NONE */

#endif // __PACKAGE_API_HPP
