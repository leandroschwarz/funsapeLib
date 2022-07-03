/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		ads1292r.hpp
 * Module:			ADS1292R module interface for FunSAPE++ Embedded Library
 * 					project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Include guard
// =============================================================================

#ifndef __ADS1292R_HPP
#define __ADS1292R_HPP							__BUILD_TO_REPLACE__

// =============================================================================
// Dependencies
// =============================================================================

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "../funsapeLibGlobalDefines.hpp"
#ifndef __FUNSAPE_LIB_GLOBAL_DEFINES_HPP
#	error	[ads1292r.hpp] Error 9 - Global definitions file (funsapeLibGlobalDefines.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_GLOBAL_DEFINES_HPP != __BUILD_TO_REPLACE__
#	error	[ads1292r.hpp] Error 10 - Build mismatch between (ads1292r.hpp) and global definitions file (funsapeLibGlobalDefines.hpp)!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "../util/funsapeLibHandler.hpp"
#ifndef __FUNSAPE_LIB_HANDLER_HPP
#	error	[ads1292r.hpp] Error 1 - Header file (funsapeLibHandler.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_HANDLER_HPP != __BUILD_TO_REPLACE__
#	error	[ads1292r.hpp] Error 2 - Build mismatch between (ads1292r.hpp) and (funsapeLibHandler.hpp) header files!
#endif
#include "../util/funsapeLibDebug.hpp"
#ifndef __FUNSAPE_LIB_DEBUG_HPP
#	error	[ads1292r.hpp] Error 1 - Header file (funsapeLibDebug.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_DEBUG_HPP != __BUILD_TO_REPLACE__
#	error	[ads1292r.hpp] Error 2 - Build mismatch between (ads1292r.hpp) and (funsapeLibDebug.hpp) header files!
#endif

// =============================================================================
// Undefining previous definitions
// =============================================================================

// NONE

// =============================================================================
// Constant definitions
// =============================================================================

// NONE

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
// Ads1292r - Class declaration
// =============================================================================

class Ads1292r
{
	// -------------------------------------------------------------------------
	// New data types
	// -------------------------------------------------------------------------

public:
	enum class ClockDivider {
		DIV_4							= 0,
		DIV_16							= 1,
	};
	enum class DataRate {
		DATA_RATE_125_SPS				= 0,
		DATA_RATE_250_SPS				= 1,
		DATA_RATE_500_SPS				= 2,
		DATA_RATE_1000_SPS				= 3,
		DATA_RATE_2000_SPS				= 4,
		DATA_RATE_4000_SPS				= 5,
		DATA_RATE_8000_SPS				= 6,
	};
	enum class DeviceId {
		ADS1191							= 0,
		ADS1192							= 1,
		ADS1291							= 2,
		ADS1292							= 3,
		ADS1292R						= 4,
		UNKNOWN							= 5,
	};
	enum class InputSelection {
		ELECTRODE						= 0,
		SHORTED							= 1,
		RLD_MEASURE						= 2,
		SUPPLY_MEASURE					= 3,
		TEMPERATURE_SENSOR				= 4,
		TEST_SIGNAL						= 5,
		RLD_DRP							= 6,
		RLD_DRM							= 7,
		RLD_DRPM						= 8,
		ROUTE_IN3P_IN3N					= 9,
		NO_CHANGE						= 10,
	};
	enum class IntRefValue {
		REF_2420_MV						= 0,
		REF_4033_MV						= 1,
		NO_CHANGE						= 2,
	};
	enum class LeadOffCurrFreq {
		DC								= 0,
		DATA_RATE_DIV_4					= 1,
		NO_CHANGE						= 2,
	};
	enum class LeadOffCurrMag {
		CURRENT_6_NA					= 0,
		CURRENT_22_NA					= 1,
		CURRENT_6_UA					= 2,
		CURRENT_22_UA					= 3,
		NO_CHANGE						= 4,
	};
	enum class LeadOffThresh {
		THRESH_95_0_PERCENT					= 0,
		THRESH_92_5_PERCENT					= 1,
		THRESH_90_0_PERCENT					= 2,
		THRESH_87_5_PERCENT					= 3,
		THRESH_85_0_PERCENT					= 4,
		THRESH_80_0_PERCENT					= 5,
		THRESH_75_0_PERCENT					= 6,
		THRESH_70_0_PERCENT					= 7,
		NO_CHANGE							= 8,
	};
	enum class PgaGain {
		GAIN_6							= 0,
		GAIN_1							= 1,
		GAIN_2							= 2,
		GAIN_3							= 3,
		GAIN_4							= 4,
		GAIN_8							= 5,
		GAIN_12							= 6,
		NO_CHANGE						= 7,
	};
	enum class RespFreq {
		FREQ_32_KHZ						= 0,
		FREQ_64_KHZ						= 1,
		NO_CHANGE						= 2,
	};
	enum class RespPhase {
		PHASE_0							= 0,
		PHASE_11_25_OR_22_50			= 1,
		PHASE_22_50_OR_45_00			= 2,
		PHASE_33_75_OR_67_50			= 3,
		PHASE_45_00_OR_90_00			= 4,
		PHASE_56_25_OR_122_50			= 5,
		PHASE_65_50_OR_135_00			= 6,
		PHASE_78_75_OR_157_50			= 7,
		PHASE_90_00						= 8,
		PHASE_101_25					= 9,
		PHASE_112_50					= 10,
		PHASE_123_75					= 11,
		PHASE_135_00					= 12,
		PHASE_146_25					= 13,
		PHASE_157_50					= 14,
		PHASE_168_75					= 15,
		NO_CHANGE						= 16,
	};
	enum class RldChopFreq {
		FMOD_DIV_16						= 0,
		FMOD_DIV_2						= 2,
		FMOD_DIV_4						= 3,
	};
	enum class RldInpConnect {
		DISCONNECTED					= 0,
		POSITIVE_CHANNEL				= 1,
		NEGATIVE_CHANNEL				= 2,
		BOTH_CHANNELS					= 3,
		NO_CHANGE						= 4,
	};
	enum class TestSignal {
		SIGNAL_OFF						= 0,
		SIGNAL_DC						= 2,
		SIGNAL_SQUARE_WAVE				= 3,
	};

private:
	enum class Command {
		WakeUp							= 0x02,
		StadBy							= 0x04,
		Reset							= 0x06,
		StartConversion					= 0x08,
		StopConversion					= 0x0A,
		OffsetCalibration				= 0x1A,
		ContinuousModeEnabled			= 0x10,
		ContinuousModeDisabled			= 0x11,
		ReadData						= 0x12,
	};
	enum class Register {
		ID									= 0x00,
		CONFIG1								= 0x01,
		CONFIG2								= 0x02,
		LOFF								= 0x03,
		CH1SET								= 0x04,
		CH2SET								= 0x05,
		RLD_SENS							= 0x06,
		LOFF_SENS							= 0x07,
		LOFF_STAT							= 0x08,
		RESP1								= 0x09,
		RESP2								= 0x0A,
		GPIO								= 0x0B,
	};

protected:
	// NONE

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
	Ads1292r(void);
	~Ads1292r(void);

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
	//     //////////////     DEVICE OPERATION CONTROL     //////////////     //
	bool calibrate(
			void
	);
	bool changeDataRate(
			DataRate dataRate_p
	);
	bool configReference(
			IntRefValue value_p,
			bool enableInternal_p = false
	);
	bool init(
			Handler *spiHandler_p,								// Communication handler
			GPIO_TypeDef *csGpioPort_p,							// Pointer to the Gpio Port of the CS pin
			uint16_t csPinValue_p								// Number of the CS Pin
	);
	bool reset(
			void
	);
	bool startContinuousConversion(
			void
	);
	bool startSingleConversion(
			void
	);
	bool stopContinuousConversion(
			void
	);
	bool shutDown(
			void
	);
	bool testSignal(
			void
	);
	bool wakeUp(
			void
	);

	//     ///////////////    CHANNEL 1 CONFIGURATION     ///////////////     //
	bool configChannel1(
			bool turnPowerOn_p,
			PgaGain gain_p = PgaGain::NO_CHANGE,
			InputSelection input_p = InputSelection::NO_CHANGE
	);

	//     ///////////////    CHANNEL 2 CONFIGURATION     ///////////////     //
	bool configChannel2(
			bool turnPowerOn_p,
			PgaGain gain_p = PgaGain::NO_CHANGE,
			InputSelection input_p = InputSelection::NO_CHANGE
	);

	//     //////////////////    RLD CONFIGURATION     //////////////////     //
	bool configRldChannel(
			RldInpConnect channel1Input_p,
			RldInpConnect channel2Input_p
	);
	bool configRldSubsystem(
			bool enableBuffer_p,
			bool useInternalBuffer_p,
			RldChopFreq chopFrequency_p,
			bool enableRldLeadOff_p = false
	);

	//     ///////////////     LEAD-OFF CONFIGURATION     ///////////////     //
	bool configLeadOffSubsystem(
			bool turnPowerOn_p,
			LeadOffThresh threshold_p = LeadOffThresh::NO_CHANGE,
			LeadOffCurrMag currentMagnitude_p = LeadOffCurrMag::NO_CHANGE,
			LeadOffCurrFreq currentFrequency_p = LeadOffCurrFreq::NO_CHANGE
	);
	bool configChannel1LeadOff(
			bool enablePositiveInput_p,
			bool enableNegativeInput_p,
			bool enableCurrentSource_p
	);
	bool configChannel2LeadOff(
			bool enablePositiveInput_p,
			bool enableNegativeInput_p,
			bool enableCurrentSource_p
	);

	//     //////////////    RESPIRATION CONFIGURATION     //////////////     //
	bool configRespSubsystem(
			bool demodCircuitEnabled_p = false,
			bool modCircuitEnabled_p = false,
			RespPhase phase_p = RespPhase::PHASE_0,
			bool externalClockEnabled_p = false,
			RespFreq freq_p = RespFreq::FREQ_32_KHZ
	);

	//     /////////////////////    DEVICE PINS     /////////////////////     //
	bool configClockPin(
			bool routeClockToPin_p
	);
	bool configResetPin(
			bool useResetPin_p,
			GPIO_TypeDef *gpio_p = nullptr,
			uint16_t pin_p = 0
	);
	bool configStartPin(
			bool useResetPin_p,
			GPIO_TypeDef *gpio_p = nullptr,
			uint16_t pin_p = 0
	);
	bool configGpio1(
			IoOperation direction_p,
			LogicLevel value_p = LogicLevel::LOW
	);
	bool configGpio2(
			IoOperation direction_p,
			LogicLevel value_p = LogicLevel::LOW
	);
	bool readGpio1(
			LogicLevel *value_p
	);
	bool readGpio2(
			LogicLevel *value_p
	);
	bool writeGpio1(
			LogicLevel value_p
	);
	bool writeGpio2(
			LogicLevel value_p
	);

	//     ///////////////////    DATA MANAGEMENT     ///////////////////     //
	bool getData(
			uint32_t *ecgBuffer_p,
			uint32_t *respBuffer_p,
			uint32_t *status_p
	);
	bool getTemperature(
			float *tempValue_p,
			ThermometricScale tempScale_p
	);
	bool getNoise(
			void
	);

	//     ////////////////////    DEVICE STATUS     ////////////////////     //
	bool getDeviceId(
			DeviceId *deviceId_p
	);
	Error getLastError(
			void
	);
	bool getLeadOffStatus(
			bool *channel1NegativeOff_p,
			bool *channel1PositiveOff_p,
			bool *channel2NegativeOff_p,
			bool *channel2PositiveOff_p,
			bool *rldOff_p
	);

private:
	//     //////////////     CLASS CONTROL AND STATUS     //////////////     //
	void _clearMembers(
			void
	);
	bool _isInitialized(
			void
	);

	//     //////////////    DEVICE CONTROL AND STATUS     //////////////     //
	bool _reset(
			void
	);
	bool _start(
			void
	);
	bool _stop(
			void
	);

	//     ////////////////    REGISTER MANIPULATION     ////////////////     //
	bool _readReg(
			Register regAddr_p,
			uint8_t *dataBuffer_p,
			uint16_t bytesToRead_p
	);
	bool _readAllRegs(
			uint8_t *regs_p
	);
	bool _sendCommand(
			Command command_p
	);
	bool _writeReg(
			Register regAddr_p,
			uint8_t *dataBuffer_p,
			uint16_t bytesToWrite_p
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

	//     ////////////////////    DEVICE STATUS     ////////////////////     //
	DeviceId		_deviceId;
	bool			_initialized								: 1;
	bool			_isLeadOffChannel1Negative					: 1;
	bool			_isLeadOffChannel1Positive					: 1;
	bool			_isLeadOffChannel2Negative					: 1;
	bool			_isLeadOffChannel2Positive					: 1;
	bool			_isLeadOffRld								: 1;
	bool			_isShuttedDown								: 1;
	Error			_lastError;

	//     /////////////////////    DEVICE PINS     /////////////////////     //
	bool			_clockPinOutputEnabled						: 1;
	uint16_t		_csPin;
	GPIO_TypeDef	 *_csPort;
	IoOperation		_gpio1Direction								: 1;
	LogicLevel		_gpio1Value									: 1;
	IoOperation		_gpio2Direction								: 1;
	LogicLevel		_gpio2Value									: 1;
	uint16_t		_resetPin;
	bool			_resetPinEnabled							: 1;
	GPIO_TypeDef	 *_resetPort;
	uint16_t		_startPin;
	bool			_startPinEnabled							: 1;
	GPIO_TypeDef	 *_startPort;

	//     //////////////     DEVICE OPERATION CONTROL     //////////////     //
	ClockDivider	_clockDivider;
	bool			_continuousModeDisabled						: 1;
	DataRate		_dataRate;
	bool			_offsetCalibrationEnabled					: 1;
	bool			_referenceInternalEnabled					: 1;
	IntRefValue		_referenceInternalValue;
	TestSignal		_testSignal;

	//     ///////////////    CHANNEL 1 CONFIGURATION     ///////////////     //
	bool			_channel1Enabled							: 1;
	InputSelection	_channel1InputSelection;
	bool			_channel1LeadOffCurrentSourceEnabled		: 1;
	bool			_channel1LeadOffNegativeEnabled				: 1;
	bool			_channel1LeadOffPositiveEnabled				: 1;
	PgaGain			_channel1PgaGain;

	//     ///////////////    CHANNEL 2 CONFIGURATION     ///////////////     //
	bool			_channel2Enabled							: 1;
	InputSelection	_channel2InputSelection;
	bool			_channel2LeadOffCurrentSourceEnabled		: 1;
	bool			_channel2LeadOffNegativeEnabled				: 1;
	bool			_channel2LeadOffPositiveEnabled				: 1;
	PgaGain			_channel2PgaGain;

	//     //////////////////    RLD CONFIGURATION     //////////////////     //
	bool			_rldBufferEnabled							: 1;
	RldChopFreq		_rldChopFrequency;
	RldInpConnect	_rldConnectToChannel1;
	RldInpConnect	_rldConnectToChannel2;
	bool			_rldLeadOffEnabled							: 1;
	bool			_rldReferenceInternalEnabled				: 1;

	//     ///////////////     LEAD-OFF CONFIGURATION     ///////////////     //
	bool			_leadOffComparatorEnabled					: 1;
	LeadOffThresh	_leadOffComparatorThreshold;
	LeadOffCurrFreq	_leadOffCurrentFrequency;
	LeadOffCurrMag	_leadOffCurrentMagnitude;

	//     //////////////    RESPIRATION CONFIGURATION     //////////////     //
	bool			_respDemodCircuitEnabled					: 1;
	bool			_respModCircuitEnabled						: 1;
	RespPhase		_respPhase;
	bool			_respExternalClockEnabled					: 1;
	RespFreq		_respFreq;



	void	(* _dataReadyInterruptCallback)(void);





	//     /////////////////    INTERRUPT CALLBACKS     /////////////////     //
	// void	(*_ambLightCancOverflowInterruptCallback)(void);
	// void	(*_dieTempReadyInterruptCallback)(void);
	// void	(*_fifoAlmostFullInterruptCallback)(void);
	// void	(*_powerReadyInterruptCallback)(void);





protected:
	// NONE

}; // class Ads1292r

// =============================================================================
// Ads1292r - Class overloading operators
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
// Ads1292r - Class inline function definitions
// =============================================================================

inlined uint8_t operator&(Ads1292r::TestSignal a, uint8_t b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) & b);
}
inlined uint8_t operator&(Ads1292r::RldInpConnect a, uint8_t b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) & b);
}

// inlined Ads1292r::InterruptFlags operator|(Ads1292r::InterruptFlags a, Ads1292r::InterruptFlags b)
// {
// 	return static_cast<Ads1292r::InterruptFlags>(static_cast<int>(a) | static_cast<int>(b));
// }

// inlined Ads1292r::InterruptFlags &operator|=(Ads1292r::InterruptFlags &a, Ads1292r::InterruptFlags b)
// {
// 	return a = static_cast<Ads1292r::InterruptFlags>(static_cast<int>(a) | static_cast<int>(b));
// }

// inlined uint8_t operator<<(Ads1292r::SampleAveraging a, uint8_t b)
// {
// 	return static_cast<uint8_t>(static_cast<uint8_t>(a) << b);
// }

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

#endif // __ADS1292R_HPP
