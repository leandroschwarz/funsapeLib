/* =============================================================================
 * Project:			STM32
 * File name:		main_functions.cpp
 * Module:			Main functions source code file
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "myMain.hpp"

// =============================================================================
// Global variables
// =============================================================================

//     //////////////////////////     ADC1     //////////////////////////     //
CircularBuffer<uint16_t> adc1CircBuffer(constAdc1CircBuffSize);
uint16_t adc1DmaBuffer[constAdc1DmaBuffSizeFull];

//     //////////////////////////     ADC3     //////////////////////////     //
uint16_t adc3DmaBuffer[constAdc3DmaBuffSizeFull];

//     ////////////////////////    BLUETOOTH     ////////////////////////     //
// CircularBuffer<uint8_t> bluetoothCircBuffer(constBlutoothCircBufferSize);
// Gpio bluetoothEnable(BT_EN_GPIO_Port, BT_EN_Pin);
// Gpio bluetoothStatus(BT_ST_GPIO_Port, BT_ST_Pin);

//     /////////////////////    ERROR AND DEBUG     /////////////////////     //
vuint64_t chronometer = 0;
volatile uint16_t globalError = 0;
Gpio debug1(DEBUG1_GPIO_Port, DEBUG1_Pin);
Gpio debug2(DEBUG2_GPIO_Port, DEBUG2_Pin);
Gpio debug3(DEBUG3_GPIO_Port, DEBUG3_Pin);
Gpio debug4(DEBUG4_GPIO_Port, DEBUG4_Pin);
Gpio debug5(DEBUG5_GPIO_Port, DEBUG5_Pin);
Gpio debug6(DEBUG6_GPIO_Port, DEBUG6_Pin);
Gpio debug7(DEBUG7_GPIO_Port, DEBUG7_Pin);
Gpio debug8(DEBUG8_GPIO_Port, DEBUG8_Pin);
Gpio led(NLED_GPIO_Port, NLED_Pin);
Gpio rtcSwo(DS1307_SWO_GPIO_Port, DS1307_SWO_Pin);
systemFlags_f systemFlags = {0};
RTC_DateTypeDef currentDate;
RTC_TimeTypeDef currentTime;
Error funsapeStatus = Error::NONE;
HAL_StatusTypeDef halStatus = HAL_OK;
Gpio tm1637Clk(TFT_DATA0_GPIO_Port, TFT_DATA0_Pin);
Gpio tm1637Dio(TFT_DATA1_GPIO_Port, TFT_DATA1_Pin);
Tm1637 display(&tm1637Dio, &tm1637Clk);

//     /////////////////////////    ENCODER     /////////////////////////     //
encoderData_s encoder = {0};

//     ///////////////////////     SPI1 (MPU)     ///////////////////////     //
// Mpu mpu1;
Gpio mpu1Cs(SPI1_CS_MPU1_GPIO_Port, SPI1_CS_MPU1_Pin);
CircularBuffer<uint16_t> mpu1CircBuffer(constMpuCircBuffSize);
// Mpu mpu2;
Gpio mpu2Cs(SPI1_CS_MPU2_GPIO_Port, SPI1_CS_MPU2_Pin);
CircularBuffer<uint16_t> mpu2CircBuffer(constMpuCircBuffSize);
// Mpu mpu3;
Gpio mpu3Cs(SPI1_CS_MPU3_GPIO_Port, SPI1_CS_MPU3_Pin);
CircularBuffer<uint16_t> mpu3CircBuffer(constMpuCircBuffSize);
// Mpu mpu4;
Gpio mpu4Cs(SPI1_CS_MPU4_GPIO_Port, SPI1_CS_MPU4_Pin);
CircularBuffer<uint16_t> mpu4CircBuffer(constMpuCircBuffSize);
uint16_t mpuStaticBuffer[constMpuBuffSize];
Spi spi1;

//     /////////////////////     SPI2 (SD CARD)     /////////////////////     //
sdCard_s sdCard;
CircularBuffer<uint8_t> sdCardCircBuffer(constSdCardCircBuffSize);
uint8_t sdCardBuffer[constSdCardBuffSize];
Spi spi2;

//     ///////////////////////    TFT DISPLAY     ///////////////////////     //
// Gpio tftCs;
// Gpio tftRd;
// Gpio tftRs;
// Gpio tftRst;
// Gpio tftWr;

//     /////////////////    TWI1 (PPG, RTC, EEPROM)     /////////////////     //
// Twi twi1;
// CircularBuffer<uint32_t> ppg1CircBuffer(constPpg1CircBufferSize);

//     //////////////////////////     TWI2     //////////////////////////     //
// Twi twi2;

//     //////////////////////////    UART1     //////////////////////////     //
// Uart uart1;
// char uart1RxBuffer[constUart1RxBuffSize];
// char uart1TxBuffer[constUart1TxBuffSize];

//     //////////////////////////    UART2     //////////////////////////     //
// Uart uart2;
// char uart2RxBuffer[constUart2RxBuffSize];
// char uart2TxBuffer[constUart2TxBuffSize];

//     //////////////////////////    UART3     //////////////////////////     //
// Uart uart3;
uint8_t uart3RxBuffer[constUart3RxBuffSize];
CircularBuffer<uint8_t> uart3RxCircBuffer(constUart3RxCircBuffSize);
char uart3TxBuffer[constUart3TxBuffSize];

//     ///////////////////////////    USB     ///////////////////////////     //
// Usb usb1;



void delayUs(uint16_t delay_p)
{
	uint16_t mark = delay_p * 3;

	htim1.Instance->CNT = 0;
	while(htim1.Instance->CNT < mark);

	return;
}

void Error_Handler(uint16_t errorCode_p)
{
	globalError = errorCode_p;
	Error_Handler();
}

void Error_Handler(void)
{
	__disable_irq();
	if(globalError == 0) {
		globalError = 0xFFFF;
	}
	display.writeDisplayData(globalError, 16);
	display.showDisplay(true);
	systemHalt();
}

void sdCardMount(void)
{
	// SD card - MOUNTING
	sdCard.result = f_mount(&(sdCard.disk), "", FATFS_FORCE_MOUNT);
	if(sdCard.result != FR_OK) {
		Error_Handler(ERROR_CODE_103B);
	}
	sprintf(uart3TxBuffer, "SD card mounted!\r");
	HAL_UART_Transmit(&huart1, (uint8_t *)uart3TxBuffer, strlen(uart3TxBuffer), 100);
}



void setupAdc1(void)
{
	// Configure peripheral
	halStatus = HAL_ADC_Start_DMA(&hadc1, (uint32_t *)(adc1DmaBuffer), constAdc1DmaBuffSizeFull);
	if((uint16_t)halStatus) {
		debugMessage((uint16_t)halStatus);
		Error_Handler(ERROR_CODE_102D);
	}
	strcpy(uart3TxBuffer, "ADC1 configured to use DMA\r");
	debug.printMessage(uart3TxBuffer);

	// Returns successfully
	return;
}

void setupAdc3(void)
{
	// Configure peripheral
	halStatus = HAL_ADC_Start_DMA(&hadc3, (uint32_t *)(adc3DmaBuffer), constAdc3DmaBuffSizeFull);
	if((uint16_t)halStatus) {
		debugMessage((uint16_t)halStatus);
		Error_Handler(ERROR_CODE_102E);
	}
	strcpy(uart3TxBuffer, "ADC3 configured to use DMA\r");
	debug.printMessage(uart3TxBuffer);

	// Returns successfully
	return;
}

void setupDebug(void)
{
	// Configure peripheral
	debug1.low();
	debug2.low();
	debug3.low();
	debug4.low();
	debug5.low();
	debug6.low();
	debug7.low();
	debug8.low();
	debugTriggerPulse();						// Trigger pulse for logic analyzer
	debug.init(&huart3);						// Debug handler
	debug.hideSuccess(true);					// Debug hide successful messages
	debug.stopOnError(false);					// Debug stop on first error
	debug.setMarkLevel(1);						// Debug mark level

	display.showDisplay(false);
	display.setDisplayContrast(Tm1637::Constrast::PERCENT_13);
	display.writeDisplayData(0);
	display.showDisplay(true);

	// Returns successfully
	return;
}

void setupHal(void)
{
	// Configure peripheral
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_ADC1_Init();
	MX_ADC3_Init();
	MX_I2C1_Init();
	MX_TIM3_Init();
	MX_TIM8_Init();
	MX_I2C2_Init();
	MX_SPI1_Init();
	MX_SPI2_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	MX_USART3_UART_Init();
	MX_USB_DEVICE_Init();
	MX_TIM1_Init();
	MX_TIM7_Init();
	if(rtcSwo.read() == LogicLevel::LOW) {
		MX_RTC_Init();
	} else {
		uint32_t dateMem;
		RTC_DateTypeDef DateToUpdate = {0};
		RTC_TimeTypeDef sTime = {0};

		hrtc.Instance = RTC;

		dateMem = BKP->DR3;
		dateMem |= BKP->DR2 << 16;

		memcpy(&DateToUpdate, &dateMem, sizeof(uint32_t));
		if(HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK) {
			Error_Handler(ERROR_CODE_102A);
		}
		if(HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK) {
			Error_Handler(ERROR_CODE_102B);
		}
		HAL_RTC_MspInit(&hrtc);
	}
	delayMs(100);

	// Returns successfully
	return;
}

void setupRtc(void)
{
	// RTC_TimeTypeDef auxTime;
	// RTC_AlarmTypeDef auxAlarm;

	// // Get current time
	// HAL_RTC_GetTime(&hrtc, &auxTime, FORMAT_BIN);

	// sprintf(uart3TxBuffer, "Current Time: %02u:%02u:%02u\r", auxTime.Hours, auxTime.Minutes, auxTime.Seconds);
	// debug.printMessage(uart3TxBuffer);

	// // Add two seconds to current time
	// auxTime.Seconds += 2;
	// if(auxTime.Seconds > 59) {
	// 	auxTime.Seconds -= 60;
	// 	auxTime.Minutes++;
	// 	if(auxTime.Minutes > 59) {
	// 		auxTime.Minutes = 0;
	// 		auxTime.Hours++;
	// 		if(auxTime.Hours > 23) {
	// 			auxTime.Hours = 0;
	// 		}
	// 	}
	// }

	// // Configure alarm structure
	// auxAlarm.Alarm = RTC_ALARM_A;
	// auxAlarm.AlarmTime = auxTime;

	// Start RTC alarm
//	halStatus = HAL_RTC_SetAlarm_IT(&hrtc, &auxAlarm, FORMAT_BIN);

	if(HAL_RTCEx_SetSecond_IT(&hrtc) != HAL_OK) {
		Error_Handler(ERROR_CODE_102C);
	}

	return;
}

void setupSpi1(void)
{
	// Configure peripheral
	if(!spi1.init(&hspi1, ((2 * constMpuBuffSize) + 2))) {
		funsapeStatus = spi1.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		Error_Handler(ERROR_CODE_102F);
	}
	strcpy(uart3TxBuffer, "SPI1 initialized\r");
	debug.printMessage(uart3TxBuffer);

	if(!spi1.setTimeout(constSpi1Timeout)) {
		funsapeStatus = spi1.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		Error_Handler(ERROR_CODE_1030);
	}
	strcpy(uart3TxBuffer, "SPI1 timeout configured\r");
	debug.printMessage(uart3TxBuffer);

	// Returns successfully
	return;
}

void setupSpi2(void)
{
	// Configure peripheral
	if(!spi2.init(&hspi2, (constSdCardBuffSize + 2))) {
		funsapeStatus = spi2.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		Error_Handler(ERROR_CODE_1031);
	}
	strcpy(uart3TxBuffer, "SPI2 initialized\r");
	debug.printMessage(uart3TxBuffer);

	if(!spi2.setTimeout(constSpi2Timeout)) {
		funsapeStatus = spi2.getLastError();
		debugMessage((uint16_t)funsapeStatus);
		Error_Handler(ERROR_CODE_1032);
	}
	strcpy(uart3TxBuffer, "SPI2 timeout configured\r");
	debug.printMessage(uart3TxBuffer);

	// Returns successfully
	return;
}

void setupTimer1(void)
{
	// Configure peripheral
	halStatus = HAL_TIM_Base_Start(&htim1);
	if((uint16_t)halStatus) {
		debugMessage((uint16_t)halStatus);
		Error_Handler(ERROR_CODE_1033);
	}
	strcpy(uart3TxBuffer, "TIMER1 configured to ???\r");
	debug.printMessage(uart3TxBuffer);

	// Returns successfully
	return;
}

void setupTimer3(void)
{
	// Configure peripheral
	halStatus = HAL_TIM_Base_Start_IT(&htim3);
	if((uint16_t)halStatus) {
		debugMessage((uint16_t)halStatus);
		Error_Handler(ERROR_CODE_1034);
	}
	strcpy(uart3TxBuffer, "TIMER3 configured to 10ms overflow\r");
	debug.printMessage(uart3TxBuffer);

	// Returns successfully
	return;
}

void setupTimer7(void)
{
	// Configure peripheral
	halStatus = HAL_TIM_Base_Start_IT(&htim7);
	clrBit(htim7.Instance->CR1, TIM_CR1_CEN_Pos);		// Stop counting
	if((uint16_t)halStatus) {
		debugMessage((uint16_t)halStatus);
		Error_Handler(ERROR_CODE_1035);
	}
	strcpy(uart3TxBuffer, "TIMER7 configured to 10us counter\r");
	debug.printMessage(uart3TxBuffer);

	// Returns successfully
	return;
}

void setupTimer8(void)
{
	// Configure peripheral
	halStatus = HAL_TIM_Base_Start_IT(&htim8);
	if((uint16_t)halStatus) {
		debugMessage((uint16_t)halStatus);
		Error_Handler(ERROR_CODE_1036);
	}
	strcpy(uart3TxBuffer, "TIMER8 configured to 8s overflow\r");
	debug.printMessage(uart3TxBuffer);

	// Returns successfully
	return;
}

void setupTwi1(void)
{
	// FIXME: Implement function

	// Returns successfully
	return;
}

void setupTwi2(void)
{
	// FIXME: Implement function

	// Returns successfully
	return;
}

void setupUart1(void)
{
	// FIXME: Implement function
	// Configure peripheral

	// Returns successfully
	return;
}

void setupUart2(void)
{
	// FIXME: Implement function

	// Configure peripheral

	// Returns successfully
	return;
}

void setupUart3(void)
{

	// Clears terminal
	uart3TxBuffer[0] = '\r';
	uart3TxBuffer[1] = '\r';
	halStatus = HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxBuffer, 2, constUart3Timeout);
	if((uint16_t)halStatus) {
//		debugMessage((uint16_t)halStatus);
		Error_Handler(ERROR_CODE_1037);
	}

	// Configure USART3 RX DMA
	halStatus = HAL_UART_Receive_DMA(&huart3, uart3RxBuffer, 2);
	if((uint16_t)halStatus) {
//		debugMessage((uint16_t)halStatus);
		Error_Handler(ERROR_CODE_1038);
	}
	strcpy(uart3TxBuffer, "UART3 RX configured to use DMA\r");
	HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxBuffer, strlen(uart3TxBuffer), constUart3Timeout);

	// Returns successfully
	return;
}

void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Initializes the RCC Oscillators according to the specified parameters
	* in the RCC_OscInitTypeDef structure.
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
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
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_ADC
			| RCC_PERIPHCLK_USB;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
	HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
}


