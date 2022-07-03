
#define delayMs(x)						HAL_Delay(x)
#define delayUs(x)						do{uint16_t_t i = x;							\
        while(i) {								\
            for(uint8_t_t j = 50; j != 0; j--) {	\
                asm volatile("nop");			\
            }									\
            i--;								\
        }										\
    } while(0)

typedef enum lcdResult_t {
    LCD_OK = 0,
    LCD_UNSUPPORTED_FONT_SIZE,
    LCD_UNSUPPORTED_INTERFACE_MODE,
    LCD_UNSUPPORTED_DISPLAY_SIZE,
    LCD_UNSUPPORTED_PARAMETER,
    LCD_DATA_PORT_NOT_CONFIGURED,
    LCD_CONTROL_PORT_NOT_CONFIGURED,
    LCD_NOT_INITIALIZED
} lcdResult_t;

typedef enum direction_t {
    LEFT				= 0,
    RIGHT				= 1,
} direction_t;

extern FILE lcdStream;

class Lcd
{
public:
    //     ///////////////////     CONFIGURATION     ////////////////////     //
    bool setEntryMode(entryMode_t entryMode_p = LCD_RIGHT_OVERWRITE);
    bool setHandler(Handler *hdlr_p);

    //     ///////////////////     TEXT COMMANDS     ////////////////////     //
    bool sendCharacter(uint8_t character_p);
    int16_t writeCharacterStd(char character_p, FILE *stream_p);

    //     /////////////////    INTERFACE COMMANDS     //////////////////     //
    bool clearScreen(void);
    bool turnCursorBlinkOn(bool state_p);
    bool turnCursorOn(bool state_p);
    bool turnDisplayOn(bool state_p);
    bool moveCursor(direction_t dir_p);
    bool moveCursor(uint8_t line_p, uint8_t column_p);
    bool moveCursorFirstLine(void);
    bool moveCursorHome(void);
    bool moveCursorNextLine(void);
    bool shiftDisplay(direction_t dir_p);

    //     ////////////////////     CHECK STATUS     ////////////////////     //
    uint8_t getLine(void);
    uint8_t getColumn(void);

// lcdResult_t lcdStdio(volatile lcdConfiguration_t * lcd);
// lcdResult_t lcdCursorGoTo(volatile lcdConfiguration_t * lcd, uint8_t line, uint8_t column);
// lcdResult_t lcdCursorMoveNextLine(volatile lcdConfiguration_t * lcd);
// lcdResult_t lcdCursorMoveFirstLine(volatile lcdConfiguration_t * lcd);
// lcdResult_t lcdCursorMove(volatile lcdConfiguration_t * lcd, direction_t dir);
// lcdResult_t lcdDisplayShift(volatile lcdConfiguration_t * lcd, direction_t dir);
// lcdResult_t lcdWriteCommand(volatile lcdConfiguration_t * lcd, uint8_t command);
// lcdResult_t lcdWriteCharacter(volatile lcdConfiguration_t * lcd, uint8_t character);
// int16_t	lcdWriteStd(int8_t c, FILE * stream);

private:
    bool _changeDisplayControl(void);
    bool _changeEntryMode(void);
    bool _sendCommand(uint8_t command_p);
    bool _holdGpio(void);
    bool _releaseGpio(void);
    bool _sendCommandForce8Bits(uint8_t command);
    bool _sendCharacter(uint8_t character);
    bool _waitUntilReady(void);
    bool _configCursorDisplayState(void);
    bool _configCursorDisplayShift(void);

private:
    //     /////////////////    PERIPHERAL HANDLER     //////////////////     //
    Handler					*_handler;

    //     /////////////////    CONTROL AND STATUS     //////////////////     //
    bool					_cursorBlinkOn		: 1;
    uint8_t					_cursorColumn		: 6;	// 0 to 39
    uint8_t					_cursorLine			: 2;	// 0 to 3
    bool					_cursorOn			: 1;
    bool					_displayOn			: 1;
    bool					_initialized		: 1;
    error_e					_lastError;

    //     ///////////////////     CONFIGURATION     ////////////////////     //
    bool					_handlerOk			: 1;
    Handler::handlerType_e	_interfaceMode;
    uint8_t					_displayMaxColumn	: 6;	// 0 to 39
    uint8_t					_displayMaxLine		: 2;	// 0 to 3
    fontSize_b				_fontSize			: 1;	// 0 5x8, 1 5x10
    entryMode_t				_entryMode			: 2;

};	// class Lcd
