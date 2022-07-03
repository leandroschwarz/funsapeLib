
#include "myMain.hpp"

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
void Error_Handler(void)
{
	myErrorHandler((uint16_t)Error::UNKNOWN);

	// Returns successfully
	return;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] isFirstBank_p description of this parameter
 */
void processAdc1DmaData(bool isFirstBank_p)
{
	uint16_t index = (isFirstBank_p) ? 0 : adc1DmaBuffSizeHalf;

	if(!adc1CircBuffer.pushBuffer(&adc1DmaBuffer[index], adc1DmaBuffSizeHalf)) {
		funsapeStatus = adc1CircBuffer.getLastError();
		myErrorHandler((uint16_t)funsapeStatus);
	}

	// Returns successfully
	return;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] isFirstBank_p description of this parameter
 */
void processAdc3DmaData(bool isFirstBank_p)
{
	// // Initializing variables
	// uint32_t rawRtcBatteryVoltage = 0;
	// uint32_t rawMainBatteryVoltage = 0;
	// uint32_t rawStepUpConverterVoltage = 0;

	// // Reading data from DMA buffer
	// if(isFirstBank_p) {
	// 	rawRtcBatteryVoltage = (uint32_t)batteryVoltagesDmaBuffer[0];
	// 	rawMainBatteryVoltage = (uint32_t)batteryVoltagesDmaBuffer[1];
	// 	rawStepUpConverterVoltage = (uint32_t)batteryVoltagesDmaBuffer[2];
	// } else {
	// 	rawRtcBatteryVoltage = (uint32_t)batteryVoltagesDmaBuffer[0 + BAT_ADC_DMA_BUFF_SIZE_HALF];
	// 	rawMainBatteryVoltage = (uint32_t)batteryVoltagesDmaBuffer[1 + BAT_ADC_DMA_BUFF_SIZE_HALF];
	// 	rawStepUpConverterVoltage = (uint32_t)batteryVoltagesDmaBuffer[2 + BAT_ADC_DMA_BUFF_SIZE_HALF];
	// }

	// // Calculate CR2032 voltage
	// rawRtcBatteryVoltage *= 825;
	// rawRtcBatteryVoltage /= 1024;
	// systemVoltages.rtcBattery = (uint16_t)rawRtcBatteryVoltage;

	// // Calculate Main Battery voltage
	// rawMainBatteryVoltage *= 825;
	// rawMainBatteryVoltage *= (POWER_MAIN_BAT_RES_TOP + POWER_MAIN_BAT_RES_BOTTOM);
	// rawMainBatteryVoltage /= (1024 * POWER_MAIN_BAT_RES_BOTTOM);
	// systemVoltages.mainBattery = (uint16_t)rawMainBatteryVoltage;

	// // Calculate Step Up converter voltage
	// rawStepUpConverterVoltage *= 825;
	// rawStepUpConverterVoltage *= (POWER_STEP_UP_RES_TOP + POWER_STEP_UP_RES_BOTTOM);
	// rawStepUpConverterVoltage /= (1024 * POWER_STEP_UP_RES_BOTTOM);
	// systemVoltages.stepUpConverter = (uint16_t)rawStepUpConverterVoltage;

	// // Signalizing that new voltage values are available
	// systemFlags.systemVoltagesUpdated = true;

	// Returns successfully
	return;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 * @param[in] errorCode_p description of this parameter
 */
void myErrorHandler(uint16_t errorCode_p)
{
	__disable_irq();

	display.writeDisplayData(errorCode_p, 16);

	while(1) {
	}

	// Returns successfully
	return;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
void setupAdc1(void)
{
	halStatus = HAL_ADC_Start_DMA(&hadc1, (uint32_t *)(adc1DmaBuffer), adc1DmaBuffSizeFull);
	strcpy(uart3TxBuffer, "ADC1 configured to use DMA\r");
//	if(!debug.printMessage(uart3TxBuffer)) {
//		myErrorHandler((uint16_t)debug.getLastError());
//	}

	// Returns successfully
	return;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
void setupAdc3(void)
{
	halStatus = HAL_ADC_Start_DMA(&hadc3, (uint32_t *)(adc3DmaBuffer), adc3DmaBuffSizeFull);
	strcpy(uart3TxBuffer, "ADC3 configured to use DMA\r");
//	if(!debug.printMessage(uart3TxBuffer)) {
//		myErrorHandler((uint16_t)debug.getLastError());
//	}

	// Returns successfully
	return;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
void setupDebug(void)
{
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

	// Returns successfully
	return;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
void setupHal(void)
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_ADC1_Init();
	MX_ADC3_Init();
	MX_I2C1_Init();
	MX_RTC_Init();
	MX_TIM1_Init();
	MX_TIM3_Init();
	MX_TIM8_Init();
	MX_I2C2_Init();
	MX_SPI1_Init();
	MX_SPI2_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	MX_USART3_UART_Init();
	MX_FATFS_Init();
	MX_USB_DEVICE_Init();

	// Returns successfully
	return;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
void setupTwi1(void)
{
	// FIXME: Implement function

	// Returns successfully
	return;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
void setupTwi2(void)
{
	// FIXME: Implement function

	// Returns successfully
	return;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
void setupSpi1(void)
{
	// FIXME: Implement function

	// Returns successfully
	return;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
void setupSpi2(void)
{
	// FIXME: Implement function

	// Returns successfully
	return;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
void setupTimer1(void)
{
	halStatus = HAL_TIM_Base_Start(&htim1);
	strcpy(uart3TxBuffer, "TIMER3 configured to 10ms interval\r");
	// debug.logMessage()
//	if(!debug.printMessage(uart3TxBuffer)) {
//		myErrorHandler((uint16_t)debug.getLastError());
//	}

	// Returns successfully
	return;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
void setupTimer3(void)
{
	halStatus = HAL_TIM_Base_Start_IT(&htim3);
	strcpy(uart3TxBuffer, "TIMER3 configured to 10ms interval\r");
//	if(!debug.printMessage(uart3TxBuffer)) {
//		myErrorHandler((uint16_t)debug.getLastError());
//	}

	// Returns successfully
	return;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
void setupTimer8(void)
{
	halStatus = HAL_TIM_Base_Start_IT(&htim8);
	strcpy(uart3TxBuffer, "TIMER8 configured to 8s interval\r");
//	if(!debug.printMessage(uart3TxBuffer)) {
//		myErrorHandler((uint16_t)debug.getLastError());
//	}

	// Returns successfully
	return;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
void setupUart1(void)
{
	// FIXME: Implement function

	// Returns successfully
	return;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
void setupUart2(void)
{
	// Configure USART3 RX DMA
	strcpy(uart3TxBuffer, "UART2 TX configured\r");
	HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxBuffer, strlen(uart3TxBuffer), uart2Timeout);

	// Returns successfully
	return;
}

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
void setupUart3(void)
{
	// Clears terminal
	uart3TxBuffer[0] = '\r';
	uart3TxBuffer[1] = '\r';
	halStatus = HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxBuffer, 2, uart3Timeout);
	if((uint16_t)halStatus) {
		myErrorHandler((uint16_t)Error::UNKNOWN);
	}

	// Configure USART3 RX DMA
	halStatus = HAL_UART_Receive_DMA(&huart3, uart3RxDmaBuffer, 2);
	if((uint16_t)halStatus) {
		myErrorHandler((uint16_t)Error::UNKNOWN);
	}
	strcpy(uart3TxBuffer, "UART3 RX configured to use DMA\r");
//	if(!debug.printMessage(uart3TxBuffer)) {
//		myErrorHandler((uint16_t)debug.getLastError());
//	}

	// Returns successfully
	return;
}















// //     ///////////    SYSTEM VOLTAGES (ADC3 + TIMER8)     ///////////     //
// #if !(DEBUG_DISABLE_ADC)
// 	halStatus = HAL_ADC_Start_DMA(&hadc3, (uint32_t *)(batteryVoltagesDmaBuffer), BAT_ADC_DMA_BUFF_SIZE);
// 	verboseStatus((uint16_t)halStatus, "ADC3 configured to use DMA\r");
// 	debugMessage((uint16_t)halStatus);
// 	if((uint16_t)halStatus) {
// 		myErrorHandler(ADC3_DMA_CONFIG);
// 	}
// 	halStatus = HAL_TIM_Base_Start_IT(&htim8);
// 	verboseStatus((uint16_t)halStatus, "TIMER8 configured\r");
// 	debugMessage((uint16_t)halStatus);
// 	if((uint16_t)halStatus) {
// 		myErrorHandler(TIMER8_CONFIG);
// 	}
// 	systemVoltages.rtcBattery = 0;
// 	systemVoltages.mainBattery = 0;
// 	systemVoltages.stepUpConverter = 0;
// #endif

// //     ///////////////////     ROTARY ENCODER     ///////////////////     //
// 	encoder.value = 0;
// 	encoder.ticksSinceLastEdge = 0;
// 	encoder.ticksSinceLastPress = 0;
// 	encoder.rotation = Rotation::CLOCKWISE;

// //     ///////////////////     CONFIGURE SPI1     ///////////////////     //

// 	if(!spi1.init(&hspi1)) {
// 		funsapeStatus = spi1.getLastError();
// 		debugMessage((uint16_t)funsapeStatus);
// 		myErrorHandler(SPI_CLASS_INIT);
// 	}

// //     ///////////////////////    SD CARD     ///////////////////////     //
// #if !(DEBUG_DISABLE_SD)
// 	funsapeStatus = sdCardCircBuffer.getLastError();
// 	if(funsapeStatus != Error::NONE) {
// 		debugMessage((uint16_t)funsapeStatus);
// 		myErrorHandler(SD_CIRC_BUFF_INIT);
// 	}
// // SD card mounting
// 	sdCard.result = f_mount(&sdCard.disk, "", 1);
// 	verboseStatus((uint16_t)sdCard.result, "SD Card volume mounted\r");
// 	debugMessage((uint16_t)sdCard.result);
// 	if(sdCard.result) {
// 		sdCard.diskIsMounted = false;
// 		myErrorHandler(SD_CARD_MOUNT);
// 	} else {
// 		sdCard.diskIsMounted = true;
// 	}
// // Search for last created file
// 	sdCard.fileNumber = 1;
// 	do {
// 		sprintf(sdCard.fileName, "file%04u.txt", sdCard.fileNumber);
// 		sdCard.result = f_open(&sdCard.fileHandler, sdCard.fileName, (BYTE)(FA_WRITE | FA_CREATE_NEW));
// 		if(!sdCard.result) {
// 			break;
// 		}
// 	} while((++sdCard.fileNumber) <= 9999);
// 	verboseStatus((uint16_t)sdCard.result, "New file created\r");
// 	debugMessage((uint16_t)sdCard.result);
// 	if(sdCard.result) {
// 		sdCard.fileIsOpenned = false;
// 		myErrorHandler(SD_CARD_OPEN_FILE);
// 	} else {
// 		sdCard.fileIsOpenned = true;
// 	}
// // Saving empty file
// 	sdCard.result = f_sync(&sdCard.fileHandler);
// 	verboseStatus((uint16_t)sdCard.result, "Empty file saved\r");
// 	debugMessage((uint16_t)sdCard.result);
// 	if(sdCard.result) {
// 		myErrorHandler(SD_CARD_SAVE_FILE);
// 	}
// // Closing file
// 	sdCard.result = f_close(&sdCard.fileHandler);
// 	verboseStatus((uint16_t)sdCard.result, "Empty file closed\r");
// 	debugMessage((uint16_t)sdCard.result);
// 	if(sdCard.result) {
// 		myErrorHandler(SD_CARD_CLOSE_FILE);
// 		sdCard.fileIsOpenned = false;
// 	}
// // SD card umounting
// 	sdCard.result = f_mount(NULL, "", 0);
// 	verboseStatus((uint16_t)sdCard.result, "SD Card unmounted\r");
// 	debugMessage((uint16_t)sdCard.result);
// 	if(sdCard.result) {
// 		myErrorHandler(SD_CARD_CLOSE_FILE);
// 		sdCard.diskIsMounted = false;
// 	}
// #endif

// //     ///////////////////     RESP (MPU9250)     ///////////////////     //
// #if !(DEBUG_DISABLE_MPU)
// 	if(!mpu0.init(&spi1, mpu0SelectDevice, mpu0DeselectDevice)) {
// 		funsapeStatus = mpu0.getLastError();
// 		debugMessage((uint16_t)funsapeStatus);
// 		myErrorHandler(RESP_MPU9250_INIT);
// 	}
// 	if(!mpu1.init(&spi1, mpu1SelectDevice, mpu1DeselectDevice)) {
// 		funsapeStatus = mpu1.getLastError();
// 		debugMessage((uint16_t)funsapeStatus);
// 		myErrorHandler(RESP_MPU9250_INIT);
// 	}
// 	if(!mpu2.init(&spi1, mpu2SelectDevice, mpu2DeselectDevice)) {
// 		funsapeStatus = mpu2.getLastError();
// 		debugMessage((uint16_t)funsapeStatus);
// 		myErrorHandler(RESP_MPU9250_INIT);
// 	}
// 	if(!mpu3.init(&spi1, mpu3SelectDevice, mpu3DeselectDevice)) {
// 		funsapeStatus = mpu3.getLastError();
// 		debugMessage((uint16_t)funsapeStatus);
// 		myErrorHandler(RESP_MPU9250_INIT);
// 	}

// 	if(!mpu0.configAccel(Mpu9250::AccelConfig::FREQ_4_KHZ_FILTER_FREQ_1046_HZ, Mpu9250::AccelFullScale::FULL_SCALE_2_G)) {
// 		funsapeStatus = mpu0.getLastError();
// 		debugMessage((uint16_t)funsapeStatus);
// 		myErrorHandler(RESP_MPU9250_CONFIG_ACCEL);
// 	}
// 	if(!mpu1.configAccel(Mpu9250::AccelConfig::FREQ_4_KHZ_FILTER_FREQ_1046_HZ, Mpu9250::AccelFullScale::FULL_SCALE_2_G)) {
// 		funsapeStatus = mpu1.getLastError();
// 		debugMessage((uint16_t)funsapeStatus);
// 		myErrorHandler(RESP_MPU9250_CONFIG_ACCEL);
// 	}
// 	if(!mpu2.configAccel(Mpu9250::AccelConfig::FREQ_4_KHZ_FILTER_FREQ_1046_HZ, Mpu9250::AccelFullScale::FULL_SCALE_2_G)) {
// 		funsapeStatus = mpu2.getLastError();
// 		debugMessage((uint16_t)funsapeStatus);
// 		myErrorHandler(RESP_MPU9250_CONFIG_ACCEL);
// 	}
// 	if(!mpu3.configAccel(Mpu9250::AccelConfig::FREQ_4_KHZ_FILTER_FREQ_1046_HZ, Mpu9250::AccelFullScale::FULL_SCALE_2_G)) {
// 		funsapeStatus = mpu3.getLastError();
// 		debugMessage((uint16_t)funsapeStatus);
// 		myErrorHandler(RESP_MPU9250_CONFIG_ACCEL);
// 	}
// #endif

// //     ///////////////////     CONFIGURE I2C1     ///////////////////     //
// 	if(!twi1.init(&hi2c1)) {
// 		funsapeStatus = twi1.getLastError();
// 		debugMessage((uint16_t)funsapeStatus);
// 		myErrorHandler(TWI_CLASS_INIT);
// 	}
// 	if(!twi1.setAddress(0x68, false)) {
// 		funsapeStatus = twi1.getLastError();
// 		debugMessage((uint16_t)funsapeStatus);
// 		myErrorHandler(TWI_CLASS_INIT);
// 	}

// 	while(1) {
// 		uint8_t aux[4];

// 		if(!twi1.readReg(0x75, aux, 1)) {
// 			funsapeStatus = twi1.getLastError();
// 			debugMessage((uint16_t)funsapeStatus);
// 			myErrorHandler(TWI_CLASS_INIT);
// 		}
// 		debugMessage((uint16_t)aux[0]);
// 		ledToggle();
// 		delayMs(100);
// 	}

// //     //////////////////////////////////////////////////////////////     //
// //     ////////////////////     TESTING CODE     ////////////////////     //
// //     //////////////////////////////////////////////////////////////     //
// #if !(DEBUG_DISABLE_MPU)
// 	for(uint16_t i = 0; i < 1000; i++) {
// 		sdCardCircBuffer.push((uint8_t)i);
// 	}
// 	char auxString[1000];
// 	sprintf(auxString, "%u\r", sdCardCircBuffer.getOccupation());
// 	HAL_UART_Transmit(&huart3, (uint8_t *)auxString, strlen(auxString), uart3TimeoutUART3_TIMEOUT);

// 	while(1) {
// 		char auxString[1000];
// 		int16_t accelBuff[3] = {0, 0, 0};

// 		if(!mpu0.getAccelData(accelBuff)) {
// 			myErrorHandler(4);
// 		}
// 		sprintf(auxString, "%08d %08d %08d \r", accelBuff[0], accelBuff[1], accelBuff[2]);
// 		HAL_UART_Transmit(&huart3, (uint8_t *)auxString, strlen(auxString), uart3TimeoutUART3_TIMEOUT);

// 		if(!mpu0.getAccelOffset(accelBuff)) {
// 			myErrorHandler(5);
// 		}
// 		sprintf(auxString, "%08d %08d %08d \r\r", accelBuff[0], accelBuff[1], accelBuff[2]);
// 		HAL_UART_Transmit(&huart3, (uint8_t *)auxString, strlen(auxString), uart3TimeoutUART3_TIMEOUT);

// 		ledToggle();
// 		delayMs(200);
// 	}
// #endif


void processEventEncoderRotation(void)
{
	// TODO: Action when encoder is rotated

	return;
}

void processEventEncoderButtonPress(void)
{
	// TODO: Action when encoder button is pressed

	return;
}

void processEventSystemVoltagesUpdated(void)
{
	// TODO: Action when new values of system voltage is available

	return;
}

void processEventAcquisitionStartNew(void)
{
	// TODO: Start the devices and create a new file

	return;
}

// void processEventSdCardWriteData(void)
// {
// 	// Pop data from circular buffer
// 	uint8_t sdCardBuff[SD_CARD_BUFF_SIZE];
// 	sdCardCircBuffer.popBuffer(sdCardBuff, SD_CARD_BUFF_SIZE);
// 	// SD card mounting
// 	sdCard.result = f_mount(&sdCard.disk, "", 1);
// 	verboseStatus((uint16_t)sdCard.result, "SD Card volume mounted\r");
// 	debugMessage((uint16_t)sdCard.result);
// 	if(sdCard.result) {
// 		sdCard.diskIsMounted = false;
// 		myErrorHandler(SD_CARD_MOUNT);
// 	} else {
// 		sdCard.diskIsMounted = true;
// 	}
// 	// Open last created file for append
// 	sdCard.result = f_open(&sdCard.fileHandler, sdCard.fileName, (BYTE)(FA_WRITE | FA_OPEN_ALWAYS));
// 	verboseStatus((uint16_t)sdCard.result, "File opened for write operation\r");
// 	debugMessage((uint16_t)sdCard.result);
// 	if(sdCard.result) {
// 		sdCard.fileIsOpenned = false;
// 		myErrorHandler(SD_CARD_OPEN_FILE);
// 	} else {
// 		sdCard.fileIsOpenned = true;
// 	}
// 	sdCard.result = f_lseek(&sdCard.fileHandler, f_size(&sdCard.fileHandler));
// 	verboseStatus((uint16_t)sdCard.result, "Moving pointer to append data\r");
// 	debugMessage((uint16_t)sdCard.result);
// 	if(sdCard.result) {
// 		sdCard.fileIsOpenned = false;
// 		myErrorHandler(SD_CARD_OPEN_FILE);
// 	} else {
// 		sdCard.fileIsOpenned = true;
// 	}
// 	// Writing data
// 	UINT bytesWrote;
// 	sdCard.result = f_write(&sdCard.fileHandler, sdCardBuff, SD_CARD_BUFF_SIZE, &bytesWrote);
// 	verboseStatus((uint16_t)sdCard.result, "Data written to file\r");
// 	debugMessage((uint16_t)sdCard.result);
// 	if(sdCard.result) {
// 		myErrorHandler(SD_CARD_WRITE_FILE);
// 	}
// 	// Saving empty file
// 	sdCard.result = f_sync(&sdCard.fileHandler);
// 	verboseStatus((uint16_t)sdCard.result, "File saved\r");
// 	debugMessage((uint16_t)sdCard.result);
// 	if(sdCard.result) {
// 		myErrorHandler(SD_CARD_SAVE_FILE);
// 	}
// 	// Closing file
// 	sdCard.result = f_close(&sdCard.fileHandler);
// 	verboseStatus((uint16_t)sdCard.result, "File closed\r");
// 	debugMessage((uint16_t)sdCard.result);
// 	if(sdCard.result) {
// 		myErrorHandler(SD_CARD_CLOSE_FILE);
// 		sdCard.fileIsOpenned = false;
// 	}
// 	// SD card umounting
// 	sdCard.result = f_mount(NULL, "", 0);
// 	verboseStatus((uint16_t)sdCard.result, "SD Card unmounted\r");
// 	debugMessage((uint16_t)sdCard.result);
// 	if(sdCard.result) {
// 		myErrorHandler(SD_CARD_CLOSE_FILE);
// 		sdCard.diskIsMounted = false;
// 	}

// 	return;
// }
