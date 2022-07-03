
#include <reg51.h>
#define	LCD	 P2		// lcd connected to this port

void delay()
{
	unsigned char i;
	for (i=0; i<255 ;i++);	
}

void control_word(unsigned char ch)
{
	unsigned char copy;

	copy  = ch;			//save a copy of it
	ch = ch >> 4;		//get the higher nibble
	ch = ch & 0x0f;		// -- do --
	ch = ch | 0x20;		// add E(bit 6) &  RS (bit 5)
	LCD = ch;			//out to lcd data port HIGHER nibble with E=1, RS=0

	ch = ch & 0x0f;		// make E=0, RS=0 with higher nibble
	LCD = ch;			//out to lcd data port higher nibble with E=0, RS=0

	ch = copy;			//get original byte
	ch = ch & 0x0f;		//to get lower nibble
	ch = ch | 0x20;		// E=1, RS=0
	LCD = ch;			//out to lcd data port LOWER nibble with E=1, RS=0

	ch = ch & 0x0f;		// make E=0, RS=0
	LCD = ch;			//out to lcd data port lower nibble with E=0, RS=0

	delay();			// give some delay here 
						//
}

void data_word(unsigned char ch)
{
	unsigned char copy;
	copy  = ch;			//save a copy of it
	ch = ch >> 4;		//get the higher nibble
	ch = ch & 0x0f;		// -- do --
	ch = ch | 0x30;		// add E(bit 6) &  RS (bit 5)
	LCD = ch;			//out to lcd data port HIGHER nibble with E=1, RS=1

	ch = ch & 0x1f;		// make E=0, RS=1 with higher nibble
	LCD = ch;			//out to lcd data port higher nibble with E=0, RS=1

	ch = copy;			//get original byte
	ch = ch & 0x0f;		//to get lower nibble
	ch = ch | 0x30;		// E=1, RS=1
	LCD = ch;			//out to lcd data port LOWER nibble with E=1, RS=1

	ch = ch & 0x1f;		// make E=0, RS=1
	LCD = ch;			//out to lcd data port lower nibble with E=0, RS=1

	delay();			// give some delay here 
}


void lcd_init(void)
{
	control_word(0x28);// 4bit, 2lines, 5x7
	control_word(0x28);// again
	control_word(0x0E);// disp On, cursor On, no blink
	control_word(0x01);// clear LCD
	control_word(0x06);// CUR_RIGHT
	control_word(0x80);// line 1 cur. position 0
}

void disp_str(unsigned char msg[])
{	
	unsigned char i;
	while(msg[i] != '\0')
	{
		data_word(msg[i++]);
	}	
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Bringing Cursor to (X,Y) location of LCD
		X -> 1,2
		Y -> 1,16
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void cursorxy(unsigned char x, unsigned char y)
{
	if((x<1||x>2)&&(y<1||y>16))
	{
		x=1;
		y=1;
	}
	if(x == 1)
		control_word(0x7F+y);
	else
		control_word(0xBF+y);
}

void wrt_value(unsigned char x)
{
	unsigned char temp;
	temp = x/0x10;
	if(temp>9)
		return;
	else
		data_word(temp+0x30);
	temp = x%0x10;
	if(temp>9)
		return;
	else
		data_word(temp+0x30);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Writing string to LCD
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void wrt_string(unsigned char *string)
{
	delay();
	while(*string)
		data_word(*string++);
}
//-----------------------------------------------

void wrt_long(unsigned long a)
{
	unsigned char i;
	unsigned char b,c[8]={48,48,48,48,48,48,48,48};
	for(i = 7;a > 0;i--)
	{
		b = a % 16;
		if(b <= 9)
			b = b + 48;
		else
			b = b + 55;
		a =  a / 16;
		c[i] = b;
	 }
	for(i =0; i<=7;i++)
	data_word(c[i]);
}

//-----------------------------------------------
void wrt_int(unsigned int a)
{
	unsigned char i;
	unsigned char b,c[4]={48,48,48,48};
	for(i = 3;a > 0;i--)
	{
		b = a % 16;
		if(b <= 9)
			b = b + 48;
		else
			b = b + 55;
		a =  a / 16;
		c[i] = b;
	 }
	for(i =0; i<=3;i++)
	data_word(c[i]);
}

//-----------------------------------------------
void wrt_char(unsigned char x)
{
	unsigned char temp;
	temp = x/0x10;
	if(temp>9)
		data_word(temp+55);
	else
		data_word(temp+48);
	temp = x%0x10;
	if(temp>9)
		data_word(temp+55);
	else
		data_word(temp+48);
}
//------------------------------------
void wrt_dec_char(unsigned char a)
{
	unsigned char i;
	unsigned char b,c[3]={48,48,48};
	for(i = 2;a > 0;i--)
	{
		b = a % 10;
		b = b + 48;

		a =  a / 10;
		c[i] = b;
	 }
	for(i =0; i<=2;i++)
	data_word(c[i]);
}
//-----------------------------------------------
void wrt_dec_long(unsigned long a)
{
	unsigned char i;
	unsigned char b,c[10]={48,48,48,48,48,48,48,48,48,48};
	for(i = 9;a > 0;i--)
	{
		b = a % 10;
		b = b + 48;

		a =  a / 10;
		c[i] = b;
	 }
	for(i =0; i<=9;i++)
	data_word(c[i]);
}

//-----------------------------------------------
void wrt_dec_int(unsigned int a)
{
	unsigned char i;
	unsigned char b,c[5]={48,48,48,48,48};
	for(i = 4;a > 0;i--)   
	{
		b = a % 10;
		b = b + 48;

		a =  a / 10;
		c[i] = b;
	 }
	for(i =0; i<=4;i++)
	data_word(c[i]);
}
//------------------------------------
void wrt_2dec_place(unsigned char a)
/* function to write the fractional part after decimal upto
	two decimal places
	INPUT : the decimal part as unsigned char	*/
{
	unsigned char i;
	unsigned char b,c[2]={48,48};
	for(i = 1;a > 0;i--)
	{
		b = a % 10;
		b = b + 48;

		a =  a / 10;
		c[i] = b;
	 }
	for(i =0; i<=1;i++)
	data_word(c[i]);
}
//------------------------------------
void wrt_float(float a)
/* function to write the floating point number upto
	two decimal places
	INPUT : floating point number	
	OUTPUT : xxxxx.yy   on lcd at current cursor position */
{
	unsigned int i;
	unsigned char j;

	i = (unsigned int)a;
	wrt_dec_int(i);

	wrt_string(".");
	j = (unsigned char)((a - i) * 100);

	wrt_2dec_place(j);
}
//-----------------------------------------------
