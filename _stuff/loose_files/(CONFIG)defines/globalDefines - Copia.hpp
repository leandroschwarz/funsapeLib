


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

//#include "stm32l0xx_hal.h"

// -----------------------------------------------------------------------------
// Constant definitions --------------------------------------------------------

#define __DEBUG_MODE					0

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

/* NONE */

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

typedef enum outputDriver_t {
	PUSH_PULL	= 0,
	OPEN_DRAIN	= 1,
} outputDriver_t;

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

typedef enum error_e {
	// System	0-29
	ERROR_NONE														= 0,	// Success - No errors occurred
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

// New Data Type - Classes
/*
class SystemStatus
{
public:
	// Methods
	SystemStatus(uint32_t mainClock = F_CPU);
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

	// Members
private:
	uint32_t				mainSourceClock_	: 26;
	uint32_t				cpuClock_			: 26;
	systemClockPrescaler_e	clockPrescaler_		: 10;
	volatile uint32_t		stopWatchValue_;
	volatile uint32_t		stopWatchMark_;
	bool_t					stopWatchHalted_	: 1;
	error_e					lastError_;
	bool_t					unusedBits			: 1;
protected:

};
*/
// -----------------------------------------------------------------------------
// Extern Global Variables -----------------------------------------------------
/*
extern SystemStatus systemStatus;
*/


// -----------------------------------------------------------------------------
// Definitions for each microcontroller grouped by family ----------------------

// -----------------------------------------------------------------------------
// Public Functions Declarations ------------------------------------------------

weekDay_t dayOfWeek(uint8_t monthDay, month_t month, uint16_t year);


#endif
