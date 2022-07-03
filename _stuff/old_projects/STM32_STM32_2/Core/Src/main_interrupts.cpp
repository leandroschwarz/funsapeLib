
#include "myMain.hpp"

// =============================================================================
// Interrupt callback function definitions
// =============================================================================

// -----------------------------------------------------------------------------
// UART interrupt callbacks ----------------------------------------------------

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	if(!uart3RxCircBuffer.push(uart3RxDmaBuffer[0])) {
		myErrorHandler((uint16_t)Error::UNKNOWN);
	}

	return;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(!uart3RxCircBuffer.push(uart3RxDmaBuffer[1])) {
		myErrorHandler((uint16_t)Error::UNKNOWN);
	}

	return;
}

// -----------------------------------------------------------------------------
// ADC interrupt callbacks -----------------------------------------------------

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
	if(hadc->Instance == ADC1) {
		debug2.toggle();						// DEBUG
		processAdc1DmaData(true);
	} else if(hadc->Instance == ADC3) {
		debug5.toggle();						// DEBUG
		processAdc3DmaData(true);
	}

	return;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if(hadc->Instance == ADC1) {
		debug3.toggle();						// DEBUG
		processAdc1DmaData(false);
	} else if(hadc->Instance == ADC3) {
		debug6.toggle();						// DEBUG
		processAdc3DmaData(false);
	}

	return;
}

// -----------------------------------------------------------------------------
// TIMER interrupt callbacks ---------------------------------------------------

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	if(htim->Instance == TIM3) {

		// Animation counter
		static uint8_t errorAnimationCounter = 0;
		errorAnimationCounter++;
		if(errorAnimationCounter == 5) {
			systemFlags.errorAnimationUpdate = true;
			errorAnimationCounter = 0;
		}

		debug1.toggle();						// DEBUG
	} else if(htim->Instance == TIM8) {
		debug4.toggle();						// DEBUG
	}
}

// -----------------------------------------------------------------------------
// EXTI interrupt callbacks ----------------------------------------------------

// void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
// {
// 	// Handling the encoder CLK interrupt
// 	if(GPIO_Pin == GPIO_PIN_5) {
// 		uint32_t currentTick = HAL_GetTick();

// 		if((currentTick - encoder.ticksSinceLastEdge) > ENCODER_CLOCK_DEBOUNCE) {
// 			if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_7)) {
// 				encoder.value++;
// 				encoder.rotation = Rotation::CLOCKWISE;
// 			} else {
// 				encoder.value--;
// 				encoder.rotation = Rotation::COUNTERCLOCKWISE;
// 			}
// 			encoder.ticksSinceLastEdge = currentTick;
// 			systemFlags.encoderValueUpdated = true;
// 		}
// 	}

// 	// Handling the encoder BTN interrupt
// 	else if(GPIO_Pin == GPIO_PIN_6) {
// 		uint32_t currentTick = HAL_GetTick();

// 		if((currentTick - encoder.ticksSinceLastPress) > ENCODER_BUTTON_DEBOUNCE) {
// 			systemFlags.encoderButtonPressed = true;
// 			encoder.ticksSinceLastPress = currentTick;
// 		}
// 	}

// 	// Handling other pin interrupt
// 	else if(GPIO_Pin == GPIO_PIN_8) {
// 		// TODO Handle interrupt from the RTC SWO pin
// 	}

// }
