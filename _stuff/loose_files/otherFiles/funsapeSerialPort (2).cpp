// *INDENT-OFF*

/* 
 * Project:
 * File name:		funsapeSerialPort.cpp
 * Module:			Serial Port Manager for Windows platforms
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// 
// Dependencies
// 

#include "funsapeSerialPort.h"
#ifndef __FUNSAPE_SERIAL_PORT_H
#	error	[funsapeSerialPort.cpp] Error 16 - Header file (funsapeSerialPort.h) is missing or corrupted!
#elif __FUNSAPE_SERIAL_PORT_H != __BUILD_TO_REPLACE__
#	error	[funsapeSerialPort.cpp] Error 17 - Build mismatch between source (funsapeSerialPort.cpp) and header (funsapeSerialPort.h) files!
#endif

// 
// File exclusive - Constants
// 

// NONE

// 
// File exclusive - New data types
// 

// NONE

// 
// File exclusive - Macro-functions
// 

// NONE

// 
// Global variables
// 

// NONE

// 
// Static functions declarations
// 

// NONE

// 
// Class constructors
// 

SerialPort::SerialPort(void)
{
	// Resets data members - Status
	this->_initialized          = false;
	this->_connected            = false;
	// Resets data members - Configuration
	this->_onConnectCallback    = nullptr;
	this->_onDisconnectCallback = nullptr;
	// Resets data members - Data Buffers
	this->_rxCircBuffer         = nullptr;

	// Returns successfully
	this->_lastError = Error::NONE;
	return;
}

SerialPort::SerialPort(CircularBuffer<uint8_t> * rxCircBuffer_p)
{
	// Resets data members - Status
	this->_initialized          = false;
	this->_connected            = false;
	// Resets data members - Configuration
	this->_onConnectCallback    = nullptr;
	this->_onDisconnectCallback = nullptr;
	// Resets data members - Data Buffers
	this->_rxCircBuffer         = nullptr;

	// Checks for error - circular buffer pointer
	if(!isPointerValid(rxCircBuffer_p)) {
		// Returns error
		this->_lastError = Error::BUFFER_POINTER_NULL;
		return;
	}
	// Checks for error - circular buffer initialization
	if(!rxCircBuffer_p->isInitialized()) {
		// Returns error
		this->_lastError = Error::BUFFER_NOT_INITIALIZED;
		return;
	}

	// Updates data members
	this->_rxCircBuffer = rxCircBuffer_p;
	this->_rxCircBuffer->flush();
	this->_initialized = true;

	// Returns successfully
	this->_lastError = Error::NONE;
	return;
}

SerialPort::~SerialPort(void)
{
	// Resets data members

	// Deletes circular buffer

	// Deallocate memory

	// Returns successfully
	this->_lastError = Error::NONE;
	return;
}

// 
// Class own methods - Public
// 

//     /////////////////////////     STATUS     /////////////////////////     //

Error SerialPort::getLastError(void)
{
	// Returns last error
	return this->_lastError;
}

bool_t SerialPort::isInitialized(void)
{
	// Returns status
	return this->_initialized;
}

//     //////////////////////    CONFIGURATION     //////////////////////     //

bool_t SerialPort::init(CircularBuffer<uint8_t> * rxCircBuffer_p)
{
	// Resets data members - Status
	this->_initialized           = false;
	this->_connected            = false;
	// Resets data members - Configuration
	this->_onConnectCallback    = nullptr;
	this->_onDisconnectCallback = nullptr;
	// Resets data members - Data Buffers
	this->_rxCircBuffer         = nullptr;

	// Checks for error - circular buffer pointer
	if(!isPointerValid(rxCircBuffer_p)) {
		// Returns error
		this->_lastError = Error::BUFFER_POINTER_NULL;
		return false;
	}
	// Checks for error - circular buffer initialization
	if(!rxCircBuffer_p->isInitialized()) {
		// Returns error
		this->_lastError = Error::BUFFER_NOT_INITIALIZED;
		return false;
	}

	// Updates data members
	this->_rxCircBuffer = rxCircBuffer_p;
	this->_rxCircBuffer->flush();
	this->_initialized = true;

	// Returns successfully
	this->_lastError = Error::NONE;
	return true;
}

bool_t SerialPort::setOnConnectCallback(void (*callbackFunction_p)(void))
{
	// Resets data members - Configuration
	this->_onConnectCallback = nullptr;

	// Checks for error - function pointer null
	if(!isPointerValid(callbackFunction_p)) {
		// Returns error
		this->_lastError = Error::FUNCTION_POINTER_NULL;
		return false;
	}

	// Updates data members
	this->_onConnectCallback = callbackFunction_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	return true;
}

bool_t SerialPort::setOnDisconnectCallback(void (*callbackFunction_p)(void))
{
	// Resets data members - Configuration
	this->_onDisconnectCallback = nullptr;

	// Checks for error - function pointer null
	if(!isPointerValid(callbackFunction_p)) {
		// Returns error
		this->_lastError = Error::FUNCTION_POINTER_NULL;
		return false;
	}

	// Updates data members
	this->_onDisconnectCallback = callbackFunction_p;

	// Returns successfully
	this->_lastError = Error::NONE;
	return true;
}

//     /////////////////////////    CONTROL     /////////////////////////     //

bool_t SerialPort::connect(AnsiString portName_p, uint32_t baudRate_p, DataBits dataBits_p, ParityMode parityMode_p, StopBits stopBits_p, PortDirection direction_p, FlowControl flowControl_p, PortSharing sharingFlags_p)
{
	// Checks for error - not initialized
	if(!this->_initialized) {
		// Returns error
		this->_lastError = Error::NOT_INITIALIZED;
		return false;
	}
	// Checks for error - not initialized
	if(!this->_initialized) {
		// Returns error
		this->_lastError = Error::NOT_INITIALIZED;
		return false;
	}





	// Returns successfully
	this->_lastError = Error::NONE;
	return true;
}

bool_t SerialPort::disconnect(void)
{
	// Returns successfully
	this->_lastError = Error::NONE;
	return true;
}

bool_t SerialPort::scan(std::vector<AnsiString> * portNames_p)
{
	// Returns successfully
	this->_lastError = Error::NONE;
	return true;
}

bool_t SerialPort::write(uint8_t buffer_p, uint32_t bufferSize_p, uint32_t *bytesSent_p)
{
	// Returns successfully
	this->_lastError = Error::NONE;
	return true;
}







//     ////////////////    FRAME BUFFER MANIPULATION     ////////////////     //

//     /////////////////    FRAME DATA MANIPULATION     /////////////////     //

// 
// Class own methods - Private
// 

//     ///////////////////     CONTROL AND STATUS     ///////////////////     //

// 
// Class own methods - Protected
// 

// NONE

// 
// END OF FILE
// 

// *INDENT-ON*






////////////////////////////////////////////////////////////////////////////////






//---------------------------------------------------------------------------
/*

#pragma hdrstop

#include "funsapeSerialPort.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


// Events
#define SIG_POWER_DOWN              0
#define SIG_READER                  1
#define SIG_READ_DONE               2       // data received has been read
#define SIG_WRITER                  3
#define SIG_DATA_TO_TX              4       // data waiting to be sent
#define SIG_MODEM_EVENTS            5
#define SIG_MODEM_CHECKED           6

#define EXIT_TIMEOUT                2000


void funsapeSerialPortThreadStart(void *arg);
wchar_t *AnsiToWchar_t(AnsiString auxAnsiString);

typedef unsigned(WINAPI *PBEGINTHREADEX_THREADFUNC)(LPVOID lpThreadParameter);
typedef unsigned *PBEGINTHREADEX_THREADID;

//     ///////////////////////    CONSTRUCTOR     ///////////////////////     //

funsapeSerialPort::funsapeSerialPort(CircularBuffer<uint8_t> *rxBuffer_p)
{
	// Update data members - status
    this->_initialized                  = false;
	this->_isConnected                  = false;
	this->_isReady                      = false;
	this->_isRxInProgress               = false;
	this->_isTxInProgress               = false;
	this->_isWaitCommEventInProgress    = false;
	this->_lastError                    = Error::NONE;
	// Update data members - port configuration
	this->_baudRate                     = 9600;
	this->_dataBits                     = DataBits::DATA_BITS_8;
	this->_direction                    = PortDirection::TX_RX;
	this->_flowControl                  = FlowControl::NO_CONTROL;
	this->_parityMode                   = ParityMode::NO_PARITY;
	this->_sharingFlags                 = PortSharing::NO_SHARE;
	this->_stopBits                     = StopBits::ONE;
	// Update data members - windows variables
	this->_dcb                          = {0};
	this->_ovReader                     = {0};
	this->_ovWaitEvent                  = {0};
	this->_ovWriter                     = {0};
	for(uint8_t i = 0;i < SERIAL_SIGNAL_NBR;i++) {
		this->_serialEvents[i]          = INVALID_HANDLE_VALUE;
	}
	this->_serialHandler                = INVALID_HANDLE_VALUE;
	this->_threadHandler                = INVALID_HANDLE_VALUE;
	this->_threadId                     = 0;
	// Update data members - data buffers
	this->_rxBuffer                     = nullptr;
	this->_maxRxTempBuffer              = 1;
	this->_rxTempBufferLength           = 0;
	this->_txTempBufferLength           = 0;
	// Update data members - callback functions
	this->_managerFunction              = nullptr;
	this->_onConnectCallback            = nullptr;
	this->_onDisconnectCallback         = nullptr;

	// Check for error - pointer null
	if(!isPointerValid(rxBuffer_p)) {
		// Returns error
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		return;
	}
	this->_rxBuffer                     = rxBuffer_p;

	// Create events for the different sources
	for(int i = 0; i < SERIAL_SIGNAL_NBR; i++) {
		if((i == SIG_READER) || (i == SIG_WRITER) || (i == SIG_MODEM_EVENTS)) {
			this->_serialEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);  // Manual Reset
		} else {
			this->_serialEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL); // Auto reset
		}
	}

	// Update data members
	this->_initialized = true;

	// Returns successfully
	this->_lastError = Error::NONE;
	return;
}

funsapeSerialPort::~funsapeSerialPort(void)
{
	// Delete thread handler
	if(this->_threadHandler != INVALID_HANDLE_VALUE) {
		WaitForSingleObject(this->_threadHandler, EXIT_TIMEOUT);
	}
	this->_threadHandler = INVALID_HANDLE_VALUE;

	// Delete the events
	for(int i = 0; i < SERIAL_SIGNAL_NBR; i++) {
		if(this->_serialEvents[i] != INVALID_HANDLE_VALUE) {
			CloseHandle(this->_serialEvents[i]);
		}
		this->_serialEvents[i] = INVALID_HANDLE_VALUE;
	}

	// Delete serial handler
	if(this->_serialHandler != INVALID_HANDLE_VALUE) {
		CloseHandle(this->_serialHandler);
	}
	this->_serialHandler = INVALID_HANDLE_VALUE;

	// Returns successfully
	return;
}

//     //////////////////////    CONFIGURATION     //////////////////////     //

bool funsapeSerialPort::connect(AnsiString portName_p, uint32_t baudRate_p, DataBits dataBits_p, ParityMode parityMode_p, StopBits stopBits_p, PortDirection direction_p, FlowControl flowControl_p, PortSharing sharingFlags_p)
{
	// Local variables
	DWORD           auxBaudRate         = 0;
	DWORD           auxDirection        = 0;
	DWORD           auxSharingFlags     = 0;
	BYTE            auxDataBits         = 0;
	BYTE            auxParityMode       = 0;
	BYTE            auxStopBits         = 0;
	DCB             auxDcb;
	COMMTIMEOUTS    auxTimeouts         = {0, 0, 0, 0, 0};
	bool            hard_handshake      = false;
	AnsiString auxString;

	// Checks initialization
	if(!this->_initialized) {
		// Returns error
		this->_lastError = Error::NOT_INITIALIZED;
		this->_isConnected = false;
		return false;
	}

	// Disconnect if already connected
	if(this->_serialHandler != INVALID_HANDLE_VALUE) {
		// Close handler
		this->_isConnected = false;
		CloseHandle(this->_serialHandler);
		this->_serialHandler = INVALID_HANDLE_VALUE;
		// Call onDisconnect Event Callback
		if(isPointerValid(this->_onDisconnectCallback)) {
			this->_onDisconnectCallback();
		}
	}

	// Check for error - baud rate
	if((baudRate_p < 600) || (baudRate_p > 2'000'000)) {
		// Register error
		this->_lastError = Error::ARGUMENT_VALUE_INVALID;
		// Returns error
		return false;
	}

	// Process arguments
	auxBaudRate = (DWORD)(baudRate_p);
	auxDataBits = (BYTE)(dataBits_p);
	auxParityMode = (BYTE)(parityMode_p);
	auxStopBits = (BYTE)(stopBits_p);
	if(direction_p == PortDirection::RX_ONLY) {
		auxDirection = (DWORD)(GENERIC_READ);
	} else if(direction_p == PortDirection::TX_ONLY) {
		auxDirection = (DWORD)(GENERIC_WRITE);
	} else {
		auxDirection = (DWORD)(GENERIC_READ | GENERIC_WRITE);
	}
	auxSharingFlags = (DWORD)sharingFlags_p;

	// Clear overlapped events
	ZeroMemory(&this->_ovReader, sizeof(this->_ovReader));
	ZeroMemory(&this->_ovWriter, sizeof(this->_ovWriter));
	ZeroMemory(&this->_ovWaitEvent, sizeof(this->_ovWaitEvent));

	// Event serial port
	this->_ovReader.hEvent = this->_serialEvents[SIG_READER];
	this->_ovWriter.hEvent = this->_serialEvents[SIG_WRITER];
	this->_ovWaitEvent.hEvent = this->_serialEvents[SIG_MODEM_EVENTS];

	// Open serial port
	this->_serialHandler = CreateFile(AnsiToWchar_t(portName_p), auxDirection, auxSharingFlags, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if(this->_serialHandler == INVALID_HANDLE_VALUE) {
		// Register error
		this->_lastError = Error::UNKNOWN;
		// Call onDisconnect Event Callback
		if(isPointerValid(this->_onDisconnectCallback)) {
			this->_onDisconnectCallback();
		}
		// Returns error
		return false;
	}

	// Set of events to be monitored
	if(!SetCommMask(this->_serialHandler, 0)) {
		// Close handler
		CloseHandle(this->_serialHandler);
		this->_serialHandler = INVALID_HANDLE_VALUE;
		// Register error
		this->_lastError = Error::UNKNOWN;
		// Call onDisconnect Event Callback
		if(isPointerValid(this->_onDisconnectCallback)) {
			this->_onDisconnectCallback();
		}
		// Returns error
		return false;
	}

	// Set timeouts
	if(!SetCommTimeouts(this->_serialHandler, &auxTimeouts)) {
		// Close handler
		CloseHandle(this->_serialHandler);
		this->_serialHandler = INVALID_HANDLE_VALUE;
		// Register error
		this->_lastError = Error::UNKNOWN;
		// Call onDisconnect Event Callback
		if(isPointerValid(this->_onDisconnectCallback)) {
			this->_onDisconnectCallback();
		}
		// Returns error
		return false;
	}

	// Clear DCB struct
	memset(&auxDcb, 0, sizeof(DCB));

	// Configure DCB struct
	auxDcb.DCBlength = sizeof(DCB);                 // Struct size
	auxDcb.BaudRate = auxBaudRate;                  // Baud rate
	auxDcb.fParity = (auxParityMode == 0) ? 0 : 1;  // Parity enable/disable
	auxDcb.Parity = auxParityMode;                  // Parity mode
	auxDcb.StopBits = auxStopBits;                  // Stop bits
	auxDcb.ByteSize = auxDataBits;                  // Data size
	auxDcb.fInX = 0;                                // XON/XOFF input flow control enable/disable
	auxDcb.fOutX = 0;                               // XON/XOFF output flow control enable/disable
	auxDcb.XonLim = 2;                              // Minimum number of bytes in use allowed in the input buffer before flow control is activated to allow transmission by the sender
	auxDcb.XoffLim = 4;                             // Minimum number of free bytes allowed in the input buffer before flow control is activated to inhibit the sender
	auxDcb.XonChar = 0x13;                          // Value of the XON character for both transmission and reception
	auxDcb.XoffChar = 0x19;                         // Value of the XOFF character for both transmission and reception
	auxDcb.fTXContinueOnXoff = FALSE;               // Does not continue until the input buffer is within XonLim bytes of being empty and the driver has transmitted the XonChar character to resume reception
	if(hard_handshake) {
		auxDcb.fOutxDsrFlow = TRUE;                 // DSR output flow control enable
		auxDcb.fOutxCtsFlow = TRUE;                 // CTS output flow control enable
		auxDcb.fRtsControl = RTS_CONTROL_HANDSHAKE; // RTS flow control mode
		auxDcb.fDtrControl = DTR_CONTROL_HANDSHAKE; // DTR flow control mode
	} else {
		auxDcb.fOutxDsrFlow = FALSE;                // DSR output flow control disable
		auxDcb.fOutxCtsFlow = FALSE;                // CTS output flow control disable
		auxDcb.fRtsControl = RTS_CONTROL_ENABLE;    // RTS flow control mode
		auxDcb.fDtrControl = DTR_CONTROL_ENABLE;    // DTR flow control mode
	}
	auxDcb.fNull = FALSE;                           // Disable null bytes (0x00) discard
	auxDcb.fErrorChar = FALSE;                      // Disable char substitution on parity error
	auxDcb.ErrorChar = 0x00;                        // Char to substitute on parity error
	auxDcb.fBinary = TRUE;                          // Enable binary mode
	auxDcb.fAbortOnError = FALSE;                   // Disable abort all operations on error
	auxDcb.wReserved = 0;                           // Reserved (must be 0)
	auxDcb.EvtChar = 0x00;                          // Value of the character used to signal an event
	auxDcb.fDsrSensitivity = FALSE;                 // Do not ignore received bytes when DSR signal is low
	auxDcb.EofChar = 0x00;                          // Value of the character used to signal the end of data

	// Set DCB struct
	if(!SetCommState(this->_serialHandler, &auxDcb)) {
		// Close handler
		CloseHandle(this->_serialHandler);
		this->_serialHandler = INVALID_HANDLE_VALUE;
		// Register error
		this->_lastError = Error::UNKNOWN;
		// Call onDisconnect Event Callback
		if(isPointerValid(this->_onDisconnectCallback)) {
			this->_onDisconnectCallback();
		}
		// Returns error
		return false;
	}

	// Purge data
	if(!PurgeComm(this->_serialHandler, PURGE_RXABORT | PURGE_TXCLEAR)){
		// Close handler
		CloseHandle(this->_serialHandler);
		this->_serialHandler = INVALID_HANDLE_VALUE;
		// Register error
		this->_lastError = Error::UNKNOWN;
		// Call onDisconnect Event Callback
		if(isPointerValid(this->_onDisconnectCallback)) {
			this->_onDisconnectCallback();
		}
		// Returns error
		return false;
	}

	// Check serial events
	for(int i = 0; i < SERIAL_SIGNAL_NBR; i++) {
		if(this->_serialEvents[i] == INVALID_HANDLE_VALUE) {
			// Close handler
			CloseHandle(this->_serialHandler);
			this->_serialHandler = INVALID_HANDLE_VALUE;
			// Register error
			this->_lastError = Error::UNKNOWN;
			// Call onDisconnect Event Callback
			if(isPointerValid(this->_onDisconnectCallback)) {
				this->_onDisconnectCallback();
			}
			// Returns error
			return false;
		}
	}

	// Start thread
	this->_threadHandler = (HANDLE)_beginthreadex(NULL, 0, (PBEGINTHREADEX_THREADFUNC)funsapeSerialPortThreadStart, this, 0, &this->_threadId);

	// Updates connection status
	this->_isConnected = true;

	// Call onConnect Event Callback
	if(isPointerValid(this->_onConnectCallback)) {
		this->_onConnectCallback();
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	return true;
}

// this function is the main loop of the Tserial object. There is a
// do while() loop executed until either an error or a PowerDown is
// received.
// this is not a busy wait since we use the WaitForMultipleObject function

void funsapeSerialPort::run(void)
{
	bool done = false;
	long          status;
	unsigned long read_nbr, result_nbr;
	char          success;

	// Checks connection
	if(!this->_isConnected){
		// Just returns
		return;
	}

	// Update data members
	this->_isReady                      = true;
	this->_isRxInProgress               = false;
	this->_isTxInProgress               = false;
	this->_isWaitCommEventInProgress    = false;

	if(this->_managerFunction != 0) {
		this->_managerFunction((uint32_t)this, SERIAL_CONNECTED);
	}

	// Clears pending errors
	GetLastError();
	SetEvent(this->_serialEvents[SIG_READ_DONE]);

	while(!done) {
		// ---------------------------------------------------------------------
		// Wait for signals ----------------------------------------------------
		// ---------------------------------------------------------------------
		// Main wait function. Waiting for something to happen. This may be
		// either the completion of a Read or a Write or the reception of modem
		// events, Power Down, new Tx.
		// ---------------------------------------------------------------------
		status = WaitForMultipleObjects(SERIAL_SIGNAL_NBR, this->_serialEvents, FALSE, INFINITE);
		// Process answer to filter other failures
		status = status - WAIT_OBJECT_0;
		if((status < 0) || (status >= SERIAL_SIGNAL_NBR)) {
			// error
			done = true;
		} else {
			// Event dispatcher
			switch(status) {

			case SIG_POWER_DOWN:            // receiving a POWER down signal. Stopping the thread
				done = true;
				break;

			case SIG_READ_DONE:             // Rx
				// Checking last reading process
				if(!this->_isRxInProgress) {
					this->_isRxInProgress = true;
					success = (char)ReadFile(this->_serialHandler, &this->_rxTempBuffer, this->_maxRxTempBuffer, &read_nbr,
									&this->_ovReader);
					if(!success) {
						// Get error
						if(GetLastError() != ERROR_IO_PENDING) {
							// real failure => quiting
							done = true;
							// Returns error
							this->_lastError = Error::UNKNOWN;
						} else {
							// Do nothing - read is pending
						}
					} else {
						// Do nothing - since the overlapped will be
						// signaled anyway, so the processing wiil be taken
						// there
					}
				}
				break;

			case SIG_READER:            // Reading the result of the terminated read
				if(GetOverlappedResult(this->_serialHandler, &this->_ovReader, &result_nbr, FALSE)) {
					// Read operation completed successfully
					ResetEvent(this->_serialEvents[SIG_READER]);
					// Write operation completed successfully
					this->_rxTempBufferLength = result_nbr;
					this->_isRxInProgress = false;
					// Process incoming data
					if((result_nbr != 0) && (this->_managerFunction != 0)) {
						this->_managerFunction((uint32_t)this, SERIAL_DATA_ARRIVAL);
					}
				} else {
					// GetOverlapped didn't succeed!
					if(GetLastError() != ERROR_IO_PENDING) {
						done = true;
						// Returns error
						this->_lastError = Error::UNKNOWN;
					}
				}
				break;

			case SIG_DATA_TO_TX:        // Tx
				// Signal asserted that there is a new valid message in the
				// "txBuffer" variable sending data to the port
				success = (char)WriteFile(this->_serialHandler, this->_txTempBuffer, this->_txTempBufferLength, &result_nbr,
								&this->_ovWriter);
				if(!success) {
					// Get error
					if(GetLastError() != ERROR_IO_PENDING) {
						// real failure => quiting
						done = true;
						// Returns error
						this->_lastError = Error::UNKNOWN;
					} else {
						// Do nothing - write is pending
					}
				} else {
					// Do nothing - since the overlapped will be
					// signaled anyway, so the processing wiil be taken
					// there
				}
				break;

			case SIG_WRITER:            // WriteFile has terminated
				if(GetOverlappedResult(this->_serialHandler, &this->_ovWriter, &result_nbr, FALSE)) {
					// Write operation completed successfully
					ResetEvent(this->_serialEvents[SIG_WRITER]);
					// Write operation completed successfully
					this->_isTxInProgress = false;
					// Process incoming data
					if(this->_managerFunction != 0) {
						this->_managerFunction((uint32_t)this, SERIAL_DATA_SENT);
					}
				} else {
					// GetOverlapped didn't succeed!
					if(GetLastError() != ERROR_IO_PENDING) {
						done = true;
						// Returns error
						this->_lastError = Error::UNKNOWN;
					}
				}
				break;

			case SIG_MODEM_CHECKED:     // Modem events
				break;

			case SIG_MODEM_EVENTS:      // Reading the result of the terminated wait
//					if (GetOverlappedResult(this->_seriaHandler, &this->_ovWaitEvent, &result_nbr, FALSE)) {
//						// Wait operation completed successfully
//						ResetEvent(this->_serialEvents[SIG_MODEM_EVENTS]);
//						this->_isWaitCommEventInProgress = false;
//						// Process incoming data
//						OnEvent(dwCommEvent);
//						// automatically starting a new check
//						SetEvent(this->_serialEvents[SIG_MODEM_CHECKED]);
//					} else {
//						// GetOverlapped didn't succeed !
//						// What's the reason ?
//						if(GetLastError() != ERROR_IO_PENDING) {
//							done = true;
//						}
//					}
				break;
			} // switch(status)
		} // if((status < 0) || (status >= SERIAL_SIGNAL_NBR)
	} // while(!done)

	// Close handler
	this->_isReady = false;
	if(this->_serialHandler != INVALID_HANDLE_VALUE) {
		// Close handler
		CloseHandle(this->_serialHandler);
		this->_serialHandler = INVALID_HANDLE_VALUE;
		// Call onDisconnect Event Callback
		if(!isPointerValid(this->_onDisconnectCallback)) {
			this->_onDisconnectCallback();
		}
		this->_isConnected = false;
	}

	if(this->_managerFunction != 0) {
		this->_managerFunction((uint32_t)this, SERIAL_DISCONNECTED);
	}

	return;
}




bool funsapeSerialPort::scan(std::vector<AnsiString> *ports_p)
{
	// Local variables
	AnsiString auxString = "";

	// Checks initialization
	if(!this->_initialized) {
		// Returns error
		this->_lastError = Error::NOT_INITIALIZED;
		this->_isConnected = false;
		return false;
	}

	// Checks for errors - argument pointer
	if(!isPointerValid(ports_p)) {
		// Returns error
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		return false;
	}

	// Checks connection
	if(this->_serialHandler != INVALID_HANDLE_VALUE) {
		CloseHandle(this->_serialHandler);
		this->_serialHandler = INVALID_HANDLE_VALUE;
	}

	// Clears port list
	ports_p->clear();

	// Scan for ports
	for (uint16_t i = 0; i < 256; i++) {
		auxString = "COM" + AnsiString(i);
		// Try to open port
		this->_serialHandler = CreateFile(AnsiToWchar_t(auxString), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		// Port available
		if (this->_serialHandler != INVALID_HANDLE_VALUE) {
			ports_p->push_back(auxString);
			// Close port
			CloseHandle(this->_serialHandler);
		}
	}
	this->_isConnected = false;

	// Returns sucessfully
	this->_lastError = Error::NONE;
	return true;
}

bool funsapeSerialPort::writeData(uint8_t *buffer_p, uint32_t bytesToSend_p, uint32_t *bytesSent_p)
{
	// Local variables
	BOOL auxSuccess = false;
	DWORD dwRes;

	// Checks initialization status
	if(!this->_initialized) {
		// Returns error
		this->_lastError = Error::NOT_INITIALIZED;
		this->_isConnected = false;
		return false;
	}
	// Checks connection status
	if(!this->_isConnected) {
		// Returns error
		this->_lastError = Error::UNKNOWN;
		return false;
	}
	// Check for errors
	if(!isPointerValid(buffer_p)) {
		// Returns error
		this->_lastError = Error::BUFFER_POINTER_NULL;
		return false;
	}
	// Check for errors
	if(!isPointerValid(bytesSent_p)) {
		// Returns error
		this->_lastError = Error::ARGUMENT_POINTER_NULL;
		return false;
	}

	// Try to send data
	auxSuccess = WriteFile(this->_serialHandler, buffer_p, (DWORD)bytesToSend_p, (DWORD *)bytesSent_p, NULL);
	if(!auxSuccess) {
		// Returns error
		this->_lastError = Error::UNKNOWN;
		return false;
	}

	// Returns successfully
	this->_lastError = Error::NONE;
	return true;
}

//     /////////////////////    OTHER FUNCTIONS     /////////////////////     //

void funsapeSerialPortThreadStart(void *arg)
{
	class funsapeSerialPort *serialUnit;

	serialUnit = (funsapeSerialPort *)arg;

	if(serialUnit != 0) {
		serialUnit->run();
	}

	return;
}

wchar_t *AnsiToWchar_t(AnsiString auxAnsiString)
{
	wchar_t *str = new wchar_t[auxAnsiString.WideCharBufSize()];
	return auxAnsiString.WideChar(str, auxAnsiString.WideCharBufSize());
	delete[] str;
}























//int funsapeSerialPort::getNbrOfBytes(void)
//{
//	struct _COMSTAT status;
//	int             n;
//	unsigned long   etat;
//
//	n = 0;
//
//	if(this->_serialHandler != INVALID_HANDLE_VALUE) {
//		ClearCommError(this->_serialHandler, &etat, &status);
//		n = status.cbInQue;
//	}
//
//	return(n);
//}



//bool funsapeSerialPort::setManagerFunction(void (*managerFunction_p)(uint32_t object_p, uint32_t event_p))
//{
//	// Check for error - function pointer null
//	if(!isPointerValid(managerFunction_p)) {
//		// Returns error
//		this->_lastError = ErrorCode::FUNCTION_POINTER_NULL;
//		return false;
//	}
//
//	// Update data members
//	this->_managerFunction = managerFunction_p;
//
//	// Returns successfully
//	this->_lastError = ErrorCode::NONE;
//	return true;
//}
//

//
//void funsapeSerialPort::dataHasBeenRead(void)
//{
//	SetEvent(this->_serialEvents[SIG_READ_DONE]);
//
//	return;
//}
//
//bool funsapeSerialPort::disconnect(void)
//{
//	this->_isReady = false;
//	SetEvent(this->_serialEvents[SIG_POWER_DOWN]);
//
//	if(this->_threadHandler != 0) {
//		WaitForSingleObject(this->_threadHandler, EXIT_TIMEOUT);
//	}
//	this->_threadHandler = 0;
//
//	return true;
//}
*/
