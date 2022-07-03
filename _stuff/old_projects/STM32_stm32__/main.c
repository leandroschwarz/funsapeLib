/*----------------------------------------------------------------------*/
/* FAT file system sample project for FatFs            (C)ChaN, 2018    */
/*----------------------------------------------------------------------*/

#include <string.h>
#include "STM32F100.h"
#include "uart_stm32f1.h"
#include "rtc_stm32f1.h"
#include "xprintf.h"

/*---------------------------------------------*/
/* 1kHz timer process                          */
/*---------------------------------------------*/

void SysTick_Handler(void)
{
	static uint16_t led;


	Timer++;	/* Increment performance counter */

	if(++led >= 500) {
		led = 0;
		GPIOC_ODR ^= _BV(9) | _BV(8);		/* Flip Green/Blue LED state */
	}

	disk_timerproc();	/* Disk timer process */
}



/*---------------------------------------------------------*/
/* User provided RTC function for FatFs module             */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called back     */
/* from FatFs module.                                      */

#if !FF_FS_NORTC && !FF_FS_READONLY
DWORD get_fattime(void)
{
	RTCTIME rtc;

	/* Get local time */
	if(!rtc_gettime(&rtc)) {
		return 0;
	}

	/* Pack date and time into a DWORD variable */
	return ((DWORD)(rtc.year - 1980) << 25)
			| ((DWORD)rtc.month << 21)
			| ((DWORD)rtc.mday << 16)
			| ((DWORD)rtc.hour << 11)
			| ((DWORD)rtc.min << 5)
			| ((DWORD)rtc.sec >> 1);
}
#endif


/*--------------------------------------------------------------------------*/
/* Monitor                                                                  */
/*--------------------------------------------------------------------------*/

static
FRESULT scan_files(
		char *path		/* Pointer to the path name working buffer */
)
{
	DIR dirs;
	FRESULT res;
	BYTE i;


	if((res = f_opendir(&dirs, path)) == FR_OK) {
		i = strlen(path);
		while(((res = f_readdir(&dirs, &Finfo)) == FR_OK) && Finfo.fname[0]) {
			if(Finfo.fattrib & AM_DIR) {
				AccDirs++;
				*(path + i) = '/'; strcpy(path + i + 1, Finfo.fname);
				res = scan_files(path);
				*(path + i) = '\0';
				if(res != FR_OK) {
					break;
				}
			} else {
				/*	xprintf("%s/%s\n", path, fn); */
				AccFiles++;
				AccSize += Finfo.fsize;
			}
		}
	}

	return res;
}



static
void put_rc(FRESULT rc)
{
	const char *str =
			"OK\0" "DISK_ERR\0" "INT_ERR\0" "NOT_READY\0" "NO_FILE\0" "NO_PATH\0"
			"INVALID_NAME\0" "DENIED\0" "EXIST\0" "INVALID_OBJECT\0" "WRITE_PROTECTED\0"
			"INVALID_DRIVE\0" "NOT_ENABLED\0" "NO_FILE_SYSTEM\0" "MKFS_ABORTED\0" "TIMEOUT\0"
			"LOCKED\0" "NOT_ENOUGH_CORE\0" "TOO_MANY_OPEN_FILES\0" "INVALID_PARAMETER\0";
	FRESULT i;

	for(i = 0; i != rc && *str; i++) {
		while(*str++) ;
	}
	xprintf("rc=%u FR_%s\n", (UINT)rc, str);
}


static
const char HelpMsg[] =
		"[Disk contorls]\n"
		" di <pd#> - Initialize disk\n"
		" dd [<pd#> <lba>] - Dump a secrtor\n"
		" ds <pd#> - Show disk status\n"
		"[Buffer controls]\n"
		" bd <ofs> - Dump working buffer\n"
		" be <ofs> [<data>] ... - Edit working buffer\n"
		" br <pd#> <lba> [<count>] - Read disk into working buffer\n"
		" bw <pd#> <lba> [<count>] - Write working buffer into disk\n"
		" bf <val> - Fill working buffer\n"
		"[File system controls]\n"
		" fi <ld#> [<mount>]- Force initialized the volume\n"
		" fs [<path>] - Show volume status\n"
		" fl [<path>] - Show a directory\n"
		" fo <mode> <file> - Open a file\n"
		" fc - Close the file\n"
		" fe <ofs> - Move fp in normal seek\n"
		" fd <len> - Read and dump the file\n"
		" fr <len> - Read the file\n"
		" fw <len> <val> - Write to the file\n"
		" fn <org.name> <new.name> - Rename an object\n"
		" fu <name> - Unlink an object\n"
		" fv - Truncate the file at current fp\n"
		" fk <name> - Create a directory\n"
		" fa <atrr> <mask> <object name> - Change attribute of an object\n"
		" ft <year> <month> <day> <hour> <min> <sec> <name> - Change timestamp of an object\n"
		" fx <src.file> <dst.file> - Copy a file\n"
		" fg <path> - Change current directory\n"
		" fq - Show current directory\n"
		" fb <name> - Set volume label\n"
		" fm <ld#> <type> <csize> - Create file system\n"
		" fz [<len>] - Change/Show R/W length for fr/fw/fx command\n"
		"[Misc commands]\n"
		" md[b|h|w] <addr> [<count>] - Dump memory\n"
		" mf <addr> <value> <count> - Fill memory\n"
		" me[b|h|w] <addr> [<value> ...] - Edit memory\n"
		" t [<year> <mon> <mday> <hour> <min> <sec>] - Set/Show RTC\n"
		"\n";



int main(void)
{
	xdev_in
}
