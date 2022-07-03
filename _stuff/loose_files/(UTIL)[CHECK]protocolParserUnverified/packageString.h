/*
 * packageString.h
 *
 * Created: 10/04/2018 15:53:44
 *  Author: Schwarz
 */


#ifndef PACKAGE_STRING_H_
#define PACKAGE_STRING_H_ 1

#include "globalDefines.h"
#include <stdlib.h>
#include <avr/pgmspace.h>

typedef enum packageStringError_t {
	PKG_STR_OK,
	PKG_STR_NOT_INITIALIZED,
	PKG_STR_DATA_BUFFER_SIZE_ZERO,
	PKG_STR_DATA_BUFFER_MEMORY_ERROR,
	PKG_STR_BUSY_BUFFER_MEMORY_ERROR,
	PKG_STR_IGNORE_SPACE,
	PKG_STR_IGNORE_CHARACTER,
	PKG_STR_DATA_BUFFER_FULL,
	PKG_STR_DATA_BUFFER_EMPTY,
	PKG_STR_DATA_WAITING,
	PKG_STR_UKNOWN_ERROR						= 255
} packageStringError_t;

typedef struct packageString_t {
	char *					dataBuffer;
	uint16_t				dataBufferMaxSize;
	uint16_t				dataBufferIndex;
	bool_t					dataBufferReady		: 1;
	bool_t					dataBufferBusy		: 1;
	char *					busyBuffer;
	uint16_t				busyBufferMaxSize;
	uint16_t				busyBufferIndex;
	bool_t					initialized			: 1;
	uint8_t					unusedBits			: 5;
	packageStringError_t	lastError;
} packageString_t;

packageStringError_t packageStringCreate(packageString_t * pkg, uint16_t dataBuffSize, uint16_t busyBuffSize);
const char * packageStringVerboseError(packageString_t * pkg);
packageStringError_t packageStringGetByte(packageString_t * pkg, uint8_t byte);
bool_t packageStringIsReady(packageString_t * pkg);
char * packageStringReturnString(packageString_t * pkg);
packageStringError_t packageStringResetPackage(packageString_t * pkg);

#endif
