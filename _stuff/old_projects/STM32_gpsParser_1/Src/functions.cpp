// #include "..\Inc\functions.hpp"





// void printStatus(void)
// {
//     HAL_StatusTypeDef halStatus = HAL_OK;

//     sprintf(uart1TxDmaBuffer, "LE=%d, OC=%d, ML=%d, FL=%d, LE=%d, CR=%d, RP=%d, DA=%d\r",
//             uart1RxCircBuff.getLastError(), uart1RxCircBuff.getOccupation(),
//             gpsParser._sentenceLength, gpsParser._fieldLength, gpsParser._lastError,
//             gpsParser.isClearToReceiveData(), gpsParser.isReadyToProcessData(), gpsParser.isNewDataReady());
//     halStatus = HAL_UART_Transmit(&huart1, (uint8_t *)uart1TxDmaBuffer, strlen(uart1TxDmaBuffer), 100);
//     if (halStatus != HAL_OK)
//     {
//         errorCode = ERROR_CODE_UART_UNKNOWN;
//     }
//     if (gpsParser._sentenceLength > 0)
//     {
//         halStatus = HAL_UART_Transmit_DMA(&huart1, gpsParser._sentenceData, gpsParser._sentenceLength);
//     }
//     if (halStatus != HAL_OK)
//     {
//         errorCode = ERROR_CODE_UART_UNKNOWN;
//     }

//     return;
// }


