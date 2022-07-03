/* =============================================================================
 * Project:		MPU9259 example application
 * File name:	main.cpp
 * Authors:		Leandro Schwarz
 * Date:		June 24, 2018
 * ========================================================================== */

// =============================================================================
// Include files
// =============================================================================

#include <globalDefines.hpp>
#include <circularBuffer8.hpp>
#include <externalInterrupt.hpp>
#include <mpu9250.hpp>
#include <spi.hpp>
#include <twi.hpp>
#include <usart0.hpp>
#include <avr/pgmspace.h>

// =============================================================================
// File exclusive - Constants
// =============================================================================

// Mpu9250 Device 1
#define SPI_IMU1_DDR		DDRD
#define SPI_IMU1_OUT		PORTD
#define SPI_IMU1_BIT		PD3
// Mpu9250 Device 2
#define SPI_IMU2_DDR		DDRD
#define SPI_IMU2_OUT		PORTD
#define SPI_IMU2_BIT		PD4
// Mpu9250 Device 3
#define SPI_IMU3_DDR		DDRD
#define SPI_IMU3_OUT		PORTD
#define SPI_IMU3_BIT		PD5
// Mpu9250 Device 4
#define SPI_IMU4_DDR		DDRD
#define SPI_IMU4_OUT		PORTD
#define SPI_IMU4_BIT		PD6
// Mpu9250 Device 5
#define SPI_IMU5_DDR		DDRD
#define SPI_IMU5_OUT		PORTD
#define SPI_IMU5_BIT		PD7
// Mpu9250 Device 6
#define SPI_IMU6_DDR		DDRC
#define SPI_IMU6_OUT		PORTC
#define SPI_IMU6_BIT		PC0
// Mpu9250 Device 7
#define SPI_IMU7_DDR		DDRC
#define SPI_IMU7_OUT		PORTC
#define SPI_IMU7_BIT		PC1
// Mpu9250 Device 8
#define SPI_IMU8_DDR		DDRC
#define SPI_IMU8_OUT		PORTC
#define SPI_IMU8_BIT		PC2

// =============================================================================
// Data Stored in Flash
// *INDENT-OFF*
// =============================================================================

/* NONE */

// =============================================================================
// *INDENT-ON*
// File exclusive - New data types
// =============================================================================

typedef union projectFlags_f {
	struct {
		uint8_t	dataReady	: 1;
		uint8_t	unusedBits	: 7;
	};
	uint8_t allFlags;
} projectFlags_f;

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

/* NONE */

// =============================================================================
// Global variables
// =============================================================================

volatile projectFlags_f projectFlags;

// =============================================================================
// Function declarations
// =============================================================================

void doNothing(void);
void wakeUpImu1(void);
void sleepDownImu1(void);
void wakeUpImu2(void);
void sleepDownImu2(void);
void wakeUpImu3(void);
void sleepDownImu3(void);
void wakeUpImu4(void);
void sleepDownImu4(void);
void wakeUpImu5(void);
void sleepDownImu5(void);
void wakeUpImu6(void);
void sleepDownImu6(void);
void wakeUpImu7(void);
void sleepDownImu7(void);
void wakeUpImu8(void);
void sleepDownImu8(void);

// =============================================================================
// Main function
// =============================================================================

// =============================================================================
// Main function ==============================================================-

int main(void)
{
	// =========================================================================
	// General purpose variables
	// =========================================================================
	int16_t accelBuf[3];
	int16_t gyroBuf[3];

	// =========================================================================
	// Variable initialization
	// =========================================================================

	// Clear project flags
	projectFlags.allFlags = 0;

	// =========================================================================
	// USART configuration
	// =========================================================================

	// Setup USART module
	// usart0.init();
	// usart0.setTransmitter(true);
	// usart0.setReceiver(true);
	// usart0.setDefault();
	Usart0 usart0(USART_BAUD_9600, USART_MODE_ASYNCHRONOUS, USART_FRAME_FORMAT_8_N_1);
	usart0.initialize();
	usart0.enableTransmitter();
	usart0.enableReceiver();
	usart0.stdio();

	// Show message
	_delay_ms(2000);
	printf_P(PSTR("MPU9250 - Example\r\r"));
	printf_P(PSTR("[ OK! ] - USART configured!\r"));

	// Enable Global Interrupts
	sei();

	// =========================================================================
	// GPIO configuration
	// =========================================================================

	// Chip select configuration
	setBit(SPI_IMU1_OUT, SPI_IMU1_BIT);
	setBit(SPI_IMU1_DDR, SPI_IMU1_BIT);
	setBit(SPI_IMU2_OUT, SPI_IMU2_BIT);
	setBit(SPI_IMU2_DDR, SPI_IMU2_BIT);
	setBit(SPI_IMU3_OUT, SPI_IMU3_BIT);
	setBit(SPI_IMU3_DDR, SPI_IMU3_BIT);
	setBit(SPI_IMU4_OUT, SPI_IMU4_BIT);
	setBit(SPI_IMU4_DDR, SPI_IMU4_BIT);
	setBit(SPI_IMU5_OUT, SPI_IMU5_BIT);
	setBit(SPI_IMU5_DDR, SPI_IMU5_BIT);
	setBit(SPI_IMU6_OUT, SPI_IMU6_BIT);
	setBit(SPI_IMU6_DDR, SPI_IMU6_BIT);
	setBit(SPI_IMU7_OUT, SPI_IMU7_BIT);
	setBit(SPI_IMU7_DDR, SPI_IMU7_BIT);
	setBit(SPI_IMU8_OUT, SPI_IMU8_BIT);
	setBit(SPI_IMU8_DDR, SPI_IMU8_BIT);

	printf_P(PSTR("[ OK! ] - GPIO configured!\r"));

	// =========================================================================
	// SPI configuration
	// =========================================================================

	// Setup SPI module
	spi0.init(1000000, 10);
	printf_P(PSTR("[ OK! ] - SPI configured to master mode 1 at 1 MHz!\r"));

	// =========================================================================
	// SPI configuration
	// =========================================================================

	// Start MPU9250 devices
	Mpu9250 imu1;
	Mpu9250 imu2;
	Mpu9250 imu3;
	Mpu9250 imu4;
	Mpu9250 imu5;
	Mpu9250 imu6;
	Mpu9250 imu7;
	Mpu9250 imu8;

	imu1.initSpi(&spi0, wakeUpImu1, sleepDownImu1);
	imu2.initSpi(&spi0, wakeUpImu2, sleepDownImu2);
	imu3.initSpi(&spi0, wakeUpImu3, sleepDownImu3);
	imu4.initSpi(&spi0, wakeUpImu4, sleepDownImu4);
	imu5.initSpi(&spi0, wakeUpImu5, sleepDownImu5);
	imu6.initSpi(&spi0, wakeUpImu6, sleepDownImu6);
	imu7.initSpi(&spi0, wakeUpImu7, sleepDownImu7);
	imu8.initSpi(&spi0, wakeUpImu8, sleepDownImu8);

	if (imu1.getLastError()) {
		printf_P(PSTR("[ERROR] - %d Device %d not found!\r"), imu1.getLastError(), 1);
	} else {
		printf_P(PSTR("[ OK! ] - %d Device %d found!\r"), imu1.getLastError(), 1);
	}
	if (imu2.getLastError()) {
		printf_P(PSTR("[ERROR] - %d Device %d not found!\r"), imu2.getLastError(), 2);
	} else {
		printf_P(PSTR("[ OK! ] - %d Device %d found!\r"), imu2.getLastError(), 2);
	}
	if (imu3.getLastError()) {
		printf_P(PSTR("[ERROR] - %d Device %d not found!\r"), imu3.getLastError(), 3);
	} else {
		printf_P(PSTR("[ OK! ] - %d Device %d found!\r"), imu3.getLastError(), 3);
	}
	if (imu4.getLastError()) {
		printf_P(PSTR("[ERROR] - %d Device %d not found!\r"), imu4.getLastError(), 4);
	} else {
		printf_P(PSTR("[ OK! ] - %d Device %d found!\r"), imu4.getLastError(), 4);
	}
	if (imu5.getLastError()) {
		printf_P(PSTR("[ERROR] - %d Device %d not found!\r"), imu5.getLastError(), 5);
	} else {
		printf_P(PSTR("[ OK! ] - %d Device %d found!\r"), imu5.getLastError(), 5);
	}
	if (imu6.getLastError()) {
		printf_P(PSTR("[ERROR] - %d Device %d not found!\r"), imu6.getLastError(), 6);
	} else {
		printf_P(PSTR("[ OK! ] - %d Device %d found!\r"), imu6.getLastError(), 6);
	}
	if (imu7.getLastError()) {
		printf_P(PSTR("[ERROR] - %d Device %d not found!\r"), imu7.getLastError(), 7);
	} else {
		printf_P(PSTR("[ OK! ] - %d Device %d found!\r"), imu7.getLastError(), 7);
	}
	if (imu8.getLastError()) {
		printf_P(PSTR("[ERROR] - %d Device %d not found!\r"), imu8.getLastError(), 8);
	} else {
		printf_P(PSTR("[ OK! ] - %d Device %d found!\r"), imu8.getLastError(), 8);
	}

	imu1.configAccel(AXIS_ALL, MPU9250_AFS_2G, MPU9250_ACCEL_SAMPLE_RATE_1_KHZ, MPU9250_ACCEL_DLPF_218_HZ);
	imu2.configAccel(AXIS_ALL, MPU9250_AFS_2G, MPU9250_ACCEL_SAMPLE_RATE_1_KHZ, MPU9250_ACCEL_DLPF_218_HZ);
	imu3.configAccel(AXIS_ALL, MPU9250_AFS_2G, MPU9250_ACCEL_SAMPLE_RATE_1_KHZ, MPU9250_ACCEL_DLPF_218_HZ);
	imu4.configAccel(AXIS_ALL, MPU9250_AFS_2G, MPU9250_ACCEL_SAMPLE_RATE_1_KHZ, MPU9250_ACCEL_DLPF_218_HZ);
	imu5.configAccel(AXIS_ALL, MPU9250_AFS_2G, MPU9250_ACCEL_SAMPLE_RATE_1_KHZ, MPU9250_ACCEL_DLPF_218_HZ);
	imu6.configAccel(AXIS_ALL, MPU9250_AFS_2G, MPU9250_ACCEL_SAMPLE_RATE_1_KHZ, MPU9250_ACCEL_DLPF_218_HZ);
	imu7.configAccel(AXIS_ALL, MPU9250_AFS_2G, MPU9250_ACCEL_SAMPLE_RATE_1_KHZ, MPU9250_ACCEL_DLPF_218_HZ);
	imu8.configAccel(AXIS_ALL, MPU9250_AFS_2G, MPU9250_ACCEL_SAMPLE_RATE_1_KHZ, MPU9250_ACCEL_DLPF_218_HZ);

	imu1.configGyro(AXIS_ALL, MPU9250_GFS_250DPS, MPU9250_GYRO_SAMPLE_RATE_1_KHZ, MPU9250_GYRO_DLPF_250_HZ);
	imu2.configGyro(AXIS_ALL, MPU9250_GFS_250DPS, MPU9250_GYRO_SAMPLE_RATE_1_KHZ, MPU9250_GYRO_DLPF_250_HZ);
	imu3.configGyro(AXIS_ALL, MPU9250_GFS_250DPS, MPU9250_GYRO_SAMPLE_RATE_1_KHZ, MPU9250_GYRO_DLPF_250_HZ);
	imu4.configGyro(AXIS_ALL, MPU9250_GFS_250DPS, MPU9250_GYRO_SAMPLE_RATE_1_KHZ, MPU9250_GYRO_DLPF_250_HZ);
	imu5.configGyro(AXIS_ALL, MPU9250_GFS_250DPS, MPU9250_GYRO_SAMPLE_RATE_1_KHZ, MPU9250_GYRO_DLPF_250_HZ);
	imu6.configGyro(AXIS_ALL, MPU9250_GFS_250DPS, MPU9250_GYRO_SAMPLE_RATE_1_KHZ, MPU9250_GYRO_DLPF_250_HZ);
	imu7.configGyro(AXIS_ALL, MPU9250_GFS_250DPS, MPU9250_GYRO_SAMPLE_RATE_1_KHZ, MPU9250_GYRO_DLPF_250_HZ);
	imu8.configGyro(AXIS_ALL, MPU9250_GFS_250DPS, MPU9250_GYRO_SAMPLE_RATE_1_KHZ, MPU9250_GYRO_DLPF_250_HZ);

	accelBuf[0] = 0;
	accelBuf[1] = 0;
	accelBuf[2] = 0;
	imu1.setAccelOffset(accelBuf, AXIS_ALL);
	imu2.setAccelOffset(accelBuf, AXIS_ALL);
	imu3.setAccelOffset(accelBuf, AXIS_ALL);
	imu4.setAccelOffset(accelBuf, AXIS_ALL);
	imu5.setAccelOffset(accelBuf, AXIS_ALL);
	imu6.setAccelOffset(accelBuf, AXIS_ALL);
	imu7.setAccelOffset(accelBuf, AXIS_ALL);
	imu8.setAccelOffset(accelBuf, AXIS_ALL);

	gyroBuf[0] = 0;
	gyroBuf[1] = 0;
	gyroBuf[2] = 0;
	imu1.setGyroOffset(gyroBuf, AXIS_ALL);
	imu2.setGyroOffset(gyroBuf, AXIS_ALL);
	imu3.setGyroOffset(gyroBuf, AXIS_ALL);
	imu4.setGyroOffset(gyroBuf, AXIS_ALL);
	imu5.setGyroOffset(gyroBuf, AXIS_ALL);
	imu6.setGyroOffset(gyroBuf, AXIS_ALL);
	imu7.setGyroOffset(gyroBuf, AXIS_ALL);
	imu8.setGyroOffset(gyroBuf, AXIS_ALL);

//	imu1.configIntPin(LOW, PUSH_PULL, false, true);
//	imu1.setIntDataReady(true);
//	int0.config(PORT_INPUT_PULL_UP, INT_SENSE_FALLING_EDGE);
//	int0.clearRequest();
//	int0.activate();

	/*
			// MPU9250 configuration
		#define INT_PIN_P20     (0x202)
		#define INT_EXIT_LPM0   (1)
			int_param.cb = doNothing;
			int_param.pin = INT_PIN_P20;
			int_param.lp_exit = INT_EXIT_LPM0;
			int_param.active_low = 1;
			setBit(PORTD, PD2);
			_delay_us(100);
			clrBit(PORTD, PD2);


			mpu9250CreateDevice(&device, MPU9250_COMM_INTERFACE_TWI);
			mpu9250TwiInit(&device, MPU9250_TWI_SLAVE_ADDRESS_0x68);
			mpu9250SetGyroFullScale(&device, MPU9250_GYRO_FULL_SCALE_2000_DPS);

		//	result = mpu_init(&int_param);
			while (1);
			if (result) {
				printf("Could not initialize gyro.\r");
				while (1);
			}
		*/
	while (1) {
		imu1.getAccelData(accelBuf, AXIS_ALL);
		imu1.getGyroData(gyroBuf, AXIS_ALL);
		printf("1 <> %6d %6d %6d\r", accelBuf[0], accelBuf[1], accelBuf[2]);
		imu2.getAccelData(accelBuf, AXIS_ALL);
		imu2.getGyroData(gyroBuf, AXIS_ALL);
		printf("2 <> %6d %6d %6d\r", accelBuf[0], accelBuf[1], accelBuf[2]);
		imu3.getAccelData(accelBuf, AXIS_ALL);
		imu3.getGyroData(gyroBuf, AXIS_ALL);
		printf("3 <> %6d %6d %6d\r", accelBuf[0], accelBuf[1], accelBuf[2]);
		imu4.getAccelData(accelBuf, AXIS_ALL);
		imu4.getGyroData(gyroBuf, AXIS_ALL);
		printf("4 <> %6d %6d %6d\r", accelBuf[0], accelBuf[1], accelBuf[2]);
		imu5.getAccelData(accelBuf, AXIS_ALL);
		imu5.getGyroData(gyroBuf, AXIS_ALL);
		printf("5 <> %6d %6d %6d\r", accelBuf[0], accelBuf[1], accelBuf[2]);
		imu6.getAccelData(accelBuf, AXIS_ALL);
		imu6.getGyroData(gyroBuf, AXIS_ALL);
		printf("6 <> %6d %6d %6d\r", accelBuf[0], accelBuf[1], accelBuf[2]);
		imu7.getAccelData(accelBuf, AXIS_ALL);
		imu7.getGyroData(gyroBuf, AXIS_ALL);
		printf("7 <> %6d %6d %6d\r", accelBuf[0], accelBuf[1], accelBuf[2]);
		imu8.getAccelData(accelBuf, AXIS_ALL);
		imu8.getGyroData(gyroBuf, AXIS_ALL);
		printf("8 <> %6d %6d %6d\r\r", accelBuf[0], accelBuf[1], accelBuf[2]);

		_delay_ms(500);
	}

	return 0;
}

// =============================================================================
// Interruption handlers ======================================================-

ISR(INT0_vect)
{
	projectFlags.dataReady = true;
}

// =============================================================================
// Function definitions ========================================================

void doNothing(void)
{
	return;
}

void wakeUpImu1(void)
{
	clrBit(SPI_IMU1_OUT, SPI_IMU1_BIT);
	return;
}

void sleepDownImu1(void)
{
	setBit(SPI_IMU1_OUT, SPI_IMU1_BIT);
	return;
}

void wakeUpImu2(void)
{
	clrBit(SPI_IMU2_OUT, SPI_IMU2_BIT);
	return;
}

void sleepDownImu2(void)
{
	setBit(SPI_IMU2_OUT, SPI_IMU2_BIT);
	return;
}

void wakeUpImu3(void)
{
	clrBit(SPI_IMU3_OUT, SPI_IMU3_BIT);
	return;
}

void sleepDownImu3(void)
{
	setBit(SPI_IMU3_OUT, SPI_IMU3_BIT);
	return;
}

void wakeUpImu4(void)
{
	clrBit(SPI_IMU4_OUT, SPI_IMU4_BIT);
	return;
}

void sleepDownImu4(void)
{
	setBit(SPI_IMU4_OUT, SPI_IMU4_BIT);
	return;
}

void wakeUpImu5(void)
{
	clrBit(SPI_IMU5_OUT, SPI_IMU5_BIT);
	return;
}

void sleepDownImu5(void)
{
	setBit(SPI_IMU5_OUT, SPI_IMU5_BIT);
	return;
}

void wakeUpImu6(void)
{
	clrBit(SPI_IMU6_OUT, SPI_IMU6_BIT);
	return;
}

void sleepDownImu6(void)
{
	setBit(SPI_IMU6_OUT, SPI_IMU6_BIT);
	return;
}

void wakeUpImu7(void)
{
	clrBit(SPI_IMU7_OUT, SPI_IMU7_BIT);
	return;
}

void sleepDownImu7(void)
{
	setBit(SPI_IMU7_OUT, SPI_IMU7_BIT);
	return;
}

void wakeUpImu8(void)
{
	clrBit(SPI_IMU8_OUT, SPI_IMU8_BIT);
	return;
}

void sleepDownImu8(void)
{
	setBit(SPI_IMU8_OUT, SPI_IMU8_BIT);
	return;
}
