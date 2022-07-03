// -----------------------------------------------------------------------------
// Arquivo:			funcoes_data_hora.c
// Modulo:			Funções de ajuste de data e hora com o RTC e LCD
// Autor:			Leandro Schwarz
// Versão:			1.0
// Modificado em:	09/06/2012
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Arquivos necessários --------------------------------------------------------

#include "funcoes_data_hora.h"

extern volatile unsigned char flags;

// -----------------------------------------------------------------------------
// Função:		ajusta_hora
// Parâmetros:	nenhum
// Retorna:		1 sucesso, 0 falha
// Objetivo:	Ajusta a hora do RTC
// -----------------------------------------------------------------------------

unsigned char ajusta_hora()
{
	unsigned char aux;
	unsigned char posicao = 0;
	unsigned char estado = 1;
	unsigned char tecla;
	unsigned char segundos = DS1337_data.seconds;
	unsigned char minutos = DS1337_data.minutes;
	unsigned char horas = DS1337_data.hours;

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
						switch(posicao)
						{
							case 0:						// Dezena da hora
								switch(tecla)
								{
									case '0':
									case '1':
									case '2':
										aux = horas % 10;
										aux += ((tecla - 0x30) * 10);
										horas = aux;
										printf("%c",tecla);
										posicao = 1;
										break;
									case 'A':
										LCD_cursor_desliga();
										return 0;
									case 'B':
										LCD_envia_comando(0x81);
										horas = (horas >= 23)?0:horas+1;
										printf("%02d",horas);
										LCD_envia_comando(0x81);
										break;
									case 'C':
										LCD_envia_comando(0x81);
										horas = (horas == 0)?23:horas-1;
										printf("%02d",horas);
										LCD_envia_comando(0x81);
										break;
									case 'D':
										if(horas > 23)
										{
											LCD_cursor_desliga();
											LCD_envia_comando(0x81);
											printf("%c%c",0xFF,0xFF);
											_delay_ms(2000);
											LCD_envia_comando(0x81);
											printf("%02d",horas);
											LCD_envia_comando(0x81);
											LCD_cursor_piscante();
											posicao = 0;
										}
										else
										{
											posicao = 2;
											estado = 1;
										}
										break;
							
								}
								break;
							case 1:						// Unidade da hora
								switch(tecla)
								{
									case '0':
									case '1':
									case '2':
									case '3':
									case '4':
									case '5':
									case '6':
									case '7':
									case '8':
									case '9':
										aux = horas / 10;
										aux *= 10;
										aux += (tecla - 0x30);
										horas = aux;
										printf("%c",tecla);
										posicao = 0;
										LCD_envia_comando(0x81);
										break;
									case 'A':
										LCD_cursor_desliga();
										return 0;
									case 'B':
										LCD_envia_comando(0x81);
										horas = (horas >= 23)?0:horas+1;
										printf("%02d",horas);
										LCD_envia_comando(0x81);
										break;
									case 'C':
										LCD_envia_comando(0x81);
										horas = (horas == 0)?23:horas-1;
										printf("%02d",horas);
										LCD_envia_comando(0x81);
										break;
									case 'D':
										if(horas > 23)
										{
											LCD_cursor_desliga();
											LCD_envia_comando(0x81);
											printf("%c%c",0xFF,0xFF);
											_delay_ms(2000);
											LCD_envia_comando(0x81);
											printf("%02d",horas);
											LCD_envia_comando(0x81);
											LCD_cursor_piscante();
											posicao = 0;
										}
										else
										{
											posicao = 2;
											estado = 1;
										}
										break;
								}
								break;
							case 2:						// Dezena do minuto
								switch(tecla)
								{
									case '0':
									case '1':
									case '2':
									case '3':
									case '4':
									case '5':
										aux = minutos % 10;
										aux += ((tecla - 0x30) * 10);
										minutos = aux;
										printf("%c",tecla);
										posicao = 3;
										break;
									case 'A':
										LCD_cursor_desliga();
										return 0;
									case 'B':
										LCD_envia_comando(0x85);
										minutos = (minutos >= 59)?0:minutos+1;
										printf("%02d",minutos);
										LCD_envia_comando(0x85);
										break;
									case 'C':
										LCD_envia_comando(0x85);
										minutos = (minutos == 0)?59:minutos-1;
										printf("%02d",minutos);
										LCD_envia_comando(0x85);
										break;
									case 'D':
										if(minutos > 59)
										{
											LCD_cursor_desliga();
											LCD_envia_comando(0x85);
											printf("%c%c",0xFF,0xFF);
											_delay_ms(2000);
											LCD_envia_comando(0x85);
											printf("%02d",minutos);
											LCD_envia_comando(0x85);
											LCD_cursor_piscante();
											posicao = 2;
										}
										else
										{
											posicao = 4;
											estado = 1;
										}
										break;
							
								}
								break;
							case 3:						// Unidade do minuto
								switch(tecla)
								{
									case '0':
									case '1':
									case '2':
									case '3':
									case '4':
									case '5':
									case '6':
									case '7':
									case '8':
									case '9':
										aux = minutos / 10;
										aux *= 10;
										aux += (tecla - 0x30);
										minutos = aux;
										printf("%c",tecla);
										posicao = 2;
										LCD_envia_comando(0x85);
										break;
									case 'A':
										LCD_cursor_desliga();
										return 0;
									case 'B':
										LCD_envia_comando(0x85);
										minutos = (minutos >= 59)?0:minutos+1;
										printf("%02d",minutos);
										LCD_envia_comando(0x85);
										break;
									case 'C':
										LCD_envia_comando(0x85);
										minutos = (minutos == 0)?59:minutos-1;
										printf("%02d",minutos);
										LCD_envia_comando(0x85);
										break;
									case 'D':
										if(horas > 59)
										{
											LCD_cursor_desliga();
											LCD_envia_comando(0x85);
											printf("%c%c",0xFF,0xFF);
											_delay_ms(2000);
											LCD_envia_comando(0x85);
											printf("%02d",minutos);
											LCD_envia_comando(0x85);
											LCD_cursor_piscante();
											posicao = 2;
										}
										else
										{
											posicao = 4;
											estado = 1;
										}
										break;
								}
								break;
							case 4:						// Dezena do segundo
								switch(tecla)
								{
									case '0':
									case '1':
									case '2':
									case '3':
									case '4':
									case '5':
										aux = segundos % 10;
										aux += ((tecla - 0x30) * 10);
										segundos = aux;
										printf("%c",tecla);
										posicao = 5;
										break;
									case 'A':
										LCD_cursor_desliga();
										return 0;
									case 'B':
										LCD_envia_comando(0x89);
										segundos = (segundos >= 59)?0:segundos+1;
										printf("%02d",segundos);
										LCD_envia_comando(0x89);
										break;
									case 'C':
										LCD_envia_comando(0x89);
										segundos = (segundos == 0)?59:segundos-1;
										printf("%02d",segundos);
										LCD_envia_comando(0x89);
										break;
									case 'D':
										if(segundos > 59)
										{
											LCD_cursor_desliga();
											LCD_envia_comando(0x89);
											printf("%c%c",0xFF,0xFF);
											_delay_ms(2000);
											LCD_envia_comando(0x89);
											printf("%02d",segundos);
											LCD_envia_comando(0x89);
											LCD_cursor_piscante();
											posicao = 4;
										}
										else
											estado = 2;
										break;
							
								}
								break;
							case 5:						// Unidade do segundo
								switch(tecla)
								{
									case '0':
									case '1':
									case '2':
									case '3':
									case '4':
									case '5':
									case '6':
									case '7':
									case '8':
									case '9':
										aux = segundos / 10;
										aux *= 10;
										aux += (tecla - 0x30);
										segundos = aux;
										printf("%c",tecla);
										posicao = 4;
										LCD_envia_comando(0x89);
										break;
									case 'A':
										LCD_cursor_desliga();
										return 0;
									case 'B':
										LCD_envia_comando(0x89);
										segundos = (segundos >= 59)?0:segundos+1;
										printf("%02d",segundos);
										LCD_envia_comando(0x89);
										break;
									case 'C':
										LCD_envia_comando(0x89);
										segundos = (segundos == 0)?59:segundos-1;
										printf("%02d",segundos);
										LCD_envia_comando(0x89);
										break;
									case 'D':
										if(segundos > 59)
										{
											LCD_cursor_desliga();
											LCD_envia_comando(0x89);
											printf("%c%c",0xFF,0xFF);
											_delay_ms(2000);
											LCD_envia_comando(0x89);
											printf("%02d",segundos);
											LCD_envia_comando(0x89);
											LCD_cursor_piscante();
											posicao = 4;
										}
										else
											estado = 2;
										break;
								}
								break;
						}
					}
				}
				break;
			case 1:			// Atualiza tela
				LCD_move_primeira_linha();
				switch(posicao)
				{
					case 0:
					case 1:
						printf("%c%02d:%c%02d:%c%02d\n",0x7E,horas,' ',minutos,' ',segundos);
						LCD_cursor_piscante();
						LCD_envia_comando(0x80 + 1);
						break;
					case 2:
					case 3:
						printf("%c%02d:%c%02d:%c%02d\n",' ',horas,0x7E,minutos,' ',segundos);
						LCD_cursor_piscante();
						LCD_envia_comando(0x80 + 5);
						break;
					case 4:
					case 5:
						printf("%c%02d:%c%02d:%c%02d\n",' ',horas,' ',minutos,0x7E,segundos);
						LCD_cursor_piscante();
						LCD_envia_comando(0x80 + 9);
						break;
				}
				estado = 0;
				break;
			case 2:			// Sair
				LCD_cursor_desliga();
				LCD_move_primeira_linha();
				printf("%c%02d:%c%02d:%c%02d OK\n",' ',horas,' ',minutos,' ',segundos);
#ifdef __LS_DS1337_H
				DS1337_set_time(horas,minutos,segundos,DS1337_24);
#endif
#ifdef __LS_DS1307_H
				DS1307_set_time(horas,minutos,segundos,DS1307_24);
#endif
				printf("Hora ajustada\n");
				_delay_ms(2000);
				return 1;
		}
	}
	return 0;
}

// -----------------------------------------------------------------------------
// Função:		ajusta_data
// Parâmetros:	nenhum
// Retorna:		1 sucesso, 0 falha
// Objetivo:	Ajusta a data do RTC
// -----------------------------------------------------------------------------

unsigned char ajusta_data(void)
{
	unsigned char aux;
	unsigned char posicao = 0;
	unsigned char estado = 1;
	unsigned char tecla;
	unsigned char dia_mes = DS1337_data.month_day;
	unsigned char dia_semana = DS1337_data.week_day;
	unsigned char mes = DS1337_data.month;
	unsigned char ano = DS1337_data.year;

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
						switch(posicao)
						{
							case 0:						// Dezena do dia do mês
								switch(tecla)
								{
									case '0':
									case '1':
									case '2':
									case '3':
										aux = dia_mes % 10;
										aux += ((tecla - 0x30) * 10);
										dia_mes = aux;
										printf("%c",tecla);
										posicao = 1;
										break;
									case 'A':
										LCD_cursor_desliga();
										return 0;
									case 'B':
										LCD_envia_comando(0x81);
										dia_mes = (dia_mes >= 31)?1:dia_mes+1;
										printf("%02d",dia_mes);
										LCD_envia_comando(0x81);
										break;
									case 'C':
										LCD_envia_comando(0x81);
										dia_mes = (dia_mes <= 1)?31:dia_mes-1;
										printf("%02d",dia_mes);
										LCD_envia_comando(0x81);
										break;
									case 'D':
										if((dia_mes > 31) || (dia_mes == 0))
										{
											LCD_cursor_desliga();
											LCD_envia_comando(0x81);
											printf("%c%c",0xFF,0xFF);
											_delay_ms(2000);
											LCD_envia_comando(0x81);
											printf("%02d",dia_mes);
											LCD_envia_comando(0x81);
											LCD_cursor_piscante();
											posicao = 0;
											estado = 1;
										}
										else
										{
											posicao = 2;
											estado = 1;
										}
										break;
							
								}
								break;
							case 1:						// Unidade do dia do mês
								switch(tecla)
								{
									case '0':
									case '1':
									case '2':
									case '3':
									case '4':
									case '5':
									case '6':
									case '7':
									case '8':
									case '9':
										aux = dia_mes / 10;
										aux *= 10;
										aux += (tecla - 0x30);
										dia_mes = aux;
										printf("%c",tecla);
										posicao = 0;
										LCD_envia_comando(0x81);
										break;
									case 'A':
										LCD_cursor_desliga();
										return 0;
									case 'B':
										LCD_envia_comando(0x81);
										dia_mes = (dia_mes >= 31)?1:dia_mes+1;
										printf("%02d",dia_mes);
										LCD_envia_comando(0x81);
										break;
									case 'C':
										LCD_envia_comando(0x81);
										dia_mes = (dia_mes <= 1)?31:dia_mes-1;
										printf("%02d",dia_mes);
										LCD_envia_comando(0x81);
										break;
									case 'D':
										if((dia_mes > 31) || (dia_mes == 0))
										{
											LCD_cursor_desliga();
											LCD_envia_comando(0x81);
											printf("%c%c",0xFF,0xFF);
											_delay_ms(2000);
											LCD_envia_comando(0x81);
											printf("%02d",dia_mes);
											LCD_envia_comando(0x81);
											LCD_cursor_piscante();
											posicao = 0;
											estado = 1;
										}
										else
										{
											posicao = 2;
											estado = 1;
										}
										break;
								}
								break;
							case 2:						// Dezena do mês
								switch(tecla)
								{
									case '0':
									case '1':
										aux = mes % 10;
										aux += ((tecla - 0x30) * 10);
										mes = aux;
										printf("%c",tecla);
										posicao = 3;
										break;
									case 'A':
										LCD_cursor_desliga();
										return 0;
									case 'B':
										LCD_envia_comando(0x85);
										mes = (mes >= 12)?1:mes+1;
										printf("%02d",mes);
										LCD_envia_comando(0x85);
										break;
									case 'C':
										LCD_envia_comando(0x85);
										mes = (mes <= 1)?12:mes-1;
										printf("%02d",mes);
										LCD_envia_comando(0x85);
										break;
									case 'D':
										if((mes > 12) || (mes == 0))
										{
											LCD_cursor_desliga();
											LCD_envia_comando(0x85);
											printf("%c%c",0xFF,0xFF);
											_delay_ms(2000);
											LCD_envia_comando(0x85);
											printf("%02d",mes);
											LCD_envia_comando(0x85);
											LCD_cursor_piscante();
											posicao = 2;
											estado = 1;
										}
										else
										{
											if((dia_mes == 31) && ((mes == 2) || (mes == 4) || (mes == 6) || (mes == 9) || (mes == 11)))
											{
												LCD_cursor_desliga();
												LCD_envia_comando(0x81);
												printf("%c%c/ %c%c",0xFF,0xFF,0xFF,0xFF);
												_delay_ms(2000);
												LCD_envia_comando(0x81);
												printf("%02d/ %02d",dia_mes,mes);
												LCD_envia_comando(0x81);
												LCD_cursor_piscante();
												posicao = 0;
												estado = 1;
											}
											else
											{
												if((dia_mes > 29) && (mes == 2))
												{
													LCD_cursor_desliga();
													LCD_envia_comando(0x81);
													printf("%c%c/ %c%c",0xFF,0xFF,0xFF,0xFF);
													_delay_ms(2000);
													LCD_envia_comando(0x81);
													printf("%02d/ %02d",dia_mes,mes);
													LCD_envia_comando(0x81);
													LCD_cursor_piscante();
													posicao = 0;
													estado = 1;
												}
												else
												{
													posicao = 4;
													estado = 1;
												}
											}
										}
										break;
								}
								break;
							case 3:						// Unidade do mês
								switch(tecla)
								{
									case '0':
									case '1':
									case '2':
									case '3':
									case '4':
									case '5':
									case '6':
									case '7':
									case '8':
									case '9':
										aux = mes / 10;
										aux *= 10;
										aux += (tecla - 0x30);
										mes = aux;
										printf("%c",tecla);
										posicao = 2;
										LCD_envia_comando(0x85);
										break;
									case 'A':
										LCD_cursor_desliga();
										return 0;
									case 'B':
										LCD_envia_comando(0x85);
										mes = (mes >= 12)?1:mes+1;
										printf("%02d",mes);
										LCD_envia_comando(0x85);
										break;
									case 'C':
										LCD_envia_comando(0x85);
										mes = (mes <= 1)?12:mes-1;
										printf("%02d",mes);
										LCD_envia_comando(0x85);
										break;
									case 'D':
										if((mes > 12) || (mes == 0))
										{
											LCD_cursor_desliga();
											LCD_envia_comando(0x85);
											printf("%c%c",0xFF,0xFF);
											_delay_ms(2000);
											LCD_envia_comando(0x85);
											printf("%02d",mes);
											LCD_envia_comando(0x85);
											LCD_cursor_piscante();
											posicao = 2;
											estado = 1;
										}
										else
										{
											if((dia_mes == 31) && ((mes == 2) || (mes == 4) || (mes == 6) || (mes == 9) || (mes == 11)))
											{
												LCD_cursor_desliga();
												LCD_envia_comando(0x81);
												printf("%c%c/ %c%c",0xFF,0xFF,0xFF,0xFF);
												_delay_ms(2000);
												LCD_envia_comando(0x81);
												printf("%02d/ %02d",dia_mes,mes);
												LCD_envia_comando(0x81);
												LCD_cursor_piscante();
												posicao = 0;
												estado = 1;
											}
											else
											{
												if((dia_mes > 29) && (mes == 2))
												{
													LCD_cursor_desliga();
													LCD_envia_comando(0x81);
													printf("%c%c/ %c%c",0xFF,0xFF,0xFF,0xFF);
													_delay_ms(2000);
													LCD_envia_comando(0x81);
													printf("%02d/ %02d",dia_mes,mes);
													LCD_envia_comando(0x81);
													LCD_cursor_piscante();
													posicao = 0;
													estado = 1;
												}
												else
												{
													posicao = 4;
													estado = 1;
												}
											}
										}
										break;
								}
								break;
							case 4:						// Dezena do ano
								switch(tecla)
								{
									case '0':
									case '1':
									case '2':
									case '3':
									case '4':
									case '5':
									case '6':
									case '7':
									case '8':
									case '9':
										aux = ano % 10;
										aux += ((tecla - 0x30) * 10);
										ano = aux;
										printf("%c",tecla);
										posicao = 5;
										break;
									case 'A':
										LCD_cursor_desliga();
										return 0;
									case 'B':
										LCD_envia_comando(0x89);
										ano = (ano >= 99)?0:ano+1;
										printf("%02d",ano);
										LCD_envia_comando(0x89);
										break;
									case 'C':
										LCD_envia_comando(0x89);
										ano = (ano == 0)?99:ano-1;
										printf("%02d",ano);
										LCD_envia_comando(0x89);
										break;
									case 'D':
										if((dia_mes == 29) && (mes == 2))
										{
											if(ano % 4 != 0)
											{
												LCD_cursor_desliga();
												LCD_envia_comando(0x81);
												printf("%c%c/ %c%c/ %c%c",0xFF,0xFF,0xFF,0xFF,0xFF,0xFF);
												_delay_ms(2000);
												LCD_envia_comando(0x81);
												printf("%02d/ %02d/ %02d",dia_mes,mes,ano);
												LCD_envia_comando(0x81);
												LCD_cursor_piscante();
												posicao = 0;
												estado = 1;
											}
											else
											{
												posicao = 6;
												estado = 1;
											}
										}
										else
										{
											posicao = 6;
											estado = 1;
										}
										break;
								}
								break;
							case 5:						// Unidade do ano
								switch(tecla)
								{
									case '0':
									case '1':
									case '2':
									case '3':
									case '4':
									case '5':
									case '6':
									case '7':
									case '8':
									case '9':
										aux = ano / 10;
										aux *= 10;
										aux += (tecla - 0x30);
										ano = aux;
										printf("%c",tecla);
										posicao = 4;
										LCD_envia_comando(0x89);
										break;
									case 'A':
										LCD_cursor_desliga();
										return 0;
									case 'B':
										LCD_envia_comando(0x89);
										ano = (ano >= 99)?0:ano+1;
										printf("%02d",ano);
										LCD_envia_comando(0x89);
										break;
									case 'C':
										LCD_envia_comando(0x89);
										ano = (ano == 0)?99:ano-1;
										printf("%02d",ano);
										LCD_envia_comando(0x89);
										break;
									case 'D':
										if((dia_mes == 29) && (mes == 2))
										{
											if(ano % 4 != 0)
											{
												LCD_cursor_desliga();
												LCD_envia_comando(0x81);
												printf("%c%c/ %c%c/ %c%c",0xFF,0xFF,0xFF,0xFF,0xFF,0xFF);
												_delay_ms(2000);
												LCD_envia_comando(0x81);
												printf("%02d/ %02d/ %02d",dia_mes,mes,ano);
												LCD_envia_comando(0x81);
												LCD_cursor_piscante();
												posicao = 0;
												estado = 1;
											}
											else
											{
												posicao = 6;
												estado = 1;
											}
										}
										else
										{
											posicao = 6;
											estado = 1;
										}
										break;
								}
								break;
							case 6:						// Dia da semana
								switch(tecla)
								{
									case 'A':
										LCD_cursor_desliga();
										return 0;
									case 'B':
										LCD_envia_comando(0x8C);
										dia_semana = (dia_semana == 6)?0:dia_semana+1;
										imprime_dia_da_semana(dia_semana);
										LCD_envia_comando(0x8C);
										break;
									case 'C':
										LCD_envia_comando(0x8C);
										dia_semana = (dia_semana == 0)?6:dia_semana-1;
										imprime_dia_da_semana(dia_semana);
										LCD_envia_comando(0x8C);
										break;
									case 'D':
										estado = 2;
										break;
								}
								break;
						}
					}
				}
				break;
			case 1:			// Atualiza tela
				LCD_move_primeira_linha();
				switch(posicao)
				{
					case 0:
					case 1:
						printf("%c%02d/ %02d/ %02d ",0x7E,dia_mes,mes,ano);
						imprime_dia_da_semana(DS1337_data.week_day);
						LCD_cursor_piscante();
						LCD_envia_comando(0x80 + 1);
						break;
					case 2:
					case 3:
						printf(" %02d/%c%02d/ %02d ",dia_mes,0x7E,mes,ano);
						imprime_dia_da_semana(DS1337_data.week_day);
						LCD_cursor_piscante();
						LCD_envia_comando(0x80 + 5);
						break;
					case 4:
					case 5:
						printf(" %02d/ %02d/%c%02d ",dia_mes,mes,0x7E,ano);
						imprime_dia_da_semana(DS1337_data.week_day);
						LCD_cursor_piscante();
						LCD_envia_comando(0x80 + 9);
						break;
					case 6:
						printf(" %02d/ %02d/ %02d%c",dia_mes,mes,ano,0x7E);
						imprime_dia_da_semana(DS1337_data.week_day);
						LCD_cursor_piscante();
						LCD_envia_comando(0x80 + 12);
						break;
				}
				estado = 0;
				break;
			case 2:			// Sair
				LCD_cursor_desliga();
				LCD_move_primeira_linha();
				printf(" %02d/ %02d/ %02d ",dia_mes,mes,ano);
				imprime_dia_da_semana(DS1337_data.week_day);
				printf("\n");
#ifdef __LS_DS1337_H
				DS1337_set_date(ano,mes,dia_mes,dia_semana);
#endif
#ifdef __LS_DS1307_H
				DS1307_set_date(ano,mes,dia_mes,dia_semana);
#endif
				printf("Data ajustada\n");
				_delay_ms(2000);
				return 1;
		}
	}
	return 0;
}

// -----------------------------------------------------------------------------
// Função:		imprime_dia_da_semana
// Parâmetros:	dia_da_semana
// Retorna:		nada
// Objetivo:	Imprime o dia da semana no LCD
// -----------------------------------------------------------------------------

void imprime_dia_da_semana(unsigned char dia_semana)
{
//	char * aux = "    ";

	switch(dia_semana)
	{
//		case 0: aux[0] = 'd';aux[1] = 'o';aux[2] = 'm';aux[3] = '\0';break;
//		case 1: aux[0] = 's';aux[1] = 'e';aux[2] = 'g';aux[3] = '\0';break;
//		case 2: aux[0] = 't';aux[1] = 'e';aux[2] = 'r';aux[3] = '\0';break;
//		case 3: aux[0] = 'q';aux[1] = 'u';aux[2] = 'a';aux[3] = '\0';break;
//		case 4: aux[0] = 'q';aux[1] = 'u';aux[2] = 'i';aux[3] = '\0';break;
//		case 5: aux[0] = 's';aux[1] = 'e';aux[2] = 'x';aux[3] = '\0';break;
//		case 6: aux[0] = 's';aux[1] = 'a';aux[2] = 'b';aux[3] = '\0';break;
		case 0: printf("DOM");break;
		case 1: printf("SEG");break;
		case 2: printf("TER");break;
		case 3: printf("QUA");break;
		case 4: printf("QUI");break;
		case 5: printf("SEX");break;
		case 6: printf("SAB");break;
	}

	return;
}
