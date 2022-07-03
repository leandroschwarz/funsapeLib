/*!
	\file		circularBuffer16.h
	\brief		Circular buffer of uint16_t variables implementation for the GPDSE AVR8 Integrated Library
	\author		Hazael dos Santos Batista
	\author		Leandro Schwarz
	\date		May 16, 2018
*/

#ifndef __CIRCULAR_BUFFER_16_H
#define __CIRCULAR_BUFFER_16_H 2

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "..\globalDefines.h"
#if __GLOBAL_DEFINES_H != 2
	#error Error 100 - globalDefines.h - wrong build (globalDefines must be build 2).
#endif	// #if __GLOBAL_DEFINES_H != 2
#include <stdlib.h>

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

/*!
	\struct			circularBuffer16_s
		\details	Circular buffer configuration struct
*/
typedef struct circularBuffer16_s {
	uint16_t nextRead;
	uint16_t nextWrite;
	uint16_t size;
	uint16_t occupation;
	uint16_t * data;
} circularBuffer16_s;

// -----------------------------------------------------------------------------
// Macro-functions -------------------------------------------------------------

#define circularBuffer16Create() (circularBuffer16_s){.nextRead = 0, .nextWrite = 0, .size = 0, .occupation = 0, .data = NULL}

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

/*!
	\fn		circularBuffer16Init(circularBuffer16_s * buffer, uint16_t bufferSize)
		\details	Initializes the circular buffer
		\param		buffer		pointer to buffer struct
		\param		bufferSize	number of storage elements
		\return					TRUE or FALSE
*/
bool_t	circularBuffer16Init(circularBuffer16_s * buffer, uint16_t bufferSize);

/*!
	\fn		circularBuffer16PushData(circularBuffer16_s * buffer, uint16_t data)
		\details	Pushes data into circular buffer
		\param		buffer		pointer to buffer struct
		\param		data		data to be stored
		\return					TRUE or FALSE
*/
bool_t	circularBuffer16PushData(circularBuffer16_s * buffer, uint16_t data);

/*!
	\fn		circularBuffer16PopData(circularBuffer16_s * buffer, uint16_t * data)
		\details	Pops data from circular buffer
		\param		buffer		pointer to buffer struct
		\param		data		pointer to data read
		\return					TRUE or FALSE
*/
bool_t	circularBuffer16PopData(circularBuffer16_s * buffer, uint16_t * data);

/*!
	\fn		circularBuffer16IsEmpty(circularBuffer16_s * buffer)
		\details	Verifies if the circular buffer is empty
		\param		buffer		pointer to buffer struct
		\return					TRUE or FALSE
*/
bool_t	circularBuffer16IsEmpty(circularBuffer16_s * buffer);

/*!
	\fn		circularBuffer16GetOccupation(circularBuffer16_s * buffer)
		\details	Retrieves the number of elements in buffer
		\param		buffer		pointer to buffer struct
		\return					number of elements in buffer
*/
uint16_t	circularBuffer16GetOccupation(circularBuffer16_s * buffer);

#endif	// #ifndef __CIRCULAR_BUFFER_16_H
