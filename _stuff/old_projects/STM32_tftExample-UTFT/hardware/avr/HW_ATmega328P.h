// *** Hardwarespecific functions ***
void UTFT::_hw_special_init()
{
}

void UTFT::LCD_Write_Bus_8(char VL)
{
    PORTD = VL;
    pulse_low(P_WR, B_WR);
}

void UTFT::_set_direction_registers(byte mode)
{
	DDRD = 0xFF;
	if (mode==16)
	{
		DDRB |= 0x3F;
		DDRC |= 0x03;
	}

}
