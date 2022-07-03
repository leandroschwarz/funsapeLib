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

PackageApi::PackageApi(uint16_t packageSize)
{
	// Reset data members
	this->initialized_ = false;
	this->ready_ = false;
	this->locked_ = false;
	this->data_ = NULL;
	this->index_ = 0;
	this->size_ = 0;
	this->escapeEnabled_ = false;
	this->escapeCharData_ = NULL;
	this->escapeCharSize_ = 0;
	this->startDelimiter_ = 0x7E;
	this->escapeCharacter_ = 0x7D;
	this->escapeXorValue_ = 0x20;

	// Check for errors - Package size
	if (packageSize == 0) {
		this->lastError_ = ERROR_BUFFER_SIZE_ZERO;
		return;
	} else if (packageSize < 10) {
		this->lastError_ = ERROR_BUFFER_SIZE_TOO_SMALL;
		return;
	}

	// Allocate memory
	this->data_ = (uint8_t *)calloc(packageSize, sizeof(uint8_t));
	if (this->data_ == NULL) {
		this->lastError_ = ERROR_MEMORY_ALLOCATION;
		return;
	}

	// Update data members
	this->initialized_ = true;
	this->maxSize_ = packageSize;

	this->lastError_ = ERROR_NONE;
	return;
}

// =============================================================================
// Class public methods
// =============================================================================

bool PackageApi::createNewPackage(uint8_t id, uint8_t destAddr, uint8_t sourAddr, uint8_t * payload, uint8_t payloadSize)
{
	uint8_t checksum = 0xFF;
	bool auxBool;

	// Check for errors - Not initialized
	if (!this->checkInitilized()) {
		return false;
	}
	// Check for errors - Package locked
	if (!this->checkUnlocked()) {
		return false;
	}
	// Check for errors - Zero payload size
	if (payloadSize == 0) {
		this->lastError_ = ERROR_BUFFER_SIZE_ZERO;
		return false;
	}
	// Check for errors - No payload
	if (payload == NULL) {
		this->lastError_ = ERROR_BUFFER_NULL_POINTER;
		return false;
	}
	// Check for errors - No source address
	if (sourAddr == 0) {
		this->lastError_ = ERROR_PACKAGE_API_INVALID_SOURCE_ADDRESS;
		return false;
	}
	// Total package size

	// Lock package
	this->locked_ = true;						// Masks package as "locked"
	this->ready_ = false;						// Marks package as "not ready to transfer"

	// Update data members
	this->index_ = 0;
	this->size_ = 0;

	// Start delimiter
	this->data_[this->size_++] = this->startDelimiter_;
	checksum = 0xFF;

	// Package ID
	auxBool = this->pushPackageData(id);
	if (!auxBool) {
		this->locked_ = false;
		return false;
	}
	checksum -= id;

	// Destination address
	auxBool = this->pushPackageData(destAddr);
	if (!auxBool) {
		this->locked_ = false;
		return false;
	}
	checksum -= destAddr;

	// Source address
	auxBool = this->pushPackageData(sourAddr);
	if (!auxBool) {
		this->locked_ = false;
		return false;
	}
	checksum -= sourAddr;

	// Payload size
	auxBool = this->pushPackageData(payloadSize);
	if (!auxBool) {
		this->locked_ = false;
		return false;
	}
	checksum -= payloadSize;

	// Payload data
	for (uint8_t i = 0; i < payloadSize; i++) {
		auxBool = this->pushPackageData(payload[i]);
		if (!auxBool) {
			this->locked_ = false;
			return false;
		}
		checksum -= payload[i];
	}

	// Checksum
	auxBool = this->pushPackageData(checksum);
	if (!auxBool) {
		this->locked_ = false;
		return false;
	}

	// Final package flags setup
	this->ready_ = true;						// Marks package as "ready to transfer"
	this->locked_ = false;						// Masks package as "unlocked"
	this->lastError_ = ERROR_NONE;
	return true;
}

uint8_t * PackageApi::getPackageData(void)
{
	// Check for errors - Not initialized
	if (!this->checkInitilized()) {
		return NULL;
	}
	// Check for errors - Package locked
	if (!this->checkUnlocked()) {
		return NULL;
	}
	// Check for errors - Package not ready
	if (!this->ready_) {
		return NULL;
	}

	this->lastError_ = ERROR_NONE;
	return this->data_;
}

uint16_t PackageApi::getPackageSize(void)
{
	// Check for errors - Not initialized
	if (!this->checkInitilized()) {
		return 0;
	}
	// Check for errors - Package locked
	if (!this->checkUnlocked()) {
		return 0;
	}
	// Check for errors - Package not ready
	if (!this->ready_) {
		return 0;
	}

	this->lastError_ = ERROR_NONE;
	return this->size_;
}

bool PackageApi::setEscapeCharacters(bool enable, uint8_t * charList, uint8_t listSize)
{
	// Check for errors - Not initialized
	if (!this->checkInitilized()) {
		return false;
	}
	// Check for errors - Argument mismatch
	if (!enable) {
		if ((charList != NULL) || (listSize != 0)) {
			this->lastError_ = ERROR_ARGUMENT_MISMATCH;
			return false;
		}
	} else {
		if ((charList != NULL) && (listSize == 0)) {
			this->lastError_ = ERROR_ARGUMENT_MISMATCH;
			return false;
		}
		if ((charList == NULL) && (listSize != 0)) {
			this->lastError_ = ERROR_ARGUMENT_MISMATCH;
			return false;
		}
	}

	// Destroy previous list
	if (this->escapeCharData_ != NULL) {
		free(this->escapeCharData_);
	}
	this->escapeCharData_ = NULL;
	this->escapeCharSize_ = 0;
	this->escapeEnabled_ = false;

	// If must enable escape
	if (enable) {
		// Allocate memory
		this->escapeCharData_ = (uint8_t *)calloc((listSize + 2), sizeof(uint8_t));
		if (this->escapeCharData_ == NULL) {
			this->lastError_ = ERROR_MEMORY_ALLOCATION;
			return false;
		}
		// Populate list
		this->escapeCharData_[0] = this->startDelimiter_;
		this->escapeCharData_[1] = this->escapeCharacter_;
		for (uint8_t i = 0; i < listSize; i++) {
			this->escapeCharData_[i + 2] = charList[i];
		}
		this->escapeCharSize_ = (listSize + 2);
		this->escapeEnabled_ = true;
	}

	this->lastError_ = ERROR_NONE;
	return true;
}

error_e PackageApi::getLastError(void)
{
	return this->lastError_;
}

// =============================================================================
// Class private methods
// =============================================================================

bool PackageApi::checkInitilized(void)
{
	if (!this->initialized_) {
		this->lastError_ = ERROR_NOT_INITIALIZED;
		return false;
	}

	return true;
}

bool PackageApi::checkUnlocked(void)
{
	if (this->locked_) {
		this->lastError_ = ERROR_LOCKED;
		return false;
	}

	return true;
}

bool PackageApi::checkReady(void)
{
	if (!this->ready_) {
		this->lastError_ = ERROR_PACKAGE_API_PACKAGE_NOT_READY;
		return false;
	}

	return true;
}

bool PackageApi::pushPackageData(uint8_t data)
{
	// Check for errors - Package size
	if ((this->size_ + 2) >= this->maxSize_) {
		this->lastError_ = ERROR_BUFFER_FULL;
		return false;
	}

	// Escape characters DISABLED
	if (!this->escapeEnabled_) {
		this->data_[this->size_++] = data;
		return true;
	}

	// Escape characters is ENABLED
	for (uint8_t i = 0; i < this->escapeCharSize_; i++) {
		if (data == this->escapeCharData_[i]) {
			this->data_[this->size_++] = this->escapeCharacter_;
			this->data_[this->size_++] = data ^ this->escapeXorValue_;
			return true;
		}
	}
	this->data_[this->size_++] = data;
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
