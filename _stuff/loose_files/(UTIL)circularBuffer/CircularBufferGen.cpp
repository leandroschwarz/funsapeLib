#include "CircularBufferGen.h"

#include <stdio.h>
#include <stdlib.h>



#define SIZE_OF_BUFFER 50


// Contructor
CircularBufferGen::CircularBufferGen(unsigned int size)
{
	this->nextWritePos_ = 0;							// Pointer to next index to write
	this->nextReadPos_ = 0;							// Pointer to next index to read
	this->bufferLength_ = 0;					// Length of bufferData_
	this->bufferData_ = NULL;					// Buffer
	this->bufferMaxSize_ = 0;
	this->bufferOccupation_ = 0;
	this->initialized_ = false;

	if (size == 0) {
		return;
	}
	this->bufferData_ = (unsigned char *)calloc(size, sizeof(unsigned char));
	if (this->bufferData_ == NULL) {
		return;
	}
	this->bufferMaxSize_ = size;
	this->initialized_ = true;
}


// Destructor
CircularBufferGen::~CircularBufferGen()
{
	this->nextWritePos_ = 0;
	this->nextReadPos_ = 0;
	this->bufferLength_ = 0;
}


// Adding a new value
bool CircularBufferGen::pushData(unsigned char data)
{
	// Check for errors - Not initialized
	if (!this->initialized_) {
		return false;
	}

	// Stores data
	this->bufferData[this->nextWritePos_] = data;							// Push data into buffer
	this->nextWritePos_ = ((this->nextWritePos_ + 1) %
						   this->bufferMaxSize_);	// Calculates next write index
	this->bufferOccupation_++;										// Updates occupation
	this->full_ = (this->bufferOccupation_ == this->bufferMaxSize_);		// Updates full status
	this->empty_ = false;										// Updates empty status

	return true;
}


bool CircularBufferGen::popData(unsigned char *data)
{
	// Check for errors - Not initialized
	if (!this->initialized_) {
		return false;
	}
	// Check for errors - Buffer empty
	if (this->empty_) {
		return false;
	}
	// Check for errors - Buffer locked
	if (this->data == NULL) {
		return false;
	}

	// Retrieves data
	*data = this->bufferData_[this->nextReadPos_];						// Pop data from buffer
	this->nextReadPos_ = ((this->nextReadPos_ + 1) %
						  this->bufferMaxSize_);	// Calculates next read index
	this->bufferOccupation_--;										// Updates occupation
	this->full_ = false;										// Updates full status
	this->empty_ = (this->bufferOccupation_ == 0);					// Updates empty status

	return true;
}

unsigned int CircularBufferGen::getOccupation(void)
{
	return this->bufferOccupation_;
}

unsigned int CircularBufferGen::getFreeSpace(void)
{
	return (this->bufferMaxSize_ - this->bufferOccupation_);
}

bool CircularBufferGen::isEmpty(void)
{
	return this->empty_;
}

bool CircularBufferGen::isFull(void)
{
	return this->full_;
}

bool CircularBufferGen::flush(void)
{
	// Flushes data
	this->nextReadPos_ = 0;;
	this->nextWritePos_ = 0;
	this->bufferOccupation_ = 0;
	this->empty_ = true;
	this->full_ = false;

	return true;
}

/*
void CircularBufferGen::printBuffer()
{
	// just printing the bufferData_
	printf("\nBuffer Data\n");
	for (int i = 0 ; i < SIZE_OF_BUFFER ; i++ ) {
		printf("%02X\t", this->bufferData_[i]);
	}
	printf("\n");
}


void CircularBufferGen::printLengthOfBuffer()
{
	// just printing the bufferData_ length
	printf("\nBuffer Length = ");
	printf("%d", this->bufferLength_);
}


void CircularBufferGen::printNextWrite()
{
	// just printing the pointer nextWritePos_
	printf("\nNext Write = ");
	printf("%d", this->nextWritePos_);
}


void CircularBufferGen::printNextRead()
{
	// just printing the pointer nextReadPos_
	printf("\nNext Read = ");
	printf("%d", this->nextReadPos_);
}


bool CircularBufferGen::isBufferEmpty()
{
	if (this->bufferLength_ == 0) {
		return true;
	} else {
		return false;
	}
}

bool CircularBufferGen::isBufferFull()
{
	if (this->bufferLength_ == SIZE_OF_BUFFER) {
		return true;
	} else {
		return false;
	}
}
*/

CircularBufferGen serialBuffer(300);

unsigned char aux8 = 30;

if (!serialBuffer.isFull())
{
	if (!serialBuffer.push(aux8)) {
		// Error
	}
}

if (!serialBuffer.isEmpty())
{
	if (!serialBuffer.pop(&aux8)) {
		// Error
	}
}

unsigned char auxBuffer[50];

if (serialBuffer.getFreeSpace() >= 50)
{
	for (unsigned char i = 0; i < 50; i++) {
		if (!serialBuffer.push(auxBuffer[i])) {
			// Error
		}
	}
}


unsigned char auxBuffer[50];
if (!popBuffer(auxBuffer, 50))
{

}


