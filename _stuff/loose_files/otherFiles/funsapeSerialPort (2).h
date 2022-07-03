// *INDENT-OFF*

/* 
 * Project:
 * File name:		funsapeSerialPort.h
 * Module:			Serial Port Manager for Windows platforms
 * Authors:			__AUTHORS_TO_REPLACE__
 * Build:			__BUILD_TO_REPLACE__
 * Last edition:	__DATE_TO_REPLACE__
 * ========================================================================== */

// 
// Include guard - Begin
// 

#ifndef __FUNSAPE_SERIAL_PORT_H
#define __FUNSAPE_SERIAL_PORT_H					__BUILD_TO_REPLACE__

// 
// Dependencies
// 

// -----------------------------------------------------------------------------
// Global definitions file -----------------------------------------------------

#include "globalDefines.h"
#ifndef __GLOBAL_DEFINES_H
#	error	[funsapeSerialPort.h] Error 12 - Global definitions file (globalDefines.h) is missing or corrupted!
#elif __GLOBAL_DEFINES_H != __BUILD_TO_REPLACE__
#	error	[funsapeSerialPort.h] Error 13 - Build mismatch between global definitions file (globalDefines.h) and current header file (funsapeSerialPort.h)!
#endif

// -----------------------------------------------------------------------------
// Header files - Library header files -----------------------------------------

#include "circularBuffer.h"
#ifndef __CIRCULAR_BUFFER_H
#	error	[funsapeSerialPort.h] Error 14 - Required module (circularBuffer.h) is missing or corrupted!
#elif __CIRCULAR_BUFFER_H != __BUILD_TO_REPLACE__
#	error	[funsapeSerialPort.h] Error 15 - Build mismatch between required module (circularBuffer.h) and current header file (funsapeSerialPort.h)!
#endif


// -----------------------------------------------------------------------------
// Header files - External header files ----------------------------------------

#include <vcl.h>
#include <vector>

// 
// Platform verification
// 

// NONE

// 
// Undefining previous definitions
// 

// NONE

// 
// Constant definitions
// 

// NONE

// 
// Macro-function definitions
// INDENT-OFF*
// 

// NONE

// 
// New data types
// 

// NONE

// 
// Extern global variables
// 

// NONE

// 
// SerialPort - Class declaration
// 

/**
 * @brief
 *
 * Detailed description of this element.
 *
 */
class SerialPort
{
	// -------------------------------------------------------------------------
	// New data types ----------------------------------------------------------

public:
	enum class DataBits : uint8_t {
		DATA_BITS_5 = 5,
		DATA_BITS_6 = 6,
		DATA_BITS_7 = 7,
		DATA_BITS_8 = 8,
	};
	enum class FlowControl : uint8_t {
		NO_CONTROL	= 0,
		CTS_RTS		= 1,
		DSR_DTR		= 2,
		XON_XOFF	= 4,
	};
	enum class ParityMode : uint8_t {
		NO_PARITY	= 0,
		ODD			= 1,
		EVEN		= 2,
		MARK		= 3,
		SPACE		= 4,
	};
	enum class PortDirection : uint8_t {
		RX_ONLY		= 0,
		TX_ONLY		= 1,
		TX_RX		= 2
	};
	enum class PortSharing : uint8_t {
		NO_SHARE		= 0,
		ALLOW_DELETE	= 4,
		ALLOW_READ		= 1,
		ALLOW_WRITE		= 2,
	};
	enum class StopBits : uint8_t {
		ONE			= 0,
		ONE_HALF	= 1,
		TWO			= 2,
	};

private:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Operators overloading ---------------------------------------------------

public:
	// NONE

private:
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Constructors ------------------------------------------------------------

public:
	SerialPort(
		void
	);
	SerialPort(
		CircularBuffer<uint8_t> * rxCircBuffer_p
	);
	~SerialPort(
		void
	);

	// -------------------------------------------------------------------------
	// Methods - Class own methods ---------------------------------------------

public:
	//     ///////////////////////     STATUS     ///////////////////////     //
	Error getLastError(
		void
	);
	bool_t isInitialized(
		void
	);

	//     ////////////////////    CONFIGURATION     ////////////////////     //
	bool_t init(
		CircularBuffer<uint8_t> * rxCircBuffer_p
	);
	bool_t setOnConnectCallback(
		void (*callbackFunction_p)(void)
	);
	bool_t setOnDisconnectCallback(
		void (*callbackFunction_p)(void)
	);

	//     ///////////////////////    CONTROL     ///////////////////////     //
	bool_t connect(
		AnsiString      portName_p,
		uint32_t        baudRate_p      = 9600,
		DataBits        dataBits_p      = DataBits::DATA_BITS_8,
		ParityMode      parityMode_p    = ParityMode:: NO_PARITY,
		StopBits        stopBits_p      = StopBits::ONE,
		PortDirection   direction_p     = PortDirection::TX_RX,
		FlowControl     flowControl_p   = FlowControl::NO_CONTROL,
		PortSharing     sharingFlags_p  = PortSharing::NO_SHARE
	);
	bool_t disconnect(
		void
	);
	bool_t scan(
		std::vector<AnsiString> * portNames_p
	);
	bool_t write(
		uint8_t     buffer_p,
		uint32_t    bufferSize_p,
		uint32_t    *bytesSent_p
	);




private:
	//     ///////////////////////     STATUS     ///////////////////////     //
	// NONE

protected:
	// NONE

	// -------------------------------------------------------------------------
	// Properties --------------------------------------------------------------

public:
	// NONE

private:
	//     ///////////////////////     STATUS     ///////////////////////     //
	bool_t _initialized                 : 1;    // Setted if circular buffer pointer is valid
	bool_t _connected                   : 1;    // Setted if port connection is open
	Error _lastError;

	//     ////////////////////    CONFIGURATION     ////////////////////     //
	void (*_onConnectCallback)(void);
	void (*_onDisconnectCallback)(void);


	//     /////////////////////    DATA BUFFER     /////////////////////     //
	CircularBuffer<uint8_t> *_rxCircBuffer;


	//     //////////////////    FRAME BUFFER DATA     //////////////////     //

	//     /////////////////     FRAME DECODED DATA     /////////////////     //


protected:
	// NONE

};  // class SerialPort

// 
// SerialPort - Class overloading operators
// 

// NONE

// 
// Global variables
// 

// -----------------------------------------------------------------------------
// Externally defined global variables -----------------------------------------

// NONE

// -----------------------------------------------------------------------------
// Internally defined global variables -----------------------------------------

// NONE

// 
// SerialPort - Class inline function definitions
// 

// NONE

// 
// General public functions declarations
// 

// NONE
void myErrorHandler(char * message);

// 
// General inline functions definitions
// 

// NONE

// 
// External default objects
// 

// NONE

// 
// Include guard - End
// 

#endif // __FUNSAPE_SERIAL_PORT_H

// 
// END OF FILE
// 

// *INDENT-ON*













////////////////////////////////////////////////////////////////////////////////


/*


//---------------------------------------------------------------------------

#ifndef funsapeSerialPortH
#define funsapeSerialPortH
//---------------------------------------------------------------------------

#include <vcl.h>
#include <windows.h>
#include <fileapi.h>
#include <synchapi.h>
#include <process.h>
#include <vector>
#include "circularBuffer.h"

#define SERIAL_SIGNAL_NBR           7       // number of events in the thread
#define SERIAL_MAX_RX               256     // Input buffer max size
#define SERIAL_MAX_TX               256     // output buffer max size

#define SERIAL_CONNECTED            0
#define SERIAL_DISCONNECTED         1
#define SERIAL_DATA_SENT            2
#define SERIAL_DATA_ARRIVAL         3
#define SERIAL_RING                 4
#define SERIAL_CD_ON                5
#define SERIAL_CD_OFF               6

class funsapeSerialPort
{
	// New data types
public:


	// Methods
public:
	//     /////////////////////    CONSTRUCTOR     /////////////////////     //
	funsapeSerialPort(
		CircularBuffer<uint8_t> * rxBuffer_p
	);
	~funsapeSerialPort(
		void
	);

	//     ///////////////////////     STATUS     ///////////////////////     //
	Error getLastError(
		void
	);
	bool isInitialized(
		void
	);
//	int getNbrOfBytes(
//		void
//	);

	//     ////////////////////    CONFIGURATION     ////////////////////     //
	bool connect(
		AnsiString      portName_p,
		uint32_t        baudRate_p      = 9600,
		DataBits        dataBits_p      = DataBits::DATA_BITS_8,
		ParityMode      parityMode_p    = ParityMode:: NO_PARITY,
		StopBits        stopBits_p      = StopBits::ONE,
		PortDirection   direction_p     = PortDirection::TX_RX,
		FlowControl     flowControl_p   = FlowControl::NO_CONTROL,
		PortSharing     sharingFlags_p  = PortSharing::NO_SHARE
	);
	void run(
		void
	);
	bool scan(
		std::vector<AnsiString> *ports_p
	);

	bool writeData(
		uint8_t *buffer_p,
		uint32_t bytesToSend_p,
		uint32_t *bytesSent_p
	);



//	bool setManagerFunction(
//		void (*managerFunction_p)(uint32_t object_p, uint32_t event_p)
//	);
//	bool setOnRescanPortCallback(
//		void (*callbackFunction_p)(void)
//	);

//	char * getDataInBuffer(void) {
//		return(this->_rxTempBuffer);
//	}
//	int getDataInSize(void) {
//		return(this->_rxTempBufferLength);
//	}
//	void dataHasBeenRead(
//		void
//	);
//	bool disconnect(
//		void
//	);

private:

	// Properties
private:
	//     ///////////////////////     STATUS     ///////////////////////     //
	bool                    _isReady                        : 1;
	bool                    _isRxInProgress                 : 1;
	bool                    _isTxInProgress                 : 1;
	bool                    _isWaitCommEventInProgress      : 1;

	//     /////////////////     PORT CONFIGURATION     /////////////////     //
	uint32_t                    _baudRate;
	DataBits                    _dataBits;
	PortDirection               _direction;
	FlowControl                 _flowControl;
	ParityMode                  _parityMode;
	PortSharing                 _sharingFlags;
	StopBits                    _stopBits;

	//     //////////////////    WINDOWS VARIABLES     //////////////////     //
	DCB                         _dcb;
	OVERLAPPED                  _ovReader;      // Overlapped structure for ReadFile
	OVERLAPPED                  _ovWaitEvent;   // Overlapped structure for WaitCommEvent
	OVERLAPPED                  _ovWriter;      // Overlapped structure for WriteFile
	HANDLE                      _serialEvents[SERIAL_SIGNAL_NBR];
	HANDLE                      _serialHandler;
	HANDLE                      _threadHandler;
	unsigned int                _threadId;

	//     ////////////////////     DATA BUFFERS     ////////////////////     //
	CircularBuffer<uint8_t>     *_rxBuffer;
	char                        _rxTempBuffer[SERIAL_MAX_RX];
	uint16_t                    _maxRxTempBuffer;
	uint16_t                    _rxTempBufferLength;
	char                        _txTempBuffer[SERIAL_MAX_TX];
	uint16_t                    _txTempBufferLength;

	//     /////////////////     CALLBACK FUNCTIONS     /////////////////     //
	void (*_managerFunction)(uint32_t object_p, uint32_t event_p);

};

#endif



*/
