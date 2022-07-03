// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------



FILE lcdStream = FDEV_SETUP_STREAM(lcdWriteStd, NULL, _FDEV_SETUP_WRITE);
lcdConfiguration_t *defaultDisplay = NULL;

/* -----------------------------------------------------------------------------
 * Function:	lcdSetEntryMode
 * Purpose:		Configures how the display will behave when a character is
 *				written
 * Arguments:	lcd			Pointer to the LCD struct
 *				dir			write direction (enumerations defined at
 *							lcdIncrementDecrement_t)
 *				mode		insert/overwrite (enumerations defined at
 *							lcdShiftOverwrite_t)
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdSetEntryMode(lcdConfiguration_t *lcd, lcdIncrementDecrement_t dir, lcdShiftOverwrite_t mode)
{
    uint8 command = LCD_ENTRY_SET;

    if (dir != LCD_INCDEC_NO_CHANGE) {
        lcd->entryIncDec = (dir & 0x01);
    }
    if (mode != LCD_DISPLAY_NO_CHANGE) {
        lcd->entryShiftDisplay = (mode & 0x01);
    }

    if (lcd->entryIncDec == LCD_INCREMENT) {
        command |= LCD_ENTRY_INCREMENT;
    } else {
        command |= LCD_ENTRY_DECREMENT;
    }
    if (lcd->entryShiftDisplay == LCD_DISPLAY_SHIFT) {
        command |= LCD_ENTRY_SHIFT;
    } else {
        command |= LCD_ENTRY_OVERWRITE;
    }
    this->_waitUntilReady();								// ONLY - FLAGGED MODE
    lcdWriteCommand(lcd, command);

    return;
}

/* -----------------------------------------------------------------------------
 * Function:	lcdStdio
 * Purpose:		Associate the LCD to the default output for printf function
 * Arguments:	lcd			Pointer to the LCD struct
 * Returns:		-
 * -------------------------------------------------------------------------- */

void lcdStdio(lcdConfiguration_t *lcd)
{
    stdin = stdout = stderr = &lcdStream;
    defaultDisplay = lcd;

    return;
}

/* -----------------------------------------------------------------------------
 * Function:	this->_waitUntilReady
 Purpose:		Checks busy flag until LCD is ready to receive commands.
 * Arguments:	lcd			Pointer to the LCD struct
 * Returns:		-
 * Note:		This function must not be used outside this file
 * -------------------------------------------------------------------------- */

void this->_waitUntilReady(* lcd)				// ONLY - FLAGGED MODE
{
    uint8 busyFlag = 0;

    *(lcd->dataPORT) = 0x00;									// ONLY - 8 BITS MODE
    *(lcd->dataDDR) = 0x00;										// ONLY - 8 BITS MODE
    clrMaskOffset(*(lcd->dataPORT), 0x0F, lcd->dataFirst);			// ONLY - 4 BITS MODE
    clrMaskOffset(*(lcd->dataDDR), 0x0F, lcd->dataFirst);				// ONLY - 4 BITS MODE
    clrBit(*(lcd->controlPORT), lcd->controlRS);	// LCD in command mode
    setBit(*(lcd->controlPORT), lcd->controlRW);	// LCD in write mode
    clrBit(*(lcd->controlPORT), lcd->controlE);		// Makes sure enable is LOW

    do {
        busyFlag = 0;
        setBit(*(lcd->controlPORT), lcd->controlE);		// Enable pulse start
        _delay_us(1);
        busyFlag = *(lcd->dataPIN);
        busyFlag &= (0x08 << lcd->dataFirst);					// ONLY - 4 BITS MODE
        busyFlag &= 0x80;										// ONLY - 8 BITS MODE
        clrBit(*(lcd->controlPORT), lcd->controlE);		// Enable pulse end
        _delay_us(1);									// Discarding lower nibble	// ONLY - 4 BITS MODE
        setBit(*(lcd->controlPORT), lcd->controlE);		// Enable pulse start		// ONLY - 4 BITS MODE
        _delay_us(1);																// ONLY - 4 BITS MODE
        clrBit(*(lcd->controlPORT), lcd->controlE);		// Enable pulse end			// ONLY - 4 BITS MODE
        _delay_us(1);
    } while (busyFlag);

    clrBit(*(lcd->controlPORT), lcd->controlRW);	// LCD in write mode
    *(lcd->dataPORT) = 0x00;									// ONLY - 8 BITS MODE
    *(lcd->dataDDR) = 0xFF;										// ONLY - 8 BITS MODE
    clrMaskOffset(*(lcd->dataPORT), 0x0F, lcd->dataFirst);			// ONLY - 4 BITS MODE
    setMaskOffset(*(lcd->dataDDR), 0x0F, lcd->dataFirst);				// ONLY - 4 BITS MODE

    return;
}

// -----------------------------------------------------------------------------
// Debug function definitions - EXCLUDE BEFORE RELEASE -------------------------

uint8 lcdGetColumn(lcdConfiguration_t *lcd)
{
    return lcd->cursorColumn;
}

uint8 lcdGetLine(lcdConfiguration_t *lcd)
{
    return lcd->cursorLine;
}
