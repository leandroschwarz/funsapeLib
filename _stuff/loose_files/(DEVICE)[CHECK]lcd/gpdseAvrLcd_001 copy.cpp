
// =============================================================================
// Class public methods
// =============================================================================




int16_t Lcd::writeCharacterStd(char character, FILE *stream)
{
    uint8_t i = 0;

    if (character == '\n') {
        for (i = this->_cursorColumn; i < (this->_displayColumns + 1); i++) {
            this->_sendCharacter(' ');
        }
        this->moveCursorNextLine();
    } else {
        this->_sendCharacter(character);
    }

    return 0;
}


void Lcd::remapStdio(void)
{
    stdin = stdout = stderr = &lcdStream;
    lcdDefaultHandler = this;
    return;
}

// =============================================================================
// Class private methods
// =============================================================================









// =============================================================================
// Class protected methods
// =============================================================================

// NONE

// =============================================================================
// Static functions definitions
// =============================================================================

int lcdWriteStdWrapper(char data, FILE *stream)
{
    return lcdDefaultHandler->writeCharacterStd(data, stream);
}



uint8_t Lcd::lcdGetLine(void)
{
    return this->_cursorLine;
}
uint8_t Lcd::lcdGetColumn(void)
{
    return this->_cursorColumn;
}
