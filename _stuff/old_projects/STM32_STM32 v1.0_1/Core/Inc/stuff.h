

#ifndef __STUFF_H
#define __STUFF_H

typedef int inv_error_t;

#define MY_MPL_LOGE(string) do{			\
		char auxString[] = string;		\
		HAL_UART_Transmit(&huart3, (uint8_t *)auxString, strlen(auxString), 100);\
	} while(0)


#endif
