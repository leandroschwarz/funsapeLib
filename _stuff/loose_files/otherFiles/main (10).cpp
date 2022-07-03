


// =============================================================================
// Header files
// =============================================================================

// -----------------------------------------------------------------------------
// HAL generated header files --------------------------------------------------
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "usbd_cdc_if.h"

// -----------------------------------------------------------------------------
// FunSAPE Library header files ------------------------------------------------
#include "funsapeLibGlobalDefines.hpp"
#include "funsapeLibTwi.hpp"
// #include "devices/max30102.hpp"
#include "devices/max30105.hpp"
#include "funsapeLibStopwatch.hpp"
#include "util/funsapeLibCircularBuffer.hpp"

// =============================================================================
// Constant definitions
// =============================================================================

// -----------------------------------------------------------------------------
// Debounce time constants -----------------------------------------------------
#define ENCODER_BUTTON_DEBOUNCE_TIME	10
#define ENCODER_VALUE_DEBOUNCE_TIME		10

// =============================================================================
// Macrofunction definitions
// =============================================================================

// -----------------------------------------------------------------------------
// LED macrofunctions ----------------------------------------------------------
#define ledTurnOn()						do {HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_RESET);} while(0)
#define ledTurnOff()					do {HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_SET);} while(0)
#define ledToggle()						do {HAL_GPIO_TogglePin(NLED_GPIO_Port, NLED_Pin);} while(0)

// =============================================================================
// New data types
// =============================================================================

// -----------------------------------------------------------------------------
// System flags ----------------------------------------------------------------
typedef union {
	uint32_t allFlags;
	struct {
		uint32_t unusedBits				: 32;
	};
} systemFlags_t;

// =============================================================================
// Global variables
// =============================================================================

// -----------------------------------------------------------------------------
// System variables ------------------------------------------------------------
Error funsapeError;
//systemFlags_t systemFlags;

// -----------------------------------------------------------------------------
// Communication handlers ------------------------------------------------------
Twi i2c1;
Twi i2c2;
// Spi spi1;
// Spi spi2;
// Uart uart1;
// Uart uart2;
// Uart uart3;

// -----------------------------------------------------------------------------
// Peripheral devices objects --------------------------------------------------
Max30105 ppg1;
CircularBuffer<uint16_t> ppg1RedCircBuffer(200, true);
CircularBuffer<uint16_t> ppg1IrCircBuffer(200, true);

// -----------------------------------------------------------------------------
// Encoder variables -----------------------------------------------------------
vint16_t encoderValue = 0;
Stopwatch encoderValueStopwatch;
Stopwatch encoderButtonStopwatch;

// -----------------------------------------------------------------------------
// Interruption stopwatches ----------------------------------------------------
Stopwatch rtcSwqIntStopwatch;

// -----------------------------------------------------------------------------
// UART variables --------------------------------------------------------------
char uart1TxBuffer[200];

// -----------------------------------------------------------------------------
// USB variables ---------------------------------------------------------------
uint8_t usbFlag;
CircularBuffer<uint8_t> usbRxBuffer(200);

// =============================================================================
// Function declarations
// =============================================================================

// -----------------------------------------------------------------------------
// HAL function declarions -----------------------------------------------------
void SystemClock_Config(void);

// -----------------------------------------------------------------------------
// General function declarations -----------------------------------------------
void showError(uint8_t errorCode);
bool usbTransmit(uint8_t *bufData, uint16_t bufSize, uint16_t timeOut);

// =============================================================================
// Main function
// =============================================================================

int main(void)
{
	// -------------------------------------------------------------------------
	// Local variables
	// -------------------------------------------------------------------------

	// -------------------------------------------------------------------------
	// HAL initialization
	// -------------------------------------------------------------------------

	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_SPI1_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	MX_USB_DEVICE_Init();
	MX_DMA_Init();
	MX_ADC1_Init();
	MX_I2C1_Init();
	MX_I2C2_Init();
	MX_SPI2_Init();
	MX_TIM1_Init();
	MX_USART3_UART_Init();

	// -------------------------------------------------------------------------
	// FunSAPE DEBUG configuration
	// -------------------------------------------------------------------------

	debug.init(&huart1, true, true, false);
	ledTurnOff();
	HAL_Delay(10);
	setIoPin(DEBUG7_GPIO_Port, DEBUG7_Pin);
	HAL_Delay(10);
	clrIoPin(DEBUG7_GPIO_Port, DEBUG7_Pin);

	// -------------------------------------------------------------------------
	// Timer configuration
	// -------------------------------------------------------------------------

	// Set TIMER 1 to 1ms
	HAL_TIM_Base_Start_IT(&htim1);

	// -------------------------------------------------------------------------
	// SD Card configuration
	// -------------------------------------------------------------------------

	strcpy(uart1TxBuffer, "Teste da UART1!\r\r");
	HAL_UART_Transmit(&huart1, (uint8_t *)uart1TxBuffer, strlen(uart1TxBuffer), 10);
	// Getting RTC Data
	// RTC_DateTypeDef rtcDate;
	// RTC_TimeTypeDef rtcTime;
	// HAL_RTC_GetDate(&hrtc, &rtcDate, RTC_FORMAT_BIN);
	// HAL_RTC_GetTime(&hrtc, &rtcTime, RTC_FORMAT_BIN);
	// sprintf(uart2TxBuffer, "Current time is 20%02u/%02u/%02u %u %02u:%02u:%02u\r\r",
	// 		rtcDate.Year,
	// 		rtcDate.Month,
	// 		rtcDate.Date,
	// 		rtcDate.WeekDay,
	// 		rtcTime.Hours,
	// 		rtcTime.Minutes,
	// 		rtcTime.Seconds);
	// HAL_UART_Transmit(&huart2, (uint8_t *)uart2TxBuffer, strlen(uart2TxBuffer), 10);




	systemHalt();



	// -------------------------------------------------------------------------
	// I2C configuration
	// -------------------------------------------------------------------------

	// Initialize I2C1
	if(!i2c1.init(&hi2c1)) {
		showError(1);
	}

	// Initialize I2C2
	if(!i2c2.init(&hi2c2)) {
		showError(2);
	}

	// -------------------------------------------------------------------------
	// Encoder configuration
	// -------------------------------------------------------------------------

	encoderButtonStopwatch.start();
	encoderValue = 0;
	encoderValueStopwatch.start();

	// -------------------------------------------------------------------------
	// Max30102 configuration
	// -------------------------------------------------------------------------

	if(!ppg1.init(&i2c1)) {
		funsapeError = ppg1.getLastError();
		debugMessage(funsapeError);
	}
	if(!ppg1.setSpo2Mode(Max30105::LedCurrent::LED_CURRENT_40_MA, Max30105::LedCurrent::LED_CURRENT_40_MA)) {
		funsapeError = ppg1.getLastError();
		debugMessage(funsapeError);
	}
	if(!ppg1.setFullScale(Max30105::FullScale::FULL_SCALE_8192_NA)) {
		funsapeError = ppg1.getLastError();
		debugMessage(funsapeError);
	}
	if(!ppg1.setSampleRate(Max30105::SampleRate::SAMPLE_RATE_200_HZ)) {
		funsapeError = ppg1.getLastError();
		debugMessage(funsapeError);
	}
	if(!ppg1.setResolution(Max30105::Resolution::RESOLUTION_16_BITS)) {
		funsapeError = ppg1.getLastError();
		debugMessage(funsapeError);
	}
	if(!ppg1.configFifo(Max30105::FifoAveraging::FIFO_NO_AVERAGE, true, 8)) {
		funsapeError = ppg1.getLastError();
		debugMessage(funsapeError);
	}
	if(!ppg1.flushFifo()) {
		funsapeError = ppg1.getLastError();
		debugMessage(funsapeError);
	}
	if(!ppg1.activateInterrupts(Max30105::InterruptFlags::INTERRUPT_FIFO_ALMOST_FULL)) {
		funsapeError = ppg1.getLastError();
		debugMessage(funsapeError);
	}
	Max30105::InterruptFlags auxFlags;
	if(!ppg1.getInterruptStatus(&auxFlags)) {
		funsapeError = ppg1.getLastError();
		debugMessage(funsapeError);
	}
	if(!ppg1.wakeUp()) {
		funsapeError = ppg1.getLastError();
		debugMessage(funsapeError);
	}

	// -------------------------------------------------------------------------
	// Main loop
	// -------------------------------------------------------------------------

	while(1) {

		// uint8_t samplesWaiting = 0;
		// uint8_t samplesLost = 0;

		// if(!ppg1.getFifoOccupation(&samplesWaiting, &samplesLost)) {
		// 	showError(11);
		// }
		// sprintf(usbTxBuffer, "%d samples, %d lost (%d, %d, %d)\r",
		// 		samplesWaiting, samplesLost,
		// 		(uint8_t)ppg1._fifoWritePointer, (uint8_t)ppg1._fifoReadPointer, (uint8_t)ppg1._fifoOverflowCounter
		// );
		HAL_UART_Transmit(&huart1, (uint8_t *)uart1TxBuffer, strlen(uart1TxBuffer), 100);
		HAL_Delay(100);
	}
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

	/** Initializes the CPU, AHB and APB busses clocks
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
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

	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC | RCC_PERIPHCLK_USB;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
	HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
}

/* USER CODE BEGIN 4 */

uint16_t counter = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	counter++;
	if(counter == 500) {
//		HAL_GPIO_TogglePin(NLED_GPIO_Port, NLED_Pin);
		counter = 0;
	}
}

void showError(uint8_t errorCode)
{
	while(1) {
		HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_SET);
		for(uint8_t i = 0; i < errorCode; i++) {
			HAL_Delay(200);
			HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_RESET);
			HAL_Delay(200);
			HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_SET);
		}
		HAL_Delay(2000);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	// RTC SQWE interruption handler
	if(GPIO_Pin == RTC_SQW_Pin) {
		if(rtcSwqIntStopwatch.elapsed() > 500) {
			rtcSwqIntStopwatch.start();
		}
	}
	// Encoder A line interruption handler
	else if(GPIO_Pin == ENC_A_Pin) {
		if(encoderValueStopwatch.elapsed() > ENCODER_VALUE_DEBOUNCE_TIME) {
			if(HAL_GPIO_ReadPin(ENC_B_GPIO_Port, ENC_B_Pin)) {
				encoderValue++;
			} else {
				encoderValue--;
			}
			encoderValueStopwatch.start();
		}
	}
	// Encoder button interruption handler
	else if(GPIO_Pin == ENC_BTN_Pin) {
		if(encoderButtonStopwatch.elapsed() > ENCODER_BUTTON_DEBOUNCE_TIME) {
			HAL_GPIO_TogglePin(NLED_GPIO_Port, NLED_Pin);
			encoderButtonStopwatch.start();
			Max30105::InterruptFlags auxflags;
			ppg1.getInterruptStatus(&auxflags);
			uint32_t redData[20];
			uint32_t irData[20];
			ppg1.getFifoData(20, redData, irData);
			for(uint8_t i = 0; i < 20; i++) {
				ppg1RedCircBuffer.push((uint16_t)(redData[i]));
				ppg1IrCircBuffer.push((uint16_t)(irData[i]));
			}
		}
	}
}




// USB WRAPPER Workaround
#undef CDC_Receive_FS
#include "usbd_cdc.h"
extern USBD_HandleTypeDef hUsbDeviceFS;

bool usbTransmit(uint8_t *bufData, uint16_t bufSize, uint16_t timeOut)
{
	uint32_t tickstart = 0U;

	tickstart = HAL_GetTick();

	while(CDC_Transmit_FS(bufData, bufSize) == USBD_BUSY) {
		// Check for the Timeout
		if(timeOut != HAL_MAX_DELAY) {
			if((timeOut == 0U) || ((HAL_GetTick() - tickstart) > timeOut)) {
				return false;
			}
		}
		// Try send again (just continue in loop)
	}

	return true;
}

int8_t CDC_Receive_FS(uint8_t *Buf, uint32_t *Len)
{
	for(uint32_t i = 0; i < *Len; i++) {
//		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		usbRxBuffer.push(Buf[i]);
	}

	USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
	USBD_CDC_ReceivePacket(&hUsbDeviceFS);
	return (USBD_OK);
}













/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
//	HAL_GPIO_TogglePin(NLED_GPIO_Port, NLED_Pin);

	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	   tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
