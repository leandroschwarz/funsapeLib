
#include "myFunctions.hpp"


char SDPath[4];


void sdCardInit(FATFS * SDFatFs)
{
	if (FATFS_LinkDriver(&SD_Driver, SDPath) == 0) {
		if (f_mount(SDFatFs, (TCHAR const*)SDPath, 0) != FR_OK) {
			Error_Handler();
		}
	}
}

void sdCardDeinit(void)
{
	FATFS_UnLinkDriver (SDPath);
}
