// -----------------------------------------------------------------------------
// Arquivo:			funcoes.c
// Autor:			Leandro Schwarz
// Versão:			1.0
// Modificado em:	04/12/2011
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Arquivos necessários --------------------------------------------------------

#include "funcoes.h"

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


void menu_proximo(void)
{
	if(menu == 7)
		menu = 0;
	else
		menu++;
	menu_atualiza();

	return;

}

void menu_anterior(void)
{
	if(menu == 0)
		menu = 7;
	else
		menu--;
	menu_atualiza();

	return;
}

void menu_atualiza(void)
{
	LCD_stdio();
	LCD_move_primeira_linha();
	switch(menu)
	{
		case 0:
			printf("Reportar ativ.\n");
			break;
		case 1:
			printf("Auto-teste\n");
			break;
		case 2:
			printf("Ler data/hora\n");
			break;
		case 3:
			printf("Ajusta data/hora\n");
			break;
		case 4:
			printf("Ler cor1 LCD\n");
			break;
		case 5:
			printf("Ajusta cor1 LCD\n");
			break;
		case 6:
			printf("Ler cor2 LCD\n");
			break;
		case 7:
			printf("Ajusta cor2 LCD\n");
			break;
	}

	return;
}

void menu_aceita(void)
{
	menu_flag = MENU_FLAG_ESCOLHA_DA_ESTACAO;
	estacao_atualiza();

	return;
}

void estacao_atualiza(void)
{
	LCD_stdio();
	LCD_move_primeira_linha();
	switch(menu)
	{
		case 0:
			printf("ATIV \n");
			break;
		case 1:
			printf("AUTO \n");
			break;
		case 2:
			printf("LEDH \n");
			break;
		case 3:
			printf("AJDH \n");
			break;
		case 4:
			printf("LCL1 \n");
			break;
		case 5:
			printf("ACL1 \n");
			break;
		case 6:
			printf("LCL2 \n");
			break;
		case 7:
			printf("ACL2 \n");
			break;
	}
	printf("Estacao n: %03d",estacao);

	return;
}

void estacao_aceita(void)
{
	LCD_stdio();
	LCD_move_primeira_linha();
	switch(menu)
	{
		case 0:
			printf("ATIV %03d\n",estacao);
			menu_flag = MENU_FLAG_TRANSMITIR;
			break;
		case 1:
			printf("AUTO %03d\n",estacao);
			menu_flag = MENU_FLAG_TRANSMITIR;
			break;
		case 2:
			printf("LEDH %03d\n",estacao);
			menu_flag = MENU_FLAG_TRANSMITIR;
			break;
		case 3:
			printf("AJDH %03d\n",estacao);
			menu_flag = MENU_FLAG_DADOS_EXTRAS;
			break;
		case 4:
			printf("LCL1 %03d\n",estacao);
			menu_flag = MENU_FLAG_TRANSMITIR;
			break;
		case 5:
			printf("ACL1 %03d\n",estacao);
			menu_flag = MENU_FLAG_DADOS_EXTRAS;
			break;
		case 6:
			printf("LCL2 %03d\n",estacao);
			menu_flag = MENU_FLAG_TRANSMITIR;
			break;
		case 7:
			printf("ACL2 %03d\n",estacao);
			menu_flag = MENU_FLAG_DADOS_EXTRAS;
			break;
	}

	return;
}

void configura_dados_adicionais(unsigned char tecla)
{
	unsigned char c_lixo;

	if(menu == 3)
	{
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
				break;
			case 0x0F:
				incrementa_dados_adicionais();
				break;
			case 0x0D:
				decrementa_dados_adicionais();
				break;
			case 0x0A:
				LCD_stdio();
				LCD_move_primeira_linha();
				printf("\n\n");
				menu_flag = MENU_FLAG_ESCOLHA_DO_MENU;
				menu_atualiza();
				break;
			case 0x0B:
				if(dados_adicionais == 7)
				{
					c_lixo = 0;
					if(hora > 23) c_lixo = 1;
					if(minuto > 59) c_lixo = 1;
					if(segundo > 59) c_lixo = 1;
					if(dia_semana > 7) c_lixo = 1;
					if(dia_mes > 31) c_lixo = 1;
					if(mes > 12) c_lixo = 1;
					if(ano > 99) c_lixo = 1;
					if((dia_mes == 30)&&((mes == 4)||(mes == 6)||(mes == 9)||(mes == 11))) c_lixo = 1;
					if(((ano % 4) == 0)&&(mes == 2)&&(dia_mes > 29)) c_lixo = 1;
					if(((ano % 4) != 0)&&(mes == 2)&&(dia_mes > 28)) c_lixo = 1;
					if(c_lixo)
						dados_adicionais = 0;
					else
					{
						menu_flag = MENU_FLAG_TRANSMITIR;
						break;
					}
				}
				else
					dados_adicionais++;
				break;
		}
	}
	else if(menu == 5)
	{
	}
	else if(menu == 7)
	{
	}
	return;
}

void atualiza_dados_adicionais(void)
{
	LCD_stdio();
	LCD_move_primeira_linha();

	switch(menu)
	{
		case 3:
			if(dados_adicionais == 0) printf("%c",0x7E);
			printf("%02d/",dia_mes);
			if(dados_adicionais == 1) printf("%c",0x7E);
			printf("%02d/",mes);
			if(dados_adicionais == 2) printf("%c",0x7E);
			printf("%02d ",ano);
			if(dados_adicionais == 3) printf("%c",0x7E);
			switch(dia_semana)
			{
				case 1:	printf("Dom\n");	break;
				case 2:	printf("Seg\n");	break;
				case 3:	printf("Ter\n");	break;
				case 4:	printf("Qua\n");	break;
				case 5:	printf("Qui\n");	break;
				case 6:	printf("Sex\n");	break;
				case 7:	printf("Sab\n");	break;
			}
			if(dados_adicionais == 4) printf("%c",0x7E);
			printf("%02d:",hora);
			if(dados_adicionais == 5) printf("%c",0x7E);
			printf("%02d:",minuto);
			if(dados_adicionais == 6) printf("%c",0x7E);
			printf("%02d:",segundo);
			break;
		case 5:
			break;
		case 7:
			break;
	}

	return;
}

void incrementa_dados_adicionais(void)
{
	if(menu == 3)					// Ajusta data e hora
	{
		switch(dados_adicionais)
		{
			case 0:	if(dia_mes == 31){		dia_mes = 1;	}else{	dia_mes++;		}break;
			case 1:	if(mes == 12){			mes = 1;		}else{	mes++;			}break;
			case 2:	if(ano == 99){			ano = 0;		}else{	ano++;			}break;
			case 3:	if(dia_semana == 7){	dia_semana = 1;	}else{	dia_semana++;	}break;
			case 4:	if(hora == 23){			hora = 0;		}else{	hora++;			}break;
			case 5:	if(minuto == 59){		minuto = 0;		}else{	minuto++;		}break;
			case 6:	if(segundo == 59){		segundo = 0;	}else{	segundo++;		}break;
		}
	}
	else if(menu == 5)
	{
	}
	else if(menu == 7)
	{
	}

	return;
}

void decrementa_dados_adicionais(void)
{
	if(menu == 3)					// Ajusta data e hora
	{
		switch(dados_adicionais)
		{
			case 0:	if(dia_mes == 1){		dia_mes = 31;	}else{	dia_mes--;		}break;
			case 1:	if(mes == 1){			mes = 12;		}else{	mes--;			}break;
			case 2:	if(ano == 0){			ano = 99;		}else{	ano--;			}break;
			case 3:	if(dia_semana == 1){	dia_semana = 7;	}else{	dia_semana--;	}break;
			case 4:	if(hora == 0){			hora = 23;		}else{	hora--;			}break;
			case 5:	if(minuto == 0){		minuto = 59;	}else{	minuto--;		}break;
			case 6:	if(segundo == 0){		segundo = 59;	}else{	segundo--;		}break;
		}
	}
	else if(menu == 5)
	{
	}
	else if(menu == 7)
	{
	}

	return;
}

void modifica_dados_adicionais(unsigned char tecla)
{
	if(menu == 3)					// Ajusta data e hora
	{
		switch(dados_adicionais)
		{
			case 0:
				dia_mes %= 10;
				dia_mes *= 10;
				if((dia_mes + tecla) > 31)
					dia_mes = tecla;
				else
					dia_mes += tecla;
				break;
			case 1:
				mes %= 10;
				mes *= 10;
				if((mes + tecla) > 12)
					mes = tecla;
				else
					mes += tecla;
				break;
			case 2:
				ano %= 10;
				ano *= 10;
				if((ano + tecla) > 99)
					ano = tecla;
				else
					ano += tecla;
				break;
			case 4:
				hora %= 10;
				hora *= 10;
				if((hora + tecla) > 23)
					hora = tecla;
				else
					hora += tecla;
				break;
			case 5:
				minuto %= 10;
				minuto *= 10;
				if((minuto + tecla) > 59)
					minuto = tecla;
				else
					minuto += tecla;
				break;
			case 6:
				segundo %= 10;
				segundo *= 10;
				if((segundo + tecla) > 59)
					segundo = tecla;
				else
					segundo += tecla;
				break;
		}
	}
	else if(menu == 5)
	{
	}
	else if(menu == 7)
	{
	}
	
	return;
}
