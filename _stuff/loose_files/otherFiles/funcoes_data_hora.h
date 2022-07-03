// -----------------------------------------------------------------------------
// Arquivo:			funcoes_data_hora.h
// Modulo:			Funções de ajuste de data e hora com o RTC e LCD
// Autor:			Leandro Schwarz
// Versão:			1.0
// Modificado em:	09/06/2012
// -----------------------------------------------------------------------------

#ifndef __FUNCOES_DATA_HORA_H
#define __FUNCOES_DATA_HORA_H 10

// -----------------------------------------------------------------------------
// Arquivos de cabeçalhos ------------------------------------------------------

#include "defines.h"
#include "LS_defines.h"
#include "LS_LCD.h"
//#include "LS_DS1307.h"
#include "LS_DS1337.h"
#include "LS_keypad.h"

// -----------------------------------------------------------------------------
// Declaração de funções -------------------------------------------------------

unsigned char ajusta_hora(void);
unsigned char ajusta_data(void);
void imprime_dia_da_semana(unsigned char dia_semana);

#endif
