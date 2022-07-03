// -----------------------------------------------------------------------------
// Arquivo:			teste_acelerometro.c
// Modulo:			Teste do acelerometro
// Autor:			Mayara de Sosua
// Versão:			1.5
// Modificado em:	28/02/2012
// -----------------------------------------------------------------------------

#include <stdio.h>				// BIBLIOTECA DE DEFINIÇOES DO COMPONENTE ESPECIFICADO("ATMEGA328")
#include <avr/interrupt.h>		// Macros para o tratamento de interrupções
#include "ATmega328.h"			// Macro-funções de configuração para o ATmega328
#include "LS_LCD.h"				// Macro-funções de configuração para o LCD
#include "defines.h"			// Definições para o projeto
#include "ACELEROMETRO.h"		// Macro-funções de configuração de pinos para o Acelerometro

// -----------------------------------------------------------------------------
// Declaração das variáveis globais --------------------------------------------

volatile unsigned int valor_x;
volatile unsigned int valor_y;
volatile unsigned int valor_z;
volatile unsigned char canal;
volatile unsigned char estado;
volatile unsigned int cont1;

//Definindo LCD com dispositivo de saída
FILE stream = FDEV_SETUP_STREAM(LCD_escreve, NULL, _FDEV_SETUP_WRITE);

// -----------------------------------------------------------------------------
// Declaração de funções -------------------------------------------------------
ISR(ADC_vect);

// -----------------------------------------------------------------------------
//Programa principal
int main(void)
{
	
	DDRB = 0b11000000;//Cartão SD e D+
	DDRC = 0b0111000;//Eixos x,y,z(PC0, PC1, PC2),SEL0(PC3), SEL1(PC4), Sleep(PC5)
	PORTC= 0b1110000;//Pull ups PC6, PC5 e PC4 ligados
	DDRD = 0b11111111;//LCD
	
	valor_x = 0;
	valor_y = 0;
	valor_z = 0;
	canal = 0;
	estado = 0;

	// Desabilita a USART
	USART_DESATIVA_RECEPTOR();
	USART_DESATIVA_TRANSMISSOR();

	PULL_UPS_TODOS_NORMAIS();
	
	// ADC
	ADC_REFERENCIA_AREF();
	ADC_AJUSTA_RESULTADO_DIREITA();
	ADC_DESABILITA_ENTRADA_DIGITAL_0();
	ADC_DESABILITA_ENTRADA_DIGITAL_1();
	ADC_DESABILITA_ENTRADA_DIGITAL_2();
	ADC_TRIGGER_CONTINUO();
	ADC_DESABILITA_MODO_AUTOMATICO();
	ADC_CLOCK_PRESCALER_128();
	ADC_HABILITA();
	ADC_ATIVA();
	ADC_SELECIONA_CANAL_0();
	ADC_INICIA();
	
	//LCD
	LCD_inicia();
	stdout = stdin = stderr = &stream;

	// Acelerômetro
	ACEL_rang_1_5g();	//Escolhe rang de 1.5g
	ACEL_desativa_modo_sleep();
	sei();//Habilita interrupções
	printf("  ACELEROMETRIA  ");
	
	while(valor_x == 0)//Não sai do laço, somente depois de alterar X
	{
		ADC_INICIA();
	}	// Não faz nada pois é a primeira conversão
	
	while(1)
	{
		switch(estado)
		{
			case 0:	// OCIOSO
				break;
			case 1: // ATUALIZA DISPLAY
				LCD_move_segunda_linha();
				printf("%03d, %03d, %03d",valor_x,valor_y,valor_z);
				ADC_INICIA();
				estado = 0;
				break;
		}
		
	}
	return 0;
}


// -----------------------------------------------------------------------------
// Tratamento da interrupção do ADC --------------------------------------------

ISR(ADC_vect)
{
	switch(canal)
	{
		case 0:
			valor_x = ADC;	//Captura o valor do ADC0 e coloca em X
			ADC_SELECIONA_CANAL_1();
			ADC_INICIA();
			canal++;
			break;
		case 1:
			valor_y = ADC;//Captura o valor do ADC1 e coloca em Y
			ADC_SELECIONA_CANAL_2();
			ADC_INICIA();
			canal++;
			break;
		case 2:
			valor_z = ADC;//Captura o valor do ADC2 e coloca em Z
			ADC_SELECIONA_CANAL_0();
			canal = 0;
			estado = 1;
			break;
	}
}
