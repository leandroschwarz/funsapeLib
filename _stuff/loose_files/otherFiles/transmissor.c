// -----------------------------------------------------------------------------
// Projeto:			Transmissor ZigBee
// Dispositivo:		ATmega328
// Autor:			Leandro Schwarz
// Versão:			1.0
// Modificado em:	09/11/2011
// -----------------------------------------------------------------------------

// Definições ------------------------------------------------------------------
// -----------------------------------------------------------------------------

#define F_CPU 16000000UL
#define HANDLER_USART_8BITS
#define HANDLER_LCD

#define TEMPORIZADOR					15000
#define NUMERO_TENTATIVAS_TRANSMISSAO	30

// Arquivos necessários --------------------------------------------------------
// -----------------------------------------------------------------------------

#include "defines.h"
#include "ATmega328.h"
#include "funcoes.h"
#include "LS_LCD.h"
#include "LS_teclado_matricial.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

// Declaração de funções -------------------------------------------------------
// -----------------------------------------------------------------------------

ISR(TIMER1_OVF_vect);
void transmite_a_bagaca(void);
unsigned char espera_a_resposta(void);

// Declaração das variáveis globais --------------------------------------------
// -----------------------------------------------------------------------------

extern unsigned char menu_flag;
extern unsigned char menu;
extern unsigned char estacao;
extern unsigned char dados_adicionais;
extern unsigned char hora;
extern unsigned char minuto;
extern unsigned char segundo;
extern unsigned char dia_mes;
extern unsigned char dia_semana;
extern unsigned char mes;
extern unsigned char ano;
extern unsigned char cor1_vm;
extern unsigned char cor1_vd;
extern unsigned char cor1_az;
extern unsigned char cor2_vm;
extern unsigned char cor2_vd;
extern unsigned char cor2_az;

volatile unsigned char	estado_RXTX;				// Máquina de estados da comunicação
volatile unsigned char	estado_maquina;				// Máquina de estados do sistema
unsigned char			recepcao_pacote[265];		// String para guardar o pacote recebido
unsigned int			recepcao_indice;			// Índice do pacote de recepção
unsigned int			recepcao_tamanho_pacote;	// Tamanho do pacote de recepção
volatile unsigned char	comunicacao_flag;			// Flag do estado da comunicação
unsigned char			transmissao_pacote[265];	// String para guardar o pacote a ser transmitido
unsigned int			transmissao_indice;			// Índice do pacote de transmissão
unsigned int			transmissao_tamanho_pacote;	// Tamanho do pacote de transmissão
unsigned char			string_lixo[8];				// String auxiliar
unsigned char			contador_transmissoes;		// Contador do número de transmissões
volatile unsigned char	timeout;					// Acusar estouro do timer1

// Função principal ------------------------------------------------------------
// -----------------------------------------------------------------------------

int main()
{
	// Declaração de variáveis locais
	unsigned char tecla;
	unsigned char c_lixo;
	unsigned int i_lixo;

	// Configuração dos ports
	DDRB	= 0b00001111;
	PORTB	= 0b11111111;
	DDRC	= 0b11111111;
	PORTC	= 0b00000000;
	DDRD	= 0b11000000;
	PORTD	= 0b11111100;

	// Inicialização de variáveis
	estado_RXTX			= ESTADO_OCIOSO;
	estado_maquina		= ESTADO_OCIOSO;
	menu				= 0;
	menu_flag			= MENU_FLAG_ESCOLHA_DO_MENU;
	estacao				= 0;
	dados_adicionais	= DADOS_ADICIONAIS_DIA_MES;
	tecla				= 0xFF;
	hora				= 0;
	minuto				= 0;
	segundo				= 0;
	dia_mes				= 1;
	dia_semana			= 1;
	mes					= 1;
	ano					= 11;
	cor1_vm				= COR_ROSA_VERMELHO;
	cor1_vd				= COR_ROSA_VERDE;
	cor1_az				= COR_ROSA_AZUL;
	cor2_vm				= COR_VERDE_VERMELHO;
	cor2_vd				= COR_VERDE_VERDE;
	cor2_az				= COR_VERDE_AZUL;

	// Configuração do LCD
	LCD_inicia();
	menu_atualiza();

	// Configuração da USART
	USART_configura(USART_MODO_ASSINCRONO_NORMAL);
	USART_PARIDADE_DESABILITADA();
	USART_STOP_BIT_SIMPLES();
	USART_DATA_BITS_8();
	USART_ATIVA_RECEPTOR();
	USART_ATIVA_TRANSMISSOR();

	// Configuração do Timer0
	TIMER1_CLOCK_DESABILITA();
	TIMER1_DESATIVA();

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
					switch(estado_maquina)
					{
						case ESTADO_OCIOSO:

							// Rotação dos menus --------------------------------------------------------------
							// --------------------------------------------------------------------------------
							if(menu_flag == MENU_FLAG_ESCOLHA_DO_MENU)
							{
								tecla = TECLADO_varrer();
								switch(tecla)
								{
									case 0x0F:
										menu_proximo();
										break;
									case 0x0E:
										menu_anterior();
										break;
									case 0x0B:
										menu_flag = MENU_FLAG_ESCOLHA_DA_ESTACAO;
										estacao_atualiza();
										break;
								}
							}

							// Seleção da estação de destino --------------------------------------------------
							// --------------------------------------------------------------------------------
							else if(menu_flag == MENU_FLAG_ESCOLHA_DA_ESTACAO)
							{
								tecla = TECLADO_varrer();
								switch(tecla)
								{
									case 0x00:
									case 0x01:
									case 0x02:
									case 0x03:
									case 0x04:
									case 0x05:
									case 0x06:
									case 0x07:
									case 0x08:
									case 0x09:
										i_lixo = (estacao * 10) + tecla;
										if(i_lixo > 255)
											estacao = 255;
										else
											estacao = (unsigned char)(i_lixo & 0xFF);
										estacao_atualiza();
										break;
									case 0x0A:	// Cancela
										menu_flag = MENU_FLAG_ESCOLHA_DO_MENU;
										menu_atualiza();
										break;
									case 0x0B:	// Aceita
										switch(menu)
										{
											case 0:	// Reportar atividade
											case 1:	// Auto-teste
											case 2:	// Ler data e hora
											case 4:	// Ler cor do LCD1
											case 6:	// Ler cor do LCD2
												menu_flag = MENU_FLAG_TRANSMITIR;
												break;
											case 3:	// Ajusta data e hora
											case 5:	// Ajusta cor do LCD1
											case 7:	// Ajusta cor do LCD2
												dados_adicionais = 0;
												atualiza_dados_adicionais();
												menu_flag = MENU_FLAG_DADOS_EXTRAS;
												break;
										}
										break;
									case 0x0F:	// Aumenta
										estacao++;
										estacao_atualiza();
										break;
									case 0x0E:	// Diminui
										estacao--;
										estacao_atualiza();
										break;
								}
							}

							// Configuração dos dados adicionais ----------------------------------------------
							// --------------------------------------------------------------------------------
							else if(menu_flag == MENU_FLAG_DADOS_EXTRAS)
							{
								tecla = TECLADO_varrer();
								switch(tecla)
								{
									case 0x00:
									case 0x01:
									case 0x02:
									case 0x03:
									case 0x04:
									case 0x05:
									case 0x06:
									case 0x07:
									case 0x08:
									case 0x09:
										modifica_dados_adicionais(tecla);
										atualiza_dados_adicionais();
										break;
									case 0x0A:	// Cancela
										menu_flag = MENU_FLAG_ESCOLHA_DO_MENU;
										menu_atualiza();
										break;
									case 0x0B:	// Aceita
										aceita_dados_adicionais();
										break;
									case 0x0F:	// Aumenta
										incrementa_dados_adicionais();
										atualiza_dados_adicionais();
										break;
									case 0x0E:	// Diminui
										decrementa_dados_adicionais();
										atualiza_dados_adicionais();
										break;
								}
							}

							// Transmissão do comando ---------------------------------------------------------
							// --------------------------------------------------------------------------------
							else if(menu_flag == MENU_FLAG_TRANSMITIR)
							{
								dados_adicionais = 0;
								menu_flag = MENU_FLAG_ESCOLHA_DO_MENU;
								estado_RXTX = ESTADO_MONTA_PACOTE_TRANSMISSAO;
								break;
							}// END IF
							break;
					}// END SWITCH
				}// END IF
				break;

			// ----------------------------------------------------------------------
			// Prepara o pacote para transmissão
			// ----------------------------------------------------------------------
			case ESTADO_MONTA_PACOTE_TRANSMISSAO:
LCD_stdio();
LCD_display_limpa();
LCD_move_primeira_linha();
printf("Preparando para\n");
printf("Transmitir");
				c_lixo = 0;
				// Monta o pacote de transmissão
				transmissao_pacote[c_lixo++] = 0x18;				// Inicializador
				transmissao_pacote[c_lixo++] = 0x02;				// Inicializador
				transmissao_pacote[c_lixo++] = estacao;				// Destino
				transmissao_pacote[c_lixo++] = ESTACAO_NUMERO;		// Origem
				switch(menu)										// Tamanho
				{
					case 0:
					case 1:
					case 2:
					case 4:
					case 6:	transmissao_pacote[c_lixo++] = 0x01;	break;
					case 3:	transmissao_pacote[c_lixo++] = 0x07;	break;
					case 5:
					case 7:	transmissao_pacote[c_lixo++] = 0x03;	break;
				}
				switch(menu)										// Comando
				{
					case 0:	transmissao_pacote[c_lixo++] = COMANDO_VERIFICAR_ESTACAO_ATIVA;				break;
					case 1:	transmissao_pacote[c_lixo++] = COMANDO_EXECUTAR_AUTO_TESTE;					break;
					case 2:	transmissao_pacote[c_lixo++] = COMANDO_LER_HORA_E_DATA;						break;
					case 3:	transmissao_pacote[c_lixo++] = COMANDO_AJUSTAR_HORA_E_DATA;					break;
					case 4:	transmissao_pacote[c_lixo++] = COMANDO_LCD_LER_COR_FUNDO_PRINCIPAL;			break;
					case 5:	transmissao_pacote[c_lixo++] = COMANDO_LCD_AJUSTAR_COR_FUNDO_PRINCIPAL;		break;
					case 6:	transmissao_pacote[c_lixo++] = COMANDO_LCD_LER_COR_FUNDO_SECUNDARIA;		break;
					case 7:	transmissao_pacote[c_lixo++] = COMANDO_LCD_AJUSTAR_COR_FUNDO_SECUNDARIA;	break;
				}
				switch(menu)										// Dados adicionais
				{
					case 3:
						transmissao_pacote[c_lixo++] = ano;
						transmissao_pacote[c_lixo++] = mes;
						transmissao_pacote[c_lixo++] = dia_mes;
						transmissao_pacote[c_lixo++] = dia_semana;
						transmissao_pacote[c_lixo++] = hora;
						transmissao_pacote[c_lixo++] = minuto;
						transmissao_pacote[c_lixo++] = segundo;
						break;
					case 5:
						transmissao_pacote[c_lixo++] = cor1_vm;
						transmissao_pacote[c_lixo++] = cor1_vd;
						transmissao_pacote[c_lixo++] = cor1_az;
						break;
					case 7:
						transmissao_pacote[c_lixo++] = cor2_vm;
						transmissao_pacote[c_lixo++] = cor2_vd;
						transmissao_pacote[c_lixo++] = cor2_az;
						break;
				}
				i_lixo = CRC16CCITT(transmissao_pacote,c_lixo,0x1021);
				transmissao_pacote[c_lixo++] = (i_lixo >> 8);		// CRC
				transmissao_pacote[c_lixo++] = (i_lixo & 0xFF);		// CRC
				transmissao_pacote[c_lixo++] = 0xE7;				// Terminador
				transmissao_pacote[c_lixo++] = 0xFD;				// Terminador
				transmissao_tamanho_pacote = c_lixo;
				transmissao_indice = 0;
				contador_transmissoes = 0;
				comunicacao_flag = FLAG_PACOTE_SENDO_TRANSMITIDO;
				estado_RXTX = ESTADO_TRANSMITE_PACOTE;
LCD_stdio();
LCD_display_limpa();
LCD_move_primeira_linha();
printf("Transmitindo");
				break;

			// ----------------------------------------------------------------------
			// Transmite o pacote
			// ----------------------------------------------------------------------
			case ESTADO_TRANSMITE_PACOTE:
				if(contador_transmissoes == NUMERO_TENTATIVAS_TRANSMISSAO)
				{
LCD_stdio();
LCD_display_limpa();
LCD_move_primeira_linha();
printf("Erro na\ntransmissao");
_delay_ms(3000);
					contador_transmissoes = 0;
					estado_RXTX = ESTADO_OCIOSO;
					comunicacao_flag = FLAG_NENHUM_PACOTE;
					menu_flag = MENU_FLAG_ESCOLHA_DO_MENU;
					estado_maquina = ESTADO_OCIOSO;
					menu_atualiza();
				}
				else
				{
					transmite_a_bagaca();
LCD_stdio();
LCD_display_limpa();
LCD_move_primeira_linha();
printf("Transmitiu %d",contador_transmissoes);
					contador_transmissoes++;
					timeout = 0;
					estado_RXTX = ESTADO_RECEBE_PACOTE;
				}
				break;

			// ----------------------------------------------------------------------
			// Estado que fica recebendo o pacote até o termino ou timeout
			// ----------------------------------------------------------------------
			case ESTADO_RECEBE_PACOTE:
				TIMER1_CLOCK_DESABILITA();
				TIMER1_DESATIVA();
				TIMER1_CLOCK_PRESCALER_64();
				TIMER1_CONFIGURA(TEMPORIZADOR);
				TIMER1_ATIVA();
				if(espera_a_resposta() == 0)	// Dado recebido
				{
					estado_RXTX = ESTADO_VERIFICA_PACOTE_RECEPCAO;
LCD_stdio();
LCD_display_limpa();
LCD_move_primeira_linha();
printf("Recebeu");
				}
				else
				{
					estado_RXTX = ESTADO_TRANSMITE_PACOTE;
				}
				break;

			// ----------------------------------------------------------------------
			// Verifica se o pacote recebido está correto e se é para esta estação
			// ----------------------------------------------------------------------
			case ESTADO_VERIFICA_PACOTE_RECEPCAO:
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
				LCD_stdio();
				LCD_display_limpa();
				LCD_move_primeira_linha();
				switch(menu)
				{
					case 0:	// COMANDO_VERIFICAR_ESTACAO_ATIVA
						if(recepcao_pacote[5] == COMANDO_RESPONDER_AFIRMATIVAMENTE)
							printf("Estação %03d esta\nativa",estacao);
						else
							printf("Problemas na\nestacao %03d",estacao);
						break;
					case 1:	// COMANDO_EXECUTAR_AUTO_TESTE
						if(recepcao_pacote[5] == COMANDO_RESPONDER_AFIRMATIVAMENTE)
							printf("Auto-teste OK");
						else if(recepcao_pacote[5] == COMANDO_RESPONDER_NEGATIVAMENTE)
							printf("Falha no\nauto-teste");
						else
							printf("Problemas na\nestacao %03d",estacao);
						break;
					case 2:	// COMANDO_LER_HORA_E_DATA
						if(recepcao_pacote[5] == COMANDO_RETORNAR_HORA_E_DATA)
						{
							printf("%02x/%02x/%02x ",recepcao_pacote[8],recepcao_pacote[7],recepcao_pacote[6]);
							switch(recepcao_pacote[9])
							{
								case 1:	printf("Dom\n");	break;
								case 2:	printf("Seg\n");	break;
								case 3:	printf("Ter\n");	break;
								case 4:	printf("Qua\n");	break;
								case 5:	printf("Qui\n");	break;
								case 6:	printf("Sex\n");	break;
								case 7:	printf("Sab\n");	break;
							}
							printf("%02x:%02x:%02x",recepcao_pacote[10],recepcao_pacote[11],recepcao_pacote[12]);
						}
						else
							printf("Problemas na\nestacao %03d",estacao);
						break;
					case 3:	// COMANDO_AJUSTAR_HORA_E_DATA
						if(recepcao_pacote[5] == COMANDO_RESPONDER_AFIRMATIVAMENTE)
							printf("Data e hora\najustadas");
						else if(recepcao_pacote[5] == COMANDO_RESPONDER_NEGATIVAMENTE)
							printf("Falha no ajuste\nde data e hora");
						else
							printf("Problemas na\nestacao %03d",estacao);
						break;
					case 4:	// COMANDO_LCD_LER_COR_FUNDO_PRINCIPAL
						if(recepcao_pacote[5] == COMANDO_LCD_RETORNAR_COR_FUNDO_PRINCIPAL)
						{
							printf("Cor principal:\n");
							printf("R%03d G%03d B%03d",recepcao_pacote[6],recepcao_pacote[7],recepcao_pacote[8]);
						}
						else
							printf("Problemas na\nestacao %03d",estacao);
						break;
					case 5:	// COMANDO_LCD_AJUSTAR_COR_FUNDO_PRINCIPAL
						if(recepcao_pacote[5] == COMANDO_RESPONDER_AFIRMATIVAMENTE)
							printf("Cor principal\najustada");
						else if(recepcao_pacote[5] == COMANDO_RESPONDER_NEGATIVAMENTE)
							printf("Falha no ajuste\nda cor principal");
						else
							printf("Problemas na\nestacao %03d",estacao);
						break;
					case 6:	// COMANDO_LCD_LER_COR_FUNDO_SECUNDARIA
						if(recepcao_pacote[5] == COMANDO_LCD_RETORNAR_COR_FUNDO_SECUNDARIA)
						{
							printf("Cor secundaria:\n");
							printf("R%03d G%03d B%03d",recepcao_pacote[6],recepcao_pacote[7],recepcao_pacote[8]);
						}
						else
							printf("Problemas na\nestacao %03d",estacao);
						break;
					case 7:	// COMANDO_LCD_AJUSTAR_COR_FUNDO_SECUNDARIA
						if(recepcao_pacote[5] == COMANDO_RESPONDER_AFIRMATIVAMENTE)
							printf("Cor secundaria\najustadas");
						else if(recepcao_pacote[5] == COMANDO_RESPONDER_NEGATIVAMENTE)
							printf("Falha no ajuste\nda cor secundaria");
						else
							printf("Problemas na\nestacao %03d",estacao);
						break;
				}
				_delay_ms(3000);
				estado_RXTX = ESTADO_OCIOSO;
				comunicacao_flag = FLAG_NENHUM_PACOTE;
				menu_flag = MENU_FLAG_ESCOLHA_DO_MENU;
				estado_maquina = ESTADO_OCIOSO;
				menu_atualiza();
				break;

			// ----------------------------------------------------------------------
			// Descarta o pacote incompleto ou de estação errada
			// ----------------------------------------------------------------------
			case ESTADO_DESCARTA_PACOTE:
				recepcao_tamanho_pacote = 0;
				estado_RXTX = ESTADO_OCIOSO;
				comunicacao_flag = FLAG_NENHUM_PACOTE;
				menu_flag = MENU_FLAG_ESCOLHA_DO_MENU;
				estado_maquina = ESTADO_OCIOSO;
LCD_stdio();
LCD_display_limpa();
LCD_move_primeira_linha();
printf("Pacote descartado!");
_delay_ms(3000);
				menu_atualiza();
				break;

		}// END SWITCH
	}// END WHILE

	return 0;
}

ISR(TIMER1_OVF_vect)
{
cpl_bit(PORTD,PD6);
	timeout = 1;
	TIMER1_DESATIVA();
	TIMER1_CLOCK_DESABILITA();
//	if(comunicacao_flag == FLAG_PACOTE_SENDO_RECEBIDO)
//		estado_RXTX = ESTADO_DESCARTA_PACOTE;
	return;
}

void transmite_a_bagaca(void)
{
	unsigned int i;

	for(i = 0;i < transmissao_tamanho_pacote;i++)
		USART_transmite(transmissao_pacote[i]);

	return;
}

unsigned char espera_a_resposta(void)
{
	unsigned int i;
	unsigned char c_lixo;

	i = 0;
	while(!timeout)
	{
		if(USART_RECEPTOR_DADO_AGUARDANDO())
		{
			c_lixo = USART_recebe(NULL);
			recepcao_pacote[i] = c_lixo;
			if(i == 0)			// Início do pacote
				recepcao_tamanho_pacote = 0;
			else if(i == 4)		// Número de bytes de dados
			{
				if(c_lixo == 0)
					recepcao_tamanho_pacote = 265;
				else
					recepcao_tamanho_pacote = (unsigned int)c_lixo + 9;
				break;
			}
			else if(i == (recepcao_tamanho_pacote - 1))
			{
				comunicacao_flag = FLAG_PACOTE_RECEBIDO;
				estado_RXTX = ESTADO_VERIFICA_PACOTE_RECEPCAO;
				break;
			}
			i++;
		}
	}

	return timeout;
}
