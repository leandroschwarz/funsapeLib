// -----------------------------------------------------------------------------
// Arquivo:			funcoes_data_hora.h
// Modulo:			Fun��es de ajuste de data e hora com o RTC e LCD
// Autor:			Leandro Schwarz
// Vers�o:			1.0
// Modificado em:	09/06/2012
// -----------------------------------------------------------------------------

#ifndef __FUNCOES_DATA_HORA_H
#define __FUNCOES_DATA_HORA_H 10

// -----------------------------------------------------------------------------
// Arquivos de cabe�alhos ------------------------------------------------------

#include "defines.h"
#include "LS_defines.h"
#include "LS_LCD.h"
//#include "LS_DS1307.h"
#include "LS_DS1337.h"
#include "LS_keypad.h"

// -----------------------------------------------------------------------------
// Declara��o de fun��es -------------------------------------------------------

unsigned char ajusta_hora(void);
unsigned char ajusta_data(void);
void imprime_dia_da_semana(unsigned char dia_semana);

#endif
