// -----------------------------------------------------------------------------
// Arquivo:			ACELEROMETRO.h
// Modulo:			Acelerômetro
// Autores:			Leandro Schwarz
//					Mayara de Sousa
// Versão:			1.5
// Modificado em:	15/02/2012
// Observações:		Para utilizar, é necessário alterar os valor das constantes
//					ACEL_S0, ACEL_S1, ACEL_SLEEP, ACEL_CONTROLE	PORTx, definidas 
//					na seqüência
// -------------------------------------------------------------------------

#ifndef __ACELEROMETRO_H
#define __ACELEROMETRO_H

// Arquivos necessários --------------------------------------------------------

#include "defines.h"
#include <stdio.h>
// -------------------------------------------------------------------------
// Definições configuráveis ------------------------------------------------

#define ACEL_S0			PC3
#define ACEL_S1			PC4
#define ACEL_SLEEP		PC5
#define ACEL_CONTROLE	PORTC
//Eixo x está em ADC0
//Eixo y está em ADC1
//Eixo z está em ADC2



// -------------------------------------------------------------------------
// Definições das funções macro necessárias --------------------------------

#define __acel_set_bit(endereco,bit) (endereco|=(1 << bit))
#define __acel_clr_bit(endereco,bit) (endereco&=~(1 << bit))
#define __acel_tst_bit(endereco,bit) ((endereco>>bit)&1)
#define __acel_cpl_bit(endereco,bit) (endereco^=(1 << bit))
#define pulso_enable 	_delay_us(1); __lcd_set_bit(LCD_CONTROLE,LCD_E); _delay_us(1); __lcd_clr_bit(LCD_CONTROLE,LCD_E); _delay_us(45)

// -------------------------------------------------------------------------
// Definições das funções macro de controle --------------------------------

#define ACEL_desativa_modo_sleep()		__acel_set_bit(ACEL_CONTROLE,ACEL_SLEEP)
#define ACEL_rang_1_5g()				__acel_clr_bit(ACEL_CONTROLE,ACEL_S0);__acel_clr_bit(ACEL_CONTROLE,ACEL_S1)
#define ACEL_rang_2g()					__acel_set_bit(ACEL_CONTROLE,ACEL_S0);__acel_clr_bit(ACEL_CONTROLE,ACEL_S1)
#define ACEL_rang_4g()					__acel_clr_bit(ACEL_CONTROLE,ACEL_S0);__acel_set_bit(ACEL_CONTROLE,ACEL_S1)
#define ACEL_rang_6g()					__acel_set_bit(ACEL_CONTROLE,ACEL_S0);__acel_set_bit(ACEL_CONTROLE,ACEL_S1)


// -------------------------------------------------------------------------
// Declaração das funções da bibioteca -------------------------------------


#endif
