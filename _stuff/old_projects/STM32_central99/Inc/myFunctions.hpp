
#include "main.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h"

#ifndef __MY_FUNCTIONS_HPP
#define __MY_FUNCTIONS_HPP

#define ADC_CHANNELS_IN_SAMPLE				6
#define ADC_SAMPLES_IN_CIRCULAR_BUFFER		10
#define ADC_BYTES_IN_CHANNEL				2

#define ADC_CHANNELS_IN_CIRCULAR_BUFFER		(ADC_CHANNELS_IN_SAMPLE * ADC_SAMPLES_IN_CIRCULAR_BUFFER)
#define ADC_BYTES_IN_SAMPLE					(ADC_BYTES_IN_CHANNEL * ADC_CHANNELS_IN_SAMPLE)
#define ADC_BYTES_IN_CIRCULAR_BUFFER		(ADC_BYTES_IN_SAMPLE * ADC_SAMPLES_IN_CIRCULAR_BUFFER)


extern Diskio_drvTypeDef SD_Driver;

void sdCardInit(FATFS * SDFatFs);
void sdCardDeinit(void);

#endif
