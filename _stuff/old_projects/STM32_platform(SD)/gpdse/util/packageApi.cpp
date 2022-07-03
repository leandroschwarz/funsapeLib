/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	packageApi.cpp
 * Module:		API package parser for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1002
 * Date:		June 06, 2018
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "packageApi.hpp"
#if __PACKAGE_API_HPP != 1002
#	error [packageApi.cpp] Error 102 - Build mismatch on header and source code files.
#endif
#include <stdlib.h>

#include "../../mainDefs.h"
#include "../../functions.hpp"
#include "../../peripheral.hpp"
#include "../STM32/STM32L0xx_HAL_Driver/stm32l0xx_hal.h"

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
// =============================================================================

/* NONE */

// =============================================================================
// Class constructors
// =============================================================================

PackageApi::PackageApi(uint8_t payloadMaxSize, bool useEscapeChar)
{
	// Reset data members - General information
	this->initialized_ = false;
	this->lastError_ = ERROR_NOT_INITIALIZED;
	this->payloadMaxSize_ = 0;
	this->rawMaxSize_ = 0;
	// Reset data members - Start delimiter and escape options
	this->configEscapeCharacter_ = 0x7D;
	this->configEscapeCharData_ = NULL;
	this->configEscapeCharSize_ = 0;
	this->configEscapeEnabled_ = false;
	this->configEscapeXorDecoded_ = 0x20;
	this->configStartDelimiter_ = 0x7E;
	// Reset data members - Ingoing package data
	this->rxRawChecksum_ = 0xFF;
	this->rxRawData_ = NULL;
	this->rxRawEscape_ = false;
	this->rxRawIndex_ = 0;
	this->rxRawReady_ = false;
	this->rxRawSize_ = 0;
	this->rxRawState_ = STATE_START_DELIMITER;
	this->rxDecodedAuxData_ = 0;
	this->rxDecodedDestAddress_ = 0;
	this->rxDecodedId_ = 0;
	this->txDecodedIdAutoIncrement_ = false;
	this->rxDecodedPayloadData_ = NULL;
	this->rxDecodedPayloadIndex_ = 0;
	this->rxDecodedPayloadSize_ = 0;
	this->rxDecodedSourAddress_ = 0;
	// Reset data members - Outgoing package data
	this->txRawData_ = NULL;
	this->txRawIndex_ = 0;
	this->txRawSize_ = 0;
	this->txDecodedDestAddress_ = 0;
	this->txDecodedId_ = 0;
	this->txDecodedPayloadData_ = NULL;
	this->txDecodedPayloadIndex_ = 0;
	this->txDecodedPayloadSize_ = 0;
	this->txDecodedSourAddress_ = 0;

	// Check for errors
	if (payloadMaxSize == 0) {
		this->lastError_ = ERROR_BUFFER_SIZE_ZERO;
		return;
	}
	if (payloadMaxSize < 2) {
		this->lastError_ = ERROR_BUFFER_SIZE_TOO_SMALL;
		return;
	}

	// Update data members
	this->configEscapeEnabled_ = useEscapeChar;
	this->payloadMaxSize_ = payloadMaxSize;
	this->rawMaxSize_ = 5 + (uint16_t)this->payloadMaxSize_;	// (ID+DA+SA+SZ+CH)+PL
	if (useEscapeChar) {
		this->rawMaxSize_ *= 2;									// Double for escapes
	}
	this->rawMaxSize_++;										// Add START

	// Memory allocation
	// Raw Rx Data
	this->rxRawData_ = (uint8_t *)calloc(this->rawMaxSize_, sizeof(uint8_t));
	if (this->rxRawData_ == NULL) {					// ERROR
		this->lastError_ = ERROR_MEMORY_ALLOCATION;
		return;
	}
	// Raw Tx Data
	this->txRawData_ = (uint8_t *)calloc(this->rawMaxSize_, sizeof(uint8_t));
	if (this->txRawData_ == NULL) {					// ERROR
		free(this->rxRawData_);
		this->lastError_ = ERROR_MEMORY_ALLOCATION;
		return;
	}
	// Decoded Rx Data
	this->rxDecodedPayloadData_ = (uint8_t *)calloc(this->payloadMaxSize_, sizeof(uint8_t));
	if (this->rxDecodedPayloadData_ == NULL) {		// ERROR
		free(this->rxRawData_);
		free(this->txRawData_);
		this->lastError_ = ERROR_MEMORY_ALLOCATION;
		return;
	}
	// Decoded Tx Data
	this->txDecodedPayloadData_ = (uint8_t *)calloc(this->payloadMaxSize_, sizeof(uint8_t));
	if (this->txDecodedPayloadData_ == NULL) {		// ERROR
		free(this->rxRawData_);
		free(this->txRawData_);
		free(this->rxDecodedPayloadData_);
		this->lastError_ = ERROR_MEMORY_ALLOCATION;
		return;
	}
	this->initialized_ = true;

	this->lastError_ = ERROR_NONE;
	return;
}

// =============================================================================
// Class public methods
// =============================================================================

//////////////////////////////////   CONFIG   //////////////////////////////////

bool PackageApi::configCharSet(uint8_t startDelimiter)
{
	// Check for errors
	if (!this->checkInitialized()) {
		return false;
	}
	if (this->configEscapeEnabled_) {
		this->lastError_ = ERROR_ARGUMENTS_WRONG_NUMBER;
		return false;
	}

	// Free configEscapeCharData_ memory
	if (this->configEscapeCharData_ != NULL) {
		free(this->configEscapeCharData_);
		this->configEscapeCharData_ = NULL;
	}

	// Update data members
	this->configEscapeCharacter_ = 0x00;
	this->configEscapeXorDecoded_ = 0;
	this->configEscapeCharSize_ = 0;
	this->configStartDelimiter_ = startDelimiter;

	this->lastError_ = ERROR_NONE;
	return true;
}

bool PackageApi::configCharSet(uint8_t startDelimiter, uint8_t escapeChar, uint8_t escapeXor)
{
	// Check for errors
	if (!this->checkInitialized()) {
		return false;
	}
	if (!this->configEscapeEnabled_) {
		this->lastError_ = ERROR_ARGUMENTS_WRONG_NUMBER;
		return false;
	}

	// Free configEscapeCharData_ memory
	if (this->configEscapeCharData_ != NULL) {
		free(this->configEscapeCharData_);
		this->configEscapeCharData_ = NULL;
	}
	this->configEscapeCharSize_ = 0;

	// Memory allocation
	this->configEscapeCharData_ = (uint8_t *)calloc(2, sizeof(uint8_t));
	if (this->configEscapeCharData_ == NULL) {
		this->lastError_ = ERROR_MEMORY_ALLOCATION;
		return false;
	}
	this->configEscapeCharData_[0] = startDelimiter;
	this->configEscapeCharData_[1] = escapeChar;

	// Update data members
	this->configEscapeCharSize_ = 2;
	this->configEscapeXorDecoded_ = escapeXor;
	this->configEscapeCharacter_ = escapeChar;
	this->configStartDelimiter_ = startDelimiter;

	this->lastError_ = ERROR_NONE;
	return true;
}

bool PackageApi::configCharSet(uint8_t startDelimiter, uint8_t escapeChar, uint8_t escapeXor, uint8_t * othersChars, uint8_t othersSize)
{
	// Check for errors
	if (!this->checkInitialized()) {
		return false;
	}
	if (!this->configEscapeEnabled_) {
		this->lastError_ = ERROR_ARGUMENTS_WRONG_NUMBER;
		return false;
	}
	if (othersChars == NULL) {
		this->lastError_ = ERROR_BUFFER_NULL_POINTER;
		return false;
	}
	if (othersSize == 0) {
		this->lastError_ = ERROR_BUFFER_SIZE_ZERO;
		return false;
	}
	if (othersSize > 30) {
		this->lastError_ = ERROR_BUFFER_SIZE_TOO_LARGE;
		return false;
	}

	// Free configEscapeCharData_ memory
	if (this->configEscapeCharData_ != NULL) {
		free(this->configEscapeCharData_);
		this->configEscapeCharData_ = NULL;
	}
	this->configEscapeCharSize_ = 0;

	// Memory allocation
	this->configEscapeCharData_ = (uint8_t *)calloc((othersSize + 2), sizeof(uint8_t));
	if (this->configEscapeCharData_ == NULL) {
		this->lastError_ = ERROR_MEMORY_ALLOCATION;
		return false;
	}
	this->configEscapeCharData_[0] = startDelimiter;
	this->configEscapeCharData_[1] = escapeChar;
	for (uint8_t i = 0; i < othersSize; i++) {
		this->configEscapeCharData_[i + 2] = othersChars[i];
	}

	// Update data members
	this->configEscapeCharSize_ = (othersSize + 2);
	this->configEscapeXorDecoded_ = escapeXor;
	this->configEscapeCharacter_ = escapeChar;
	this->configStartDelimiter_ = startDelimiter;

	this->lastError_ = ERROR_NONE;
	return true;
}

error_e PackageApi::getLastError(void)
{
	return this->lastError_;
}

////////////////////////////////////   RX   ////////////////////////////////////

bool PackageApi::rxAddData(uint8_t data)
{
	// Check for errors
	if (!this->checkInitialized()) {
		return false;
	}
	if (this->rxRawReady_) {
		this->lastError_ = ERROR_PACKAGE_API_PACKAGE_AWAITING;
		return false;
	}
	if (this->rxRawState_ == STATE_READY) {
		this->lastError_ = ERROR_PACKAGE_API_PACKAGE_AWAITING;
		return false;
	}

	// Update auxData
	this->rxDecodedAuxData_ = data;

	// Process escape character
	if (!this->rxProcessEscapeCharacter()) {		// Could be an error, checking...
		if (this->lastError_ == ERROR_NONE) {		// Not an error, return successful
			return true;
		} else {									// It is a really an error
			return false;
		}
	} // No error detected, continuing process

	// Check frame format
	if (!rxProcessFrameFormat()) {					// Frame error
		return false;
	}

	// Put data in raw package
	if (!this->rxProcessRawAddData()) {
		return false;
	}

	// Evaluates states machine next state
	return this->rxProcessNextState();
}

void PackageApi::rxFreePackage(void)
{
	this->rxRawReady_ = false;
	this->rxResetPackage();
}

bool PackageApi::rxGetDestinationAddress(uint8_t * destAddress)
{
	// Check for errors
	if (!this->checkInitialized()) {
		return false;
	}
	if (destAddress == NULL) {
		this->lastError_ = ERROR_BUFFER_NULL_POINTER;
		return false;
	}

	*destAddress = this->rxDecodedDestAddress_;
	this->lastError_ = ERROR_NONE;
	return true;
}

bool PackageApi::rxGetId(uint8_t * id)
{
	// Check for errors
	if (!this->checkInitialized()) {
		return false;
	}
	if (id == NULL) {
		this->lastError_ = ERROR_BUFFER_NULL_POINTER;
		return false;
	}

	*id = this->rxDecodedId_;
	this->lastError_ = ERROR_NONE;
	return true;
}

bool PackageApi::rxGetPayload(uint8_t * payloadData, uint8_t * payloadSize, uint8_t maxChars)
{
	// Check for errors
	if (!this->checkInitialized()) {
		return false;
	}
	if ((payloadData == NULL) || (payloadSize == NULL)) {
		this->lastError_ = ERROR_BUFFER_NULL_POINTER;
		return false;
	}

	// Retrieve payload size
	*payloadSize = this->rxDecodedPayloadSize_;

	// Retrieve payload data
	for (uint8_t i = 0; i < this->rxDecodedPayloadSize_; i++) {
		if (i == maxChars) {
			this->lastError_ = ERROR_BUFFER_FULL;
			return false;
		}
		payloadData[i] = this->rxDecodedPayloadData_[i];
	}
	this->lastError_ = ERROR_NONE;
	return true;
}

bool PackageApi::rxGetSourceAddress(uint8_t * sourAddress)
{
	// Check for errors
	if (!this->checkInitialized()) {
		return false;
	}
	if (sourAddress == NULL) {
		this->lastError_ = ERROR_BUFFER_NULL_POINTER;
		return false;
	}

	*sourAddress = this->rxDecodedSourAddress_;
	this->lastError_ = ERROR_NONE;
	return true;
}

bool PackageApi::rxIsReady(void)
{
	return this->rxRawReady_;
}

////////////////////////////////////   TX   ////////////////////////////////////

bool PackageApi::txCreatePackage(uint8_t * packageData, uint16_t * packageSize, uint16_t maxChars)
{
#if (GPDSE_DEBUG_MODULE_ALL == 1) || (GPDSE_DEBUG_MODULE_PACKAGE_API == 1)
	uint8_t debBuf[30];
	debModule_e debModule = DEB_MOD_PACKAGE_API;
	debFunctionId_e debFunctionId = DEB_FUN_PACKAGE_API_TX_CREATE_PACKAGE;
	debugPackageApi(1);
#endif

	// Check for errors
	if (!this->checkInitialized()) {
#if (GPDSE_DEBUG_MODULE_ALL == 1) || (GPDSE_DEBUG_MODULE_PACKAGE_API == 1)
		debugPackageApi(2);
#endif
		return false;
	}
	if ((packageData == NULL) || (packageSize == NULL)) {
		this->lastError_ = ERROR_BUFFER_NULL_POINTER;
#if (GPDSE_DEBUG_MODULE_ALL == 1) || (GPDSE_DEBUG_MODULE_PACKAGE_API == 1)
		debugPackageApi(3);
#endif
		return false;
	}

	// Reset Raw Tx data members
	this->txRawIndex_ = 0;
	this->txRawSize_ = 0;

	// Encode raw package
	if (!this->txEncodePackage()) {
#if (GPDSE_DEBUG_MODULE_ALL == 1) || (GPDSE_DEBUG_MODULE_PACKAGE_API == 1)
		debugPackageApi(4);
#endif
		return false;
	}


	// Transfer data
	for (uint16_t i = 0; i < this->txRawSize_; i++) {

		if (i == maxChars) {
			this->lastError_ = ERROR_BUFFER_FULL;
#if (GPDSE_DEBUG_MODULE_ALL == 1) || (GPDSE_DEBUG_MODULE_PACKAGE_API == 1)
			debugPackageApi(5);
#endif
			return false;
		}
		packageData[i] = this->txRawData_[i];
	}
	*packageSize = this->txRawSize_;

	this->lastError_ = ERROR_NONE;
#if (GPDSE_DEBUG_MODULE_ALL == 1) || (GPDSE_DEBUG_MODULE_PACKAGE_API == 1)
	debugPackageApi(6);
#endif
	return true;
}

bool PackageApi::txSetDestinationAddress(uint8_t destAddress)
{
	// Check for errors
	if (!this->checkInitialized()) {
		return false;
	}

	this->txDecodedDestAddress_ = destAddress;
	this->lastError_ = ERROR_NONE;
	return true;
}

bool PackageApi::txSetId(uint8_t id, bool autoIncrement)
{
	// Check for errors
	if (!this->checkInitialized()) {
		return false;
	}

	this->txDecodedId_ = id;
	this->txDecodedIdAutoIncrement_ = autoIncrement;
	this->lastError_ = ERROR_NONE;
	return true;
}

bool PackageApi::txSetPayload(uint8_t * payload, uint8_t size)
{
	// Check for errors
	if (!this->checkInitialized()) {
		return false;
	}
	if (size == 0) {
		this->lastError_ = ERROR_BUFFER_SIZE_ZERO;
		return false;
	}
	if (size < 6) {
		this->lastError_ = ERROR_BUFFER_SIZE_TOO_SMALL;
		return false;
	}
	if (size > this->payloadMaxSize_) {
		this->lastError_ = ERROR_BUFFER_SIZE_TOO_LARGE;
		return false;
	}
	if (payload == NULL) {
		this->lastError_ = ERROR_BUFFER_NULL_POINTER;
		return false;
	}

	this->txDecodedPayloadSize_ = size;
	for (uint8_t i = 0; i < size; i++) {
		this->txDecodedPayloadData_[i] = payload[i];
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool PackageApi::txSetSourceAddress(uint8_t sourAddress)
{
	// Check for errors
	if (!this->checkInitialized()) {
		return false;
	}

	this->txDecodedSourAddress_ = sourAddress;
	this->lastError_ = ERROR_NONE;
	return true;
}

// =============================================================================
// Class private methods
// =============================================================================

bool PackageApi::checkInitialized(void)
{
	if (!this->initialized_) {
		this->lastError_ = ERROR_NOT_INITIALIZED;
		return false;
	}
	this->lastError_ = ERROR_NONE;
	return true;
}

bool PackageApi::rxProcessEscapeCharacter(void)
{
	// Escape character is not in use!
	if (!this->configEscapeEnabled_) {
		this->lastError_ = ERROR_NONE;
		return true;
	}

	// Escape character found!
	if (this->rxDecodedAuxData_ == this->configEscapeCharacter_) {
		// Check for error
		if (this->rxRawEscape_) {
			// Last character was an escape!
			this->lastError_ = ERROR_PACKAGE_API_FRAME_ERROR;
			this->rxResetPackage();
			return false;
		} else {
			// Mark as escape character
			this->rxRawEscape_ = true;
			this->rxProcessRawAddData();
			// Procedure was successful but, must indicate that the
			// function should end, because no next state must be
			// evaluated and no further data must be added to buffers
			// So, ERROR_NONE and false must be checked at caller function
			this->lastError_ = ERROR_NONE;
			return false;
		}
	}

	// Checks for misplaced start delimiter
	if (this->rxDecodedAuxData_ == this->configStartDelimiter_) {
		if (this->rxRawState_ != STATE_START_DELIMITER) {
			// Start delimiter must not happen outside STATE_START_DELIMITER
			this->lastError_ = ERROR_PACKAGE_API_FRAME_ERROR;
			this->rxResetPackage();
			return false;
		}
	}

	// Normal character found!
	if (this->rxRawEscape_) {			// It must be escaped?
		this->rxDecodedAuxData_ ^= this->configEscapeXorDecoded_;
		this->rxRawEscape_ = false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool PackageApi::rxProcessFrameFormat(void)
{
	switch (this->rxRawState_) {
	case STATE_START_DELIMITER:
		if (this->rxDecodedAuxData_ != this->configStartDelimiter_) {
			this->lastError_ = ERROR_PACKAGE_API_FRAME_ERROR;
			this->rxResetPackage();
			return false;
		}
		break;
	case STATE_PAYLOAD_SIZE:
		if (this->rxDecodedAuxData_ == 0) {
			this->lastError_ = ERROR_PACKAGE_API_FRAME_ERROR;
			this->rxResetPackage();
			return false;
		}
		break;
	case STATE_PAYLOAD_DATA:
		if ((this->rxDecodedPayloadIndex_ + 1) == this->payloadMaxSize_) {
			this->rxResetPackage();
			this->lastError_ = ERROR_BUFFER_FULL;
			return false;
		}
		break;
	case STATE_CHECKSUM:
		if (this->rxDecodedAuxData_ != this->rxRawChecksum_) {	// Checksum error
			this->rxResetPackage();
			this->lastError_ = ERROR_PACKAGE_API_CHECKSUM_ERROR;
			return false;
		}
	default:
		break;
	}

	// Everything OK
	this->lastError_ = ERROR_NONE;
	return true;
}

bool PackageApi::rxProcessNextState(void)
{
	if (this->rxRawEscape_) {
		return true;
	}

	switch (this->rxRawState_) {
	case STATE_START_DELIMITER:
		this->rxRawChecksum_ = 0xFF;
		this->rxRawState_++;
		break;
	case STATE_ID:
		this->rxDecodedId_ = this->rxDecodedAuxData_;
		this->rxRawChecksum_ -= this->rxDecodedAuxData_;
		this->rxRawState_++;
		break;
	case STATE_DESTINATION_ADDRESS:
		this->rxDecodedDestAddress_ = this->rxDecodedAuxData_;
		this->rxRawChecksum_ -= this->rxDecodedAuxData_;
		this->rxRawState_++;
		break;
	case STATE_SOURCE_ADDRESS:
		this->rxDecodedSourAddress_ = this->rxDecodedAuxData_;
		this->rxRawChecksum_ -= this->rxDecodedAuxData_;
		this->rxRawState_++;
		break;
	case STATE_PAYLOAD_SIZE:
		this->rxDecodedPayloadIndex_ = 0;
		this->rxDecodedPayloadSize_ = this->rxDecodedAuxData_;
		this->rxRawChecksum_ -= this->rxDecodedAuxData_;
		this->rxRawState_++;
		break;
	case STATE_PAYLOAD_DATA:
		this->rxDecodedPayloadData_[this->rxDecodedPayloadIndex_++] = this->rxDecodedAuxData_;
		this->rxRawChecksum_ -= this->rxDecodedAuxData_;
		if (this->rxDecodedPayloadIndex_ == this->rxDecodedPayloadSize_) {
			this->rxRawState_++;
		}
		break;
	case STATE_CHECKSUM:
		this->rxRawReady_ = true;
		this->rxRawSize_ = this->rxRawIndex_;
		this->rxRawState_ = STATE_READY;
		break;
	default:
		// Unreachable code area
		this->lastError_ = ERROR_UNKNOWN;
		this->rxResetPackage();
		return false;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

bool PackageApi::rxProcessRawAddData(void)
{
	if ((this->rxRawIndex_ + 1) == this->rawMaxSize_) {
		this->lastError_ = ERROR_BUFFER_FULL;
		this->rxResetPackage();
		return false;
	}
	this->rxRawData_[this->rxRawIndex_++] = this->rxDecodedAuxData_;
	this->lastError_ = ERROR_NONE;
	return true;
}

void PackageApi::rxResetPackage(void)
{
	rxDecodedPayloadSize_ = 0;
	rxDecodedPayloadIndex_ = 0;
	rxRawChecksum_ = 0xFF;
	rxRawSize_ = 0;
	rxRawIndex_ = 0;
	rxRawReady_ = false;
	rxRawState_ = STATE_START_DELIMITER;
	rxRawEscape_ = false;
}

bool PackageApi::txAddData(uint8_t data)
{
	// Check for errors - Package size
	if ((this->txRawIndex_ + 2) >= this->rawMaxSize_) {
		this->lastError_ = ERROR_BUFFER_FULL;
		return false;
	}

	// Escape characters DISABLED
	if (!this->configEscapeEnabled_) {
		this->txRawData_[this->txRawIndex_++] = data;
		this->lastError_ = ERROR_NONE;
		return true;
	}

	// Escape characters is ENABLED
	for (uint8_t i = 0; i < this->configEscapeCharSize_; i++) {
		if (data == this->configEscapeCharData_[i]) {
			this->txRawData_[this->txRawIndex_++] = this->configEscapeCharacter_;
			this->txRawData_[this->txRawIndex_++] = data ^ this->configEscapeXorDecoded_;
			this->lastError_ = ERROR_NONE;
			return true;
		}
	}
	this->txRawData_[this->txRawIndex_++] = data;
	this->lastError_ = ERROR_NONE;
	return true;
}

bool PackageApi::txEncodePackage(void)
{
	uint8_t checksum = 0xFF;
#if (GPDSE_DEBUG_MODULE_ALL == 1) || (GPDSE_DEBUG_MODULE_PACKAGE_API == 1)
	uint8_t debBuf[30];
	debModule_e debModule = DEB_MOD_PACKAGE_API;
	debFunctionId_e debFunctionId = DEB_FUN_PACKAGE_API_TX_ENCODE_PACKAGE;
	debugPackageApi(1);
#endif

	// Check for errors - Not initialized
	if (!this->checkInitialized()) {
#if (GPDSE_DEBUG_MODULE_ALL == 1) || (GPDSE_DEBUG_MODULE_PACKAGE_API == 1)
		debugPackageApi(2);
#endif
		return false;
	}
	// Check for errors - Zero payload size
	if (this->txDecodedPayloadSize_ == 0) {
		this->lastError_ = ERROR_BUFFER_SIZE_ZERO;
#if (GPDSE_DEBUG_MODULE_ALL == 1) || (GPDSE_DEBUG_MODULE_PACKAGE_API == 1)
		debugPackageApi(3);
#endif
		return false;
	}

	// Update data members
	this->txRawSize_ = 0;
	this->txRawIndex_ = 0;
	this->txDecodedPayloadIndex_ = 0;

	// Create raw package
	// Start delimiter
	this->txRawData_[this->txRawIndex_++] = this->configStartDelimiter_;
	checksum = 0xFF;

	// Package ID
	if (!this->txAddData(this->txDecodedId_)) {
#if (GPDSE_DEBUG_MODULE_ALL == 1) || (GPDSE_DEBUG_MODULE_PACKAGE_API == 1)
		debugPackageApi(4);
#endif
		return false;
	}
	checksum -= this->txDecodedId_;
	if (this->txDecodedIdAutoIncrement_) {
		this->txDecodedId_ = (this->txDecodedId_ == 255) ? 1 : (this->txDecodedId_ + 1);
	}

	// Destination address
	if (!this->txAddData(this->txDecodedDestAddress_)) {
#if (GPDSE_DEBUG_MODULE_ALL == 1) || (GPDSE_DEBUG_MODULE_PACKAGE_API == 1)
		debugPackageApi(5);
#endif
		return false;
	}
	checksum -= this->txDecodedDestAddress_;

	// Source address
	if (!this->txAddData(this->txDecodedSourAddress_)) {
#if (GPDSE_DEBUG_MODULE_ALL == 1) || (GPDSE_DEBUG_MODULE_PACKAGE_API == 1)
		debugPackageApi(6);
#endif
		return false;
	}
	checksum -= this->txDecodedSourAddress_;

	// Payload size
	if (!this->txAddData(this->txDecodedPayloadSize_)) {
#if (GPDSE_DEBUG_MODULE_ALL == 1) || (GPDSE_DEBUG_MODULE_PACKAGE_API == 1)
		debugPackageApi(7);
#endif
		return false;
	}
	checksum -= this->txDecodedPayloadSize_;

	// Payload data
	for (uint8_t i = 0; i < this->txDecodedPayloadSize_; i++) {
		if (!this->txAddData(this->txDecodedPayloadData_[i])) {
#if (GPDSE_DEBUG_MODULE_ALL == 1) || (GPDSE_DEBUG_MODULE_PACKAGE_API == 1)
			debugPackageApi(8);
#endif
			return false;
		}
		checksum -= this->txDecodedPayloadData_[i];
	}

	// Checksum
	if (!this->txAddData(checksum)) {
#if (GPDSE_DEBUG_MODULE_ALL == 1) || (GPDSE_DEBUG_MODULE_PACKAGE_API == 1)
		debugPackageApi(9);
#endif
		return false;
	}

	// Final package flags setup
	this->txRawSize_ = this->txRawIndex_;
	this->lastError_ = ERROR_NONE;
#if (GPDSE_DEBUG_MODULE_ALL == 1) || (GPDSE_DEBUG_MODULE_PACKAGE_API == 1)
	debugPackageApi(10);
#endif
	return true;
}

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
