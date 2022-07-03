/*
 * packageString.c
 *
 * Created: 10/04/2018 15:53:55
 *  Author: Schwarz
 */


#include "packageString.h"

packageStringError_t packageStringCreate(packageString_t * pkg, uint16_t dataBuffSize, uint16_t busyBuffSize)
{
	// Reset package value
	pkg->initialized = FALSE;		// First thing. Makes package unusable for a while
	pkg->lastError = PKG_STR_NOT_INITIALIZED;
	pkg->unusedBits = 0;
	pkg->dataBufferIndex = 0;
	pkg->dataBufferMaxSize = 0;
	pkg->dataBufferReady = FALSE;
	pkg->dataBufferBusy = FALSE;
	pkg->busyBufferIndex = 0;
	pkg->busyBufferMaxSize = 0;
	pkg->dataBuffer = NULL;
	pkg->busyBuffer = NULL;

	// Error check - size of dataBuffer is 0
	if(dataBuffSize == 0) {
		pkg->lastError = PKG_STR_DATA_BUFFER_SIZE_ZERO;
		return PKG_STR_DATA_BUFFER_SIZE_ZERO;
	}
	pkg->dataBuffer = (char *)calloc(dataBuffSize, sizeof(char));
	// Error check - memory allocation fail
	if(pkg->dataBuffer == NULL) {
		pkg->lastError = PKG_STR_DATA_BUFFER_MEMORY_ERROR;
		return PKG_STR_DATA_BUFFER_MEMORY_ERROR;
	}
	pkg->dataBufferMaxSize = dataBuffSize;

	// If busy buffer is enabled
	if(busyBuffSize != 0) {
		pkg->busyBuffer = (char *)calloc(busyBuffSize, sizeof(char));
		// Error check - memory allocation fail
		if(pkg->busyBuffer == NULL) {
			free(pkg->dataBuffer);
			pkg->dataBufferMaxSize = 0;
			pkg->dataBuffer = NULL;
			pkg->lastError = PKG_STR_DATA_BUFFER_MEMORY_ERROR;
			return PKG_STR_DATA_BUFFER_MEMORY_ERROR;
		}
		pkg->dataBufferMaxSize = dataBuffSize;
	}
	pkg->initialized = TRUE;
	pkg->lastError = PKG_STR_OK;

	return PKG_STR_OK;
}

packageStringError_t packageStringGetByte(packageString_t * pkg, uint8_t byte)
{
	// Check for initialization procedure
	if(!pkg->initialized) {
		pkg->lastError = PKG_STR_NOT_INITIALIZED;
		return PKG_STR_NOT_INITIALIZED;
	}

	// There is data waiting
	if(pkg->dataBufferReady) {
		pkg->lastError = PKG_STR_DATA_WAITING;
		return PKG_STR_DATA_WAITING;
	}

// COMANDS - WILL NOT COUNT AS DATA

	// Process BACKSPACE key
	if(byte == 0x08) {
		if(pkg->dataBufferIndex == 0) {								// There is nothing to delete
			pkg->lastError = PKG_STR_DATA_BUFFER_EMPTY;
			return PKG_STR_DATA_BUFFER_EMPTY;
		} else {													// Delete a character
			pkg->dataBufferIndex--;
			pkg->lastError = PKG_STR_OK;
			return PKG_STR_OK;
		}
	}

	// Process ESC key
	if(byte == 0x1B) {												// RESET string
		pkg->dataBufferIndex = 0;
		pkg->lastError = PKG_STR_OK;
		return PKG_STR_OK;
	}

// DATA - MUST BE INCREMENTED
	// Checks if buffer is full
	if((pkg->dataBufferIndex + 1) == pkg->dataBufferMaxSize) {
		pkg->lastError = PKG_STR_DATA_BUFFER_FULL;
		return PKG_STR_DATA_BUFFER_FULL;
	}

	// Process ENTER key
	if(byte == 0x0D) {
		pkg->dataBufferReady = TRUE;
		if(pkg->dataBufferIndex != 0) {								// Take the last space out
			if(pkg->dataBuffer[pkg->dataBufferIndex - 1] == 0x20) {
				pkg->dataBufferIndex--;
			}
		}
		pkg->dataBuffer[pkg->dataBufferIndex] = '\0';
		pkg->lastError = PKG_STR_OK;
		return PKG_STR_OK;
	}

// Process SPACE key
	if(byte == 0x20) {
		if(pkg->dataBufferIndex == 0) {									// Trim string
			pkg->lastError = PKG_STR_IGNORE_SPACE;
			return PKG_STR_IGNORE_SPACE;
		} else if(pkg->dataBuffer[pkg->dataBufferIndex - 1] == byte) {	// Ignore space
			pkg->lastError = PKG_STR_IGNORE_SPACE;
			return PKG_STR_IGNORE_SPACE;
		} else {														// OK, process key
			pkg->dataBuffer[pkg->dataBufferIndex++] = byte;
			pkg->lastError = PKG_STR_OK;
			return PKG_STR_OK;
		}
	}

// Process valid text key
	if((byte > 0x20) && (byte < 0x7F)) {
		pkg->dataBuffer[pkg->dataBufferIndex++] = byte;
		pkg->lastError = PKG_STR_OK;
		return PKG_STR_OK;
	}

// Ignores the rest
	pkg->lastError = PKG_STR_IGNORE_CHARACTER;
	return PKG_STR_IGNORE_CHARACTER;
}

const char * packageStringVerboseError(packageString_t * pkg)
{
	switch(pkg->lastError) {
	case PKG_STR_OK:
		return PSTR("No error detected");
	case PKG_STR_NOT_INITIALIZED:
		return PSTR("Package struct not initialized");
	case PKG_STR_DATA_BUFFER_SIZE_ZERO:
		return PSTR("Data buffer size cannot be 0");
	case PKG_STR_DATA_BUFFER_MEMORY_ERROR:
		return PSTR("Not enough memory to allocate data buffer");
	case PKG_STR_BUSY_BUFFER_MEMORY_ERROR:
		return PSTR("Not enough memory to allocate busy buffer");
	case PKG_STR_IGNORE_SPACE:
		return PSTR("A SPACE key press was ignored");
	case PKG_STR_IGNORE_CHARACTER:
		return PSTR("A character was ignored");
	case PKG_STR_DATA_BUFFER_FULL:
		return PSTR("Data buffer was full");
	case PKG_STR_DATA_BUFFER_EMPTY:
		return PSTR("Data buffer was empty");
	case PKG_STR_DATA_WAITING:
		return PSTR("Couldn't process - a data is waiting to be processed");
	case PKG_STR_UKNOWN_ERROR:
		return PSTR("Some strange unknown error has occurred");
	}

	pkg->lastError = PKG_STR_OK;

	return NULL;
}

char * packageStringReturnString(packageString_t * pkg)
{
	pkg->lastError = PKG_STR_OK;
	return pkg->dataBuffer;
}

bool_t packageStringIsReady(packageString_t * pkg)
{
	pkg->lastError = PKG_STR_OK;
	return pkg->dataBufferReady;
}

packageStringError_t packageStringResetPackage(packageString_t * pkg)
{
	pkg->dataBufferIndex = 0;
	pkg->dataBufferReady = FALSE;

	pkg->lastError = PKG_STR_OK;
	return PKG_STR_OK;
}

