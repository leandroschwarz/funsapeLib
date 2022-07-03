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
// Declaração das variáveis globais da biblioteca ------------------------------

unsigned char menu_flag;
unsigned char menu;
unsigned char estacao;
unsigned char dados_adicionais;
unsigned char hora;
unsigned char minuto;
unsigned char segundo;
unsigned char dia_mes;
unsigned char dia_semana;
unsigned char mes;
unsigned char ano;
unsigned char cor1_vm;
unsigned char cor1_vd;
unsigned char cor1_az;
unsigned char cor2_vm;
unsigned char cor2_vd;
unsigned char cor2_az;

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
	LCD_display_limpa();
	LCD_move_primeira_linha();
	switch(menu)
	{
		case 0:	printf("Reportar ativ.\n");		break;
		case 1:	printf("Auto-teste\n");			break;
		case 2:	printf("Ler data/hora\n");		break;
		case 3:	printf("Ajusta data/hora\n");	break;
		case 4:	printf("Ler cor1 LCD\n");		break;
		case 5:	printf("Ajusta cor1 LCD\n");	break;
		case 6:	printf("Ler cor2 LCD\n");		break;
		case 7:	printf("Ajusta cor2 LCD\n");	break;
	}

	return;
}

void estacao_atualiza(void)
{
	LCD_stdio();
	LCD_display_limpa();
	LCD_move_primeira_linha();
	switch(menu)
	{
		case 0:	printf("ATIV \n");	break;
		case 1:	printf("AUTO \n");	break;
		case 2:	printf("LEDH \n");	break;
		case 3:	printf("AJDH \n");	break;
		case 4:	printf("LCL1 \n");	break;
		case 5:	printf("ACL1 \n");	break;
		case 6:	printf("LCL2 \n");	break;
		case 7:	printf("ACL2 \n");	break;
	}
	printf("Estacao n: %03d",estacao);

	return;
}

void modifica_dados_adicionais(unsigned char tecla)
{
	unsigned int i_lixo;

	LCD_stdio();
	LCD_display_limpa();
	LCD_move_primeira_linha();
	if(menu == 3)		// Ajusta data e hora
	{
		switch(dados_adicionais)
		{
			case DADOS_ADICIONAIS_DIA_MES:		i_lixo = dia_mes * 10 + tecla;	if(i_lixo > 31){	dia_mes = 1;	}else{ dia_mes = (i_lixo & 0xFF);	}break;
			case DADOS_ADICIONAIS_MES:			i_lixo = mes * 10 + tecla;		if(i_lixo > 12){	mes = 1;		}else{ mes = (i_lixo & 0xFF);		}break;
			case DADOS_ADICIONAIS_ANO:			i_lixo = ano * 10 + tecla;		if(i_lixo > 99){	ano = 0;		}else{ ano = (i_lixo & 0xFF);		}break;
			case DADOS_ADICIONAIS_DIA_SEMANA:	break;
			case DADOS_ADICIONAIS_HORA:			i_lixo = hora * 10 + tecla;		if(i_lixo > 23){	hora = 0;		}else{ hora = (i_lixo & 0xFF);		}break;
			case DADOS_ADICIONAIS_MINUTO:		i_lixo = minuto * 10 + tecla;	if(i_lixo > 59){	minuto = 0;		}else{ minuto = (i_lixo & 0xFF);	}break;
			case DADOS_ADICIONAIS_SEGUNDO:		i_lixo = segundo * 10 + tecla;	if(i_lixo > 59){	segundo = 0;	}else{ segundo = (i_lixo & 0xFF);	}break;
		}
	}
	else if(menu == 5)	// Ajusta cor LCD 1
	{
		switch(dados_adicionais)
		{
			case DADOS_ADICIONAIS_VERMELHO:		i_lixo = cor1_vm * 10 + tecla;	if(i_lixo > 255){	cor1_vm = 0;	}else{ cor1_vm = (i_lixo & 0xFF);	}break;
			case DADOS_ADICIONAIS_VERDE:		i_lixo = cor1_vd * 10 + tecla;	if(i_lixo > 255){	cor1_vd = 0;	}else{ cor1_vd = (i_lixo & 0xFF);	}break;
			case DADOS_ADICIONAIS_AZUL:			i_lixo = cor1_az * 10 + tecla;	if(i_lixo > 255){	cor1_az = 0;	}else{ cor1_az = (i_lixo & 0xFF);	}break;
		}
	}
	else if(menu == 7)	// Ajusta cor LCD 2
	{
		switch(dados_adicionais)
		{
			case DADOS_ADICIONAIS_VERMELHO:		i_lixo = cor2_vm * 10 + tecla;	if(i_lixo > 255){	cor2_vm = 0;	}else{ cor2_vm = (i_lixo & 0xFF);	}break;
			case DADOS_ADICIONAIS_VERDE:		i_lixo = cor2_vd * 10 + tecla;	if(i_lixo > 255){	cor2_vd = 0;	}else{ cor2_vd = (i_lixo & 0xFF);	}break;
			case DADOS_ADICIONAIS_AZUL:			i_lixo = cor2_az * 10 + tecla;	if(i_lixo > 255){	cor2_az = 0;	}else{ cor2_az = (i_lixo & 0xFF);	}break;
		}
	}
	return;
}

void atualiza_dados_adicionais(void)
{
	LCD_stdio();
	LCD_display_limpa();
	LCD_move_primeira_linha();
	if(menu == 3)		// Ajusta data e hora
	{
		if(dados_adicionais == DADOS_ADICIONAIS_DIA_MES)	printf("%c",LCD_SETA);
		printf("%02d/",dia_mes);
		if(dados_adicionais == DADOS_ADICIONAIS_MES)		printf("%c",LCD_SETA);
		printf("%02d/",mes);
		if(dados_adicionais == DADOS_ADICIONAIS_ANO)		printf("%c",LCD_SETA);
		printf("%02d ",ano);
		if(dados_adicionais == DADOS_ADICIONAIS_DIA_SEMANA)	printf("%c",LCD_SETA);
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
		if(dados_adicionais == DADOS_ADICIONAIS_HORA)		printf("%c",LCD_SETA);
		printf("%02d:",hora);
		if(dados_adicionais == DADOS_ADICIONAIS_MINUTO)		printf("%c",LCD_SETA);
		printf("%02d:",minuto);
		if(dados_adicionais == DADOS_ADICIONAIS_SEGUNDO)	printf("%c",LCD_SETA);
		printf("%02d",segundo);
	}
	else if(menu == 5)	// Ajusta cor LCD 1
	{
		printf("ACL1 - %03d\n",estacao);
		if(dados_adicionais == DADOS_ADICIONAIS_VERMELHO)	printf("%c",LCD_SETA);
		printf("R%03d ",cor1_vm);
		if(dados_adicionais == DADOS_ADICIONAIS_VERDE)		printf("%c",LCD_SETA);
		printf("G%03d ",cor1_vd);
		if(dados_adicionais == DADOS_ADICIONAIS_AZUL)		printf("%c",LCD_SETA);
		printf("B%03d ",cor1_az);
	}
	else if(menu == 7)	// Ajusta cor LCD 2
	{
		printf("ACL2 - %03d\n",estacao);
		if(dados_adicionais == DADOS_ADICIONAIS_VERMELHO)	printf("%c",LCD_SETA);
		printf("R%03d ",cor2_vm);
		if(dados_adicionais == DADOS_ADICIONAIS_VERDE)		printf("%c",LCD_SETA);
		printf("G%03d ",cor2_vd);
		if(dados_adicionais == DADOS_ADICIONAIS_AZUL)		printf("%c",LCD_SETA);
		printf("B%03d ",cor2_az);
	}
	return;
}

void aceita_dados_adicionais(void)
{
	unsigned char dados_corretos;

	if(menu == 3)		// Ajusta data e hora
	{
		switch(dados_adicionais)
		{
			case DADOS_ADICIONAIS_DIA_MES:
			case DADOS_ADICIONAIS_MES:
			case DADOS_ADICIONAIS_ANO:
			case DADOS_ADICIONAIS_DIA_SEMANA:
			case DADOS_ADICIONAIS_HORA:
			case DADOS_ADICIONAIS_MINUTO:
				dados_adicionais++;
				atualiza_dados_adicionais();
				break;
			case DADOS_ADICIONAIS_SEGUNDO:
				dados_corretos = 1;
				if((dia_mes == 31) && ((mes == 2) || (mes == 4) || (mes == 6) || (mes == 9) || (mes == 11)))
					dados_corretos = 0;
				if((dia_mes == 30) && (mes == 2))
					dados_corretos = 0;
				if(((ano % 4) != 0) && (mes == 2) && (dia_mes == 29))
					dados_corretos = 0;
				if(dados_corretos == 1)
					menu_flag = MENU_FLAG_TRANSMITIR;
				else
				{
					LCD_stdio();
					LCD_display_limpa();
					LCD_move_primeira_linha();
					printf("Dados incorretos\n");
					printf("    AGUARDE     ");
					_delay_ms(3000);
					dados_adicionais = 0;
					atualiza_dados_adicionais();
				}
				break;
		}
	}
	else if(menu == 5)	// Ajusta cor LCD 1
	{
		switch(dados_adicionais)
		{
			case DADOS_ADICIONAIS_VERMELHO:
			case DADOS_ADICIONAIS_VERDE:
				dados_adicionais++;
				atualiza_dados_adicionais();
				break;
			case DADOS_ADICIONAIS_AZUL:
				menu_flag = MENU_FLAG_TRANSMITIR;
				break;
		}
	}
	else if(menu == 7)	// Ajusta cor LCD 2
	{
		switch(dados_adicionais)
		{
			case DADOS_ADICIONAIS_VERMELHO:
			case DADOS_ADICIONAIS_VERDE:
				dados_adicionais++;
				atualiza_dados_adicionais();
				break;
			case DADOS_ADICIONAIS_AZUL:
				menu_flag = MENU_FLAG_TRANSMITIR;
				break;
		}
	}
	return;
}

void incrementa_dados_adicionais(void)
{
	if(menu == 3)		// Ajusta data e hora
	{
		switch(dados_adicionais)
		{
			case DADOS_ADICIONAIS_DIA_MES:		if(dia_mes == 31)	{	dia_mes = 1;	}else{	dia_mes++;		}break;
			case DADOS_ADICIONAIS_MES:			if(mes == 12)		{	mes = 1;		}else{	mes++;			}break;
			case DADOS_ADICIONAIS_ANO:			if(ano == 99)		{	ano = 0;		}else{	ano++;			}break;
			case DADOS_ADICIONAIS_DIA_SEMANA:	if(dia_semana == 7)	{	dia_semana = 1;	}else{	dia_semana++;	}break;
			case DADOS_ADICIONAIS_HORA:			if(hora == 23)		{	hora = 0;		}else{	hora++;			}break;
			case DADOS_ADICIONAIS_MINUTO:		if(minuto == 59)	{	minuto = 0;		}else{	minuto++;		}break;
			case DADOS_ADICIONAIS_SEGUNDO:		if(segundo == 59)	{	segundo = 0;	}else{	segundo++;		}break;
		}
	}
	else if(menu == 5)	// Ajusta cor LCD 1
	{
		switch(dados_adicionais)
		{
			case DADOS_ADICIONAIS_VERMELHO:	if(cor1_vm == 255)	{	cor1_vm = 0;	}else{	cor1_vm++;	}break;
			case DADOS_ADICIONAIS_VERDE:	if(cor1_vd == 255)	{	cor1_vd = 0;	}else{	cor1_vd++;	}break;
			case DADOS_ADICIONAIS_AZUL:		if(cor1_az == 255)	{	cor1_az = 0;	}else{	cor1_az++;	}break;
		}
	}
	else if(menu == 7)	// Ajusta cor LCD 2
	{
		switch(dados_adicionais)
		{
			case DADOS_ADICIONAIS_VERMELHO:	if(cor2_vm == 255)	{	cor2_vm = 0;	}else{	cor2_vm++;	}break;
			case DADOS_ADICIONAIS_VERDE:	if(cor2_vd == 255)	{	cor2_vd = 0;	}else{	cor2_vd++;	}break;
			case DADOS_ADICIONAIS_AZUL:		if(cor2_az == 255)	{	cor2_az = 0;	}else{	cor2_az++;	}break;
		}
	}
	return;
}

void decrementa_dados_adicionais(void)
{
	if(menu == 3)		// Ajusta data e hora
	{
		switch(dados_adicionais)
		{
			case DADOS_ADICIONAIS_DIA_MES:		if(dia_mes == 1)	{	dia_mes = 31;	}else{	dia_mes--;		}break;
			case DADOS_ADICIONAIS_MES:			if(mes == 1)		{	mes = 12;		}else{	mes--;			}break;
			case DADOS_ADICIONAIS_ANO:			if(ano == 0)		{	ano = 99;		}else{	ano--;			}break;
			case DADOS_ADICIONAIS_DIA_SEMANA:	if(dia_semana == 1)	{	dia_semana = 7;	}else{	dia_semana--;	}break;
			case DADOS_ADICIONAIS_HORA:			if(hora == 0)		{	hora = 23;		}else{	hora--;			}break;
			case DADOS_ADICIONAIS_MINUTO:		if(minuto == 0)		{	minuto = 59;	}else{	minuto--;		}break;
			case DADOS_ADICIONAIS_SEGUNDO:		if(segundo == 0)	{	segundo = 59;	}else{	segundo--;		}break;
		}
	}
	else if(menu == 5)	// Ajusta cor LCD 1
	{
		switch(dados_adicionais)
		{
			case DADOS_ADICIONAIS_VERMELHO:	if(cor1_vm == 0)	{	cor1_vm = 255;	}else{	cor1_vm--;	}break;
			case DADOS_ADICIONAIS_VERDE:	if(cor1_vd == 0)	{	cor1_vd = 255;	}else{	cor1_vd--;	}break;
			case DADOS_ADICIONAIS_AZUL:		if(cor1_az == 0)	{	cor1_az = 255;	}else{	cor1_az--;	}break;
		}
	}
	else if(menu == 7)	// Ajusta cor LCD 2
	{
		switch(dados_adicionais)
		{
			case DADOS_ADICIONAIS_VERMELHO:	if(cor2_vm == 0)	{	cor2_vm = 255;	}else{	cor2_vm--;	}break;
			case DADOS_ADICIONAIS_VERDE:	if(cor2_vd == 0)	{	cor2_vd = 255;	}else{	cor2_vd--;	}break;
			case DADOS_ADICIONAIS_AZUL:		if(cor2_az == 0)	{	cor2_az = 255;	}else{	cor2_az--;	}break;
		}
	}
	return;
}
