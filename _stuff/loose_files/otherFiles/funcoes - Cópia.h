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

// -----------------------------------------------------------------------------
// Declaração das funções da biblioteca ----------------------------------------

unsigned char CRC8CCITT(unsigned char * dados,unsigned int tamanho,unsigned char polinomio);
unsigned int CRC16CCITT(unsigned char * dados,unsigned int tamanho,unsigned int polinomio);
void menu_proximo(void);
void menu_anterior(void);
void menu_aceita(void);
void menu_atualiza(void);
void estacao_atualiza(void);
void estacao_aceita(void);
void modifica_dados_adicionais(unsigned char tecla);
void atualiza_dados_adicionais(void);
void incrementa_dados_adicionais(void);
void decrementa_dados_adicionais(void);
void modifica_dados_adicionais(unsigned char tecla);

// -----------------------------------------------------------------------------
// Declaração das variáveis globais da biblioteca ------------------------------

unsigned char menu_flag;
unsigned char menu;
unsigned char estacao;
unsigned char dados_adicionais;
unsigned char hora;
unsigned char minuto;
unsigned char segundo;
unsigned char dia_mes;
unsigned char dia_semana;
unsigned char mes;
unsigned char ano;
unsigned char cor1_vm;
unsigned char cor1_vd;
unsigned char cor1_az;
unsigned char cor2_vm;
unsigned char cor2_vd;
unsigned char cor2_az;

#endif
