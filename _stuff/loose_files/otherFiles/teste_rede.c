// -----------------------------------------------------------------------------
// Projeto:			teste_rede
// Dispositivo:		ATmega328
// Autores:			Mayara de Sousa
//					Bruna Ramos
// 					Leandro Schwarz
// Versão:			1.0
// Modificado em:	03/12/2011
// -----------------------------------------------------------------------------

// Definições ------------------------------------------------------------------
// -----------------------------------------------------------------------------

#define F_CPU 16000000UL
#define HANDLER_LCD
#define HANDLER_USART_8BITS

#define ESTACAO_NUMERO					0xF3

#define ESTADO_OCIOSO					0
#define ESTADO_RECEBE_PACOTE			1
#define ESTADO_VERIFICA_PACOTE_RECEPCAO	2
#define ESTADO_EXECUTA_PACOTE_RECEPCAO	3
#define ESTADO_MONTA_PACOTE_TRANSMISSAO	4
#define ESTADO_TRANSMITE_PACOTE			5
#define ESTADO_DESCARTA_PACOTE			6

#define FLAG_NENHUM_PACOTE				0
#define FLAG_PACOTE_SENDO_RECEBIDO		1
#define FLAG_PACOTE_RECEBIDO			2
#define FLAG_PACOTE_VALIDO				3
#define FLAG_PACOTE_INVALIDO			4
#define FLAG_PACOTE_SENDO_TRANSMITIDO	5

// Arquivos necessários --------------------------------------------------------
// -----------------------------------------------------------------------------

#include "defines.h"
#include "ATmega328.h"
#include "LS_LCD.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Declaração de funções -------------------------------------------------------
// -----------------------------------------------------------------------------

ISR(USART_TX_vect);
ISR(USART_RX_vect);
ISR(TIMER0_OVF_vect);
unsigned char CRC8CCITT(unsigned char * dados,unsigned int tamanho,unsigned char polinomio);
unsigned int CRC16CCITT(unsigned char * dados,unsigned int tamanho,unsigned int polinomio);

// Declaração das variáveis globais --------------------------------------------
// -----------------------------------------------------------------------------

unsigned char			recepcao_pacote[266];		// String para guardar o pacote recebido
unsigned int			recepcao_indice;			// Índice do pacote de recepção
volatile unsigned char	comunicacao_flag;			// Flag do estado da comunicação
unsigned int			recepcao_tamanho_pacote;	// Tamanho do pacote de recepção
unsigned char			transmissao_pacote[266];	// String para guardar o pacote a ser transmitido
unsigned int			transmissao_indice;			// Índice do pacote de transmissão
unsigned int			transmissao_tamanho_pacote;	// Tamanho do pacote de transmissão
unsigned char			string_lixo[8];				// String auxiliar
volatile unsigned char	estado_RXTX;				// Máquina de estados da comunicação

// Função principal ------------------------------------------------------------
// -----------------------------------------------------------------------------

int main()
{
	// Declaração de variáveis locais
	unsigned char c_lixo;
	unsigned int i_lixo;

	// Inicialização de variáveis
	estado_RXTX				= ESTADO_OCIOSO;
	recepcao_indice			= 0;
	recepcao_tamanho_pacote	= 0;
	comunicacao_flag		= FLAG_NENHUM_PACOTE;

	// Configuração dos ports
	DDRC = 0b11111111;

	// Configuração da USART
	USART_configura(USART_MODO_ASSINCRONO_NORMAL);
	USART_PARIDADE_DESABILITADA();
	USART_STOP_BIT_SIMPLES();
	USART_DATA_BITS_8();
	USART_HABILITA_INTERRUPCAO_RECEPTOR();
	USART_HABILITA_INTERRUPCAO_TRANSMISSOR();
	USART_ATIVA_RECEPTOR();
	USART_ATIVA_TRANSMISSOR();

	// Configuração do Timer0
	TIMER0_CLOCK_DESABILITA();
	TIMER0_DESATIVA();

	// Configuração do LCD
	LCD_inicia();
LCD_stdio();
printf("FOI MESMO 0xF3");

	sei();

	while(1)
	{

		switch(estado_RXTX)
		{
			// ----------------------------------------------------------------------
			// Estado de espera
			// ----------------------------------------------------------------------
			case ESTADO_OCIOSO:
				if(comunicacao_flag == FLAG_NENHUM_PACOTE)
				{
					// 
				}
				break;

			// ----------------------------------------------------------------------
			// Estado que fica recebendo o pacote até o termino ou timeout
			// ----------------------------------------------------------------------
			case ESTADO_RECEBE_PACOTE:
TIMER0_CLOCK_DESABILITA();
TIMER0_DESATIVA();
TIMER0_CONFIGURA(200);
TIMER0_CLOCK_PRESCALER_1024();
TIMER0_ATIVA();
				c_lixo = USART_recebe(NULL);

				if(recepcao_indice == 0)		// Início do frame
				{
					estado_RXTX = ESTADO_OCIOSO;
					recepcao_tamanho_pacote = 0;
				}
				else if(recepcao_indice == 4)	// Número de bytes de dados
				{
					estado_RXTX = ESTADO_OCIOSO;
					if(c_lixo == 0)
						recepcao_tamanho_pacote = 265;
					else
						recepcao_tamanho_pacote = (unsigned int)c_lixo + 9;
				}
				else if(recepcao_indice == (recepcao_tamanho_pacote - 1))
				{
					comunicacao_flag = FLAG_PACOTE_RECEBIDO;
					estado_RXTX = ESTADO_VERIFICA_PACOTE_RECEPCAO;
				}
				else
					estado_RXTX = ESTADO_OCIOSO;
				recepcao_pacote[recepcao_indice] = c_lixo;
				recepcao_indice++;
				USART_HABILITA_INTERRUPCAO_TRANSMISSOR();
				USART_HABILITA_INTERRUPCAO_RECEPTOR();
				break;

			// ----------------------------------------------------------------------
			// Verifica se o pacote recebido está correto e se é para esta estação
			// ----------------------------------------------------------------------
			case ESTADO_VERIFICA_PACOTE_RECEPCAO:
				recepcao_indice = 0;
				// Verifica o inicializador
				if((recepcao_pacote[0] != 0x18) || (recepcao_pacote[1] != 0x02))
				{
					comunicacao_flag = FLAG_PACOTE_INVALIDO;
					estado_RXTX = ESTADO_DESCARTA_PACOTE;
					break;
				}
				// Verifica a estação de destino
				if(recepcao_pacote[2] != ESTACAO_NUMERO)
				{
					comunicacao_flag = FLAG_PACOTE_INVALIDO;
					estado_RXTX = ESTADO_DESCARTA_PACOTE;
					break;
				}
				// Verifica o número de bytes de dados
				if(recepcao_pacote[4] != (recepcao_tamanho_pacote - 9))
				{
					comunicacao_flag = FLAG_PACOTE_INVALIDO;
					estado_RXTX = ESTADO_DESCARTA_PACOTE;
					break;
				}
				// Verifica o CRC
				i_lixo = recepcao_pacote[recepcao_tamanho_pacote - 3];
				i_lixo |= (recepcao_pacote[recepcao_tamanho_pacote - 4] << 8);
				if(i_lixo != CRC16CCITT(recepcao_pacote,(recepcao_tamanho_pacote - 4),0x1021))
				{
					comunicacao_flag = FLAG_PACOTE_INVALIDO;
					estado_RXTX = ESTADO_DESCARTA_PACOTE;
					break;
				}
				// Verifica o terminador
				if((recepcao_pacote[recepcao_tamanho_pacote - 2] != 0xE7) || (recepcao_pacote[recepcao_tamanho_pacote - 1] != 0xFD))
				{
					comunicacao_flag = FLAG_PACOTE_INVALIDO;
					estado_RXTX = ESTADO_DESCARTA_PACOTE;
					break;
				}
				comunicacao_flag = FLAG_PACOTE_VALIDO;
				estado_RXTX = ESTADO_EXECUTA_PACOTE_RECEPCAO;
				break;

			// ----------------------------------------------------------------------
			// Executa a tarefa que o pacote determinou
			// ----------------------------------------------------------------------
			case ESTADO_EXECUTA_PACOTE_RECEPCAO:
				switch(recepcao_pacote[5])
				{
					case 0x01:
						LCD_stdio();
						printf("Recebido");
						_delay_ms(1000);
						printf("\n\n");
						break;
				}
				estado_RXTX = ESTADO_MONTA_PACOTE_TRANSMISSAO;
				break;

			// ----------------------------------------------------------------------
			// Prepara o pacote de resposta para transmissão
			// ----------------------------------------------------------------------
			case ESTADO_MONTA_PACOTE_TRANSMISSAO:
				c_lixo = 0;
				// Monta o pacote de resposta
				transmissao_pacote[c_lixo++] = 0x18;				// Inicializador
				transmissao_pacote[c_lixo++] = 0x02;				// Inicializador
				transmissao_pacote[c_lixo++] = recepcao_pacote[3];	// Destino
				transmissao_pacote[c_lixo++] = ESTACAO_NUMERO;		// Origem
				switch(recepcao_pacote[5])
				{
					case 0x01:
						transmissao_pacote[c_lixo++] = 0x81;
						break;
				}
				i_lixo = CRC16CCITT(transmissao_pacote,c_lixo,0x1021);
				transmissao_pacote[c_lixo++] = (i_lixo >> 8);		// CRC
				transmissao_pacote[c_lixo++] = (i_lixo & 0xFF);		// CRC
				transmissao_pacote[c_lixo++] = 0xE7;				// Terminador
				transmissao_pacote[c_lixo++] = 0xFD;				// Terminador
				transmissao_tamanho_pacote = c_lixo;
				transmissao_indice = 0;
				comunicacao_flag = FLAG_PACOTE_SENDO_TRANSMITIDO;
				estado_RXTX = ESTADO_TRANSMITE_PACOTE;
				break;

			// ----------------------------------------------------------------------
			// Transmite o pacote de resposta
			// ----------------------------------------------------------------------
			case ESTADO_TRANSMITE_PACOTE:
				if(transmissao_indice < transmissao_tamanho_pacote)
				{
					USART_transmite(transmissao_pacote[transmissao_indice]);
					transmissao_indice++;
				}
				else
				{
					transmissao_indice = 0;
					transmissao_tamanho_pacote = 0;
					comunicacao_flag = FLAG_NENHUM_PACOTE;
				}
				estado_RXTX = ESTADO_OCIOSO;
				USART_HABILITA_INTERRUPCAO_TRANSMISSOR();
				USART_HABILITA_INTERRUPCAO_RECEPTOR();
				break;

			// ----------------------------------------------------------------------
			// Descarta o pacote incompleto ou de estação errada
			// ----------------------------------------------------------------------
			case ESTADO_DESCARTA_PACOTE:
				comunicacao_flag = FLAG_NENHUM_PACOTE;
				recepcao_indice = 0;
				recepcao_tamanho_pacote = 0;
printf("%16s\n","PACOTE INVALIDO");
_delay_ms(1000);
printf("\n\n");
				estado_RXTX = ESTADO_OCIOSO;
				break;
		}

	}

	return 0;
}


ISR(USART_RX_vect)
{
	comunicacao_flag = FLAG_PACOTE_SENDO_RECEBIDO;
	USART_DESABILITA_INTERRUPCAO_TRANSMISSOR();
	USART_DESABILITA_INTERRUPCAO_RECEPTOR();
	estado_RXTX = ESTADO_RECEBE_PACOTE;
	return;
}

ISR(USART_TX_vect)
{
	USART_DESABILITA_INTERRUPCAO_TRANSMISSOR();
	USART_DESABILITA_INTERRUPCAO_RECEPTOR();
	estado_RXTX = ESTADO_TRANSMITE_PACOTE;
	return;
}

ISR(TIMER0_OVF_vect)
{
	TIMER0_DESATIVA();
	TIMER0_CLOCK_DESABILITA();
	if(comunicacao_flag == FLAG_PACOTE_SENDO_RECEBIDO)
		estado_RXTX = ESTADO_DESCARTA_PACOTE;
	return;
}

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
