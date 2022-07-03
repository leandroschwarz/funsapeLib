

#include "package.h"


void packageSendOk(package_t * pkg)
{
	uint8_t i = 0;
	uint8_t aux8 = 0;
	uint16_t aux16 = 0;
	char line[200];

	// Initial package flags setup
	pkg->inUse = TRUE;		// Marks package as "in use"

	strcpy(line, "[OK] ");
	usartTransmit(pkg->data[0]);			// Send start delimiter
	for(i = 1; i < pkg->index; i++) {
		aux8 = pkg->data[i];
		if((aux8 == 0x7E) || (aux8 == 0x7D)) {
			sprintf(line + strlen(line), " 7D %02X", (aux8 ^ 0x20));
			usartTransmit(0x7D);
			usartTransmit(aux8 ^ 0x20);
		} else {
			sprintf(line + strlen(line), " %02X", aux8);
			usartTransmit(aux8);
		}
	}

	sprintf(line + strlen(line), "\r\n");
	setBit(PORTC, PC1);
	f_write(&file, line, strlen(line), &aux16);
	clrBit(PORTC, PC1);
	f_sync(&file);

	// Final package flags setup
	pkg->inUse = FALSE;		// Marks package as "not in use"
	pkg->ready = FALSE;		// Marks package as "not ready"

	return;
}

void packageSendErr(package_t * pkg)
{
	uint8_t i = 0;
	uint8_t errIndex = 0;
	uint8_t errOffSet = 0;
	uint8_t aux8 = 0;
	uint16_t aux16 = 0;
	uint8_t auxStr[41];
	uint8_t strIndex = 0;
	char line[200];

	// Initial package flags setup
	pkg->inUse = TRUE;		// Marks package as "in use"

	// Transfer data to auxStr
	auxStr[0] = pkg->data[0];
	for(i = 1, strIndex = 1; i < pkg->index; i++) {
		aux8 = pkg->data[i];
		if((aux8 == 0x7E) || (aux8 == 0x7D)) {
			auxStr[strIndex++] = 0x7D;
			auxStr[strIndex++] = (aux8 ^ 0x20);
		} else {
			auxStr[strIndex++] = aux8;
		}
	}

	// Random error
	errIndex = (uint8_t)(rand() % (uint16_t)(strIndex));
	errOffSet = (uint8_t)(rand() % 254) + 1;
	auxStr[errIndex] += errOffSet;
	strcpy(line, "[ERR]");
	for(i = 0; i < strIndex; i++) {
		if(i != errIndex) {
			sprintf(line + strlen(line), " %02X", auxStr[i]);
		} else {
			sprintf(line + strlen(line), " %02X(%02X)", auxStr[i], (uint8_t)(auxStr[i] - errOffSet));
		}
	}
	sprintf(line + strlen(line), "\r\n");
	f_write(&file, line, strlen(line), &aux16);
	f_sync(&file);

	// Send package
	for(i = 0; i < strIndex; i++) {
		usartTransmit(auxStr[i]);
	}

	// Final package flags setup
	pkg->inUse = FALSE;		// Marks package as "not in use"
	pkg->ready = FALSE;		// Marks package as "not ready"

	return;
}

void packageCreate(package_t * pkg, uint8_t id, uint8_t destAddr, uint8_t sourAddr, uint8_t * payload, uint8_t paylSize)
{
	uint8_t i;
	uint8_t aux8;

	// Initial package flags setup
	pkg->inUse = TRUE;		// Marks package as "in use"
	pkg->ready = FALSE;		// Masks package as "not ready to transfer"
	pkg->index = 0;			// Clears package index

	// Mounts package header
	pkg->data[PACKAGE_START] = 0x7E;				// Start delimiter
	pkg->data[PACKAGE_ID] = id;						// Package ID
	pkg->data[PACKAGE_DEST_ADDR] = destAddr;		// Destinatioon address
	pkg->data[PACKAGE_SOUR_ADDR] = sourAddr;		// Source address
	pkg->data[PACKAGE_PAYL_SIZE] = paylSize;		// Payload size

	// Mounts package payload
	for(i = 0; i < paylSize; i++) {
		pkg->data[PACKAGE_PAYLOAD + i] = payload[i];
	}

	// Mounts package footer
	aux8 = 0xFF;
	for(i = 1; i < (PACKAGE_PAYLOAD + paylSize); i++) {
		aux8 -= pkg->data[i];
	}
	pkg->data[i] = aux8;							// Checksum

	// Final package flags setup
	pkg->inUse = FALSE;		// Marks package as "not in use"
	pkg->ready = TRUE;		// Masks package as "ready to transfer"
	pkg->index = i + 1;		// Clears package index

	return;
}
