/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		max30102.hpp
 * Module:			MAX30102 module interface for FunSAPE++ Embedded Library
 * 					project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __MAX30102_HPP
#define __MAX30102_HPP							__BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#	error	[max30102.hpp] Error 9 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[max30102.hpp] Error 10 - Build mismatch between (max30102.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "../util/funsapeLibHandler.hpp"
#ifndef __FUNSAPE_LIB_HANDLER_HPP
#	error	[max30102.hpp] Error 1 - Header file (funsapeLibHandler.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_HANDLER_HPP != __BUILD_TO_REPLACE__
#	error	[max30102.hpp] Error 2 - Build mismatch between (max30102.hpp) and (funsapeLibHandler.hpp) header files!
#endif
#include "../util/funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#	error	[max30102.hpp] Error 1 - Header file (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_TO_REPLACE__
#	error	[max30102.hpp] Error 2 - Build mismatch between (max30102.hpp) and (funsapeLibDebug.hpp) header files!
#endif

// =============================================================================
// Undefining previous definitions
// =============================================================================

// NONE

// =============================================================================
// Constant definitions
// =============================================================================

#define TWI_SPEED_STANDARD        100000
#define TWI_SPEED_FAST            400000
#define TWI_BUFFER_LENGTH         32

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

// =============================================================================
// Max30102 - Class declaration
// =============================================================================

class Max30102
{
	// -------------------------------------------------------------------------
	// New data types
	// -------------------------------------------------------------------------

public:
	enum class FifoAveraging {
		FIFO_NO_AVERAGE					= 0,
		FIFO_AVERAGE_2					= 1,
		FIFO_AVERAGE_4					= 2,
		FIFO_AVERAGE_8					= 3,
		FIFO_AVERAGE_16					= 4,
		FIFO_AVERAGE_32					= 5,
	};
	enum class FullScale {
		FULL_SCALE_2048_NA				= 0,
		FULL_SCALE_4096_NA				= 1,
		FULL_SCALE_8192_NA				= 2,
		FULL_SCALE_16384_NA				= 3
	};
	typedef enum InterruptFlags {
		INTERRUPT_FIFO_ALMOST_FULL		= 0x01,
		INTERRUPT_FIFO_NEW_DATA			= 0x02,
		INTERRUPT_AMBIENT_LIGHT_CANC	= 0x04,
		INTERRUPT_PROXIMITY				= 0x08,
		INTERRUPT_DIE_TEMPERATURE		= 0x10,
		INTERRUPT_POWER_READY			= 0x20,
		INTERRUPT_ALL					= 0x3F,
	} InterruptFlags;
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
	enum class ModeControl {
		HEART_RATE_MODE					= 2,
		SPO2_MODE						= 3,
		MULTI_LED_MODE					= 7
	};
	enum class Resolution {
		RESOLUTION_15_BITS				= 0,
		RESOLUTION_16_BITS				= 1,
		RESOLUTION_17_BITS				= 2,
		RESOLUTION_18_BITS				= 3
	};
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
	Max30102(
			Handler *handler_p			// TODO: Describe this parameter
	);

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
	Error getLastError(void);
	bool activateInterrupts(
			InterruptFlags activationMask_p						// TODO: Describe parameter
	);
	bool deactivateInterrupts(
			InterruptFlags deactivationMask_p					// TODO: Describe parameter
	);
	bool getInterruptStatus(
			InterruptFlags *interruptStatus_p					// TODO: Describe parameter
	);
	bool reset(void);
	bool shutDown(void);
	bool wakeUp(void);
	bool setHeartRateMode(
			LedCurrent redLedCurrent_p							// TODO: Describe parameter
	);
	bool setSpo2Mode(
			LedCurrent redLedCurrent_p,							// TODO: Describe parameter
			LedCurrent infraredLedCurrent_p						// TODO: Describe parameter
	);
	bool setSampleRate(
			SampleRate sampleRate_p								// TODO: Describe parameter
	);
	bool setResolution(
			Resolution resolution_p								// TODO: Describe parameter
	);
	bool setFullScale(
			FullScale sensorFullScale_p							// TODO: Describe parameter
	);
	bool configFifo(
			FifoAveraging averages_p,							// TODO: Describe parameter
			bool overflowEnable_p,								// TODO: Describe parameter
			uint8_t remainingSpaceInterrupt_p					// TODO: Describe parameter
	);
	bool flushFifo(void);
	bool getFifoOccupation(
			uint8_t *samplesWaiting_p,							// TODO: Describe parameter
			uint8_t *samplesLost_p = nullptr					// TODO: Describe parameter
	);
	bool getNewestSample(
			uint32_t *redValue_p,								// TODO: Describe parameter
			uint32_t *infraredValue_p = nullptr					// TODO: Describe parameter
	);
	bool getNewestSample(
			uint16_t *redValue_p,								// TODO: Describe parameter
			uint16_t *infraredValue_p = nullptr					// TODO: Describe parameter
	);
	bool getLastSample(
			uint32_t *redValue_p,								// TODO: Describe parameter
			uint32_t *infraredValue_p = nullptr					// TODO: Describe parameter
	);
	bool getFifoData(
			uint8_t samplesCount_p,								// TODO: Describe parameter
			uint16_t *redLedBuffer_p,							// TODO: Describe parameter
			uint16_t *infraredLedBuffer_p						// TODO: Describe parameter
	);
	bool getFifoData(
			uint8_t samplesCount_p,								// TODO: Describe parameter
			uint32_t *redLedBuffer_p,							// TODO: Describe parameter
			uint32_t *infraredLedBuffer_p						// TODO: Describe parameter
	);

	uint8_t getReadPointer(void);
	uint8_t getWritePointer(void);
	bool setReadPointer(uint8_t pointer_p);
	bool setWritePointer(uint8_t pointer_p);

	/*
		bool setCustomMode(
				max30102LedCurrent_e redLedCurrent = LED_CURRENT_1_MA,
				max30102LedCurrent_e infraredLedCurrent = LED_CURRENT_1_MA,
				max30102LedCurrent_e auxLedCurrent = LED_CURRENT_1_MA
		);
	*/

	/*
		uint32_t getRed(void); //Returns immediate red value
		uint32_t getIR(void); //Returns immediate IR value
		uint32_t getGreen(void); //Returns immediate green value
		bool safeCheck(uint8_t maxTimeToCheck); //Given a max amount of time, check for new data
		void softReset();
		void setADCRange(uint8_t adcRange);
		void setSampleRate(uint8_t sampleRate);
		void setPulseWidth(uint8_t pulseWidth);
		void setProximityThreshold(uint8_t threshMSB);
		void enableSlot(uint8_t slotNumber, uint8_t device); //Given slot number, assign a device to slot
		void disableSlots(void);
		uint8_t getINT1(void); //Returns the main interrupt group
		uint8_t getINT2(void); //Returns the temp ready interrupt
		uint16_t check(void); //Checks for new data and fills FIFO
		uint8_t available(void); //Tells caller how many new samples are available (head - tail)
		void nextSample(void); //Advances the tail of the sense array
		uint32_t getFIFORed(void); //Returns the FIFO sample pointed to by tail
		uint32_t getFIFOIR(void); //Returns the FIFO sample pointed to by tail
		uint32_t getFIFOGreen(void); //Returns the FIFO sample pointed to by tail
		uint8_t getWritePointer(void);
		uint8_t getReadPointer(void);
		void clearFIFO(void); //Sets the read/write pointers to zero
		void setPROXINTTHRESH(uint8_t val);
		float readTemperature();
		float readTemperatureF();
		void setup(uint8_t powerLevel = 0x1F, uint8_t sampleAverage = 4, uint8_t ledMode = 3, int sampleRate = 400, int pulseWidth = 411, int adcRange = 4096);
		uint8_t readRegister8(uint8_t address, uint8_t reg);
		void writeRegister8(uint8_t address, uint8_t reg, uint8_t value);
	*/

private:
	bool _isInitialized(void);
	bool _getFifoInfo(void);
	bool _readFifoData(
			uint8_t nSamples,
			void *buff1,
			void *buff2,
			uint8_t varSize
	);

	/*
		bool _readNewestSample(
				uint16_t *redValue,
				uint16_t *infraredValue,
				uint8_t varSize
		);
	*/
protected:
	// NONE

	// -------------------------------------------------------------------------
	// Properties
	// -------------------------------------------------------------------------

public:
	uint8_t			_fifoWritePointer							: 5;
	uint8_t			_fifoOverflowCounter						: 5;
	uint8_t			_fifoReadPointer							: 5;

	// NONE

private:
	//     /////////////////    PERIPHERAL HANDLER     //////////////////     //
	Handler			*_handler;

	//     //////////////     INTERRUPT CONFIGURATION     ///////////////     //
	bool			_ambientLightCancelationInterruptEnable		: 1;
	bool			_dieTemperatureInterruptEnable				: 1;
	bool			_fifoAlmostFullInterruptEnable				: 1;
	bool			_fifoNewDataInterruptEnable					: 1;

	//     //////////////////    INTERRUPT STATUS     ///////////////////     //
	bool			_ambientLightCancelationInterruptStatus		: 1;
	bool			_dieTemperatureInterruptStatus				: 1;
	bool			_fifoAlmostFullInterruptStatus				: 1;
	bool			_fifoNewDataInterruptStatus					: 1;
	bool			_powerReadyInterruptStatus					: 1;

	//     /////////////////    CONTROL AND STATUS     //////////////////     //
	bool			_initialized								: 1;
	Error			_lastError;
	uint8_t			_revisionId;
	bool			_shutDownEnabled							: 1;

	//     ///////////////////     CONFIGURATION     ////////////////////     //
	FullScale		_fullScale;
	LedCurrent		_infraredLedCurrent;
	ModeControl		_modeControl;
	LedCurrent		_redLedCurrent;
	Resolution		_resolution;
	SampleRate		_sampleRate;
	uint8_t			_enabledSlotsCounter						: 3;

	//     /////////////////    FIFO CONFIGURATION     //////////////////     //
	FifoAveraging	_fifoAveraging;
	bool			_fifoOverflowEnable							: 1;
	uint8_t			_fifoInterruptRemainingSpace				: 4;
	// uint8_t			_fifoWritePointer							: 5;
	// uint8_t			_fifoOverflowCounter						: 5;
	// uint8_t			_fifoReadPointer							: 5;
	uint8_t			_fifoData;



	/*

		uint8_t						_multiLedSlot1Control			: 3;
		uint8_t						_multiLedSlot2Control			: 3;
		uint8_t						_multiLedSlot3Control			: 3;
		uint8_t						_multiLedSlot4Control			: 3;
		uint8_t						_dieTemperatureInteger;
		uint8_t						_dieTemperatureFraction			: 4;
	//	Gets set during setup. Allows check() to calculate how many uint8_ts to read from FIFO
	//	uint8_t						_activeLEDs;
	*/
protected:
	// NONE

}; // class Max30102

// =============================================================================
// Max30102 - Class overloading operators
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
// Max30102 - Class inline function definitions
// =============================================================================

// NONE

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

inlined Max30102::InterruptFlags operator|(Max30102::InterruptFlags a, Max30102::InterruptFlags b)
{
	return static_cast<Max30102::InterruptFlags>(static_cast<int>(a) | static_cast<int>(b));
}

inlined Max30102::InterruptFlags &operator|=(Max30102::InterruptFlags &a, Max30102::InterruptFlags b)
{
	return a = static_cast<Max30102::InterruptFlags>(static_cast<int>(a) | static_cast<int>(b));
}

inlined uint8_t operator<<(Max30102::ModeControl a, uint8_t b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) << b);
}

inlined uint8_t operator<<(Max30102::FullScale a, uint8_t b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) << b);
}

inlined uint8_t operator<<(Max30102::Resolution a, uint8_t b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) << b);
}

inlined uint8_t operator<<(Max30102::SampleRate a, uint8_t b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) << b);
}

inlined uint8_t operator<<(Max30102::FifoAveraging a, uint8_t b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) << b);
}

#endif // __MAX30102_HPP
