#ifndef __CIRCULAR_BUFFER_H
#define __CIRCULAR_BUFFER_H

class CircularBufferGen {
public:
	CircularBufferGen(unsigned int size = 100);
	~CircularBufferGen();

	bool 			pushData(unsigned char data);	// Add some value to the bufferData_
	bool 			popData(unsigned char *data);					// Get and delete value of index nextReadPos_
	bool			isEmpty(void);				// Check if buffer is empty
	bool			isFull(void);				// Check if buffer is full
	unsigned int	getOccupation(void);		// Number of occupied elements at the buffer
	unsigned int	getFreeSpace(void);			// Number of free elements at the buffer
	bool			flush(void);				// Flushes all buffer data
	bool 			pushBuffer(unsigned char *buffer, unsigned int size);
	bool 			popBuffer(unsigned char *buffer, unsigned int size);


	void 			printBuffer();						// Debug all bufferData_ (even index not added)
	void 			printLengthOfBuffer();				// Debug length
	void 			printNextWrite();					// Debug nextWritePos_
	void 			printNextRead();					// Debug nextReadPos_

private:
	unsigned int	nextWritePos_;							// Pointer to next index to write
	unsigned int	nextReadPos_;							// Pointer to next index to read
	unsigned int	bufferLength_;					// Length of bufferData_
	unsigned char 	*bufferData_;					// Buffer
	unsigned int 	bufferMaxSize_;
	unsigned int 	bufferOccupation_;
	bool			initialized_		: 1;
	bool			empty_				: 1;
	bool			full_				: 1;
};

#endif // __CIRCULAR_BUFFER_H
