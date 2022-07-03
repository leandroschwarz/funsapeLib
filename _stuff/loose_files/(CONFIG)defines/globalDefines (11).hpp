/* -----------------------------------------------------------------------------
 * Project:		GPSDE++ AVR Library
 * File name:	globalDefines.hpp
 * Module:		Global definitions for GPDSE++ AVR Library project
 * Authors:		Leadro Schwarz
 *				Hazael dos Santos Batista
 * Build:		1001
 * Date:		May 26, 2018
 * -------------------------------------------------------------------------- */

// -----------------------------------------------------------------------------
// Include guards --------------------------------------------------------------

#ifndef __GLOBAL_DEFINES_HPP
#define __GLOBAL_DEFINES_HPP 1001

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// New data types --------------------------------------------------------------

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

typedef enum activation_t {
	OFF		= 0,
	ON		= 1
} activation_t;

typedef enum bool_t {
	FALSE	= 0,
	TRUE	= 1
} bool_t;

typedef enum dataFlow_t {
	INPUT	= 0,
	OUTPUT	= 1
} dataFlow_t;

typedef enum edge_t {
	FALLING_EDGE	= 0,
	RISING_EDGE		= 1
} edge_t;

typedef enum logicLevel_t {
	LOW		= 0,
	HIGH	= 1,
} logicLevel_t;

typedef enum outputDriver_t {
	PUSH_PULL	= 0,
	OPEN_DRAIN	= 1,
} outputDriver_t;

typedef enum rotacionalDirection_t {
	CLOCKWISE			= 0,
	COUNTERCLOCKWISE	= 1
} rotacionalDirection_t;

// New Data Type - Single variables => small element-limited enumeratio that can be understood as a single variable

typedef enum axis_t {
	AXIS_NONE	= 0,
	AXIS_X		= 1,
	AXIS_Y		= 2,
	AXIS_XY		= 3,
	AXIS_Z		= 4,
	AXIS_XZ		= 5,
	AXIS_YZ		= 6,
	AXIS_ALL	= 7
} axis_t;

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

// New Data Type - Enumerations => large enumerations or enumerations with expansible number of elements

typedef enum error_e {
	// System	0-29
	ERROR_NONE														= 0,	// Success - No errors occurred
	ERROR_UNSUPPORTED_CONFIGURATION_SYSTEM_CLOCK_PRESCALER			= 1,	// System clock prescaler configuration unsupported
	ERROR_SYSTEM_CLOCK_PRESCALER_COULD_NOT_BE_CHANGED				= 2,	// System clock prescaler change failed
	// INTx		30-39
// 	ERROR_INT0_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE			= 30,	// Int0 output port mode configuration unsupported
// 	ERROR_INT0_UNSUPPORTED_CONFIGURATION_SENSE_MODE					= 31,	// Int0 sense mode configuration unsupported
// 	ERROR_INT1_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE			= 32,	// Int1 output port mode configuration unsupported
// 	ERROR_INT1_UNSUPPORTED_CONFIGURATION_SENSE_MODE					= 33,	// Int1 sense mode configuration unsupported
// 	ERROR_INT2_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE			= 34,	// Int2 output port mode configuration unsupported
// 	ERROR_INT2_UNSUPPORTED_CONFIGURATION_SENSE_MODE					= 35,	// Int2 sense mode configuration unsupported
// 	ERROR_INT3_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE			= 36,	// Int3 output port mode configuration unsupported
// 	ERROR_INT3_UNSUPPORTED_CONFIGURATION_SENSE_MODE					= 37,	// Int3 sense mode configuration unsupported
// 	ERROR_INT4_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE			= 38,	// Int4 output port mode configuration unsupported
// 	ERROR_INT4_UNSUPPORTED_CONFIGURATION_SENSE_MODE					= 39,	// Int4 sense mode configuration unsupported
// 	ERROR_INT5_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE			= 40,	// Int5 output port mode configuration unsupported
// 	ERROR_INT5_UNSUPPORTED_CONFIGURATION_SENSE_MODE					= 41,	// Int5 sense mode configuration unsupported
// 	ERROR_INT6_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE			= 42,	// Int6 output port mode configuration unsupported
// 	ERROR_INT6_UNSUPPORTED_CONFIGURATION_SENSE_MODE					= 43,	// Int6 sense mode configuration unsupported
// 	ERROR_INT7_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE			= 44,	// Int7 output port mode configuration unsupported
// 	ERROR_INT7_UNSUPPORTED_CONFIGURATION_SENSE_MODE					= 45,	// Int7 sense mode configuration unsupported
	// PCINTx	50-59
// 	ERROR_PCINT0_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE			= 50,	// Pcint0 output port mode configuration unsupported
// 	ERROR_PCINT1_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE			= 51,	// Pcint1 output port mode configuration unsupported
// 	ERROR_PCINT2_UNSUPPORTED_CONFIGURATION_PORT_OUTPUT_MODE			= 52,	// Pcint2 output port mode configuration unsupported
	// TIMER0	80-89
// 	ERROR_TIMER0_UNSUPPORTED_CONFIGURATION_CLOCK_PRESCALER			= 80,	// Timer0 clock prescaler value off limits
// 	ERROR_TIMER0_UNSUPPORTED_CONFIGURATION_MODE						= 81,	// Timer0 mode unsupported
// 	ERROR_TIMER0_UNSUPPORTED_CONFIGURATION_OUTPUT_COMPARE			= 82,	// Timer0 output pin configuration unsupported
	// TIMER1	90-99
// 	ERROR_TIMER1_UNSUPPORTED_CONFIGURATION_CLOCK_PRESCALER			= 90,	// Timer1 clock prescaler value off limits
// 	ERROR_TIMER1_UNSUPPORTED_CONFIGURATION_MODE						= 91,	// Timer1 mode unsupported
// 	ERROR_TIMER1_UNSUPPORTED_CONFIGURATION_OUTPUT_COMPARE			= 92,	// Timer1 output pin configuration unsupported
// 	ERROR_TIMER1_UNSUPPORTED_CONFIGURATION_CAPTURE_EDGE				= 93,	// Timer1 invalid edge section on capture mode
// 	ERROR_TIMER1_UNSUPPORTED_CONFIGURATION_CAPTURE_NOISE_CANCELER	= 94,	// Timer0 invalid noise canceler configuration
	// TIMER2	100-109
// 	ERROR_TIMER2_UNSUPPORTED_CONFIGURATION_CLOCK_PRESCALER			= 100,	// Timer2 clock prescaler value off limits
// 	ERROR_TIMER2_UNSUPPORTED_CONFIGURATION_MODE						= 101,	// Timer2 mode unsupported
// 	ERROR_TIMER2_UNSUPPORTED_CONFIGURATION_OUTPUT_COMPARE			= 102,	// Timer2 output pin configuration unsupported
	// ADC		110-119
// 	ERROR_ADC_UNSUPPORTED_CONFIGURATION_REFERENCE					= 110,	// ADC reference selection unsupported
// 	ERROR_ADC_UNSUPPORTED_CONFIGURATION_CLOCK_PRESCALER				= 111,	// ADC clock prescaler value off limits
// 	ERROR_ADC_UNSUPPORTED_CONFIGURATION_MODE						= 112,	// ADC mode unsupported
// 	ERROR_ADC_UNSUPPORTED_CONFIGURATION_CHANNEL						= 113,	// ADC invalid channel selection
// 	ERROR_ADC_UNSUPPORTED_CONFIGURATION_DATA_PRESENTATION			= 114,	// ADC invalid data presentation order
	// USART	120-129
	ERROR_USART_UNSUPPORTED_CONFIGURATION_STOP_BIT					= 120,	// USART invalid number of stop bits
	ERROR_USART_UNSUPPORTED_CONFIGURATION_PARITY					= 121,	// USART invalid parity selection
	ERROR_USART_UNSUPPORTED_CONFIGURATION_DATA_BITS					= 122,	// USART invalid number of data bits
// 	ERROR_USART_UNSUPPORTED_CONFIGURATION_MODE						= 123,	// USART unsupported mode
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

// New Data Type - Enumerations => parametric table that doesn't maintain value equivalence with datasheet's values

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

// New Data Type - Flags => enumerations that can be combined in the form of flags

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
#define byteLowNibble(dataByte)						(dataByte & 0x0F)
#define byteHighNibble(dataByte)					((dataByte >> 4) & 0x0F)
#define byteSwitchNibbles(dataByte)					(byteLowNibble(dataByte) << 4) | byteHighNibble(dataByte)
#define wordLowByte(dataWord)						(dataWord & 0x00FF)
#define wordHighByte(dataWord)						((dataWord >> 8) & 0x00FF)
#define wordSwitchBytes(dataWord)					(wordLowByte(dataWord) << 8) | wordHighByte(dataWord)

// System actions
#define systemHalt()								while(1) {}
#define die(errorCode)								do {printf("Failed with error=%u.\r", errorCode);systemHalt();} while(0)

// Value comparison macro-functions
#define minimumOf(var1 , var2)						((var1 < var2) ? var1 :var2)
#define maximumOf(var1 , var2)						((var1 > var2) ? var1 :var2)
#define truncateBetween(var, minimum, maximum)		(var < minimum) ? minimum : ((var > maximum) ? maximum : var))

// Stringification of macro arguments
#define stringifyName(constant)						#constant
#define stringifyValue(constant)					stringifyName(constant)

// -----------------------------------------------------------------------------
// Extern Global Variables -----------------------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// General public function declarations ----------------------------------------

weekDay_t dayOfWeek(uint8_t monthDay, month_t month, uint16_t year);

// -----------------------------------------------------------------------------
// General inline function declarations ----------------------------------------

/* NONE */

// -----------------------------------------------------------------------------
// SystemConfiguration class - Class Definition --------------------------------

class SystemConfiguration
{
	// New data types - Public
public:

	// New data types - Private
private:

	// New data types - Protected
protected:

	// Constructors
public:
	SystemConfiguration(uint32_t mainClock = F_CPU);

	// Methods - Public
public:
	error_e changeClockPrescaler(systemClockPrescaler_e prescaler);
	uint32_t getCpuClock(void);
	error_e getLastError(void);
	uint32_t readStopWatch(void);
	void resumeStopWatch(void);
	void pauseStopWatch(void);
	void resetStopWatch(void);
	void setStopWatchMark(void);
	void incrementStopWatch(void);
	uint32_t calculateElapsedTime(bool_t setNewMark);
	uint32_t calculateElapsedTime(uint32_t customMark);
	void announceError(error_e error);

	// Methods - Private
private:

	// Methods - Protected
protected:

	// Data members - Public
public:

	// Data members - Private
private:
	uint32_t				mainSourceClock_	: 26;
	uint32_t				cpuClock_			: 26;
	systemClockPrescaler_e	clockPrescaler_		: 10;
	volatile uint32_t		stopWatchValue_;
	volatile uint32_t		stopWatchMark_;
	bool_t					stopWatchHalted_	: 1;
	error_e					lastError_;
	// 8 bits implementation	=> unusedBits = 1
	// 32 bits implementation	=> unusedBits = 25

	// Data members - Protected
protected:

};	// class SystemConfiguration

// -----------------------------------------------------------------------------
// SystemConfiguration class - Static or inline functions definitions ----------

/* NONE */

// -----------------------------------------------------------------------------
// General static or inline functions declarations -----------------------------

/* NONE */

// -----------------------------------------------------------------------------
// External class instantiations -----------------------------------------------

extern SystemConfiguration systemConfiguration;

// -----------------------------------------------------------------------------
// Others ----------------------------------------------------------------------

typedef enum displayType_t {
	DISPLAY_COMMON_ANODE	= 0,
	DISPLAY_COMMON_CATHODE	= 1
} displayType_t;

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

// -----------------------------------------------------------------------------
// >>>>>>>>>>>>>>>>>>>>         TWI MODULE SPECIFIC         <<<<<<<<<<<<<<<<<<<<
// -----------------------------------------------------------------------------

typedef enum twiOperation_e {
	TWI_WRITE			= 0,
	TWI_READ			= 1,
	TWI_SET_THEN_READ	= 2
} twiOperation_e;

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

// -----------------------------------------------------------------------------
// >>>>>>>>>>>>>>>>>>>>      MICROCONTROLLER  SPECIFIC      <<<<<<<<<<<<<<<<<<<<
// -----------------------------------------------------------------------------

#endif	// __GLOBAL_DEFINES_HPP
