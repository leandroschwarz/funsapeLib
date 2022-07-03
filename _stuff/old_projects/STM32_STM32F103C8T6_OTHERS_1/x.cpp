typedef struct __SPI_HandleTypeDef {
	SPI_TypeDef				*Instance;	/*!< SPI registers base address */
	SPI_InitTypeDef			Init;		 /*!< SPI communication parameters */
	uint8_t					*pTxBuffPtr;  /*!< Pointer to SPI Tx transfer Buffer */
	uint16_t				   TxXferSize;   /*!< SPI Tx Transfer size */
	volatile uint16_t			  TxXferCount;  /*!< SPI Tx Transfer Counter */
	uint8_t					*pRxBuffPtr;  /*!< Pointer to SPI Rx transfer Buffer */
	uint16_t				   RxXferSize;   /*!< SPI Rx Transfer size */
} SPI_HandleTypeDef;


typedef struct {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t CRCPR;
	volatile uint32_t RXCRCR;
	volatile uint32_t TXCRCR;
	volatile uint32_t I2SCFGR;
} SPI_TypeDef;

typedef struct {
	uint32_t Mode;				/*!< Specifies the SPI operating mode.
									This parameter can be a value of @ref SPI_Mode */
	uint32_t Direction;			/*!< Specifies the SPI Directional mode state.
									This parameter can be a value of @ref SPI_Direction */
	uint32_t DataSize;			/*!< Specifies the SPI data size.
									This parameter can be a value of @ref SPI_Data_Size */
	uint32_t CLKPolarity;		/*!< Specifies the serial clock steady state.
									This parameter can be a value of @ref SPI_Clock_Polarity */
	uint32_t CLKPhase;			/*!< Specifies the clock active edge for the bit capture.
									This parameter can be a value of @ref SPI_Clock_Phase */
	uint32_t NSS;				/*!< Specifies whether the NSS signal is managed by
									hardware (NSS pin) or by software using the SSI bit.
									This parameter can be a value of @ref SPI_Slave_Select_management */
	uint32_t BaudRatePrescaler;	/*!< Specifies the Baud Rate prescaler value which will be
									used to configure the transmit and receive SCK clock.
									This parameter can be a value of @ref SPI_BaudRate_Prescaler
									@note The communication clock is derived from the master
									 clock. The slave clock does not need to be set. */
	uint32_t FirstBit;			/*!< Specifies whether data transfers start from MSB or LSB bit.
									This parameter can be a value of @ref SPI_MSB_LSB_transmission */
	uint32_t TIMode;			/*!< Specifies if the TI mode is enabled or not.
									This parameter can be a value of @ref SPI_TI_mode */
	uint32_t CRCCalculation;	/*!< Specifies if the CRC calculation is enabled or not.
									This parameter can be a value of @ref SPI_CRC_Calculation */
	uint32_t CRCPolynomial;		/*!< Specifies the polynomial used for the CRC calculation.
									This parameter must be a number between Min_Data = 0 and Max_Data = 65535 */
} SPI_InitTypeDef;
