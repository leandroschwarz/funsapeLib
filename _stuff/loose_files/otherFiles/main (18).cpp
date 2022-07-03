/*
 * MPU9250_GPDSEpp_AVR.cpp
 *
 * Created: 26/05/2018 14:38:53
 * Author : Schwarz
 */

#include "gpdse1001/globalConfigs.hpp"
#include "gpdse1001/globalDefines.hpp"
#include "gpdse1001/twiMaster.hpp"
#include "gpdse1001/usart0.hpp"
#include "gpdse1001/devices/mpu9250.hpp"


typedef enum fixedPointFormat_e {
	FIXED_POINT_PLAIN			= 0,
	FIXED_POINT_Q1				= 1,
	FIXED_POINT_Q2				= 2,
	FIXED_POINT_Q3				= 3,
	FIXED_POINT_Q4				= 4,
	FIXED_POINT_Q5				= 5,
	FIXED_POINT_Q6				= 6,
	FIXED_POINT_Q7				= 7,
	FIXED_POINT_Q8				= 8,
	FIXED_POINT_Q9				= 9,
	FIXED_POINT_Q10				= 10,
	FIXED_POINT_Q11				= 11,
	FIXED_POINT_Q12				= 12,
	FIXED_POINT_Q13				= 13,
	FIXED_POINT_Q14				= 14,
	FIXED_POINT_Q15				= 15,
	FIXED_POINT_Q16				= 16,
} fixedPointFormat_e;

uint32_t modUint32(uint32_t number, uint32_t * fractional, uint8_t precision, fixedPointFormat_e format);
int32_t modInt32(int32_t number, int32_t * fractional, char * signal, uint8_t precision, fixedPointFormat_e format);



int main(void)
{
	// Local variables
	Mpu9250 myImu(&twiMaster);
	mpu9250Error_e myImuError;
	Usart0 terminal(USART_BAUD_57600);
	uint32_t tempRaw = 0;
	uint32_t auxUInteger = 0;
	uint32_t auxUFractional = 0;
	int32_t auxInteger = 0;
	int32_t auxFractional = 0;
	int32_t accelBuffG[3] = {0, 0, 0};
	int16_t accelBuffRaw[3] = {0, 0, 0};
	int16_t gyroBuffRaw[3] = {0, 0, 0};
	int32_t gyroBuffDps[3] = {0, 0, 0};
	char auxSignal;

	// TWI configuration
	twiMaster.setClockSpeed(10000);

	// Enable global interruptions
	sei();

	// USART configuration
	terminal.initialize();
	terminal.enableTransmitter();
	terminal.stdio();
	printf("USART test\r");

	setBit(DDRB, PB5);

	// MPU9250 configuration
	setBit(PORTC, PC4);
	setBit(PORTC, PC5);
	myImuError = myImu.initialize();
	if(myImuError) {
		setBit(PORTB, PB5);
		systemHalt();
	}

	while(1) {
		myImu.getTemp(&tempRaw);
		myImu.getAccel(accelBuffG, AXIS_ALL, accelBuffRaw);
		myImu.getGyro(gyroBuffDps, AXIS_ALL, gyroBuffRaw);

		auxUInteger = modUint32((uint32_t)tempRaw, &auxUFractional, 4, FIXED_POINT_Q9);
		printf("T = %u.%u(%u) ", (uint16_t)auxUInteger, (uint16_t)auxUFractional, (uint16_t)tempRaw);

		auxInteger = modInt32(accelBuffG[0], &auxFractional, &auxSignal, 4, FIXED_POINT_Q9);
		printf("A = %c%ld.%ld(%ld;%d) ", auxSignal, auxInteger, auxFractional, accelBuffG[0], accelBuffRaw[0]);
		auxInteger = modInt32(accelBuffG[1], &auxFractional, &auxSignal, 4, FIXED_POINT_Q9);
		printf("%c%ld.%ld(%ld;%d) ", auxSignal, auxInteger, auxFractional, accelBuffG[1], accelBuffRaw[1]);
		auxInteger = modInt32(accelBuffG[2], &auxFractional, &auxSignal, 4, FIXED_POINT_Q9);
		printf("%c%ld.%ld(%ld;%d) ", auxSignal, auxInteger, auxFractional, accelBuffG[2], accelBuffRaw[2]);

		auxInteger = modInt32(gyroBuffDps[0], &auxFractional, &auxSignal, 4, FIXED_POINT_Q9);
		printf("G = %c%ld.%ld(%ld;%d) ", auxSignal, auxInteger, auxFractional, gyroBuffDps[0], gyroBuffRaw[0]);
		auxInteger = modInt32(gyroBuffDps[1], &auxFractional, &auxSignal, 4, FIXED_POINT_Q9);
		printf("%c%ld.%ld(%ld;%d) ", auxSignal, auxInteger, auxFractional, gyroBuffDps[1], gyroBuffRaw[1]);
		auxInteger = modInt32(gyroBuffDps[2], &auxFractional, &auxSignal, 4, FIXED_POINT_Q9);
		printf("%c%ld.%ld(%ld;%d)\r", auxSignal, auxInteger, auxFractional, gyroBuffDps[2], gyroBuffRaw[2]);
		_delay_ms(500);
	}

	return 0;
}


uint32_t modUint32(uint32_t number, uint32_t * fractional, uint8_t precision, fixedPointFormat_e format)
{
	uint64_t auxNumber;
	uint32_t auxMultiplier = 1;
	uint32_t auxDivider = 1;

	// Calculates precision multiplier
	while(precision > 0) {
		auxMultiplier *= 10;
		precision--;
	}
	// Calculates Q format divider
	while(format > 0) {
		auxDivider <<= 1;
		format = (fixedPointFormat_e)((uint8_t)(format) - 1);
	}

	auxNumber = (uint64_t)number;
	auxNumber *= (uint64_t)auxMultiplier;
	auxNumber /= (uint64_t)auxDivider;
	*fractional = (uint32_t)(auxNumber % (uint64_t)auxMultiplier);

	return (uint32_t)(auxNumber / (uint64_t)auxMultiplier);
}

int32_t modInt32(int32_t number, int32_t * fractional, char * signal, uint8_t precision, fixedPointFormat_e format)
{
	if(number < 0) {
		* signal = '-';
		return (int32_t)modUint32((uint32_t)(-number), (uint32_t *)fractional, precision, format);
	}

	* signal = '+';
	return (int32_t)modUint32((uint32_t)(number), (uint32_t *)fractional, precision, format);
}
