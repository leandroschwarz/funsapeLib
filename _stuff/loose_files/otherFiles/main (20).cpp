/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file		   : main.c
  * @brief		  : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *							 www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#define __DEBUG_STOP_ON_FIRST_ERROR		1
#define __DEBUG_HIDE_SUCCESS_MESSAGES	1

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "crc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "..\FunSAPE\funsapeLibGlobalDefines.hpp"
#include "..\FunSAPE\util\funsapeLibCircularBuffer8.hpp"
#include "..\FunSAPE\util\funsapeLibNmeaParser.hpp"
#include "..\Inc\functions.hpp"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef union blinkWarning_s
{
    uint32_t allFields;
    struct
    {
        uint32_t	mixedColor							: 3;
        uint32_t	mixedFastBlinks						: 3;
        uint32_t	mixedSlowBlinks						: 3;
        uint32_t	redFastBlinks						: 3;
        uint32_t	redSlowBlinks						: 3;
        uint32_t	greenFastBlinks						: 3;
        uint32_t	greenSlowBlinks						: 3;
        uint32_t	blueFastBlinks						: 3;
        uint32_t	blueSlowBlinks						: 3;
        uint32_t	unusedBits							: 5;
    };
} blinkWarning_s;

typedef union systemFlags_t
{
    uint32_t allFlags;
    struct
    {
        bool		adcFullComplete						: 1;
        bool		adcHalfComplete						: 1;
        bool		tim3PeriodElapsed					: 1;
        bool		uart1RxFullComplete					: 1;
        bool		uart1RxHalfComplete					: 1;
        bool		uart1TxFullComplete					: 1;
        bool		uart1TxHalfComplete					: 1;
        bool		uart2RxFullComplete					: 1;
        bool		uart2RxHalfComplete					: 1;
        bool		uart2TxFullComplete					: 1;
        bool		uart2TxHalfComplete					: 1;
        bool		printStatus							: 1;
        uint32_t	unusedFlags							: 20;
    };
} systemFlags_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define ledTurnOn()						HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_RESET)
#define ledTurnOff()					HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_SET)
#define ledToggle()						HAL_GPIO_TogglePin(NLED_GPIO_Port, NLED_Pin)
#define redLedTurnOn()					HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET)
#define redLedTurnOff()					HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET)
#define redLedBlinkFast()				do{redLedTurnOn();HAL_Delay(250);redLedTurnOff();HAL_Delay(250);}while(0)
#define redLedBlinkSlow()				do{redLedTurnOn();HAL_Delay(500);redLedTurnOff();HAL_Delay(500);}while(0)
#define greenLedTurnOn()				HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET)
#define greenLedTurnOff()				HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET)
#define greenLedBlinkFast()				do{greenLedTurnOn();HAL_Delay(250);greenLedTurnOff();HAL_Delay(250);}while(0)
#define greenLedBlinkSlow()				do{greenLedTurnOn();HAL_Delay(500);greenLedTurnOff();HAL_Delay(500);}while(0)
#define blueLedTurnOn()					HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_SET)
#define blueLedTurnOff()				HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET)
#define blueLedBlinkFast()				do{blueLedTurnOn();HAL_Delay(250);blueLedTurnOff();HAL_Delay(250);}while(0)
#define blueLedBlinkSlow()				do{blueLedTurnOn();HAL_Delay(500);blueLedTurnOff();HAL_Delay(500);}while(0)
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile errorCode_e	errorCode		= ERROR_CODE_NONE;
volatile systemFlags_t	systemFlags;
uint16_t				adcDmaBuffer[ADC_DMA_BUFFER_ELEMENTS];
char					uart1RxDmaBuffer[UART1_RX_DMA_BUFFER_ELEMENTS];
char					uart1TxDmaBuffer[UART1_TX_DMA_BUFFER_ELEMENTS];
//	char					uart2RxDmaBuffer[UART2_RX_DMA_BUFFER_ELEMENTS];
char					uart2TxDmaBuffer[UART2_TX_DMA_BUFFER_ELEMENTS];
CircularBuffer8			uart1RxCircBuff(200);
NmeaParser				gpsParser(100, false);

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */
    HAL_StatusTypeDef halStatus = HAL_OK;
    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_CRC_Init();
    MX_I2C1_Init();
    MX_I2C2_Init();
    MX_SPI1_Init();
    MX_SPI2_Init();
    MX_TIM1_Init();
    MX_TIM2_Init();
    MX_TIM3_Init();
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();
    MX_USB_DEVICE_Init();
    /* USER CODE BEGIN 2 */


    ledTurnOff();

    // =========================================================================
    // ADC configuration
    // =========================================================================
    //    Adc is configured to start a sequence of conversions when the TIM3
    // Update Event occurs. Each comversion sequence is composed by 7 channels
    // and the temperature channel. The samples are stored in a buffer using
    // DMA.
    // =========================================================================

    halStatus = HAL_ADCEx_Calibration_Start(&hadc1);
    if (halStatus != HAL_OK)
    {
        showErrorStatus(ERROR_CODE_ADC_CALIBRATION_FAILED);
    }
    halStatus = HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcDmaBuffer, ADC_DMA_BUFFER_ELEMENTS);
    if (halStatus != HAL_OK)
    {
        showErrorStatus(ERROR_CODE_ADC_START_DMA_FAILED);
    }

    // =========================================================================
    // TIMER3 configuration
    // =========================================================================
    //	 Timer 3 is configured to generate an interruption each 5ms (200 Hz).
    // This time base is also used by ADC to start a sequence of conversions.
    // =========================================================================

    halStatus = HAL_TIM_Base_Start_IT(&htim3);
    if (halStatus != HAL_OK)
    {
        showErrorStatus(ERROR_CODE_TIMER_BASETIME_START_INTERRUPT_FAILED);
    }

    // =========================================================================
    // UART configuration
    // =========================================================================
    //
    // =========================================================================

    strcpy(uart1TxDmaBuffer, "Teste da UART1\r");
    halStatus = HAL_UART_Transmit_DMA(&huart1, (uint8_t *)uart1TxDmaBuffer, strlen(uart1TxDmaBuffer));
    if (halStatus != HAL_OK)
    {
        showErrorStatus(ERROR_CODE_UART_TRANSMIT_DMA_FAILED);
    }

    halStatus = HAL_UART_Receive_DMA(&huart1, (uint8_t *)uart1RxDmaBuffer, UART1_RX_DMA_BUFFER_ELEMENTS);
    if (halStatus != HAL_OK)
    {
        showErrorStatus(ERROR_CODE_UART_RECEIVE_DMA_FAILED);
    }

    strcpy(uart2TxDmaBuffer, "Teste da UART2\r");
    halStatus = HAL_UART_Transmit_DMA(&huart2, (uint8_t *)uart2TxDmaBuffer, strlen(uart2TxDmaBuffer));
    if (halStatus != HAL_OK)
    {
        showErrorStatus(ERROR_CODE_UART_TRANSMIT_DMA_FAILED);
    }

    if(!gpsParser.startToReceiveData())
    {
        showErrorStatus(gpsParser.getLastError());
    }

    /* USER CODE END 2 */
    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        // =====================================================================
        // Process UART1 DMA buffer interrupts
        // =====================================================================

        // Reading the first half of the DMA buffer
        if (systemFlags.uart1RxHalfComplete)
        {
            for (uint8_t i = 0; i < (UART1_RX_DMA_BUFFER_ELEMENTS / 2); i++)
            {
                uart1RxCircBuff.push(uart1RxDmaBuffer[i]);
            }
            systemFlags.uart1RxHalfComplete = false;
        }

        // Reading the seccond half of the DMA buffer
        if (systemFlags.uart1RxFullComplete)
        {
            for (uint8_t i = (UART1_RX_DMA_BUFFER_ELEMENTS / 2); i < UART1_RX_DMA_BUFFER_ELEMENTS; i++)
            {
                uart1RxCircBuff.push(uart1RxDmaBuffer[i]);
            }
            systemFlags.uart1RxFullComplete = false;
        }

        // =====================================================================
        // Processing UART circular buffer to decode one NMEA sentence
        // =====================================================================
        // 1 - Get one NMEA sentence from circular buffer
        if (gpsParser.isClearToReceiveData())  				// Does the parser wants more data?
        {
            if (uart1RxCircBuff.getOccupation() > 0)  			// Does the circ. buff. haves data to send?
            {
                uint8_t aux8 = 0;
                uart1RxCircBuff.pop(&aux8);
                gpsParser.pushData(aux8);
            }
        }

        // 2 - Starts to process the data
        if (gpsParser.isReadyToProcessData())  				// Does the parser is ready to process data?
        {
            gpsParser.startToDecodeData();					// So, process it
        }

        // 3 - Retrieve data from gpsParser
        if (gpsParser.isNewDataReady())  					// Does the parser finised to process last message?
        {
            char strTeste[200];
            // strcpy(strTeste, "=> ");
            // sprintf(strTeste, "%s %s", strTeste, gpsParser._sentenceRmc.course);
            // sprintf(strTeste, "%s %s", strTeste, gpsParser._sentenceRmc.date);
            // sprintf(strTeste, "%s %s", strTeste, gpsParser._sentenceRmc.latitudeValue);
            // sprintf(strTeste, "%s %s", strTeste, gpsParser._sentenceRmc.latitudeIndicator);
            // sprintf(strTeste, "%s %s", strTeste, gpsParser._sentenceRmc.longitudeValue);
            // sprintf(strTeste, "%s %s", strTeste, gpsParser._sentenceRmc.longitudeIndicator);
            // sprintf(strTeste, "%s %s", strTeste, gpsParser._sentenceRmc.speed);
            // sprintf(strTeste, "%s %s", strTeste, gpsParser._sentenceRmc.time);
            // sprintf(strTeste, "%s %s", strTeste, gpsParser._sentenceRmc.valid);
            // sprintf(strTeste, "%s %s\r\n", strTeste, gpsParser._sentenceRmc.mode);
            strcpy(strTeste, "$GPRMC");
            sprintf(strTeste, "%s,%s", strTeste, gpsParser._sentenceRmc.time);
            sprintf(strTeste, "%s,%s", strTeste, gpsParser._sentenceRmc.valid);
            sprintf(strTeste, "%s,%s", strTeste, gpsParser._sentenceRmc.latitudeValue);
            sprintf(strTeste, "%s,%s", strTeste, gpsParser._sentenceRmc.latitudeIndicator);
            sprintf(strTeste, "%s,%s", strTeste, gpsParser._sentenceRmc.longitudeValue);
            sprintf(strTeste, "%s,%s", strTeste, gpsParser._sentenceRmc.longitudeIndicator);
            sprintf(strTeste, "%s,%s", strTeste, gpsParser._sentenceRmc.speed);
            sprintf(strTeste, "%s,%s", strTeste, gpsParser._sentenceRmc.course);
            sprintf(strTeste, "%s,%s", strTeste, gpsParser._sentenceRmc.date);
            sprintf(strTeste, "%s,%s\r\n", strTeste, gpsParser._sentenceRmc.mode);
            HAL_UART_Transmit_DMA(&huart2, (uint8_t *)strTeste, strlen(strTeste));

            // GET DATA GET DATA GET DATA GET DATA GET DATA

            // Starts the process again.
            gpsParser.startToReceiveData();
        }

        if (systemFlags.printStatus)
        {

//            HAL_Delay(100);
//            sprintf(uart1TxDmaBuffer, "_clearToReceiveData = %d\r", gpsParser._clearToReceiveData);
//            HAL_UART_Transmit(&huart1, (uint8_t *)uart1TxDmaBuffer, strlen(uart1TxDmaBuffer), 100);
//            HAL_Delay(100);
//            // sprintf(uart1TxDmaBuffer, "_initialized = %d\r", gpsParser._initialized);
//            // HAL_UART_Transmit(&huart1, (uint8_t *)uart1TxDmaBuffer, strlen(uart1TxDmaBuffer), 100);
//            // HAL_Delay(100);
//            sprintf(uart1TxDmaBuffer, "_lastError = %d\r", gpsParser._lastError);
//            HAL_UART_Transmit(&huart1, (uint8_t *)uart1TxDmaBuffer, strlen(uart1TxDmaBuffer), 100);
//            HAL_Delay(100);
//            sprintf(uart1TxDmaBuffer, "_newDataAvailable = %d\r", gpsParser._newDataAvailable);
//            HAL_UART_Transmit(&huart1, (uint8_t *)uart1TxDmaBuffer, strlen(uart1TxDmaBuffer), 100);
//            HAL_Delay(100);
//            sprintf(uart1TxDmaBuffer, "_readyToProcessData = %d\r", gpsParser._readyToProcessData);
//            HAL_UART_Transmit(&huart1, (uint8_t *)uart1TxDmaBuffer, strlen(uart1TxDmaBuffer), 100);
//            HAL_Delay(100);
//            // sprintf(uart1TxDmaBuffer, "_callbackFunctionEnable = %d\r", gpsParser._callbackFunctionEnable);
//            // HAL_UART_Transmit(&huart1, (uint8_t *)uart1TxDmaBuffer, strlen(uart1TxDmaBuffer), 100);
//            // HAL_Delay(100);
//            // sprintf(uart1TxDmaBuffer, "_checksumEnabled = %d\r", gpsParser._checksumEnabled);
//            // HAL_UART_Transmit(&huart1, (uint8_t *)uart1TxDmaBuffer, strlen(uart1TxDmaBuffer), 100);
//            // HAL_Delay(100);
//            sprintf(uart1TxDmaBuffer, "_sentenceData = %s\r", gpsParser._sentenceData);
//            HAL_UART_Transmit(&huart1, (uint8_t *)uart1TxDmaBuffer, strlen(uart1TxDmaBuffer), 100);
//            HAL_Delay(100);
//            sprintf(uart1TxDmaBuffer, "_sentenceIndex = %d\r", gpsParser._sentenceIndex);
//            HAL_UART_Transmit(&huart1, (uint8_t *)uart1TxDmaBuffer, strlen(uart1TxDmaBuffer), 100);
//            HAL_Delay(100);
//            sprintf(uart1TxDmaBuffer, "_sentenceLength = %d\r", gpsParser._sentenceLength);
//            HAL_UART_Transmit(&huart1, (uint8_t *)uart1TxDmaBuffer, strlen(uart1TxDmaBuffer), 100);
//            HAL_Delay(100);
//            // sprintf(uart1TxDmaBuffer, "_sentenceMaxSize = %d\r", gpsParser._sentenceMaxSize);
//            // HAL_UART_Transmit(&huart1, (uint8_t *)uart1TxDmaBuffer, strlen(uart1TxDmaBuffer), 100);
//            // HAL_Delay(100);
//            sprintf(uart1TxDmaBuffer, "_decodedSentenceType = %d\r", (uint16_t)gpsParser._decodedSentenceType);
//            HAL_UART_Transmit(&huart1, (uint8_t *)uart1TxDmaBuffer, strlen(uart1TxDmaBuffer), 100);
//            HAL_Delay(100);
//            sprintf(uart1TxDmaBuffer, "_decodedTalkerId = %d\r", (uint16_t)gpsParser._decodedTalkerId);
//            HAL_UART_Transmit(&huart1, (uint8_t *)uart1TxDmaBuffer, strlen(uart1TxDmaBuffer), 100);
//            HAL_Delay(100);
//            // sprintf(uart1TxDmaBuffer, "_fieldLength = %d\r", gpsParser._fieldLength);
//            // HAL_UART_Transmit(&huart1, (uint8_t *)uart1TxDmaBuffer, strlen(uart1TxDmaBuffer), 100);
//            // HAL_Delay(100);
//            sprintf(uart1TxDmaBuffer, "_state = %d\r", (uint16_t)gpsParser._state);
//            HAL_UART_Transmit(&huart1, (uint8_t *)uart1TxDmaBuffer, strlen(uart1TxDmaBuffer), 100);
//            HAL_Delay(100);
            systemFlags.printStatus = false;
        }
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC | RCC_PERIPHCLK_USB;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{

}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{

}

void HAL_TIM_ErrorCallback(TIM_HandleTypeDef *htim)
{

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    // static uint16_t counter = 0;

    // if ((++counter) == 600)
    // {
    //     systemFlags.printStatus = true;
    //     counter = 0;
    // }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    systemFlags.uart1TxFullComplete = true;
}

void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    systemFlags.uart1TxHalfComplete = true;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    systemFlags.uart1RxFullComplete = true;
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    systemFlags.uart1RxHalfComplete = true;
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{

}

void HAL_UART_AbortCpltCallback(UART_HandleTypeDef *huart)
{

}

void HAL_UART_AbortTransmitCpltCallback(UART_HandleTypeDef *huart)
{

}

void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart)
{

}

void uart1ProcessReception(bool isFullCompleteCallback)
{


    return;
}





/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

    __unused blinkWarning_s blinkCode;

    // Decode errorCode
    switch (errorCode)
    {
    case 0x0000 ... 0x000F:			// General error
        break;
    case 0x0010 ... 0x001F:			// Adc error
        break;
    case 0x0020 ... 0x002F:			// Timer error
        break;
    case 0x0030 ... 0xFFFE:			// Unmapped error
        break;
    case 0xFFFF:					// Unknown error
        break;
    }

    // Display error using RGB LED
    while (1)
    {
        ledToggle();
        HAL_Delay(500);
    }
    /* USER CODE END Error_Handler_Debug */
}

void printErrorStatus(uint16_t errorCode, uint8_t* file, uint32_t line)
{
    char strTest[100];

#if __DEBUG_HIDE_SUCCESS_MESSAGES
    if(errorCode == 0)
    {
        return;
    }
#endif

    ledTurnOn();
    sprintf(strTest, "%s:%d - %d\r", file, (uint16_t)line, (uint16_t)errorCode);
    HAL_UART_Transmit_DMA(&huart2, (uint8_t *)strTest, strlen(strTest));
    HAL_Delay(30);

#if __STOP_ON_FIRST_ERROR
    if(errorCode != 0)
    {
        while (1);
    }
#endif
    ledTurnOff();

    return;
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *		 where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    while (1);
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
