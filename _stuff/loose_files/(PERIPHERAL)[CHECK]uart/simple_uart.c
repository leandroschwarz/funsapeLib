#include "LPC11XX.h"
// Function declarations
void          UartConfig(void);         // Uart configuration
unsigned char UartPutc(unsigned char my_ch);     // Uart character output
void          UartPuts(unsigned char * mytext);  // Uart string output
int           UartGetRxDataAvail(void); // Detect new data are received
unsigned char UartGetRxData(void);      // Get received data from Uart

int main(void)
{
  SystemInit();
  UartConfig();

  UartPuts("Hello\n");
  while(1){
    if (UartGetRxDataAvail()) {
	  UartPutc(UartGetRxData());  // echo received data
	  }	// end if
    } // end while
}

// Uart string output
void UartPuts(unsigned char * mytext)
{
  unsigned char CurrChar;
  CurrChar = *mytext;
  while (CurrChar != (char) 0x0){
    UartPutc(CurrChar);  // Normal data
    mytext++;
    CurrChar = *mytext;
    }
  return;
}

void UartConfig(void)
{
  // UART interface are : PIO1_7 (TXD) and PIC1_6 (RXD)
  // Other UART signals (DTR, DSR, CTS, RTS, RI) are not used

  // Enable clock to IO configuration block 
  // (bit[16] of AHBCLOCK Control register)
  LPC_SYSCON->SYSAHBCLKCTRL = LPC_SYSCON->SYSAHBCLKCTRL | (1<<16);

  // PIO1_7 IO output config
  //  bit[5]   - Hysteresis (0=disable, 1 =enable)
  //  bit[4:3] - MODE(0=inactive, 1 =pulldown, 2=pullup, 3=repeater)
  //  bit[2:0] - Function (0 = IO, 1=TXD, 2=CT32B0_MAT1)
  LPC_IOCON->PIO1_7 = (0x1) + (0<<3) + (0<<5);
  // PIO1_6 IO input config
  //  bit[5]   - Hysteresis (0=disable, 1 =enable)
  //  bit[4:3] - MODE(0=inactive, 1 =pulldown, 2=pullup, 3=repeater)
  //  bit[2:0] - Function (0 = IO, 1=RXD, 2=CT32B0_MAT0)
  LPC_IOCON->PIO1_6 = (0x1) + (2<<3) + (1<<5);

  // Enable clock to UART (bit[12] of AHBCLOCK Control register
  LPC_SYSCON->SYSAHBCLKCTRL = LPC_SYSCON->SYSAHBCLKCTRL | (1<<12);
  // UART_PCLK divide ratio = 1
  LPC_SYSCON->UARTCLKDIV = 1;

  // UART_PCLK = 48MHz, Baudrate = 38400, divide ratio = 1250
  // Line Control Register
  LPC_UART->LCR = (1<<7) |   // Enable access to Divisor Latches
    (0<<6) |   // Disable Break Control
    (0<<4) |   // Bit[5:4] parity select (odd, even, sticky-1, sticky-0)
    (0<<3) |   // parity disabled
    (0<<2) |   // 1 stop bit
    (3<<0);    // 8-bit data
  LPC_UART->DLL = 78;  // Divisor Latch Least Significant Byte 
                       // 48MHz/38400/16 = 78.125
  LPC_UART->DLM = 0;   // Divisor Latch Most Significant Byte  : 0
  LPC_UART->LCR = (0<<7) |   // Disable access to Divisor Latches
    (0<<6) |   // Disable Break Control
    (0<<4) |   // Bit[5:4] parity select (odd, even, sticky-1, sticky-0)
    (0<<3) |   // parity disabled
    (0<<2) |   // 1 stop bit
    (3<<0);    // 8-bit data

  LPC_UART->FCR = 1; // Enable FIFO

  return;
}
// Get received data
__inline unsigned char UartGetRxData(void)
{
  return ((char)LPC_UART->RBR);
}
// Detect if new received data are available
__inline int  UartGetRxDataAvail(void){
  return (LPC_UART->LSR & 0x1);
}
// Output a character, with additional formatting for HyperTerminal
unsigned char UartPutc(unsigned char my_ch)
{
  if (my_ch == '\n') {
    while ((LPC_UART->LSR & (1<<5))==0); 
	  // Wait if Transmit Holding register is not empty
    LPC_UART->THR = 13;  
	  // Output carriage return (for Windows Hyperterminal)
    }
  while ((LPC_UART->LSR & (1<<5))==0); 
      // Wait if Transmit Holding register is not empty
  LPC_UART->THR = my_ch; // write to transmit holding register

  if (my_ch == 13) {
    while ((LPC_UART->LSR & (1<<5))==0); 
	   // Wait if Transmit Holding register is not empty
    LPC_UART->THR = 10;  
	   // Output new line (for Windows Hyperterminal)
    }
  return (my_ch);
}

