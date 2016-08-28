#include "interrupt.h"
#include "memoria.h"
#include "misc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_dac.h"
#include "stm32f4xx_dac.h"

#include "setup.h"
static void init_gpio(void);
static void init_usart1(__IO uint32_t baudrate);
static void init_usart2(__IO uint32_t baudrate);
static void init_adc(void);
static void init_dma(void);
void init_timer2(void);
void init_timer3(void);
void init_extern_interrupt_GPIOD(void);
void init_SPI1( void );
void init_SPI2( void );
void init_I2C2(void);
void init_DAC1(void);
void init_timer4();
void setup(void)
{
   init_adc();
   init_dma();
   init_usart1(115200); //debug
   init_usart2(115200);//modbus
   init_gpio();
   init_timer2();
   init_timer3();
   //init_extern_interrupt_GPIOD();
   init_SPI1();
   init_SPI2();
    //init_I2C2();
  //  init_DAC1();
   init_timer4(); // pwm
}
void init_timer4()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
    TIM_TimeBaseInitTypeDef timerInitStructure;
    timerInitStructure.TIM_Prescaler = 15;
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = 4096;
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &timerInitStructure);
    TIM_Cmd(TIM4, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 
    
    GPIO_InitTypeDef gpioStructure;
    gpioStructure.GPIO_Pin = GPIO_Pin_12;
    gpioStructure.GPIO_Mode = GPIO_Mode_AF;
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &gpioStructure);
   
    TIM_OCInitTypeDef outputChannelInit = {0,};
    outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
    outputChannelInit.TIM_Pulse = 1000;
    outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
    outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High; 
    TIM_OC1Init(TIM4, &outputChannelInit);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); 
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);    
}
void init_DAC1(void)
{
 DAC_InitTypeDef DAC_InitStructure;
 GPIO_InitTypeDef GPIO_InitStruct;

/* GEPIO CONFIGURATION of DAC Pin */
RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 ;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
GPIO_Init(GPIOA, &GPIO_InitStruct);

/* DAC channel1 Configuration */ 
DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None; 
DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable ; 
DAC_Init(DAC_Channel_1, &DAC_InitStructure);
/* Enable DAC Channel1 */ 
DAC_Cmd(DAC_Channel_1, ENABLE);
    
    
}
void init_I2C2(void)
{
    I2C_InitTypeDef I2C_InitStruct;
    GPIO_InitTypeDef init_gpio;  
    GPIO_InitTypeDef conf_pin;
     NVIC_InitTypeDef init_nvic;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);    
    
    conf_pin.GPIO_Pin = GPIO_Pin_12;
    conf_pin.GPIO_Mode = GPIO_Mode_OUT;
    conf_pin.GPIO_Speed = GPIO_Speed_50MHz;
    conf_pin.GPIO_OType = GPIO_OType_PP;
    conf_pin.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &conf_pin);
    //desliga pino GPIOB pin12
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    
    init_gpio.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;     
    init_gpio.GPIO_Mode = GPIO_Mode_AF;              
    init_gpio.GPIO_Speed = GPIO_Speed_50MHz;         
    init_gpio.GPIO_OType = GPIO_OType_PP;            
    init_gpio.GPIO_PuPd = GPIO_PuPd_UP;              
    GPIO_Init(GPIOB, &init_gpio); 
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_I2C2);//SCL
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_I2C2);//SCK
    // configure I2C2
    I2C_InitStruct.I2C_ClockSpeed = 400000;       // 400kHz
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;           // I2C mode
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;   // 50% duty cycle --> standard
    I2C_InitStruct.I2C_OwnAddress1 = 0x00;            // own address, not relevant in master mode
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;     // disable acknowledge when reading (can be changed later on)
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // set address length to 7 bit addresses
    I2C_Init(I2C2, &I2C_InitStruct);              // init I2C2       
    I2C_Cmd(I2C2, ENABLE);
    I2C_ITConfig(I2C2, I2C_IT_TXE , DISABLE); 
   //SPI_ITConfig(SPI1, SPI_IT_RXNE ,DISABLE);  
    init_nvic.NVIC_IRQChannel = I2C2_EV_IRQn;
    init_nvic.NVIC_IRQChannelPreemptionPriority = 0;
    init_nvic.NVIC_IRQChannelSubPriority = 0;
    init_nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&init_nvic);
}
void init_SPI2( void )
{
   SPI_InitTypeDef SPI_InitStruct; 
   GPIO_InitTypeDef init_gpio;  
   //NVIC_InitTypeDef init_nvic;
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
   
   init_gpio.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;     
   init_gpio.GPIO_Mode = GPIO_Mode_AF;              
   init_gpio.GPIO_Speed = GPIO_Speed_50MHz;         
   init_gpio.GPIO_OType = GPIO_OType_PP;            
   init_gpio.GPIO_PuPd = GPIO_PuPd_UP;              
   GPIO_Init(GPIOB, &init_gpio); 
   
 /*  GPIO_PinAFConfig(GPIOA,GPIO_Pin_5,SPI1);
   GPIO_PinAFConfig(GPIOA,GPIO_Pin_6,SPI1);
   GPIO_PinAFConfig(GPIOA,GPIO_Pin_7,SPI1);*/
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2); 
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
   //GPIO_PinAFConfig(GPIOE,GPIO_Pin_3,SPI1);
    SPI_I2S_DeInit(SPI2);
    SPI_InitStruct.SPI_Direction= SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; 
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft ;
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStruct);
  //  SPI_SSOutputCmd(SPI1,ENABLE); //Set SSOE bit in SPI_CR1 register
    SPI_Cmd(SPI2, ENABLE);
    init_gpio.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    init_gpio.GPIO_Mode = GPIO_Mode_OUT;
    init_gpio.GPIO_Speed = GPIO_Speed_50MHz;
    init_gpio.GPIO_OType = GPIO_OType_PP;
    init_gpio.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &init_gpio);
  
    SPI_ITConfig(SPI1, SPI_IT_TXE , DISABLE); 
    SPI_ITConfig(SPI1, SPI_IT_RXNE ,DISABLE);  
    /*init_nvic.NVIC_IRQChannel = SPI1_IRQn;
    init_nvic.NVIC_IRQChannelPreemptionPriority = 0;
    init_nvic.NVIC_IRQChannelSubPriority = 0;
    init_nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&init_nvic);*/

}
void init_SPI1( void )
{
   SPI_InitTypeDef SPI_InitStruct; 
   GPIO_InitTypeDef init_gpio;  
//   NVIC_InitTypeDef init_nvic;
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
   
   init_gpio.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;     
   init_gpio.GPIO_Mode = GPIO_Mode_AF;              
   init_gpio.GPIO_Speed = GPIO_Speed_50MHz;         
   init_gpio.GPIO_OType = GPIO_OType_PP;            
   init_gpio.GPIO_PuPd = GPIO_PuPd_UP;              
   GPIO_Init(GPIOA, &init_gpio); 
   init_gpio.GPIO_Pin     = GPIO_Pin_3;
   init_gpio.GPIO_Mode = GPIO_Mode_OUT;
   init_gpio.GPIO_Speed = GPIO_Speed_50MHz;
   init_gpio.GPIO_OType = GPIO_OType_PP;
   init_gpio.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_Init( GPIOE, &init_gpio );
   
 /*  GPIO_PinAFConfig(GPIOA,GPIO_Pin_5,SPI1);
   GPIO_PinAFConfig(GPIOA,GPIO_Pin_6,SPI1);
   GPIO_PinAFConfig(GPIOA,GPIO_Pin_7,SPI1);*/
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1); 
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
   //GPIO_PinAFConfig(GPIOE,GPIO_Pin_3,SPI1);
    SPI_I2S_DeInit(SPI1);
    SPI_InitStruct.SPI_Direction= SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; 
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft ;
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStruct);
  //  SPI_SSOutputCmd(SPI1,ENABLE); //Set SSOE bit in SPI_CR1 register
    SPI_Cmd(SPI1, ENABLE);
   /* GPIO_SetBits(GPIOE, GPIO_Pin_3);
    SPI_ITConfig(SPI1, SPI_IT_TXE , DISABLE); 
    SPI_ITConfig(SPI1, SPI_IT_RXNE ,DISABLE);  
    init_nvic.NVIC_IRQChannel = SPI1_IRQn;
    init_nvic.NVIC_IRQChannelPreemptionPriority = 0;
    init_nvic.NVIC_IRQChannelSubPriority = 0;
    init_nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&init_nvic);*/

}
void init_extern_interrupt_GPIOD(void)
{
     /* Set variables used */
    GPIO_InitTypeDef GPIO_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    
    /* Enable clock for GPIOD */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    /* Enable clock for SYSCFG */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    /* Set pin as input */
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    /* Tell system that you will use PD0 for EXTI_Line0 */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource0);;
    /* PD0 is connected to EXTI_Line0 */
    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    /* Enable interrupt */
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    /* Interrupt mode */
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    /* Triggers on rising and falling edge */
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    /* Add to EXTI */
    EXTI_Init(&EXTI_InitStruct);
 
    /* Add IRQ vector to NVIC */
    /* PD0 is connected to EXTI_Line0, which has EXTI0_IRQn vector */
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    /* Set priority */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
    /* Set sub priority */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
    /* Enable interrupt */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    /* Add to NVIC */
    NVIC_Init(&NVIC_InitStruct);
}
static void init_gpio(void)
{
     //configura o pino GPIOD 12
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
    GPIO_InitTypeDef conf_pin;
    conf_pin.GPIO_Pin = GPIO_Pin_12;
    conf_pin.GPIO_Mode = GPIO_Mode_OUT;
    conf_pin.GPIO_Speed = GPIO_Speed_50MHz;
    conf_pin.GPIO_OType = GPIO_OType_PP;
    conf_pin.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &conf_pin);
    //desliga pino GPIOD pin12
    GPIO_SetBits(GPIOD, GPIO_Pin_12);
    
     //configura o pino GPIOD 13
    conf_pin.GPIO_Pin = GPIO_Pin_13;
    conf_pin.GPIO_Mode = GPIO_Mode_OUT;
    conf_pin.GPIO_Speed = GPIO_Speed_50MHz;
    conf_pin.GPIO_OType = GPIO_OType_PP;
    conf_pin.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &conf_pin);
    //desliga pino GPIOD pin13
    GPIO_ResetBits(GPIOD, GPIO_Pin_13);
    
    //configura o pino GPIOD 14
    conf_pin.GPIO_Pin = GPIO_Pin_14;
    conf_pin.GPIO_Mode = GPIO_Mode_OUT;
    conf_pin.GPIO_Speed = GPIO_Speed_50MHz;
    conf_pin.GPIO_OType = GPIO_OType_PP;
    conf_pin.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &conf_pin);
    //desliga pino GPIOD pin15
    GPIO_SetBits(GPIOD, GPIO_Pin_14);    
    
    //configura o pino GPIOD 15
    conf_pin.GPIO_Pin = GPIO_Pin_15;
    conf_pin.GPIO_Mode = GPIO_Mode_OUT;
    conf_pin.GPIO_Speed = GPIO_Speed_50MHz;
    conf_pin.GPIO_OType = GPIO_OType_PP;
    conf_pin.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &conf_pin);
    //desliga pino GPIOD pin15
    GPIO_SetBits(GPIOD, GPIO_Pin_15);
    //configura o pino GPIOD 15
    conf_pin.GPIO_Pin = GPIO_Pin_11;
    conf_pin.GPIO_Mode = GPIO_Mode_IN;
    conf_pin.GPIO_Speed = GPIO_Speed_50MHz;
    conf_pin.GPIO_OType = GPIO_OType_PP;
    conf_pin.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &conf_pin);
    //desliga pino GPIOD pin15
    GPIO_SetBits(GPIOD, GPIO_Pin_11);
}
void init_timer3(void)
{
NVIC_InitTypeDef NVIC_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
/* Enable the TIM2 gloabal Interrupt */
NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);
 
/* TIM2 clock enable */
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
/* Time base configuration */
TIM_TimeBaseStructure.TIM_Period = 976 - 1; // 1 MHz down to 1 KHz (1 ms)
TIM_TimeBaseStructure.TIM_Prescaler = 42 - 1; // 24 MHz Clock down to 1 MHz (adjust per your clock)
TIM_TimeBaseStructure.TIM_ClockDivision = 0;
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
/* TIM IT enable */
TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
/* TIM2 enable counter */
TIM_Cmd(TIM3, ENABLE);
} 
void init_timer2(void)
{
NVIC_InitTypeDef NVIC_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
/* Enable the TIM2 gloabal Interrupt */
NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);
 
/* TIM2 clock enable */
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
/* Time base configuration */
TIM_TimeBaseStructure.TIM_Period = 1000 - 1; // 1 MHz down to 1 KHz (1 ms)
TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1; // 24 MHz Clock down to 1 MHz (adjust per your clock)
TIM_TimeBaseStructure.TIM_ClockDivision = 0;
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
/* TIM IT enable */
TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
/* TIM2 enable counter */
TIM_Cmd(TIM2, ENABLE);
} 
static void init_usart2(__IO uint32_t baudrate)
{
    GPIO_InitTypeDef init_gpio;  
    USART_InitTypeDef init_usart;
    //USART_ClockInitTypeDef clock_uart;
    NVIC_InitTypeDef init_nvic;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 

    init_gpio.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;     
    init_gpio.GPIO_Mode = GPIO_Mode_AF;              
    init_gpio.GPIO_Speed = GPIO_Speed_50MHz;         
    init_gpio.GPIO_OType = GPIO_OType_PP;            
    init_gpio.GPIO_PuPd = GPIO_PuPd_UP;              
    GPIO_Init(GPIOD, &init_gpio); 

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);  
    
    init_usart.USART_BaudRate = baudrate;   
    init_usart.USART_WordLength = USART_WordLength_8b;
    init_usart.USART_StopBits = USART_StopBits_1;
    init_usart.USART_Parity = USART_Parity_No;
    init_usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    init_usart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;          
    USART_Init(USART2,&init_usart);
    
    //clock_uart.USART_Clock = USART_Clock_Enable;
    //clock_uart.USART_CPOL = USART_CPOL_High;
    //clock_uart.USART_CPHA = USART_CPHA_1Edge;
    //clock_uart.USART_LastBit = USART_LastBit_Enable;
    //USART_ClockInit(USART1,&clock_uart);
    
    USART_ITConfig(USART2, USART_IT_RXNE , ENABLE);
    USART_ITConfig(USART2,USART_IT_TXE, DISABLE);
    init_nvic.NVIC_IRQChannel = USART2_IRQn;
    init_nvic.NVIC_IRQChannelPreemptionPriority = 1;
    init_nvic.NVIC_IRQChannelSubPriority = 1;
    init_nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&init_nvic);
    
    // Enable the USART2
    USART_Cmd(USART2, ENABLE);
}
static void init_usart1(__IO uint32_t baudrate)
{
    GPIO_InitTypeDef init_gpio;  
    USART_InitTypeDef init_usart;
    //USART_ClockInitTypeDef clock_uart;
    NVIC_InitTypeDef init_nvic;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 

    init_gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;     
    init_gpio.GPIO_Mode = GPIO_Mode_AF;              
    init_gpio.GPIO_Speed = GPIO_Speed_50MHz;         
    init_gpio.GPIO_OType = GPIO_OType_PP;            
    init_gpio.GPIO_PuPd = GPIO_PuPd_UP;              
    GPIO_Init(GPIOB, &init_gpio); 

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);  
    
    init_usart.USART_BaudRate = baudrate;   
    init_usart.USART_WordLength = USART_WordLength_8b;
    init_usart.USART_StopBits = USART_StopBits_1;
    init_usart.USART_Parity = USART_Parity_No;
    init_usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    init_usart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;          
    USART_Init(USART1,&init_usart);
    
    //clock_uart.USART_Clock = USART_Clock_Enable;
    //clock_uart.USART_CPOL = USART_CPOL_High;
    //clock_uart.USART_CPHA = USART_CPHA_1Edge;
    //clock_uart.USART_LastBit = USART_LastBit_Enable;
    //USART_ClockInit(USART1,&clock_uart);
    
    USART_ITConfig(USART1, USART_IT_RXNE , ENABLE);
    USART_ITConfig(USART1,USART_IT_TXE, DISABLE);
    init_nvic.NVIC_IRQChannel = USART1_IRQn;
    init_nvic.NVIC_IRQChannelPreemptionPriority = 0;
    init_nvic.NVIC_IRQChannelSubPriority = 0;
    init_nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&init_nvic);
    
    // Enable the USART1
    USART_Cmd(USART1, ENABLE);
}
uint16_t get_adc_channel(uint8_t channel)
{
//    return ADC_values[channel];
    
}
static void init_adc(void)
{
    //--Enable ADC1 and GPIOA--
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure; //Variable used to setup the GPIO pins
    //==Configure ADC pins (PA0 -> Channel 0 to PA7 -> Channel 7) as analog inputs==
    GPIO_StructInit(&GPIO_InitStructure); // Reset init structure, if not it can cause issues...
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2|GPIO_Pin_3 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

 
    ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    
    /* init ADC clock */

    //ADC1 configuration
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    //We will convert multiple channels
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    //select continuous conversion mode
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//!
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
    //select external timer 2 triggering
     ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
    //right 12-bit data alignment in ADC data register
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    //8 channels conversion
    ADC_InitStructure.ADC_NbrOfConversion = 11;
    //load structure values to control and status registers
    ADC_Init(ADC1, &ADC_InitStructure);
    //wake up temperature sensor
    ADC_TempSensorVrefintCmd(ENABLE);
    // start ADC1 calibration and check the end
 
    //configure each channel
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_84Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_84Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_84Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_84Cycles);
    //ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_480Cycles);
    //ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_480Cycles);
    //ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 7, ADC_SampleTime_480Cycles);
    //ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 8, ADC_SampleTime_480Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 5, ADC_SampleTime_84Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 6, ADC_SampleTime_84Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 7, ADC_SampleTime_84Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 8, ADC_SampleTime_84Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 9, ADC_SampleTime_84Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 10, ADC_SampleTime_84Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 11, ADC_SampleTime_84Cycles);
}

void init_dma(void){
  /* Initialise DMA */
     NVIC_InitTypeDef NVIC_InitStructure;
        DMA_InitTypeDef DMA_InitStructure;
        DMA_StructInit(&DMA_InitStructure);
 
        /* Enable clock on DMA1 */
        /* Enable DMA2, thats where ADC is hooked on -> see Tab 20 (RM00090) */
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);               
        /* config of DMAC */
        DMA_InitStructure.DMA_Channel = DMA_Channel_0;                     
        DMA_InitStructure.DMA_BufferSize = 11;                       /* 2 * memsize */
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;     /* direction */
        DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;      /* no FIFO */
        DMA_InitStructure.DMA_FIFOThreshold = 0;
        DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
        DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
        DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;            /* circular buffer */
        DMA_InitStructure.DMA_Priority = DMA_Priority_High;        /* high priority */
        /* config of memory */
        DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&Gui_Memoria[VALOR_ADC1]; /* target address */
        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; /* 16 bit */
        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; /* increment after wrt */
        /* config of peripheral */
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
        DMA_Init(DMA2_Stream0, &DMA_InitStructure); /* See Table 20 for mapping */
        DMA_Cmd(DMA2_Stream0, ENABLE);
        
    // Enable DMA1 Channel Transfer Complete interrupt
   
   
    //Enable DMA1 channel IRQ Channel */
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);
 
}

void 	Setup_UART2_SetStopBit( ENUM_UART_STOPBIT stopbit )
{
	switch( stopbit )
	{
		default:
		case UART_STOPBIT_ONE:
		{
			stopbit = 0;
			break;
		}
		case UART_STOPBIT_TWO:
		{
			stopbit = 1;
			break;
		}
	}
	//U2MODEbits.STSEL	= stopbit;
}


void	Setup_UART2_SetParity( ENUM_UART_PARITY parity )
{
	switch( parity )
	{
		default:
		case UART_PARITY_NONE:
		{
			parity = 0;
			break;
		}
		case UART_PARITY_EVEN:
		{
			parity = 1;
			break;
		}
		case UART_PARITY_ODD:
		{
			parity = 2;
			break;
		}

	}
	//U2MODEbits.PDSEL	=	parity;
}

/**/
void	Setup_UART2_SetBaudRate( ENUM_UART_BAUDRATE BaudRate )
{
	uint16_t brg;
	switch( BaudRate )
	{
		case UART_BAUDRATE_9600:
		{
			brg = 371;
			break;
		}
		case UART_BAUDRATE_19200:
		{
			brg = 185;
			break;
		}
		case UART_BAUDRATE_38400:
		{
			brg = 92;
			break;
		}
		case UART_BAUDRATE_115200:
		{
			brg = 30;
			break;
		}
	}
	//U2BRG	=	brg;
}