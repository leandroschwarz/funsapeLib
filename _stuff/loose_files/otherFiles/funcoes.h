
#ifndef __FUNCOES_H
#define __FUNCOES_H

#define setBit(reg, bit)              ((reg) |= (1 << (bit)))         //!< \hideinitializer
#define clrBit(reg, bit)              ((reg) &= ~(1 << (bit)))        //!< \hideinitializer
#define cplBit(reg, bit)              ((reg) ^= (1 << (bit)))         //!< \hideinitializer
#define isBitSet(reg, bit)              (((reg) >> (bit)) & 1)          //!< \hideinitializer
#define isBitClr(reg, bit)              (!(((reg) >> (bit)) & 1))       //!< \hideinitializer


typedef unsigned char uint8_t;
typedef unsigned int uint16_t;


uint8_t hexToSevenSegments(uint8_t hexValue);
void    initLedBar(uint8_t di, uint8_t srclk, uint8_t srclr, uint8_t rclk, uint8_t oe);
void    showLedBar(uint16_t value);
void    initDisplaySerial(uint8_t di, uint8_t srclk, uint8_t srclr, uint8_t rclk, uint8_t oe, uint8_t displays);
void    showDiplaySerial(uint16_t value);

#endif
