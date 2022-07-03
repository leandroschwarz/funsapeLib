// -----------------------------------------------------------------------------
// Arquivo:			defines.h
// Modulo:			Definições para o projeto
// Autor:			Leandro Schwarz
// Modificado em:	08/11/2011
// -----------------------------------------------------------------------------

#ifndef __DEFINE_H
#define __DEFINE_H

// -----------------------------------------------------------------------------
// Definições das funções macro necessárias ------------------------------------

#define set_bit(endereco,bit) (endereco |= (1 << bit))
#define clr_bit(endereco,bit) (endereco &= ~(1 << bit))
#define tst_bit(endereco,bit) ((endereco >> bit) & 1)
#define cpl_bit(endereco,bit) (endereco ^= (1 << bit))

// -----------------------------------------------------------------------------
// Definições para a CPU -------------------------------------------------------

#ifndef F_CPU
	#define F_CPU 16000000
#endif
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>

// -----------------------------------------------------------------------------
// Definições para o LCD -------------------------------------------------------

#define LCD_MODO_4_BITS
#define LCD_DADOS_PIN		PIND
#define LCD_DADOS_PORT		PORTD
#define LCD_CONTROLE_PIN	PIND
#define LCD_CONTROLE_PORT	PORTD
#define LCD_RS				PD2
#define LCD_RW				PD2
#define LCD_E				PD3
#define LCD_D4				PD4
#define LCD_LINHAS			2
#define LCD_COLUNAS			16

// -----------------------------------------------------------------------------
// Definições para a USART -----------------------------------------------------

#define FOSC F_CPU
#define BAUD 57600

// -----------------------------------------------------------------------------
// Declaração de manipuladores -------------------------------------------------

int USART_transmite_std(char dado,FILE * stream);
int USART_recebe_std(FILE * stream);
int LCD_escreve(char c, FILE * stream);

#ifdef HANDLER_LCD
	FILE LCD_stream = FDEV_SETUP_STREAM(LCD_escreve, NULL, _FDEV_SETUP_WRITE);
#endif
#ifdef HANDLER_USART_8BITS
	FILE USART_stream = FDEV_SETUP_STREAM(USART_transmite_std, USART_recebe_std, _FDEV_SETUP_RW);
#endif
#ifdef HANDLER_USART_9BITS
	FILE USART_stream = FDEV_SETUP_STREAM(USART_transmite9bits_std, USART_recebe9bits_std, _FDEV_SETUP_RW);
#endif


#endif
