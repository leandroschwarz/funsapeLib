/* -----------------------------------------------------------------------------
 * File:			zigbee.h
 * Module:			Zigbee's basics package transmission and receiving.
 * Author:			Leonardo de Oliveira da Silva
 *                  Rodrigo de Mello Garcia
 * Version:			1.0
 * Modified at:		24/02/2015
 * ---------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#define ZIGBEE_MAXDATA  33

typedef char				int8;
typedef int					int16;
typedef long int			int32;
typedef long long			int64;
typedef unsigned char		uint8;
typedef unsigned short int 	uint16;
typedef unsigned long int	uint32;
typedef unsigned long long	uint64;

void zigbeeCreateTrasmitAPI(uint8* targetArray, uint8 targetArraySize, uint8 frameID, uint64 destAddr, uint16 destNetworkAddr, uint8 radius, uint8 options, uint8* data, uint8 dataSize);

int main()
{
    int i = 0;
    uint8 pacote[ZIGBEE_MAXDATA];
    uint8 pacoteCerto[ZIGBEE_MAXDATA] = {0x7E, 0x00, 0x1D, 0x10, 0x01, 0x00, 0x13, 0xA2, 0x00, 0x40, 0x89, 0xDF, 0x91, 0xFF, 0xFE, 0x00, 0x00, 0x30, 0x09, 0x95, 0x18, 0x04, 0x63, 0x24, 0x02, 0x64, 0x30, 0x03, 0x90, 0x23, 0x02, 0x13, 0x31};
    uint8 dados[15] = {0x30, 0x09, 0x95, 0x18, 0x04, 0x63, 0x24, 0x02, 0x64, 0x30, 0x03, 0x90, 0x23, 0x02, 0x13};

    uint8 teste1;
    uint16 teste2;
    uint32 teste3;
    uint64 teste4;

    zigbeeCreateTrasmitAPI(pacote, ZIGBEE_MAXDATA, 0x01, 0x0013A2004089DF91, 0xFFFE, 0x00, 0x00, dados, 15);

    printf("\n\nPacote criado:\n");
    for(i=0;i<ZIGBEE_MAXDATA;i++){
        printf("%02X", pacote[i]);
    }
    printf("\n\n");
    for(i=0;i<ZIGBEE_MAXDATA;i++){
        printf("Pacote esperado/adquirido[%d]:\t[%02X]\t[%02X]\n", i, pacoteCerto[i], pacote[i]);
    }
    printf("\n\n");
    for(i=0;i<15;i++){
        printf("dados[%d]: [%02X]\n", i, dados[i]);
    }

    return 0;
}
void zigbeeCreateTrasmitAPI(uint8* targetArray, uint8 targetArraySize, uint8 frameID, uint64 destAddr, uint16 destNetworkAddr, uint8 radius, uint8 options, uint8* data, uint8 dataSize)
{
    uint8 apiCheckPoint, apiAux, apiCounter = targetArraySize;
    uint8 checkSum = 0xFF;
    uint16 lenght = 0x00;

    //if((targetArraySize + sizeof(frameID) + sizeof(destAddr) + sizeof(destNetworkAddr) + sizeof(radius) + sizeof(options) + dataSize))
    targetArray[(targetArraySize-apiCounter)] = 0x7E;
    printf("\ntargetArray[%d] = 0x7E", (targetArraySize-apiCounter));
    apiCounter--;

    apiCheckPoint = (targetArraySize-apiCounter);
    for(apiAux=0;(targetArraySize-apiCounter)<apiCheckPoint + 2;apiAux++){
        printf("\ntargetArray[%d] = LENGHT", (targetArraySize-apiCounter));
        apiCounter--;
    }

    targetArray[(targetArraySize-apiCounter)] = 0x10;
    printf("\ntargetArray[%d] = API Command", (targetArraySize-apiCounter));
    lenght++;
    checkSum -= targetArray[(targetArraySize-apiCounter)];
    apiCounter--;


    targetArray[(targetArraySize-apiCounter)] = frameID;
    printf("\ntargetArray[%d] = FrameID", (targetArraySize-apiCounter));
    lenght++;
    checkSum -= targetArray[(targetArraySize-apiCounter)];
    apiCounter--;

    apiCheckPoint = (targetArraySize-apiCounter);
    for(apiAux=0;(targetArraySize-apiCounter)<apiCheckPoint + sizeof(destAddr);apiAux++){
        targetArray[(targetArraySize-apiCounter)] = (destAddr >> ((sizeof(destAddr))*((sizeof(destAddr))-apiAux-1)));
        printf("\ntargetArray[%d] = Dest Addr", (targetArraySize-apiCounter));
        lenght++;
        checkSum -= targetArray[(targetArraySize-apiCounter)];
        apiCounter--;
    }

    apiCheckPoint = (targetArraySize-apiCounter);
    for(apiAux=0;(targetArraySize-apiCounter)<apiCheckPoint + sizeof(destNetworkAddr);apiAux++){
        targetArray[(targetArraySize-apiCounter)] = (destNetworkAddr >> ((sizeof(destNetworkAddr))*((sizeof(destNetworkAddr))-apiAux-1)));
        printf("\ntargetArray[%d] = Dest Network Addr", (targetArraySize-apiCounter));
        lenght++;
        checkSum -= targetArray[(targetArraySize-apiCounter)];
        apiCounter--;
    }

    targetArray[(targetArraySize-apiCounter)] = radius;
    printf("\ntargetArray[%d] = Radius", (targetArraySize-apiCounter));
    lenght++;
    checkSum -= targetArray[(targetArraySize-apiCounter)];
    apiCounter--;

    targetArray[(targetArraySize-apiCounter)] = options;
    printf("\ntargetArray[%d] = Options", (targetArraySize-apiCounter));
    lenght++;
    checkSum -= targetArray[(targetArraySize-apiCounter)];
    apiCounter--;

    for(apiAux=0;apiAux<dataSize;apiAux++){
        targetArray[(targetArraySize-apiCounter)] = data[apiAux];
        printf("\ntargetArray[%d] = data[%d] = %02X", (targetArraySize-apiCounter), apiAux, data[apiAux]);
        lenght++;
        checkSum -= targetArray[(targetArraySize-apiCounter)];
        apiCounter--;
    }

    targetArray[(targetArraySize-apiCounter)] = checkSum;
    printf("\ntargetArray[%d] = checkSum", (targetArraySize-apiCounter));
    apiCounter--;

    targetArray[1] = lenght >> 8;
    targetArray[2] = lenght;

    return;
}
