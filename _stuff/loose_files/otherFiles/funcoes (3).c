// -----------------------------------------------------------------------------
// Arquivo:			funcoes.c
// Autor:			Leandro Schwarz
// Versão:			1.0
// Modificado em:	16/11/2011
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Arquivos necessários --------------------------------------------------------

#include "funcoes.h"
#include "LS_DS1307.h"

// -----------------------------------------------------------------------------
// Função:		CRC8CCITT
// Argumentos:	pacote de dados, tamanho do pacote e o polinômio
// Retono:		CRC
// Objetivo:	Calcula o CRC em 8 bits no padrão CCITT
// -----------------------------------------------------------------------------

unsigned char CRC8CCITT(unsigned char * dados,unsigned int tamanho,unsigned char polinomio)
{
	unsigned char j ,caractere;
	unsigned int i;
	unsigned int crc;

	crc = 0xff;									// Inicializador

	for(i = 0;i < tamanho + 1;i++)				// Looping contando com o terminador
	{
		if(i < tamanho)
			caractere = dados[i];				// Ainda há dados para acrescentar
		else
			caractere = 0x00;					// Acrescenta o terminador
		for(j = 0;j < 8;j++)
		{
			crc <<= 1;							// Desloca o CRC 1 bit
			crc &= 0x1ff;						// Trunca o CRC
			if((caractere & 128) == 128)		// Caso o MSB de caractere for 1
				crc = crc + 1;					// Acrescenta o bit no CRC
			caractere <<= 1;					// Pega o proximo MSB do caractere
			if(crc >= 0x100)					// Conserta o polinomio para inclui o x^16
				crc ^= (polinomio + 0x100);		// Calcula o novo CRC
		}
	}
	return crc;
}

// -----------------------------------------------------------------------------
// Função:		CRC16CCITT
// Argumentos:	pacote de dados, tamanho do pacote e o polinômio
// Retono:		CRC
// Objetivo:	Calcula o CRC em 16 bits no padrão CCITT
// -----------------------------------------------------------------------------

unsigned int CRC16CCITT(unsigned char * dados,unsigned int tamanho,unsigned int polinomio)
{
	unsigned char j,caractere;
	unsigned int i;
	unsigned long int crc;

	crc = 0xffff;								// Inicializador

	for(i = 0;i < tamanho + 2;i++)				// Looping contando com o terminador
	{
		if(i < tamanho)
			caractere = dados[i];				// Ainda há dados para acrescentar
		else
			caractere = 0x00;					// Acrescenta o terminador
		for(j = 0;j < 8;j++)
		{
			crc <<= 1;							// Desloca o CRC 1 bit
			crc &= 0x1ffff;						// Trunca o CRC
			if((caractere & 128) == 128)		// Caso o MSB de caractere for 1
				crc = crc + 1;					// Acrescenta o bit no CRC
			caractere <<= 1;					// Pega o proximo MSB do caractere
			if(crc >= 0x10000)					// Conserta o polinomio para inclui o x^16
				crc ^= (polinomio + 0x10000);	// Calcula o novo CRC
		}
	}
	return crc;
}

// -----------------------------------------------------------------------------
// Função:		ajusta_hora_data
// Argumentos:	pacote de dados
// Retono:		1 caso sucesso, 2 caso falha
// Objetivo:	Ajustar a hora e a data do RTC
// -----------------------------------------------------------------------------

unsigned char ajusta_hora_data(unsigned char * dados)
{
	LCD_stdio();
	LCD_move_primeira_linha();
	printf("Hora ajustada\n\n");
	RTC_escreve(dados[0],RTC_ANO);
	RTC_escreve(dados[1],RTC_MES);
	RTC_escreve(dados[2],RTC_DIAMES);
	RTC_escreve(dados[3],RTC_DIASEMANA);
	RTC_escreve(dados[4],RTC_HORAS);
	RTC_escreve(dados[5],RTC_MINUTOS);
	RTC_escreve(dados[6],RTC_SEGUNDOS);
	return COMANDO_RESPONDER_AFIRMATIVAMENTE;
}

// -----------------------------------------------------------------------------
// Função:		leitura_hora_data
// Argumentos:	pacote de dados
// Retono:		1 caso sucesso, 2 caso falha
// Objetivo:	Le a hora e a data do RTC e grava no vetor de dados passado
// -----------------------------------------------------------------------------

unsigned char leitura_hora_data(unsigned char * dados)
{
	LCD_stdio();
	LCD_move_primeira_linha();
	printf("Ler data e hora\n\n");

	dados[0] = RTC_le(RTC_ANO);
	dados[1] = RTC_le(RTC_MES);
	dados[2] = RTC_le(RTC_DIAMES);
	dados[3] = RTC_le(RTC_DIASEMANA);
	dados[4] = RTC_le(RTC_HORAS);
	dados[5] = RTC_le(RTC_MINUTOS);
	dados[6] = RTC_le(RTC_SEGUNDOS);
	return COMANDO_RESPONDER_AFIRMATIVAMENTE;
}

// -----------------------------------------------------------------------------
// Função:		executar_auto_teste
// Argumentos:	Nenhum
// Retono:		1 caso sucesso, 2 caso falha
// Objetivo:	Verifica se o hardware está funcionando nos parâmetros normais
// -----------------------------------------------------------------------------

unsigned char executar_auto_teste(void)
{
	LCD_move_primeira_linha();
	printf("Auto-teste\n\n");
	return COMANDO_RESPONDER_AFIRMATIVAMENTE;
}

// -----------------------------------------------------------------------------
// Função:		LCD_leitura_cor_fundo_primaria
// Argumentos:	pacote de dados
// Retono:		1 caso sucesso, 2 caso falha
// Objetivo:	Le a cor de fundo principal do LCD e grava no vetor de dados passado
// -----------------------------------------------------------------------------

unsigned char LCD_leitura_cor_fundo_principal(unsigned char * dados)
{
	LCD_stdio();
	LCD_move_primeira_linha();
	printf("Ler LCD\nCor principal\n");
	dados[0] = 0x01;
	dados[1] = 0x02;
	dados[2] = 0x03;
	return COMANDO_RESPONDER_AFIRMATIVAMENTE;
}

// -----------------------------------------------------------------------------
// Função:		LCD_leitura_cor_fundo_secundaria
// Argumentos:	pacote de dados
// Retono:		1 caso sucesso, 2 caso falha
// Objetivo:	Le a cor de fundo secundária do LCD e grava no vetor de dados passado
// -----------------------------------------------------------------------------

unsigned char LCD_leitura_cor_fundo_secundaria(unsigned char * dados)
{
	LCD_stdio();
	LCD_move_primeira_linha();
	printf("Ler LCD\nCor secundaria\n");
	dados[0] = 0x04;
	dados[1] = 0x05;
	dados[2] = 0x06;
	return COMANDO_RESPONDER_AFIRMATIVAMENTE;
}

// -----------------------------------------------------------------------------
// Função:		LCD_ajusta_cor_fundo_primaria
// Argumentos:	pacote de dados
// Retono:		1 caso sucesso, 2 caso falha
// Objetivo:	Ajusta a cor de fundo principal do LCD
// -----------------------------------------------------------------------------

unsigned char LCD_ajusta_cor_fundo_principal(unsigned char * dados)
{
	LCD_move_primeira_linha();
	printf("Cor principal do LCD\n");
	printf("R=%03d G=%03d B=%03d\n",dados[0],dados[1],dados[2]);
	return COMANDO_RESPONDER_AFIRMATIVAMENTE;
}

// -----------------------------------------------------------------------------
// Função:		LCD_ajusta_cor_fundo_secundaria
// Argumentos:	pacote de dados
// Retono:		1 caso sucesso, 2 caso falha
// Objetivo:	Ajusta a cor de fundo secundária do LCD
// -----------------------------------------------------------------------------

unsigned char LCD_ajusta_cor_fundo_secundaria(unsigned char * dados)
{
	LCD_move_primeira_linha();
	printf("Cor secundaria do LCD\n");
	printf("R=%02X G=%02X B=%02X\n",dados[0],dados[1],dados[2]);
	return COMANDO_RESPONDER_AFIRMATIVAMENTE;
}
