#ifndef _MY_HEADER_HPP
#define _MY_HEADER_HPP

#define ledTurnOn()						HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_RESET)
#define ledTurnOff()					HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_SET)
#define ledToggle()						HAL_GPIO_TogglePin(NLED_GPIO_Port, NLED_Pin)

#define debug0TurnOn()					HAL_GPIO_WritePin(DEBUG_0_GPIO_Port, DEBUG_0_Pin, GPIO_PIN_RESET)
#define debug0TurnOff()					HAL_GPIO_WritePin(DEBUG_0_GPIO_Port, DEBUG_0_Pin, GPIO_PIN_SET)
#define debug0Toggle()					HAL_GPIO_TogglePin(DEBUG_0_GPIO_Port, DEBUG_0_Pin)
#define debug1TurnOn()					HAL_GPIO_WritePin(DEBUG_1_GPIO_Port, DEBUG_1_Pin, GPIO_PIN_RESET)
#define debug1TurnOff()					HAL_GPIO_WritePin(DEBUG_1_GPIO_Port, DEBUG_1_Pin, GPIO_PIN_SET)
#define debug1Toggle()					HAL_GPIO_TogglePin(DEBUG_1_GPIO_Port, DEBUG_1_Pin)
#define debug2TurnOn()					HAL_GPIO_WritePin(DEBUG_2_GPIO_Port, DEBUG_2_Pin, GPIO_PIN_RESET)
#define debug2TurnOff()					HAL_GPIO_WritePin(DEBUG_2_GPIO_Port, DEBUG_2_Pin, GPIO_PIN_SET)
#define debug2Toggle()					HAL_GPIO_TogglePin(DEBUG_2_GPIO_Port, DEBUG_2_Pin)
#define debug3TurnOn()					HAL_GPIO_WritePin(DEBUG_3_GPIO_Port, DEBUG_3_Pin, GPIO_PIN_RESET)
#define debug3TurnOff()					HAL_GPIO_WritePin(DEBUG_3_GPIO_Port, DEBUG_3_Pin, GPIO_PIN_SET)
#define debug3Toggle()					HAL_GPIO_TogglePin(DEBUG_3_GPIO_Port, DEBUG_3_Pin)
#define debug4TurnOn()					HAL_GPIO_WritePin(DEBUG_4_GPIO_Port, DEBUG_4_Pin, GPIO_PIN_RESET)
#define debug4TurnOff()					HAL_GPIO_WritePin(DEBUG_4_GPIO_Port, DEBUG_4_Pin, GPIO_PIN_SET)
#define debug4Toggle()					HAL_GPIO_TogglePin(DEBUG_4_GPIO_Port, DEBUG_4_Pin)
#define debug5TurnOn()					HAL_GPIO_WritePin(DEBUG_5_GPIO_Port, DEBUG_5_Pin, GPIO_PIN_RESET)
#define debug5TurnOff()					HAL_GPIO_WritePin(DEBUG_5_GPIO_Port, DEBUG_5_Pin, GPIO_PIN_SET)
#define debug5Toggle()					HAL_GPIO_TogglePin(DEBUG_5_GPIO_Port, DEBUG_5_Pin)
#define debug6TurnOn()					HAL_GPIO_WritePin(DEBUG_6_GPIO_Port, DEBUG_6_Pin, GPIO_PIN_RESET)
#define debug6TurnOff()					HAL_GPIO_WritePin(DEBUG_6_GPIO_Port, DEBUG_6_Pin, GPIO_PIN_SET)
#define debug6Toggle()					HAL_GPIO_TogglePin(DEBUG_6_GPIO_Port, DEBUG_6_Pin)
#define debug7TurnOn()					HAL_GPIO_WritePin(DEBUG_7_GPIO_Port, DEBUG_7_Pin, GPIO_PIN_RESET)
#define debug7TurnOff()					HAL_GPIO_WritePin(DEBUG_7_GPIO_Port, DEBUG_7_Pin, GPIO_PIN_SET)
#define debug7Toggle()					HAL_GPIO_TogglePin(DEBUG_7_GPIO_Port, DEBUG_7_Pin)

// Channels per sample
#define ECG_ADC_SAMPLE_SIZE				3
// Samples per buffer (Must be even)
#define ECG_ADC_BUFFER_LENGTH			8
// Channels per buffer
#define ECG_ADC_BUFFER_SIZE				ECG_ADC_BUFFER_LENGTH * ECG_ADC_SAMPLE_SIZE


#endif
