typedef enum portIo_e {
	PORT_IO_B							= 1,
	PORT_IO_C							= 2,
	PORT_IO_D							= 3,
} portIo_e;

#define GPIO_PIN_REG_OFFSET		0
#define GPIO_DDR_REG_OFFSET		1
#define GPIO_PORT_REG_OFFSET	2


volatile uint8_t *dirReg;
volatile uint8_t *outReg;
volatile uint8_t *inReg;

portIo_e x;

dirReg = &DDRx;
dirReg = (volatile uint8_t *)(GPIO_DDR_REG_OFFSET + x * 3);
