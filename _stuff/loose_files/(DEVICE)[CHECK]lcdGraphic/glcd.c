/* -----------------------------------------------------------------------------
 * Project:			GPDSE AVR8 Library
 * File:			glcd.c
 * Module:			Graphic LCD controller
 * Author:			Leandro Schwarz
 * Version:			1.0
 * Last edition:	2016-05-22
 * Purpose:			Graphic LCD controller
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------

#include "glcd.h"
#if __GLCD_H != 10
#error Error 101 - Version mismatch on header and source code files (glcd).
#endif

// -----------------------------------------------------------------------------
// Global variables ------------------------------------------------------------

// -----------------------------------------------------------------------------
// Static function declarations ------------------------------------------------

static bool_t glcdIsBusy(volatile glcdConfiguration_t *glcd);
static glcdResult_t glcdWriteCommand(volatile glcdConfiguration_t *glcd, uint8 command);
static glcdResult_t glcdWriteData(volatile glcdConfiguration_t *glcd, uint8 data);
static uint8 glcdReadData(volatile glcdConfiguration_t *glcd);
static inline void glcdDelay(void);

// -----------------------------------------------------------------------------
// Public function definitions -------------------------------------------------

/* -----------------------------------------------------------------------------
 * Function:	glcdResetConfiguration
 * Purpose:		Resets the GLCD configuration struct
 * Arguments:	glcd			Pointer to the GLCD struct
 * Returns:		GLCD_OK
 * -------------------------------------------------------------------------- */

glcdResult_t glcdResetConfiguration(volatile glcdConfiguration_t *glcd)
{
    glcd->dataDDR = NULL;
    glcd->dataPORT = NULL;
    glcd->dataPIN = NULL;
    glcd->controlDDR = NULL;
    glcd->controlPORT = NULL;
    glcd->controlWR = 0;
    glcd->controlRD = 0;
    glcd->controlCE = 0;
    glcd->controlCD = 0;
    glcd->controlRESET = 0;
    glcd->controlFS = 0;
    glcd->displaySize = GLCD_SIZE_UNDEFINED;
    glcd->fontSize = GLCD_FONT_8X8;
    glcd->dataPortConfigured = FALSE;
    glcd->controlPortConfigured = FALSE;
    glcd->initialized = FALSE;

    return GLCD_OK;
}

/* -----------------------------------------------------------------------------
 * Function:	glcdSetDataPort
 * Purpose:		Configures the GLCD I/O ports for the data bus
 * Arguments:	lcd			Pointer to the LCD struct
 *				dataDDR		Pointer to I/O Data Direction Register
 *				dataPORT	Pointer to I/O Port Output Register
 *				dataPIN		Pointer to I/O Port Input Register
 * Returns:		GLCD_OK
 * -------------------------------------------------------------------------- */

glcdResult_t glcdSetDataPort(volatile glcdConfiguration_t *glcd, volatile uint8 *dataDDR, volatile uint8 *dataPORT,
        volatile uint8 *dataPIN)
{
    glcd->dataDDR = dataDDR;
    glcd->dataPORT = dataPORT;
    glcd->dataPIN = dataPIN;
    glcd->dataPortConfigured = TRUE;

    return GLCD_OK;
}

/* -----------------------------------------------------------------------------
 * Function:	glcdSetControlPort
 * Purpose:		Configures the GLCD I/O ports for the control bus
 * Arguments:	glcd			Pointer to the GLCD struct
 *				controlDDR		Pointer to I/O Data Direction Register
 *				controlPORT		Pointer to I/O Port Output Register
 *				controlCD		Position of pin CD at the control register
 *				controlCE		Position of pin CE at the control register
 *				controlFS		Position of pin FS at the control register
 *				controlRD		Position of pin RD at the control register
 *				controlRESET	Position of pin RESET at the control register
 *				controlWR		Position of pin WR at the control register
 * Returns:		GLCD_OK
 * -------------------------------------------------------------------------- */

glcdResult_t glcdSetControlPort(volatile glcdConfiguration_t *glcd, volatile uint8 *controlDDR,
        volatile uint8 *controlPORT, uint8 controlCD, uint8 controlCE, uint8 controlFS, uint8 controlRD, uint8 controlRESET,
        uint8 controlWR)
{
    glcd->controlDDR = controlDDR;
    glcd->controlPORT = controlPORT;
    glcd->controlCD = controlCD;
    glcd->controlCE = controlCE;
    glcd->controlFS = controlFS;
    glcd->controlRD = controlRD;
    glcd->controlRESET = controlRESET;
    glcd->controlWR = controlWR;
    glcd->controlPortConfigured = TRUE;

    return GLCD_OK;
}

/* -----------------------------------------------------------------------------
 * Function:	glcdInit
 * Purpose:		Performs the LCD initialization routine
 * Arguments:	glcd			Pointer to the GLCD struct
 *				displaySize		Display size (use enumerations at glcdDisplaySize_t)
 *				fontSize		Font size (use enumerations at glcdFontSize_t)
 * Returns:		GLCD_UNSUPPORTED_FONT_SIZE
 *				GLCD_DATA_PORT_NOT_CONFIGURED
 *				GLCD_CONTROL_PORT_NOT_CONFIGURED
 *				GLCD_OK
 * -------------------------------------------------------------------------- */

glcdResult_t glcdInit(volatile glcdConfiguration_t *glcd, glcdDisplaySize_t displaySize, glcdFontSize_t fontSize)
{
    uint8 aux8 = 0;

    // GLCD font
    switch (fontSize) {
    case GLCD_FONT_6X8:
    case GLCD_FONT_8X8:
        glcd->fontSize = fontSize;
        break;
    default:
        return GLCD_UNSUPPORTED_FONT_SIZE;
    }

    // GLCD size
    switch (displaySize) {
    default:
        return GLCD_UNSUPPORTED_DISPLAY_SIZE;
        break;
    case GLCD_122X32:
    case GLCD_128X64:
    case GLCD_128X128:
    case GLCD_144X32:
    case GLCD_160X32:
    case GLCD_160X80:
    case GLCD_160X128:
    case GLCD_160X160:
    case GLCD_192X32:
    case GLCD_192X64:
    case GLCD_192X128:
    case GLCD_202X32:
    case GLCD_240X64:
    case GLCD_240X128:
    case GLCD_240X160:
    case GLCD_320X240:
        glcd->displaySize = displaySize;
        break;
    }

    // I/O initialization
    if (glcd->dataPortConfigured == FALSE) {
        return GLCD_DATA_PORT_NOT_CONFIGURED;
    }
    if (glcd->controlPortConfigured == FALSE) {
        return GLCD_CONTROL_PORT_NOT_CONFIGURED;
    }
    *(glcd->dataDDR) = 0xFF;
    aux8 =	(1 << glcd->controlCD) | (1 << glcd->controlCE) | (1 << glcd->controlFS) | (1 << glcd->controlRD) |
            (1 << glcd->controlRESET) | (1 << glcd->controlWR);
    setMaskOffset((*(glcd->controlDDR)), aux8, 0);
    setMaskOffset((*(glcd->controlPORT)), aux8, 0);
    // Reset control
    clrBit((*(glcd->controlPORT)), glcd->controlRESET);
    _delay_ms(1);
    setBit((*(glcd->controlPORT)), glcd->controlRESET);
    // Font selection
    if (glcd->fontSize == GLCD_FONT_6X8) {
        setBit((*(glcd->controlPORT)), glcd->controlFS);
    } else {
        clrBit((*(glcd->controlPORT)), glcd->controlFS);
    }

    // Commands
    glcdWriteData(glcd, GLCD_GRAPHIC_HOME & 0xFF);
    glcdWriteData(glcd, GLCD_GRAPHIC_HOME >> 8);
    glcdWriteCommand(glcd, T6963_SET_GRAPHIC_HOME_ADDRESS);
    glcdWriteData(glcd, GLCD_GRAPHIC_AREA);
    glcdWriteData(glcd, 0x00);
    glcdWriteCommand(glcd, T6963_SET_GRAPHIC_AREA);
    glcdWriteData(glcd, GLCD_TEXT_HOME);
    glcdWriteData(glcd, GLCD_TEXT_HOME >> 8);
    glcdWriteCommand(glcd, T6963_SET_TEXT_HOME_ADDRESS);
    glcdWriteData(glcd, GLCD_TEXT_AREA);
    glcdWriteData(glcd, 0x00);
    glcdWriteCommand(glcd, T6963_SET_TEXT_AREA);
    glcdWriteData(glcd, GLCD_OFFSET_REGISTER);
    glcdWriteData(glcd, 0x00);
    glcdWriteCommand(glcd, T6963_SET_OFFSET_REGISTER);
    glcdWriteCommand(glcd, T6963_DISPLAY_MODE  | T6963_GRAPHIC_DISPLAY_ON   |
            T6963_TEXT_DISPLAY_ON /*| T6963_CURSOR_DISPLAY_ON*/);
    glcdWriteCommand(glcd, T6963_MODE_SET | 0);
    glcd->initialized = TRUE;

    return GLCD_OK;
}

/* -----------------------------------------------------------------------------
 * Function:	glcdSetPixel
 * Purpose:		Draw a pixel in the GLCD
 * Arguments:	glcd		Pointer to the GLCD struct
 *				x			Pixel X position
 *				y			Pixel Y position
 *				color		Pixel color (use enumerations at glcdColor_t)
 * Returns:		GLCD_UNSUPPORTED_PARAMETER
 *				GLCD_OK
 * -------------------------------------------------------------------------- */

glcdResult_t glcdSetPixel(volatile glcdConfiguration_t *glcd, uint16 x, uint16 y, glcdColor_t color)
{
    uint8 tmp;
    uint16 address;

    address = GLCD_GRAPHIC_HOME + (x / GLCD_FONT_WIDTH) + (GLCD_GRAPHIC_AREA * y);

    GLCD_SetAddressPointer(glcd, address);

    glcdWriteCommand(glcd, T6963_DATA_READ_AND_NONVARIABLE);
    tmp = glcdReadData(glcd);

    switch (color) {
    case GLCD_COLOR_BLACK:
        tmp |= (1 << (GLCD_FONT_WIDTH - 1 - (x % GLCD_FONT_WIDTH)));
        break;
    case GLCD_COLOR_WHITE:
        tmp &= ~(1 << (GLCD_FONT_WIDTH - 1 - (x % GLCD_FONT_WIDTH)));
        break;
    default:
        return GLCD_UNSUPPORTED_PARAMETER;
    }

    GLCD_WriteDisplayData(glcd, tmp);

    return GLCD_OK;
}







void glcdClearText(volatile glcdConfiguration_t *glcd)
{
    int i;

    GLCD_SetAddressPointer(glcd, GLCD_TEXT_HOME);
    for (i = 0; i < GLCD_TEXT_SIZE; i++) {
        GLCD_WriteDisplayData(glcd, 0);
    }
    return;
}

// void GLCD_ClearCG(volatile glcdConfiguration_t * glcd)
// {
// 	unsigned int i;
//
// 	GLCD_SetAddressPointer(glcd, GLCD_EXTERNAL_CG_HOME);
//
// 	for(i = 0; i < 256 * 8; i++){
// 		GLCD_WriteDisplayData(glcd, 0);
// 	}
//
// 	return;
// }
//
// void GLCD_ClearGraphic(volatile glcdConfiguration_t * glcd)
// {
// 	int i;
//
// 	GLCD_SetAddressPointer(glcd, GLCD_GRAPHIC_HOME);
// 	for(i = 0; i < GLCD_GRAPHIC_SIZE; i++){
// 		GLCD_WriteDisplayData(glcd, 0x00);
// 	}
//
// 	return;
// }










/* -----------------------------------------------------------------------------
PRIVATE FUNCTIONS
 * -------------------------------------------------------------------------- */

/* -----------------------------------------------------------------------------
 * Function:	glcdIsBusy
 * Purpose:		Checks if the GLCD is busy
 * Arguments:	glcd		Pointer to the GLCD struct
 * Returns:		TRUE
 *				FALSE
 * -------------------------------------------------------------------------- */

static bool_t glcdIsBusy(volatile glcdConfiguration_t *glcd)
{
    uint8 aux8 = 0;

    // Data port as INPUT
    *(glcd->dataDDR) = 0x00;
    // Send READ command
    clrMaskOffset((*(glcd->controlPORT)), ((1 << glcd->controlRD) | (1 << glcd->controlCE)), 0);
    // A little delay
//	_delay_us(13);
    glcdDelay();
    // Reads data
    aux8 = *(glcd->dataPIN);
    // Data port as OUTPUT
    *(glcd->dataDDR) = 0xFF;
    // Return CONTROL lines to default
    setMaskOffset((*(glcd->controlPORT)), ((1 << glcd->controlRD) | (1 << glcd->controlCE)), 0);

    if ((aux8 & 0x03) == 0x03) {
        return FALSE;
    }
    return TRUE;
}

/* -----------------------------------------------------------------------------
 * Function:	glcdReadData
 * Purpose:		Reads data from the GLCD
 * Arguments:	glcd		Pointer to the GLCD struct
 * Returns:		data byte read from glcd
 * -------------------------------------------------------------------------- */

static uint8 glcdReadData(volatile glcdConfiguration_t *glcd)
{
    uint8 aux8;

    while (glcdIsBusy(glcd));
    *(glcd->dataDDR) = 0x00;
    clrMaskOffset((*(glcd->controlPORT)), ((1 << glcd->controlRD) | (1 << glcd->controlCE) | (1 << glcd->controlCD)), 0);
//	_delay_us(13);
    glcdDelay();
    aux8 = *(glcd->dataPIN);
    setMaskOffset((*(glcd->controlPORT)), ((1 << glcd->controlRD) | (1 << glcd->controlCE) | (1 << glcd->controlCD)), 0);
    *(glcd->dataDDR) = 0xFF;

    return aux8;
}

/* -----------------------------------------------------------------------------
 * Function:	glcdWriteCommand
 * Purpose:		Sends commands to the GLCD
 * Arguments:	glcd		Pointer to the GLCD struct
 *				command		command byte to be sent
 * Returns:		GLCD_OK
 * -------------------------------------------------------------------------- */

static glcdResult_t glcdWriteCommand(volatile glcdConfiguration_t *glcd, uint8 command)
{
    while (glcdIsBusy(glcd));
    *(glcd->dataPORT) = command;
    clrMaskOffset((*(glcd->controlPORT)), ((1 << glcd->controlWR) | (1 << glcd->controlCE)), 0);
//	_delay_us(13);
    glcdDelay();
    setMaskOffset((*(glcd->controlPORT)), ((1 << glcd->controlWR) | (1 << glcd->controlCE)), 0);

    return GLCD_OK;
}

/* -----------------------------------------------------------------------------
 * Function:	glcdWriteData
 * Purpose:		Sends data to the GLCD
 * Arguments:	glcd		Pointer to the GLCD struct
 *				data		data byte to be sent
 * Returns:		GLCD_OK
 * -------------------------------------------------------------------------- */

static glcdResult_t glcdWriteData(volatile glcdConfiguration_t *glcd, uint8 data)
{
    while (glcdIsBusy(glcd));
    *(glcd->dataPORT) = data;
    clrMaskOffset((*(glcd->controlPORT)), ((1 << glcd->controlWR) | (1 << glcd->controlCE) | (1 << glcd->controlCD)), 0);
//	_delay_us(13);
    glcdDelay();
    setMaskOffset((*(glcd->controlPORT)), ((1 << glcd->controlWR) | (1 << glcd->controlCE) | (1 << glcd->controlCD)), 0);

    return GLCD_OK;
}




























//-------------------------------------------------------------------------------------------------
//
// Sets address pointer for display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_SetAddressPointer(volatile glcdConfiguration_t *glcd, unsigned int address)
{
    glcdWriteData(glcd, address & 0xFF);
    glcdWriteData(glcd, address >> 8);
    glcdWriteCommand(glcd, T6963_SET_ADDRESS_POINTER);
}
//-------------------------------------------------------------------------------------------------
//
// Clears text area of display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_ClearText(volatile glcdConfiguration_t *glcd)
{
    int i;
    GLCD_SetAddressPointer(glcd, GLCD_TEXT_HOME);

    for (i = 0; i < GLCD_TEXT_SIZE; i++) {
        GLCD_WriteDisplayData(glcd, 0);
    }
}
//-------------------------------------------------------------------------------------------------
//
// Clears characters generator area of display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_ClearCG(volatile glcdConfiguration_t *glcd)
{
    unsigned int i;
    GLCD_SetAddressPointer(glcd, GLCD_EXTERNAL_CG_HOME);

    for (i = 0; i < 256 * 8; i++) {
        GLCD_WriteDisplayData(glcd, 0);
    }
}
//-------------------------------------------------------------------------------------------------
//
// Clears graphics area of display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_ClearGraphic(volatile glcdConfiguration_t *glcd)
{
    int i;
    GLCD_SetAddressPointer(glcd, GLCD_GRAPHIC_HOME);
    for (i = 0; i < GLCD_GRAPHIC_SIZE; i++) {
        GLCD_WriteDisplayData(glcd, 0x00);
    }
}
//-------------------------------------------------------------------------------------------------
//
// Writes a single character (ASCII code) to display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteChar(volatile glcdConfiguration_t *glcd, char charCode)
{
    GLCD_WriteDisplayData(glcd, charCode - 32);
}
//-------------------------------------------------------------------------------------------------
//
// Writes null-terminated string to display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteString(volatile glcdConfiguration_t *glcd, char *string)
{
    while (*string) {
        GLCD_WriteChar(glcd, *string++);
    }
}
//-------------------------------------------------------------------------------------------------
//
// Writes null-terminated string from program memory to display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteStringPgm(volatile glcdConfiguration_t *glcd, prog_char *string)
{
    char ch;
    while ((ch = pgm_read_byte(string++))) {
        GLCD_WriteChar(glcd, ch);
    }
}
//-------------------------------------------------------------------------------------------------
//
// Sets display coordinates
//
//-------------------------------------------------------------------------------------------------
void GLCD_TextGoTo(volatile glcdConfiguration_t *glcd, unsigned char x, unsigned char y)
{
    unsigned int address;

    address = GLCD_TEXT_HOME +  x + (GLCD_TEXT_AREA * y);

    GLCD_SetAddressPointer(glcd, address);
}
//-------------------------------------------------------------------------------------------------
//
// Writes single char pattern to character generator area of display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_DefineCharacter(volatile glcdConfiguration_t *glcd, unsigned char charCode, unsigned char *defChar)
{
    unsigned int address;
    unsigned char i;

    address = GLCD_EXTERNAL_CG_HOME + (8 * charCode);

    GLCD_SetAddressPointer(glcd, address);

    for (i = 0; i < 8 ; i++) {
        GLCD_WriteDisplayData(glcd, *(defChar + i));
    }
}


//-------------------------------------------------------------------------------------------------
//
// Writes display data and increment address pointer
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteDisplayData(volatile glcdConfiguration_t *glcd, unsigned char x)
{
    glcdWriteData(glcd, x);
    glcdWriteCommand(glcd, T6963_DATA_WRITE_AND_INCREMENT);
}
//-------------------------------------------------------------------------------------------------
//
// Sets graphics coordinates
//
//-------------------------------------------------------------------------------------------------
void GLCD_GraphicGoTo(volatile glcdConfiguration_t *glcd, unsigned char x, unsigned char y)
{
    unsigned int address;
    address = GLCD_GRAPHIC_HOME + (x / GLCD_FONT_WIDTH) + (GLCD_GRAPHIC_AREA * y);
    GLCD_SetAddressPointer(glcd, address);
}
//-------------------------------------------------------------------------------------------------
//
// Displays bitmap from program memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_Bitmap(volatile glcdConfiguration_t *glcd, unsigned char *bitmap, unsigned char x, unsigned char y,
        unsigned char width, unsigned char height)
{
    unsigned char i, j;

    for (j = 0; j < height; j++) {
        GLCD_GraphicGoTo(glcd, x, y + j);
        for (i = 0; i < width / GLCD_FONT_WIDTH; i++) {
            GLCD_WriteDisplayData(glcd, pgm_read_byte(bitmap + i + (GLCD_GRAPHIC_AREA * j)));
        }
    }
}

void GLCD_Rectangle(volatile glcdConfiguration_t *glcd, unsigned char x, unsigned char y, unsigned char b,
        unsigned char a, glcdColor_t color)
{
    unsigned char j; // zmienna pomocnicza
    // rysowanie linii pionowych (boki)
    for (j = 0; j < a; j++) {
        glcdSetPixel(glcd, x, y + j, color);
        glcdSetPixel(glcd, x + b - 1, y + j, color);
    }
    // rysowanie linii poziomych (podstawy)
    for (j = 0; j < b; j++)	{
        glcdSetPixel(glcd, x + j, y, color);
        glcdSetPixel(glcd, x + j, y + a - 1, color);
    }
}


void GLCD_Circle(volatile glcdConfiguration_t *glcd, unsigned char cx, unsigned char cy, unsigned char radius,
        glcdColor_t color)
{
    int x, y, xchange, ychange, radiusError;
    x = radius;
    y = 0;
    xchange = 1 - 2 * radius;
    ychange = 1;
    radiusError = 0;
    while (x >= y) {
        glcdSetPixel(glcd, cx + x, cy + y, color);
        glcdSetPixel(glcd, cx - x, cy + y, color);
        glcdSetPixel(glcd, cx - x, cy - y, color);
        glcdSetPixel(glcd, cx + x, cy - y, color);
        glcdSetPixel(glcd, cx + y, cy + x, color);
        glcdSetPixel(glcd, cx - y, cy + x, color);
        glcdSetPixel(glcd, cx - y, cy - x, color);
        glcdSetPixel(glcd, cx + y, cy - x, color);
        y++;
        radiusError += ychange;
        ychange += 2;
        if (2 * radiusError + xchange > 0) {
            x--;
            radiusError += xchange;
            xchange += 2;
        }
    }
}


void GLCD_Line(volatile glcdConfiguration_t *glcd, int X1, int Y1, int X2, int Y2, glcdColor_t color)
{
    int CurrentX, CurrentY, Xinc, Yinc,
        Dx, Dy, TwoDx, TwoDy,
        TwoDxAccumulatedError, TwoDyAccumulatedError;

    Dx = (X2 - X1); // obliczenie sk³adowej poziomej
    Dy = (Y2 - Y1); // obliczenie sk³adowej pionowej

    TwoDx = Dx + Dx; // podwojona sk³adowa pozioma
    TwoDy = Dy + Dy; // podwojona sk³adowa pionowa

    CurrentX = X1; // zaczynamy od X1
    CurrentY = Y1; // oraz Y1

    Xinc = 1; // ustalamy krok zwiêkszania pozycji w poziomie
    Yinc = 1; // ustalamy krok zwiêkszania pozycji w pionie

    if (Dx < 0) { // jesli sk³adowa pozioma jest ujemna
        Xinc = -1; // to bêdziemy siê "cofaæ" (krok ujemny)
        Dx = -Dx;  // zmieniamy znak sk³adowej na dodatni
        TwoDx = -TwoDx; // jak równie¿ podwojonej sk³adowej
    }

    if (Dy < 0) { // jeœli sk³adowa pionowa jest ujemna
        Yinc = -1; // to bêdziemy siê "cofaæ" (krok ujemny)
        Dy = -Dy; // zmieniamy znak sk³adowej na dodatki
        TwoDy = -TwoDy; // jak równiez podwojonej sk³adowej
    }

    glcdSetPixel(glcd, X1, Y1, color); // stawiamy pierwszy krok (zapalamy pierwszy piksel)

    if ((Dx != 0) || (Dy != 0)) { // sprawdzamy czy linia sk³ada siê z wiêcej ni¿ jednego punktu ;)
        // sprawdzamy czy sk³adowa pionowa jest mniejsza lub równa sk³adowej poziomej
        if (Dy <= Dx) { // jeœli tak, to idziemy "po iksach"
            TwoDxAccumulatedError = 0; // zerujemy zmienn¹
            do { // ruszamy w drogê
                CurrentX += Xinc; // do aktualnej pozycji dodajemy krok
                TwoDxAccumulatedError += TwoDy; // a tu dodajemy podwojon¹ sk³adow¹ pionow¹
                if (TwoDxAccumulatedError > Dx) { // jeœli TwoDxAccumulatedError jest wiêkszy od Dx
                    CurrentY += Yinc; // zwiêkszamy aktualn¹ pozycjê w pionie
                    TwoDxAccumulatedError -= TwoDx; // i odejmujemy TwoDx
                }
                glcdSetPixel(glcd, CurrentX, CurrentY, color); // stawiamy nastêpny krok (zapalamy piksel)
            } while (CurrentX != X2); // idziemy tak d³ugo, a¿ osi¹gniemy punkt docelowy
        } else { // w przeciwnym razie idziemy "po igrekach"
            TwoDyAccumulatedError = 0;
            do {
                CurrentY += Yinc;
                TwoDyAccumulatedError += TwoDx;
                if (TwoDyAccumulatedError > Dy) {
                    CurrentX += Xinc;
                    TwoDyAccumulatedError -= TwoDy;
                }
                glcdSetPixel(glcd, CurrentX, CurrentY, color);
            } while (CurrentY != Y2);
        }
    }
}



static inline void glcdDelay(void)
{
    asm("nop\nnop\nnop");

    return;
}
