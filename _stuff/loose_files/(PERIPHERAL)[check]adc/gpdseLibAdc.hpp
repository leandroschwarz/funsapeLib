/* =============================================================================
 * Project:		GPSDE++ Library
 * File name:	gpdseLibAdc.hpp
 * Module:		System definitions for GPDSE++ Library project
 * Authors:		Leandro Schwarz
 * 				Mayara de Sousa
 * Build:		__BUILD_MACRO_TO_BE_CHANGED__
 * Date:		__DATE_MACRO_TO_BE_CHANGED__
 * ========================================================================== */

// =============================================================================
// File build
// =============================================================================

#ifndef __GPDSE_LIB_ADC_HPP
#define __GPDSE_LIB_ADC_HPP						__BUILD_MACRO_TO_BE_CHANGED__

// =============================================================================
// System file dependencies
// =============================================================================

#include <gpdseLibGlobalDefines.hpp>
#if __GPDSE_LIB_GLOBAL_DEFINES_HPP != __BUILD_MACRO_TO_BE_CHANGED__
#	error	Global definitions file (gpdseLibGlobalDefines) must be build __BUILD_MACRO_TO_BE_CHANGED__.
#endif

// =============================================================================
// Constants
// =============================================================================

// -----------------------------------------------------------------------------
// Undefining previous definitions
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Defining new constants
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Defining macro-functions
// -----------------------------------------------------------------------------

// NONE

// =============================================================================
// New data types
// =============================================================================

// -----------------------------------------------------------------------------
// Single variables
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations - Logic values
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations - Finite lists
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations - Expansible lists
// -----------------------------------------------------------------------------

typedef enum adcClockPrescaler_e {
	ADC_CLOCK_PRESCALER_UNDEFINED								= 0,
	ADC_CLOCK_PRESCALER_OFF										= 1,
	ADC_CLOCK_PRESCALER_2										= 2,
	ADC_CLOCK_PRESCALER_4										= 3,
	ADC_CLOCK_PRESCALER_8										= 4,
	ADC_CLOCK_PRESCALER_16										= 5,
	ADC_CLOCK_PRESCALER_32										= 6,
	ADC_CLOCK_PRESCALER_64										= 7,
	ADC_CLOCK_PRESCALER_128										= 8,
} adcClockPrescaler_e;

typedef enum adcVoltageReference_e {
	ADC_VOLTAGE_REFERENCE_UNDEFINED								= 0,
#if ADC_VOLTAGE_REFERENCE_1_10_V_CAP_PB0_EXISTS
	ADC_VOLTAGE_REFERENCE_1_10_V								= 1,
#endif
#if ADC_VOLTAGE_REFERENCE_1_10_V_EXISTS
	ADC_VOLTAGE_REFERENCE_1_10_V								= 2,
#endif
#if ADC_VOLTAGE_REFERENCE_2_56_V_EXISTS
	ADC_VOLTAGE_REFERENCE_1_10_V								= 3,
#endif
#if ADC_VOLTAGE_REFERENCE_AREF_EXISTS
	ADC_VOLTAGE_REFERENCE_AREF									= 4,
#endif
#if ADC_VOLTAGE_REFERENCE_AVCC_EXISTS
	ADC_VOLTAGE_REFERENCE_AVCC									= 5,
#endif
} adcVoltageReference_e;

typedef enum adcChannel_e {
	ADC_CHANNEL_UNDEFINED										= 0,
#if ADC_CHANNEL_BANDGAP_1100_MV_EXISTS
	ADC_CHANNEL_BANDGAP_1100_MV									= 1,
#endif
#if ADC_CHANNEL_BANDGAP_2560_MV_EXISTS
	ADC_CHANNEL_BANDGAP_2560_MV									= 2,
#endif
#if ADC_CHANNEL_DIFERENTIAL_1_0_X10_EXISTS
	ADC_CHANNEL_DIFERENTIAL_1_0_X10								= 3,
#endif
#if ADC_CHANNEL_DIFERENTIAL_1_0_X200_EXISTS
	ADC_CHANNEL_DIFERENTIAL_1_0_X200							= 4,
#endif
#if ADC_CHANNEL_DIFERENTIAL_3_2_X10_EXISTS
	ADC_CHANNEL_DIFERENTIAL_3_2_X10								= 5,
#endif
#if ADC_CHANNEL_DIFERENTIAL_3_2_X200_EXISTS
	ADC_CHANNEL_DIFERENTIAL_3_2_X200							= 6,
#endif
#if ADC_CHANNEL_DIFERENTIAL_0_1_X1_EXISTS
	ADC_CHANNEL_DIFERENTIAL_0_1_X1								= 7,
#endif
#if ADC_CHANNEL_DIFERENTIAL_2_1_X1_EXISTS
	ADC_CHANNEL_DIFERENTIAL_2_1_X1								= 8,
#endif
#if ADC_CHANNEL_DIFERENTIAL_3_1_X1_EXISTS
	ADC_CHANNEL_DIFERENTIAL_3_1_X1								= 9,
#endif
#if ADC_CHANNEL_DIFERENTIAL_4_1_X1_EXISTS
	ADC_CHANNEL_DIFERENTIAL_4_1_X1								= 10,
#endif
#if ADC_CHANNEL_DIFERENTIAL_5_1_X1_EXISTS
	ADC_CHANNEL_DIFERENTIAL_5_1_X1								= 11,
#endif
#if ADC_CHANNEL_DIFERENTIAL_6_1_X1_EXISTS
	ADC_CHANNEL_DIFERENTIAL_6_1_X1								= 12,
#endif
#if ADC_CHANNEL_DIFERENTIAL_7_1_X1_EXISTS
	ADC_CHANNEL_DIFERENTIAL_7_1_X1								= 13,
#endif
#if ADC_CHANNEL_DIFERENTIAL_0_2_X1_EXISTS
	ADC_CHANNEL_DIFERENTIAL_0_2_X1								= 14,
#endif
#if ADC_CHANNEL_DIFERENTIAL_1_2_X1_EXISTS
	ADC_CHANNEL_DIFERENTIAL_1_2_X1								= 15,
#endif
#if ADC_CHANNEL_DIFERENTIAL_3_2_X1_EXISTS
	ADC_CHANNEL_DIFERENTIAL_3_2_X1								= 16,
#endif
#if ADC_CHANNEL_DIFERENTIAL_4_2_X1_EXISTS
	ADC_CHANNEL_DIFERENTIAL_4_2_X1								= 17,
#endif
#if ADC_CHANNEL_DIFERENTIAL_5_2_X1_EXISTS
	ADC_CHANNEL_DIFERENTIAL_5_2_X1								= 18,
#endif
#if ADC_CHANNEL_GND_EXISTS
	ADC_CHANNEL_GND												= 19,
#endif
#if ADC_CHANNEL_SINGLE_0_EXISTS
	ADC_CHANNEL_SINGLE_0										= 20,
#endif
#if ADC_CHANNEL_SINGLE_1_EXISTS
	ADC_CHANNEL_SINGLE_1										= 21,
#endif
#if ADC_CHANNEL_SINGLE_2_EXISTS
	ADC_CHANNEL_SINGLE_2										= 22,
#endif
#if ADC_CHANNEL_SINGLE_3_EXISTS
	ADC_CHANNEL_SINGLE_3										= 23,
#endif
#if ADC_CHANNEL_SINGLE_4_EXISTS
	ADC_CHANNEL_SINGLE_4										= 24,
#endif
#if ADC_CHANNEL_SINGLE_5_EXISTS
	ADC_CHANNEL_SINGLE_5										= 25,
#endif
#if ADC_CHANNEL_SINGLE_6_EXISTS
	ADC_CHANNEL_SINGLE_6										= 26,
#endif
#if ADC_CHANNEL_SINGLE_7_EXISTS
	ADC_CHANNEL_SINGLE_7										= 27,
#endif
#if ADC_CHANNEL_TEMPERATURE_EXISTS
	ADC_CHANNEL_TEMPERATURE										= 28,
#endif
} adcChannel_e;

typedef enum adcMode_e {
	ADC_MODE_UNDEFINED											= 0,
	ADC_MODE_SINGLE_CONVERSION									= 1,
#if ADC_MODE_AUTO_ANALOG_COMPARATOR_EXISTS
	ADC_MODE_AUTO_ANALOG_COMPARATOR								= 2,
#endif
#if ADC_MODE_AUTO_FREE_RUNNING_EXISTS
	ADC_MODE_AUTO_FREE_RUNNING									= 3,
#endif
#if ADC_MODE_AUTO_INT0_EXISTS
	ADC_MODE_AUTO_INT0											= 4,
#endif
#if ADC_MODE_AUTO_PCINT_EXISTS
	ADC_MODE_AUTO_PCINT											= 5,
#endif
#if ADC_MODE_AUTO_TIMER0_COMP_A_EXISTS
	ADC_MODE_AUTO_TIMER0_COMP_A									= 6,
#endif
#if ADC_MODE_AUTO_TIMER0_OVF_EXISTS
	ADC_MODE_AUTO_TIMER0_OVF									= 7,
#endif
#if ADC_MODE_AUTO_TIMER1_CAPTURE_EXISTS
	ADC_MODE_AUTO_TIMER1_CAPTURE								= 8,
#endif
#if ADC_MODE_AUTO_TIMER1_COMP_B_EXISTS
	ADC_MODE_AUTO_TIMER1_COMP_B									= 9,
#endif
#if ADC_MODE_AUTO_TIMER1_OVF_EXISTS
	ADC_MODE_AUTO_TIMER1_OVF									= 10,
#endif
#if ADC_MODE_AUTO_TIMER4_COMP_A_EXISTS
	ADC_MODE_AUTO_TIMER4_COMP_A									= 11,
#endif
#if ADC_MODE_AUTO_TIMER4_COMP_B_EXISTS
	ADC_MODE_AUTO_TIMER4_COMP_B									= 12,
#endif
#if ADC_MODE_AUTO_TIMER4_COMP_D_EXISTS
	ADC_MODE_AUTO_TIMER4_COMP_D									= 13,
#endif
#if ADC_MODE_AUTO_TIMER4_OVF_EXISTS
	ADC_MODE_AUTO_TIMER4_OVF									= 14,
#endif
#if ADC_MODE_AUTO_TIMER_COMP_A_EXISTS
	ADC_MODE_AUTO_TIMER_COMP_A									= 15,
#endif
#if ADC_MODE_AUTO_TIMER_COMP_B_EXISTS
	ADC_MODE_AUTO_TIMER_COMP_B									= 16,
#endif
#if ADC_MODE_AUTO_TIMER_OVF_EXISTS
	ADC_MODE_AUTO_TIMER_OVF										= 17,
#endif
} adcMode_e;

// -----------------------------------------------------------------------------
// Enumerations - Lists that can be algebraically manipulated
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Enumerations - Lists that can be manipulated using bitwise operations
// -----------------------------------------------------------------------------

typedef enum adcDigitalInputs_f {
#if ADC_DISABLE_DIGITAL_INPUT_REG0_EXISTS
#	if ADC_DISABLE_DIGITAL_INPUT_0_EXISTS
	ADC_DIGITAL_INPUT_0									= (1 << 0),
#	endif
#	if ADC_DISABLE_DIGITAL_INPUT_1_EXISTS
	ADC_DIGITAL_INPUT_1									= (1 << 1),
#	endif
#	if ADC_DISABLE_DIGITAL_INPUT_2_EXISTS
	ADC_DIGITAL_INPUT_2									= (1 << 2),
#	endif
#	if ADC_DISABLE_DIGITAL_INPUT_3_EXISTS
	ADC_DIGITAL_INPUT_3									= (1 << 3),
#	endif
#	if ADC_DISABLE_DIGITAL_INPUT_4_EXISTS
	ADC_DIGITAL_INPUT_4									= (1 << 4),
#	endif
#	if ADC_DISABLE_DIGITAL_INPUT_5_EXISTS
	ADC_DIGITAL_INPUT_5									= (1 << 5),
#	endif
#	if ADC_DISABLE_DIGITAL_INPUT_6_EXISTS
	ADC_DIGITAL_INPUT_6									= (1 << 6),
#	endif
#	if ADC_DISABLE_DIGITAL_INPUT_7_EXISTS
	ADC_DIGITAL_INPUT_7									= (1 << 7),
#	endif
#endif
#if ADC_DISABLE_DIGITAL_INPUT_REG2_EXISTS
#	if ADC_DISABLE_DIGITAL_INPUT_8_EXISTS
	ADC_DIGITAL_INPUT_8									= (1 << 8),
#	endif
#	if ADC_DISABLE_DIGITAL_INPUT_9_EXISTS
	ADC_DIGITAL_INPUT_9									= (1 << 9),
#	endif
#	if ADC_DISABLE_DIGITAL_INPUT_10_EXISTS
	ADC_DIGITAL_INPUT_10								= (1 << 10),
#	endif
#	if ADC_DISABLE_DIGITAL_INPUT_11_EXISTS
	ADC_DIGITAL_INPUT_11								= (1 << 11),
#	endif
#	if ADC_DISABLE_DIGITAL_INPUT_12_EXISTS
	ADC_DIGITAL_INPUT_12								= (1 << 12),
#	endif
#	if ADC_DISABLE_DIGITAL_INPUT_13_EXISTS
	ADC_DIGITAL_INPUT_13								= (1 << 13),
#	endif
#	if ADC_DISABLE_DIGITAL_INPUT_14_EXISTS
	ADC_DIGITAL_INPUT_14								= (1 << 14),
#	endif
#	if ADC_DISABLE_DIGITAL_INPUT_15_EXISTS
	ADC_DIGITAL_INPUT_15								= (1 << 15),
#	endif
#endif
	ADC_DIGITAL_INPUT_ALL								= ADC_DISABLE_DIGITAL_INPUT_MASK,
} adcDigitalInputs_f;

inlined adcDigitalInputs_f operator|(adcDigitalInputs_f a, adcDigitalInputs_f b)
{
	return static_cast<adcDigitalInputs_f>(static_cast<int>(a) | static_cast<int>(b));
}

inlined adcDigitalInputs_f& operator|=(adcDigitalInputs_f& a, adcDigitalInputs_f b)
{
	return a = a | b;
}

inlined adcDigitalInputs_f operator&(adcDigitalInputs_f a, adcDigitalInputs_f b)
{
	return static_cast<adcDigitalInputs_f>(static_cast<int>(a) & static_cast<int>(b));
}

inlined adcDigitalInputs_f& operator&=(adcDigitalInputs_f& a, adcDigitalInputs_f b)
{
	return a = a & b;
}

inlined adcDigitalInputs_f operator~(adcDigitalInputs_f a)
{
	return static_cast<adcDigitalInputs_f>(static_cast<int>(ADC_DIGITAL_INPUT_ALL) ^ static_cast<int>(a));
}

// =============================================================================
// Global variable declaration
// =============================================================================

// NONE

// =============================================================================
// Class declaration
// =============================================================================

// -----------------------------------------------------------------------------
// Class Adc - Start
// -----------------------------------------------------------------------------

#if ADC_EXISTS
class Adc
{
	// -------------------------------------------------------------------------
	// New data types
	// -------------------------------------------------------------------------

	// -------------------------------------------------------------------------
	// New data declaration ----------------------------------------------------
	// NONE

	// -------------------------------------------------------------------------
	// New data overloading operators ------------------------------------------
	// NONE

public:
	// -------------------------------------------------------------------------
	// Constructors
	// -------------------------------------------------------------------------

	Adc(	adcMode_e				mode		= ADC_MODE_SINGLE_CONVERSION,
			adcVoltageReference_e	reference	= ADC_VOLTAGE_REFERENCE_EXTERNAL
	   );
	~Adc(	void);

public:
	// -------------------------------------------------------------------------
	// Public methods
	// -------------------------------------------------------------------------

	//     ///////////////////     INITIALIZATION     ///////////////////     //
	bool deinitialize(		void);
	bool initialize(		void);
	bool startConversion(	void);

	//     ////////////////////     CHECK STATUS     ////////////////////     //
	error_e getLastError(	void);
	bool isDataReady(		void);
	bool isBusy(			void);

	//     ////////////////////    CONFIGURATION     ////////////////////     //
	bool setMode(			adcMode_e				mode);
	bool setReference(		adcVoltageReference_e	reference);
	bool selectChannel(		adcChannel_e			channel);
	bool setDigitalInput(	adcDigitalInputs_f flagInputs, bool enabled);

	//     //////////////////////    INTERRUPT     //////////////////////     //
	bool clearInterruptRequest(	void);
	bool setInterrupt(			bool enabled);




	//     //////////////     COMMUNICATION  INTERFACE     //////////////     //
	//     ///////////////////     DATA STREAMING     ///////////////////     //
	//     /////////////////////     INTERRUPTS     /////////////////////     //
	//     ////////////////     PROTOCOL  PARAMETERS     ////////////////     //
	//     ///////////////////////     TO  DO     ///////////////////////     //

private:
	// -------------------------------------------------------------------------
	// Private methods
	// -------------------------------------------------------------------------

	//     ////////////////////     CHECK STATUS     ////////////////////     //
	//     ////////////////     DATA MEMBER UPDATING     ////////////////     //
	//     //////////////////     DATA  VALIDATION     //////////////////     //
	//     ////////////////////     I/O MANAGING     ////////////////////     //
	//     //////////////////    REGISTER MANAGING     //////////////////     //
	//     ///////////////////////     TO  DO     ///////////////////////     //
	// NONE

protected:
	// -------------------------------------------------------------------------
	// Protected methods
	// -------------------------------------------------------------------------

	//     ///////////////////////     TO  DO     ///////////////////////     //
	// NONE

private:
	// -------------------------------------------------------------------------
	// Private properties
	// -------------------------------------------------------------------------

	//     ///////////////////     INITIALIZATION     ///////////////////     //
	bool				_initialized					: 1;
	bool				_deviceEnabled					: 1;

	//     ////////////////////     CHECK STATUS     ////////////////////     //
	error_e				_lastError;

	//     /////////////////////     INTERRUPTS     /////////////////////     //
	bool				_intDeviceReadyEnabled			: 1;

	//     ////////////////////    CONFIGURATION     ////////////////////     //
	//     /////////////////     INTERNAL  CONTROLS     /////////////////     //
	//     ////////////////     INTERNAL DATA BUFFER     ////////////////     //
	//     /////////////////////     INTERRUPTS     /////////////////////     //
	//     ///////////////     PROTOCOL CONFIGURATION     ///////////////     //
	//     ////////////////     PROTOCOL  PARAMETERS     ////////////////     //
	//     ///////////////////////     TO  DO     ///////////////////////     //

protected:
	// -------------------------------------------------------------------------
	// Protected properties
	// -------------------------------------------------------------------------

	//     ///////////////////////     TO  DO     ///////////////////////     //
	// NONE

}; // class Adc

// -----------------------------------------------------------------------------
// Class Adc - Class overloading operators
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Class Adc - Class inline functions definitions
// -----------------------------------------------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Class Adc - End
// -----------------------------------------------------------------------------

#endif // ADC_EXISTS

// -----------------------------------------------------------------------------
// Class PeripheralName1 - Start
// -----------------------------------------------------------------------------

#if PERIPHERAL_NAME_1_EXISTS

// -----------------------------------------------------------------------------
// Class Adc - End
// -----------------------------------------------------------------------------

#endif // PERIPHERAL_NAME_1_EXISTS

// =============================================================================
// General public functions declarations
// =============================================================================

// NONE

// =============================================================================
// General inline functions definitions
// =============================================================================

// NONE

// =============================================================================
// External default objects
// =============================================================================

// NONE

#endif // __GPDSE_LIB_ADC_HPP
