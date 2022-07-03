/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "funsapeLibGlobalDefines.hpp"
#include "funsapeLibCircularBuffer.hpp"
#include "ff.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define EXCLUDE							1
#define DW_CHAR		sizeof(char)
#define DW_SHORT	sizeof(short)
#define DW_LONG		sizeof(long)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

#define ledTurnOn()						HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_RESET)
#define ledTurnOff()					HAL_GPIO_WritePin(NLED_GPIO_Port, NLED_Pin, GPIO_PIN_SET)
#define ledToggle()						HAL_GPIO_TogglePin(NLED_GPIO_Port, NLED_Pin)

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

volatile unsigned int Timer;
uint8_t uart3RxData[2];
CircularBuffer<char> uart3RxBuffer;

static
const char HelpMsg[] =
		"[Disk contorls]\r"
		" di <pd#> - Initialize disk\r"
		" dd [<pd#> <lba>] - Dump a secrtor\r"
		" ds <pd#> - Show disk status\r"
		"[Buffer controls]\r"
		" bd <ofs> - Dump working buffer\r"
		" be <ofs> [<data>] ... - Edit working buffer\r"
		" br <pd#> <lba> [<count>] - Read disk into working buffer\r"
		" bw <pd#> <lba> [<count>] - Write working buffer into disk\r"
		" bf <val> - Fill working buffer\r"
		"[File system controls]\r"
		" fi <ld#> [<mount>]- Force initialized the volume\r"
		" fs [<path>] - Show volume status\r"
		" fl [<path>] - Show a directory\r"
		" fo <mode> <file> - Open a file\r"
		" fc - Close the file\r"
		" fe <ofs> - Move fp in normal seek\r"
		" fd <len> - Read and dump the file\r"
		" fr <len> - Read the file\r"
		" fw <len> <val> - Write to the file\r"
		" fn <org.name> <new.name> - Rename an object\r"
		" fu <name> - Unlink an object\r"
		" fv - Truncate the file at current fp\r"
		" fk <name> - Create a directory\r"
		" fa <atrr> <mask> <object name> - Change attribute of an object\r"
		" ft <year> <month> <day> <hour> <min> <sec> <name> - Change timestamp of an object\r"
		" fx <src.file> <dst.file> - Copy a file\r"
		" fg <path> - Change current directory\r"
		" fq - Show current directory\r"
		" fb <name> - Set volume label\r"
		" fm <ld#> <type> <csize> - Create file system\r"
		" fz [<len>] - Change/Show R/W length for fr/fw/fx command\r"
		"[Misc commands]\r"
		" md[b|h|w] <addr> [<count>] - Dump memory\r"
		" mf <addr> <value> <count> - Fill memory\r"
		" me[b|h|w] <addr> [<value> ...] - Edit memory\r"
		" t [<year> <mon> <mday> <hour> <min> <sec>] - Set/Show RTC\r"
		"\r";
FATFS FatFs;
FATFS *fs;
FILINFO Finfo;
BYTE Buff[4096] __attribute__((aligned(4))) ;	/* Working buffer */
FIL File[2];				/* File objects */
DIR Dir;					/* Directory object */
FRESULT res;
long p1, p2, p3;
UINT s1, s2, cnt, blen = sizeof Buff;
DWORD ofs = 0;

char dummyStringData[1000];
uint16_t dummyStringSize = 512;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

bool getKeyFromBuffer(char *key_p);
bool xatoi(char **str, long *res);
void put_rc(FRESULT rc);
void xprintf(const char *fmt, ...);
void put_dump(void *buff, unsigned long addr, int len, int width);
void ligaLed(void);
void apagaLed(void);

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
	MX_SPI2_Init();
	MX_USART3_UART_Init();
	/* USER CODE BEGIN 2 */

	ledTurnOff();

	setBit(hspi2.Instance->CR1, 6);

	char uart3TxData[500];

	sprintf(dummyStringData, "");
	for(uint8_t i = 0; i < 100; i++) {
		sprintf(dummyStringData, "%s0123456789", dummyStringData);
	}

	HAL_UART_Receive_DMA(&huart3, uart3RxData, 2);
	sprintf(uart3TxData, "UART3 is Working!\r");
	HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 10);
	sprintf(uart3TxData,
			"SPI config (CR1 = 0x%04x, CR2 = 0x%04x, SR = 0x%04x, DR = 0x%04x, CRCPR = 0x%04x, RXCRCR = 0x%04x, TXCRCR = 0x%04x)\r",
			hspi2.Instance->CR1, hspi2.Instance->CR2, hspi2.Instance->SR, hspi2.Instance->DR,
			hspi2.Instance->CRCPR, hspi2.Instance->RXCRCR, hspi2.Instance->TXCRCR
	);
	HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 10);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while(1) {
		// uint16_t uart3RxBufferFree = uart3RxBuffer.getFreeSpace();
		// sprintf(uart3TxData, "%d | %d\r", uart3RxBufferOccupation, uart3RxBufferFree);
		// HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 10);
		// if(uart3RxBufferOccupation > 10) {
		// 	uart3RxBuffer.popBuffer(uart3TxData, 10);
		// 	uart3TxData[10] = '\r';
		// 	uart3TxData[11] = '\0';
		// 	HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 10);
		// }

		// ledToggle();
		// HAL_Delay(500);
		char keyPressed;
		if(getKeyFromBuffer(&keyPressed)) {
			switch(keyPressed) {
			case '?' :	// Show Command List
				HAL_UART_Transmit(&huart3, (uint8_t *)HelpMsg, strlen(HelpMsg), 1000);
				break;

			case 'f' :	// FatFS API controls
				while(!getKeyFromBuffer(&keyPressed));
				switch(keyPressed) {
				case 'i' :	// fi [<opt>]- Initialize logical drive
					sprintf(uart3TxData, "\rInitialize logical drive\r");
					HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 100);

					res = f_mount(&FatFs, "", 1);
					put_rc(res);
					sprintf(uart3TxData, "\rDone\r");
					HAL_UART_Transmit(&huart3, (uint8_t *)uart3TxData, strlen(uart3TxData), 100);
					break;

				case 's':	// fs [<path>] - Show volume status
					/*
					// 						while(*ptr == ' ') {
					// 							ptr++;
					// 						}
					// 						res = f_getfree(ptr, (DWORD *)&p1, &fs);
					// 						if(res) {
					// 							put_rc(res);
					// 							break;
					// 						}
					// 						xprintf("FAT type = %s\r", ft[fs->fs_type]);
					// 						xprintf("Bytes/Cluster = %lu\r", (DWORD)fs->csize * 512);
					// 						xprintf("Number of FATs = %u\r", fs->n_fats);
					// 						if(fs->fs_type < FS_FAT32) {
					// 							xprintf("Root DIR entries = %u\r", fs->n_rootdir);
					// 						}
					// 						xprintf("Sectors/FAT = %lu\r", fs->fsize);
					// 						xprintf("Number of clusters = %lu\r", (DWORD)fs->n_fatent - 2);
					// 						xprintf("Volume start (lba) = %lu\r", fs->volbase);
					// 						xprintf("FAT start (lba) = %lu\r", fs->fatbase);
					// 						xprintf("DIR start (lba,clustor) = %lu\r", fs->dirbase);
					// 						xprintf("Data start (lba) = %lu\r\r", fs->database);
					// #if FF_USE_LABEL
					// 						res = f_getlabel(ptr, (char *)Buff, (DWORD *)&p2);
					// 						if(res) {
					// 							put_rc(res);
					// 							break;
					// 						}
					// 						xprintf(Buff[0] ? "Volume name is %s\r" : "No volume label\r", (char *)Buff);
					// 						xprintf("Volume S/N is %04X-%04X\r", (DWORD)p2 >> 16, (DWORD)p2 & 0xFFFF);
					// #endif
					// 						AccSize = AccFiles = AccDirs = 0;
					// 						xprintf("...");
					// 						res = scan_files(ptr);
					// 						if(res) {
					// 							put_rc(res);
					// 							break;
					// 						}
					// 						xprintf("\r%u files, %lu bytes.\r%u folders.\r"
					// 								"%lu KiB total disk space.\r%lu KiB available.\r",
					// 								AccFiles, AccSize, AccDirs,
					// 								(fs->n_fatent - 2) * (fs->csize / 2), (DWORD)p1 * (fs->csize / 2)
											);
											*/
					break;


				case 'l' :	/* fl [<path>] - Directory listing */
					res = f_opendir(&Dir, "");
					if(res) {
						put_rc(res);
						break;
					}
					p1 = s1 = s2 = 0;
					for(;;) {
						res = f_readdir(&Dir, &Finfo);
						if((res != FR_OK) || !Finfo.fname[0]) {
							break;
						}
						if(Finfo.fattrib & AM_DIR) {
							s2++;
						} else {
							s1++; p1 += Finfo.fsize;
						}
						xprintf("%c%c%c%c%c %u/%02u/%02u %02u:%02u %9lu  %s\r",
								(Finfo.fattrib & AM_DIR) ? 'D' : '-',
								(Finfo.fattrib & AM_RDO) ? 'R' : '-',
								(Finfo.fattrib & AM_HID) ? 'H' : '-',
								(Finfo.fattrib & AM_SYS) ? 'S' : '-',
								(Finfo.fattrib & AM_ARC) ? 'A' : '-',
								(Finfo.fdate >> 9) + 1980, (Finfo.fdate >> 5) & 15, Finfo.fdate & 31,
								(Finfo.ftime >> 11), (Finfo.ftime >> 5) & 63,
								Finfo.fsize, Finfo.fname);
					}
					xprintf("%4u File(s),%10lu bytes total\r%4u Dir(s)", s1, p1, s2);
					res = f_getfree("", (DWORD *)&p1, &fs);
					if(res == FR_OK) {
						xprintf(", %10lu bytes free\r", p1 * fs->csize * 512);
					} else {
						put_rc(res);
					}
					break;

				case 'o' :	// fo <mode> <file> - Open a file
					put_rc(f_open(&File[0], "teste.txt", (BYTE)(FA_WRITE | FA_CREATE_ALWAYS)));
					break;

				case 'c' :	// fc - Close a file
					put_rc(f_close(&File[0]));
					break;

				case 'e' :	// fe - Seek file pointer
					// 	if(!xatoi(&ptr, &p1)) {
					// 		break;
					// 	}
					// 	res = f_lseek(&File[0], p1);
					// 	put_rc(res);
					// 	if(res == FR_OK) {
					// 		xprintf("fptr=%lu(0x%lX)\r", File[0].fptr, File[0].fptr);
					// 	}
					break;

				case 'd' :	// fd <len> - read and dump file from current fp
					p1 = 16;
					ofs = File[0].fptr;
					while(p1) {
						if((UINT)p1 >= 16) {
							cnt = 16;
							p1 -= 16;
						} else 				{
							cnt = p1;
							p1 = 0;
						}
						res = f_read(&File[0], Buff, cnt, &cnt);
						if(res != FR_OK) {
							put_rc(res);
							break;
						}
						if(!cnt) {
							break;
						}
						put_dump(Buff, ofs, cnt, DW_CHAR);
						ofs += 16;
					}
					break;

				case 'r' :	// fr <len> - read file
					p1 = 0xFFFFFF;
					p2 = 0;
					Timer = 0;
					while(p1) {
						if((UINT)p1 >= blen) {
							cnt = blen;
							p1 -= blen;
						} else {
							cnt = p1;
							p1 = 0;
						}
						res = f_read(&File[0], Buff, cnt, &s2);
						if(res != FR_OK) {
							put_rc(res);
							break;
						}
						p2 += s2;
						if(cnt != s2) {
							break;
						}
					}
					xprintf("%lu bytes read with %lu kB/sec.\r", p2, Timer ? (p2 / Timer) : 0);
					break;

				case 'w' :	// fw <len> <val> - write file
//					sprintf((char *)Buff, "qwertyuiop1234567890");
					f_open(&File[0], "teste.txt", (BYTE)(FA_WRITE | FA_CREATE_ALWAYS));
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
						res = f_write(&File[0], dummyStringData, cnt, &s2);
						if(res != FR_OK) {
							put_rc(res);
							break;
						}
						p2 += s2;
						if(cnt != s2) {
							break;
						}
					}
					xprintf("%lu bytes written with %lu kB/sec (%lums).\r", p2, Timer ? (p2 / Timer) : 0, Timer);
					f_close(&File[0]);
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
					put_rc(f_truncate(&File[0]));
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
					// res = f_open(&File[0], ptr, FA_OPEN_EXISTING | FA_READ);
					// xputc('\r');
					// if(res) {
					// 	put_rc(res);
					// 	break;
					// }
					// xprintf("Creating \"%s\"", ptr2);
					// res = f_open(&File[1], ptr2, FA_CREATE_ALWAYS | FA_WRITE);
					// xputc('\r');
					// if(res) {
					// 	put_rc(res);
					// 	f_close(&File[0]);
					// 	break;
					// }
					// xprintf("Copying file...");
					// Timer = 0;
					// p1 = 0;
					// for(;;) {
					// 	res = f_read(&File[0], Buff, blen, &s1);
					// 	if(res || s1 == 0) {
					// 		break;    /* error or eof */
					// 	}
					// 	res = f_write(&File[1], Buff, s1, &s2);
					// 	p1 += s2;
					// 	if(res || s2 < s1) {
					// 		break;    /* error or disk full */
					// 	}
					// }
					// xprintf("\r%lu bytes copied with %lu kB/sec.\r", p1, p1 / Timer);
					// f_close(&File[0]);
					// f_close(&File[1]);
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
		long *res		// Pointer to the valiable to store the value
)
{
	unsigned long val;
	unsigned char c, r, s = 0;


	*res = 0;

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

	*res = val;
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
