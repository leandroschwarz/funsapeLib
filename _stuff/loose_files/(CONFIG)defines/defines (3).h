// -----------------------------------------------------------------------------
// File:			defines.h
// Module:			Definifitions file for the project
// Author:			Mayara de Sousa
// Version:			1.0
// Last edition:	19/06/2012
// -----------------------------------------------------------------------------

#ifndef __DEFINES_H
#define __DEFINES_H 10

// -----------------------------------------------------------------------------
// Menu states machine definitions ---------------------------------------------

#define MENU_NENHUM						0
#define MENU_PRINCIPAL					1
#define MENU_CARTAO_SD					2
#define MENU_CONFIGURA_REDE				3
#define MENU_CONFIGURA_USB				4
#define MENU_CONFIGURA_DATA_HORA		5
#define MENU_CONFIGURA_LCD				6

// -----------------------------------------------------------------------------
// System states machine definitions -------------------------------------------

#define ESTADO_OCIOSO					0
#define ESTADO_MENU_PRINCIPAL			1
#define ESTADO_MENU_SECUNDARIO			2

// -----------------------------------------------------------------------------
// System flag definitions -----------------------------------------------------

#define FLAG_RELOGIO					0
#define FLAG_LCD_BL_ON_OFF				1

// -----------------------------------------------------------------------------
// Job IDs ---------------------------------------------------------------------

#define JOB_LCD_BL_OFF					1

// -----------------------------------------------------------------------------
// Real-time clock definitions -------------------------------------------------

#define RTC_MODO_TESTE
#define RTC_INT_DDR			DDRE
#define RTC_INT_PIN			PINE
#define RTC_INT_PORT		PORTE
#define SCL					PD0
#define SDA					PD1
#define RTC_INTA			PE4	//Interrupt RTC
#define RTC_INTB			PE5 //Interrupt RTC SQW de 1Hz

// -----------------------------------------------------------------------------
// LCD definitions -------------------------------------------------------------

#define LCD_BK_DDR		DDRE
#define LCD_BK_PORT		PORTE
#define LCD_BK			PE6

// -----------------------------------------------------------------------------
// Function declarations -------------------------------------------------------

void LCD_bl_off(void);
void LCD_bl_on(void);

#endif
