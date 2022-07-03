/* =============================================================================
 * Project:			Formula UFSC
 * File name:		main.cpp
 * Module:			betaLogger
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// Problemas no codigo anterior:
//		Variáveis globais (Grande quantidade e em grande tamanho)
//		Variáveis globais (Grande quantidade e/ou variáveis muito grandes)
//		Rotinas de tratamento de iterrupção muito longas
//		Timeouts muito longos

// =============================================================================
// Hardware pin allocation
// =============================================================================

// -----------------------------------------------------------------------------
// Pin id		Module		Pin name
// -----------------------------------------------------------------------------
// PA0			ADC1		ADC_SUS_FL
// PA1			ADC1		ADC_SUS_FR
// PA2			ADC1		ADC_SUS_RL
// PA3			ADC1		ADC_SUS_RR
// PA4			ADC1		ADC_BRAKE
// PA5			ADC1		ADC_THROTTLE
// PA6			ADC1		ADC_STEERING
// PA7			ADC1		ADC_BATTERY
// PA8			GPIO		LED_R
// PA9			GPIO		LED_G
// PA10			GPIO		LED_B
// PA11			USB			USB_DM
// PA12			USB			USB_DP
// PA13			SYS			SYS_JTMS_SWIO
// PA14			SYS			SYS_JTCK_SWCLK
// PA15			GPIO		DEBUG1
// PB0			.			.
// PB1			.			.
// PB2			GPIO		BOOT1
// PB3			GPIO		DEBUG2
// PB4			GPIO		DEBUG3
// PB5			GPIO		DEBUG4
// PB6			USART1		USART1_TX
// PB7			USART1		USART1_RX
// PB8			I2C1		I2C1_SCL
// PB9			I2C1		I2C1_SDA
// PB10			USART3		USART3_TX
// PB11			USART3		USART3_RX
// PB12			GPIO		SPI2_CS_SD
// PB13			SPI2		SPI2_SCK
// PB14			SPI2		SPI2_MISO
// PB15			SPI2		SPI2_MOSI
// PC13			GPIO		NLED
// PC14			RCC			RCC_OSC32_IN
// PC15			RCC			RCC_OSC32_OUT
// PD0			RCC			RCC_OSC_IN
// PD1			RCC			RCC_OSC_OUT
// -----------------------------------------------------------------------------

// =============================================================================
// Include files
// =============================================================================

// STM32CubeMX headers
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "gpio.h"

// FunSAPE library headers
#include "funsapeLibGlobalDefines.hpp"
#include "funsapeLibDebug.hpp"
#include "funsapeLibCircularBuffer32.hpp"
#include "funsapeLibCircularBuffer16.hpp"
#include "funsapeLibCircularBuffer8.hpp"

// Standard C library headers
#include <string.h>
#include <stdio.h>

// FatFs library headers
#include "ff.h"

// =============================================================================
// File exclusive - Constant definitions
// =============================================================================

#define I2C1_BUFFER_SIZE				14
#define USART1_RX_BUFFER_SIZE			50
#define USART1_TX_BUFFER_SIZE			50
#define USART3_RX_BUFFER_SIZE			2
#define USART3_TX_BUFFER_SIZE			50
#define MPU9250_WHO_AM_I				0x71
#define MPU9250_I2C_BASE_SLAVE_ADDRESS	0x68
#define MPU9250_I2C_SLAVE_ADDRESS		(MPU9250_I2C_BASE_SLAVE_ADDRESS << 1)
#define MPU9250_REG_WHO_AM_I			0x75
#define MPU9250_REG_CONFIG				0x1A
#define MPU9250_REG_GYRO_CONFIG			0x1B
#define MPU9250_REG_ACCEL_CONFIG		0x1C
#define MPU9250_REG_ACCEL_CONFIG2		0x1D
#define MPU9250_REG_ACCEL_XOUT_H		0x3B

// =============================================================================
// File exclusive - New data types
// =============================================================================

// NONE

// =============================================================================
// File exclusive - Macro-functions definitions
// =============================================================================

#define ledToggle()						do{HAL_GPIO_TogglePin(NLED_GPIO_Port, NLED_Pin);}while(0)
#define ledTurnOff()					do{HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_SET);}while(0)
#define ledTurnOn()						do{HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_RESET);}while(0)

// =============================================================================
// File exclusive - Private functions declarations
// =============================================================================

// STM32CubeMX functions
void SystemClock_Config(void);

// Generic functions
void ledBlinkError(uint8_t errorCode);
bool usbTransmit(uint8_t *bufData, uint16_t bufSize, uint16_t timeOut);

// =============================================================================
// Global variables
// =============================================================================

CircularBuffer32 timerCircBuffer(200);
CircularBuffer16 adcCircBuffer(200);
CircularBuffer8 mpuCircBuffer(200);
CircularBuffer8 usbRxCircBuffer(200);
uint16_t adcDmaBuffer[ADC_BUFFER_SIZE_HWORDS];
uint8_t i2c1Buffer[I2C1_BUFFER_SIZE];

// uint32_t resultado;
// char textTxBuffer[500];
// char timeBuffer[11];
// uint8_t gpsReady, tenth;
// uint32_t seconds, millis, timeAux;
// uint8_t sdBufferA[20][84];
// uint8_t sdBufferB[20][84];

// uint16_t buffer[50][14];
// uint8_t iMpu, iAdc, iTim, adcReady, mpuReady, timReady, nBuffer;

// uint32_t xAcc, yAcc, zAcc, xGyr, yGyr, zGyr;

// =============================================================================
// Main function
// =============================================================================

int main(void)
{
    // -------------------------------------------------------------------------
    // Local variables
    // -------------------------------------------------------------------------

//	char				usart1RxBuffer[USART1_RX_BUFFER_SIZE];
    char				usart1TxBuffer[USART1_TX_BUFFER_SIZE];
    char				usart3RxBuffer[USART3_RX_BUFFER_SIZE];
//	char				usart3TxBuffer[USART3_TX_BUFFER_SIZE];
    FRESULT				fatfsStatus		= FR_OK;
    FATFS				sdCard;
    FIL					dataFile;
    char				sdCardPath[4];

    // -------------------------------------------------------------------------
    // HAL library initialization
    // -------------------------------------------------------------------------

    HAL_Init();

    // -------------------------------------------------------------------------
    // MCU clock configuration
    // -------------------------------------------------------------------------

    SystemClock_Config();

    // -------------------------------------------------------------------------
    // MCU peripheral initial configuration
    // -------------------------------------------------------------------------

    MX_GPIO_Init();
    MX_DMA_Init();
    MX_TIM3_Init();
    MX_USART1_UART_Init();
    MX_USART3_UART_Init();
    MX_ADC1_Init();
    MX_RTC_Init();
    MX_I2C1_Init();
    MX_SPI2_Init();
//	MX_USB_DEVICE_Init();

    debug.init(&huart1, false, false);

    // -------------------------------------------------------------------------
    // USB test message
    // -------------------------------------------------------------------------

//	strcpy(usart1TxBuffer, "betaLogger USB test\r");
//	if(!usbTransmit((uint8_t *)usart1TxBuffer, strlen(usart1TxBuffer), USART1_DEFAULT_TIMEOUT)) {
//		ledBlinkError(15);				// ERROR15
//	}

    // -------------------------------------------------------------------------
    // USART1 test message
    // -------------------------------------------------------------------------

    strcpy(usart1TxBuffer, "betaLogger USART1 test\r");
    if(HAL_UART_Transmit(&huart1, (uint8_t *)usart1TxBuffer, strlen(usart1TxBuffer), USART1_DEFAULT_TIMEOUT) != HAL_OK)
    {
        debugMessage(1);				// ERROR1
    }

    // -------------------------------------------------------------------------
    // SD card configuration
    // -------------------------------------------------------------------------

    // Mounting SD card
    // fatfsStatus = f_mount(&sdCard, (TCHAR const *)sdCardPath, (BYTE)0);
    // if(fatfsStatus != FR_OK) {
    // 	ledBlinkError(2);				// ERROR2
    // }

    // Creating a new file
    do
    {
        char fileName[13];
        uint16_t fileNumber = 0;
        char auxStr[150];
        UINT bytesWritten;
        UINT bytesToWrite;
        // Searching for last file created
        do
        {
            sprintf(fileName, "file%04d.dlf", ++fileNumber);
            fatfsStatus = f_open(&dataFile, (const TCHAR *)fileName, (BYTE)(FA_OPEN_EXISTING | FA_READ));
        }
        while(fatfsStatus == FR_OK);
        // Create the new file
        fatfsStatus = f_open(&dataFile, (const TCHAR *)fileName, (BYTE)(FA_OPEN_ALWAYS | FA_WRITE));
        if(fatfsStatus != FR_OK)
        {
            ledBlinkError(3);			// ERROR3
        }
        // Writting the file header
        strcpy(auxStr, "#V2\r#DATASTART\rDatalog Time; GPS Latitude; GPS Longitude; knots;Ax;Ay;Az;Gx;Gy;Gz;\rseg.; deg; deg; knots;un.;un.;un.;un.;un.;un.;\r");
        bytesToWrite = (UINT)sizeof(auxStr);
        fatfsStatus = f_write(&dataFile, (const void *)auxStr, bytesToWrite, &bytesWritten);
        if((fatfsStatus != FR_OK) || (bytesToWrite != bytesWritten))
        {
            ledBlinkError(4);			// ERROR4
        }
        fatfsStatus = f_sync(&dataFile);
        if(fatfsStatus != FR_OK)
        {
            ledBlinkError(5);			// ERROR5
        }
    }
    while(0);

    // -------------------------------------------------------------------------
    // ADC configuration
    // -------------------------------------------------------------------------

    // ADC calibration
    if(HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK)
    {
        debugMessage(6);				// ERROR6
    }
    if(HAL_ADC_Start_DMA(&hadc1, (uint32_t *) adcDmaBuffer, ADC_BUFFER_SIZE_HWORDS) != HAL_OK)
    {
        debugMessage(7);				// ERROR7
    }

    // -------------------------------------------------------------------------
    // TIMER3 configuration
    // -------------------------------------------------------------------------

    if(HAL_TIM_Base_Start_IT(&htim3) != HAL_OK)
    {
        debugMessage(8);				// ERROR8
    }

    // -------------------------------------------------------------------------
    // USART3 configuration
    // -------------------------------------------------------------------------

    if(HAL_UART_Receive_DMA(&huart3, (uint8_t *)usart3RxBuffer, USART3_RX_BUFFER_SIZE) != HAL_OK)
    {
        debugMessage(9);				// ERROR9
    }

    // -------------------------------------------------------------------------
    // MPU-9250 configuration
    // -------------------------------------------------------------------------

    // Checking Who Am I register
    HAL_StatusTypeDef halStatus = HAL_I2C_Mem_Read(&hi2c1, MPU9250_I2C_SLAVE_ADDRESS, MPU9250_REG_WHO_AM_I, I2C_MEMADD_SIZE_8BIT, i2c1Buffer, 1, I2C1_DEFAULT_TIMEOUT);
    if(halStatus != HAL_OK)
    {
        debugMessage(halStatus);				// ERROR10
    }
    else if(i2c1Buffer[1] != MPU9250_WHO_AM_I)
    {
        debugMessage(11);				// ERROR11
    }

    // Sending configuration info
    i2c1Buffer[0] = 0x00;
    i2c1Buffer[1] = 0x03;				// Fs = 32 kHz
    i2c1Buffer[2] = 0x08;				// 8g
    i2c1Buffer[3] = 0x00;				// Fs = 4 kHz
    if(HAL_I2C_Mem_Write(&hi2c1, MPU9250_I2C_SLAVE_ADDRESS, MPU9250_REG_CONFIG, I2C_MEMADD_SIZE_8BIT, i2c1Buffer, 4, I2C1_DEFAULT_TIMEOUT) != HAL_OK)
    {
        debugMessage(12);				// ERROR12
    }

    // -------------------------------------------------------------------------
    // Main loop
    // -------------------------------------------------------------------------

    while(1)
    {
        // strcpy(usart1TxBuffer, "betaLogger USART1 test\r");
        // if(HAL_UART_Transmit(&huart1, (uint8_t *)usart1TxBuffer, strlen(usart1TxBuffer), USART1_DEFAULT_TIMEOUT) != HAL_OK) {
        // 	ledBlinkError(1);				// ERROR1
        // }
        // HAL_Delay(2000);
    }








    // for(int i = 0; i < 20; i++) {
    // 	for(int j = 0; j < 82; j++) {
    // 		sdBufferA[i][j] = 0x30;
    // 	}
    // 	sdBufferA[i][10] = 0x41;
    // 	sdBufferA[i][22] = 0x41;
    // 	sdBufferA[i][34] = 0x41;
    // 	sdBufferA[i][40] = 0x41;
    // 	sdBufferA[i][47] = 0x41;
    // 	sdBufferA[i][54] = 0x41;
    // 	sdBufferA[i][61] = 0x41;
    // 	sdBufferA[i][68] = 0x41;
    // 	sdBufferA[i][75] = 0x41;
    // 	sdBufferA[i][82] = 0x0D;
    // 	sdBufferA[i][83] = 0x0A;
    // }

    // for(int i = 0; i < 20; i++) {
    // 	for(int j = 0; j < 84; j++) {
    // 		sdBufferB[i][j] = sdBufferA[i][j];
    // 	}
    // }

    // millis = 0;
    // adcReady = 0;
    // mpuReady = 0;
    // gpsReady = 0;
    // timReady = 0;
    // nBuffer = 1;

    // /* USER CODE END 2 */

    // /* Infinite loop */
    // /* USER CODE BEGIN WHILE */
    // while(1) {
    // 	// if (adcReady == 1 && mpuReady == 1) {
    // 	if(mpuReady == 1 && timReady == 1) {
    // 		if(nBuffer == 1) {
    // 			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
    // 			nBuffer = 2;
    // 			adcReady = 0;
    // 			mpuReady = 0;
    // 			gpsReady = 0;
    // 			timReady = 0;
    // 			f_write(&MyFile, sdBufferA, sizeof(sdBufferA), (UINT *)&byteswritten);
    // 			f_sync(&MyFile);
    // 			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
    // 		} else if(nBuffer == 2) {
    // 			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
    // 			nBuffer = 1;
    // 			adcReady = 0;
    // 			mpuReady = 0;
    // 			gpsReady = 0;
    // 			timReady = 0;
    // 			f_write(&MyFile, sdBufferB, sizeof(sdBufferB), (UINT *)&byteswritten);
    // 			f_sync(&MyFile);
    // 			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
    // 		}
    // 		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    // 	}

    // 	/* USER CODE END WHILE */

    // 	/* USER CODE BEGIN 3 */
    // }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    // Initializes the CPU, AHB and APB busses clocks
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    // Initializes the CPU, AHB and APB busses clocks
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_ADC
                                         | RCC_PERIPHCLK_USB;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static uint32_t timerCounter = 0;
    if(HAL_I2C_Mem_Read_IT(&hi2c1, MPU9250_I2C_SLAVE_ADDRESS, MPU9250_REG_ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT, i2c1Buffer, 14))
    {
        debugMessage(13);				// ERROR13
    }
    timerCircBuffer.push(timerCounter++);
    debugMessage(0);				// ERROR0
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    mpuCircBuffer.pushBuffer(i2c1Buffer, 14);
    debugMessage(0);				// ERROR0
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    debugMessage(0);				// ERROR0
    // HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);

    // if(gpsReady == 0) {
    // 	if(rawGps[1] == 0x47 && rawGps[17] == 0x41) {

    // 		if(nBuffer == 1) {
    // 			sdBufferA[0][11] = 0x2D;				//-
    // 			for(int j = 12; j < 22; j++) { 		//latitude
    // 				sdBufferA[0][j] = rawGps[j + 7];
    // 			}
    // 			sdBufferA[0][22] = 0x41;				//A
    // 			sdBufferA[0][23] = 0x2D;				//-
    // 			for(int j = 24; j < 34; j++) { 		//longitude
    // 				sdBufferA[0][j] = rawGps[j + 9];
    // 			}
    // 			sdBufferA[0][34] = 0x41;				//A
    // 			for(int j = 35; j < 40; j++) { 		//speed
    // 				sdBufferA[0][j] = rawGps[j + 11];
    // 			}
    // 			sdBufferA[0][40] = 0x41;				//A

    // 			for(int i = 1; i < 20; i++) {
    // 				for(int j = 11; j < 41; j++) {
    // 					sdBufferA[i][j] = sdBufferA[0][j];
    // 				}
    // 			}

    // 			for(int i = 0; i < 20; i++) {
    // 				for(int j = 11; j < 41; j++) {
    // 					sdBufferB[i][j] = sdBufferA[i][j];
    // 				}
    // 			}
    // 		} else if(nBuffer == 2) {
    // 			sdBufferB[0][11] = 0x2D;				//-
    // 			for(int j = 12; j < 22; j++) { 		//latitude
    // 				sdBufferB[0][j] = rawGps[j + 7];
    // 			}
    // 			sdBufferB[0][22] = 0x41;				//A
    // 			sdBufferB[0][23] = 0x2D;				//-
    // 			for(int j = 24; j < 34; j++) { 		//longitude
    // 				sdBufferB[0][j] = rawGps[j + 9];
    // 			}
    // 			sdBufferB[0][34] = 0x41;				//A
    // 			for(int j = 35; j < 40; j++) { 		//speed
    // 				sdBufferB[0][j] = rawGps[j + 11];
    // 			}
    // 			sdBufferB[0][40] = 0x41;				//A

    // 			for(int i = 1; i < 20; i++) {
    // 				for(int j = 11; j < 41; j++) {
    // 					sdBufferB[i][j] = sdBufferB[0][j];
    // 				}
    // 			}

    // 			for(int i = 0; i < 20; i++) {
    // 				for(int j = 11; j < 41; j++) {
    // 					sdBufferB[i][j] = sdBufferA[i][j];
    // 				}
    // 			}
    // 		}
    // 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
    // 	} else {
    // 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
    // 	}
    // 	gpsReady = 1;
    // 	HAL_UART_Receive_DMA(&huart3, (uint8_t *)&rawGps, 69);
    // }
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
    debugMessage(0);				// ERROR0
    adcCircBuffer.pushBuffer(&adcDmaBuffer[0], (ADC_BUFFER_SIZE_BYTES / 2));
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    debugMessage(0);				// ERROR0
    adcCircBuffer.pushBuffer(&adcDmaBuffer[ADC_BUFFER_SIZE_BYTES / 2], (ADC_BUFFER_SIZE_BYTES / 2));
}

void ledBlinkError(uint8_t errorCode)
{
    while(1)
    {
        ledTurnOn();
        HAL_Delay(20 * errorCode);
        ledTurnOff();
        HAL_Delay(200);
    }
    return;
}

void Error_Handler(void)
{
    return;
}

// USB WRAPPER workaround
#undef CDC_Receive_FS
#include "usbd_cdc.h"
extern USBD_HandleTypeDef hUsbDeviceFS;

bool usbTransmit(uint8_t *bufData, uint16_t bufSize, uint16_t timeOut)
{
    uint32_t tickstart = 0U;

    tickstart = HAL_GetTick();

    while(CDC_Transmit_FS(bufData, bufSize) == USBD_BUSY)
    {
        // Check for the Timeout
        if(timeOut != HAL_MAX_DELAY)
        {
            if((timeOut == 0U) || ((HAL_GetTick() - tickstart) > timeOut))
            {
                ledBlinkError(17);
                return false;
            }
        }
        // Try send again (just continue in loop)
    }

    return true;
}


int8_t CDC_Receive_FS(uint8_t *Buf, uint32_t *Len)
{
    for(uint32_t i = 0; i < *Len; i++)
    {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        usbRxCircBuffer.push(Buf[i]);
    }

    USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
    USBD_CDC_ReceivePacket(&hUsbDeviceFS);
    return (USBD_OK);
}
