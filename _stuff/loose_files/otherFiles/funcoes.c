

#include "funcoes.h"

uint8_t ledBarDi;
uint8_t ledBarRclk;
uint8_t ledBarSrclk;
uint8_t ledBarSrclr;
uint8_t ledBarOe;

uint8_t displaySerialDi;
uint8_t displaySerialRclk;
uint8_t displaySerialSrclk;
uint8_t displaySerialSrclr;
uint8_t displaySerialOe;
uint8_t displaySerialDisplays;






uint8_t hexToSevenSegments(uint8_t hexValue)
{
  uint8_t data = 0;

  switch(hexValue) { // PGFEDCBA
  case 0x00:
    data = 0b00111111;
    break;
  case 0x01:
    data = 0b00000110;
    break;
  case 0x02:
    data = 0b01011011;
    break;
  case 0x03:
    data = 0b01001111;
    break;
  case 0x04:
    data = 0b01100110;
    break;
  case 0x05:
    data = 0b01101101;
    break;
  case 0x06:
    data = 0b01111101;
    break;
  case 0x07:
    data = 0b00000111;
    break;
  case 0x08:
    data = 0b01111111;
    break;
  case 0x09:
    data = 0b01101111;
    break;
  case 0x0A:
    data = 0b01110111;
    break;
  case 0x0B:
    data = 0b01111100;
    break;
  case 0x0C:
    data = 0b00111001;
    break;
  case 0x0D:
    data = 0b01011110;
    break;
  case 0x0E:
    data = 0b01111001;
    break;
  case 0x0F:
    data = 0b01110001;
    break;
  }

  return (~data);
}

void initLedBar(uint8_t di, uint8_t srclk, uint8_t srclr, uint8_t rclk, uint8_t oe)
{
  ledBarDi = di;
  ledBarRclk = rclk;
  ledBarSrclk = srclk;
  ledBarSrclr = srclr;
  ledBarOe = oe;

  // portMode
  pinMode(ledBarDi, OUTPUT);
  pinMode(ledBarRclk, OUTPUT);
  pinMode(ledBarSrclk, OUTPUT);
  pinMode(ledBarSrclr, OUTPUT);
  pinMode(ledBarOe, OUTPUT);

  // DI default value
  digitalWrite(ledBarDi, LOW);
  // SRCLK clear pulse and default value
  digitalWrite(ledBarSrclk, HIGH);
  // SRCLR default value
  digitalWrite(ledBarSrclr, LOW);
  digitalWrite(ledBarSrclr, HIGH);
  // RCLK store pulse and default value
  digitalWrite(ledBarRclk, HIGH);
  digitalWrite(ledBarRclk, LOW);
  digitalWrite(ledBarRclk, HIGH);
  // OE default value
  digitalWrite(ledBarOe, LOW);
}

void showLedBar(uint16_t value)
{
  // Clear
  digitalWrite(ledBarSrclr, LOW);
  digitalWrite(ledBarSrclr, HIGH);

  // Desloca
  for(int i = 0;i < 16;i++){
    if(isBitSet(value, (15 - i)) == 1) {
      digitalWrite(ledBarDi, HIGH);
    } else {
      digitalWrite(ledBarDi, LOW);
    }
    digitalWrite(ledBarSrclk, LOW);
    digitalWrite(ledBarSrclk, HIGH);
  }
  // Grava
  digitalWrite(ledBarRclk, LOW);
  digitalWrite(ledBarRclk, HIGH);
}

void initDisplaySerial(uint8_t di, uint8_t srclk, uint8_t srclr, uint8_t rclk, uint8_t oe, uint8_t displays)
{
  displaySerialDi = di;
  displaySerialRclk = rclk;
  displaySerialSrclk = srclk;
  displaySerialSrclr = srclr;
  displaySerialOe = oe;
  displaySerialDisplays = displays;

  // portMode
  pinMode(displaySerialDi, OUTPUT);
  pinMode(displaySerialRclk, OUTPUT);
  pinMode(displaySerialSrclk, OUTPUT);
  pinMode(displaySerialSrclr, OUTPUT);
  pinMode(displaySerialOe, OUTPUT);

  // DI default value
  digitalWrite(displaySerialDi, LOW);
  // SRCLK clear pulse and default value
  digitalWrite(displaySerialSrclk, HIGH);
  // SRCLR default value
  digitalWrite(displaySerialSrclr, LOW);
  digitalWrite(displaySerialSrclr, HIGH);
  // RCLK store pulse and default value
  digitalWrite(displaySerialRclk, HIGH);
  digitalWrite(displaySerialRclk, LOW);
  digitalWrite(displaySerialRclk, HIGH);
  // OE default value
  digitalWrite(displaySerialOe, LOW);
}

void showDisplaySerial(uint16_t value)
{
  uint8_t aux8 = 0;
 
  // Clear
  digitalWrite(displaySerialSrclr, LOW);
  digitalWrite(displaySerialSrclr, HIGH);

  for(int j = 0;j < displaySerialDisplays;j++){
    aux8 = hexToSevenSegments(value % 10);
    value /= 10;
    // Desloca
    for(int i = 0;i < 8;i++){
      if(isBitSet(aux8, i) == 1) {
        digitalWrite(displaySerialDi, HIGH);
      } else {
        digitalWrite(displaySerialDi, LOW);
      }
      digitalWrite(displaySerialSrclk, LOW);
      digitalWrite(displaySerialSrclk, HIGH);
    }
  }
  // Grava
  digitalWrite(displaySerialRclk, LOW);
  digitalWrite(displaySerialRclk, HIGH);
}

