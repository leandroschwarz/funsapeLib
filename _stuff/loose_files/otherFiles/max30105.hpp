/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		max30105.hpp
 * Module:			MAX30105 module interface for FunSAPE++ Embedded Library
 * 					project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __MAX30105_HPP
#define __MAX30105_HPP							__BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#	error	[max30105.hpp] Error 9 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[max30105.hpp] Error 10 - Build mismatch between (max30105.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "../util/funsapeLibHandler.hpp"
#ifndef __FUNSAPE_LIB_HANDLER_HPP
#	error	[max30105.hpp] Error 1 - Header file (funsapeLibHandler.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_HANDLER_HPP != __BUILD_TO_REPLACE__
#	error	[max30105.hpp] Error 2 - Build mismatch between (max30105.hpp) and (funsapeLibHandler.hpp) header files!
#endif
#include "../util/funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#	error	[max30105.hpp] Error 1 - Header file (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_TO_REPLACE__
#	error	[max30105.hpp] Error 2 - Build mismatch between (max30105.hpp) and (funsapeLibDebug.hpp) header files!
#endif

// =============================================================================
// Undefining previous definitions
// =============================================================================

// NONE

// =============================================================================
// Constant definitions
// =============================================================================

#define TWI_SPEED_STANDARD				100000
#define TWI_SPEED_FAST					400000
#define TWI_BUFFER_LENGTH				32

// =============================================================================
// Macro-function definitions
// *INDENT-OFF*
// =============================================================================

// NONE

// *INDENT-ON*

// =============================================================================
// New data types
// =============================================================================

// NONE

// =============================================================================
// Extern global variables
// =============================================================================

// NONE
void showError(uint8_t errorCode);

// =============================================================================
// max30105 - Class declaration
// =============================================================================

class Max30105
{
	// -------------------------------------------------------------------------
	// New data types
	// -------------------------------------------------------------------------

public:
	//     ////////////////////    FIFO AVERAGES     ////////////////////     //
	enum class FifoAveraging {
		FIFO_NO_AVERAGE					= 0,
		FIFO_AVERAGE_2					= 1,
		FIFO_AVERAGE_4					= 2,
		FIFO_AVERAGE_8					= 3,
		FIFO_AVERAGE_16					= 4,
		FIFO_AVERAGE_32					= 5,
	};

	//     /////////////////////     FULL SCALE     /////////////////////     //
	enum class FullScale {
		FULL_SCALE_2048_NA				= 0,
		FULL_SCALE_4096_NA				= 1,
		FULL_SCALE_8192_NA				= 2,
		FULL_SCALE_16384_NA				= 3
	};

	//     ///////////////////    INTERRUPT FLAGS     ///////////////////     //
	typedef enum InterruptFlags {
		INTERRUPT_FIFO_ALMOST_FULL		= 0x01,
		INTERRUPT_FIFO_NEW_DATA			= 0x02,
		INTERRUPT_AMBIENT_LIGHT_CANC	= 0x04,
		INTERRUPT_PROXIMITY				= 0x08,
		INTERRUPT_DIE_TEMPERATURE		= 0x10,
		INTERRUPT_POWER_READY			= 0x20,
		INTERRUPT_ALL					= 0x3F,
	} InterruptFlags;

	//     /////////////////////    LED CURRENT     /////////////////////     //
	enum class LedCurrent {
		LED_OFF							= 0,
		LED_CURRENT_200_UA				= 1,
		LED_CURRENT_400_UA				= 2,
		LED_CURRENT_800_UA				= 4,
		LED_CURRENT_1_MA				= 5,
		LED_CURRENT_2_MA				= 10,
		LED_CURRENT_3_MA				= 15,
		LED_CURRENT_4_MA				= 20,
		LED_CURRENT_5_MA				= 26,
		LED_CURRENT_6_MA				= 31,
		LED_CURRENT_7_MA				= 36,
		LED_CURRENT_8_MA				= 41,
		LED_CURRENT_9_MA				= 46,
		LED_CURRENT_10_MA				= 51,
		LED_CURRENT_11_MA				= 56,
		LED_CURRENT_12_MA				= 61,
		LED_CURRENT_13_MA				= 66,
		LED_CURRENT_14_MA				= 71,
		LED_CURRENT_15_MA				= 77,
		LED_CURRENT_16_MA				= 82,
		LED_CURRENT_17_MA				= 87,
		LED_CURRENT_18_MA				= 92,
		LED_CURRENT_19_MA				= 96,
		LED_CURRENT_20_MA				= 102,
		LED_CURRENT_21_MA				= 107,
		LED_CURRENT_22_MA				= 112,
		LED_CURRENT_23_MA				= 117,
		LED_CURRENT_24_MA				= 122,
		LED_CURRENT_25_MA				= 128,
		LED_CURRENT_26_MA				= 133,
		LED_CURRENT_27_MA				= 138,
		LED_CURRENT_28_MA				= 143,
		LED_CURRENT_29_MA				= 148,
		LED_CURRENT_30_MA				= 153,
		LED_CURRENT_31_MA				= 158,
		LED_CURRENT_32_MA				= 163,
		LED_CURRENT_33_MA				= 168,
		LED_CURRENT_34_MA				= 173,
		LED_CURRENT_35_MA				= 179,
		LED_CURRENT_36_MA				= 184,
		LED_CURRENT_37_MA				= 189,
		LED_CURRENT_38_MA				= 194,
		LED_CURRENT_39_MA				= 199,
		LED_CURRENT_40_MA				= 204,
		LED_CURRENT_41_MA				= 209,
		LED_CURRENT_42_MA				= 214,
		LED_CURRENT_43_MA				= 219,
		LED_CURRENT_44_MA				= 224,
		LED_CURRENT_45_MA				= 230,
		LED_CURRENT_46_MA				= 235,
		LED_CURRENT_47_MA				= 240,
		LED_CURRENT_48_MA				= 245,
		LED_CURRENT_49_MA				= 250,
		LED_CURRENT_50_MA				= 255
	};

	//     ////////////////////     MODE CONTROL     ////////////////////     //
	enum class ModeControl {
		HEART_RATE_MODE					= 2,
		SPO2_MODE						= 3,
		MULTI_LED_MODE					= 7
	};

	//     /////////////////////     RESOLUTION     /////////////////////     //
	enum class Resolution {
		RESOLUTION_15_BITS				= 0,
		RESOLUTION_16_BITS				= 1,
		RESOLUTION_17_BITS				= 2,
		RESOLUTION_18_BITS				= 3
	};

	//     /////////////////////    SAMPLE RATE     /////////////////////     //
	enum class SampleRate {
		SAMPLE_RATE_50_HZ				= 0,
		SAMPLE_RATE_100_HZ				= 1,
		SAMPLE_RATE_200_HZ				= 2,
		SAMPLE_RATE_400_HZ				= 3,
		SAMPLE_RATE_800_HZ				= 4,
		SAMPLE_RATE_1000_HZ				= 5,
		SAMPLE_RATE_1600_HZ				= 6,
		SAMPLE_RATE_3200_HZ				= 7
	};

private:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Operators overloading
	// -------------------------------------------------------------------------

public:
	// NONE

private:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Constructors
	// -------------------------------------------------------------------------

public:
	Max30105(void);

	// -------------------------------------------------------------------------
	// Methods - Inherited methods
	// -------------------------------------------------------------------------

public:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Methods - Class own methods
	// -------------------------------------------------------------------------

public:
	//     ///////////////////     INITIALIZATION     ///////////////////     //
	bool init(
			Handler *handler_p			// I2C handler to communicate
	);
	bool reset(void);
	bool shutDown(void);
	bool wakeUp(void);

	//     ////////////////     SENSOR CONFIGURATION     ////////////////     //
	bool setCustomMode(
			LedCurrent redLedCurrent_p = LedCurrent::LED_CURRENT_1_MA,			// TODO: Describe parameter
			LedCurrent infraredLedCurrent_p = LedCurrent::LED_CURRENT_1_MA,		// TODO: Describe parameter
			LedCurrent auxLedCurrent_p = LedCurrent::LED_CURRENT_1_MA			// TODO: Describe parameter
	);
	bool setFullScale(
			FullScale sensorFullScale_p											// TODO: Describe parameter
	);
	bool setHeartRateMode(
			LedCurrent redLedCurrent_p = LedCurrent::LED_CURRENT_1_MA			// TODO: Describe parameter
	);
	bool setResolution(
			Resolution resolution_p												// TODO: Describe parameter
	);
	bool setSampleRate(
			SampleRate sampleRate_p												// TODO: Describe parameter
	);
	bool setSpo2Mode(
			LedCurrent redLedCurrent_p = LedCurrent::LED_CURRENT_1_MA,			// TODO: Describe parameter
			LedCurrent infraredLedCurrent_p = LedCurrent::LED_CURRENT_1_MA		// TODO: Describe parameter
	);

	//     /////////////////////     INTERRUPTS     /////////////////////     //
	bool activateInterrupts(
			InterruptFlags activationMask_p										// TODO: Describe parameter
	);
	bool deactivateInterrupts(
			InterruptFlags deactivationMask_p									// TODO: Describe parameter
	);
	bool getInterruptStatus(
			InterruptFlags *interruptStatus_p									// TODO: Describe parameter
	);

	//     ////////////////////////     FIFO     ////////////////////////     //
	bool configFifo(
			FifoAveraging averages_p,											// TODO: Describe parameter
			bool overflowEnable_p,												// TODO: Describe parameter
			uint8_t remainingSpaceInterrupt_p									// TODO: Describe parameter
	);
	bool flushFifo(void);
	bool getFifoData(
			uint8_t samplesCount_p,												// TODO: Describe parameter
			uint32_t *redLedBuffer_p,											// TODO: Describe parameter
			uint32_t *infraredLedBuffer_p = nullptr								// TODO: Describe parameter
	);
	bool getFifoData(
			uint8_t samplesCount_p,												// TODO: Describe parameter
			uint32_t *ledBuff1,													// TODO: Describe parameter
			uint32_t *ledBuff2,													// TODO: Describe parameter
			uint32_t *ledBuff3,													// TODO: Describe parameter
			uint32_t *ledBuff4 = nullptr										// TODO: Describe parameter
	);
	bool getFifoLastestSample(
			uint32_t *redValue_p,												// TODO: Describe parameter
			uint32_t *infraredValue_p = nullptr									// TODO: Describe parameter
	);
	bool getFifoLastestSample(
			uint32_t *ledBuff1,													// TODO: Describe parameter
			uint32_t *ledBuff2,													// TODO: Describe parameter
			uint32_t *ledBuff3,													// TODO: Describe parameter
			uint32_t *ledBuff4 = nullptr										// TODO: Describe parameter
	);
	bool getFifoNewestSample(
			uint32_t *redValue_p,												// TODO: Describe parameter
			uint32_t *infraredValue_p = nullptr									// TODO: Describe parameter
	);
	bool getFifoNewestSample(
			uint32_t *ledBuff1,													// TODO: Describe parameter
			uint32_t *ledBuff2,													// TODO: Describe parameter
			uint32_t *ledBuff3,													// TODO: Describe parameter
			uint32_t *ledBuff4 = nullptr										// TODO: Describe parameter
	);
	bool getFifoOccupation(
			uint8_t *samplesWaiting_p,											// TODO: Describe parameter
			uint8_t *samplesLost_p = nullptr									// TODO: Describe parameter
	);

	//     ///////////////////////     STATUS     ///////////////////////     //
	Error getLastError(void);
	bool getTemperature(
			float *tempValue_p,													// TODO: Describe parameter
			TermometricScale tempScale_p = TermometricScale::CELSIUS			// TODO: Describe parameter
	);

private:
	//     ///////////////////////     STATUS     ///////////////////////     //
	void _clearData(void);
	bool _isInitialized(void);

	//     ////////////////////    COMMUNICATION     ////////////////////     //
	bool _read(
			uint8_t regAddr_p,														// TODO: Describe parameter
			uint8_t *dataBuffer_p,													// TODO: Describe parameter
			uint16_t bytesToRead_p													// TODO: Describe parameter
	);
	bool _write(
			uint8_t regAddr_p,														// TODO: Describe parameter
			uint8_t *dataBuffer_p,													// TODO: Describe parameter
			uint16_t bytesToWrite_p													// TODO: Describe parameter
	);

	//     ////////////////////////     FIFO     ////////////////////////     //
	bool _getFifoData(
			uint8_t nSamples_p,													// TODO: Describe parameter
			uint32_t *buff1_p,													// TODO: Describe parameter
			uint32_t *buff2_p = nullptr,										// TODO: Describe parameter
			uint32_t *buff3_p = nullptr,										// TODO: Describe parameter
			uint32_t *buff4_p = nullptr											// TODO: Describe parameter
	);
public:
	bool _getFifoInfo(void);
private:
	uint8_t _getFifoReadPointer(void);
	uint8_t _getFifoWritePointer(void);
	bool _setFifoReadPointer(
			uint8_t pointer_p													// TODO: Describe parameter
	);
	bool _setFifoWritePointer(
			uint8_t pointer_p													// TODO: Describe parameter
	);

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Properties
	// -------------------------------------------------------------------------

public:
	// NONE

private:
	//     /////////////////     PERIPHERAL HANDLER     /////////////////     //
	Handler			*_handler;

	//     ///////////////    INTERRUPT CONFIGURATION     ///////////////     //
	bool			_ambientLightCancelationInterruptEnable		: 1;
	bool			_dieTemperatureInterruptEnable				: 1;
	bool			_fifoAlmostFullInterruptEnable				: 1;
	bool			_fifoNewDataInterruptEnable					: 1;
	bool			_proximityThresholdInterruptEnable			: 1;

	//     //////////////////     INTERRUPT STATUS     //////////////////     //
	bool			_ambientLightCancelationInterruptStatus		: 1;
	bool			_dieTemperatureInterruptStatus				: 1;
	bool			_fifoAlmostFullInterruptStatus				: 1;
	bool			_fifoNewDataInterruptStatus					: 1;
	bool			_powerReadyInterruptStatus					: 1;
	bool			_proximityThresholdInterruptStatus			: 1;

	//     /////////////////     CONTROL AND STATUS     /////////////////     //
	bool			_initialized								: 1;
	Error			_lastError;
	uint8_t			_revisionId;
	bool			_shutDownEnabled							: 1;

	//     ////////////////     SENSOR CONFIGURATION     ////////////////     //
	uint8_t			_activeLEDs									: 3;
	FullScale		_fullScale;
	// LedCurrent		_greenLedCurrent;
	LedCurrent		_infraredLedCurrent;
	ModeControl		_modeControl;
	LedCurrent		_redLedCurrent;
	Resolution		_resolution;
	SampleRate		_sampleRate;

	//     /////////////////     FIFO CONFIGURATION     /////////////////     //
public:
	uint8_t			_fifoOverflowCounter						: 5;
	uint8_t			_fifoReadPointer							: 5;
	uint8_t			_fifoWritePointer							: 5;

private:
	FifoAveraging	_fifoAveraging;
	uint8_t			_fifoData;
	uint8_t			_fifoInterruptRemainingSpace				: 4;
	bool			_fifoOverflowEnable							: 1;

	//     //////////////////     TEMPERATURE DATA     //////////////////     //
	uint8_t			_dieTemperatureFraction						: 4;
	uint8_t			_dieTemperatureInteger;

	//     /////////////     CUSTOM SLOTS CONFIGURATION     /////////////     //
	uint8_t			_enabledSlotsCounter						: 3;
	// uint8_t			_multiLedSlot1Control						: 3;
	// uint8_t			_multiLedSlot2Control						: 3;
	// uint8_t			_multiLedSlot3Control						: 3;
	// uint8_t			_multiLedSlot4Control						: 3;

protected:
	// NONE

}; // class Max30105

// =============================================================================
// Max30105 - Class overloading operators
// =============================================================================

// NONE

// =============================================================================
// Global variables
// =============================================================================

// -----------------------------------------------------------------------------
// Externally defined global variables -----------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Internally defined global variables -----------------------------------------

// NONE

// =============================================================================
// Max30105 - Class inline function definitions
// =============================================================================

// -------------------------------------------------------------------------
// Interrupt flags operator overloading
// -------------------------------------------------------------------------

inlined Max30105::InterruptFlags operator|(Max30105::InterruptFlags a, Max30105::InterruptFlags b)
{
	return static_cast<Max30105::InterruptFlags>(static_cast<int>(a) | static_cast<int>(b));
}
inlined Max30105::InterruptFlags &operator|=(Max30105::InterruptFlags &a, Max30105::InterruptFlags b)
{
	return a = static_cast<Max30105::InterruptFlags>(static_cast<int>(a) | static_cast<int>(b));
}

// -------------------------------------------------------------------------
// Fifo averaging operator overloading
// -------------------------------------------------------------------------

inlined uint8_t operator<<(Max30105::FifoAveraging a, uint8_t b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) << b);
}

// -------------------------------------------------------------------------
// Full scale operator overloading
// -------------------------------------------------------------------------

inlined uint8_t operator<<(Max30105::FullScale a, uint8_t b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) << b);
}

// -------------------------------------------------------------------------
// Mode control operator overloading
// -------------------------------------------------------------------------

inlined uint8_t operator<<(Max30105::ModeControl a, uint8_t b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) << b);
}

// -------------------------------------------------------------------------
// Resolution operator overloading
// -------------------------------------------------------------------------

inlined uint8_t operator<<(Max30105::Resolution a, uint8_t b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) << b);
}

// -------------------------------------------------------------------------
// Sample rate operator overloading
// -------------------------------------------------------------------------

inlined uint8_t operator<<(Max30105::SampleRate a, uint8_t b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) << b);
}

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

#endif // __MAX30105_HPP
