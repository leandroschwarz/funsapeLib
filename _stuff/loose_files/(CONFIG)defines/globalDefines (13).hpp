


// Include system configuration file
#include "globalConfigs.hpp"
#ifndef __GLOBAL_CONFIGS_HPP
	#error	[globalDefines.hpp] System configuration file (globalConfigs.hpp) is missing
#endif

// Include Guard
#ifndef __GLOBAL_DEFINES_HPP
#define __GLOBAL_DEFINES_HPP 1000

// -----------------------------------------------------------------------------
// Header files ----------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

#define __DEBUG_MODE					0
#define TWI_GENERAL_CALL_ADDRESS		0x00
#define TWI_BUFFER_SIZE					10

// -----------------------------------------------------------------------------
// Macro-functions definitions -------------------------------------------------

// Bit handling macro-functions
#define setBit(reg, bit)							((reg) |= (1 << (bit)))
#define clrBit(reg, bit)							((reg) &= (~(1 << (bit))))
#define cplBit(reg, bit)							((reg) ^= (1 << (bit)))
#define isBitSet(reg, bit)							(bool_t)(((reg) >> (bit)) & 0x01)
#define isBitClr(reg, bit)							(bool_t)(!(((reg) >> (bit)) & 1))

// Bit loop macro-functions
#define waitUntilBitIsSet(reg, bit)					do {} while(isBitClr((reg), (bit)))
#define waitUntilBitIsClear(reg, bit)				do {} while(isBitSet((reg), (bit)))

// Bit mask handling macro-functions
#define setMask(reg, mask, offset)					((reg) |= ((mask) << (offset)))
#define clrMask(reg, mask, offset)					((reg) &= ~((mask) << (offset)))
#define cplMask(reg, mask, offset)					((reg) ^= ((mask) << (offset)))

// Bit data manipulation
#define byteLowNibble(dataByte)			(dataByte & 0x0F)
#define byteHighNibble(dataByte)		((dataByte >> 4) & 0x0F)
#define byteSwitchNibbles(dataByte)		(byteLowNibble(dataByte) << 4) | byteHighNibble(dataByte)
#define wordLowByte(dataWord)			(dataWord & 0x00FF)
#define wordHighByte(dataWord)			((dataWord >> 8) & 0x00FF)
#define wordSwitchBytes(dataWord)		(wordLowByte(dataWord) << 8) | wordHighByte(dataWord)

// System actions
#define systemHalt()					while(1) {}
#define die(errorCode)					do {printf("Failed with error=%u.\r", errorCode);systemHalt();} while(0)

// Value comparison macro-functions
#define minimumOf(var1 , var2)						((var1 < var2) ? var1 :var2)
#define maximumOf(var1 , var2)						((var1 > var2) ? var1 :var2)
#define truncateBetween(var, minimum, maximum)		(var < minimum) ? minimum : ((var > maximum) ? maximum : var))

// Stringification of macro arguments
#define stringifyName(constant)						#constant
#define stringifyValue(constant)					stringifyName(constant)

// -----------------------------------------------------------------------------
// Undefine previous definitions -----------------------------------------------

#ifndef __DOXYGEN__
	// Sleep modes
	#undef SLEEP_MODE_NO_CHANGE
	#undef SLEEP_MODE_IDLE
	#undef SLEEP_MODE_ADC
	#undef SLEEP_MODE_PWR_DOWN
	#undef SLEEP_MODE_PWR_SAVE
	#undef SLEEP_MODE_STANDBY
	#undef SLEEP_MODE_EXT_STANDBY
	// PORT A
	#undef PA0
	#undef PA1
	#undef PA2
	#undef PA3
	#undef PA4
	#undef PA5
	#undef PA6
	#undef PA7
	// PORT B
	#undef PB0
	#undef PB1
	#undef PB2
	#undef PB3
	#undef PB4
	#undef PB5
	#undef PB6
	#undef PB7
	// PORT C
	#undef PC0
	#undef PC1
	#undef PC2
	#undef PC3
	#undef PC4
	#undef PC5
	#undef PC6
	#undef PC7
	// PORT D
	#undef PD0
	#undef PD1
	#undef PD2
	#undef PD3
	#undef PD4
	#undef PD5
	#undef PD6
	#undef PD7
	// PORT E
	#undef PE0
	#undef PE1
	#undef PE2
	#undef PE3
	#undef PE4
	#undef PE5
	#undef PE6
	#undef PE7
	// PORT F
	#undef PF0
	#undef PF1
	#undef PF2
	#undef PF3
	#undef PF4
	#undef PF5
	#undef PF6
	#undef PF7
	// PORT G
	#undef PG0
	#undef PG1
	#undef PG2
	#undef PG3
	#undef PG4
	#undef PG5
	#undef PG6
	#undef PG7
	// PORT H
	#undef PH0
	#undef PH1
	#undef PH2
	#undef PH3
	#undef PH4
	#undef PH5
	#undef PH6
	#undef PH7
	// PORT J
	#undef PJ0
	#undef PJ1
	#undef PJ2
	#undef PJ3
	#undef PJ4
	#undef PJ5
	#undef PJ6
	#undef PJ7
	// PORT K
	#undef PK0
	#undef PK1
	#undef PK2
	#undef PK3
	#undef PK4
	#undef PK5
	#undef PK6
	#undef PK7
	// PORT L
	#undef PL0
	#undef PL1
	#undef PL2
	#undef PL3
	#undef PL4
	#undef PL5
	#undef PL6
	#undef PL7
#endif

// -----------------------------------------------------------------------------
// New data types (general) ----------------------------------------------------

// New Data Type - Single variables => shortening the name of single data types
typedef volatile uint8_t	reg_t;
typedef volatile int8_t		vint8_t;
typedef volatile int16_t	vint16_t;
typedef volatile int32_t	vint32_t;
typedef volatile int64_t	vint64_t;
typedef volatile uint8_t	vuint8_t;
typedef volatile uint16_t	vuint16_t;
typedef volatile uint32_t	vuint32_t;
typedef volatile uint64_t	vuint64_t;

// New Data Type - Single variables => enumerations undestood as logic values

typedef enum bool_t {
	FALSE	= 0,
	TRUE	= 1
} bool_t;

typedef enum logicLevel_t {
	LOW		= 0,
	HIGH	= 1,
} logicLevel_t;

typedef enum activation_t {
	OFF		= 0,
	ON		= 1
} activation_t;

typedef enum dataFlow_t {
	INPUT	= 0,
	OUTPUT	= 1
} dataFlow_t;

typedef enum rotacionalDirection_t {
	CLOCKWISE			= 0,
	COUNTERCLOCKWISE	= 1
} rotacionalDirection_t;

typedef enum edge_t {
	FALLING_EDGE	= 0,
	RISING_EDGE		= 1
} edge_t;

typedef enum displayType_t {
	DISPLAY_COMMON_ANODE	= 0,
	DISPLAY_COMMON_CATHODE	= 1
} displayType_t;

// New Data Type - Single variables => small enumerations that can be understood as a single variable
typedef enum planarDirection_t {
	LEFT	= 0,
	RIGHT	= 1,
	UP		= 2,
	DOWN	= 3
} planarDirection_t;

typedef enum weekDay_t {
	WEEKDAY_UNDEFINED	= 0,
	WEEKDAY_SUNDAY		= 1,
	WEEKDAY_MONDAY		= 2,
	WEEKDAY_TUESDAY		= 3,
	WEEKDAY_WEDNESDAY	= 4,
	WEEKDAY_THURSDAY	= 5,
	WEEKDAY_FRIDAY		= 6,
	WEEKDAY_SATURDAY	= 7
} weekDay_t;

typedef enum month_t {
	MONTH_UNDEFINED	= 0,
	MONTH_JANUARY	= 1,
	MONTH_FEBRUARY	= 2,
	MONTH_MARCH		= 3,
	MONTH_APRIL		= 4,
	MONTH_MAY		= 5,
	MONTH_JUNE		= 6,
	MONTH_JULY		= 7,
	MONTH_AUGUST	= 8,
	MONTH_SEPTEMBER	= 9,
	MONTH_OCTOBER	= 10,
	MONTH_NOVEMBER	= 11,
	MONTH_DECEMBER	= 12
} month_t;

typedef enum twiOperation_t {
	TWI_WRITE			= 0,
	TWI_READ			= 1,
	TWI_SET_THEN_READ	= 2
} twiOperation_t;

// New Data Type - Enumerations => large enumerations or enumerations with expansible number of elements
typedef enum twiState_e {
	TWI_START					= 0x08,	// START has been transmitted
	TWI_REP_START				= 0x10,	// Repeated START has been transmitted
	TWI_ARB_LOST				= 0x38,	// Arbitration lost
	TWI_MTX_ADR_ACK				= 0x18,	// SLA+W has been transmitted and ACK received
	TWI_MTX_ADR_NACK			= 0x20,	// SLA+W has been transmitted and NACK received
	TWI_MTX_DATA_ACK			= 0x28,	// Data byte has been transmitted and ACK received
	TWI_MTX_DATA_NACK			= 0x30,	// Data byte has been transmitted and NACK received
	TWI_MRX_ADR_ACK				= 0x40,	// SLA+R has been transmitted and ACK received
	TWI_MRX_ADR_NACK			= 0x48,	// SLA+R has been transmitted and NACK received
	TWI_MRX_DATA_ACK			= 0x50,	// Data byte has been received and ACK transmitted
	TWI_MRX_DATA_NACK			= 0x58,	// Data byte has been received and NACK transmitted
	TWI_STX_ADR_ACK				= 0xA8,	// Own SLA+R has been received; ACK has been returned
	TWI_STX_ADR_ACK_M_ARB_LOST	= 0xB0,	// Arbitration lost in SLA+R/W as Master; own SLA+R has been received; ACK has been returned
	TWI_STX_DATA_ACK			= 0xB8,	// Data byte in TWDR has been transmitted; ACK has been received
	TWI_STX_DATA_NACK			= 0xC0,	// Data byte in TWDR has been transmitted; NOT ACK has been received
	TWI_STX_DATA_ACK_LAST_BYTE	= 0xC8,	// Last data byte in TWDR has been transmitted (TWEA = 0); ACK has been received
	TWI_SRX_ADR_ACK				= 0x60,	// Own SLA+W has been received ACK has been returned
	TWI_SRX_ADR_ACK_M_ARB_LOST	= 0x68,	// Arbitration lost in SLA+R/W as Master; own SLA+W has been received; ACK has been returned
	TWI_SRX_GEN_ACK				= 0x70,	// General call address has been received; ACK has been returned
	TWI_SRX_GEN_ACK_M_ARB_LOST	= 0x78,	// Arbitration lost in SLA+R/W as Master; General call address has been received; ACK has been returned
	TWI_SRX_ADR_DATA_ACK		= 0x80,	// Previously addressed with own SLA+W; data has been received; ACK has been returned
	TWI_SRX_ADR_DATA_NACK		= 0x88,	// Previously addressed with own SLA+W; data has been received; NOT ACK has been returned
	TWI_SRX_GEN_DATA_ACK		= 0x90,	// Previously addressed with general call; data has been received; ACK has been returned
	TWI_SRX_GEN_DATA_NACK		= 0x98,	// Previously addressed with general call; data has been received; NOT ACK has been returned
	TWI_SRX_STOP_RESTART		= 0xA0,	// A STOP condition or repeated START condition has been received while still addressed as Slave
	TWI_NO_STATE				= 0xF8,	// No relevant state information available; TWINT = 0
	TWI_BUS_ERROR				= 0x00	// Bus error due to an illegal START or STOP condition
} twiState_e;

typedef enum error_e {
	// System	0-29
	ERROR_NONE														= 0,	// Success - No errors occurred
	ERROR_UNSUPPORTED_CONFIGURATION_SYSTEM_CLOCK_PRESCALER			= 1,	// System clock prescaler configuration unsupported
	ERROR_SYSTEM_CLOCK_PRESCALER_COULD_NOT_BE_CHANGED				= 2,	// System clock prescaler change failed
	// INT0		30-39
	ERROR_INT0_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE			= 30,	// Int0 output port mode configuration unsupported
	ERROR_INT0_UNSUPPORTED_CONFIGURATION_SENSE_MODE					= 31,	// Int0 sense mode configuration unsupported
	// INT1		40-49
	ERROR_INT1_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE			= 40,	// Int1 output port mode configuration unsupported
	ERROR_INT1_UNSUPPORTED_CONFIGURATION_SENSE_MODE					= 41,	// Int1 sense mode configuration unsupported
	// PCINT0	50-59
	ERROR_PCINT0_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE			= 40,	// Pcint0 output port mode configuration unsupported
	// PCINT1	60-69
	ERROR_PCINT1_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE			= 40,	// Pcint1 output port mode configuration unsupported
	// PCINT2	70-79
	ERROR_PCINT2_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE			= 40,	// Pcint2 output port mode configuration unsupported
	// TIMER0	80-89
	ERROR_TIMER0_UNSUPPORTED_CONFIGURATION_CLOCK_PRESCALER			= 80,	// Timer0 clock prescaler value off limits
	ERROR_TIMER0_UNSUPPORTED_CONFIGURATION_MODE						= 81,	// Timer0 mode unsupported
	ERROR_TIMER0_UNSUPPORTED_CONFIGURATION_OUTPUT_COMPARE			= 82,	// Timer0 output pin configuration unsupported
	// TIMER1	90-99
	ERROR_TIMER1_UNSUPPORTED_CONFIGURATION_CLOCK_PRESCALER			= 90,	// Timer1 clock prescaler value off limits
	ERROR_TIMER1_UNSUPPORTED_CONFIGURATION_MODE						= 91,	// Timer1 mode unsupported
	ERROR_TIMER1_UNSUPPORTED_CONFIGURATION_OUTPUT_COMPARE			= 92,	// Timer1 output pin configuration unsupported
	ERROR_TIMER1_UNSUPPORTED_CONFIGURATION_CAPTURE_EDGE				= 93,	// Timer1 invalid edge section on capture mode
	ERROR_TIMER1_UNSUPPORTED_CONFIGURATION_CAPTURE_NOISE_CANCELER	= 94,	// Timer0 invalid noise canceler configuration
	// TIMER2	100-109
	ERROR_TIMER2_UNSUPPORTED_CONFIGURATION_CLOCK_PRESCALER			= 100,	// Timer2 clock prescaler value off limits
	ERROR_TIMER2_UNSUPPORTED_CONFIGURATION_MODE						= 101,	// Timer2 mode unsupported
	ERROR_TIMER2_UNSUPPORTED_CONFIGURATION_OUTPUT_COMPARE			= 102,	// Timer2 output pin configuration unsupported
	// ADC		110-119
	ERROR_ADC_UNSUPPORTED_CONFIGURATION_REFERENCE					= 110,	// ADC reference selection unsupported
	ERROR_ADC_UNSUPPORTED_CONFIGURATION_CLOCK_PRESCALER				= 111,	// ADC clock prescaler value off limits
	ERROR_ADC_UNSUPPORTED_CONFIGURATION_MODE						= 112,	// ADC mode unsupported
	ERROR_ADC_UNSUPPORTED_CONFIGURATION_CHANNEL						= 113,	// ADC invalid channel selection
	ERROR_ADC_UNSUPPORTED_CONFIGURATION_DATA_PRESENTATION			= 114,	// ADC invalid data presentation order
	// USART	120-129
	ERROR_USART_UNSUPPORTED_CONFIGURATION_STOP_BIT					= 120,	// USART invalid number of stop bits
	ERROR_USART_UNSUPPORTED_CONFIGURATION_PARITY					= 121,	// USART invalid parity selection
	ERROR_USART_UNSUPPORTED_CONFIGURATION_DATA_BITS					= 122,	// USART invalid number of data bits
	ERROR_USART_UNSUPPORTED_CONFIGURATION_MODE						= 123,	// USART unsupported mode
	ERROR_USART_UNSUPPORTED_CONFIGURATION_BAUD_RATE					= 124,	// USART baud rate value off limits
	ERROR_USART_UNSUPPORTED_CONFIGURATION_FRAME_FORMAT				= 125,	// USART unsupported frame format
	ERROR_USART_CONFIGURATION_PARAMETER_WRONG						= 126,	// USART some configuration parameter is wrong
} error_e;

typedef enum utcTimeZone_e {
	UTC_UNDEFINED		= 0,	// UNDEFINED, used to test if the variables was previously seted.
	UTC_MINUS_1200		= 1,	// USA (Baker Island, Howland Island)
	UTC_MINUS_1100		= 2,	// New Zealand (Niu), USA (Jarvis Island, Kingman Reef, Midway Atoll, Palmyra Atoll)
	UTC_MINUS_1000		= 3,	// France (French Poynesia), New Zealand (Cook Islands), USA (Hawaii, Johnston Atoll)
	UTC_MINUS_0930		= 4,	// France (Marquesas Islands)
	UTC_MINUS_0900		= 5,	// France (Gambier Islands), USA (Alaska)
	UTC_MINUS_0800		= 6,	// Canada (British Columbia, Yukon), France (Clipperton Island), USA (Pacific Time Zone)
	UTC_MINUS_0700		= 7,	// Canada (Alberta, Nortwest Territories), Mexico (Chinhuahua, Nayarit, Sinaloa, Sonora), USA (Mountain Time Zone)
	UTC_MINUS_0600		= 8,	// Belize, Canada (Central Time Zone), Chile (Easter Island), Costa Rica, El Salvador, Guatemala, Honduras, Mexico, USA (Central Time Zone)
	UTC_MINUS_0500		= 9,	// Bahamas, Brazil (Acre, Amazonas - western part), Colombia, Cuba, Ecuador, Haiti, Jamaica, Panama, Peru, USA (Esatern Time Zone)
	UTC_MINUS_0400		= 10,	// Antigua e Barbuda, Barbados, Bolivia, Brazil (Amazonas, Mato Grosso, Mato Grosso do Sul, Rondonia, Roraima), Canada (Atlantic Time Zone), Chile, Dominica, Guyana, Paraguay, Venezuela
	UTC_MINUS_0330		= 11,	// Canada (Newfoundland)
	UTC_MINUS_0300		= 12,	// Argentina, Brasil (Brazilia Official Timezone), Suriname, Uruguay
	UTC_MINUS_0230		= 13,	// St. John's
	UTC_MINUS_0200		= 14,	// Brazil (Fernando de Noronha), United Kingdom (South Georgia, South Sandwich Islands)
	UTC_MINUS_0100		= 15,	// Cape Verde, Portugal (Azores)
	UTC_EQUAL_0000		= 16,	// Burkina Faso, Cote d'Ivoire, Gambia, Ghana, Guinea, Guinea-Bissau, Iceland, Ireland, Liberia, Mali, Mauritania, Marocco, Portugal, Senegal, Sierra Leone, Togo, United Kingdom, Western Sahara
	UTC_PLUS_0100		= 17,	// Albania, Algeria, Andorra, Angola, Austria, Belgium, Benin, Bosnia and Herzegovina, Cameroon, Central African Republic, Chad, Congo, Croatia, Czech Republic, Denmark, France, Gabon, Gemany, Hungary, Italy, Kosovo, Liechtenstein, Luxembourg, Macedonia, Malta, Monaco, Montenegro, Netherlands, Niger, Nigeria, Poland, San Marino, Serbia,  Slovakia, Slovenia, Spain, Sweeden, Switzerland, Tunisia, Vatican City
	UTC_PLUS_0200		= 18,	// Botswana, Bulgaria, Burundi, Cyprus, Egypt, Estonia, Finland, Greece, Israel, Jordan, Latvia, Lebanon, Lesotho, Lithuania, Libya, Malawi, Moldova, Mozambique, Namibia, State of Palestine, Romania, Rwanda, South Africa, Sudan, Swaziland, Syria, Ukraine, Zambia, Zimbabwe
	UTC_PLUS_0300		= 19,	// Bahrain, Belarus, Comoros, Djibouti, Eritrea, Iraq, Kenya, Kuwait, Madagascar, Qatar, Russia (Moscow Time), Saudi Arabia, Somalia, South Sudan, Tanzania, Turkey, Uganda, Yemen
	UTC_PLUS_0330		= 20,	// Iran
	UTC_PLUS_0400		= 21,	// Armenia, Azerbaijan, Mauritius, Oman, Russia (Samara Time), Seychelles, United Arab Emirates
	UTC_PLUS_0430		= 22,	// Afghanistan
	UTC_PLUS_0500		= 23,	// Maldives, Pakistan, Russia (Yekaterinburg Time), Tajikistan, Turkmenistan, Uzbekistan
	UTC_PLUS_0530		= 24,	// India, Sri Lanka
	UTC_PLUS_0545		= 25,	// Nepal
	UTC_PLUS_0600		= 26,	// Bangladesh, Bhutan, Kazakhstan, Kyrgyzstan, Russia (Omsk Time), United Kingdom (British Indian Ocean Territory)
	UTC_PLUS_0630		= 27,	// Myanmar
	UTC_PLUS_0700		= 28,	// Cambodia, Laos, Mongolia, Russia (Krasnoyarsk Time), Thailand, Vietnam
	UTC_PLUS_0800		= 29,	// Brunei, China, Malaysia, Philippines, Russia (Irkutsk Time), Singapore, Taiwan
	UTC_PLUS_0830		= 30,	// North Korea
	UTC_PLUS_0845		= 31,	// Australia (Eucla)
	UTC_PLUS_0900		= 32,	// Japan, South Korea, Palau, Russia (Amur, Siberia), Timor-Leste
	UTC_PLUS_0930		= 33,	// Australia (Broken Hill, New South Wales, Northern Territory, South Australia)
	UTC_PLUS_1000		= 34,	// Australia (Australian Capital Territory), Papua New Guinea, Russia (Vladivostok Time)
	UTC_PLUS_1030		= 35,	// Australia (Lord Howe Island)
	UTC_PLUS_1100		= 36,	// Russia (Srednekolymsk Time), Salomon Islands, Vanuatu
	UTC_PLUS_1200		= 37,	// Fiji, Kiribati (Gilbert Islands), Marshall Islands, Nauru, New Zealand, Russia (Kamchatka Time), Tuvalu
	UTC_PLUS_1245		= 38,	// New Zealand (Chatham Islands)
	UTC_PLUS_1300		= 39,	// Kiribati (Phoenix Islands), New Zealand (Tokelau), Samoa, Tonga
	UTC_PLUS_1400		= 40,	// Kiribati (Line Islands)
} utcTimeZone_e;

// New Data Type - Enumerations => parametric table that doesn't mantain value equivalence with datasheet
typedef enum sleepMode_e {
	SLEEP_MODE_NO_CHANGE	= 0,	// Keep last sleep mode
	SLEEP_MODE_IDLE			= 1,	// Sleep mode IDLE
	SLEEP_MODE_ADC			= 2,	// Sleep mode for ADC noise reduction
	SLEEP_MODE_PWR_DOWN		= 3,	// Sleep mode POWER DOWN
	SLEEP_MODE_PWR_SAVE		= 4,	// Sleep mode POWER SAVR
	SLEEP_MODE_STANDBY		= 5,	// Sleep mode STANDBY
	SLEEP_MODE_EXT_STANDBY	= 6		// Sleep mode EXTENDED STANDBY
} sleepMode_e;

typedef enum timerOutput_e {
	TIMER_PORT_NO_CHANGE			= 0,	// Keep last port configuration
	TIMER_PORT_NORMAL				= 1,	// Output Compare Pin disconnected; normal GPIO operation
	TIMER_PORT_TOGGLE_ON_COMPARE	= 2,	// Toggle pin value on compare
	TIMER_PORT_CLEAR_ON_COMPARE		= 3,	// Clear pin on compare
	TIMER_PORT_SET_ON_COMPARE		= 4,	// Set pin on compare
	TIMER_PORT_NON_INVERTING_MODE	= 5,	// PWM non-inverting mode
	TIMER_PORT_INVERTING_MODE		= 6		// PWM inverting mode
} timerOutput_e;

typedef enum portMode_e {
	PORT_NO_CHANGE		= 0,		// Keep last pin configurations
	PORT_OUTPUT_LOW		= 1,		// Pin configured as OUTPUT, LOW logic level
	PORT_OUTPUT_HIGH	= 2,		// Pin configured as OUTPUT, HIGH logic level
	PORT_INPUT_TRISTATE	= 3,		// Pin configured as INPUT, PULL-UPs off
	PORT_INPUT_PULL_UP	= 4			// Pin configured as INPUT, PULL-UPs on
} portMode_e;

typedef enum lcdSize_e {
	LCD_SIZE_UNDEFINED	= 0,		// Used to verify if the variable was previously initialized
	LCD_8X1				= 108,		// LCD 8 columns x 1 line
	LCD_8X2				= 208,		// LCD 8 columns x 2 line
	LCD_10x2			= 210,		// LCD 10 columns x 2 lines
	LCD_12X2			= 212,		// LCD 12 columns x 2 lines
	LCD_12X4			= 412,		// LCD 12 columns x 4 lines
	LCD_16X1			= 116,		// LCD 16 columns x 1 line
	LCD_16X2			= 216,		// LCD 16 columns x 2 lines
	LCD_16X4			= 416,		// LCD 16 columns x 4 lines
	LCD_20X1			= 120,		// LCD 20 columns x 1 line
	LCD_20X2			= 220,		// LCD 20 columns x 2 lines
	LCD_20X4			= 420,		// LCD 20 columns x 4 lines
	LCD_24X1			= 124,		// LCD 24 columns x 1 line
	LCD_24X2			= 224,		// LCD 24 columns x 2 lines
	LCD_30X2			= 230,		// LCD 30 columns x 2 lines
	LCD_32X2			= 232,		// LCD 32 columns x 2 lines
	LCD_40X1			= 140,		// LCD 40 columns x 1 line
	LCD_40X2			= 240		// LCD 20 columns x 2 lines
} lcdSize_e;

typedef enum lcdFont_e {
	LCD_FONT_NO_CHANGE		= 0,	// Keep last value configured
	LCD_FONT_5X8			= 1,	// Use font 5x8 pixels
	LCD_FONT_5X10			= 2		// Use font 5x8 pixels
} lcdFont_e;

typedef enum lcdDisplayMode_e {
	LCD_DISPLAY_MODE_NO_CHANGE	= 0,	// Keep last mode configured
	LCD_DISPLAY_MODE_OVERWRITE	= 1,	// Overwrite mode
	LCD_DISPLAY_MODE_SHIFT		= 2		// Insert mode (shift display)
} lcdDisplayMode_e ;

typedef enum lcdDisplayDirection_e {
	LCD_DISPLAY_DIRECTION_NO_CHANGE		= 0,	// Keep last value configured
	LCD_DISPLAY_DIRECTION_DECREMENT		= 1,	// Decrement address after character write
	LCD_DISPLAY_DIRECTION_INCREMENT		= 2		// Increment address after character write
} lcdDisplayDirection_e;

// New Data Type - Flags => enumerations that can be combined in the form of flags
typedef enum powerReductionFlags_f {
	PRR_NONE		= 0,	// Keep last value configured
	PRR_ADC			= 1,	// Keep last value configured
	PRR_USART0		= 2,	// Keep last value configured
	PRR_SPI			= 4,	// Keep last value configured
	PRR_TIM0		= 8,	// Keep last value configured
	PRR_TIM1		= 16,	// Keep last value configured
	PRR_TIM2		= 32,	// Keep last value configured
	PRR_PRTWI		= 64	// Keep last value configured
} powerReductionFlags_f;

// New Data Type - Structures, Unions and Bit Fields
typedef struct dateTime_s {
	uint16_t		year		: 11;		// Year in four digits format; valid values in range [0 to 2047]
	month_t			month		: 4;		// Month number; valid values in range [1 to 12], 0 is reserved for NOT DEFINED
	uint8_t			day			: 5;		// Month day number; valid values in range [1 to 31], 0 is reserved for NOT DEFINED
	weekDay_t		weekDay		: 3;		// Week day number; valid values in range [1 to 7], 0 is reserved for NOT DEFINED
	uint8_t			hour		: 5;		// Hour in 24-hours format; valid values in range [0 to 24]
	uint8_t			minute		: 6;		// Minutes; valid values in range [0 to 59]
	uint8_t			second		: 6;		// Seconds; valid values in range [0 to 59]
	bool_t			leapYear	: 1;		// If the year is a leap year
	bool_t			summerTime	: 1;		// If summer time is active
	utcTimeZone_e	timeZone	: 6;		// Current time zone
} dateTime_s;								// \private

typedef enum systemClockPrescaler_e {
	SYSTEM_CLOCK_PRESCALER_1		= 1,
	SYSTEM_CLOCK_PRESCALER_2		= 2,
	SYSTEM_CLOCK_PRESCALER_4		= 4,
	SYSTEM_CLOCK_PRESCALER_8		= 8,
	SYSTEM_CLOCK_PRESCALER_16		= 16,
	SYSTEM_CLOCK_PRESCALER_32		= 32,
	SYSTEM_CLOCK_PRESCALER_64		= 64,
	SYSTEM_CLOCK_PRESCALER_128		= 128,
	SYSTEM_CLOCK_PRESCALER_256		= 256
} systemClockPrescaler_e;


// New Data Type - Classes

class SystemStatus
{
public:
	// Methods
	SystemStatus(uint32_t mainClock = F_CPU);
	error_e changeClockPrescaler(systemClockPrescaler_e prescaler);
	uint32_t getCpuClock(void);
	error_e getLastError(void);
	void announceError(error_e error);

	// Members
private:
	uint32_t				mainSourceClock_	: 27;
	uint32_t				cpuClock_			: 27;
	systemClockPrescaler_e	clockPrescaler_		: 10;
	uint32_t				millis_;
	error_e					lastError_;
protected:

};


// -----------------------------------------------------------------------------
// Extern Global Variables -----------------------------------------------------

#ifndef __DOXYGEN__
	extern SystemStatus systemStatus;
#endif



















// -----------------------------------------------------------------------------
// Definitions for each microcontroller grouped by family ----------------------

// -----------------------------------------------------------------------------
// ATmega48			ATmega48A		ATmega48P		Atmega48PA		ATmega48PB
// ATmega88			ATmega88A		ATmega88P		Atmega88PA		ATmega88PB
// ATmega168		ATmega168A		ATmega168P		Atmega168PA		ATmega168PB
// ATmega328						ATmega328P						ATmega328PB
// -----------------------------------------------------------------------------

// Entire family
#if defined(__AVR_ATmega48__) || defined(__AVR_ATmega48A__) || defined(__AVR_ATmega48P__) || defined(__AVR_ATmega48PA__) || defined(__AVR_ATmega48PB__) || defined(__AVR_ATmega88__) || defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88P__) || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega88PB__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168A__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega168PA__) || defined(__AVR_ATmega168PB__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328PB__)
	#define MCU_ATMEGAX8_ALL
#endif

// Grouped by memory size
#if defined(__AVR_ATmega48__) || defined(__AVR_ATmega48A__) || defined(__AVR_ATmega48P__) || defined(__AVR_ATmega48PA__) || defined(__AVR_ATmega48PB__)
	#define MCU_ATMEGA48_ALL
#elif defined(__AVR_ATmega88__) || defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88P__) || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega88PB__)
	#define MCU_ATMEGA88_ALL
#elif defined(__AVR_ATmega168__) || defined(__AVR_ATmega168A__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega168PA__) || defined(__AVR_ATmega168PB__)
	#define MCU_ATMEGA168_ALL
#elif defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328PB__)
	#define MCU_ATMEGA328_ALL
#endif

// Grouped by suffix
#if defined(__AVR_ATmega48__) || defined(__AVR_ATmega88__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega328__)
	#define MCU_ATMEGAX8
#elif defined(__AVR_ATmega48A__) || (__AVR_ATmega88A__) || defined(__AVR_ATmega168A__)
	#define MCU_ATMEGAX8A
#elif defined(__AVR_ATmega48P__) || defined(__AVR_ATmega88P__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__)
	#define MCU_ATMEGAX8P
#elif defined(__AVR_ATmega48PA__) || defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega168PA__)
	#define MCU_ATMEGAX8PA
#elif defined(__AVR_ATmega48PB__) || defined(__AVR_ATmega88PB__) || defined(__AVR_ATmega168PB__) || defined(__AVR_ATmega328PB__)
	#define MCU_ATMEGAX8PB
#endif

// -----------------------------------------------------------------------------
// Constant definitions (microcontroller-specific) -----------------------------

#ifndef __DOXYGEN__
	// Memories
	#if defined(MCU_ATMEGAX8_ALL)
		// Memory start address
		#define EEPROM_START				0x0000
		#define FLASH_START					0x0000
		#define RAM_START					0x0100
		// Memory size and organization
		#define EEPROM_PAGE_SIZE			4
		#if defined(MCU_ATMEGA48_ALL)
			#define EEPROM_SIZE				256
			#define FLASH_PAGE_SIZE			64
			#define FLASH_SIZE				4096
			#define RAM_SIZE				512
		#elif defined(MCU_ATMEGA88_ALL)
			#define EEPROM_SIZE				512
			#define FLASH_PAGE_SIZE			64
			#define FLASH_SIZE				8192
			#define RAM_SIZE				1024
		#elif defined(MCU_ATMEGA168_ALL)
			#define EEPROM_SIZE				512
			#define FLASH_PAGE_SIZE			128
			#define FLASH_SIZE				16384
			#define RAM_SIZE				1024
		#elif defined(MCU_ATMEGA328_ALL)
			#define EEPROM_SIZE				1024
			#define FLASH_PAGE_SIZE			128
			#define FLASH_SIZE				32768
			#define RAM_SIZE				2048
		#endif
		// Memory end address
		#define FLASH_END					(FLASH_START + FLASH_SIZE - 1)
		#define RAM_END						(RAM_START + RAM_SIZE - 1)
		#define EEPROM_END					(EEPROM_START + EEPROM_SIZE - 1)
		// External RAM memory
		#define XRAM_START					(RAM_END + 1)
		#define XRAM_SIZE					0
		#define XRAM_END					(XRAM_START + XRAM_END - 1)
	#endif

	// ADC internal reference voltage
	#if defined(MCU_ATMEGAX8_ALL)
		#define ADC_INTERNAL_REFERENCE_VALUE_V		1.1
		#define ADC_INTERNAL_REFERENCE_VALUE_MV		1100
	#endif

	// Unique Serial Number
	#define SERIAL_NUMBER_0		_SFR_MEM8(0xF0)
	#define SERIAL_NUMBER_1		_SFR_MEM8(0xF1)
	#define SERIAL_NUMBER_2		_SFR_MEM8(0xF2)
	#define SERIAL_NUMBER_3		_SFR_MEM8(0xF3)
	#define SERIAL_NUMBER_4		_SFR_MEM8(0xF4)
	#define SERIAL_NUMBER_5		_SFR_MEM8(0xF5)
	#define SERIAL_NUMBER_6		_SFR_MEM8(0xF6)
	#define SERIAL_NUMBER_7		_SFR_MEM8(0xF7)
	#define SERIAL_NUMBER_8		_SFR_MEM8(0xF8)

	// -----------------------------------------------------------------------------
	// PORT generic pin names ------------------------------------------------------

	#if defined(MCU_ATMEGAX8_ALL)
		// PORT B
		#define PB0		0
		#define PB1		1
		#define PB2		2
		#define PB3		3
		#define PB4		4
		#define PB5		5
		#define PB6		6
		#define PB7		7
		// PORT C
		#define PC0		0
		#define PC1		1
		#define PC2		2
		#define PC3		3
		#define PC4		4
		#define PC5		5
		#define PC6		6
		// PORT D
		#define PD0		0
		#define PD1		1
		#define PD2		2
		#define PD3		3
		#define PD4		4
		#define PD5		5
		#define PD6		6
		#define PD7		7
		// PORT E Input Register
		#if defined(MCU_ATMEGAX8PB)
			#define PE0		0
			#define PE1		1
			#define PE2		2
			#define PE3		3
		#endif
	#endif
#endif


// -----------------------------------------------------------------------------
// Public Functions Declarations ------------------------------------------------

/*!
\fn			weekDay_t dayOfWeek(uint8_t monthDay, month_t month, uint16_t year);
\details	Calculates the day of the week for a given day.
\param		monthDay	day of the month
\param		month		number of month (\a month_t data type compatible)
\param		year		year (four digits format)
\return		the day of the week (\a weekDay_t data type compatible)
*/
weekDay_t dayOfWeek(uint8_t monthDay, month_t month, uint16_t year);





#endif
