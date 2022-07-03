/* -----------------------------------------------------------------------------
 * Project:		GPSDE++ AVR Library
 * File name:	XXXX.cpp
 * Module:		XXXXX XXXXXX for GPDSE++ AVR Library project
 * Authors:		Leadro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1001
 * Date:		May 26, 2018
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "globalConfigs.hpp"
#ifndef __GLOBAL_CONFIGS_HPP
#	error	[twiMaster.hpp] Error 100 - System configuration file (globalConfigs.hpp) is missing
#else
#	if __GLOBAL_CONFIGS_HPP != 1001
#		error 	[twiMaster.hpp] Error 101 - Build mismatch (globalConfigs.hpp and twiMaster.hpp)
#	endif		// __GLOBAL_CONFIGS_HPP != 1001
#endif		// __GLOBAL_CONFIGS_HPP

#if __USE_OWN_DEFINITIONS_FILE == 1
#	include "userDefines.hpp"
#else
#	include "globalDefines.hpp"
#	if __GLOBAL_DEFINES_HPP != 1001
#		error 	[twiMaster.hpp] Error 102 - Build mismatch (globalDefines.hpp and twiMaster.hpp)
#	endif		// __GLOBAL_CONFIGS_HPP != 1001
#endif		// __USE_OWN_DEFINITIONS_FILE == 1

#include "twiMaster.hpp"
#if __TWI_MASTER_HPP != 1001
#	error [twiMaster.cpp] Error 103 - Build mismatch on header and source code files.
#endif

// -----------------------------------------------------------------------------
// File exclusive - Constant definitions ---------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// File exclusive - New data types ---------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// File exclusive - Macro-functions definitions --------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// XXXXXXX class - Constructors definitions ------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// XXXXXXX class - Public methods definitions ----------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// XXXXXXX class - Private methods definitions ---------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// XXXXXXX class - Protected methods definitions -------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// General public functions definitions ----------------------------------------

/* NONE */
