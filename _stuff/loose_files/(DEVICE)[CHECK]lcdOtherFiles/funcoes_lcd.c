// -----------------------------------------------------------------------------
// Arquivo:			funcoes_LCD.c
// Modulo:			Funções de ajuste de intensidade e economia de energia do LCD
// Autor:			Leandro Schwarz
// Versão:			1.0
// Modificado em:	20/06/2012
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Arquivos necessários --------------------------------------------------------

#include "funcoes_LCD.h"

extern volatile unsigned char flags;

// -----------------------------------------------------------------------------
// Função:		ajusta_intensidade_LCD
// Parâmetros:	nenhum
// Retorna:		1 sucesso, 0 falha
// Objetivo:	Ajusta a intensidade do backlight do LCD
// -----------------------------------------------------------------------------

unsigned char ajusta_intensidade_LCD()
{
	unsigned char estado = 1;
	unsigned char tecla;
	unsigned char intensidade = LCD_configuration.LCD_bl_intensity;

	LCD_CLEAR_SCREEN();
	while(1)
	{
		switch(estado)
		{
			case 0:			// Varre teclado
				tecla = keypad_read();
				if(tecla != 0xFF)
				{
					if(!tst_bit(flags,FLAG_LCD_BL_ON_OFF))
						LCD_bl_on();
					else
					{
						switch(tecla)
						{
							case 'A':
								LCD_cursor_desliga();
								return 0;
							case 'B':
								LCD_envia_comando(0x8C);
								intensidade = (intensidade == 10)?10:intensidade+1;
								printf("%3d",(intensidade * 10));
								LCD_envia_comando(0x8E);
								break;
							case 'C':
								LCD_envia_comando(0x8C);
								intensidade = (intensidade <= 1)?1:intensidade-1;
								printf("%3d",(intensidade * 10));
								LCD_envia_comando(0x8E);
								break;
							case 'D':
								estado = 2;
								break;
						}
					}
				}
				break;
			case 1:			// Atualiza tela
				LCD_move_primeira_linha();
				printf("Intensidade %3d%%\n",(intensidade * 10));
				LCD_cursor_piscante();
				LCD_envia_comando(0x8E);
				estado = 0;
				break;
			case 2:			// Sair
				LCD_cursor_desliga();
				LCD_move_primeira_linha();
				printf("Intensidade %3d%%\n",(intensidade * 10));
				LCD_configuration.LCD_bl_intensity = intensidade;
				eeprom_write_byte(EEPROM_LCD_BACKLIGHT_INTENSITY_ADDRESS,intensidade);
				printf("Intens. ajustada\n");
				_delay_ms(2000);
				return 1;
		}
	}
	return 0;
}

// -----------------------------------------------------------------------------
// Função:		ajusta_economia_energia_LCD
// Parâmetros:	nenhum
// Retorna:		1 sucesso, 0 falha
// Objetivo:	Ajusta o tempo de economia de energia do LCD
// -----------------------------------------------------------------------------

unsigned char ajusta_economia_energia_LCD(void)
{
	unsigned char estado = 1;
	unsigned char tecla;
	unsigned char economia = LCD_configuration.LCD_economy_mode;

	LCD_CLEAR_SCREEN();
	while(1)
	{
		switch(estado)
		{
			case 0:			// Varre teclado
				tecla = keypad_read();
				if(tecla != 0xFF)
				{
					if(!tst_bit(flags,FLAG_LCD_BL_ON_OFF))
						LCD_bl_on();
					else
					{
						switch(tecla)
						{
							case 'A':
								LCD_cursor_desliga();
								return 0;
							case 'B':
								LCD_envia_comando(0x8C);
								economia = (economia == 15)?0:economia+1;
								printf("%3d",((economia + 1) * 10));
								LCD_envia_comando(0x8E);
								break;
							case 'C':
								LCD_envia_comando(0x8C);
								economia = (economia == 0)?15:economia-1;
								printf("%3d",((economia + 1) * 10));
								LCD_envia_comando(0x8E);
								break;
							case 'D':
								estado = 2;
								break;
						}
					}
				}
				break;
			case 1:			// Atualiza tela
				LCD_move_primeira_linha();
				printf("Tempo aceso %3d\n",((economia + 1) * 10));
				LCD_cursor_piscante();
				LCD_envia_comando(0x8E);
				estado = 0;
				break;
			case 2:			// Sair
				LCD_cursor_desliga();
				LCD_move_primeira_linha();
				printf("Tempo aceso %3d\n",((economia + 1) * 10));
				LCD_configuration.LCD_economy_mode = economia;
				eeprom_write_byte(EEPROM_LCD_ECONOMY_MODE_ADDRESS,economia);
				printf("Economia conf.\n");
				LCD_bl_on();
				_delay_ms(2000);
				return 1;
		}
	}
	return 0;
}
