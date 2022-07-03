// --------------------------------------------------------------------------------
// File:			ZigBee.c
// Module:			Programação base do ZigBee
// Author:			Mayara de Sousa
// Version:			1.0
// Last edition:	19/06/2012
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// Global definitions -------------------------------------------------------------

#define F_CPU 8000000UL
#define HANDLER_LCD
#define HANDLER_USART_8BITS

// --------------------------------------------------------------------------------
// Include files ------------------------------------------------------------------

#include "LS_defines.h"
#include "defines.h"
#include "AT90USB1287.h"
#include "LS_keypad.h"
#include "LS_LCD.h"
#include "LS_DS1337.h"
#include "LS_job.h"
#include "LS_ZigBee.h"
#include "LS_circbuffer.h"
#include "funcoes_data_hora.h"
#include "funcoes_LCD.h"
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/eeprom.h>

// --------------------------------------------------------------------------------
// Global variables ---------------------------------------------------------------

volatile unsigned char flags;							// System flags
volatile unsigned char estado_MAQUINA;					// System states machine
volatile unsigned char menu_flag;						// Menu flags
unsigned char menu_opcao;								// Menu selected item
volatile unsigned char contador_PWM_backlight;			// PWM counter for LCD backlight
volatile unsigned char contador_modo_economia;			// Counter for LCD economy mode

ZigBee_node_t ZB_node[5];								// ZigBee connected nodes

extern volatile circbuffer_t ZigBee_reception_buffer;
extern volatile circbuffer_t ZigBee_transmission_buffer;
extern volatile uint8_t ZigBee_transmission_frame_id;
extern volatile ZigBee_api_frame_t ZigBee_api_frame_transmission;

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

ISR(INT5_vect);
ISR(USART1_RX_vect);
ISR(USART1_TX_vect);
ISR(TIMER0_OVF_vect);
ISR(TIMER3_OVF_vect);
void atualiza_tela_menu(unsigned char flag,unsigned char opcao);

// --------------------------------------------------------------------------------
// Main function ------------------------------------------------------------------

int main()
{
	// Local variable declarations
	unsigned char estado = 0;
	unsigned char tecla;

	// Variable definition
	contador_PWM_backlight = 0;

	// Port Initialization
	DDRA = 0b11110100;
	DDRB = 0b11111011;
	DDRD = 0b11100000;
	DDRE = 0b01000011;
	PORTE= 0b00000000;
	DDRF = 0x00;

	// Timer0 configuration
	TIMER0_CLOCK_PRESCALER_8();
	TIMER0_ATIVA();

	// TIMER3 configuration
	TIMER3_CLOCK_PRESCALER_1024();
	TIMER3_CONFIGURA(64755);
	TIMER3_ATIVA();

	// LCD configuration
	set_bit(LCD_DATA_DDR,LCD_D4);
	set_bit(LCD_DATA_DDR,(LCD_D4 + 1));
	set_bit(LCD_DATA_DDR,(LCD_D4 + 2));
	set_bit(LCD_DATA_DDR,(LCD_D4 + 3));
	set_bit(LCD_CONTROL_DDR,LCD_E);
	set_bit(LCD_CONTROL_DDR,LCD_RS);
	set_bit(LCD_BK_DDR,LCD_BK);
	clr_bit(LCD_BK_PORT,LCD_BK);
	LCD_configuration.LCD_bl_intensity = eeprom_read_byte(EEPROM_LCD_BACKLIGHT_INTENSITY_ADDRESS);
	LCD_configuration.LCD_economy_mode = eeprom_read_byte(EEPROM_LCD_ECONOMY_MODE_ADDRESS);
	LCD_init();
	LCD_STDIO();
	printf(" Projeto AcelER \n");
	printf("  Modulo Base   \n");
//_delay_ms(3000);
	LCD_bl_off();

	// Keypad configuration
	keypad_init('1','4','7','*','2','5','8','0','3','6','9','#','A','B','C','D');

	// ZigBee configuration
	ZigBee_init();
	set_bit(ZIGBEE_COMISS_DDR,ZIGBEE_COMISS);
	set_bit(ZIGBEE_ASSOC_DDR,ZIGBEE_ASSOC);
	set_bit(ZIGBEE_SLEEP_REQ_DDR,ZIGBEE_SLEEP_REQ);
	set_bit(ZIGBEE_RESET_DDR,ZIGBEE_RESET);
	set_bit(ZIGBEE_SLEEP_DDR,ZIGBEE_SLEEP);
	set_bit(ZIGBEE_SLEEP_PORT,ZIGBEE_SLEEP);
	set_bit(ZIGBEE_RESET_PORT,ZIGBEE_RESET);

	// RTC configuration
	DS1337_init();
	DS1337_ENABLE();
	DS1337_MODE_24_HOURS();
	DS1337_SQUARE_WAVE_1HZ();
	if(DS1337_check_oscillator_halted())
	{
		LCD_move_primeira_linha();
		printf("Atencao! Relogio\n");
		printf("necessita ajuste\n");
		_delay_ms(2000);
		DS1337_CLEAR_OSCILLATOR_HALTED_FLAG();
	}
	DS1337_get_date();
	DS1337_get_time();

	// External interrupt INT5 configuration
	INT5_CONFIGURA_BORDA_SUBIDA();
	//INT5_ATIVA();

	// USART configuration
	USART_control.mode = USART_MODE_ASYNCHRONOUS_NORMAL;
	USART_control.parity = USART_PARITY_DISABLED;
	USART_control.stop_bits = USART_SINGLE_STOP_BIT;
	USART_control.data_bits = USART_8_DATA_BITS;
	USART_RECEPTION_COMPLETE_INTERRUPT_ENABLE();
	USART_TRANSMISSION_COMPLETE_INTERRUPT_ENABLE();
	USART_TRANSMITTER_ENABLE();
	USART_RECEIVER_ENABLE();
	USART_init();

	// System Flags initialization
	clr_bit(flags,FLAG_RELOGIO);
	clr_bit(flags,FLAG_LCD_BL_ON_OFF);

menu_flag = MENU_NENHUM;
estado = ESTADO_OCIOSO;
menu_opcao = 0;
atualiza_tela_menu(menu_flag,menu_opcao);


clr_bit(ZIGBEE_RESET_PORT,ZIGBEE_RESET);
_delay_us(10);
set_bit(ZIGBEE_RESET_PORT,ZIGBEE_RESET);


	// Enable global interrupts
	sei();

	// Main loop
	while(1)
	{
		//ZigBee_check_transmission_state();

		switch(estado)
		{
			case ESTADO_OCIOSO:
				tecla = keypad_read();
				if(tecla != 0xFF)
				{
					if(!tst_bit(flags,FLAG_LCD_BL_ON_OFF))
						LCD_bl_on();
					else
					{
						switch(tecla)
						{
							case '#':		// Show/hide data/time
								cpl_bit(flags,FLAG_RELOGIO);
								atualiza_tela_menu(menu_flag,menu_opcao);
								break;
							case '*':
								//ZigBee node discovery --- TESTING ONLY
								ZigBee_transmit_ATCommand("ND", NULL,0);

								break;
							default:		// Enter menu
								menu_flag = MENU_PRINCIPAL;
								menu_opcao = 0;
								estado = ESTADO_MENU_PRINCIPAL;
								atualiza_tela_menu(menu_flag,menu_opcao);
								break;
						}
					}
				}
				break;
			case ESTADO_MENU_PRINCIPAL:
				tecla = keypad_read();
				if(tecla != 0xFF)
				{
					if(!tst_bit(flags,FLAG_LCD_BL_ON_OFF))
						LCD_bl_on();
					else
					{
						switch(tecla)
						{
							case 'A':		// Return to previous menu
								menu_opcao = 0;
								menu_flag = MENU_NENHUM;
								estado = ESTADO_OCIOSO;
								atualiza_tela_menu(menu_flag,menu_opcao);
								break;
							case 'B':		// Previous menu item
								menu_opcao = (menu_opcao == 0)?4:menu_opcao-1;
								atualiza_tela_menu(menu_flag,menu_opcao);
								break;
							case 'C':		// Next menu item
								menu_opcao = (menu_opcao == 4)?0:menu_opcao+1;
								atualiza_tela_menu(menu_flag,menu_opcao);
								break;
							case 'D':		// Accept
								menu_flag = MENU_PRINCIPAL + menu_opcao + 1;
								menu_opcao = 0;
								estado = ESTADO_MENU_SECUNDARIO;
								atualiza_tela_menu(menu_flag,menu_opcao);
								break;
						}
					}
				}
				break;
			case ESTADO_MENU_SECUNDARIO:
				tecla = keypad_read();
				if(tecla != 0xFF)
				{
					if(!tst_bit(flags,FLAG_LCD_BL_ON_OFF))
						LCD_bl_on();
					else
					{
						switch(tecla)
						{
							case 'A':		// Return to previous menu
								menu_opcao = 0;
								menu_flag = MENU_PRINCIPAL;
								estado = ESTADO_MENU_PRINCIPAL;
								atualiza_tela_menu(menu_flag,menu_opcao);
								break;
							case 'B':		// Previous menu item
								switch(menu_flag)
								{
									case MENU_CARTAO_SD:
										break;
									case MENU_CONFIGURA_REDE:
										break;
									case MENU_CONFIGURA_USB:
										break;
									case MENU_CONFIGURA_DATA_HORA:
										menu_opcao = (menu_opcao == 0)?1:0;
										atualiza_tela_menu(menu_flag,menu_opcao);
										break;
									case MENU_CONFIGURA_LCD:
										menu_opcao = (menu_opcao == 0)?1:0;
										atualiza_tela_menu(menu_flag,menu_opcao);
										break;
								}
								break;
							case 'C':		// Next menu item
								switch(menu_flag)
								{
									case MENU_CARTAO_SD:
										break;
									case MENU_CONFIGURA_REDE:
										break;
									case MENU_CONFIGURA_USB:
										break;
									case MENU_CONFIGURA_DATA_HORA:
										menu_opcao = (menu_opcao == 0)?1:0;
										atualiza_tela_menu(menu_flag,menu_opcao);
										break;
									case MENU_CONFIGURA_LCD:
										menu_opcao = (menu_opcao == 0)?1:0;
										atualiza_tela_menu(menu_flag,menu_opcao);
										break;
								}
								break;
							case 'D':		// Accept
								switch(menu_flag)
								{
									case MENU_CARTAO_SD:
										break;
									case MENU_CONFIGURA_REDE:
										break;
									case MENU_CONFIGURA_USB:
										break;
									case MENU_CONFIGURA_DATA_HORA:
										if(menu_opcao == 0)
											ajusta_data();
										else
											ajusta_hora();
										atualiza_tela_menu(menu_flag,menu_opcao);
										break;
									case MENU_CONFIGURA_LCD:
										if(menu_opcao == 0)
											ajusta_intensidade_LCD();
										else
											ajusta_economia_energia_LCD();
										atualiza_tela_menu(menu_flag,menu_opcao);
										break;
								}
								break;							
						}
					}
				}
				break;
		}
	}

	return 0;
}//Main

// -----------------------------------------------------------------------------
// INT5 interruption handler ---------------------------------------------------

ISR(INT5_vect)
{
	cli();				// Disable global interrupts
	DS1337_get_time();
	DS1337_get_date();
	if((tst_bit(flags,FLAG_RELOGIO)) && (menu_flag == MENU_NENHUM))
		atualiza_tela_menu(MENU_NENHUM,0);
	sei();				// Enable global interrupts
	return;
}

// -----------------------------------------------------------------------------
// Function atualiza_tela_menu -------------------------------------------------

void atualiza_tela_menu(unsigned char flag,unsigned char opcao)
{
	LCD_STDIO();
	LCD_move_primeira_linha();
	switch(flag)
	{
		case MENU_NENHUM:
			if(!tst_bit(flags,FLAG_RELOGIO))
			{
				printf(" Projeto AcelER \n");
				printf("  Modulo Base   \n");
			}
			else
			{
				LCD_move_primeira_linha();
				printf("    %02d:%02d:%02d\n",DS1337_data.hours,DS1337_data.minutes,DS1337_data.seconds);
				printf(" %02d/%02d/20%02d ",DS1337_data.month_day,DS1337_data.month,DS1337_data.year);
				imprime_dia_da_semana(DS1337_data.week_day);
			}
			break;
		case MENU_PRINCIPAL:
			switch(opcao)
			{
				case 0:					// Cartão SD
					printf("%cCartao SD\n Conf. Rede\n",0x7E);
					break;
				case 1:					// Conf. Rede
					printf("%cConf. Rede\n Conf. USB\n",0x7E);
					break;
				case 2:					// Conf. USB
					printf("%cConf. USB\n Conf. Data/Hora\n",0x7E);
					break;
				case 3:					// Conf. Data/Hora
					printf("%cConf. Data/Hora\n Conf. LCD\n",0x7E);
					break;
				case 4:					// Conf. LCD
					printf("%cConf. LCD\n Cartao SD\n",0x7E);
					break;
			}
			break;
		case MENU_CARTAO_SD://ZigBee_check_transmission_state();
			break;
		case MENU_CONFIGURA_REDE:
			break;
		case MENU_CONFIGURA_USB:
			switch(opcao)
			{
				case 0:					// Modo armazenamento
					printf("%cModo Armaz.\n Modo Transm.\n",0x7E);
					break;
				case 1:					// Modo transmissão
					printf("%cModo Transm.\n Modo Armaz.\n",0x7E);
					break;
			}
			break;
		case MENU_CONFIGURA_DATA_HORA:
			switch(opcao)
			{
				case 0:					// Ajusta data
					printf("%cAjusta Data\n Ajusta Hora\n",0x7E);
					break;
				case 1:					// Ajusta hora
					printf("%cAjusta Hora\n Ajusta Data\n",0x7E);
					break;
			}
			break;
		case MENU_CONFIGURA_LCD:
			switch(opcao)
			{
				case 0:					// Intensidade do LCD
					printf("%cIntensidade LCD\n Econ. Energia\n",0x7E);
					break;
				case 1:					// Economia de energia
					printf("%cEcon. Energia\n Intensidade LCD\n",0x7E);
					break;
			}
			break;
	}
	return;
}

ISR(USART1_RX_vect)
{
	circbuffer_write_byte((circbuffer_t *) &ZigBee_reception_buffer, USART_receive());
	return;
}

ISR(USART1_TX_vect)
{
	if(!circbuffer_empty((circbuffer_t *) &ZigBee_transmission_buffer))
	{
		USART_transmit(circbuffer_read_byte((circbuffer_t *) &ZigBee_transmission_buffer));
	}
	return;
}

ISR(TIMER0_OVF_vect)
{
	if(contador_PWM_backlight == 10)
		contador_PWM_backlight = 0;
	else
		contador_PWM_backlight++;

	if(contador_PWM_backlight >= LCD_configuration.LCD_bl_intensity)
		set_bit(LCD_BK_PORT,LCD_BK);
	else
		clr_bit(LCD_BK_PORT,LCD_BK);
	return;
}

ISR(TIMER3_OVF_vect)
{
	TIMER3_DESATIVA();
	job_check();
	TIMER3_CONFIGURA(64755);
	TIMER3_ATIVA();
	return;
}

void LCD_bl_off(void)
{
	clr_bit(flags,FLAG_LCD_BL_ON_OFF);
	set_bit(LCD_BK_PORT,LCD_BK);
	TIMER0_DESATIVA();
	return;
}

void LCD_bl_on(void)
{
	job_add_to_queue(1, LCD_bl_off, ((LCD_configuration.LCD_economy_mode + 1) * 100), 0);
	set_bit(flags,FLAG_LCD_BL_ON_OFF);
	clr_bit(LCD_BK_PORT,LCD_BK);
	TIMER0_ATIVA();
	return;
}
