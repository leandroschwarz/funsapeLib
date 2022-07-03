// -----------------------------------------------------------------------------
// Arquivo:			funcoes.h
// Autor:			Leandro Schwarz
// Versão:			1.0
// Modificado em:	04/12/2011
// -----------------------------------------------------------------------------

#ifndef __FUNCOES_H
#define __FUNCOES_H

// -----------------------------------------------------------------------------
// Arquivos necessários --------------------------------------------------------

#include "defines.h"
#include "LS_LCD.h"

// -----------------------------------------------------------------------------
// Definições das funções macro de controle ------------------------------------

#define MENU_FLAG_ESCOLHA_DO_MENU		0
#define MENU_FLAG_ESCOLHA_DA_ESTACAO	1
#define MENU_FLAG_DADOS_EXTRAS			2
#define MENU_FLAG_TRANSMITIR			3

#define DADOS_ADICIONAIS_DIA_MES		0
#define DADOS_ADICIONAIS_MES			1
#define DADOS_ADICIONAIS_ANO			2
#define DADOS_ADICIONAIS_DIA_SEMANA		3
#define DADOS_ADICIONAIS_HORA			4
#define DADOS_ADICIONAIS_MINUTO			5
#define DADOS_ADICIONAIS_SEGUNDO		6

#define DADOS_ADICIONAIS_VERMELHO		0
#define DADOS_ADICIONAIS_VERDE			1
#define DADOS_ADICIONAIS_AZUL			2

#define LCD_SETA						0x7E
//#define LCD_SETA						0x1A

#define ESTACAO_NUMERO					0x00

#define ESTADO_OCIOSO					0
#define ESTADO_RECEBE_PACOTE			1
#define ESTADO_VERIFICA_PACOTE_RECEPCAO	2
#define ESTADO_EXECUTA_PACOTE_RECEPCAO	3
#define ESTADO_MONTA_PACOTE_TRANSMISSAO	4
#define ESTADO_TRANSMITE_PACOTE			5
#define ESTADO_DESCARTA_PACOTE			6


#define COR_ROSA_VERMELHO	255
#define COR_ROSA_VERDE		40
#define COR_ROSA_AZUL		111
#define COR_VERDE_VERMELHO	29
#define COR_VERDE_VERDE		236
#define COR_VERDE_AZUL		23

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

unsigned int CRC16CCITT(unsigned char * dados,unsigned int tamanho,unsigned int polinomio);
void menu_proximo(void);
void menu_anterior(void);
void menu_atualiza(void);
void estacao_atualiza(void);
void modifica_dados_adicionais(unsigned char tecla);
void atualiza_dados_adicionais(void);
void aceita_dados_adicionais(void);
void incrementa_dados_adicionais(void);
void decrementa_dados_adicionais(void);

#endif
