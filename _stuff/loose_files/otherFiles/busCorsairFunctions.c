
#include "busCorsairConf.h"

busCorsair_t busCorsair;

// -------------------------------------------------------------
// STRUCT HANDLER FUNCTIONS
// -------------------------------------------------------------

void bcResetData(void)
{
	busCorsair.menu = BC_MENU_HIZ;
	busCorsair.randMin = 0;
	busCorsair.randMax = 65535;
	busCorsair.paramData[0] = NULL;
	busCorsair.paramData[1] = NULL;
	busCorsair.paramData[2] = NULL;
	busCorsair.paramData[3] = NULL;
	busCorsair.paramData[4] = NULL;
	busCorsair.paramData[5] = NULL;
	busCorsair.paramData[6] = NULL;
	busCorsair.paramData[7] = NULL;
	busCorsair.paramNum = 0;
	busCorsair.pwmFrequency = 1000;
	busCorsair.pwmDutty[0] = 0xFF;
	busCorsair.pwmDutty[1] = 0xFF;
	busCorsair.pwmDutty[2] = 0xFF;
	busCorsair.pwmDutty[3] = 0xFF;
	busCorsair.pwmDutty[4] = 0xFF;
	busCorsair.threshold[0] = 0xFFFF;
	busCorsair.threshold[1] = 0xFFFF;
	busCorsair.threshold[2] = 0xFFFF;
	busCorsair.threshold[3] = 0xFFFF;
	busCorsair.threshold[4] = 0xFFFF;
	busCorsair.prescaler = TIMER1_CLOCK_DISABLE;
	busCorsair.top = 2000;
	busCorsair.pwmOn = 0;

	return;
}

// -------------------------------------------------------------
// COMMAND PARSE
// -------------------------------------------------------------

void bcParseCommand(char * strCom)
{
	char * token = NULL;
	uint8_t i = 0;

	// Parse command
	token = strtok(strCom, " ");
	strCom = token;
	// Parse parameters
	token = strtok(NULL, " ");
	while((token != NULL) && (i < 8)) {
		busCorsair.paramData[i++] = token;
		token = strtok(NULL, " ");
	}
	busCorsair.paramNum = i;

	// Help command
	if(strcmp(strCom, "?") == 0) {
		bcExecuteCommandHelp();
		return;
	}
	// Aux pin output low command
	if(strcmp(strCom, "a") == 0) {
		bcAuxPinSetLevelLow();
		return;
	}
	// Aux pin output high command
	if(strcmp(strCom, "A") == 0) {
		bcAuxPinSetLevelHigh();
		return;
	}
	// Aux pin read logic level command
	if(strcmp(strCom, "@") == 0) {
		bcAuxPinReadLevel();
		return;
	}
	// Aux pin ADC once command
	if(strcmp(strCom, "d") == 0) {
		bcAuxPinReadVoltageOnce();
		return;
	}
	// Aux pin ADC continuously command
	if(strcmp(strCom, "D") == 0) {
		bcAuxPinReadVoltageContinuously();
		return;
	}
	// Aux pin read frequency once command
	if(strcmp(strCom, "f") == 0) {
		bcAuxPinReadFrequencyOnce();
		return;
	}
	// Aux pin read frequency continuously command
	if(strcmp(strCom, "F") == 0) {
		bcAuxPinReadFrequencyContinuously();
		return;
	}
	// Aux pin generate PWM at pin command
	if(strcmp(strCom, "g") == 0) {
		bcAuxPinPwmDutty();
		return;
	}
	// Aux change PWM frequency command
	if(strcmp(strCom, "G") == 0) {
		bcAuxPinPwmFreq();
		return;
	}
	// Change baud rate command
	if(strcmp(strCom, "b") == 0) {
		bcChangeBaudRate();
		return;
	}
























	// Change mode command
	if(strcmp(strCom, "m") == 0) {
		// XXX_ Command
		return;
	}
	// Disable pull-ups command
	if(strcmp(strCom, "p") == 0) {
		// XXX_ Command
		return;
	}
	// Enable pull-ups command
	if(strcmp(strCom, "P") == 0) {
		// XXX_ Command
		return;
	}
	// Generate random number command
	if(strcmp(strCom, "r") == 0) {
		// XXX_ Command
		return;
	}
	// Set random number limits command
	if(strcmp(strCom, "R") == 0) {
		// XXX_ Command
		return;
	}
	// Show pin voltages source command
	if(strcmp(strCom, "v") == 0) {
		// XXX_ Command
		return;
	}
	// Enable target power source command
	if(strcmp(strCom, "W") == 0) {
		// XXX_ Command
		return;
	}
	// Disable target power source command
	if(strcmp(strCom, "w") == 0) {
		// XXX_ Command
		return;
	}
	// List macros command
	if(strcmp(strCom, "(0)") == 0) {
		// XXX_ Command
		return;
	}
	// Execute macro command
	// Unknown command
	// XXX_ Command

	return;
}

// -------------------------------------------------------------
// HIGH LEVEL EXECUTION (COMMAND LEVEL)
// -------------------------------------------------------------

void bcExecuteCommandHelp(void)
{
	uint8_t i = 0;

	for(i = 0; i < bcHelpMenuSize; i++) {
		printf_P(bcHelpMenuStringPtr[i]);
	}

	return;
}

void bcAuxPinSetLevelHigh(void)
{
	uint8_t pin = 0;

	if(busCorsair.paramNum != 1) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_USAGE_AUX_PIN], 'A');
		return;
	}
	if(!strToUint8(busCorsair.paramData[0], &pin)) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_PARAMETER_X_INCORRECT], 1);
		return;
	}
	if(pin > 4) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_PARAMETER_X_INCORRECT], 1);
		return;
	}
	bcAuxPinPwmContTurnOff(&pin);
	setBit(BC_AUX_PORT, pin);
	setBit(BC_AUX_DDR, pin);
	printf_P(bcParamStringPtr[BC_PARAM_STR_AUX_PIN_X_SET_TO_X_X], pin, "OUTPUT", "HIGH");

	return;
}

void bcAuxPinSetLevelLow(void)
{
	uint8_t pin = 0;

	if(busCorsair.paramNum != 1) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_USAGE_AUX_PIN], 'a');
		return;
	}
	if(!strToUint8(busCorsair.paramData[0], &pin)) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_PARAMETER_X_INCORRECT], 1);
		return;
	}
	if(pin > 4) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_PARAMETER_X_INCORRECT], 1);
		return;
	}
	bcAuxPinPwmContTurnOff(&pin);
	clrBit(BC_AUX_PORT, pin);
	setBit(BC_AUX_DDR, pin);
	printf_P(bcParamStringPtr[BC_PARAM_STR_AUX_PIN_X_SET_TO_X_X], pin, "OUTPUT", "LOW");

	return;
}

void bcAuxPinReadLevel(void)
{
	uint8_t pin = 0;
	logic_t level;

	if(busCorsair.paramNum != 1) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_USAGE_AUX_PIN], '@');
		return;
	}
	if(!strToUint8(busCorsair.paramData[0], &pin)) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_PARAMETER_X_INCORRECT], 1);
		return;
	}
	if(pin > 4) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_PARAMETER_X_INCORRECT], 1);
		return;
	}
	bcAuxPinPwmContTurnOff(&pin);
	clrBit(BC_AUX_DDR, pin);
	clrBit(BC_AUX_PORT, pin);
	asm volatile("nop");
	level = isBitSet(BC_AUX_PIN, pin);

	if(level == HIGH) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_AUX_PIN_X_IN_LOGIC_LEVEL_X], pin, "HIGH");
	} else {
		printf_P(bcParamStringPtr[BC_PARAM_STR_AUX_PIN_X_IN_LOGIC_LEVEL_X], pin, "LOW");
	}

	return;
}

void bcAuxPinReadVoltageOnce(void)
{
	uint8_t pin = 0;
	uint16_t adcValue = 0;
	uint32_t aux32 = 5000;
	uint16_t voltage = 0;

	if(busCorsair.paramNum != 1) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_USAGE_AUX_PIN], 'd');
		return;
	}
	if(!strToUint8(busCorsair.paramData[0], &pin)) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_PARAMETER_X_INCORRECT], 1);
		return;
	}
	if(pin > 4) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_PARAMETER_X_INCORRECT], 1);
		return;
	}
	bcAuxPinPwmContTurnOff(&pin);
	clrBit(BC_AUX_DDR, pin);
	clrBit(BC_AUX_PORT, pin);

	// ADC configuration
	adcConfig(ADC_MODE_SINGLE_CONVERSION, ADC_REFERENCE_POWER_SUPPLY, ADC_PRESCALER_128);
	adcSelectChannel(pin);
	adcDeactivateInterrupt();
	adcEnable();
	adcStartConversion();
	adcWaitUntilConversionFinish();		// Ignoring first conversion
	adcValue = ADC;
	adcStartConversion();
	adcWaitUntilConversionFinish();
	adcValue = ADC;
	adcDisable();						// Disabling ADC
	adcConfig(ADC_MODE_SINGLE_CONVERSION, ADC_REFERENCE_POWER_SUPPLY, ADC_PRESCALER_OFF);

	aux32 *= (uint32_t)adcValue;
	aux32 /= 1024;
	voltage = (uint16_t)aux32;
	printf_P(bcParamStringPtr[BC_PARAM_STR_AUX_PIN_VOLTAGE_IS_X_X], (voltage / 1000), (voltage % 1000));

	return;
}

void bcAuxPinReadVoltageContinuously(void)
{
	/*
	uint8_t pin = 0;
	uint16_t adcValue = 0;
	uint32_t aux32 = 5000;
	uint16_t voltage = 0;

	if(busCorsair.paramNum != 1) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_USAGE_AUX_PIN], 'D');
		return;
	}
	if(!strToUint8(busCorsair.paramData[0], &pin)) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_PARAMETER_X_INCORRECT], 1);
		return;
	}
	if(pin > 4) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_PARAMETER_X_INCORRECT], 1);
		return;
	}
	bcAuxPinPwmContTurnOff();
	clrBit(BC_AUX_DDR, pin);
	clrBit(BC_AUX_PORT, pin);
	busCorsair.auxPinFunc[pin] = BC_AUX_PIN_FUNC_INPUT_TRISTATE;

	// ADC configuration
	adcConfig(ADC_MODE_SINGLE_CONVERSION, ADC_REFERENCE_POWER_SUPPLY, ADC_PRESCALER_128);
	adcSelectChannel(pin);
	adcDeactivateInterrupt();
	adcEnable();
	adcStartConversion();
	adcWaitUntilConversionFinish();		// Ignoring first conversion
	adcValue = ADC;
	adcStartConversion();
	adcWaitUntilConversionFinish();
	adcValue = ADC;
	adcDisable();						// Disabling ADC
	adcConfig(ADC_MODE_SINGLE_CONVERSION, ADC_REFERENCE_POWER_SUPPLY, ADC_PRESCALER_OFF);

	aux32 *= (uint32_t)adcValue;
	aux32 /= 1024;
	voltage = (uint16_t)aux32;
	printf_P(bcParamStringPtr[BC_PARAM_STR_AUX_PIN_VOLTAGE_IS_X_X], (voltage / 1000), (voltage % 1000));
	*/
	return;
}

void bcAuxPinReadFrequencyOnce(void)
{
	return;
}

void bcAuxPinReadFrequencyContinuously(void)
{
	return;
}

void bcAuxPinPwmDutty(void)
{
	uint8_t pin = 0;
	uint8_t dutty = 0;
	uint32_t aux32 = 0;

	if(busCorsair.paramNum != 2) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_USAGE_AUX_PWM_DUTTY], 'g');
		return;
	}
	if(!strToUint8(busCorsair.paramData[0], &pin)) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_PARAMETER_X_INCORRECT], 1);
		return;
	}
	if(pin > 4) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_PARAMETER_X_INCORRECT], 1);
		return;
	}
	if(!strToUint8(busCorsair.paramData[1], &dutty)) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_PARAMETER_X_INCORRECT], 2);
		return;
	}
	if(dutty > 100) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_PARAMETER_X_INCORRECT], 2);
		return;
	}

	if(dutty == 0) {			// Turn Off
		busCorsair.threshold[pin] = 0xFFFF;
		busCorsair.pwmDutty[pin] = 0xFF;
		clrBit(busCorsair.pwmOn, pin);
		clrBit(PORTC, pin);
		clrBit(DDRC, pin);
		printf_P(bcParamStringPtr[BC_PARAM_STR_PWM_AUX_PIN_X_TURNED_OFF], pin);
		return;
	}

	aux32 = (uint32_t)dutty * (uint32_t)busCorsair.top;
	aux32 /= 100;
	busCorsair.threshold[pin] = (uint16_t)aux32;
	busCorsair.pwmDutty[pin] = dutty;
	setBit(busCorsair.pwmOn, pin);
	setBit(PORTC, pin);
	setBit(DDRC, pin);

	return;
}

void bcAuxPinPwmFreq(void)
{
	uint8_t i;
	uint8_t aux8 = 0;
	uint16_t freq = 0;
	uint16_t prescaler = 1024;
	timer1PrescalerValue_t prescalerLabel = TIMER1_PRESCALER_1024;
	uint32_t top = 16000000;
	uint32_t aux32;

	if(busCorsair.paramNum != 1) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_USAGE_AUX_PWM_FREQ], 'G');
		return;
	}
	if(!strToUint16(busCorsair.paramData[0], &freq)) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_PARAMETER_X_INCORRECT], 1);
		return;
	}
	if((freq == 0) || (freq > 16000)) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_PARAMETER_X_INCORRECT], 1);
		return;
	}

	if(freq <= 15) {
		prescaler = 1024;
		prescalerLabel = TIMER1_PRESCALER_1024;
	} else if(freq <= 62) {
		prescaler = 256;
		prescalerLabel = TIMER1_PRESCALER_256;
	} else if(freq <= 250) {
		prescaler = 64;
		prescalerLabel = TIMER1_PRESCALER_64;
	} else if(freq <= 2000) {
		prescaler = 8;
		prescalerLabel = TIMER1_PRESCALER_8;
	} else {
		prescaler = 1;
		prescalerLabel = TIMER1_PRESCALER_1;
	}
	top /= freq;
	top /= prescaler;
	busCorsair.top = (uint16_t)top;
	busCorsair.prescaler = prescalerLabel;
	busCorsair.pwmFrequency = freq;

	timer1Config(TIMER1_MODE_CTC_OCRA, prescalerLabel);
	timer1SetCompareAValue((uint16_t)top);
	printf_P(bcParamStringPtr[BC_PARAM_STR_PWM_FREQUENCY_SET_TO_X], freq);

	aux8 = 0;
	for(i = 0; i < 5; i++) {
		if(busCorsair.pwmDutty[i] < 101) {
			aux32 = (uint32_t)busCorsair.pwmDutty[i] * (uint32_t)busCorsair.top;
			aux32 /= 100;
			busCorsair.threshold[i] = (uint16_t)aux32;
			aux8++;
			printf_P(bcParamStringPtr[BC_PARAM_STR_PWM_AUX_PIN_X_ADJUSTED], i);
		}
	}
	if(aux8 == 0) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_PWM_NO_AUX_PINS_WERE_AFFECTED]);
	}

	return;
}

void bcAuxPinPwmContTurnOff(uint8_t * pin)
{
	if(isBitSet(busCorsair.pwmOn, (*pin))) {
		clrBit(busCorsair.pwmOn, (*pin));
	}

	return;
}

void bcChangeBaudRate(void)
{
	uint16_t baudRate = 0;
	uint32_t aux32 = 0;
	uint16_t aux16 = 0;

	if(busCorsair.paramNum != 1) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_USAGE_CHANGE_BAUD_RATE], 'b');
		return;
	}
	if(!strToUint16(busCorsair.paramData[0], &baudRate)) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_PARAMETER_X_INCORRECT], 1);
		return;
	}
	if((baudRate > 57600) || (baudRate < 9600)) {
		printf_P(bcParamStringPtr[BC_PARAM_STR_PARAMETER_X_INCORRECT], 1);
		return;
	}

	aux32 = 1000000;
	aux32 /= (uint32_t)baudRate;
	aux16 = (uint16_t)aux32;
	aux16 -= 1;
	printf_P(bcPlainStringPtr[BC_PLAIN_STR_BAUD_RATE_MESSAGE_1]);
	printf_P(bcPlainStringPtr[BC_PLAIN_STR_BAUD_RATE_MESSAGE_2]);
	printf_P(bcPlainStringPtr[BC_PLAIN_STR_BAUD_RATE_MESSAGE_3]);
	UBRR0 = aux16;

	return;
}
