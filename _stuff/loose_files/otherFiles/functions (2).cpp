#include "..\Inc\functions.hpp"

CircularBuffer8		uart1RxCircBuff(200);
NmeaParser			gpsParser(100, true);


void uart1ProcessReception(bool isFullCompleteCallback)
{
	if (isFullCompleteCallback) {		// Complete callback
		for (uint8_t i = (UART1_RX_DMA_BUFFER_ELEMENTS / 2); i < UART1_RX_DMA_BUFFER_ELEMENTS; i++) {
			uart1RxCircBuff.push(uart1RxDmaBuffer[i]);
		}
		sprintf(uart1TxDmaBuffer, "FULL\r\n");
		HAL_UART_Transmit_DMA(&huart1, (uint8_t *)uart1TxDmaBuffer, strlen(uart1TxDmaBuffer));
	} else {		// Half complete callback

		for (uint8_t i = 0; i < (UART1_RX_DMA_BUFFER_ELEMENTS / 2); i++) {
			uart1RxCircBuff.push(uart1RxDmaBuffer[i]);
		}
		sprintf(uart1TxDmaBuffer, "HALF\r\n");
		HAL_UART_Transmit_DMA(&huart1, (uint8_t *)uart1TxDmaBuffer, strlen(uart1TxDmaBuffer));
	}

	return;
}




void printStatus(void)
{
	HAL_StatusTypeDef halStatus = HAL_OK;

	sprintf(uart1TxDmaBuffer, "LE=%d, OC=%d, ML=%d, FL=%d, LE=%d, CR=%d, RP=%d, DA=%d\r",
		uart1RxCircBuff.getLastError(), uart1RxCircBuff.getOccupation(),
		gpsParser._messageLength, gpsParser._fieldLength, gpsParser._lastError,
		gpsParser.isClearToReceiveData(), gpsParser.isReadyToProcessData(), gpsParser.isNewDataReady());
	halStatus = HAL_UART_Transmit(&huart1, (uint8_t *)uart1TxDmaBuffer, strlen(uart1TxDmaBuffer), 100);
	if (halStatus != HAL_OK) {
		errorCode = ERROR_CODE_UART_UNKNOWN;
	}
	if (gpsParser._messageLength > 0) {
		halStatus = HAL_UART_Transmit_DMA(&huart1, gpsParser._messageData, gpsParser._messageLength);
	}
	if (halStatus != HAL_OK) {
		errorCode = ERROR_CODE_UART_UNKNOWN;
	}

	return;
}


