/*
 * busCorsairMessages.c
 *
 * Created: 11/04/2018 17:43:20
 *  Author: Schwarz
 */

#include "busCorsairConf.h"

// Plain strings
const char bcPlainString0Ptr[] PROGMEM = "Running initialization procedure!\r";
const char bcPlainString1Ptr[] PROGMEM = "Initialization procedure finished!\r";
const char bcPlainString2Ptr[] PROGMEM = "[OK!] USART configured!\r";
const char bcPlainString3Ptr[] PROGMEM = "[OK!] User interface configured!\r";
const char bcPlainString4Ptr[] PROGMEM = "[ERR] User interface configuration failed!\r";
const char bcPlainString5Ptr[] PROGMEM = "[OK!] Random generator initiated!\r";
const char bcPlainString6Ptr[] PROGMEM = "[OK!] Circular Buffer created!\r";
const char bcPlainString7Ptr[] PROGMEM = "[ERR]Circular buffer creation failed!\r";
const char bcPlainString8Ptr[] PROGMEM = "[OPS] Strange... Let's pretend it didn't happened!\r";
const char bcPlainString9Ptr[] PROGMEM = "\r--------------------------------------\r";
const char bcPlainString10Ptr[] PROGMEM = "BUS Corsair Help Screen";
const char bcPlainString11Ptr[] PROGMEM = "COMMAND LIST\r";
const char bcPlainString12Ptr[] PROGMEM = "?\tThis help screen\r";
const char bcPlainString13Ptr[] PROGMEM = "a/A/@\tAUX pin(low / HIGH / READ\r";
const char bcPlainString14Ptr[] PROGMEM = "b\tChange baudrate\r";
const char bcPlainString15Ptr[] PROGMEM = "d/D\tMeasure analog voltage at AUX pin (once / CONTINUOUSLY)\r";
const char bcPlainString16Ptr[] PROGMEM = "f/F\tMeasure frequency at AUX pin (once / CONTINUOUSLY)\r";
const char bcPlainString17Ptr[] PROGMEM = "g/G\tPWM signal at AUX pin (pin and dutty/SET FREQUENCY)\r";
const char bcPlainString18Ptr[] PROGMEM = "m\tChange mode\r";
const char bcPlainString19Ptr[] PROGMEM = "p/P\tPull - up resistors(off / ON)\r";
const char bcPlainString20Ptr[] PROGMEM = "r/R\tRandom number(generate / SET LIMITS)\r";
const char bcPlainString21Ptr[] PROGMEM = "v\tShow pin voltages / states\r";
const char bcPlainString22Ptr[] PROGMEM = "w/W\tTarget Power Source(off / ON)\r";
const char bcPlainString23Ptr[] PROGMEM = "(0)\tList macros\r";
const char bcPlainString24Ptr[] PROGMEM = "(x)\tExecute macro\r\r";
const char bcPlainString25Ptr[] PROGMEM = "Warning: the baud rate will be permanently changed!\r";
const char bcPlainString26Ptr[] PROGMEM = "If you want to load baud rate default (57600), reset the board with MISO and MOSI connected together!\r";
const char bcPlainString27Ptr[] PROGMEM = "You may adjust your terminal now. Press any key!\r";
char const * bcHelpMenuStringPtr[] = {
	bcPlainString9Ptr,
	bcPlainString10Ptr,
	bcPlainString9Ptr,
	bcPlainString11Ptr,
	bcPlainString12Ptr,
	bcPlainString13Ptr,
	bcPlainString14Ptr,
	bcPlainString15Ptr,
	bcPlainString16Ptr,
	bcPlainString17Ptr,
	bcPlainString18Ptr,
	bcPlainString19Ptr,
	bcPlainString20Ptr,
	bcPlainString21Ptr,
	bcPlainString22Ptr,
	bcPlainString23Ptr,
	bcPlainString24Ptr
};
const uint8_t bcHelpMenuSize = BC_PLAIN_STR_HELP_COMMAND_EXEC_MACRO - BC_PLAIN_STR_HELP_BAR + 2;
char const * bcPlainStringPtr[] = {
	bcPlainString0Ptr,
	bcPlainString1Ptr,
	bcPlainString2Ptr,
	bcPlainString3Ptr,
	bcPlainString4Ptr,
	bcPlainString5Ptr,
	bcPlainString6Ptr,
	bcPlainString7Ptr,
	bcPlainString8Ptr,
	bcPlainString9Ptr,
	bcPlainString10Ptr,
	bcPlainString11Ptr,
	bcPlainString12Ptr,
	bcPlainString13Ptr,
	bcPlainString14Ptr,
	bcPlainString15Ptr,
	bcPlainString16Ptr,
	bcPlainString17Ptr,
	bcPlainString18Ptr,
	bcPlainString19Ptr,
	bcPlainString20Ptr,
	bcPlainString21Ptr,
	bcPlainString22Ptr,
	bcPlainString23Ptr,
	bcPlainString24Ptr,
	bcPlainString25Ptr,
	bcPlainString26Ptr,
	bcPlainString27Ptr
};

// Parametric strings
const char bcParamString0Ptr[] PROGMEM = "\rBUS Corsair - v %s (" __DATE__ ", " __TIME__")\r\r";
const char bcParamString1Ptr[] PROGMEM = "Wrong command syntax - USAGE: %c pinNum(0-4)\r";
const char bcParamString2Ptr[] PROGMEM = "Wrong command syntax - USAGE: %c pinNum(0-4) dutty(0-100)\r";
const char bcParamString3Ptr[] PROGMEM = "Wrong command syntax - USAGE: %c freq(1-16000)\r";
const char bcParamString4Ptr[] PROGMEM = "Parameter %d is incorrect - type command to see USAGE\r";
const char bcParamString5Ptr[] PROGMEM = "AUX pin %d set to %s %s\r";
const char bcParamString6Ptr[] PROGMEM = "AUX pin %d is in logic level %s\r";
const char bcParamString7Ptr[] PROGMEM = "Voltage in AUX pin is %01d.%03d V\r";
const char bcParamString8Ptr[] PROGMEM = "PWM frequency set to %u Hz\r";
const char bcParamString9Ptr[] PROGMEM = "AUX pin %d is now responding to the new frequency\r";
const char bcParamString10Ptr[] PROGMEM = "No AUX pins were affected\r";
const char bcParamString11Ptr[] PROGMEM = "PWM at AUX pin %d was turned off\r";
const char bcParamString12Ptr[] PROGMEM = "Wrong command syntax - USAGE: %c baud rate(9600-57600)\r";
char const * bcParamStringPtr[] = {
	bcParamString0Ptr,
	bcParamString1Ptr,
	bcParamString2Ptr,
	bcParamString3Ptr,
	bcParamString4Ptr,
	bcParamString5Ptr,
	bcParamString6Ptr,
	bcParamString7Ptr,
	bcParamString8Ptr,
	bcParamString9Ptr,
	bcParamString10Ptr,
	bcParamString11Ptr,
	bcParamString12Ptr
};
