// -----------------------------------------------------------------------------
// Arquivo:			funcoes.h
// Autor:			Leandro Schwarz
// Versão:			1.0
// Modificado em:	16/11/2011
// -----------------------------------------------------------------------------

#ifndef __FUNCOES_H
#define __FUNCOES_H

// -----------------------------------------------------------------------------
// Arquivos necessários --------------------------------------------------------

#include "defines.h"
#include "LS_LCD.h"

// -----------------------------------------------------------------------------
// Definições das funções macro de controle ------------------------------------

#define FLAG_NENHUM_PACOTE				0
#define FLAG_PACOTE_SENDO_RECEBIDO		1
#define FLAG_PACOTE_RECEBIDO			2
#define FLAG_PACOTE_VALIDO				3
#define FLAG_PACOTE_INVALIDO			4
#define FLAG_PACOTE_SENDO_TRANSMITIDO	5

#define COMANDO_VERIFICAR_ESTACAO_ATIVA				0x00
#define COMANDO_EXECUTAR_AUTO_TESTE					0x01
#define COMANDO_LER_HORA_E_DATA						0x10
#define COMANDO_AJUSTAR_HORA_E_DATA					0x11
#define COMANDO_LER_ALARMES							0x20
#define COMANDO_AJUSTAR_ALARMES						0x21
#define COMANDO_LCD_LER_COR_FUNDO_PRINCIPAL			0x30
#define COMANDO_LCD_LER_COR_FUNDO_SECUNDARIA		0x31
#define COMANDO_LCD_AJUSTAR_COR_FUNDO_PRINCIPAL		0x32
#define COMANDO_LCD_AJUSTAR_COR_FUNDO_SECUNDARIA	0x33
#define COMANDO_RESPONDER_AFIRMATIVAMENTE			0x80
#define COMANDO_RESPONDER_NEGATIVAMENTE				0x81
#define COMANDO_RETORNAR_HORA_E_DATA				0x90
#define COMANDO_RETORNAR_ALARMES					0xA0
#define COMANDO_LCD_RETORNAR_COR_FUNDO_PRINCIPAL	0xB0
#define COMANDO_LCD_RETORNAR_COR_FUNDO_SECUNDARIA	0xB1

// -----------------------------------------------------------------------------
// Declaração das funções da biblioteca ----------------------------------------

unsigned char CRC8CCITT(unsigned char * dados,unsigned int tamanho,unsigned char polinomio);
unsigned int CRC16CCITT(unsigned char * dados,unsigned int tamanho,unsigned int polinomio);
unsigned char ajusta_hora_data(unsigned char * dados);
unsigned char leitura_hora_data(unsigned char * dados);
unsigned char executar_auto_teste(void);
unsigned char LCD_leitura_cor_fundo_principal(unsigned char * dados);
unsigned char LCD_leitura_cor_fundo_secundaria(unsigned char * dados);
unsigned char LCD_ajusta_cor_fundo_principal(unsigned char * dados);
unsigned char LCD_ajusta_cor_fundo_secundaria(unsigned char * dados);

// -----------------------------------------------------------------------------
// Declaração das variáveis globais da biblioteca ------------------------------

#endif
