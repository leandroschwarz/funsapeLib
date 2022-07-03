/*
 * IncFile1.h
 *
 * Created: 10/04/2018 15:46:52
 *  Author: Schwarz
 */

#ifndef __BUS_CORSAIR_CONF_H
#define __BUS_CORSAIR_CONF_H 1

#include "globalDefines.h"
#include "adc.h"
#include "timer1.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "strConv.h"

#define FIRMWARE_VERSION 1.0

#define BC_AUX_DDR		DDRC
#define BC_AUX_PORT		PORTC
#define BC_AUX_PIN		PINC

typedef enum bcPlainString_e {
	BC_PLAIN_STR_INITIALIZATION_STARTED		= 0,
	BC_PLAIN_STR_INITIALIZATION_FINISHED	= 1,
	BC_PLAIN_STR_USART_CONFIGURED_OK		= 2,
	BC_PLAIN_STR_USER_INTERFACE_OK			= 3,
	BC_PLAIN_STR_USER_INTERFACE_ERR			= 4,
	BC_PLAIN_STR_RANDOM_GENERATOR_OK		= 5,
	BC_PLAIN_STR_CIRCULAR_BUFFER_OK			= 6,
	BC_PLAIN_STR_CIRCULAR_BUFFER_ERR		= 7,
	BC_PLAIN_STR_STRANGE_ERROR				= 8,
	BC_PLAIN_STR_HELP_BAR					= 9,
	BC_PLAIN_STR_HELP_BUS_CORSAIR			= 10,
	BC_PLAIN_STR_HELP_LIST_COMMANDS			= 11,
	BC_PLAIN_STR_HELP_COMMAND_HELP			= 12,
	BC_PLAIN_STR_HELP_COMMAND_AUX			= 13,
	BC_PLAIN_STR_HELP_COMMAND_BAUD			= 14,
	BC_PLAIN_STR_HELP_COMMAND_ADC			= 15,
	BC_PLAIN_STR_HELP_COMMAND_FREQ			= 16,
	BC_PLAIN_STR_HELP_COMMAND_PWM			= 17,
	BC_PLAIN_STR_HELP_COMMAND_MODE			= 18,
	BC_PLAIN_STR_HELP_COMMAND_PULL_UP		= 19,
	BC_PLAIN_STR_HELP_COMMAND_RANDOM		= 20,
	BC_PLAIN_STR_HELP_COMMAND_VOLTAGES		= 21,
	BC_PLAIN_STR_HELP_COMMAND_POWER_CTRL	= 22,
	BC_PLAIN_STR_HELP_COMMAND_LIST_MACROS	= 23,
	BC_PLAIN_STR_HELP_COMMAND_EXEC_MACRO	= 24,
	BC_PLAIN_STR_BAUD_RATE_MESSAGE_1		= 25,
	BC_PLAIN_STR_BAUD_RATE_MESSAGE_2		= 26,
	BC_PLAIN_STR_BAUD_RATE_MESSAGE_3		= 27
} bcPlainString_e;

typedef enum bcParamString_e {
	BC_PARAM_STR_FIRMWARE_INFO					= 0,
	BC_PARAM_STR_USAGE_AUX_PIN					= 1,
	BC_PARAM_STR_USAGE_AUX_PWM_DUTTY			= 2,
	BC_PARAM_STR_USAGE_AUX_PWM_FREQ				= 3,
	BC_PARAM_STR_PARAMETER_X_INCORRECT			= 4,
	BC_PARAM_STR_AUX_PIN_X_SET_TO_X_X			= 5,
	BC_PARAM_STR_AUX_PIN_X_IN_LOGIC_LEVEL_X		= 6,
	BC_PARAM_STR_AUX_PIN_VOLTAGE_IS_X_X			= 7,
	BC_PARAM_STR_PWM_FREQUENCY_SET_TO_X			= 8,
	BC_PARAM_STR_PWM_AUX_PIN_X_ADJUSTED			= 9,
	BC_PARAM_STR_PWM_NO_AUX_PINS_WERE_AFFECTED	= 10,
	BC_PARAM_STR_PWM_AUX_PIN_X_TURNED_OFF		= 11,
	BC_PARAM_STR_USAGE_CHANGE_BAUD_RATE			= 12
} bcParamString_e;

typedef enum bcMenu_e {
	BC_MENU_HIZ		= 0,
	BC_MENU_ADC		= 1
} bcMenu_e;

typedef enum bcAuxPinOption_t {
	BC_AUX_PIN_OPT_OUTPUT_LOW	= 0,
	BC_AUX_PIN_OPT_OUTPUT_HIGH	= 1,
	BC_AUX_PIN_OPT_INPUT_READ	= 2,
	BC_AUX_PIN_OPT_ADC_SINGLE	= 3,
	BC_AUX_PIN_OPT_ADC_CONT		= 4,
	BC_AUX_PIN_OPT_INPUT_FREQ	= 5,
	BC_AUX_PIN_OPT_OUTPUT_PWM	= 6,
	// Number of elements
	BC_AUX_PIN_OPT_COUNT		= 7
} bcAuxPinOption_t;

typedef struct busCorsair_t {
	bcMenu_e				menu;
	uint16_t				randMin;
	uint16_t				randMax;
	char *					paramData[8];
	uint8_t					paramNum;
	uint16_t				pwmFrequency;
	uint8_t					pwmDutty[5];
	uint16_t				threshold[5];
	timer1PrescalerValue_t	prescaler;
	uint16_t				top;
	uint8_t					pwmOn : 5;
} busCorsair_t;

extern char const * bcHelpMenuStringPtr[];
extern const uint8_t bcHelpMenuSize;
extern char const * bcPlainStringPtr[];
extern char const * bcParamStringPtr[];
extern busCorsair_t busCorsair;

void bcParseCommand(char * strCom);
void bcResetData(void);

void bcExecuteCommandHelp(void);
bool_t bcExecuteCommandAuxPin(void);
bool_t bcExecuteCommandSetPwmFrequency(void);

void bcAuxPinSetLevelHigh(void);
void bcAuxPinSetLevelLow(void);
void bcAuxPinReadLevel(void);
void bcAuxPinReadVoltageOnce(void);
void bcAuxPinReadVoltageContinuously(void);
void bcAuxPinReadFrequencyOnce(void);
void bcAuxPinReadFrequencyContinuously(void);
void bcAuxPinPwmDutty(void);
void bcAuxPinPwmFreq(void);
void bcChangeBaudRate(void);

void bcAuxPinPwmContTurnOff(uint8_t * pin);

#endif
