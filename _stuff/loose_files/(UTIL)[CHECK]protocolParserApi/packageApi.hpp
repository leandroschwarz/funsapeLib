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
	// New data types - Definitions
private:
	enum state_e {
		STATE_START_DELIMITER = 0,
		STATE_ID,
		STATE_DESTINATION_ADDRESS,
		STATE_SOURCE_ADDRESS,
		STATE_PAYLOAD_SIZE,
		STATE_PAYLOAD_DATA,
		STATE_CHECKSUM,
		STATE_READY
	};

	// New data types - Operators overloading
public:
	friend PackageApi::state_e &operator++(PackageApi::state_e &auxEnum);
	friend PackageApi::state_e operator++(PackageApi::state_e &auxEnum, int auxInt);
	friend PackageApi::state_e &operator--(PackageApi::state_e &auxEnum);
	friend PackageApi::state_e operator--(PackageApi::state_e &auxEnum, int auxInt);

	// Constructors
public:
	PackageApi(uint8_t payloadMaxSize = 255, bool useEscapeChar = false);

	// Methods
public:
	////////////////////////////////   CONFIG   ////////////////////////////////
	bool configCharSet(uint8_t startDelimiter);
	bool configCharSet(uint8_t startDelimiter, uint8_t escapeChar, uint8_t escapeXor);
	bool configCharSet(uint8_t startDelimiter, uint8_t escapeChar, uint8_t escapeXor, uint8_t * othersChars, uint8_t othersSize);
	error_e getLastError(void);

	//////////////////////////////////   RX   //////////////////////////////////
	bool rxAddData(uint8_t data);
	void rxFreePackage(void);
	bool rxGetDestinationAddress(uint8_t * destAddress);
	bool rxGetId(uint8_t * id);
	bool rxGetPayload(uint8_t * payloadData, uint8_t * payloadSize, uint8_t maxChars);
	bool rxGetSourceAddress(uint8_t * sourcAddress);
	bool rxIsReady(void);

	//////////////////////////////////   TX   //////////////////////////////////
	bool txCreatePackage(uint8_t * packageData, uint16_t * packageSize, uint16_t maxChars);
	bool txSetDestinationAddress(uint8_t destAddress);
	bool txSetId(uint8_t id, bool autoIncrement);
	bool txSetPayload(uint8_t * payload, uint8_t size);
	bool txSetSourceAddress(uint8_t sourcAddress);

private:
	bool checkInitialized(void);
	bool rxProcessEscapeCharacter(void);
	bool rxProcessFrameFormat(void);
	bool rxProcessNextState(void);
	bool rxProcessRawAddData(void);
	void rxResetPackage(void);
	bool txAddData(uint8_t data);
	bool txEncodePackage(void);

protected:

	// Data members
public:

private:
	// Data members - General information
	error_e		lastError_;
	uint16_t	rawMaxSize_					: 10;
	uint8_t		payloadMaxSize_;
	bool		initialized_				: 1;
	// Data members - Start delimiter and escape options
	bool		configEscapeEnabled_		: 1;
	uint8_t		configEscapeCharacter_;
	uint8_t		configEscapeXorDecoded_;
	uint8_t		configEscapeCharSize_		: 5;
	uint8_t		* configEscapeCharData_;
	uint8_t		configStartDelimiter_;
	// Reset data members - Ingoing package data
	uint8_t		rxRawChecksum_;
	uint8_t		* rxRawData_;
	bool		rxRawEscape_				: 1;
	uint16_t	rxRawIndex_					: 10;
	bool		rxRawReady_					: 1;
	uint16_t	rxRawSize_					: 10;
	state_e		rxRawState_					: 4;
	uint8_t		rxDecodedAuxData_;
	uint8_t		rxDecodedDestAddress_;
	uint8_t		rxDecodedId_;
	uint8_t		* rxDecodedPayloadData_;
	uint8_t		rxDecodedPayloadIndex_;
	uint8_t		rxDecodedPayloadSize_;
	uint8_t		rxDecodedSourAddress_;
	// Data members - Outgoing package data
	uint8_t		* txRawData_;
	uint16_t	txRawIndex_					: 10;
	uint16_t	txRawSize_					: 10;
	uint8_t		txDecodedDestAddress_;
	uint8_t		txDecodedId_;
	bool		txDecodedIdAutoIncrement_	: 1;
	uint8_t		* txDecodedPayloadData_;
	uint8_t		txDecodedPayloadIndex_;
	uint8_t		txDecodedPayloadSize_;
	uint8_t		txDecodedSourAddress_;

protected:

}; // class PackageApi

// =============================================================================
// PackageApi - Class inline function definitions
// =============================================================================

inline PackageApi::state_e &operator++(PackageApi::state_e &auxEnum)
{
	auxEnum = (auxEnum == PackageApi::STATE_READY) ? PackageApi::STATE_START_DELIMITER : PackageApi::state_e(auxEnum + 1);
	return auxEnum;
}

inline PackageApi::state_e operator++(PackageApi::state_e &auxEnum, int auxInt)
{
	PackageApi::state_e auxEnum2(auxEnum);
	++auxEnum;
	return auxEnum2;
}

inline PackageApi::state_e &operator--(PackageApi::state_e &auxEnum)
{
	auxEnum = (auxEnum == PackageApi::STATE_START_DELIMITER) ? PackageApi::STATE_READY : PackageApi::state_e(auxEnum - 1);
	return auxEnum;
}

inline PackageApi::state_e operator--(PackageApi::state_e &auxEnum, int auxInt)
{
	PackageApi::state_e auxEnum2(auxEnum);
	--auxEnum;
	return auxEnum2;
}

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
