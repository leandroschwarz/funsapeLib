
#define HANDLER_LCD
#include "defines.h"
#include "LS_LCD.h"

unsigned char CRC8CCITT(char * dados,unsigned int tamanho,unsigned char polinomio);
unsigned int CRC16CCITT(char * dados,unsigned int tamanho,unsigned int polinomio);

int main()
{
	char frame[264];
	unsigned int aux;
	unsigned int i = 0;

	DDRC = 0xFF;
	DDRD = 0xFF;
	PORTC = 0x00;

	LCD_inicia();
	LCD_stdio();

	frame[i++] = 0x18;	// Primeiro byte do campo inicializador
	frame[i++] = 0x02;	// Segundo byte do campo inicializador
	frame[i++] = 0x00;	// Estação de destino
	frame[i++] = 0x01;	// Estação de origem
	frame[i++] = 0x01;	// Número de bytes de dados
	frame[i++] = 0x80;	// Comando
	aux = CRC16CCITT(frame,i,0x1021);
	frame[i++] = (aux >> 8);	// CRC
	frame[i++] = (aux & 0xFF);	// CRC
	frame[i++] = 0xE7;	// Finalizador
	frame[i++] = 0xFD;	// Finalizador
	printf("%X",aux);

	while(1)
	{
	}
	return 0;
}


unsigned char CRC8CCITT(char * dados,unsigned int tamanho,unsigned char polinomio)
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

unsigned int CRC16CCITT(char * dados,unsigned int tamanho,unsigned int polinomio)
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
