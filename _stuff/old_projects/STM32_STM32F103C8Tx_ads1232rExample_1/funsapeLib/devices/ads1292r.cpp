/* =============================================================================
 * Project:			FunSAPE++ Embedded Library
 * File name:		ads1292r.cpp
 * Module:			ADS1292R module interface for FunSAPE++ Embedded Library
 * 					project
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "ads1292r.hpp"
#ifndef __ADS1292R_HPP
#	error	[ads1292r.cpp] Error 1 - Header file (ads1292r.hpp) is missing or corrupted!
#elif __ADS1292R_HPP != __BUILD_TO_REPLACE__
#	error	[ads1292r.cpp] Error 13 - Build mismatch between source (ads1292r.cpp) and header (ads1292r.hpp) files!
#endif

// -----------------------------------------------------------------------------
// Header files - FunSAPE Library header files ---------------------------------

#include "../util/funsapeLibStopwatch.hpp"
#ifndef __FUNSAPE_LIB_STOPWATCH_HPP
#	error	[funsapeLibStopwatch.hpp] Error 1 - Header file (funsapeLibStopwatch.hpp) is missing or corrupted!
#elif __FUNSAPE_LIB_STOPWATCH_HPP != __BUILD_TO_REPLACE__
#	error	[funsapeLibStopwatch.hpp] Error 2 - Build mismatch between (ads1292r.hpp) and (funsapeLibStopwatch.hpp) header files!
#endif

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define ADS1292R_READ_REG				0x20
#define ADS1292R_WRITE_REG				0x40

// =============================================================================
// File exclusive - New data types
// =============================================================================

enum class BitIndex {
	// ID register
	REV_ID5								= 5,
	REV_ID								= 0,

	// CONFIG1 register
	SINGLE_SHOT							= 7,
	DR									= 0,

	// CONFIG2 register
	PDB_LOFF_COMP						= 6,
	PDB_REFBUF							= 5,
	VREF_4V								= 4,
	CLK_EN								= 3,
	INT_TEST							= 1,
	TEST_FREQ							= 0,

	// LOFF register
	COMP_TH								= 5,
	ILEAD_OFF							= 2,
	FLEAD_OFF							= 0,

	// CH1SET register
	PD1									= 7,
	GAIN1								= 4,
	MUX1								= 0,

	// CH2SET register
	PD2									= 7,
	GAIN2								= 4,
	MUX2								= 0,

	// RLD_SENS register
	CHOP								= 6,
	PDB_RLD								= 5,
	RLD_LOFF_SENS						= 4,
	RLD2N								= 3,
	RLD2P								= 2,
	RLD1N								= 1,
	RLD1P								= 0,

	// LOFF_SENS register
	FLIP2								= 5,
	FLIP1								= 4,
	LOFF2N								= 3,
	LOFF2P								= 2,
	LOFF1N								= 1,
	LOFF1P								= 0,

	// LOFF_STAT register
	CLK_DIV								= 6,
	RLD_STAT							= 4,
	IN2N_OFF							= 3,
	IN2P_OFF							= 2,
	IN1N_OFF							= 1,
	IN1P_OFF							= 0,

	// RESP1 register
	RESP_DEMOD_EN						= 7,
	RESP_MOD_EN							= 6,
	RESP_PH								= 2,
	RESP_CTRL							= 0,

	// RESP2 register
	CALIB_ON							= 7,
	RESP_FREQ							= 2,
	RLDREF_INT							= 1,

	// GPIO register
	GPIOC2								= 3,
	GPIOC1								= 2,
	GPIOD2								= 1,
	GPIOD1								= 0,
};

// =============================================================================
// Operators overloading
// =============================================================================

inlined uint8_t operator<<(uint8_t a, BitIndex b)
{
	return static_cast<uint8_t>(a << static_cast<uint8_t>(b));
}

inlined uint8_t operator<<(Ads1292r::InputSelection a, BitIndex b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) << static_cast<uint8_t>(b));
}

inlined uint8_t operator<<(Ads1292r::PgaGain a, BitIndex b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) << static_cast<uint8_t>(b));
}

inlined uint8_t operator<<(Ads1292r::DataRate a, BitIndex b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) << static_cast<uint8_t>(b));
}

inlined uint8_t operator<<(Ads1292r::IntRefValue a, BitIndex b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) << static_cast<uint8_t>(b));
}

inlined uint8_t operator<<(Ads1292r::LeadOffCurrMag a, BitIndex b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) << static_cast<uint8_t>(b));
}

inlined uint8_t operator<<(Ads1292r::LeadOffCurrFreq a, BitIndex b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) << static_cast<uint8_t>(b));
}

inlined uint8_t operator<<(Ads1292r::LeadOffThresh a, BitIndex b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) << static_cast<uint8_t>(b));
}

inlined uint8_t operator<<(Ads1292r::RldChopFreq a, BitIndex b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) << static_cast<uint8_t>(b));
}

inlined uint8_t operator<<(Ads1292r::RldInpConnect a, BitIndex b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) << static_cast<uint8_t>(b));
}

inlined uint8_t operator<<(Ads1292r::RespPhase a, BitIndex b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) << static_cast<uint8_t>(b));
}

inlined uint8_t operator<<(Ads1292r::RespFreq a, BitIndex b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) << static_cast<uint8_t>(b));
}

inlined uint8_t operator<<(LogicLevel a, BitIndex b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) << static_cast<uint8_t>(b));
}

inlined uint8_t operator<<(IoOperation a, BitIndex b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) << static_cast<uint8_t>(b));
}

inlined uint8_t operator<<(Ads1292r::ClockDivider a, BitIndex b)
{
	return static_cast<uint8_t>(static_cast<uint8_t>(a) << static_cast<uint8_t>(b));
}

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

#define customDelay()		do {		\
		uint32_t i = 100;				\
		while(i--) {					\
			asm("NOP");					\
		}								\
	} while(0)

// =============================================================================
// Global variables
// =============================================================================

// NONE

// =============================================================================
// Static functions declarations
// =============================================================================

// NONE

// =============================================================================
// Class constructors
// =============================================================================

/**
 * @brief		Construct a new Ads1292r::Ads1292r object.
 * @details		Construct a new Ads1292r object and reset all class' internal
 * 				data members to their deafult values.
 */
Ads1292r::Ads1292r(void)
{
	// Mark passage for debugging purpose
	debugMark("Ads1292r(void)", 3);

	// Reset data members
	this->_csPin						= 0;
	this->_csPort						= nullptr;
	this->_dataReadyInterruptCallback	= nullptr;
	this->_handler						= nullptr;
	this->_resetPin						= 0;
	this->_resetPinEnabled				= false;
	this->_resetPort					= nullptr;
	this->_startPin						= 0;
	this->_startPinEnabled				= false;
	this->_startPort					= nullptr;
	this->_clearMembers();

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return;
}

/**
 * @brief		Destroy an Ads1292r::Ads1292r object.
 * @details		Destroy an Ads1292r::Ads1292r object.
 */
Ads1292r::~Ads1292r(void)
{
	return;
}

// =============================================================================
// Inherited methods - Public
// =============================================================================

// NONE

// =============================================================================
// Inherited methods - Protected
// =============================================================================

// NONE

// =============================================================================
// Class own methods - Public
// =============================================================================

//     ////////////////     DEVICE OPERATION CONTROL     ////////////////     //

/**
 * @brief		Calibrates the device.
 * @details		Calibrates the device.
 * @return		true		Operation successfull.
 * @return		false		Operation failed. Retrieve error calling
 * 							getLastError() function.
 */
bool Ads1292r::calibrate(void)
{
	// Local variables
	uint8_t auxBuff[2] __unused;

	// Mark passage for debugging purpose
	debugMark("calibrate(void)", 3);

	// Checks initialization
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// DEBUG: Under implementation

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

/**
 * @brief		Configures the acquisition data rate.
 * @details		Configures the acquisition data rate.
 * @param[in]	dataRate_p	\ref{DataRate} acquisiton data rate
 * @return		true		Operation successfull.
 * @return		false		Operation failed. Retrieve error calling
 * 							getLastError() function.
 */
bool Ads1292r::changeDataRate(DataRate dataRate_p)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debugging purpose
	debugMark("changeDataRate(DataRate)", 3);

	// Checks initialization
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Prepare data to send
	auxBuff =
			(this->_continuousModeDisabled << BitIndex::SINGLE_SHOT) |
			(dataRate_p << BitIndex::DR);

	// Send data
	if(!this->_writeReg(Register::CONFIG1, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_dataRate = dataRate_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

/**
 * @brief		Configures the device's reference source.
 * @details		Configures the device's reference source.
 * @param[in]	value_p				\ref{IntRefValue} reference value
 * @param[in]	enableInternal_p	enables/disables internal oscillator
 * @return		true		Operation successfull.
 * @return		false		Operation failed. Retrieve error calling
 * 							getLastError() function.
 */
bool Ads1292r::configReference(IntRefValue value_p, bool enableInternal_p)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debugging purpose
	debugMark("configReference(IntRefValue, bool)", 3);

	// Checks initialization
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Rectify NO_CHANGE arguments
	if(value_p == IntRefValue::NO_CHANGE) {
		value_p = this->_referenceInternalValue;
	}

	// Prepare data to send
	auxBuff =
			(1 << 7) |
			(this->_leadOffComparatorEnabled << BitIndex::PDB_LOFF_COMP) |
			(this->_referenceInternalEnabled << BitIndex::PDB_REFBUF) |
			(value_p << BitIndex::VREF_4V) |
			(this->_clockPinOutputEnabled << BitIndex::CLK_EN) |
			(((this->_testSignal & (1 << BitIndex::INT_TEST)) ? 1 : 0) << BitIndex::INT_TEST) |
			(((this->_testSignal & (1 << BitIndex::TEST_FREQ)) ? 1 : 0) << BitIndex::TEST_FREQ);

	// Send data
	if(!this->_writeReg(Register::CONFIG2, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_referenceInternalValue = value_p;
	this->_referenceInternalEnabled = enableInternal_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

/**
 * @brief		Initializes the device.
 * @details		Initializes the handler and communicates with the device to
 * 				check it's part ID.
 * @param[in]	spiHandler_p	\ref{Spi} class handler associated with the
 * 									device
 * @param[in]	csGpioPort_p	\ref{GPIO_TypeDef} pointer to the Gpio Port of
 * 									the CS pin
 * @param[in]	csPinValue_p	number of the CS Pin
 * @return		true		Operation successfull.
 * @return		false		Operation failed. Retrieve error calling
 * 							getLastError() function.
 */
bool Ads1292r::init(Handler *spiHandler_p, GPIO_TypeDef *csGpioPort_p, uint16_t csPinValue_p)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debugging purpose
	debugMark("init(Handler *, void (*), void (*))", 3);

	// Reset data members
	this->_clearMembers();

	// Update data members
	this->_csPin = csPinValue_p;
	this->_csPort = _csPort;
	this->_handler = spiHandler_p;

	// Check function arguments for errors
	if(!isPointerValid(spiHandler_p)) {			// Testing spiHandler_p parameter
		// Returns error
		this->_lastError = Error::HANDLER_POINTER_NULL;
		debugMessage(Error::HANDLER_POINTER_NULL);
		return false;
	} else if(spiHandler_p->getHandlerType() != Handler::HandlerType::SPI) {
		// Returns error
		this->_lastError = Error::HANDLER_UNSUPPORTED;
		debugMessage(Error::HANDLER_UNSUPPORTED);
		return false;
	}
	if(!isPointerValid(csGpioPort_p)) {			// Testing csGpioPort_p parameter
		// Returns error
		this->_lastError = Error::GPIO_PORT_INVALID;
		debugMessage(Error::GPIO_PORT_INVALID);
		return false;
	}

	// Send RESET
	if(!this->_sendCommand(Command::Reset)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	if(!this->_sendCommand(Command::StopConversion)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	if(!this->_sendCommand(Command::ContinuousModeDisabled)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Configuration Register 1
	this->_dataRate = DataRate::DATA_RATE_125_SPS;
	auxBuff =
			(this->_continuousModeDisabled << BitIndex::SINGLE_SHOT) |
			(this->_dataRate << BitIndex::DR);
	if(!this->_writeReg(Register::CONFIG1, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Configuration Register 2
	this->_referenceInternalEnabled = true;
	auxBuff =
			(1 << 7) |
			(this->_leadOffComparatorEnabled << BitIndex::PDB_LOFF_COMP) |
			(this->_referenceInternalEnabled << BitIndex::PDB_REFBUF) |
			(this->_referenceInternalValue << BitIndex::VREF_4V) |
			(this->_clockPinOutputEnabled << BitIndex::CLK_EN) |
			(((this->_testSignal & (1 << BitIndex::INT_TEST)) ? 1 : 0) << BitIndex::INT_TEST) |
			(((this->_testSignal & (1 << BitIndex::TEST_FREQ)) ? 1 : 0) << BitIndex::TEST_FREQ);
	if(!this->_writeReg(Register::CONFIG2, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Lead-Off Control Register
	auxBuff =
			(this->_leadOffComparatorThreshold << BitIndex::COMP_TH) |
			(1 << 4) |
			(this->_leadOffCurrentMagnitude << BitIndex::ILEAD_OFF) |
			(this->_leadOffCurrentFrequency << BitIndex::FLEAD_OFF);
	if(!this->_writeReg(Register::LOFF, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Channel 1 Settings Register
	this->_channel1PgaGain = PgaGain::GAIN_4;
	auxBuff =
			(this->_channel1Enabled << BitIndex::PD1) |
			(this->_channel1PgaGain << BitIndex::GAIN1) |
			(this->_channel1InputSelection << BitIndex::MUX1);
	if(!this->_writeReg(Register::CH1SET, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Channel 2 Settings Register
	this->_channel2PgaGain = PgaGain::GAIN_12;
	auxBuff =
			(this->_channel2Enabled << BitIndex::PD2) |
			(this->_channel2PgaGain << BitIndex::GAIN2) |
			(this->_channel2InputSelection << BitIndex::MUX2);
	if(!this->_writeReg(Register::CH2SET, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Right Leg Drive Sense Selection Register
	this->_rldBufferEnabled = true;
	this->_rldConnectToChannel2 = RldInpConnect::BOTH_CHANNELS;
	auxBuff =
			(this->_rldChopFrequency << BitIndex::CHOP) |
			(this->_rldBufferEnabled << BitIndex::PDB_RLD) |
			(this->_rldLeadOffEnabled << BitIndex::RLD_LOFF_SENS) |
			(((this->_rldConnectToChannel2 & (1 << BitIndex::RLD2N)) ? 1 : 0) << BitIndex::RLD2N) |
			(((this->_rldConnectToChannel2 & (1 << BitIndex::RLD2P)) ? 1 : 0) << BitIndex::RLD2P) |
			(((this->_rldConnectToChannel1 & (1 << BitIndex::RLD1N)) ? 1 : 0) << BitIndex::RLD1N) |
			(((this->_rldConnectToChannel1 & (1 << BitIndex::RLD1P)) ? 1 : 0) << BitIndex::RLD1P);
	if(!this->_writeReg(Register::RLD_SENS, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Lead-Off Sense Selection Register
	auxBuff =
			(this->_channel2LeadOffCurrentSourceEnabled << BitIndex::FLIP2) |
			(this->_channel1LeadOffCurrentSourceEnabled << BitIndex::FLIP1) |
			(this->_channel2LeadOffNegativeEnabled << BitIndex::LOFF2N) |
			(this->_channel2LeadOffPositiveEnabled << BitIndex::LOFF2P) |
			(this->_channel1LeadOffNegativeEnabled << BitIndex::LOFF1N) |
			(this->_channel1LeadOffPositiveEnabled << BitIndex::LOFF1P);
	if(!this->_writeReg(Register::LOFF_SENS, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Lead-Off Status Register
	auxBuff =
			(this->_clockDivider << BitIndex::CLK_DIV) |
			(0 << BitIndex::RLD_STAT) |
			(0 << BitIndex::IN2N_OFF) |
			(0 << BitIndex::IN2P_OFF) |
			(0 << BitIndex::IN1N_OFF) |
			(0 << BitIndex::IN1P_OFF);
	if(!this->_writeReg(Register::LOFF_STAT, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Respiration Control Register 1
	this->_respDemodCircuitEnabled = true;
	this->_respModCircuitEnabled = true;
	this->_respPhase = RespPhase::PHASE_135_00;
	auxBuff =
			(this->_respDemodCircuitEnabled << BitIndex::RESP_DEMOD_EN) |
			(this->_respModCircuitEnabled << BitIndex::RESP_MOD_EN) |
			(this->_respPhase << BitIndex::RESP_PH) |
			(1 << 1) |
			(this->_respExternalClockEnabled << BitIndex::RESP_CTRL);
	if(!this->_writeReg(Register::RESP1, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Respiration Control Register 2
	auxBuff =
			(this->_offsetCalibrationEnabled << BitIndex::CALIB_ON) |
			(this->_respFreq << BitIndex::RESP_FREQ) |
			(this->_rldReferenceInternalEnabled << BitIndex::RLDREF_INT) |
			(1 << 0);
	if(!this->_writeReg(Register::RESP2, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// General-Purpose I/O Register
	auxBuff =
			(this->_gpio2Direction << BitIndex::GPIOC2) |
			(this->_gpio1Direction << BitIndex::GPIOC1) |
			(this->_gpio2Value << BitIndex::GPIOD2) |
			(this->_gpio1Value << BitIndex::GPIOD1);
	if(!this->_writeReg(Register::GPIO, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	if(!this->_sendCommand(Command::ContinuousModeEnabled)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}
	if(!this->_sendCommand(Command::StartConversion)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}
	if(!this->_sendCommand(Command::ContinuousModeDisabled)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// FIXME: Check device ID

	// Update data members
	this->_initialized = true;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

/**
 * @brief		Resets the device.
 * @details		Sends a reset signal to device and clears internal class data members.
 * @return		true		Operation successfull.
 * @return		false		Operation failed. Retrieve error calling
 * 							getLastError() function.
 */
bool Ads1292r::reset(void)
{
	// Mark passage for debugging purpose
	debugMark("reset(void)", 3);

	// Reset data members
	this->_clearMembers();

	// Send RESET
	if(!this->_reset()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool startContinuousConversion(void)
{
	// FIXME: Implement startContinuousConversion() function
	return false;
}

bool startSingleConversion(void)
{
	// FIXME: Implement startSingleConversion() function
	return false;
}

bool stopContinuousConversion(void)
{
	// FIXME: Implement stopContinuousConversion() function
	return false;
}

/**
 * @brief		Makes the device to enter in power-save mode.
 * @details		Makes the device to enter in power-save mode. While power-save
 * 				mode, the LEDs, the ADC sensor, and all interrupts are tuned
 * 				off, but all device registers retain their values (allowing
 * 				quick wake-up). Also, write/read operations function as normal.
 * @return		true		Operation successfull.
 * @return		false		Operation failed. Retrieve error calling
 * 							getLastError() function.
 */
bool Ads1292r::shutDown(void)
{
	// FIXME: Implement shutDown() function
	return false;
}

/**
 * @brief		Test the device.
 * @details		Test the device.
 * @return		true		Operation successfull.
 * @return		false		Operation failed. Retrieve error calling
 * 							getLastError() function.
 */
bool Ads1292r::testSignal(void)
{
	// FIXME: Implement testSignal() function
	return false;
}

/**
 * @brief		Wakes up the device when in power-save mode.
 * @details		Wakes up the device when in power-save mode. Since all device
 * 				registers retain their values during power-save mode, all
 * 				previous device configuration are active and the device responds
 * 				accordingly imediately (quick wake-up).
 * @return		true		Operation successfull.
 * @return		false		Operation failed. Retrieve error calling
 * 							getLastError() function.
 */
bool Ads1292r::wakeUp(void)
{
	// FIXME: Implement wakeUp() function
	return false;
}

//     /////////////////    CHANNEL 1 CONFIGURATION     /////////////////     //

/**
 * @brief		Configures the channel 1.
 * @details		Configures the device's channel 1.
 * @param[in]	turnPowerOn_p	\ref{Twi} class handler associated with the
 * 									device
 * @param[in]	gain_p			\ref{PgaGain} gain of the channel's PGA
 * @param[in]	input_p			\ref{InputSelection} selects the channel's input
 * 									source
 * @return		true		Operation successfull.
 * @return		false		Operation failed. Retrieve error calling
 * 							getLastError() function.
 */
bool Ads1292r::configChannel1(bool turnPowerOn_p, PgaGain gain_p, InputSelection input_p)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debugging purpose
	debugMark("configChannel1(bool, PgaGain, InputSelection)", 3);

	// Checks initialization
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Rectify NO_CHANGE arguments
	if(gain_p == PgaGain::NO_CHANGE) {
		gain_p = this->_channel1PgaGain;
	}
	if(input_p == InputSelection::NO_CHANGE) {
		input_p = this->_channel1InputSelection;
	}

	// Prepare data to send
	auxBuff =
			(turnPowerOn_p << BitIndex::PD1) |
			(gain_p << BitIndex::GAIN1) |
			(_channel1PgaGain << BitIndex::MUX1);

	// Send data
	if(!this->_writeReg(Register::CH1SET, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_channel1Enabled = turnPowerOn_p;
	this->_channel1PgaGain = gain_p;
	this->_channel1InputSelection = input_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

//     /////////////////    CHANNEL 2 CONFIGURATION     /////////////////     //

/**
 * @brief		Configures the channel 2.
 * @details		Configures the device's channel 2.
 * @param[in]	turnPowerOn_p	\ref{Twi} class handler associated with the
 * 									device
 * @param[in]	gain_p			\ref{PgaGain} gain of the channel's PGA
 * @param[in]	input_p			\ref{InputSelection} selects the channel's input
 * 									source
 * @return		true		Operation successfull.
 * @return		false		Operation failed. Retrieve error calling
 * 							getLastError() function.
 */
bool Ads1292r::configChannel2(bool turnPowerOn_p, PgaGain gain_p, InputSelection input_p)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debugging purpose
	debugMark("configChannel2(bool, PgaGain, InputSelection)", 3);

	// Checks initialization
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Rectify NO_CHANGE arguments
	if(gain_p == PgaGain::NO_CHANGE) {
		gain_p = this->_channel1PgaGain;
	}
	if(input_p == InputSelection::NO_CHANGE) {
		input_p = this->_channel1InputSelection;
	}

	// Prepare data to send
	auxBuff =
			(turnPowerOn_p << BitIndex::PD2) |
			(gain_p << BitIndex::GAIN2) |
			(input_p << BitIndex::MUX2);

	// Send data
	if(!this->_writeReg(Register::CH2SET, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_channel2Enabled = turnPowerOn_p;
	this->_channel2PgaGain = gain_p;
	this->_channel2InputSelection = input_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

//     ////////////////////    RLD CONFIGURATION     ////////////////////     //

/**
 * @brief		Configures the device's RLD's input connection.
 * @details		Configures the device's RLD's input connection.
 * @param[in]	channel1Input_p		\ref{RldInpConnect} RLD's input setup
 * @param[in]	channel2Input_p		\ref{RldInpConnect} RLD's input setup
 * @return		true		Operation successfull.
 * @return		false		Operation failed. Retrieve error calling
 * 							getLastError() function.
 */
bool Ads1292r::configRldChannel(RldInpConnect channel1Input_p, RldInpConnect channel2Input_p)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debugging purpose
	debugMark("configRldChannel(RldInpConnect, RldInpConnect)", 3);

	// Checks initialization
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Rectify NO_CHANGE arguments
	if(channel2Input_p == RldInpConnect::NO_CHANGE) {
		channel2Input_p = this->_rldConnectToChannel2;
	}
	if(channel1Input_p == RldInpConnect::NO_CHANGE) {
		channel1Input_p = this->_rldConnectToChannel1;
	}

	// Prepare data to send
	auxBuff =
			(this->_rldChopFrequency << BitIndex::CHOP) |
			(this->_rldBufferEnabled << BitIndex::PDB_RLD) |
			(this->_rldLeadOffEnabled << BitIndex::RLD_LOFF_SENS) |
			(((channel2Input_p & (1 << BitIndex::RLD2N)) ? 1 : 0) << BitIndex::RLD2N) |
			(((channel2Input_p & (1 << BitIndex::RLD2P)) ? 1 : 0) << BitIndex::RLD2P) |
			(((channel1Input_p & (1 << BitIndex::RLD1N)) ? 1 : 0) << BitIndex::RLD1N) |
			(((channel1Input_p & (1 << BitIndex::RLD1P)) ? 1 : 0) << BitIndex::RLD1P);

	// Send data
	if(!this->_writeReg(Register::RLD_SENS, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_rldConnectToChannel2 = channel2Input_p;
	this->_rldConnectToChannel1 = channel1Input_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

/**
 * @brief		Configures the device's RLD's subsystem.
 * @details		Configures the device's RLD's subsystem.
 * @param[in]	enableBuffer_p		enable/disable RLD buffer
 * @param[in]	useInternalBuffer_p	use internal or external
 * @param[in]	chopFrequency_p		\ref{RldChopFreq} RLD's input setup
 * @param[in]	enableRldLeadOff_p	RLD's input setup
 * @return		true		Operation successfull.
 * @return		false		Operation failed. Retrieve error calling
 * 							getLastError() function.
 */
bool Ads1292r::configRldSubsystem(bool enableBuffer_p, bool useInternalBuffer_p, RldChopFreq chopFrequency_p, bool enableRldLeadOff_p)
{
	// FIXME: implement configRldSubsystem() function
	return false;
}

//     /////////////////     LEAD-OFF CONFIGURATION     /////////////////     //

/**
 * @brief		Configures the device's lead off detection subsystem.
 * @details		Configures the device's lead off detection subsystem.
 * @param[in]	turnPowerOn_p		enables/disables subsystem
 * @param[in]	threshold_p			\ref{LeadOffThresh} detection threshold
 * @param[in]	currentMagnitude_p	\ref{LeadOffCurrMag} current magnitude
 * @param[in]	currentFrequency_p	\ref{LeadOffCurrFreq} current frequency
 * @return		true		Operation successfull.
 * @return		false		Operation failed. Retrieve error calling
 * 							getLastError() function.
 */
bool Ads1292r::configLeadOffSubsystem(bool turnPowerOn_p, LeadOffThresh threshold_p, LeadOffCurrMag currentMagnitude_p, LeadOffCurrFreq currentFrequency_p)
{
	// Local variables
	uint8_t auxBuff[2];

	// Mark passage for debugging purpose
	debugMark("configLeadOffSubsystem(bool, LeadOffThresh, LeadOffCurrMag, LeadOffCurrFreq)", 3);

	// Checks initialization
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Rectify NO_CHANGE arguments
	if(threshold_p == LeadOffThresh::NO_CHANGE) {
		threshold_p = this->_leadOffComparatorThreshold;
	}
	if(currentMagnitude_p == LeadOffCurrMag::NO_CHANGE) {
		currentMagnitude_p = this->_leadOffCurrentMagnitude;
	}
	if(currentFrequency_p == LeadOffCurrFreq::NO_CHANGE) {
		currentFrequency_p = this->_leadOffCurrentFrequency;
	}

	// Prepare data to send
	auxBuff[0] =
			(1 << 7) |
			(turnPowerOn_p << BitIndex::PDB_LOFF_COMP) |
			(this->_referenceInternalEnabled << BitIndex::PDB_REFBUF) |
			(this->_referenceInternalValue << BitIndex::VREF_4V) |
			(this->_clockPinOutputEnabled << BitIndex::CLK_EN) |
			(((this->_testSignal & (1 << BitIndex::INT_TEST)) ? 1 : 0) << BitIndex::INT_TEST) |
			(((this->_testSignal & (1 << BitIndex::TEST_FREQ)) ? 1 : 0) << BitIndex::TEST_FREQ);
	auxBuff[1] =
			(threshold_p << BitIndex::COMP_TH) |
			(1 << 4) |
			(currentMagnitude_p << BitIndex::ILEAD_OFF) |
			(currentFrequency_p << BitIndex::FLEAD_OFF);

	// Send data
	if(!this->_writeReg(Register::CONFIG2, auxBuff, 2)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_leadOffComparatorEnabled = turnPowerOn_p;
	this->_leadOffComparatorThreshold = threshold_p;
	this->_leadOffCurrentMagnitude = currentMagnitude_p;
	this->_leadOffCurrentFrequency = currentFrequency_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

/**
 * @brief		Configures the lead off options for channel 1.
 * @details		Configures the lead off options for channel 1.
 * @param[in]	enablePositiveInput_p	enables/disables the channel's positive
 * 											electrode lead off detection
 * @param[in]	enableNegativeInput_p	enables/disables the channel's negative
 * 											electrode lead off detection
 * @param[in]	enableCurrentSource_p	enables/disables the channel's lead off
 * 										current source
 * @return		true		Operation successfull.
 * @return		false		Operation failed. Retrieve error calling
 * 							getLastError() function.
 */
bool Ads1292r::configChannel1LeadOff(bool enablePositiveInput_p, bool enableNegativeInput_p, bool enableCurrentSource_p)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debugging purpose
	debugMark("configChannel1LeadOff(bool, bool, bool)", 3);

	// Checks initialization
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Prepare data to send
	auxBuff =
			(this->_channel2LeadOffCurrentSourceEnabled << BitIndex::FLIP2) |
			(enableCurrentSource_p << BitIndex::FLIP1) |
			(this->_channel2LeadOffNegativeEnabled << BitIndex::LOFF2N) |
			(this->_channel2LeadOffPositiveEnabled << BitIndex::LOFF2P) |
			(enableNegativeInput_p << BitIndex::LOFF1N) |
			(enablePositiveInput_p << BitIndex::LOFF1P);

	// Send data
	if(!this->_writeReg(Register::LOFF_SENS, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_channel1LeadOffCurrentSourceEnabled = enableCurrentSource_p;
	this->_channel1LeadOffNegativeEnabled = enableNegativeInput_p;
	this->_channel1LeadOffPositiveEnabled = enablePositiveInput_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

/**
 * @brief		Configures the lead off options for channel 2.
 * @details		Configures the lead off options for channel 2.
 * @param[in]	enablePositiveInput_p	enables/disables the channel's positive
 * 											electrode lead off detection
 * @param[in]	enableNegativeInput_p	enables/disables the channel's negative
 * 											electrode lead off detection
 * @param[in]	enableCurrentSource_p	enables/disables the channel's lead off
 * 										current source
 * @return		true		Operation successfull.
 * @return		false		Operation failed. Retrieve error calling
 * 							getLastError() function.
 */
bool Ads1292r::configChannel2LeadOff(bool enablePositiveInput_p, bool enableNegativeInput_p, bool enableCurrentSource_p)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debugging purpose
	debugMark("configChannel2LeadOff(bool, bool, bool)", 3);

	// Checks initialization
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Prepare data to send
	auxBuff =
			(enableCurrentSource_p << BitIndex::FLIP2) |
			(this->_channel1LeadOffCurrentSourceEnabled << BitIndex::FLIP1) |
			(enableNegativeInput_p << BitIndex::LOFF2N) |
			(enablePositiveInput_p << BitIndex::LOFF2P) |
			(this->_channel1LeadOffNegativeEnabled << BitIndex::LOFF1N) |
			(this->_channel1LeadOffPositiveEnabled << BitIndex::LOFF1P);

	// Send data
	if(!this->_writeReg(Register::LOFF_SENS, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_channel2LeadOffCurrentSourceEnabled = enableCurrentSource_p;
	this->_channel2LeadOffNegativeEnabled = enableNegativeInput_p;
	this->_channel2LeadOffPositiveEnabled = enablePositiveInput_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

//     ////////////////    RESPIRATION CONFIGURATION     ////////////////     //

/**
 * @brief		Configures the device's respiration subsystem.
 * @details		Configures the device's respiration subsystem.
 * @param[in]	demodCircuitEnabled_p	enable/disable demodulation circuitry
 * @param[in]	modCircuitEnabled_p		enable/disable modulation circuitry
 * @param[in]	phase_p					\ref{RespPhase} demodulation respiration
 * 											phase
 * @param[in]	externalClockEnabled_p	enable external clock for repiration
 * 											circuitry
 * @param[in]	freq_p					\ref{RespFreq} respiration control
 * 											frequency
 * @return		true		Operation successfull.
 * @return		false		Operation failed. Retrieve error calling
 * 							getLastError() function.
 */
bool Ads1292r::configRespSubsystem(bool demodCircuitEnabled_p, bool modCircuitEnabled_p, RespPhase phase_p, bool externalClockEnabled_p, RespFreq freq_p)
{
	// Local variables
	uint8_t auxBuff[2];

	// Mark passage for debugging purpose
	debugMark("configRespSubsystem(bool, bool, RespPhase, bool, RespFreq)", 3);

	// Checks initialization
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Rectify NO_CHANGE arguments
	if(phase_p == RespPhase::NO_CHANGE) {
		phase_p = this->_respPhase;
	}
	if(freq_p == RespFreq::NO_CHANGE) {
		freq_p = this->_respFreq;
	}

	// Prepare data to send
	auxBuff[0] =
			(this->_respDemodCircuitEnabled << BitIndex::RESP_DEMOD_EN) |
			(this->_respModCircuitEnabled << BitIndex::RESP_MOD_EN) |
			(this->_respPhase << BitIndex::RESP_PH) |
			(1 << 1) |
			(this->_respExternalClockEnabled << BitIndex::RESP_CTRL);
	auxBuff[1] =
			(this->_offsetCalibrationEnabled << BitIndex::CALIB_ON) |
			(this->_respFreq << BitIndex::RESP_FREQ) |
			(this->_rldReferenceInternalEnabled << BitIndex::RLDREF_INT) |
			(1 << 0);

	// Send data
	if(!this->_writeReg(Register::RESP1, auxBuff, 2)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_respDemodCircuitEnabled = demodCircuitEnabled_p;
	this->_respModCircuitEnabled = modCircuitEnabled_p;
	this->_respPhase = phase_p;
	this->_respExternalClockEnabled = externalClockEnabled_p;
	this->_respFreq = freq_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

//     ///////////////////////    DEVICE PINS     ///////////////////////     //

bool Ads1292r::configClockPin(bool routeClockToPin_p)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debugging purpose
	debugMark("configClockPin(bool)", 3);

	// Checks initialization
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Prepare data to send
	auxBuff =
			(1 << 7) |
			(this->_leadOffComparatorEnabled << BitIndex::PDB_LOFF_COMP) |
			(this->_referenceInternalEnabled << BitIndex::PDB_REFBUF) |
			(this->_referenceInternalValue << BitIndex::VREF_4V) |
			(routeClockToPin_p << BitIndex::CLK_EN) |
			(((this->_testSignal & (1 << BitIndex::INT_TEST)) ? 1 : 0) << BitIndex::INT_TEST) |
			(((this->_testSignal & (1 << BitIndex::TEST_FREQ)) ? 1 : 0) << BitIndex::TEST_FREQ);

	// Send data
	if(!this->_writeReg(Register::RESP1, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_clockPinOutputEnabled = routeClockToPin_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

/**
 * @brief		Configure the RESET pin.
 * @details		Configure the RESET pin.
 * @param[in]	usePin_p	enable/disable the use of the external pin
 * @param[in]	gpio_p		\ref{GPIO_TypeDef} pointer to the Gpio Port of the
 * 								RESET pin
 * @param[in]	pin_p		number of the RESET Pin
 * @return		true		Operation successfull.
 * @return		false		Operation failed. Retrieve error calling
 * 							getLastError() function.
 */
bool Ads1292r::configResetPin(bool usePin_p, GPIO_TypeDef *gpio_p, uint16_t pin_p)
{
	// Mark passage for debugging purpose
	debugMark("configResetPin(bool, GPIO_TypeDef *, uint16_t)", 3);

	// Check function arguments for errors
	if(usePin_p) {
		if(!isPointerValid(gpio_p)) {				// Testing gpio_p parameter
			// Returns error
			this->_lastError = Error::GPIO_PORT_INVALID;
			debugMessage(Error::GPIO_PORT_INVALID);
			return false;
		}
	}

	// Update data members
	this->_resetPinEnabled = usePin_p;
	if(usePin_p) {
		this->_resetPort = gpio_p;
		this->_resetPin = pin_p;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

/**
 * @brief		Configure the START pin.
 * @details		Configure the START pin.
 * @param[in]	usePin_p	enable/disable the use of the external pin
 * @param[in]	gpio_p		\ref{GPIO_TypeDef} pointer to the Gpio Port of the
 * 								START pin
 * @param[in]	pin_p		number of the START Pin
 * @return		true		Operation successfull.
 * @return		false		Operation failed. Retrieve error calling
 * 							getLastError() function.
 */
bool Ads1292r::configStartPin(bool usePin_p, GPIO_TypeDef *gpio_p, uint16_t pin_p)
{
	// Mark passage for debugging purpose
	debugMark("configStartPin(bool, GPIO_TypeDef *, uint16_t)", 3);

	// Check function arguments for errors
	if(usePin_p) {
		if(!isPointerValid(gpio_p)) {				// Testing gpio_p parameter
			// Returns error
			this->_lastError = Error::GPIO_PORT_INVALID;
			debugMessage(Error::GPIO_PORT_INVALID);
			return false;
		}
	}

	// Update data members
	this->_startPinEnabled = usePin_p;
	if(usePin_p) {
		this->_startPort = gpio_p;
		this->_startPin = pin_p;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Ads1292r::configGpio1(IoOperation direction_p, LogicLevel value_p)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debugging purpose
	debugMark("configGpio1(IoOperation, LogicLevel)", 3);

	// Checks initialization
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Check function arguments for errors
	if(direction_p == IoOperation::WRITE) {
		if(this->_respExternalClockEnabled) {
			// Returns error
			this->_lastError = Error::MODE_UNSUPPORTED;
			debugMessage(Error::MODE_UNSUPPORTED);
			return false;
		}
	}

	// Prepare data to send
	auxBuff =
			(this->_gpio2Direction << BitIndex::GPIOC2) |
			(direction_p << BitIndex::GPIOC1) |
			(this->_gpio2Value << BitIndex::GPIOD2) |
			(value_p << BitIndex::GPIOD1);

	// Send data
	if(!this->_writeReg(Register::GPIO, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_gpio1Direction = direction_p;
	this->_gpio1Value = value_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Ads1292r::configGpio2(IoOperation direction_p, LogicLevel value_p)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debugging purpose
	debugMark("configGpio2(IoOperation, LogicLevel)", 3);

	// Checks initialization
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Check function arguments for errors
	if(direction_p == IoOperation::WRITE) {
		if(this->_respExternalClockEnabled) {
			// Returns error
			this->_lastError = Error::MODE_UNSUPPORTED;
			debugMessage(Error::MODE_UNSUPPORTED);
			return false;
		}
	}

	// Prepare data to send
	auxBuff =
			(direction_p << BitIndex::GPIOC2) |
			(this->_gpio1Direction << BitIndex::GPIOC1) |
			(value_p << BitIndex::GPIOD2) |
			(this->_gpio1Value << BitIndex::GPIOD1);

	// Send data
	if(!this->_writeReg(Register::GPIO, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_gpio2Direction = direction_p;
	this->_gpio2Value = value_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Ads1292r::readGpio1(LogicLevel *value_p)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debugging purpose
	debugMark("readGpio1(LogicLevel *)", 3);

	// Checks initialization
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Check function arguments for errors
	if(!isPointerValid(value_p)) {
		// Returns error
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		debugMessage(Error::ARGUMENT_POINTER_NULL);
		return false;
	}

	// Get data
	if(!this->_readReg(Register::GPIO, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Process received data
	*value_p = (auxBuff & (1 << BitIndex::GPIOD1)) ? (LogicLevel::HIGH) : (LogicLevel::LOW);

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Ads1292r::readGpio2(LogicLevel *value_p)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debugging purpose
	debugMark("readGpio2(LogicLevel *)", 3);

	// Checks initialization
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Check function arguments for errors
	if(!isPointerValid(value_p)) {
		// Returns error
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		debugMessage(Error::ARGUMENT_POINTER_NULL);
		return false;
	}

	// Get data
	if(!this->_readReg(Register::GPIO, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Process received data
	*value_p = (auxBuff & (1 << BitIndex::GPIOD2)) ? (LogicLevel::HIGH) : (LogicLevel::LOW);

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Ads1292r::writeGpio1(LogicLevel value_p)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debugging purpose
	debugMark("writeGpio1(LogicLevel)", 3);

	// Checks initialization
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Check function arguments for errors
	if(this->_gpio1Direction == IoOperation::READ) {
		// Returns error
		this->_lastError = Error::MODE_UNSUPPORTED;
		debugMessage(Error::MODE_UNSUPPORTED);
		return false;
	}

	// Prepare data to send
	auxBuff =
			(this->_gpio2Direction << BitIndex::GPIOC2) |
			(this->_gpio1Direction << BitIndex::GPIOC1) |
			(this->_gpio2Value << BitIndex::GPIOD2) |
			(value_p << BitIndex::GPIOD1);

	// Send data
	if(!this->_writeReg(Register::GPIO, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_gpio1Value = value_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Ads1292r::writeGpio2(LogicLevel value_p)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debugging purpose
	debugMark("writeGpio2(LogicLevel)", 3);

	// Checks initialization
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Check function arguments for errors
	if(this->_gpio2Direction == IoOperation::READ) {
		// Returns error
		this->_lastError = Error::MODE_UNSUPPORTED;
		debugMessage(Error::MODE_UNSUPPORTED);
		return false;
	}

	// Prepare data to send
	auxBuff =
			(this->_gpio2Direction << BitIndex::GPIOC2) |
			(this->_gpio1Direction << BitIndex::GPIOC1) |
			(value_p << BitIndex::GPIOD2) |
			(this->_gpio1Value << BitIndex::GPIOD1);

	// Send data
	if(!this->_writeReg(Register::GPIO, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Update data members
	this->_gpio2Value = value_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

//     /////////////////////    DATA MANAGEMENT     /////////////////////     //

bool Ads1292r::getData(uint32_t *ecgBuffer_p, uint32_t *respBuffer_p, uint32_t *status_p)
{
	// Checks initialization
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// FIXME: Implement getData() function
	return false;
}

bool Ads1292r::getTemperature(float *tempValue_p, ThermometricScale tempScale_p)
{
	// Checks initialization
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// FIXME: Implement getTemperature() function
	return false;
}

bool Ads1292r::getNoise(void)
{
	// Checks initialization
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// FIXME: Implement getNoise() function
	return false;
}

//     //////////////////////    DEVICE STATUS     //////////////////////     //

bool Ads1292r::getDeviceId(DeviceId *deviceId_p)
{
	// Local variables
	uint8_t auxBuff;
	uint8_t rev0;
	uint8_t rev1;

	// Mark passage for debugging purpose
	debugMark("getDeviceId(DeviceId *)", 3);

	// Checks initialization
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Check function arguments for errors
	if(!isPointerValid(deviceId_p)) {
		// Returns error
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		debugMessage(Error::ARGUMENT_POINTER_NULL);
		return false;
	}

	// Get data
	if(!this->_readReg(Register::ID, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Process received data
	rev1 = (auxBuff >> (uint8_t)(BitIndex::REV_ID5)) & 0x03;
	rev0 = (auxBuff >> (uint8_t)(BitIndex::REV_ID)) & 0x03;
	if(isBitClr(auxBuff, 4)) {
		// Returns error
		this->_lastError = Error::DEVICE_UNSUPPORTED;
		debugMessage(Error::DEVICE_UNSUPPORTED);
		return false;
	}
	if(rev1 == 0x02) {
		switch(rev0) {
		case 0x00: *deviceId_p = DeviceId::ADS1191; break;
		case 0x01: *deviceId_p = DeviceId::ADS1192; break;
		case 0x02: *deviceId_p = DeviceId::ADS1291; break;
		case 0x03: *deviceId_p = DeviceId::ADS1292; break;
		default:
			// Returns error
			this->_lastError = Error::DEVICE_ID_MATCH_FAILED;
			debugMessage(Error::DEVICE_ID_MATCH_FAILED);
			return false;
		}
	} else if(rev1 == 0x03) {
		*deviceId_p = DeviceId::ADS1292R;
	} else {
		// Returns error
		this->_lastError = Error::DEVICE_ID_MATCH_FAILED;
		debugMessage(Error::DEVICE_ID_MATCH_FAILED);
		return false;
	}

	// Update data members
	this->_deviceId = *deviceId_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

/**
 * @brief		Returns the code of the last error.
 * @details		Returns the FunSAPE++ Embedded Library error code of the last
 * 				operation. If the operation was successfull, the return code
 * 				will be Error::NONE.
 * @return		Error	Error:: error code.
 */
Error Ads1292r::getLastError(void)
{
	// Returns last error
	return this->_lastError;
}

bool Ads1292r::getLeadOffStatus(bool *channel1NegativeOff_p, bool *channel1PositiveOff_p, bool *channel2NegativeOff_p, bool *channel2PositiveOff_p, bool *rldOff_p)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debugging purpose
	debugMark("getLeadOffStatus(bool *, bool *, bool *, bool *, bool *)", 3);

	// Checks initialization
	if(!this->_isInitialized()) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Check function arguments for errors
	if(
			(!isPointerValid(channel1NegativeOff_p)) ||
			(!isPointerValid(channel1PositiveOff_p)) ||
			(!isPointerValid(channel2NegativeOff_p)) ||
			(!isPointerValid(channel2PositiveOff_p)) ||
			(!isPointerValid(rldOff_p))
	) {
		// Returns error
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		debugMessage(Error::ARGUMENT_POINTER_NULL);
		return false;
	}

	// Get data
	if(!this->_readReg(Register::LOFF_STAT, &auxBuff, 1)) {
		// Returns error
		debugMessage(this->_lastError);
		return false;
	}

	// Process received data
	*channel1NegativeOff_p = (bool)((auxBuff >> (uint8_t)(BitIndex::IN1P_OFF)) & 0x01);
	*channel1PositiveOff_p = (bool)((auxBuff >> (uint8_t)(BitIndex::IN1N_OFF)) & 0x01);
	*channel2NegativeOff_p = (bool)((auxBuff >> (uint8_t)(BitIndex::IN2P_OFF)) & 0x01);
	*channel2PositiveOff_p = (bool)((auxBuff >> (uint8_t)(BitIndex::IN2N_OFF)) & 0x01);
	*rldOff_p = (bool)((auxBuff >> (uint8_t)(BitIndex::RLD_STAT)) & 0x01);

	// Update data members
	this->_isLeadOffChannel1Positive = *channel1NegativeOff_p;
	this->_isLeadOffChannel1Negative = *channel1PositiveOff_p;
	this->_isLeadOffChannel2Positive = *channel2NegativeOff_p;
	this->_isLeadOffChannel2Negative = *channel2PositiveOff_p;
	this->_isLeadOffRld = *rldOff_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

// =============================================================================
// Class own methods - Private
// =============================================================================

//     ////////////////     CLASS CONTROL AND STATUS     ////////////////     //

/**
 * @brief		Reset internal class members.
 * @details		Reset internal class members related to the device's registers
 * 				and the internal class members used to control the device. The
 * 				function is used after a device physical reset, when the
 * 				internal registers are reset to their default values.
 */
void Ads1292r::_clearMembers(void)
{
	// Reset class members
	//     /////////////////////    ID REGISTER     /////////////////////     //
	this->_deviceId								= DeviceId::UNKNOWN;

	//     //////////////////     CONFIG1 REGISTER     //////////////////     //
	this->_continuousModeDisabled				= false;
	this->_dataRate								= DataRate::DATA_RATE_500_SPS;

	//     //////////////////     CONFIG2 REGISTER     //////////////////     //
	this->_leadOffComparatorEnabled				= false;
	this->_referenceInternalEnabled				= false;
	this->_referenceInternalValue				= IntRefValue::REF_2420_MV;
	this->_clockPinOutputEnabled				= false;
	this->_testSignal							= TestSignal::SIGNAL_OFF;

	//     ////////////////////    LOFF REGISTER     ////////////////////     //
	this->_leadOffComparatorThreshold			= LeadOffThresh::THRESH_95_0_PERCENT;
	this->_leadOffCurrentMagnitude				= LeadOffCurrMag::CURRENT_6_NA;
	this->_leadOffCurrentFrequency				= LeadOffCurrFreq::DC;

	//     ///////////////////    CH1SET REGISTER     ///////////////////     //
	this->_channel1Enabled						= true;
	this->_channel1PgaGain						= PgaGain::GAIN_6;
	this->_channel1InputSelection				= InputSelection::ELECTRODE;

	//     ///////////////////    CH2SET REGISTER     ///////////////////     //
	this->_channel2Enabled						= true;
	this->_channel2PgaGain						= PgaGain::GAIN_6;
	this->_channel2InputSelection				= InputSelection::ELECTRODE;

	//     //////////////////    RLD_SENS REGISTER     //////////////////     //
	this->_rldChopFrequency						= RldChopFreq::FMOD_DIV_16;
	this->_rldBufferEnabled						= false;
	this->_rldLeadOffEnabled					= false;
	this->_rldConnectToChannel2					= RldInpConnect::DISCONNECTED;
	this->_rldConnectToChannel1					= RldInpConnect::DISCONNECTED;

	//     /////////////////     LOFF_SENS REGISTER     /////////////////     //
	this->_channel2LeadOffCurrentSourceEnabled	= false;
	this->_channel1LeadOffCurrentSourceEnabled	= false;
	this->_channel2LeadOffNegativeEnabled		= false;
	this->_channel2LeadOffPositiveEnabled		= false;
	this->_channel1LeadOffNegativeEnabled		= false;
	this->_channel1LeadOffPositiveEnabled		= false;

	//     /////////////////     LOFF_STAT REGISTER     /////////////////     //
	this->_clockDivider							= ClockDivider::DIV_4;
	this->_isLeadOffRld							= false;
	this->_isLeadOffChannel2Negative			= false;
	this->_isLeadOffChannel2Positive			= false;
	this->_isLeadOffChannel1Negative			= false;
	this->_isLeadOffChannel1Positive			= false;

	//     ///////////////////     RESP1 REGISTER     ///////////////////     //
	this->_respDemodCircuitEnabled				= false;
	this->_respModCircuitEnabled				= false;
	this->_respPhase							= RespPhase::PHASE_0;
	this->_respExternalClockEnabled				= false;

	//     ///////////////////     RESP2 REGISTER     ///////////////////     //
	this->_offsetCalibrationEnabled				= false;
	this->_respFreq								= RespFreq::FREQ_32_KHZ;
	this->_rldReferenceInternalEnabled			= true;

	//     ////////////////////    GPIO REGISTER     ////////////////////     //
	this->_gpio2Direction						= IoOperation::READ;
	this->_gpio1Direction						= IoOperation::READ;
	this->_gpio2Value							= LogicLevel::LOW;
	this->_gpio1Value							= LogicLevel::LOW;

	//     /////////////////    OTHER CLASS MEMBERS     /////////////////     //
	this->_initialized							= false;
	this->_isShuttedDown						= false;

	// Returns successfully
	this->_lastError = Error::NONE;
	return;
}

/**
 * @brief		Checks if the device was initialized.
 * @details		Checks if the device was initialized.
 * @note		This function is private, so before calling it, the
 * 				programmer must ensure that all parameters are checked.
 * @attention	This function only verifies the device initialization, it does
 * 				not verifies if the device is configured properly.
 * @return		true		Device is initialized.
 * @return		false		Device is not initialized.
 */
bool Ads1292r::_isInitialized(void)
{
	// Checks if device is initialized
	if(!this->_initialized) {
		// Returns error
		this->_lastError = Error::NOT_INITIALIZED;
		return false;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	return true;
}

//     ////////////////    DEVICE CONTROL AND STATUS     ////////////////     //

bool Ads1292r::_reset(void)
{
	// Send RESET
	if(this->_resetPinEnabled) {
		HAL_GPIO_WritePin(this->_csPort, this->_csPin, GPIO_PIN_SET);
		HAL_Delay(1);
		HAL_GPIO_WritePin(this->_csPort, this->_csPin, GPIO_PIN_RESET);
		HAL_Delay(10);
		HAL_GPIO_WritePin(this->_csPort, this->_csPin, GPIO_PIN_SET);
	} else {
		// Check function arguments for errors
		if(!isPointerValid(this->_handler)) {			// Testing handler parameter
			// Returns error
			this->_lastError = Error::HANDLER_POINTER_NULL;
			debugMessage(Error::HANDLER_POINTER_NULL);
			return false;
		} else if(this->_handler->getHandlerType() != Handler::HandlerType::SPI) {
			// Returns error
			this->_lastError = Error::HANDLER_UNSUPPORTED;
			debugMessage(Error::HANDLER_UNSUPPORTED);
			return false;
		}
		if(!this->_sendCommand(Command::Reset)) {
			// Returns error
			debugMessage(this->_lastError);
			return false;
		}
	}
	HAL_Delay(50);

	// Returns successfully
	this->_lastError = Error::NONE;
	return true;
}

bool Ads1292r::_start(void)
{
	// FIXME: Implement _start() function
	return false;
}

bool Ads1292r::_stop(void)
{
	// FIXME: Implement _stop() function
	return false;
}

//     //////////////////    REGISTER MANIPULATION     //////////////////     //

bool Ads1292r::_readReg(Register regAddr_p, uint8_t *dataBuffer_p, uint16_t bytesToRead_p)
{
	// FIXME: Implement _readReg() function
	return false;
}

bool Ads1292r::_readAllRegs(uint8_t *regs_p)
{
	// FIXME: Implement _readAllRegs() function
	return false;
}

bool Ads1292r::_sendCommand(Command command_p)
{
	// Local variables
	uint8_t auxBuff;

	// Mark passage for debugging purpose
	debugMark("_sendCommand(Command)", 3);

	// Configure communication handler parameters
	if(!this->_handler->setDevice(*(this->_csPort), this->_csPin)) {
		// Returns error
		this->_lastError = this->_handler->getLastError();
		debugMessage(this->_lastError);
		return false;
	}

	// Send command to device
	auxBuff = (uint8_t)(command_p);
	if(!this->_handler->sendData(&auxBuff, 1)) {
		// Returns error
		this->_lastError = this->_handler->getLastError();
		debugMessage(this->_lastError);
		return false;
	}

	// Delay
	customDelay();
	if(command_p == Command::Reset) {
		HAL_Delay(50);
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

bool Ads1292r::_writeReg(Register regAddr_p, uint8_t *dataBuffer_p, uint16_t bytesToWrite_p)
{
	// Local variables
	uint8_t auxBuff[15];
	uint8_t index = 0;

	// Mark passage for debugging purpose
	debugMark("_writeReg(Register, uint8_t *, uint16_t)", 3);

	// Prepare data to send
	auxBuff[index++] = ADS1292R_WRITE_REG | (uint8_t)(regAddr_p);
	auxBuff[index++] = 0x00 + (bytesToWrite_p - 1);
	for(uint8_t j = 0; j < bytesToWrite_p; j++) {
		auxBuff[index++] = dataBuffer_p[j];
	}

	// Configure communication handler parameters
	if(!this->_handler->setDevice(*(this->_csPort), this->_csPin)) {
		// Returns error
		this->_lastError = this->_handler->getLastError();
		debugMessage(this->_lastError);
		return false;
	}

	// Send command to device
	if(!this->_handler->sendData(auxBuff, index)) {
		// Returns error
		this->_lastError = this->_handler->getLastError();
		debugMessage(this->_lastError);
		return false;
	}
	customDelay();

	// Returns successfully
	this->_lastError = Error::NONE;
	debugMessage(Error::NONE);
	return true;
}

// =============================================================================
// Class own methods - Protected
// =============================================================================

// NONE

// =============================================================================
// Static functions definitions
// =============================================================================

// NONE

/* =============================================================================
 * End of file
 * ========================================================================== */
