//#include <ctype.h>
//#include <stdint.h>
#include <stdio.h>
//#include <util/delay.h>

#include "LS_LCD.h"


FILE stream = FDEV_SETUP_STREAM(LCD_escreve, NULL, _FDEV_SETUP_WRITE);




int main(void)
{
	
	DDRB = 0b0000000;
	DDRC = 0b0111111;
	DDRD = 0b0000000;
	
	LCD_inicia();

	stdout = stdin = stderr = &stream;

	printf("\n3 2 1 ...\n");
	while(1);



}



