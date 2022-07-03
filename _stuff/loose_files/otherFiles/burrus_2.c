// -----------------------------------------------------------------------------
// Arquivo:			burrus_2.h
// DIspositivo:		ATmega328
// Autor:			
// Modificado em:	08/11/2011
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Definições necessárias ------------------------------------------------------

#define HANDLER_LCD
#define HANDLER_USART_8BITS
#define F_CPU 16000000

#define LED_COR_AM	0
#define LED_COR_AZ	1
#define LED_COR_VD	2
#define LED_COR_VM	3

#define TEMPO_LED_ACESO		300
#define TEMPO_ENTRE_LEDS	300

#define NIVEL_MAXIMO		4

#define ESTADO_OCIOSO		0
#define ESTADO_ACENDE_LEDS	1
#define ESTADO_LE_BOTOES	2
#define ESTADO_VERIFICA_COR	3

#define FLAG_JOGO_NAO_INICIADO	0
#define FLAG_JOGO_INICIADO		1
#define FLAG_JOGO_PERDEDOR		2
#define FLAG_JOGO_VENCEDOR		3

// -----------------------------------------------------------------------------
// Arquivos necessários --------------------------------------------------------

#include "defines.h"
#include "ATmega328.h"
#include "LS_LCD.h"
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>

// -----------------------------------------------------------------------------
// Definições dos pinos --------------------------------------------------------

#define LEDS_PORT		PORTD
#define LEDS_VD			PD2
#define LEDS_AZ			PD3
#define LEDS_VM			PD4
#define LEDS_AM			PD5
#define BUZZER_PORT		PORTD
#define BUZZER_BIT		PD6
#define BOTOES_PORT		PORTC
#define BOTOES_PIN		PINC
#define BOTOES_VD		PC0
#define BOTOES_AZ		PC1
#define BOTOES_VM		PC2
#define BOTOES_AM		PC3

// -----------------------------------------------------------------------------
// Variáveis globais -----------------------------------------------------------

volatile unsigned char estado;
volatile unsigned char flag;

// -----------------------------------------------------------------------------
// Declarações das funções -----------------------------------------------------

void AcendeLED(unsigned char cor);
void AnunciaPerdedor(void);
void AnunciaVencedor(void);
ISR(TIMER1_OVF_vect);

// -----------------------------------------------------------------------------
// Função Principal ------------------------------------------------------------

int main()
{

	// Variáveis locais
	unsigned char sequencia[NIVEL_MAXIMO];
	unsigned char nivel_atual;
	unsigned char nivel_jogador = 0;
	unsigned char i;
	unsigned char j;
	estado = ESTADO_OCIOSO;
	nivel_atual = 2;
	flag = FLAG_JOGO_NAO_INICIADO;
	
	// Configuração dos pinos
	DDRB = 0b00111111;
	DDRC = 0b00110000;
	DDRD = 0b11111100;

	// Condição inicial
	set_bit(LEDS_PORT,LEDS_VD);
	set_bit(LEDS_PORT,LEDS_VM);
	set_bit(LEDS_PORT,LEDS_AZ);
	set_bit(LEDS_PORT,LEDS_AM);
	set_bit(BOTOES_PORT,BOTOES_VD);
	set_bit(BOTOES_PORT,BOTOES_VM);
	set_bit(BOTOES_PORT,BOTOES_AZ);
	set_bit(BOTOES_PORT,BOTOES_AM);

	// Inicializa o LCD
	LCD_inicia();

	// Configura a USART
	USART_configura(USART_MODO_ASSINCRONO_NORMAL);
	USART_PARIDADE_DESABILITADA();
	USART_STOP_BIT_SIMPLES();
	USART_DATA_BITS_8();
	USART_HABILITA_INTERRUPCAO_RECEPTOR();
	USART_HABILITA_INTERRUPCAO_TRANSMISSOR();
	USART_ATIVA_RECEPTOR();
	USART_ATIVA_TRANSMISSOR();

	// Configura Timer0
	TIMER1_CLOCK_DESABILITA();
	TIMER1_DESATIVA();
	TIMER1_CONFIGURA(0);

	// Escreve no LCD
//	stdin = stdout = stderr = &USART_stream;
//	printf("Teste do LCD\nNumero = %d",2578);

	// Habilita interrupções
	sei();

	while(1)
	{
		switch(estado)
		{
			case ESTADO_OCIOSO:
			nivel_atual = 2;
			nivel_jogador = 0;
			flag = FLAG_JOGO_NAO_INICIADO;
			// Aleatoriza o vetor
			LCD_stdio();
			LCD_move_primeira_linha();
			printf("     BURRUS     \nAperte o verde! ");
			i = 0;
			while(tst_bit(BOTOES_PIN,BOTOES_VD) == 1);	// Enquanto não for pressionado
			_delay_ms(100);
			while(tst_bit(BOTOES_PIN,BOTOES_VD) == 0)	// Enquanto está pressionado
				i++;
			srand(i);
			//stdin = stdout = stderr = &USART_stream;
			for(i = 0;i < NIVEL_MAXIMO;i++)
			{
				sequencia[i] = rand() % 4;
				//printf("%d ",sequencia[i]);
			}
			LCD_move_primeira_linha();
			printf("     BURRUS     \n                ");
			flag = FLAG_JOGO_INICIADO;
			estado = ESTADO_ACENDE_LEDS;
				break;
			case ESTADO_ACENDE_LEDS:
				for(i = 0;i <= nivel_atual;i++)
					AcendeLED(sequencia[i]);
				nivel_jogador = 0;
				estado = ESTADO_LE_BOTOES;
				break;
			case ESTADO_LE_BOTOES:
				j = 4;
				TIMER1_CONFIGURA(0);
				TIMER1_CLOCK_PRESCALER_256();
				TIMER1_ATIVA();
				while(flag == FLAG_JOGO_INICIADO)
				{
					if(tst_bit(BOTOES_PIN,BOTOES_VD) == 0)	// Pressionado verde
					{
						_delay_ms(50);
						while(tst_bit(BOTOES_PIN,BOTOES_VD) == 0);
						j = LED_COR_VD;
						break;
					}
					if(tst_bit(BOTOES_PIN,BOTOES_VM) == 0)	// Pressionado vermelho
					{
						_delay_ms(50);
						while(tst_bit(BOTOES_PIN,BOTOES_VM) == 0);
						j = LED_COR_VM;
						break;
					}
					if(tst_bit(BOTOES_PIN,BOTOES_AZ) == 0)	// Pressionado azul
					{
						_delay_ms(50);
						while(tst_bit(BOTOES_PIN,BOTOES_AZ) == 0);
						j = LED_COR_AZ;
						break;
					}
					if(tst_bit(BOTOES_PIN,BOTOES_AM) == 0)	// Pressionado amarelo
					{
						_delay_ms(50);
						while(tst_bit(BOTOES_PIN,BOTOES_AM) == 0);
						j = LED_COR_AM;
						break;
					}
				}
				// Verificar se está certo
				if((sequencia[nivel_jogador] == j) && (flag == FLAG_JOGO_INICIADO))
				{
					TIMER1_CONFIGURA(0);
					TIMER1_CLOCK_DESABILITA();
					TIMER1_DESATIVA();
					if(nivel_jogador == nivel_atual)
					{
						if(nivel_atual == (NIVEL_MAXIMO - 1))
							flag = FLAG_JOGO_VENCEDOR;
						else
						{
							nivel_atual++;
							estado = ESTADO_ACENDE_LEDS;
						}
					}
					else
					{
						nivel_jogador++;
					}
				}
				else
				{
					flag = FLAG_JOGO_PERDEDOR;
				}
				if(flag == FLAG_JOGO_PERDEDOR)
				{
					TIMER1_CONFIGURA(0);
					TIMER1_CLOCK_DESABILITA();
					TIMER1_DESATIVA();
					AnunciaPerdedor();
					estado = ESTADO_OCIOSO;
				}
				if(flag == FLAG_JOGO_VENCEDOR)
				{
					TIMER1_CONFIGURA(0);
					TIMER1_CLOCK_DESABILITA();
					TIMER1_DESATIVA();
					AnunciaVencedor();
					estado = ESTADO_OCIOSO;
				}
				break;
		}
		
	}
	return 0;
}

void AcendeLED(unsigned char cor)
{
	switch(cor)
	{
		case LED_COR_AM:
			clr_bit(LEDS_PORT,LEDS_AM);
			_delay_ms(TEMPO_LED_ACESO);
			set_bit(LEDS_PORT,LEDS_AM);
			_delay_ms(TEMPO_ENTRE_LEDS);
			break;
		case LED_COR_AZ:
			clr_bit(LEDS_PORT,LEDS_AZ);
			_delay_ms(TEMPO_LED_ACESO);
			set_bit(LEDS_PORT,LEDS_AZ);
			_delay_ms(TEMPO_ENTRE_LEDS);
			break;
		case LED_COR_VD:
			clr_bit(LEDS_PORT,LEDS_VD);
			_delay_ms(TEMPO_LED_ACESO);
			set_bit(LEDS_PORT,LEDS_VD);
			_delay_ms(TEMPO_ENTRE_LEDS);
			break;
		case LED_COR_VM:
			clr_bit(LEDS_PORT,LEDS_VM);
			_delay_ms(TEMPO_LED_ACESO);
			set_bit(LEDS_PORT,LEDS_VM);
			_delay_ms(TEMPO_ENTRE_LEDS);
			break;
	}
	return;
}

void AnunciaPerdedor(void)
{
	LCD_stdio();
	LCD_move_primeira_linha();
	printf("     BURRUS     \nPERDEU! BURRO!!!");
	_delay_ms(3000);
	return;
}

void AnunciaVencedor(void)
{
	LCD_stdio();
	LCD_move_primeira_linha();
	printf(" BURRUS NAO !!! \nGENIUS !!!!!!!  ");
	_delay_ms(3000);
	return;
}

ISR(TIMER1_OVF_vect)
{
	TIMER1_CONFIGURA(0);
	TIMER1_CLOCK_DESABILITA();
	TIMER1_DESATIVA();
	flag = FLAG_JOGO_PERDEDOR;
	return;
}

