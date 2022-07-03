// -----------------------------------------------------------------------------
// Projeto:			Receptor ZigBee
// Dispositivo:		AT90USB646
// Autores:			Mayara de Sousa
//					Bruna Ramos
// 					Leandro Schwarz
// Versão:			2.0
// Modificado em:	18/11/2011
// -----------------------------------------------------------------------------

// Definições ------------------------------------------------------------------
// -----------------------------------------------------------------------------

#define F_CPU 16000000UL
#define HANDLER_LCD
#define HANDLER_USART_8BITS

#define ESTACAO_NUMERO					0x0A

#define ESTADO_OCIOSO					0
#define ESTADO_RECEBE_PACOTE			1
#define ESTADO_VERIFICA_PACOTE_RECEPCAO	2
#define ESTADO_EXECUTA_PACOTE_RECEPCAO	3
#define ESTADO_MONTA_PACOTE_TRANSMISSAO	4
#define ESTADO_TRANSMITE_PACOTE			5
#define ESTADO_DESCARTA_PACOTE			6

#define ESTADO_OCIOSO					0
#define ESTADO_LE_HORA					1
#define ESTADO_AJUSTA_74595				2
#define ESTADO_REAJUSTA_74595			3
#define ESTADO_ATUALIZA_TELA			4

#define MENU_RELOGIO			0
#define MENU_CRONOMETRO			1
#define MENU_ALARME_1			2
#define MENU_ALARME_2			3
#define MENU_MUDA_COR			4
#define MENU_BIG_NUMBER			5

#define SN74595_PORT		PORTA
#define SN74595_PIN			PINA
#define SN74595_RCLK		PA0
#define SN74595_SRCLK		PA1
#define SN74595_SRCLR		PA2
#define SN74595_OE			PA3
#define SN74595_DI			PA4

#define BOTOES_PORT			PORTC
#define BOTOES_PIN			PINC
#define BOTAO_0				PC4
#define BOTAO_1				PC5
#define BOTAO_2				PC6
#define BOTAO_3				PC7

#define RTC_MODO_TESTE
#define RTC_INT_PIN			PINE
#define RTC_INT_PORT		PORTE
#define RTC_INTA			PE4
#define RTC_INTB			PE5

#define LCD_BL_PIN			PINB
#define LCD_BL_PORT			PORTB
#define LCD_BL_RED			PB6
#define LCD_BL_GREEN		PB5
#define LCD_BL_BLUE			PB4

#define COR_ROSA_VERMELHO	255
#define COR_ROSA_VERDE		40
#define COR_ROSA_AZUL		111
#define COR_VERDE_VERMELHO	29
#define COR_VERDE_VERDE		236
#define COR_VERDE_AZUL		23



// Arquivos necessários --------------------------------------------------------
// -----------------------------------------------------------------------------

#include "defines.h"
#include "AT90USB646.h"
#include "LS_LCD.h"
#include "LS_DS1337.h"
#include "funcoes.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Declaração de funções -------------------------------------------------------
// -----------------------------------------------------------------------------

ISR(USART1_TX_vect);
ISR(USART1_RX_vect);
ISR(TIMER2_OVF_vect);
ISR(TIMER0_OVF_vect);					// Interrupção para o PWM do backlight
ISR(INT5_vect);							// Interrupção para o oscilador de 1 Hz do DS1337


// Declaração das variáveis globais --------------------------------------------
// -----------------------------------------------------------------------------

unsigned char	recepcao_pacote[266];		// String para guardar o pacote recebido
unsigned int	recepcao_indice;			// Índice do pacote de recepção
volatile unsigned char	comunicacao_flag;			// Flag do estado da comunicação
unsigned int	recepcao_tamanho_pacote;	// Tamanho do pacote de recepção
unsigned char	transmissao_pacote[266];	// String para guardar o pacote a ser transmitido
unsigned int	transmissao_indice;			// Índice do pacote de transmissão
unsigned int	transmissao_tamanho_pacote;	// Tamanho do pacote de transmissão
unsigned char	string_lixo[8];				// String auxiliar
volatile unsigned char	estado_RXTX;				// Máquina de estados da comunicação
volatile unsigned char	estado_MAQUINA;				// Máquina de estados do sistema
volatile unsigned char	PWM_vermelho1;				// Intensidade de vermelho do backlight
volatile unsigned char	PWM_verde1;					// Intensidade de verde do backlight
volatile unsigned char	PWM_azul1;					// Intensidade de azul do backlight
volatile unsigned char	PWM_vermelho2;				// Intensidade de vermelho do backlight
volatile unsigned char	PWM_verde2;					// Intensidade de verde do backlight
volatile unsigned char	PWM_azul2;					// Intensidade de azul do backlight
volatile unsigned char	PWM_contador;				// Contador para o PWM do backlight
volatile unsigned char menu;			// Menu ativo no display
volatile unsigned char segundo;


// Função principal ------------------------------------------------------------
// -----------------------------------------------------------------------------

int main()
{
	// Declaração de variáveis locais
	unsigned char c_lixo;
	unsigned int i_lixo;
	unsigned char i, j;
	unsigned char led_seg[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	unsigned char hora;
	unsigned char minuto;
	unsigned char dia_mes;
	unsigned char mes;
	unsigned char ano;
	unsigned char dia_semana;

	// Inicialização de variáveis
	estado_RXTX				= ESTADO_OCIOSO;
	estado_MAQUINA			= ESTADO_OCIOSO;
	recepcao_indice			= 0;
	recepcao_tamanho_pacote	= 0;
	comunicacao_flag		= FLAG_NENHUM_PACOTE;
	PWM_vermelho1	= COR_ROSA_VERMELHO;
	PWM_verde1		= COR_ROSA_VERDE;
	PWM_azul1		= COR_ROSA_AZUL;
	PWM_vermelho2	= COR_VERDE_VERMELHO;
	PWM_verde2		= COR_VERDE_VERDE;
	PWM_azul2		= COR_VERDE_AZUL;
	PWM_contador	= 0;
	hora			= 0;
	minuto			= 0;
	segundo			= 0;
	dia_mes			= 0;
	mes				= 0;
	ano				= 0;
	dia_semana		= 0;

	// Configuração dos ports
	DDRA = 0b11111111;
	DDRB = 0b11111111;
//DDRC = 0b11111111;
	DDRC = 0b00000000;
	DDRD = 0b00001000;
	PORTD = 0b11111111;
	DDRE = 0b00000000;
	DDRF = 0b00000000;

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
	TIMER0_CLOCK_PRESCALER_64();
	TIMER0_ATIVA();
	TIMER0_CONFIGURA(247);

	// Configuração do Timer2
	TIMER2_CLOCK_DESABILITA();
	TIMER2_DESATIVA();

	// Configuração do 74595
	clr_bit(SN74595_PORT,SN74595_SRCLR);
	set_bit(SN74595_PORT,SN74595_SRCLR);
	clr_bit(SN74595_PORT,SN74595_OE);

	// Configuração dos botões
	set_bit(BOTOES_PORT,BOTAO_0);
	set_bit(BOTOES_PORT,BOTAO_1);
	set_bit(BOTOES_PORT,BOTAO_2);
	set_bit(BOTOES_PORT,BOTAO_3);

	// Configuração da interrupção externa INT5
	INT5_CONFIGURA_BORDA_SUBIDA();
	INT5_ATIVA();

	// Configuração do LCD
	LCD_inicia();
	clr_bit(LCD_BL_PORT,LCD_BL_RED);	// Aciona o backlight vermelho
	clr_bit(LCD_BL_PORT,LCD_BL_GREEN);	// Aciona o backlight verde
	clr_bit(LCD_BL_PORT,LCD_BL_BLUE);	// Aciona o backlight azul
LCD_stdio();
printf("foi-mesmo");

	// Configuração do RTC
	clr_bit(TWSR,TWPS1);				// Configura o prescaler para 1
	clr_bit(TWSR,TWPS0);				// Configura o prescaler para 1
	TWBR = 123;							// Freqüência de 16 kHz
	set_bit(RTC_INT_PORT,RTC_INTA);		// Pull-up das interrupções do DS1337
	set_bit(RTC_INT_PORT,RTC_INTB);		// Pull-up das interrupções do DS1337
	RTC_MODO_24_HORAS();
	RTC_ATIVA_OSCILADOR_1HZ();

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

					switch(estado_MAQUINA)
					{
						case ESTADO_OCIOSO:
							break;
						case ESTADO_LE_HORA:
							RTC_le(RTC_HORAS);
							hora = ((RTC_horas >> 4) * 10) + (RTC_horas & 0x0F);
							RTC_le(RTC_MINUTOS);
							minuto = ((RTC_minutos >> 4) * 10) + (RTC_minutos & 0x0F);
							RTC_le(RTC_SEGUNDOS);
							segundo = ((RTC_segundos >> 4) * 10) + (RTC_segundos & 0x0F);
							RTC_le(RTC_DIAMES);
							dia_mes = ((RTC_diames >> 4) * 10) + (RTC_diames & 0x0F);
							RTC_le(RTC_MES);
							mes = ((RTC_mes >> 4) * 10) + (RTC_mes & 0x0F);
							RTC_le(RTC_ANO);
							ano = ((RTC_ano >> 4) * 10) + (RTC_ano & 0x0F);
							RTC_le(RTC_DIASEMANA);
							dia_semana = RTC_diasemana;
							estado_MAQUINA = ESTADO_ATUALIZA_TELA;
							break;
						case ESTADO_ATUALIZA_TELA:
							if(menu == MENU_RELOGIO)
							{
								LCD_stdio();
								LCD_move_primeira_linha();
								printf("%02d:%02d:%02d\n",hora,minuto,segundo);
								LCD_move_segunda_linha();
								switch(dia_semana)
								{
									case 1:	printf("%02d/%02d/20%02d  Dom\n",dia_mes,mes,ano);break;
									case 2:	printf("%02d/%02d/20%02d  Seg\n",dia_mes,mes,ano);break;
									case 3:	printf("%02d/%02d/20%02d  Ter\n",dia_mes,mes,ano);break;
									case 4:	printf("%02d/%02d/20%02d  Qua\n",dia_mes,mes,ano);break;
									case 5:	printf("%02d/%02d/20%02d  Qui\n",dia_mes,mes,ano);break;
									case 6:	printf("%02d/%02d/20%02d  Sex\n",dia_mes,mes,ano);break;
									case 7:	printf("%02d/%02d/20%02d  Sab\n",dia_mes,mes,ano);break;
								}
								for(j = 0;j < 8; j++)
									led_seg[j] = 0x00;
								if(segundo < 8)
								{
									for(j = 0;j < (segundo + 1);j++)
										led_seg[0] |= 1 << j;
								}
								else if(segundo < 16)
								{
									led_seg[0] = 0xFF;
									for(j = 0;j < (segundo - 7);j++)
										led_seg[1] |= 1 << j;
								}
								else if(segundo < 24)
								{
									led_seg[0] = 0xFF;
									led_seg[1] = 0xFF;
									for(j = 0;j < (segundo - 15);j++)
										led_seg[2] |= 1 << j;
								}
								else if(segundo < 32)
								{
									led_seg[0] = 0xFF;
									led_seg[1] = 0xFF;
									led_seg[2] = 0xFF;
									for(j = 0;j < (segundo - 23);j++)
										led_seg[3] |= 1 << j;
								}
								else if(segundo < 40)
								{
									led_seg[0] = 0xFF;
									led_seg[1] = 0xFF;
									led_seg[2] = 0xFF;
									led_seg[3] = 0xFF;
									for(j = 0;j < (segundo - 31);j++)
										led_seg[4] |= 1 << j;
								}
								else if(segundo < 48)
								{
									led_seg[0] = 0xFF;
									led_seg[1] = 0xFF;
									led_seg[2] = 0xFF;
									led_seg[3] = 0xFF;
									led_seg[4] = 0xFF;
									for(j = 0;j < (segundo - 39);j++)
										led_seg[5] |= 1 << j;
								}
								else if(segundo < 56)
								{
									led_seg[0] = 0xFF;
									led_seg[1] = 0xFF;
									led_seg[2] = 0xFF;
									led_seg[3] = 0xFF;
									led_seg[4] = 0xFF;
									led_seg[5] = 0xFF;
									for(j = 0;j < (segundo - 47);j++)
										led_seg[6] |= 1 << j;
								}
								else
								{
									led_seg[0] = 0xFF;
									led_seg[1] = 0xFF;
									led_seg[2] = 0xFF;
									led_seg[3] = 0xFF;
									led_seg[4] = 0xFF;
									led_seg[5] = 0xFF;
									led_seg[6] = 0xFF;
									for(j = 0;j < (segundo - 55);j++)
										led_seg[7] |= 1 << j;
								}
								if((minuto % 2) != 0)
								{
									led_seg[0] ^= 0xFF;
									led_seg[1] ^= 0xFF;
									led_seg[2] ^= 0xFF;
									led_seg[3] ^= 0xFF;
									led_seg[4] ^= 0xFF;
									led_seg[5] ^= 0xFF;
									led_seg[6] ^= 0xFF;
									led_seg[7] ^= 0xFF;
								}
								clr_bit(SN74595_PORT,SN74595_SRCLR);
								set_bit(SN74595_PORT,SN74595_SRCLR);
								for(i = 0;i < 8;i++)
								{
									for(j = 0;j < 8;j++)
									{
										if((led_seg[i] & (1 << j)) == (1 << j))
											set_bit(SN74595_PORT,SN74595_DI);
										else
											clr_bit(SN74595_PORT,SN74595_DI);
										set_bit(SN74595_PORT,SN74595_SRCLK);
										clr_bit(SN74595_PORT,SN74595_SRCLK);
									}
								}
								set_bit(SN74595_PORT,SN74595_RCLK);
								clr_bit(SN74595_PORT,SN74595_RCLK);
								estado_MAQUINA = ESTADO_OCIOSO;
								sei();
							}
							else if(menu == MENU_BIG_NUMBER)
							{
								LCD_move_primeira_linha();
								printf("Big Number...");
							}
							break;
					}

				}
				break;

				// ----------------------------------------------------------------------
				// Estado que fica recebendo o pacote até o termino ou timeout
				// ----------------------------------------------------------------------
				case ESTADO_RECEBE_PACOTE:
	TIMER2_CLOCK_DESABILITA();
	TIMER2_DESATIVA();
	TIMER2_CONFIGURA(200);
	TIMER2_CLOCK_PRESCALER_1024();
	TIMER2_ATIVA();
	set_bit(PORTB,PB7);
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
clr_bit(PORTB,PB7);
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
					case COMANDO_VERIFICAR_ESTACAO_ATIVA:
LCD_stdio();
LCD_move_primeira_linha();
printf("Reportar atividade\n\n");
						// NADA
						break;

					case COMANDO_EXECUTAR_AUTO_TESTE:
						string_lixo[7] = executar_auto_teste();
						break;

					case COMANDO_LER_HORA_E_DATA:
						string_lixo[7] = leitura_hora_data(string_lixo);
						break;

					case COMANDO_AJUSTAR_HORA_E_DATA:
						string_lixo[0] = recepcao_pacote[6];//ano
						string_lixo[1] = recepcao_pacote[7];//mes
						string_lixo[2] = recepcao_pacote[8];//dia
						string_lixo[3] = recepcao_pacote[9];//dia semana
						string_lixo[4] = recepcao_pacote[10];//hora
						string_lixo[5] = recepcao_pacote[11];//min
						string_lixo[6] = recepcao_pacote[12];//seg
						string_lixo[7] = ajusta_hora_data(string_lixo);
						break;

					case COMANDO_LER_ALARMES:
						break;

					case COMANDO_AJUSTAR_ALARMES:
						break;

					case COMANDO_LCD_LER_COR_FUNDO_PRINCIPAL:
						string_lixo[7] = LCD_leitura_cor_fundo_principal(string_lixo);
						break;

					case COMANDO_LCD_LER_COR_FUNDO_SECUNDARIA:
						string_lixo[7] = LCD_leitura_cor_fundo_secundaria(string_lixo);
						break;

					case COMANDO_LCD_AJUSTAR_COR_FUNDO_PRINCIPAL:
						string_lixo[0] = recepcao_pacote[6];
						string_lixo[1] = recepcao_pacote[7];
						string_lixo[2] = recepcao_pacote[8];
						string_lixo[7] = LCD_ajusta_cor_fundo_principal(string_lixo);
						break;

					case COMANDO_LCD_AJUSTAR_COR_FUNDO_SECUNDARIA:
						string_lixo[0] = recepcao_pacote[6];
						string_lixo[1] = recepcao_pacote[7];
						string_lixo[2] = recepcao_pacote[8];
						string_lixo[7] = LCD_ajusta_cor_fundo_secundaria(string_lixo);
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
					case COMANDO_VERIFICAR_ESTACAO_ATIVA:
						transmissao_pacote[c_lixo++] = 0x01;
						transmissao_pacote[c_lixo++] = COMANDO_RESPONDER_AFIRMATIVAMENTE;
						break;

					case COMANDO_EXECUTAR_AUTO_TESTE:
					case COMANDO_AJUSTAR_HORA_E_DATA:
					case COMANDO_AJUSTAR_ALARMES:
					case COMANDO_LCD_AJUSTAR_COR_FUNDO_PRINCIPAL:
					case COMANDO_LCD_AJUSTAR_COR_FUNDO_SECUNDARIA:
						transmissao_pacote[c_lixo++] = 0x01;
						transmissao_pacote[c_lixo++] = string_lixo[7];
						break;

					case COMANDO_LER_HORA_E_DATA:
						if(string_lixo[7] == COMANDO_RESPONDER_AFIRMATIVAMENTE)
						{
							transmissao_pacote[c_lixo++] = 0x08;
							transmissao_pacote[c_lixo++] = COMANDO_RETORNAR_HORA_E_DATA;
							transmissao_pacote[c_lixo++] = string_lixo[0];//ano
							transmissao_pacote[c_lixo++] = string_lixo[1];
							transmissao_pacote[c_lixo++] = string_lixo[2];
							transmissao_pacote[c_lixo++] = string_lixo[3];
							transmissao_pacote[c_lixo++] = string_lixo[4];
							transmissao_pacote[c_lixo++] = string_lixo[5];
							transmissao_pacote[c_lixo++] = string_lixo[6];
						}
						else
						{
							transmissao_pacote[c_lixo++] = 0x01;
							transmissao_pacote[c_lixo++] = COMANDO_RESPONDER_NEGATIVAMENTE;
						}
						break;

					case COMANDO_LER_ALARMES:
transmissao_pacote[c_lixo++] = 0x01;
transmissao_pacote[c_lixo++] = COMANDO_RESPONDER_NEGATIVAMENTE;
						break;

					case COMANDO_LCD_LER_COR_FUNDO_PRINCIPAL:
					case COMANDO_LCD_LER_COR_FUNDO_SECUNDARIA:
						if(string_lixo[7] == COMANDO_RESPONDER_AFIRMATIVAMENTE)
						{
							transmissao_pacote[c_lixo++] = 0x04;
							transmissao_pacote[c_lixo++] = string_lixo[0];
							transmissao_pacote[c_lixo++] = string_lixo[1];
							transmissao_pacote[c_lixo++] = string_lixo[2];
						}
						else
						{
							transmissao_pacote[c_lixo++] = 0x01;
							transmissao_pacote[c_lixo++] = COMANDO_RESPONDER_NEGATIVAMENTE;
						}
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
				estado_RXTX = ESTADO_OCIOSO;
				break;
		}

	}

	return 0;
}


ISR(USART1_RX_vect)
{
	comunicacao_flag = FLAG_PACOTE_SENDO_RECEBIDO;
	USART_DESABILITA_INTERRUPCAO_TRANSMISSOR();
	USART_DESABILITA_INTERRUPCAO_RECEPTOR();
	estado_RXTX = ESTADO_RECEBE_PACOTE;
	return;
}

ISR(USART1_TX_vect)
{
	USART_DESABILITA_INTERRUPCAO_TRANSMISSOR();
	USART_DESABILITA_INTERRUPCAO_RECEPTOR();
	estado_RXTX = ESTADO_TRANSMITE_PACOTE;
	return;
}

ISR(TIMER2_OVF_vect)
{
	TIMER2_DESATIVA();
	TIMER2_CLOCK_DESABILITA();
	if(comunicacao_flag == FLAG_PACOTE_SENDO_RECEBIDO)
		estado_RXTX = ESTADO_DESCARTA_PACOTE;
	return;
}


ISR(TIMER0_OVF_vect)
{
	TIMER0_CONFIGURA(247);

	if(!(segundo % 5))	// Verde
	{
		// PWM para o backlight vermelho
		if(PWM_contador > PWM_vermelho2)
			set_bit(LCD_BL_PORT,LCD_BL_RED);
		else
			clr_bit(LCD_BL_PORT,LCD_BL_RED);

		// PWM para o backlight verde
		if(PWM_contador > PWM_verde2)
			set_bit(LCD_BL_PORT,LCD_BL_GREEN);
		else
			clr_bit(LCD_BL_PORT,LCD_BL_GREEN);

		// PWM para o backlight azul
		if(PWM_contador > PWM_azul2)
			set_bit(LCD_BL_PORT,LCD_BL_BLUE);
		else
			clr_bit(LCD_BL_PORT,LCD_BL_BLUE);	
	}
	else				// Rosa
	{
		// PWM para o backlight vermelho
		if(PWM_contador > PWM_vermelho1)
			set_bit(LCD_BL_PORT,LCD_BL_RED);
		else
			clr_bit(LCD_BL_PORT,LCD_BL_RED);

		// PWM para o backlight verde
		if(PWM_contador > PWM_verde1)
			set_bit(LCD_BL_PORT,LCD_BL_GREEN);
		else
			clr_bit(LCD_BL_PORT,LCD_BL_GREEN);

		// PWM para o backlight azul
		if(PWM_contador > PWM_azul1)
			set_bit(LCD_BL_PORT,LCD_BL_BLUE);
		else
			clr_bit(LCD_BL_PORT,LCD_BL_BLUE);
	}

	PWM_contador++;
	return;
}

ISR(INT5_vect)
{
	cli();				// Desabilita interrupções
	estado_MAQUINA = ESTADO_LE_HORA;
	return;
}
