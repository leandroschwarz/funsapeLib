// -----------------------------------------------------------------------------
// Arquivo:			funcoes_LCD.h
// Modulo:			Fun��es de ajuste de intensidade e economia de energia do LCD
// Autor:			Leandro Schwarz
// Vers�o:			1.0
// Modificado em:	20/06/2012
// -----------------------------------------------------------------------------

#ifndef __FUNCOES_LCD_H
#define __FUNCOES_LCD_H 10

// -----------------------------------------------------------------------------
// Arquivos de cabe�alhos ------------------------------------------------------

#include "defines.h"
#include "LS_defines.h"
#include "LS_LCD.h"
#include "LS_keypad.h"
#include <avr/eeprom.h>

// -----------------------------------------------------------------------------
// Defini��es ------------------------------------------------------------------

#define EEPROM_LCD_BACKLIGHT_INTENSITY_ADDRESS	(void *)0x0000
#define EEPROM_LCD_ECONOMY_MODE_ADDRESS			(void *)0x0001

// -----------------------------------------------------------------------------
// Declara��o de fun��es -------------------------------------------------------

unsigned char ajusta_intensidade_LCD(void);
unsigned char ajusta_economia_energia_LCD(void);

#endif
