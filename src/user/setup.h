typedef enum{

	UART_BAUDRATE_9600,     //0
	UART_BAUDRATE_19200,	//1
	UART_BAUDRATE_38400,	//2
	UART_BAUDRATE_115200	//3
} ENUM_UART_BAUDRATE;


typedef enum{
	UART_PARITY_NONE,//0
	UART_PARITY_EVEN,//1
	UART_PARITY_ODD//2
} ENUM_UART_PARITY;

typedef enum{
	UART_STOPBIT_ONE,//0
	UART_STOPBIT_TWO//1
} ENUM_UART_STOPBIT;


uint16_t ADCBuffer[3];
void setup(void);
uint16_t get_adc_channel(uint8_t channel);
void	Setup_UART2_SetBaudRate	( ENUM_UART_BAUDRATE BaudRate );
void	Setup_UART2_SetParity	( ENUM_UART_PARITY parity );
void 	Setup_UART2_SetStopBit	( ENUM_UART_STOPBIT stopbit );
