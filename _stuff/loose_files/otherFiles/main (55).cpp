/* =============================================================================
 * Project:		MAX30100 example application
 * File name:	main.cpp
 * Authors:		Leandro Schwarz
 * Date:		June 24, 2018
 * ========================================================================== */

// =============================================================================
// Include files
// =============================================================================

#include <globalDefines.hpp>
#include <externalInterrupt.hpp>
#include <timer0.hpp>
#include <timer2.hpp>
#include <twiMaster.hpp>
#include <usart0.hpp>
#include <devices/max30102.hpp>
#include <FatFs/ff.h>
#include <FatFs/mmc_avr.h>
#include <util/circularBuffer32.hpp>
#include <avr/pgmspace.h>
#include <util/atomic.h>

// =============================================================================
// File exclusive - Constants
// =============================================================================

#define LOGIC_ANALYZER_BIT		PD5
#define LOGIC_ANALYZER_DIR		DDRD
#define LOGIC_ANALYZER_OUT		PORTD

#define BUTTON_DOWN_BIT			PORTC
#define BUTTON_DOWN_DIR			DDRC
#define BUTTON_DOWN_IN			PINC
#define BUTTON_DOWN_INT			PCINT17_PIN
#define BUTTON_DOWN_OUT			PC1
#define BUTTON_START_BIT		PORTC
#define BUTTON_START_DIR		DDRC
#define BUTTON_START_IN			PINC
#define BUTTON_START_INT		PCINT18_PIN
#define BUTTON_START_OUT		PC2
#define BUTTON_STOP_BIT			PORTC
#define BUTTON_STOP_DIR			DDRC
#define BUTTON_STOP_IN			PINC
#define BUTTON_STOP_INT			PCINT19_PIN
#define BUTTON_STOP_OUT			PC3
#define BUTTON_UP_BIT			PORTC
#define BUTTON_UP_DIR			DDRC
#define BUTTON_UP_IN			PINC
#define BUTTON_UP_INT			PCINT16_PIN
#define BUTTON_UP_OUT			PC0

#define NUMBER_OF_TASKS			10
#define SAMPLES_PER_RECORD		13
#define SIGNALS_PER_SAMPLE		2
#define BYTES_PER_SIGNAL		4


int freeRam ()
{
	extern int __heap_start, *__brkval;
	int v;
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

// =============================================================================
// Data Stored in Flash
// *INDENT-OFF*
// =============================================================================

// Strings
const char errorCircularBuffer[]		PROGMEM = "[ERROR] - Failed to allocate memory to circular buffer!\r";
const char errorFileConfig[]			PROGMEM = "[ERROR] - Failed to load task configuration file: file must be badly formatted!\r";
const char errorFileHeader[]			PROGMEM = "[ERROR] - Failed to create HEADER file: error %d!\r";
const char errorFileOpen[]				PROGMEM = "[ERROR] - Failed to open file: error %d!\r";
const char errorFilePpg[]				PROGMEM = "[ERROR] - Failed to create PPG file: error %d!\r";
const char errorSdMount[]				PROGMEM = "[ERROR] - Failed to mount SD card: error %d!\r";
const char infoConfigSystemComp[]		PROGMEM = "[INFO!] - Configuring system components...\r";
const char infoCreateHeaderFile[]		PROGMEM = "[INFO!] - Creating new HEADER file: %s\r";
const char infoCreatePpgFile[]			PROGMEM = "[INFO!] - Creating new PPG file: %s\r";
const char infoFreeRamMemory[]			PROGMEM = "[INFO!] - Free RAM memory = %d bytes!\r";
const char infoMemoryCircularBuffer[]	PROGMEM = "[INFO!] - Allocating memory to circular buffer...\r";
const char infoOpenConfigFile[]			PROGMEM = "[INFO!] - Opening task configuration file...\r";
const char infoPpgFilesFound[]			PROGMEM = "[INFO!] - PPG files found: %d!\r";
const char infoPpgFilesNotFound[]		PROGMEM = "[INFO!] - No old PPG files where found!\r";
const char infoSdMounting[]				PROGMEM = "[INFO!] - Mounting SD card...\r";
const char infoSearchPpgFiles[]			PROGMEM = "[INFO!] - Searching for old PPG files...\r";
const char infoSelectedTask[]			PROGMEM = "[INFO!] - Selected Task: %s\r";
const char infoStartingTask[]			PROGMEM = "[INFO!] - Starting task %s...\r";
const char infoTasksLoaded[]			PROGMEM = "[INFO!] - Loading finished: %d task where loaded!\r";
const char infoTasksLoading[]			PROGMEM = "[INFO!] - Loading task configuration data from file...\r";
const char infoUsartWorking[]			PROGMEM = "[INFO!] - If you can see this message, USART is working!\r";
const char infoWaitingCommand[]			PROGMEM = "[INFO!] - Waiting for command!\r\r";
const char okButtonDown[]				PROGMEM = "[ OK! ] - DOWN button enabled!\r";
const char okButtonStart[]				PROGMEM = "[ OK! ] - START button enabled!\r";
const char okButtonStop[]				PROGMEM = "[ OK! ] - STOP button enabled!\r";
const char okButtonUp[]					PROGMEM = "[ OK! ] - UP button enabled!\r";
const char okCircularBuffer[]			PROGMEM = "[ OK! ] - Circular buffer ready!\r";
const char okFileConfig[]				PROGMEM = "[ OK! ] - Task configuration file found!\r";
const char okFileHeader[]				PROGMEM = "[ OK! ] - Header file created successfully!\r";
const char okFilePpg[]					PROGMEM = "[ OK! ] - PPG file ready!\r";
const char okPulseSensor[]				PROGMEM = "[ OK! ] - Pulse Sensor configured!\r";
const char okSdMount[]					PROGMEM = "[ OK! ] - SD card mounting successful!\r";
const char okStopwatch[]				PROGMEM = "[ OK! ] - Stopwatch enabled!\r";
const char okSystemConfig[]				PROGMEM = "[ OK! ] - System configuration ready!\r";
const char taskRecorded[]				PROGMEM = "[TASK!] - Recorded %ld bytes: %ld samples of %d (%ld and %ld still in buffers)!\r";
const char taskRunnung[]				PROGMEM = "[TASK!] - Task Running...\r";
const char taskStopped[]				PROGMEM = "[TASK!] - Task Stopped: %ld samples of %ld were recorded (%ld and %ld still in buffer)!\r";
const char warnSystemHalted[]			PROGMEM = "\r[WARN!] - System is halted, press reset when ready!\r";

// String array
const char *const memStr[] PROGMEM = {
	errorCircularBuffer, errorFileConfig, errorFileHeader, errorFileOpen,
	errorFilePpg, errorSdMount, infoConfigSystemComp, infoCreateHeaderFile,
	infoCreatePpgFile, infoFreeRamMemory, infoMemoryCircularBuffer,
	infoOpenConfigFile, infoPpgFilesFound, infoPpgFilesNotFound, infoSdMounting,
	infoSearchPpgFiles, infoSelectedTask, infoStartingTask, infoTasksLoaded,
	infoTasksLoading, infoUsartWorking, infoWaitingCommand, okButtonDown,
	okButtonStart, okButtonStop, okButtonUp, okCircularBuffer, okFileConfig,
	okFileHeader, okFilePpg, okPulseSensor, okSdMount, okStopwatch,
	okSystemConfig, taskRecorded, taskRunnung, taskStopped, warnSystemHalted
};

// =============================================================================
// *INDENT-ON*
// File exclusive - New data types
// =============================================================================

typedef enum memoryString_e {
	ERROR_CIRCULAR_BUFFER,
	ERROR_FILE_CONFIG,
	ERROR_FILE_HEADER,
	ERROR_FILE_OPEN,
	ERROR_FILE_PPG,
	ERROR_SD_MOUNT,
	INFO_CONFIG_SYSTEM_COMP,
	INFO_CREATE_HEADER_FILE,
	INFO_CREATE_PPG_FILE,
	INFO_FREE_RAM_MEMORY,
	INFO_MEMORY_CIRCULAR_BUFFER,
	INFO_OPEN_CONFIG_FILE,
	INFO_PPG_FILES_FOUND,
	INFO_PPG_FILES_NOT_FOUND,
	INFO_SD_MOUNTING,
	INFO_SEARCH_PPG_FILES,
	INFO_SELECTED_TASK,
	INFO_STARTING_TASK,
	INFO_TASKS_LOADED,
	INFO_TASKS_LOADING,
	INFO_USART_WORKING,
	INFO_WAITING_COMMAND,
	OK_BUTTON_DOWN,
	OK_BUTTON_START,
	OK_BUTTON_STOP,
	OK_BUTTON_UP,
	OK_CIRCULAR_BUFFER,
	OK_FILE_CONFIG,
	OK_FILE_HEADER,
	OK_FILE_PPG,
	OK_PULSE_SENSOR,
	OK_SD_MOUNT,
	OK_STOP_WATCH,
	OK_SYSTEM_CONFIG,
	TASK_RECORDED,
	TASK_RUNNUNG,
	TASK_STOPPED,
	WARN_SYSTEM_HALTED
} memoryString_e;

typedef struct task_s {
	max30102LedCurrent_e		ledCurrent;
	max30102SensorSampleRate_e	sampleRate	: 3;
	max30102SensorFullScale_e	fullScale	: 2;
	uint16_t					duration	: 11;
	char						testName[17];
} task_s;

typedef enum token_e {
	TOKEN_NONE				= 0,
	TOKEN_START_COMMAND		= 1,
	TOKEN_END_COMMAND		= 2,
	TOKEN_START_VALUE		= 3,
	TOKEN_END_VALUE			= 4,
	TOKEN_ASSIGNMENT		= 5,
	TOKEN_SEPARATOR			= 6,
} token_e;

typedef enum command_e {
	COMMAND_NONE			= 0,
	COMMAND_TEST_NAME		= 1,
	COMMAND_LED_CURRENT		= 2,
	COMMAND_SAMPLE_RATE		= 3,
	COMMAND_FULL_SCALE		= 4,
	COMMAND_DURATION		= 5,
} command_e;

typedef union projectFlags_f {
	struct {
		bool taskRunning			: 1;
		bool newDataAwaiting		: 1;
		bool commandTaskUp			: 1;
		bool commandTaskDown		: 1;
		bool commandTaskStop		: 1;
		bool commandTaskStart		: 1;
		bool checkFreeRam			: 1;
		uint8_t sdRefreshCounter	: 4;
		uint8_t currentTask			: 4;
		uint8_t lastTask			: 4;
		uint32_t unusedBits			: 5;
	};
	uint8_t allFlags[3];
} projectFlags_f;

// =============================================================================
// File exclusive - Macro-functions
// =============================================================================

/* NONE */

// =============================================================================
// Global variables
// =============================================================================

volatile projectFlags_f	projectFlags;
vuint32_t testTotalBytes = 0;
vuint32_t testTotalSamples = 0;
vuint32_t testReadBytes = 0;
vuint32_t testReadSamples = 0;
vuint32_t testStoredBytes = 0;
vuint32_t testStoredSamples = 0;
uint8_t aux8 = 0;
uint16_t aux16 = 0;
uint32_t aux32 = 0;

// =============================================================================
// Function declarations
// =============================================================================

bool processLine(char *line, task_s *task);
max30102LedCurrent_e decodeLedCurrent(char *string);
max30102SensorSampleRate_e decodeSampleRate(char *string);
max30102SensorFullScale_e decodeFullScale(char *string);

// =============================================================================
// Main function
// =============================================================================

int main()
{
	// =========================================================================
	// General purpose variables
	// =========================================================================

	task_s task[NUMBER_OF_TASKS];
	FATFS sdCard;
	FIL file;
	FRESULT res;
	uint16_t logFileNumber = 1;
	Usart0 uart(USART_BAUD_57600);
	CircularBuffer32 redBuffer(SAMPLES_PER_RECORD);
	CircularBuffer32 infraredBuffer(SAMPLES_PER_RECORD);
	Max30102 pulseSensor;

	// =========================================================================
	// Variable initialization
	// =========================================================================

	testTotalSamples = 0;
	testTotalBytes = 0;
	testTotalSamples = 0;
	testStoredBytes = 0;
	testStoredSamples = 0;
	testReadBytes = 0;
	testReadSamples = 0;
	projectFlags.allFlags[0] = 0;
	projectFlags.allFlags[1] = 0;
	projectFlags.allFlags[2] = 0;
	for (uint8_t i = 0; i < NUMBER_OF_TASKS; i++) {
		task[i].ledCurrent = MAX30102_LED_OFF;
		task[i].sampleRate = MAX30102_SENSOR_SAMPLE_RATE_50_HZ;
		task[i].fullScale = MAX30102_SENSOR_FULL_SCALE_2048_NA;
		task[i].duration = 0;
		task[i].testName[0] = '\0';
	}

	// =========================================================================
	// Logic analyzer trigger signal
	// =========================================================================

	do {
		setBit(LOGIC_ANALYZER_DIR, LOGIC_ANALYZER_BIT);
		setBit(LOGIC_ANALYZER_OUT, LOGIC_ANALYZER_BIT);
		_delay_ms(50);
		clrBit(LOGIC_ANALYZER_OUT, LOGIC_ANALYZER_BIT);
		_delay_ms(50);
		setBit(LOGIC_ANALYZER_OUT, LOGIC_ANALYZER_BIT);
		_delay_ms(50);
		clrBit(LOGIC_ANALYZER_OUT, LOGIC_ANALYZER_BIT);
	} while (0);

	// =========================================================================
	// USART configuration
	// =========================================================================

	do {
		uart.initialize();
		uart.enableTransmitter();
		uart.stdio();
		printf("MAX30102 Data Logger v 1.0\r\r");
		printf_P(memStr[INFO_USART_WORKING]);
		printf_P(memStr[INFO_CONFIG_SYSTEM_COMP]);
	} while (0);

	// =========================================================================
	// CircularBuffer16 configuration
	// =========================================================================

	do {
		printf_P(memStr[INFO_MEMORY_CIRCULAR_BUFFER]);
		if (redBuffer.getLastError() || infraredBuffer.getLastError()) {
			printf_P(memStr[ERROR_CIRCULAR_BUFFER]);
			printf_P(memStr[WARN_SYSTEM_HALTED]);
			systemHalt();
		}
		printf_P(memStr[OK_CIRCULAR_BUFFER]);
	} while (0);

	// =========================================================================
	// SD Card configuration
	// =========================================================================

	do {
		char logFileName[] = "PPG00001.DAT";

		// Mount SD card
		printf_P(memStr[INFO_SD_MOUNTING]);
		res = f_mount(&sdCard, "", 0);
		if (res != FR_OK) {
			printf_P(memStr[ERROR_SD_MOUNT], res);
			printf_P(memStr[WARN_SYSTEM_HALTED]);
			systemHalt();
		}
		printf_P(memStr[OK_SD_MOUNT]);

		// Open configuration file
		printf_P(memStr[INFO_OPEN_CONFIG_FILE]);
		strcpy(logFileName, "system.cfg");
		res = f_open(&file, logFileName, FA_OPEN_EXISTING | FA_READ);
		if (res != FR_OK) {
			printf_P(memStr[ERROR_FILE_OPEN], res);
			printf_P(memStr[WARN_SYSTEM_HALTED]);
			systemHalt();
		}
		printf_P(memStr[OK_FILE_CONFIG]);

		// Loads configurations from file
		printf_P(memStr[INFO_TASKS_LOADING]);
		for (aux8 = 0; aux8 < NUMBER_OF_TASKS;) {
			char strAux[100];
			// Reads a line from file and breaks if NULL
			if (f_gets(strAux, 100, &file) == NULL) {
				break;
			}
			// Process line to extract tasks
			if ((strcmp(strAux, "") != 0) && (strcmp(strAux, "\n") != 0)) {
				if (processLine(strAux, &(task[aux8])) == false) {
					printf_P(memStr[ERROR_FILE_CONFIG]);
					printf_P(memStr[WARN_SYSTEM_HALTED]);
					systemHalt();
				}
				aux8++;
			}
		}
		printf_P(memStr[INFO_TASKS_LOADED], aux8);
		projectFlags.lastTask = aux8 - 1;
		f_close(&file);

		// SD card - Search for the last created filename
		printf_P(memStr[INFO_SEARCH_PPG_FILES]);
		for (logFileNumber = 1; logFileNumber < 9999; logFileNumber++) {
			sprintf(logFileName, "PPG%05u.dat", logFileNumber);
			res = f_open(&file, logFileName, FA_OPEN_EXISTING | FA_READ);
			if (res != FR_OK) {
				break;
			}
			f_close(&file);
		}
		if (logFileNumber == 1) {
			printf_P(memStr[INFO_PPG_FILES_NOT_FOUND]);
		} else {
			printf_P(memStr[INFO_PPG_FILES_FOUND], (logFileNumber - 1));
		}
	} while (0);

	// =========================================================================
	// PulseSensor configuration
	// =========================================================================

	do {
		// TWI configuration
		twiMaster.setClockSpeed(100000);

		// Pulse sensor configuration
		pulseSensor.init(MAX30102_SENSOR_SAMPLE_RATE_100_HZ,
						 MAX30102_SENSOR_RESOLUTION_18_BITS,
						 MAX30102_SENSOR_FULL_SCALE_4096_NA);
		pulseSensor.shutDown();
		pulseSensor.fifoConfig(MAX30102_FIFO_AVERAGE_8, true, 0);
		pulseSensor.setSpo2Mode(MAX30102_LED_CURRENT_10_MA, MAX30102_LED_CURRENT_10_MA);
		pulseSensor.activateInterrupts(MAX30102_INTERRUPT_FIFO_NEW_DATA);
		pulseSensor.getInterruptStatus(MAX30102_INTERRUPT_FIFO_NEW_DATA, true);
		pulseSensor.flushFifo();
		printf_P(memStr[OK_PULSE_SENSOR]);

		// INT0 configuration
		int0Config(PORT_INPUT_PULL_UP, INT_SENSE_FALLING_EDGE);
		int0ClearInterruptRequest();
		int0ActivateInterrupt();
	} while (0);

	// =========================================================================
	// BUTTON configuration
	// =========================================================================

	do {
		// TIMER0 configuration
		timer0SetCompareAValue(80);
		timer0SetCounterValue(0);
		timer0ClearCompareAInterruptRequest();
		timer0Config(TIMER0_MODE_CTC_OCRA, TIMER0_PRESCALER_1024);
		timer0ActivateCompareAInterrupt();
		printf_P(memStr[OK_STOP_WATCH]);

		// PCINT2 configuration
		pcint2ActivatePins(	(BUTTON_UP_INT |
							 BUTTON_DOWN_INT |
							 BUTTON_START_INT |
							 BUTTON_STOP_INT),
							PORT_INPUT_PULL_UP);
		pcint2ClearInterruptRequest();
		pcint2Enable();
		printf_P(memStr[OK_BUTTON_UP]);
		printf_P(memStr[OK_BUTTON_DOWN]);
		printf_P(memStr[OK_BUTTON_STOP]);
		printf_P(memStr[OK_BUTTON_START]);
	} while (0);

	// =========================================================================
	// System configuration ready
	// =========================================================================

	do {
		printf_P(memStr[OK_SYSTEM_CONFIG]);
		printf_P(memStr[INFO_WAITING_COMMAND]);
		printf_P(memStr[INFO_SELECTED_TASK],
				 task[projectFlags.currentTask].testName);
		printf_P(memStr[INFO_FREE_RAM_MEMORY], freeRam());
	} while (0);

	// =========================================================================
	// Main loop
	// =========================================================================

	while (1) {

		// =====================================================================
		// Read PPG data
		// =====================================================================
		if (projectFlags.newDataAwaiting) {
			uint32_t redLedValue, infraredLedValue;

			// Reset flag
			projectFlags.newDataAwaiting = false;

			// How many samples are available?
			pulseSensor.getFifoOccupation(&aux8);
			if (aux8 == 0) {
				//something REALLY strange happened!
				// Let's ignore it, and life goes on...
				continue;
			}

			// Reads device data - sample by sample
			redBuffer.blockRead(true);
			redBuffer.blockWrite(false);
			infraredBuffer.blockRead(true);
			infraredBuffer.blockWrite(false);
			for (uint8_t i = 0; i < aux8; i++) {
				pulseSensor.getFifoData(1, &redLedValue, &infraredLedValue);
				// Stores data in buffer
				redBuffer.push(redLedValue);
				infraredBuffer.push(infraredLedValue);
				testReadSamples++;
				testReadBytes += 8;
			}
			infraredBuffer.blockWrite(true);
			infraredBuffer.blockRead(false);
			redBuffer.blockWrite(true);
			redBuffer.blockRead(false);
		}

		// =====================================================================
		// UP button press
		// =====================================================================
		if (projectFlags.commandTaskUp) {

			// Ignores if task is running, starting or stopping
			if (		(projectFlags.taskRunning) ||
						(projectFlags.commandTaskStart) ||
						(projectFlags.commandTaskStop)) {
				// Reset flag
				projectFlags.commandTaskUp = false;
				continue;
			}

			// Increment or cycle to first task
			if (projectFlags.currentTask == projectFlags.lastTask) {
				projectFlags.currentTask = 0;
			} else {
				projectFlags.currentTask++;
			}

			// Update screen
			printf_P(memStr[INFO_SELECTED_TASK],
					 task[projectFlags.currentTask].testName);

			// Reset flag
			projectFlags.commandTaskUp = false;
		}

		// =====================================================================
		// DOWN button press
		// =====================================================================
		if (projectFlags.commandTaskDown) {

			// Ignores if task is running, starting or stopping
			if (		(projectFlags.taskRunning) ||
						(projectFlags.commandTaskStart) ||
						(projectFlags.commandTaskStop)) {
				// Reset flag
				projectFlags.commandTaskDown = false;
				continue;
			}

			// Decrement or cycle to last task
			if (projectFlags.currentTask == 0) {
				projectFlags.currentTask = projectFlags.lastTask;
			} else {
				projectFlags.currentTask--;
			}

			// Update screen
			printf_P(memStr[INFO_SELECTED_TASK],
					 task[projectFlags.currentTask].testName);

			// Reset flag
			projectFlags.commandTaskDown = false;
		}

		// =====================================================================
		// START button press
		// =====================================================================
		if (projectFlags.commandTaskStart) {

			uint16_t auxSampleRate = 0;
			uint16_t auxFullScale = 0;
			uint16_t auxLedCurrent = 0;
			char logFileName[] = "PPG00001.DAT";
			char auxStr[100];

			// Ignores if task is running or stopping
			if ((projectFlags.taskRunning) || (projectFlags.commandTaskStop)) {
				// Reset flag
				projectFlags.commandTaskStart = false;
				continue;
			}

			// Show message
			printf_P(memStr[INFO_STARTING_TASK],
					 task[projectFlags.currentTask].testName);

			// Evaluate sample rate
			do {
				aux16 = task[projectFlags.currentTask].sampleRate;
				auxSampleRate = aux16;
				if (aux16 == 5) {	// Sample Rate = 1000 Hz
					auxSampleRate = 1000;
				} else {
					auxSampleRate = 50;
					if (aux16 > 5) {	// Correction factor
						aux16--;
					}
					while (aux16 > 0) {
						auxSampleRate *= 2;
						aux16--;
					}
				}
			} while (0);
			testTotalSamples = auxSampleRate * task[projectFlags.currentTask].duration;
			testTotalBytes = testTotalSamples * SIGNALS_PER_SAMPLE * BYTES_PER_SIGNAL;

			// Evaluate full scale
			do {
				aux16 = task[projectFlags.currentTask].fullScale;
				auxFullScale = 2048;
				while (aux16 > 0) {
					auxFullScale *= 2;
					aux16--;
				}
			} while (0);

			// Evaluate LED current
			do {
				aux32 = task[projectFlags.currentTask].ledCurrent;
				aux32 *= 50000;
				aux32 /= 255;
				auxLedCurrent = (uint16_t)aux32;
			} while (0);

			// Creates new HEADER file
			do {

				printf_P(memStr[INFO_CREATE_HEADER_FILE], logFileName);
				sprintf(logFileName, "LOG%05u.txt", logFileNumber);
				res = f_open(&file, logFileName, FA_CREATE_ALWAYS | FA_WRITE);
				if (res != FR_OK) {
					printf_P(memStr[ERROR_FILE_HEADER], res);
					printf_P(memStr[WARN_SYSTEM_HALTED]);
					systemHalt();
				}
				sprintf(auxStr, "Detalhes da tarefa\r\n");
				f_write(&file, auxStr, strlen(auxStr), (UINT *)&aux16);
				sprintf(auxStr, "Name = %s\r\n", task[projectFlags.currentTask].testName);
				f_write(&file, auxStr, strlen(auxStr), (UINT *)&aux16);
				sprintf(auxStr, "Corrente do LED = %d uA\r\n", auxLedCurrent);
				f_write(&file, auxStr, strlen(auxStr), (UINT *)&aux16);
				sprintf(auxStr, "F_amost. = %d Hz\r\n", auxSampleRate);
				f_write(&file, auxStr, strlen(auxStr), (UINT *)&aux16);
				sprintf(auxStr, "Escala = %d\r\n", auxFullScale);
				f_write(&file, auxStr, strlen(auxStr), (UINT *)&aux16);
				sprintf(auxStr, "Duração = %d\r\n", task[projectFlags.currentTask].duration);
				f_write(&file, auxStr, strlen(auxStr), (UINT *)&aux16);
				f_sync(&file);
				f_close(&file);
				printf_P(memStr[OK_FILE_HEADER]);
			} while (0);

			// Creates new PPG file
			do {
				printf_P(memStr[INFO_CREATE_PPG_FILE], logFileName);
				sprintf(logFileName, "PPG%05u.dat", logFileNumber);
				res = f_open(&file, logFileName, FA_CREATE_NEW | FA_WRITE);
				if (res != FR_OK) {
					printf_P(memStr[ERROR_FILE_PPG], res);
					printf_P(memStr[WARN_SYSTEM_HALTED]);
					systemHalt();
				}
				aux8 = 0;
				auxStr[aux8++] = 0x18;
				auxStr[aux8++] = 0x02;
				auxStr[aux8++] = (char)wordHighByte(task[projectFlags.currentTask].duration);
				auxStr[aux8++] = (char)wordLowByte(task[projectFlags.currentTask].duration);
				auxStr[aux8++] = (char)wordHighByte(auxSampleRate);
				auxStr[aux8++] = (char)wordLowByte(auxSampleRate);
				auxStr[aux8++] = (char)wordHighByte(auxFullScale);
				auxStr[aux8++] = (char)wordLowByte(auxFullScale);
				auxStr[aux8++] = (char)wordHighByte(auxLedCurrent);
				auxStr[aux8++] = (char)wordLowByte(auxLedCurrent);
				f_write(&file, auxStr, aux8, (UINT *)&aux16);
				f_sync(&file);
				printf_P(memStr[OK_FILE_PPG]);
			} while (0);

			// Reset variables
			testStoredBytes = 0;
			testStoredSamples = 0;
			testReadBytes = 0;
			testReadSamples = 0;

			// Pulse Sensor configuration
			redBuffer.flush();
			infraredBuffer.flush();
			pulseSensor.flushFifo();
			pulseSensor.wakeUp();

			// Task running
			printf_P(memStr[TASK_RUNNUNG]);
			projectFlags.taskRunning = true;

			// Reset flag
			projectFlags.commandTaskStart = false;
		}

		// =====================================================================
		// STOP button press
		// =====================================================================
		if (projectFlags.commandTaskStop) {

			uint8_t bufferIndex = 0;
			uint8_t bufferData[SAMPLES_PER_RECORD * SIGNALS_PER_SAMPLE * BYTES_PER_SIGNAL];

			// Ignores if test is NOT running
			if (!projectFlags.taskRunning) {
				projectFlags.commandTaskStop = false;
				continue;
			}

			// Pulse Sensor disable
			pulseSensor.shutDown();
			pulseSensor.flushFifo();

			// Stores the rest of the data into the SD card
			while ((!redBuffer.isEmpty()) && (!infraredBuffer.isEmpty())) {
				redBuffer.pop(&aux32);
				bufferData[bufferIndex++] = (uint8_t)dwordByte3(aux32);
				bufferData[bufferIndex++] = (uint8_t)dwordByte2(aux32);
				bufferData[bufferIndex++] = (uint8_t)dwordByte1(aux32);
				bufferData[bufferIndex++] = (uint8_t)dwordByte0(aux32);
				testStoredBytes += 4;
				infraredBuffer.pop(&aux32);
				bufferData[bufferIndex++] = (uint8_t)dwordByte3(aux32);
				bufferData[bufferIndex++] = (uint8_t)dwordByte2(aux32);
				bufferData[bufferIndex++] = (uint8_t)dwordByte1(aux32);
				bufferData[bufferIndex++] = (uint8_t)dwordByte0(aux32);
				testStoredBytes += 4;
				testStoredSamples++;
			}
			f_write(&file, (char *)bufferData, bufferIndex, (UINT *)&aux16);
			f_sync(&file);
			f_close(&file);
			logFileNumber++;

			// Task stopped
			printf_P(memStr[TASK_STOPPED], testStoredSamples, testTotalSamples,
					 redBuffer.getOccupation(), infraredBuffer.getOccupation());
			printf_P(memStr[INFO_WAITING_COMMAND]);
			printf_P(memStr[INFO_SELECTED_TASK],
					 task[projectFlags.currentTask].testName);

			redBuffer.flush();
			infraredBuffer.flush();
			projectFlags.taskRunning = false;
			projectFlags.commandTaskStop = false;
		}

		// =====================================================================
		// Recording data
		// =====================================================================
		if ((projectFlags.taskRunning) && (!projectFlags.commandTaskStop)) {
			uint16_t redSamples = redBuffer.getOccupation();
			uint16_t infraredSamples = infraredBuffer.getOccupation();
			uint16_t samples = minimumOf(redSamples, infraredSamples);
			uint8_t bufferData[SAMPLES_PER_RECORD * SIGNALS_PER_SAMPLE * BYTES_PER_SIGNAL];
			uint8_t bufferIndex = 0;
			uint32_t auxRed;
			uint32_t auxInfrared;

			if (samples >= SAMPLES_PER_RECORD) {
				// Transfer buffer
				bufferIndex = 0;
				for (uint16_t i = 0; i < samples; i++) {
					// This must be atomic
					ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
						redBuffer.pop(&auxRed);
						redBuffer.pop(&auxInfrared);
					}
					bufferData[bufferIndex++] = (uint8_t)dwordByte3(auxRed);
					bufferData[bufferIndex++] = (uint8_t)dwordByte2(auxRed);
					bufferData[bufferIndex++] = (uint8_t)dwordByte1(auxRed);
					bufferData[bufferIndex++] = (uint8_t)dwordByte0(auxRed);
					bufferData[bufferIndex++] = (uint8_t)dwordByte3(auxInfrared);
					bufferData[bufferIndex++] = (uint8_t)dwordByte2(auxInfrared);
					bufferData[bufferIndex++] = (uint8_t)dwordByte1(auxInfrared);
					bufferData[bufferIndex++] = (uint8_t)dwordByte0(auxInfrared);
					testStoredBytes += 8;
					testStoredSamples++;
				}
				f_write(&file, (char *)bufferData, bufferIndex, (UINT *)&aux16);
				f_sync(&file);
				printf_P(memStr[TASK_STOPPED], testStoredSamples, testTotalSamples,
						 redBuffer.getOccupation(), infraredBuffer.getOccupation());
			}
		}

		// =====================================================================
		// Check free memory
		// =====================================================================
		if (projectFlags.checkFreeRam) {
			printf_P(memStr[INFO_FREE_RAM_MEMORY], freeRam());
			projectFlags.checkFreeRam = false;
		}

		// =====================================================================
		// END LOOP
		// =====================================================================
	}

	return 0;
}

// =============================================================================
// Interrupt handlers
// =============================================================================

ISR(TIMER0_COMPA_vect)
{
	systemStatus.incrementStopwatch();
	projectFlags.sdRefreshCounter++;
	if (projectFlags.sdRefreshCounter == 10) {
		projectFlags.sdRefreshCounter = 0;
		projectFlags.checkFreeRam = true;
		mmc_disk_timerproc();
	}
}

ISR(INT0_vect)
{
	projectFlags.newDataAwaiting = true;
}

ISR(PCINT2_vect)
{
	// Ignores if less that debounce time
	if (systemStatus.calculateElapsedTime() < GPDSE_DEBOUNCE_TIME) {
		return;
	}

	// Valid button press (set new stopwatch mark)
	systemStatus.setStopwatchMark();

	// CASE 1 -> STOP button
	if (isBitClr(BUTTON_STOP_IN, BUTTON_STOP_BIT)) {
		// CASE 1a -> Task is not running (ignore button press)
		if (!projectFlags.taskRunning) {
			// Reset old flags
			projectFlags.commandTaskStop = false;
			projectFlags.commandTaskStart = false;
			projectFlags.commandTaskUp = false;
			projectFlags.commandTaskDown = false;
			return;
		}
		// CASE 1b -> STOP command NOT issued (send STOP command)
		if (!projectFlags.commandTaskStop) {
			projectFlags.commandTaskStop = true;
		}
		// CASE 1c -> STOP command already issued (ignores and reset old flags)
		projectFlags.commandTaskStart = false;
		projectFlags.commandTaskUp = false;
		projectFlags.commandTaskDown = false;
		return;
	}

	// CASE 2 -> START button
	if (isBitClr(BUTTON_START_IN, BUTTON_START_BIT)) {
		// CASE 2a -> Task is already running (ignore button press)
		if (projectFlags.taskRunning) {
			// Reset old flags
			projectFlags.commandTaskStart = false;
			projectFlags.commandTaskUp = false;
			projectFlags.commandTaskDown = false;
			return;
		}
		// CASE 2b -> START command NOT issued (send START command)
		if (!projectFlags.commandTaskStart) {
			projectFlags.commandTaskStart = true;
		}
		// CASE 2c -> START command already issued (ignores and reset old flags)
		// Reset old flags
		projectFlags.commandTaskUp = false;
		projectFlags.commandTaskDown = false;
		return;
	}

	// CASE 3 -> UP button
	if (isBitClr(BUTTON_UP_IN, BUTTON_UP_BIT)) {
		// CASE 3a -> Task is already running (ignore button press)
		if (projectFlags.taskRunning) {
			// Reset old flags
			projectFlags.commandTaskUp = false;
			projectFlags.commandTaskDown = false;
			return;
		}
		// CASE 3b -> UP command NOT issued (send UP command)
		if (!projectFlags.commandTaskUp) {
			projectFlags.commandTaskUp = true;
		}
		// CASE 3c -> UP command already issued (ignores and reset old flags)
		// Reset old flags
		projectFlags.commandTaskDown = false;
		return;
	}

	// CASE 4 -> DOWN button
	if (isBitClr(BUTTON_DOWN_IN, BUTTON_DOWN_BIT)) {
		// CASE 4a -> Task is already running (ignore button press)
		if (projectFlags.taskRunning) {
			// Reset old flags
			projectFlags.commandTaskUp = false;
			return;
		}
		// CASE 3b -> DOWN command NOT issued (send DOWN command)
		if (!projectFlags.commandTaskDown) {
			projectFlags.commandTaskDown = true;
		}
		// CASE 3c -> DOWN command already issued (ignores and reset old flags)
		// Reset old flags
		projectFlags.commandTaskUp = false;
		return;
	}
}

// =============================================================================
// Functions definitions
// =============================================================================

bool processLine(char *line, task_s *task)
{
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t aux8 = 0;
	command_e command = COMMAND_NONE;
	char auxStr[17];
	token_e lastToken = TOKEN_NONE;

	// Process line for tokens (checks file formatting)
	aux8 = strlen(line);
	for (i = 0; i < aux8; i++) {
		switch (line[i]) {
		// Start command token
		case '{':
			if ((lastToken != TOKEN_NONE) && (lastToken != TOKEN_SEPARATOR)) {
				return false;
			}
			lastToken = TOKEN_START_COMMAND;
			j = 0;
			continue;

		// End command token
		case '}':
			if (lastToken != TOKEN_START_COMMAND) {
				return false;
			}
			auxStr[j] = '\0';
			if (strcmp(auxStr, "testName") == 0) {
				command = COMMAND_TEST_NAME;
			} else if (strcmp(auxStr, "ledCurrent") == 0) {
				command = COMMAND_LED_CURRENT;
			} else if (strcmp(auxStr, "sampleRate") == 0) {
				command = COMMAND_SAMPLE_RATE;
			} else if (strcmp(auxStr, "fullScale") == 0) {
				command = COMMAND_FULL_SCALE;
			} else if (strcmp(auxStr, "duration") == 0) {
				command = COMMAND_DURATION;
			} else {
				return false;
			}
			lastToken = TOKEN_END_COMMAND;
			continue;

		// Equation token
		case '=':
			if (lastToken != TOKEN_END_COMMAND) {
				return false;
			}
			lastToken = TOKEN_ASSIGNMENT;
			continue;

		// Start value token
		case '[':
			if (lastToken != TOKEN_ASSIGNMENT) {
				return false;
			}
			lastToken = TOKEN_START_VALUE;
			j = 0;
			continue;

		// End value token
		case ']':
			if (lastToken != TOKEN_START_VALUE) {
				return false;
			}
			auxStr[j] = '\0';
			if (command == COMMAND_TEST_NAME) {
				strcpy(task->testName, auxStr);
			} else if (command == COMMAND_LED_CURRENT) {
				task->ledCurrent = decodeLedCurrent(auxStr);
				if (task->ledCurrent == MAX30102_LED_OFF) {
					return false;
				}
			} else if (command == COMMAND_SAMPLE_RATE) {
				task->sampleRate = decodeSampleRate(auxStr);
			} else if (command == COMMAND_FULL_SCALE) {
				task->fullScale = decodeFullScale(auxStr);
			} else if (command == COMMAND_DURATION) {
				task->duration = atoi(auxStr);
				if (task->duration == 0) {
					return false;
				}
			} else {
				return false;
			}
			lastToken = TOKEN_END_VALUE;
			continue;

		// Separator token
		case ';':
			if (lastToken != TOKEN_END_VALUE) {
				return false;
			}
			lastToken = TOKEN_SEPARATOR;
			continue;

		// LF token
		case '\n':
			continue;
		// EOL token
		case '\0':
			return true;

		// Character
		default:
			auxStr[j++] = line[i];
			continue;
		}
	}

	return true;
}

max30102LedCurrent_e decodeLedCurrent(char *string)
{
	max30102LedCurrent_e ledCurrent = MAX30102_LED_OFF;

	if (strcmp(string, "200_uA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_200_UA;
	} else if (strcmp(string, "400_uA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_400_UA;
	} else if (strcmp(string, "800_uA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_800_UA;
	} else if (strcmp(string, "1_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_1_MA;
	} else if (strcmp(string, "2_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_2_MA;
	} else if (strcmp(string, "3_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_3_MA;
	} else if (strcmp(string, "4_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_4_MA;
	} else if (strcmp(string, "5_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_5_MA;
	} else if (strcmp(string, "6_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_6_MA;
	} else if (strcmp(string, "7_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_7_MA;
	} else if (strcmp(string, "8_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_8_MA;
	} else if (strcmp(string, "9_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_9_MA;
	} else if (strcmp(string, "10_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_10_MA;
	} else if (strcmp(string, "11_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_11_MA;
	} else if (strcmp(string, "12_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_12_MA;
	} else if (strcmp(string, "13_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_13_MA;
	} else if (strcmp(string, "14_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_14_MA;
	} else if (strcmp(string, "15_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_15_MA;
	} else if (strcmp(string, "16_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_16_MA;
	} else if (strcmp(string, "17_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_17_MA;
	} else if (strcmp(string, "18_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_18_MA;
	} else if (strcmp(string, "19_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_19_MA;
	} else if (strcmp(string, "20_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_20_MA;
	} else if (strcmp(string, "21_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_21_MA;
	} else if (strcmp(string, "22_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_22_MA;
	} else if (strcmp(string, "23_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_23_MA;
	} else if (strcmp(string, "24_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_24_MA;
	} else if (strcmp(string, "25_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_25_MA;
	} else if (strcmp(string, "26_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_26_MA;
	} else if (strcmp(string, "27_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_27_MA;
	} else if (strcmp(string, "28_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_28_MA;
	} else if (strcmp(string, "29_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_29_MA;
	} else if (strcmp(string, "30_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_30_MA;
	} else if (strcmp(string, "31_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_31_MA;
	} else if (strcmp(string, "32_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_32_MA;
	} else if (strcmp(string, "33_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_33_MA;
	} else if (strcmp(string, "34_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_34_MA;
	} else if (strcmp(string, "35_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_35_MA;
	} else if (strcmp(string, "36_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_36_MA;
	} else if (strcmp(string, "37_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_37_MA;
	} else if (strcmp(string, "38_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_38_MA;
	} else if (strcmp(string, "39_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_39_MA;
	} else if (strcmp(string, "40_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_40_MA;
	} else if (strcmp(string, "41_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_41_MA;
	} else if (strcmp(string, "42_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_42_MA;
	} else if (strcmp(string, "43_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_43_MA;
	} else if (strcmp(string, "44_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_44_MA;
	} else if (strcmp(string, "45_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_45_MA;
	} else if (strcmp(string, "46_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_46_MA;
	} else if (strcmp(string, "47_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_47_MA;
	} else if (strcmp(string, "48_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_48_MA;
	} else if (strcmp(string, "49_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_49_MA;
	} else if (strcmp(string, "50_mA") == 0) {
		ledCurrent = MAX30102_LED_CURRENT_50_MA;
	}

	return ledCurrent;
}

max30102SensorSampleRate_e decodeSampleRate(char *string)
{
	max30102SensorSampleRate_e sampleRate = MAX30102_SENSOR_SAMPLE_RATE_50_HZ;

	if (strcmp(string, "50") == 0) {
		sampleRate = MAX30102_SENSOR_SAMPLE_RATE_50_HZ;
	} else if (strcmp(string, "100") == 0) {
		sampleRate = MAX30102_SENSOR_SAMPLE_RATE_100_HZ;
	} else if (strcmp(string, "200") == 0) {
		sampleRate = MAX30102_SENSOR_SAMPLE_RATE_200_HZ;
	} else if (strcmp(string, "400") == 0) {
		sampleRate = MAX30102_SENSOR_SAMPLE_RATE_400_HZ;
	} else if (strcmp(string, "800") == 0) {
		sampleRate = MAX30102_SENSOR_SAMPLE_RATE_800_HZ;
	} else if (strcmp(string, "1000") == 0) {
		sampleRate = MAX30102_SENSOR_SAMPLE_RATE_1000_HZ;
	} else if (strcmp(string, "1600") == 0) {
		sampleRate = MAX30102_SENSOR_SAMPLE_RATE_1600_HZ;
	} else if (strcmp(string, "3200") == 0) {
		sampleRate = MAX30102_SENSOR_SAMPLE_RATE_3200_HZ;
	}

	return sampleRate;
}

max30102SensorFullScale_e decodeFullScale(char *string)
{
	max30102SensorFullScale_e fullScale = MAX30102_SENSOR_FULL_SCALE_2048_NA;

	if (strcmp(string, "2048") == 0) {
		fullScale = MAX30102_SENSOR_FULL_SCALE_2048_NA;
	} else if (strcmp(string, "4096") == 0) {
		fullScale = MAX30102_SENSOR_FULL_SCALE_4096_NA;
	} else if (strcmp(string, "8192") == 0) {
		fullScale = MAX30102_SENSOR_FULL_SCALE_8192_NA;
	} else if (strcmp(string, "16384") == 0) {
		fullScale = MAX30102_SENSOR_FULL_SCALE_16384_NA;
	}

	return fullScale;
}
