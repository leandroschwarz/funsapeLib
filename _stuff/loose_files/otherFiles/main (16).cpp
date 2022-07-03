/*
 * dotMatrixDisplay.cpp
 *
 * Created: 11/11/2018 17:49:48
 * Author : Schwarz
 */

#include "gpdseLibGlobalDefines.hpp"
#include <avr/pgmspace.h>


const uint64_t IMAGES[] PROGMEM = {
	0x10387cfefeee4400,					// <CHAR 0> => Hearts
	0x38107cfe7c381000,					// <CHAR 1> => Swords
	0x10387cfe7c381000,					// <CHAR 2> => Golds
	0x381054fe54381000,					// <CHAR 3> => Cups
	0x383838fe7c381000,					// <CHAR 4> => Arrow Up
	0x10387cfe38383800,					// <CHAR 5> => Arrow Down
	0x10307efe7e301000,					// <CHAR 6> => Arrow Right
	0x1018fcfefc181000,					// <CHAR 7> => Arrow Left
	0xfefe7c7c38381000,					// <CHAR 8> => Large Triangle Up
	0x1038387c7cfefe00,					// <CHAR 9> => Large Triangle Down
	0x061e7efe7e1e0600,					// <CHAR 10> => Large Triangle Right
	0xc0f0fcfefcf0c000,					// <CHAR 11> => Large Triangle Left
	0x007e3c187e3c1800,					// <CHAR 12> => Double Triangle Up
	0x00183c7e183c7e00,					// <CHAR 13> => Double Triangle Down
	0x0012367e7e361200,					// <CHAR 14> => Double Triangle Right
	0x00486c7e7e6c4800,					// <CHAR 15> => Double Triangle Left
	0x7c92aa82aa827c00,					// <CHAR 16> => Smile
	0x7ceed6fed6fe7c00,					// <CHAR 17> => Smile Inverted
	0x00387c7c7c380000,					// <CHAR 18> => Circle
	0xffc7838383c7ffff,					// <CHAR 19> => Circle Inverted
	0x0038444444380000,					// <CHAR 20> => Circumference
	0xffc7bbbbbbc7ffff,					// <CHAR 21> => Circumference Inverted
	0x0e1111119ea0c0f0,					// <CHAR 22> => Male Sign
	0x107c103844444438,					// <CHAR 23> => Female Sign
	0x060e0c0808281800,					// <CHAR 24> => Seminima
	0x066eecc88898f000,					// <CHAR 25> => Double Seminima
	0x105438ee38541000,					// <CHAR 26> => Sun
	0xefabc711c7abefff,					// <CHAR 27> => Sun Inverted
	0x1038541054381000,					// <CHAR 28> => Arrow Up-Down
	0x002844fe44280000,					// <CHAR 29> => Arrow Right-Left
	0x6666006666666600,					// <CHAR 30> => !!
	0xfe8282c66c381000,					// <CHAR 31> => Pentagon
	0x0000000000000000,					// <CHAR 32> => <SPACE>
	0x180018183c3c1800,					// <CHAR 33> => !
	0x00000000246c6c00,					// <CHAR 34> => "
	0x6c6cfe6cfe6c6c00,					// <CHAR 35> => #
	0x103c403804781000,					// <CHAR 36> => $
	0x60660c1830660600,					// <CHAR 37> => %
	0xfc66a6143c663c00,					// <CHAR 38> => &
	0x0000000018181800,					// <CHAR 39> => '
	0x6030181818306000,					// <CHAR 40> => (
	0x060c1818180c0600,					// <CHAR 41> => )
	0x006c38fe386c0000,					// <CHAR 42> => *
	0x0010107c10100000,					// <CHAR 43> => +
	0x060c0c0c00000000,					// <CHAR 44> => ,
	0x0000003c00000000,					// <CHAR 45> => -
	0x0606000000000000,					// <CHAR 46> => .
	0x00060c1830600000,					// <CHAR 47> => /
	0x1c08080808080c08,					// <CHAR 48> => 0
	0x3e0408102020221c,					// <CHAR 49> => 1
	0x1c2220201820221c,					// <CHAR 50> => 2
	0x20203e2224283020,					// <CHAR 51> => 3
	0x1c2220201e02023e,					// <CHAR 52> => 4
	0x1c2222221e02221c,					// <CHAR 53> => 5
	0x040404081020203e,					// <CHAR 54> => 6
	0x1c2222221c22221c,					// <CHAR 55> => 7
	0x1c22203c2222221c,					// <CHAR 56> => 8
	0x1c2222222222221c,					// <CHAR 57> => 9
	0x0018180018180000,					// <CHAR 58> => :
	0x0c18180018180000,					// <CHAR 59> => ;
	0x6030180c18306000,					// <CHAR 60> => <
	0x00003c003c000000,					// <CHAR 61> => =
	0x060c1830180c0600,					// <CHAR 62> => >
	0x1800183860663c00,					// <CHAR 63> => ?
	0x003c421a3a221c00,					// <CHAR 64> => @
	0x6666667e66663c00,					// <CHAR 65> => A
	0x3e66663e66663e00,					// <CHAR 66> => B
	0x3c66060606663c00,					// <CHAR 67> => C
	0x3e66666666663e00,					// <CHAR 68> => D
	0x7e06063e06067e00,					// <CHAR 69> => E
	0x0606063e06067e00,					// <CHAR 70> => F
	0x3c66760606663c00,					// <CHAR 71> => G
	0x6666667e66666600,					// <CHAR 72> => H
	0x3c18181818183c00,					// <CHAR 73> => I
	0x1c36363030307800,					// <CHAR 74> => J
	0x66361e0e1e366600,					// <CHAR 75> => K
	0x7e06060606060600,					// <CHAR 76> => L
	0xc6c6c6d6feeec600,					// <CHAR 77> => M
	0xc6c6e6f6decec600,					// <CHAR 78> => N
	0x3c66666666663c00,					// <CHAR 79> => O
	0x06063e6666663e00,					// <CHAR 80> => P
	0x603c766666663c00,					// <CHAR 81> => Q
	0x66361e3e66663e00,					// <CHAR 82> => R
	0x3c66603c06663c00,					// <CHAR 83> => S
	0x18181818185a7e00,					// <CHAR 84> => T
	0x7c66666666666600,					// <CHAR 85> => U
	0x183c666666666600,					// <CHAR 86> => V
	0xc6eefed6c6c6c600,					// <CHAR 87> => W
	0xc6c66c386cc6c600,					// <CHAR 88> => X
	0x1818183c66666600,					// <CHAR 89> => Y
	0x7e060c1830607e00,					// <CHAR 90> => Z
	0x7818181818187800,					// <CHAR 91> => [
	0x006030180c060000,					// <CHAR 92> => '\'
	0x1e18181818181e00,					// <CHAR 93> => ]
	0x000000663c180000,					// <CHAR 94> => ^
	0x7e00000000000000,					// <CHAR 95> => _
	0x0000000010180c00,					// <CHAR 96> => `
	0x7c667c603c000000,					// <CHAR 97> => a
	0x3e66663e06060600,					// <CHAR 98> => b
	0x3c6606663c000000,					// <CHAR 99> => c
	0x7c66667c60606000,					// <CHAR 100> => d
	0x3c067e663c000000,					// <CHAR 101> => e
	0x0c0c3e0c0c6c3800,					// <CHAR 102> => f
	0x3c607c66667c0000,					// <CHAR 103> => g
	0x6666663e06060600,					// <CHAR 104> => h
	0x3c18181800180000,					// <CHAR 105> => i
	0x1c36363030003000,					// <CHAR 106> => j
	0x66361e3666060600,					// <CHAR 107> => k
	0x1818181818181800,					// <CHAR 108> => l
	0xd6d6feeec6000000,					// <CHAR 109> => m
	0x6666667e3e000000,					// <CHAR 110> => n
	0x3c6666663c000000,					// <CHAR 111> => o
	0x06063e66663e0000,					// <CHAR 112> => p
	0xf0b03c36363c0000,					// <CHAR 113> => q
	0x060666663e000000,					// <CHAR 114> => r
	0x3e403c027c000000,					// <CHAR 115> => s
	0x1818187e18180000,					// <CHAR 116> => t
	0x7c66666666000000,					// <CHAR 117> => u
	0x183c666600000000,					// <CHAR 118> => v
	0x7cd6d6d6c6000000,					// <CHAR 119> => w
	0x663c183c66000000,					// <CHAR 120> => x
	0x3c607c6666000000,					// <CHAR 121> => y
	0x3c0c18303c000000,					// <CHAR 122> => z
	0x7018180c18187000,					// <CHAR 123> => {
	0x1818181818181818,					// <CHAR 124> => |
	0x0e18183018180e00,					// <CHAR 125> => }
	0x000000365c000000,					// <CHAR 126> => ~
	0xffffffffffffffff,					// <CHAR 127> => Filled Square
	0x0100000000000000,					// <CHAR 128> => Moving Pixel Animation 0
	0x0200000000000000,					// <CHAR 129> => Moving Pixel Animation 1
	0x0400000000000000,					// <CHAR 130> => Moving Pixel Animation 2
	0x0800000000000000,					// <CHAR 131> => Moving Pixel Animation 3
	0x1000000000000000,					// <CHAR 132> => Moving Pixel Animation 4
	0x2000000000000000,					// <CHAR 133> => Moving Pixel Animation 5
	0x4000000000000000,					// <CHAR 134> => Moving Pixel Animation 6
	0x8000000000000000,					// <CHAR 135> => Moving Pixel Animation 7
	0x0001000000000000,					// <CHAR 136> => Moving Pixel Animation 8
	0x0002000000000000,					// <CHAR 137> => Moving Pixel Animation 9
	0x0004000000000000,					// <CHAR 138> => Moving Pixel Animation 10
	0x0008000000000000,					// <CHAR 139> => Moving Pixel Animation 11
	0x0010000000000000,					// <CHAR 140> => Moving Pixel Animation 12
	0x0020000000000000,					// <CHAR 141> => Moving Pixel Animation 13
	0x0040000000000000,					// <CHAR 142> => Moving Pixel Animation 14
	0x0080000000000000,					// <CHAR 143> => Moving Pixel Animation 15
	0x0000010000000000,					// <CHAR 144> => Moving Pixel Animation 16
	0x0000020000000000,					// <CHAR 145> => Moving Pixel Animation 17
	0x0000040000000000,					// <CHAR 146> => Moving Pixel Animation 18
	0x0000080000000000,					// <CHAR 147> => Moving Pixel Animation 19
	0x0000100000000000,					// <CHAR 148> => Moving Pixel Animation 20
	0x0000200000000000,					// <CHAR 149> => Moving Pixel Animation 21
	0x0000400000000000,					// <CHAR 150> => Moving Pixel Animation 22
	0x0000800000000000,					// <CHAR 151> => Moving Pixel Animation 23
	0x0000000100000000,					// <CHAR 152> => Moving Pixel Animation 24
	0x0000000200000000,					// <CHAR 153> => Moving Pixel Animation 25
	0x0000000400000000,					// <CHAR 154> => Moving Pixel Animation 26
	0x0000000800000000,					// <CHAR 155> => Moving Pixel Animation 27
	0x0000001000000000,					// <CHAR 156> => Moving Pixel Animation 28
	0x0000002000000000,					// <CHAR 157> => Moving Pixel Animation 29
	0x0000004000000000,					// <CHAR 158> => Moving Pixel Animation 30
	0x0000008000000000,					// <CHAR 159> => Moving Pixel Animation 31
	0x0000000001000000,					// <CHAR 160> => Moving Pixel Animation 32
	0x0000000002000000,					// <CHAR 161> => Moving Pixel Animation 33
	0x0000000004000000,					// <CHAR 162> => Moving Pixel Animation 34
	0x0000000008000000,					// <CHAR 163> => Moving Pixel Animation 35
	0x0000000010000000,					// <CHAR 164> => Moving Pixel Animation 36
	0x0000000020000000,					// <CHAR 165> => Moving Pixel Animation 37
	0x0000000040000000,					// <CHAR 166> => Moving Pixel Animation 38
	0x0000000080000000,					// <CHAR 167> => Moving Pixel Animation 39
	0x0000000000010000,					// <CHAR 168> => Moving Pixel Animation 40
	0x0000000000020000,					// <CHAR 169> => Moving Pixel Animation 41
	0x0000000000040000,					// <CHAR 170> => Moving Pixel Animation 42
	0x0000000000080000,					// <CHAR 171> => Moving Pixel Animation 43
	0x0000000000100000,					// <CHAR 172> => Moving Pixel Animation 44
	0x0000000000200000,					// <CHAR 173> => Moving Pixel Animation 45
	0x0000000000400000,					// <CHAR 174> => Moving Pixel Animation 46
	0x0000000000800000,					// <CHAR 175> => Moving Pixel Animation 47
	0x0000000000000100,					// <CHAR 176> => Moving Pixel Animation 48
	0x0000000000000200,					// <CHAR 177> => Moving Pixel Animation 49
	0x0000000000000400,					// <CHAR 178> => Moving Pixel Animation 50
	0x0000000000000800,					// <CHAR 179> => Moving Pixel Animation 51
	0x0000000000001000,					// <CHAR 180> => Moving Pixel Animation 52
	0x0000000000002000,					// <CHAR 181> => Moving Pixel Animation 53
	0x0000000000004000,					// <CHAR 182> => Moving Pixel Animation 54
	0x0000000000008000,					// <CHAR 183> => Moving Pixel Animation 55
	0x0000000000000001,					// <CHAR 184> => Moving Pixel Animation 56
	0x0000000000000002,					// <CHAR 185> => Moving Pixel Animation 57
	0x0000000000000004,					// <CHAR 186> => Moving Pixel Animation 58
	0x0000000000000008,					// <CHAR 187> => Moving Pixel Animation 59
	0x0000000000000010,					// <CHAR 188> => Moving Pixel Animation 60
	0x0000000000000020,					// <CHAR 189> => Moving Pixel Animation 61
	0x0000000000000040,					// <CHAR 190> => Moving Pixel Animation 62
	0x0000000000000080,					// <CHAR 191> => Moving Pixel Animation 63
	0x8000000000000000,					// <CHAR 192> => Spiral Animation Frame 0
	0x8080000000000000,					// <CHAR 193> => Spiral Animation Frame 1
	0x8080800000000000,					// <CHAR 194> => Spiral Animation Frame 2
	0x8080808000000000,					// <CHAR 195> => Spiral Animation Frame 3
	0x8080808080000000,					// <CHAR 196> => Spiral Animation Frame 4
	0x8080808080800000,					// <CHAR 197> => Spiral Animation Frame 5
	0x8080808080808000,					// <CHAR 198> => Spiral Animation Frame 6
	0x8080808080808080,					// <CHAR 199> => Spiral Animation Frame 7
	0x80808080808080c0,					// <CHAR 200> => Spiral Animation Frame 8
	0x80808080808080e0,					// <CHAR 201> => Spiral Animation Frame 9
	0x80808080808080f0,					// <CHAR 202> => Spiral Animation Frame 10
	0x80808080808080f8,					// <CHAR 203> => Spiral Animation Frame 11
	0x80808080808080fc,					// <CHAR 204> => Spiral Animation Frame 12
	0x80808080808080fe,					// <CHAR 205> => Spiral Animation Frame 13
	0x80808080808080ff,					// <CHAR 206> => Spiral Animation Frame 14
	0x80808080808081ff,					// <CHAR 207> => Spiral Animation Frame 15
	0x80808080808181ff,					// <CHAR 208> => Spiral Animation Frame 16
	0x80808080818181ff,					// <CHAR 209> => Spiral Animation Frame 17
	0x80808081818181ff,					// <CHAR 210> => Spiral Animation Frame 18
	0x80808181818181ff,					// <CHAR 211> => Spiral Animation Frame 19
	0x80818181818181ff,					// <CHAR 212> => Spiral Animation Frame 20
	0x81818181818181ff,					// <CHAR 213> => Spiral Animation Frame 21
	0x83818181818181ff,					// <CHAR 214> => Spiral Animation Frame 22
	0x87818181818181ff,					// <CHAR 215> => Spiral Animation Frame 23
	0x8f818181818181ff,					// <CHAR 216> => Spiral Animation Frame 24
	0x9f818181818181ff,					// <CHAR 217> => Spiral Animation Frame 25
	0xbf818181818181ff,					// <CHAR 218> => Spiral Animation Frame 26
	0xff818181818181ff,					// <CHAR 219> => Spiral Animation Frame 27
	0xffc18181818181ff,					// <CHAR 220> => Spiral Animation Frame 28
	0xffc1c181818181ff,					// <CHAR 221> => Spiral Animation Frame 29
	0xffc1c1c1818181ff,					// <CHAR 222> => Spiral Animation Frame 30
	0xffc1c1c1c18181ff,					// <CHAR 223> => Spiral Animation Frame 31
	0xffc1c1c1c1c181ff,					// <CHAR 224> => Spiral Animation Frame 32
	0xffc1c1c1c1c1c1ff,					// <CHAR 225> => Spiral Animation Frame 33
	0xffc1c1c1c1c1e1ff,					// <CHAR 226> => Spiral Animation Frame 34
	0xffc1c1c1c1c1f1ff,					// <CHAR 227> => Spiral Animation Frame 35
	0xffc1c1c1c1c1f9ff,					// <CHAR 228> => Spiral Animation Frame 36
	0xffc1c1c1c1c1fdff,					// <CHAR 229> => Spiral Animation Frame 37
	0xffc1c1c1c1c1ffff,					// <CHAR 230> => Spiral Animation Frame 38
	0xffc1c1c1c1c3ffff,					// <CHAR 231> => Spiral Animation Frame 39
	0xffc1c1c1c3c3ffff,					// <CHAR 232> => Spiral Animation Frame 40
	0xffc1c1c3c3c3ffff,					// <CHAR 233> => Spiral Animation Frame 41
	0xffc1c3c3c3c3ffff,					// <CHAR 234> => Spiral Animation Frame 42
	0xffc3c3c3c3c3ffff,					// <CHAR 235> => Spiral Animation Frame 43
	0xffc7c3c3c3c3ffff,					// <CHAR 236> => Spiral Animation Frame 44
	0xffcfc3c3c3c3ffff,					// <CHAR 237> => Spiral Animation Frame 45
	0xffdfc3c3c3c3ffff,					// <CHAR 238> => Spiral Animation Frame 46
	0xffffc3c3c3c3ffff,					// <CHAR 239> => Spiral Animation Frame 47
	0xffffe3c3c3c3ffff,					// <CHAR 240> => Spiral Animation Frame 48
	0xffffe3e3c3c3ffff,					// <CHAR 241> => Spiral Animation Frame 49
	0xffffe3e3e3c3ffff,					// <CHAR 242> => Spiral Animation Frame 50
	0xffffe3e3e3e3ffff,					// <CHAR 243> => Spiral Animation Frame 51
	0xffffe3e3e3f3ffff,					// <CHAR 244> => Spiral Animation Frame 52
	0xffffe3e3e3fbffff,					// <CHAR 245> => Spiral Animation Frame 53
	0xffffe3e3e3ffffff,					// <CHAR 246> => Spiral Animation Frame 54
	0xffffe3e3e7ffffff,					// <CHAR 247> => Spiral Animation Frame 55
	0xffffe3e7e7ffffff,					// <CHAR 248> => Spiral Animation Frame 56
	0xffffe7e7e7ffffff,					// <CHAR 249> => Spiral Animation Frame 57
	0xffffefe7e7ffffff,					// <CHAR 250> => Spiral Animation Frame 58
	0xffffffe7e7ffffff,					// <CHAR 251> => Spiral Animation Frame 59
	0xfffffff7e7ffffff,					// <CHAR 252> => Spiral Animation Frame 60
	0xfffffff7f7ffffff,					// <CHAR 253> => Spiral Animation Frame 61
	0xfffffff7ffffffff,					// <CHAR 254> => Spiral Animation Frame 62
	0xffffffffffffffff,					// <CHAR 255> => Spiral Animation Frame 63
};
const int IMAGES_LEN = sizeof(IMAGES) / 8;

#define SN74HC595_DIR	DDRD
#define SN74HC595_OUT	PORTD
#define SN74HC595_IN	PIND
#define SN74HC595_RED	PD0
#define SN74HC595_GRE	PD1
#define SN74HC595_BLU	PD2
#define SN74HC595_SHI	PD3
#define SN74HC595_STO	PD4
#define SN74HC595_CLE	PD5
#define SN74HC595_OEN	PD6

#define SN74HC138_DIR	DDRB
#define SN74HC138_OUT	PORTB
#define SN74HC138_IN	PINB
#define SN74HC138_AD0	PB0
#define SN74HC138_AD1	PB1
#define SN74HC138_AD2	PB2
#define SN74HC138_OEN	PB3

#define ANIMATION_START	192
#define ANIMATION_STOP	255

typedef union systemFlags_f {
	struct {
		bool		frameRefresh	: 1;
		bool		frameChange		: 1;
		uint32_t	ticksCounter	: 28;
		uint32_t	unusedBits		: 2;
	};
	uint32_t allFlags;
} systemFlags_f;

void dotMatrixChangeActiveRow(void);
void dotMatrixChangeFrame(void);

typedef struct dotMatrix_s {
	uint64_t	currentFrameRed;
	uint64_t	currentFrameGreen;
	uint64_t	currentFrameBlue;
	uint8_t		animationFrameIndex;
	uint8_t		animationFrameColor		: 3;
	uint8_t		currentRow				: 3;
	uint8_t		unusedBits				: 2;
} dotMatrix_s;

volatile systemFlags_f systemFlags;
volatile dotMatrix_s dotMatrixData;

int main(void)
{
	systemFlags.allFlags = 0;
	dotMatrixData.currentRow = 7;
	dotMatrixData.animationFrameIndex = 0;
	dotMatrixData.currentFrameRed = 0x0000000000000000;
	dotMatrixData.currentFrameGreen = 0x0000000000000000;
	dotMatrixData.currentFrameBlue = 0x0000000000000000;
	dotMatrixData.animationFrameColor = 0;

	UCSR0B = 0x00;

	// SN74HC595 port configuration
	setBit(SN74HC595_OUT, SN74HC595_OEN);	// OEN turned off (negative logic)
	setBit(SN74HC595_OUT, SN74HC595_RED);	// RED input turned off (negative logic)
	setBit(SN74HC595_OUT, SN74HC595_GRE);	// GREEN input turned off (negative logic)
	setBit(SN74HC595_OUT, SN74HC595_BLU);	// BLUE input turned off (negative logic)
	clrBit(SN74HC595_OUT, SN74HC595_SHI);	// SHIFT in LOW
	clrBit(SN74HC595_OUT, SN74HC595_STO);	// STORE in LOW
	setBit(SN74HC595_OUT, SN74HC595_CLE);	// CLEAR input turned off (negative logic)
	setMask(SN74HC595_DIR,					// Configure ports as OUTPUTS
	        (1 << SN74HC595_RED) | (1 << SN74HC595_GRE) | (1 << SN74HC595_BLU) |
	        (1 << SN74HC595_SHI) | (1 << SN74HC595_STO) | (1 << SN74HC595_CLE) |
	        (1 << SN74HC595_OEN),
	        0);
	clrBit(SN74HC595_OUT, SN74HC595_CLE);	// CLEAR pulse
	_delay_us(1);
	setBit(SN74HC595_OUT, SN74HC595_CLE);	// ...
	setBit(SN74HC595_OUT, SN74HC595_STO);	// STORE pulse
	_delay_us(1);
	clrBit(SN74HC595_OUT, SN74HC595_STO);	// ...

	// SN74HC138 port configuration
	clrBit(SN74HC138_OUT, SN74HC138_AD2);	// Select first row
	clrBit(SN74HC138_OUT, SN74HC138_AD1);	// ...
	clrBit(SN74HC138_OUT, SN74HC138_AD0);	// ...
	clrBit(SN74HC138_OUT, SN74HC138_OEN);	// OEN turned off (positive logic)
	setMask(SN74HC138_DIR,					// Configure ports as OUTPUTS
	        (1 << SN74HC138_AD2) | (1 << SN74HC138_AD1) | (1 << SN74HC138_AD0) |
	        (1 << SN74HC138_OEN),
	        0);

	// Timer 1 configuration
	TCCR0A = (1 << WGM01);
	TCCR0B = (0 << CS02) | (1 << CS01) | (1 << CS00);
	TCNT0 = 0x00;
	OCR0A = 250;
	OCR0B = 0;
	TIMSK0 = (1 << OCIE0A);
	TIFR0 = (1 << OCF0A);
	setBit(DDRB, PB5);


	sei();
	while(1) {

		if(systemFlags.frameChange) {
			dotMatrixChangeFrame();
			systemFlags.frameChange = false;
		}
		if(systemFlags.frameRefresh) {
			dotMatrixChangeActiveRow();
			systemFlags.frameRefresh = false;
		}
	}
}


ISR(TIMER0_COMPA_vect)
{
	systemFlags.frameRefresh = true;
	dotMatrixData.currentRow++;				// Increment active row
	systemFlags.ticksCounter++;
	if((systemFlags.ticksCounter % 50) == 0) {
		cplBit(PORTB, PB5);
		systemFlags.frameChange = true;
	}
}

void dotMatrixChangeActiveRow(void)
{
	// Turn row off
	clrBit(SN74HC138_OUT, SN74HC138_OEN);	// OE turned off on 74138 (positive logic)
	_delay_us(1);
	setBit(SN74HC595_OUT, SN74HC595_OEN);	// OE turned off on 74595 (negative logic)
	_delay_us(1);
	// Prepare next row
//	aux64 = (uint64_t *)(&(dotMatrixData.currentFrame));
//	aux64_ = (uint64_t)0x06063e6666663e00;
//	aux8 = *(&aux64 + dotMatrixData.currentRow);

	// Write next row at 74595
	for(uint8_t i = 0; i < 8; i++) {
		if(isBitSet(dotMatrixData.currentFrameRed, ((8 * dotMatrixData.currentRow) + i))) {
			clrBit(SN74HC595_OUT, SN74HC595_RED);
		} else {
			setBit(SN74HC595_OUT, SN74HC595_RED);
		}
		if(isBitSet(dotMatrixData.currentFrameGreen, ((8 * dotMatrixData.currentRow) + i))) {
			clrBit(SN74HC595_OUT, SN74HC595_GRE);
		} else {
			setBit(SN74HC595_OUT, SN74HC595_GRE);
		}
		if(isBitSet(dotMatrixData.currentFrameBlue, ((8 * dotMatrixData.currentRow) + i))) {
			clrBit(SN74HC595_OUT, SN74HC595_BLU);
		} else {
			setBit(SN74HC595_OUT, SN74HC595_BLU);
		}
		setBit(SN74HC595_OUT, SN74HC595_SHI);	// SHIFT pulse
		_delay_us(1);
		clrBit(SN74HC595_OUT, SN74HC595_SHI);	// ...
	}
	setBit(SN74HC595_OUT, SN74HC595_STO);	// STORE pulse
	_delay_us(1);
	clrBit(SN74HC595_OUT, SN74HC595_STO);	// ...
	// Select next row at 74138
	clrMask(SN74HC138_OUT, 0x07, SN74HC138_AD0);
	_delay_us(1);
	setMask(SN74HC138_OUT, dotMatrixData.currentRow, SN74HC138_AD0);
	_delay_us(1);
	// Turn row on
	setBit(SN74HC138_OUT, SN74HC138_OEN);	// OE turned on on 74138 (positive logic)
	_delay_us(1);
	clrBit(SN74HC595_OUT, SN74HC595_OEN);	// OE turned on on 74595 (negative logic)

	return;
}

void dotMatrixChangeFrame(void)
{
	uint64_t aux64 = 0;


	// Last frame achieved
	if((dotMatrixData.animationFrameIndex + ANIMATION_START) == ANIMATION_STOP) {
		dotMatrixData.animationFrameIndex = 0;
		if(dotMatrixData.animationFrameColor == 2) {
			dotMatrixData.animationFrameColor = 0;
		} else {
			dotMatrixData.animationFrameColor++;
		}
	} else {
		dotMatrixData.animationFrameIndex++;
	}
	// Gets frame from memory
	memcpy_P(&aux64, &IMAGES[ANIMATION_START + dotMatrixData.animationFrameIndex], 8);

	switch(dotMatrixData.animationFrameColor) {
	case 0:
		clrMask(dotMatrixData.currentFrameBlue, aux64, 0);
		dotMatrixData.currentFrameRed = aux64;
		break;
	case 1:
		clrMask(dotMatrixData.currentFrameRed, aux64, 0);
		dotMatrixData.currentFrameGreen = aux64;
		break;
	case 2:
		clrMask(dotMatrixData.currentFrameGreen, aux64, 0);
		dotMatrixData.currentFrameBlue = aux64;
		break;
	default:
		break;
	}

	return;
}

