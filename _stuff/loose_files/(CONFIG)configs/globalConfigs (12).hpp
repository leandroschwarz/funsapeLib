

// -----------------------------------------------------------------------------
// Hardware connections --------------------------------------------------------

/* -----------------------------------------------------------------------------
 * Module 1 Name		Module 1 Pin		Module 2 Pin		Module 2 Name
 * -----------------------------------------------------------------------------
 * SD Card Module		(1) GND				GND					-
 * SD Card Module		(2) 3.3 V			NC					-
 * SD Card Module		(3) 5 V				5 V					-
 * SD Card Module		(4) CS				(D53) PB0			Arduino Mega 2560
 * SD Card Module		(5) MOSI			(D51) PB2			Arduino Mega 2560
 * SD Card Module		(6) SCLK			(D52) PB1			Arduino Mega 2560
 * SD Card Module		(7) MISO			(D50) PB3			Arduino Mega 2560
 * SD Card Module		(8) GND				GND					-
 * NOTES:				Just one GND is needed.
 * --------------------
 * TCS230				(1) S0				(PWM4) PG5			Arduino Mega 2560
 * TCS230				(2) S1				(PWM5) PE3			Arduino Mega 2560
 * TCS230				(3) OE				GND					-
 * TCS230				(4) GND				GND					-
 * TCS230				(5) VDD				5 V					-
 * TCS230				(6) OUT				(PWM3) PE5			Arduino Mega 2560
 * TCS230				(7) S2				(PWM6) PH3			Arduino Mega 2560
 * TCS230				(8) S3				(PWM7) PH4			Arduino Mega 2560
 * NOTES:				None.
 * --------------------
 * SN74HC595			(1) Q1				NC					-
 * SN74HC595			(2) Q2				(1) Resistor Pin	Resistor R1
 * SN74HC595			(3) Q3				(1) Resistor Pin	Resistor R2
 * SN74HC595			(4) Q4				(1) Resistor Pin	Resistor R3
 * SN74HC595			(5) Q5				(1) Resistor Pin	Resistor R4
 * SN74HC595			(6) Q6				(1) Resistor Pin	Resistor R5
 * SN74HC595			(7) Q7				(1) Resistor Pin	Resistor R6
 * SN74HC595			(8) GND				GND					-
 * SN74HC595			(9) Q7'				NC					-
 * SN74HC595			(10) SRCLR			5 V					-
 * SN74HC595			(11) SRCLK			(PWM8) PH5			Arduino Mega 2560
 * SN74HC595			(12) RCLK			(PWM9) PH6			Arduino Mega 2560
 * SN74HC595			(13) OE				GND					-
 * SN74HC595			(14) SER			(PWM10) PB4			Arduino Mega 2560
 * SN74HC595			(15) Q0				NC					-
 * SN74HC595			(16) Vcc			5 V					-
 * NOTES:				None.
 * --------------------
 * Resistor R1 (4k7)	(2) Resistor Pin	(B) Base			Transistor Q1
 * Resistor R2 (4k7)	(2) Resistor Pin	(B) Base			Transistor Q2
 * Resistor R3 (4k7)	(2) Resistor Pin	(B) Base			Transistor Q3
 * Resistor R4 (4k7)	(2) Resistor Pin	(B) Base			Transistor Q4
 * Resistor R5 (4k7)	(2) Resistor Pin	(B) Base			Transistor Q5
 * Resistor R6 (4k7)	(2) Resistor Pin	(B) Base			Transistor Q6
 * NOTES:				None.
 * --------------------
 * Trans. Q1 (BC547)	(C) Collector		LED YELLOW			LED BAR
 * Trans. Q1 (BC547)	(E) Emitter			GND					-
 * Trans. Q2 (BC547)	(C) Collector		LED WHITE			LED BAR
 * Trans. Q2 (BC547)	(E) Emitter			GND					-
 * Trans. Q3 (BC547)	(C) Collector		LED RED				LED BAR
 * Trans. Q3 (BC547)	(E) Emitter			GND					-
 * Trans. Q4 (BC547)	(C) Collector		LED INFRARED		LED BAR
 * Trans. Q4 (BC547)	(E) Emitter			GND					-
 * Trans. Q5 (BC547)	(C) Collector		LED GREEN			LED BAR
 * Trans. Q5 (BC547)	(E) Emitter			GND					-
 * Trans. Q6 (BC547)	(C) Collector		LED BLUE			LED BAR
 * Trans. Q6 (BC547)	(E) Emitter			GND					-
 * NOTES:				None.
 * --------------------
 * BUTTON SELECT		(1)	Button Pin		(PWM2) PE4			Arduino Mega 2560
 * BUTTON SELECT		(2)	Button Pin		GND					-
 * BUTTON START			(1)	Button Pin		(COM18) PD3			Arduino Mega 2560
 * BUTTON START			(2)	Button Pin		GND					-
 * BUTTON STOP			(1)	Button Pin		(COM19) PD2			Arduino Mega 2560
 * BUTTON STOP			(2)	Button Pin		GND					-
 * -------------------------------------------------------------------------- */

#ifndef __GLOBAL_CONFIGS_HPP
#define __GLOBAL_CONFIGS_HPP 1000

// EDITABLE DEFINITIONS

// System clock
#define F_CPU					16000000UL

// Maximum number of tasks
#define NUMBER_OF_TASKS			30

// Debouce time for buttons in ms
#define DEBOUNCE_TIME			20

// TCS320 editable pins
#define TCS230_FILTER_S3_DIR	DDRH
#define TCS230_FILTER_S3_OUT	PORTH
#define TCS230_FILTER_S3_BIT	PH4
#define TCS230_FILTER_S2_DIR	DDRH
#define TCS230_FILTER_S2_OUT	PORTH
#define TCS230_FILTER_S2_BIT	PH3
#define TCS230_SCALE_S1_DIR		DDRE
#define TCS230_SCALE_S1_OUT		PORTE
#define TCS230_SCALE_S1_BIT		PE3
#define TCS230_SCALE_S0_DIR		DDRG
#define TCS230_SCALE_S0_OUT		PORTG
#define TCS230_SCALE_S0_BIT		PG5

// Shift Register editable pins
#define SN74595_SER_DIR			DDRB
#define SN74595_SER_OUT			PORTB
#define SN74595_SER_BIT			PB4
#define SN74595_SRCLK_DIR		DDRH
#define SN74595_SRCLK_OUT		PORTH
#define SN74595_SRCLK_BIT		PH5
#define SN74595_RCLK_DIR		DDRH
#define SN74595_RCLK_OUT		PORTH
#define SN74595_RCLK_BIT		PH6

#endif
