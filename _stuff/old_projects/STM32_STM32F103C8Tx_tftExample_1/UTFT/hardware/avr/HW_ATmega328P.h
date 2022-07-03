// *** Hardwarespecific functions ***

void UTFT::_set_direction_registers(byte mode)
{
	DDRD = 0xFF;
	if (mode==16)
	{
		DDRB |= 0x3F;
		DDRC |= 0x03;
	}

}
