//------------------------------------------------------------------------------
// File:		twi_slave.c
// Purpose:		Two-wire slave implementation for ATmega328P
// Version:		1.0
// Author:		Hazael dos Santos Batista
//------------------------------------------------------------------------------

#include <avr/interrupt.h>
#include <stdlib.h>
#include "twiSlave.h"

static volatile unsigned char twiIntAddrByte = 0;
static volatile unsigned char twiBusy = 0;

volatile static struct {
    unsigned char data[TWI_BUFFER_SIZE];
    unsigned char index;
} twiBuffer;

void twiSlaveInit(unsigned char twiSlaveAddr, unsigned char twiGenCallEnabled)
{
    TWAR = (twiSlaveAddr << 1) | (twiGenCallEnabled & 1);
    TWCR = (1 << TWEN) |
           (1 << TWIE) | (1 << TWINT) |
           (1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |
           (0 << TWWC);
    twiBusy = 0;
}

twiBuffer_t* twiGetBuffer(void)
{
    return (twiBuffer.data);
}

unsigned char twiTransceiverBusy(void)
{
    return twiBusy;
}

void twiStartTransceiver(void)
{
    while (twiTransceiverBusy()) {}

    TWCR = (1 << TWEN) |
           (1 << TWIE) | (1 << TWINT) |
           (1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |
           (0 << TWWC);
    twiBusy = 0;
}

ISR(TWI_vect)
{
    switch (TWSR) {
    case TWI_STX_ADR_ACK:
    case TWI_STX_DATA_ACK:
        TWDR = twiBuffer.data[twiBuffer.index++];

        if(twiBuffer.index >= TWI_BUFFER_SIZE) {
            twiBuffer.index = 0;
        }

        TWCR = (1 << TWEN) |
               (1 << TWIE) | (1 << TWINT) |
               (1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |
               (0 << TWWC);
        twiBusy = 1;
        break;

    case TWI_STX_DATA_NACK:
        TWCR = (1 << TWEN) |
               (1 << TWIE) | (1 << TWINT) |
               (1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |
               (0 << TWWC);
        twiBusy = 0;
        break;

    case TWI_SRX_GEN_ACK:
    case TWI_SRX_ADR_ACK:
        TWCR = (1 << TWEN) |
               (1 << TWIE) | (1 << TWINT) |
               (1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |
               (0 << TWWC);
        twiIntAddrByte = TRUE;
        twiBusy = 1;
        break;

    case TWI_SRX_ADR_DATA_ACK:
    case TWI_SRX_GEN_DATA_ACK:
        if(twiIntAddrByte == TRUE) {
            twiIntAddrByte = FALSE;
            twiBuffer.index = TWDR;
        }
        else {
            twiBuffer.data[twiBuffer.index] = TWDR;

            twiBuffer.index++;

            if(twiBuffer.index >= TWI_BUFFER_SIZE) {
                twiBuffer.index = 0;
            }
        }

        TWCR = (1 << TWEN) |
               (1 << TWIE) | (1 << TWINT) |
               (1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |
               (0 << TWWC);
        twiBusy = 1;
        break;

    case TWI_SRX_STOP_RESTART:
        TWCR = (1 << TWEN) |
               (1 << TWIE) | (1 << TWINT) |
               (1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |
               (0 << TWWC);
        twiBusy = 0;
        break;

    case TWI_SRX_ADR_DATA_NACK:
    case TWI_SRX_GEN_DATA_NACK:
    case TWI_STX_DATA_ACK_LAST_BYTE:
    case TWI_BUS_ERROR:
        TWCR = (1 << TWSTO) | (1 << TWINT);
        break;

    default:
        TWCR = (1 << TWEN) |
               (1 << TWIE) | (1 << TWINT) |
               (1 << TWEA) | (0 << TWSTA) | (0 << TWSTO) |
               (0 << TWWC);
        twiBusy = 0;
    }
}
