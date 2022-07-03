#include "main.h"
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "funsapeLibGlobalDefines.hpp"
#include "funsapeLibCircularBuffer.hpp"
#include "ff.h"

#define ledTurnOn()						HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_RESET)
#define ledTurnOff()					HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_SET)
#define ledToggle()						HAL_GPIO_TogglePin(NLED_GPIO_Port, NLED_Pin)

vuint32_t Timer;
uint8_t uart3RxData[2];
CircularBuffer<char> uart3RxBuffer;
FATFS sdCardDrive;
BYTE sdCardDataBuffer[4096] __attribute__((aligned(4))) ;	/* Working buffer */
FIL sdCardFile;				/* sdCardFile objects */
FRESULT sdCardResult;
uint32_t auxUint32;
char uart3TxData[500];

void SystemClock_Config(void);
bool getKeyFromBuffer(char *key_p);
bool xatoi(char **str, long *sdCardResult);
void put_rc(FRESULT rc);
void xprintf(const char *fmt, ...);
void put_dump(void *buff, unsigned long addr, int len, int width);

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_SPI2_Init();
	MX_USART3_UART_Init();
	ledTurnOff();

	HAL_UART_Receive_DMA(&huart3, uart3RxData, 2);
	sprintf(uart3TxData, "UART3 is Working!\r");
	HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 100);

	// Mounting SD Card
	sprintf(uart3TxData, "Mounting SD Card:     ");
	HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 100);
	sdCardResult = f_mount(&sdCardDrive, "", 1);
	if(!sdCardResult) {
		sprintf(uart3TxData, "[FAILED]\r");
		HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 100);
		systemHalt();
	}
	sprintf(uart3TxData, "[  OK  ]\r");
	HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 100);

	// Create a new file
	sprintf(uart3TxData, "Creating a new file:  ");
	HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 100);
	sdCardResult = f_open(&sdCardFile, "teste.txt", (BYTE)(FA_WRITE | FA_CREATE_ALWAYS));
	if(!sdCardResult) {
		sprintf(uart3TxData, "[FAILED]\r");
		HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 100);
		systemHalt();
	}
	sprintf(uart3TxData, "[  OK  ]\r");
	HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 100);

	// Writing data
	sprintf(uart3TxData, "Writing data:         ");
	HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 100);
	sdCardResult = f_write(&sdCardFile, sdCardDataBuffer, 4096, &auxUint32);
	if(!sdCardResult) {
		sprintf(uart3TxData, "[FAILED]\r");
		HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 100);
		systemHalt();
	}
	sprintf(uart3TxData, "[  OK  ]\r");
	HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 100);



	while(1) {




		put_rc(f_close(&sdCardFile[0]));
		break;

		p1 = 4096;
		p2 = 0;
		Timer = 0;

		while(p1) {
			if((UINT)p1 >= dummyStringSize) {
				cnt = dummyStringSize;
				p1 -= dummyStringSize;
			} else {
				cnt = p1;
				p1 = 0;
			}

			if(sdCardResult != FR_OK) {
				put_rc(sdCardResult);
				break;
			}
			p2 += s2;
			if(cnt != s2) {
				break;
			}
		}
		xprintf("%lu bytes written with %lu kB/sec (%lums).\r", p2, Timer ? (p2 / Timer) : 0, Timer);
		f_close(&sdCardFile[0]);
		break;

	case 'n' :	// fn <org.name> <new.name> - Change name of an object
		// while(*ptr == ' ') {
		// 	ptr++;
		// }
		// ptr2 = strchr(ptr, ' ');
		// if(!ptr2) {
		// 	break;
		// }
		// *ptr2++ = 0;
		// while(*ptr2 == ' ') {
		// 	ptr2++;
		// }
		// put_rc(f_rename(ptr, ptr2));
		break;

	case 'u' :	// fu <name> - Unlink an object
		// while(*ptr == ' ') {
		// 	ptr++;
		// }
		// put_rc(f_unlink(ptr));
		break;

	case 'v' :	// fv - Truncate file
		put_rc(f_truncate(&sdCardFile[0]));
		break;

	case 'k' :	// fk <name> - Create a directory
		// while(*ptr == ' ') {
		// 	ptr++;
		// }
		// put_rc(f_mkdir(ptr));
		break;

	case 'a' :	// fa <atrr> <mask> <name> - Change attribute of an object
		// if(!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2)) {
		// 	break;
		// }
		// while(*ptr == ' ') {
		// 	ptr++;
		// }
		// put_rc(f_chmod(ptr, p1, p2));
		break;

	case 't' :	// ft <year> <month> <day> <hour> <min> <sec> <name> - Change timestamp of an object
		// if(!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2) || !xatoi(&ptr, &p3)) {
		// 	break;
		// }
		// Finfo.fdate = ((p1 - 1980) << 9) | ((p2 & 15) << 5) | (p3 & 31);
		// if(!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2) || !xatoi(&ptr, &p3)) {
		// 	break;
		// }
		// Finfo.ftime = ((p1 & 31) << 11) | ((p2 & 63) << 5) | ((p3 >> 1) & 31);
		// put_rc(f_utime(ptr, &Finfo));
		break;

	case 'x' :	// fx <src.name> <dst.name> - Copy a file
		// while(*ptr == ' ') {
		// 	ptr++;
		// }
		// ptr2 = strchr(ptr, ' ');
		// if(!ptr2) {
		// 	break;
		// }
		// *ptr2++ = 0;
		// while(*ptr2 == ' ') {
		// 	ptr2++;
		// }
		// xprintf("Opening \"%s\"", ptr);
		// sdCardResult = f_open(&sdCardFile[0], ptr, FA_OPEN_EXISTING | FA_READ);
		// xputc('\r');
		// if(sdCardResult) {
		// 	put_rc(sdCardResult);
		// 	break;
		// }
		// xprintf("Creating \"%s\"", ptr2);
		// sdCardResult = f_open(&sdCardFile[1], ptr2, FA_CREATE_ALWAYS | FA_WRITE);
		// xputc('\r');
		// if(sdCardResult) {
		// 	put_rc(sdCardResult);
		// 	f_close(&sdCardFile[0]);
		// 	break;
		// }
		// xprintf("Copying file...");
		// Timer = 0;
		// p1 = 0;
		// for(;;) {
		// 	sdCardResult = f_read(&sdCardFile[0], sdCardDataBuffer, blen, &s1);
		// 	if(sdCardResult || s1 == 0) {
		// 		break;    /* error or eof */
		// 	}
		// 	sdCardResult = f_write(&sdCardFile[1], sdCardDataBuffer, s1, &s2);
		// 	p1 += s2;
		// 	if(sdCardResult || s2 < s1) {
		// 		break;    /* error or disk full */
		// 	}
		// }
		// xprintf("\r%lu bytes copied with %lu kB/sec.\r", p1, p1 / Timer);
		// f_close(&sdCardFile[0]);
		// f_close(&sdCardFile[1]);
		break;

	default:
		break;
	}
	break;

default:
	break;
}

}	// if

/* USER CODE END WHILE */


/* USER CODE BEGIN 3 */
}
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

	/** Initializes the CPU, AHB and APB busses clocks
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV2;
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
	HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
}

/* USER CODE BEGIN 4 */


void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	uart3RxBuffer.push(uart3RxData[0]);
	return;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uart3RxBuffer.push(uart3RxData[1]);
	return;
}



////////////////////////////////////////////////////////////////////////////////
bool getKeyFromBuffer(char *key_p)
{
	uint16_t uart3RxBufferOccupation = uart3RxBuffer.getOccupation();

	if(!uart3RxBufferOccupation) {
		return false;
	}
	uart3RxBuffer.pop(key_p, false);
	return true;
}

bool xatoi(    // 0:Failed, 1:Successful
		char **str,		// Pointer to pointer to the string
		long *sdCardResult		// Pointer to the valiable to store the value
)
{
	unsigned long val;
	unsigned char c, r, s = 0;


	*sdCardResult = 0;

	while((c = **str) == ' ') {
		(*str)++;    // Skip leading spaces
	}

	if(c == '-') {		// negative?
		s = 1;
		c = *(++(*str));
	}

	if(c == '0') {
		c = *(++(*str));
		switch(c) {
		case 'x':		// hexdecimal
			r = 16; c = *(++(*str));
			break;
		case 'b':		// binary
			r = 2; c = *(++(*str));
			break;
		default:
			if(c <= ' ') {
				return 1;    // single zero
			}
			if(c < '0' || c > '9') {
				return 0;    // invalid char
			}
			r = 8;		// octal
		}
	} else {
		if(c < '0' || c > '9') {
			return 0;    // EOL or invalid char
		}
		r = 10;			// decimal
	}

	val = 0;
	while(c > ' ') {
		if(c >= 'a') {
			c -= 0x20;
		}
		c -= '0';
		if(c >= 17) {
			c -= 7;
			if(c <= 9) {
				return 0;    // invalid char
			}
		}
		if(c >= r) {
			return 0;    // invalid char for current radix
		}
		val = val * r + c;
		c = *(++(*str));
	}
	if(s) {
		val = 0 - val;    // apply sign if needed
	}

	*sdCardResult = val;
	return 1;
}

void put_rc(FRESULT rc)
{
	const char *str =
			"OK\0" "DISK_ERR\0" "INT_ERR\0" "NOT_READY\0" "NO_FILE\0" "NO_PATH\0"
			"INVALID_NAME\0" "DENIED\0" "EXIST\0" "INVALID_OBJECT\0" "WRITE_PROTECTED\0"
			"INVALID_DRIVE\0" "NOT_ENABLED\0" "NO_FILE_SYSTEM\0" "MKFS_ABORTED\0" "TIMEOUT\0"
			"LOCKED\0" "NOT_ENOUGH_CORE\0" "TOO_MANY_OPEN_FILES\0" "INVALID_PARAMETER\0";

	for(uint16_t i = 0; i != (uint16_t)rc && *str; i++) {
		while(*str++) ;
	}
	xprintf("rc=%u FR_%s\r", (UINT)rc, str);
}

void xprintf(const char *fmt, ...)
{
	va_list arp;

	char auxBuff[500];

	va_start(arp, fmt);
	vsprintf(auxBuff, fmt, arp);
	va_end(arp);

	HAL_UART_Transmit(&huart3, (uint8_t *)auxBuff, strlen(auxBuff), 500);
	return;
}

void put_dump(void *buff, unsigned long addr, int len, int width)
{
	int i;
	unsigned char *bp;
	unsigned short *sp;
	unsigned long *lp;
	uint8_t auxChar = ' ';

	xprintf("%08lX:", addr);		/* address */

	switch(width) {
	// case DW_CHAR:
	case 8:
		bp = (unsigned char *)buff;
		for(i = 0; i < len; i++) {	/* Hexdecimal dump */
			xprintf(" %04x", bp[i]);
		}
		auxChar = ' ';
		HAL_UART_Transmit(&huart3, &auxChar, 1, 500);
		for(i = 0; i < len; i++) {	/* ASCII dump */
			auxChar = (bp[i] >= ' ' && bp[i] <= '~') ? bp[i] : '.';
			HAL_UART_Transmit(&huart3, &auxChar, 1, 500);
		}
		break;
	// case DW_SHORT:
	case 16:
		sp = (unsigned short *)buff;
		do {							/* Hexdecimal dump */
			xprintf(" %04X", *sp++);
		} while(--len);
		break;
	// case DW_LONG:
	case 64:
		lp = (unsigned long *)buff;
		do {							/* Hexdecimal dump */
			xprintf(" %08LX", *lp++);
		} while(--len);
		break;
	}

#if !_LF_CRLF
	auxChar = '\r';
	HAL_UART_Transmit(&huart3, &auxChar, 1, 500);
#endif
	auxChar = '\n';
	HAL_UART_Transmit(&huart3, &auxChar, 1, 500);
}


void ligaLed(void)
{
	ledTurnOn();
}

void apagaLed(void)
{
	ledTurnOff();
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
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
