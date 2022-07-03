/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			circularBuffer8.h
 * Module:			Circular Buffer Implementation for 8 bits variables
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 18, 2018
 * -------------------------------------------------------------------------- */

#ifndef __MY_LIB_AT_COMMAND_PARSER_H
#define __MY_LIB_AT_COMMAND_PARSER_H 1

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "myLibGlobalDefinitions.h"
#if __MY_LIB_GLOBAL_DEFINITIONS_H != 1
	#error Error 100 - myLibGlobalDefinitions.h - wrong build (globalDefinitions must be build 1).
#endif
#include <ctype.h>
#include <string.h>

typedef enum myLibCommand_e {
	MYLIB_CMD_NONE				= 0,
	MYLIB_CMD_RTC_GET_DATA,
	MYLIB_CMD_RTC_SET_DATA,
} myLibCommand_e;

class myLibAtCommandParser
{
public:
	myLibAtCommandParser();
	bool_t append(char character);
	void reset(void);
	myLibCommand_e lastCommand(void);
	void lastArguments(char * arguments);


//	myLibErrors_e error(void);


private:
	bool_t process(void);
	bool_t decode(void);
	bool_t strip();

	char			buffer[50];
	char			strCommand[50];
	char			strArguments[50];
	uint8_t			index;
	myLibCommand_e	command;


};

#endif
