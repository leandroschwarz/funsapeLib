/* -----------------------------------------------------------------------------
 * Project:		GPSDE++ AVR Library
 * File name:	globalConfigs.hpp
 * Module:		Global configurations for GPDSE++ AVR Library project
 * Authors:		Leadro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1001
 * Date:		May 26, 2018
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Include guards --------------------------------------------------------------

#ifndef __GLOBAL_CONFIGS_HPP
#define __GLOBAL_CONFIGS_HPP	1001

// -----------------------------------------------------------------------------
// >>>>>>>>>>>>>>>>>>>>        HOW TO EDIT THIS FILE        <<<<<<<<<<<<<<<<<<<<
// -----------------------------------------------------------------------------

/* -----------------------------------------------------------------------------
   Before stating to edit this file - READ THIS NOTES
   1 - This file hosts the main editable definitions and macro-functions in
   order to allow the portability of the library to other platforms.
   2 - This file must be unique for each project. Make a backup copy before
   stating to modify the file.
   3 - To update the project to a version of the library, you must first read
   the change.log file in the library release. The file will inform on changes
   needed to ensure the correct update of the project.
   4 - Do not edit after the indicated part on the file, under the risk of
   library unpredicted function and lost of ability to update.

   Usage:

   Constants -> values are indicated on comments when applied.
         (boolean) must take values 0 (for OFF-FALSE) or 1 (for ON-TRUE)
         (integer) must take integer values, [min;max] states allowed range
         (float) must take float point values, [min;max] states allowed range
   Macro-functions -> allows platform interoperability
         You MUST NOT CHANGE THE NUMBER OF ARGUMENTS IN MACRO-FUNCTION
         DECLARATION, however, you may insert or ignore arguments on its
         definition.
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// >>>>>>>>>>>>>>>>>>>>      EDITABLE PART BEGINS HERE      <<<<<<<<<<<<<<<<<<<<
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdio.h>

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

/* -----------------------------------------------------------------------------
 * Purpose:	System clock crystal value
 * Notes:	Used to inform the library modules about clock information.
 * Values:	(integer) [1 ; 67108863UL]
 * -------------------------------------------------------------------------- */
#define F_CPU								16000000UL

/* -----------------------------------------------------------------------------
 * Purpose:	Turns ON/OFF debug mode
 * Notes:	Provide extra functions and preprocessor messages, therefore, code
 *			generated will be larger.
 * Values:	(boolean) [0;1]
 * -------------------------------------------------------------------------- */
#define __DEBUG_MODE						1

/* -----------------------------------------------------------------------------
 * Purpose:	Select definitions file
 * Notes:	If set, user must provide his own (userDefines.hpp) file, otherwise,
 *			GPDSE++ AVR Library default global definitions file will be used.
 * Values:	(boolean) [0;1]
 * -------------------------------------------------------------------------- */
#define __USE_OWN_DEFINITIONS_FILE			0

#define TWI_BUFFER_SIZE						10

#define TWI_LIBRARY_PATH					"..\twiMaster.hpp"
#define TWI_HANDLER_CLASS					TwiMaster

#define	GPDSE_ATOMIC_BLOCK					ATOMIC_BLOCK(ATOMIC_RESTORESTATE)


// EDITABLE DEFINITIONS


// -----------------------------------------------------------------------------
// >>>>>>>>>>>>>>>>>>>>       EDITABLE PART ENDS HERE       <<<<<<<<<<<<<<<<<<<<
// -----------------------------------------------------------------------------

#define TWI_STATUS_REGISTER			TWSR
#define TWI_STATUS_PRESCALER_BIT0	TWPS0
#define TWI_STATUS_PRESCALER_BIT1	TWPS1
#define TWI_BAUD_RATE_REGISTER		TWBR
#define TWI_DATA_REGISTER			TWDR
#define TWI_CONTROL_REGISTER		TWCR
#define	TWI_CONTROL_ENABLE_BIT		TWEN
#define	TWI_CONTROL_INT_ENABLE_BIT	TWIE
#define	TWI_CONTROL_INT_FLAG_BIT	TWINT
#define	TWI_CONTROL_START_BIT		TWSTA
#define	TWI_CONTROL_STOP_BIT		TWSTO
#define	TWI_CONTROL_ENABLE_ACK_BIT	TWEA



#endif	// __GLOBAL_CONFIGS_HPP
