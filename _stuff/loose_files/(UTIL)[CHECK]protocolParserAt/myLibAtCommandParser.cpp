/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			circularBuffer8.c
 * Module:			Circular Buffer Implementation for 8 bits variables
 * Authors:			Hazael dos Santos Batista
 *					Leandro Schwarz
 * Build:			1
 * Last edition:	March 18, 2018
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "myLibAtCommandParser.h"

#if __MY_LIB_AT_COMMAND_PARSER_H != 1
	#error Error 101 - Build mismatch on header and source code files (circularBuffer8).
#endif

// -----------------------------------------------------------------------------
// Initializes the circular buffer, allocating memory.
// -----------------------------------------------------------------------------
myLibAtCommandParser::myLibAtCommandParser()
{
	// Initialize properties
	this->index = 0;
	this->command = MYLIB_CMD_NONE;

	return;
}

bool_t myLibAtCommandParser::append(char character)
{
	bool_t validCharacter = FALSE;

	// Process characters
	if ((character >= 'a') && (character <= 'z')) {			// Lower case letters
		character = toupper(character);
		validCharacter = TRUE;
	} else if ((character >= 'A') && (character <= 'Z')) {	// Upper case letters
		validCharacter = TRUE;
	} else if ((character >= '0') && (character <= '9')) {	// Numbers
		validCharacter = TRUE;
	} else if ((character >= '(') && (character <= ')')) {	// ( )
		validCharacter = TRUE;
	} else if ((character >= ':') && (character <= '@')) {	// : ; < = > ? @
		validCharacter = TRUE;
	} else if ((character >= '[') && (character <= ']')) {	// [ \ ]
		validCharacter = TRUE;
	} else if ((character >= '{') && (character <= '}')) {	// { | }
		validCharacter = TRUE;
	} else if (character == '"') {							// "
		validCharacter = TRUE;
	} else if (character == '+') {							// +
		validCharacter = TRUE;
	} else if (character == '-') {							// -
		validCharacter = TRUE;
	}

	// Append character and return NOT READY
	if (validCharacter) {
		this->buffer[this->index++] = character;
		return FALSE;
	}

	// Process end command and return STATUS
	if (character == '\r') {
		this->buffer[this->index++] = '\0';
		return this->process();
	}

	// Ignore the rest and return NOT READY
	return FALSE;
}

void myLibAtCommandParser::reset(void)
{
	this->index = 0;
}

bool_t myLibAtCommandParser::process(void)
{
	if(!this->strip()) {
		this->reset();
		return FALSE;
	}

	if(!this->decode()) {
		this->reset();
		return FALSE;
	}

	return TRUE;
}

bool_t myLibAtCommandParser::strip(void)
{
	uint8_t bufferIndex = 0;
	uint8_t commandIndex = 0;
	uint8_t argumentIndex = 0;

	// Retrieve command
	for(bufferIndex = 0;bufferIndex < this->index;bufferIndex++) {
		if(this->buffer[bufferIndex] == '=') {
			this->strCommand[commandIndex++] = '\0';
			break;
		}
		this->strCommand[commandIndex++] = this->buffer[bufferIndex];
	}

	// Check validity
	if(strlen(this->strCommand) == 0) {
		return FALSE;
	}

	// Retrieve arguments
	for(++bufferIndex;bufferIndex < this->index;bufferIndex++) {
		this->strArguments[argumentIndex++] = this->buffer[bufferIndex];
	}
	this->strArguments[argumentIndex++] = '\0';

	return TRUE;
}

bool_t myLibAtCommandParser::decode(void)
{
	if(strcmp(this->strCommand, "ATRTC") == 0) {
		// Retrive arguments
		if(strlen(this->strArguments) == 0) {
			this->command = MYLIB_CMD_RTC_GET_DATA;
			return TRUE;
		} else if(strcmp(this->strArguments, "?") == 0) {
			this->command = MYLIB_CMD_RTC_GET_DATA;
			return TRUE;
		} else {
			this->command = MYLIB_CMD_RTC_SET_DATA;
			return TRUE;
		}
	}

	this->command = MYLIB_CMD_NONE;
	return TRUE;
}

myLibCommand_e myLibAtCommandParser::lastCommand(void)
{
	return this->command;
}

void myLibAtCommandParser::lastArguments(char * arguments)
{
	strcpy(arguments, this->strArguments);

	return;
}
