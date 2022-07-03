

#ifndef __PACKAGE_H
#define __PACKAGE_H 1

#include "globalDefines.h"
#include <string.h>
#include <stdlib.h>
#include "usart.h"
#include "ff.h"

#define ADDRESS_MY			0x10
#define ADDRESS_DESTINATION	0x11

#define COMMAND_SEND_STATUS		0x0A
#define COMMAND_SEND_DATA		0x10
#define COMMAND_CONFIG			0x17
#define COMMAND_RECEIVED_STATUS	0x8A
#define COMMAND_RECEIVED_DATA	0x90
#define COMMAND_CONFIG_RESULT	0x97


typedef struct package_t {
	uint8_t *	data;
	uint8_t		index	: 7;
	uint8_t		maxSize : 7;
	bool_t		ready	: 1;
	bool_t		inUse	: 1;
} package_t;

extern FIL file;

enum packageElements_t {
	PACKAGE_START		= 0,
	PACKAGE_ID			= 1,
	PACKAGE_DEST_ADDR	= 2,
	PACKAGE_SOUR_ADDR	= 3,
	PACKAGE_PAYL_SIZE	= 4,
	PACKAGE_PAYLOAD		= 5
} packageElements_t;



void packageCreate(package_t * pkg, uint8_t id, uint8_t destAddr, uint8_t sourAddr, uint8_t * payload, uint8_t paylSize);
void packageSendOk(package_t * pkg);
void packageSendErr(package_t * pkg);

#endif