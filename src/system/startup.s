/*******************************************************************************
    Autor:
        Ezequiel Donhauser
    
    Codigo:
        Startup para STM32F407VG

    Boot:
        Quando o microcontrolador é inicializado, o processador carrega
        para o registrador SP (Stack Pointer) o valor que estiver no
        endereço 0x00000000 que é o endereço fisico da flash, e o virtual
        0x80000000, esse valor será 0x20020000 que é o ultimo endereço na
        memoria RAM, feito isso o microcontrolador vai carregar para o
        registrador PC (Program Counter) o valor que estiver no endereço
        0x00000004 (fisico) ou 0x80000004 (virtual), esse valor será o 
        endereço para a label "NVIC_Reset_Handler" no qual o programa
        se iniciará.

    Descrição:
        Este codigo faz o startup do microcontrolador STM32F407VG,
        Configura FPU, e chama funcao user_main
    
*******************************************************************************/

.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb

.word  _sidata 
.word  _sdata
.word  _edata
.word  _sbss
.word  _ebss

/* para as interrupçoes nao implementadas chama esse bloco 
   que por sua vez nao faz nada apenas retorna para onde
   esta executando anteriormente */
.section .NVIC_Handler, #execinstr, #alloc
.type  Default_Handler, %function
Default_Handler:
    bx lr
    nop

/* primeiro bloco de codigo a ser executado
   apos a inicialização do microcontrolador */
.section .NVIC_Handler, #execinstr, #alloc
.type  NVIC_Reset_Handler, %function
NVIC_Reset_Handler:
	movs  r1, #0
	b  LoopCopyDataInit

CopyDataInit:
  ldr  r3, =_sidata
  ldr  r3, [r3, r1]
  str  r3, [r0, r1]
  adds  r1, r1, #4
    
LoopCopyDataInit:
  ldr  r0, =_sdata
  ldr  r3, =_edata
  adds  r2, r0, r1
  cmp  r2, r3
  bcc  CopyDataInit
  ldr  r2, =_sbss
  b  LoopFillZerobss
  
/* Zero fill the bss segment. */  
FillZerobss:
	movs  r3, #0
	str  r3, [r2], #4

/**/  
LoopFillZerobss:
    ldr  r3, = _ebss
    cmp  r2, r3
    bcc  FillZerobss
    ldr  r0, =0xE000ED88           
    ldr  r1,[r0]
    orr  r1,r1,#(0xF << 20)
    str  r1,[r0]
    bl   SystemInit   
    bl   user_main
    bx   lr   

/* vetor de interrupção, quando ocorrer uma interrupção
   será chamada a label correspondente, se tiver sido
   implementada, senao chama label padrão "Default_Handler" */
.section  .NVIC_Table, #alloc
.type  isr_vector, %object
isr_vector:
  .word  0x10010000
  .word  NVIC_Reset_Handler                 /* Init             */ 
  .word  NVIC_NMI_Handler                   
  .word  NVIC_HardFault_Handler             
  .word  NVIC_MemManage_Handler             
  .word  NVIC_BusFault_Handler              
  .word  NVIC_UsageFault_Handler            
  .word  0
  .word  0
  .word  0
  .word  0
  .word  NVIC_SVC_Handler                   
  .word  NVIC_DebugMon_Handler              
  .word  0
  .word  NVIC_PendSV_Handler                
  .word  NVIC_SysTick_Handler               /* System Timer                 */
  .word  NVIC_WDG_IRQHandler                /* Window WatchDog              */                                        
  .word  NVIC_PVD_IRQHandler                /* PVD  EXTI Line detection     */                        
  .word  NVIC_TAMP_STAMP_IRQHandler         /* Tamper, TStamp the EXTI line */            
  .word  NVIC_RTC_WKUP_IRQHandler           /* RTC Wakeup  the EXTI line    */                      
  .word  NVIC_FLASH_IRQHandler              /* FLASH                        */                                          
  .word  NVIC_RCC_IRQHandler                /* RCC                          */                                            
  .word  NVIC_EXTI0_IRQHandler              /* EXTI Line0                   */                        
  .word  NVIC_EXTI1_IRQHandler              /* EXTI Line1                   */                          
  .word  NVIC_EXTI2_IRQHandler              /* EXTI Line2                   */                          
  .word  NVIC_EXTI3_IRQHandler              /* EXTI Line3                   */                          
  .word  NVIC_EXTI4_IRQHandler              /* EXTI Line4                   */                          
  .word  NVIC_DMA1_Stream0_IRQHandler       /* DMA1 Stream 0                */                  
  .word  NVIC_DMA1_Stream1_IRQHandler       /* DMA1 Stream 1                */                   
  .word  NVIC_DMA1_Stream2_IRQHandler       /* DMA1 Stream 2                */                   
  .word  NVIC_DMA1_Stream3_IRQHandler       /* DMA1 Stream 3                */                   
  .word  NVIC_DMA1_Stream4_IRQHandler       /* DMA1 Stream 4                */                   
  .word  NVIC_DMA1_Stream5_IRQHandler       /* DMA1 Stream 5                */                   
  .word  NVIC_DMA1_Stream6_IRQHandler       /* DMA1 Stream 6                */                   
  .word  NVIC_ADC_IRQHandler                /* ADC1, ADC2 and ADC3s         */                   
  .word  NVIC_CAN1_TX_IRQHandler            /* CAN1 TX                      */                         
  .word  NVIC_CAN1_RX0_IRQHandler           /* CAN1 RX0                     */                          
  .word  NVIC_CAN1_RX1_IRQHandler           /* CAN1 RX1                     */                          
  .word  NVIC_CAN1_SCE_IRQHandler           /* CAN1 SCE                     */                          
  .word  NVIC_EXTI9_5_IRQHandler            /* External Line[9:5]s          */                          
  .word  NVIC_TIM1_BRK_TIM9_IRQHandler      /* TIM1 Break and TIM9          */         
  .word  NVIC_TIM1_UP_TIM10_IRQHandler      /* TIM1 Update and TIM10        */         
  .word  NVIC_TIM1_TRG_COM_TIM11_IRQHandler /* TIM1 Trigger, COM, TIM11     */
  .word  NVIC_TIM1_CC_IRQHandler            /* TIM1 Capture Compare         */                          
  .word  NVIC_TIM2_IRQHandler               /* TIM2                         */                   
  .word  NVIC_TIM3_IRQHandler               /* TIM3                         */                   
  .word  NVIC_TIM4_IRQHandler               /* TIM4                         */                   
  .word  NVIC_I2C1_EV_IRQHandler            /* I2C1 Event                   */                          
  .word  NVIC_I2C1_ER_IRQHandler            /* I2C1 Error                   */                          
  .word  NVIC_I2C2_EV_IRQHandler            /* I2C2 Event                   */                          
  .word  NVIC_I2C2_ER_IRQHandler            /* I2C2 Error                   */                            
  .word  NVIC_SPI1_IRQHandler               /* SPI1                         */                   
  .word  NVIC_SPI2_IRQHandler               /* SPI2                         */                   
  .word  NVIC_USART1_IRQHandler             /* USART1                       */                   
  .word  NVIC_USART2_IRQHandler             /* USART2                       */                   
  .word  NVIC_USART3_IRQHandler             /* USART3                       */                   
  .word  NVIC_EXTI15_10_IRQHandler          /* External Line[15:10]s        */                          
  .word  NVIC_RTC_Alarm_IRQHandler          /* RTC Alarm (A & B) EXTI Line  */                 
  .word  NVIC_OTG_FS_WKUP_IRQHandler        /* USB OTG FS Wakeup  EXTI line */                       
  .word  NVIC_TIM8_BRK_TIM12_IRQHandler     /* TIM8 Break and TIM12         */         
  .word  NVIC_TIM8_UP_TIM13_IRQHandler      /* TIM8 Update and TIM13        */         
  .word  NVIC_TIM8_TRG_COM_TIM14_IRQHandler /* TIM8 Trigger and  and TIM14  */
  .word  NVIC_TIM8_CC_IRQHandler            /* TIM8 Capture Compare         */                          
  .word  NVIC_DMA1_Stream7_IRQHandler       /* DMA1 Stream7                 */                          
  .word  NVIC_FSMC_IRQHandler               /* FSMC                         */                   
  .word  NVIC_SDIO_IRQHandler               /* SDIO                         */                   
  .word  NVIC_TIM5_IRQHandler               /* TIM5                         */                   
  .word  NVIC_SPI3_IRQHandler               /* SPI3                         */                   
  .word  NVIC_UART4_IRQHandler              /* UART4                        */                   
  .word  NVIC_UART5_IRQHandler              /* UART5                        */                   
  .word  NVIC_TIM6_DAC_IRQHandler           /* TIM6, DAC1&2 underrun errors */                   
  .word  NVIC_TIM7_IRQHandler               /* TIM7                         */
  .word  NVIC_DMA2_Stream0_IRQHandler       /* DMA2 Stream 0                */                   
  .word  NVIC_DMA2_Stream1_IRQHandler       /* DMA2 Stream 1                */                   
  .word  NVIC_DMA2_Stream2_IRQHandler       /* DMA2 Stream 2                */                   
  .word  NVIC_DMA2_Stream3_IRQHandler       /* DMA2 Stream 3                */                   
  .word  NVIC_DMA2_Stream4_IRQHandler       /* DMA2 Stream 4                */                   
  .word  NVIC_ETH_IRQHandler                /* Ethernet                     */                   
  .word  NVIC_ETH_WKUP_IRQHandler           /* Ethernet Wakeup  EXTI line   */                     
  .word  NVIC_CAN2_TX_IRQHandler            /* CAN2 TX                      */                          
  .word  NVIC_CAN2_RX0_IRQHandler           /* CAN2 RX0                     */                          
  .word  NVIC_CAN2_RX1_IRQHandler           /* CAN2 RX1                     */                          
  .word  NVIC_CAN2_SCE_IRQHandler           /* CAN2 SCE                     */                          
  .word  NVIC_OTG_FS_IRQHandler             /* USB OTG FS                   */                   
  .word  NVIC_DMA2_Stream5_IRQHandler       /* DMA2 Stream 5                */                   
  .word  NVIC_DMA2_Stream6_IRQHandler       /* DMA2 Stream 6                */                   
  .word  NVIC_DMA2_Stream7_IRQHandler       /* DMA2 Stream 7                */                   
  .word  NVIC_USART6_IRQHandler             /* USART6                       */                    
  .word  NVIC_I2C3_EV_IRQHandler            /* I2C3 event                   */                          
  .word  NVIC_I2C3_ER_IRQHandler            /* I2C3 error                   */                          
  .word  NVIC_OTG_HS_EP1_OUT_IRQHandler     /* USB OTG HS End Point 1 Out   */                   
  .word  NVIC_OTG_HS_EP1_IN_IRQHandler      /* USB OTG HS End Point 1 In    */                   
  .word  NVIC_OTG_HS_WKUP_IRQHandler        /* USB OTG HS Wakeup  EXTI      */                         
  .word  NVIC_OTG_HS_IRQHandler             /* USB OTG HS                   */                   
  .word  NVIC_DCMI_IRQHandler               /* DCMI                         */                   
  .word  NVIC_CRYP_IRQHandler               /* CRYP crypto                  */                   
  .word  NVIC_HASH_RNG_IRQHandler           /* Hash and Rng                 */
  .word  NVIC_FPU_IRQHandler                /* FPU                          */                         
                         
/* para implementar uma dessas funcoes basta copiar o nome
   e implmentar a funcao ex: void NVIC_NMI_Handler(void)
*******************************************************************************/
.weak      NVIC_NMI_Handler
.thumb_set NVIC_NMI_Handler,Default_Handler

.weak      NVIC_HardFault_Handler
.thumb_set NVIC_HardFault_Handler,Default_Handler

.weak      NVIC_MemManage_Handler
.thumb_set NVIC_MemManage_Handler,Default_Handler

.weak      NVIC_BusFault_Handler
.thumb_set NVIC_BusFault_Handler,Default_Handler

.weak      NVIC_UsageFault_Handler
.thumb_set NVIC_UsageFault_Handler,Default_Handler

.weak      NVIC_SVC_Handler
.thumb_set NVIC_SVC_Handler,Default_Handler

.weak      NVIC_DebugMon_Handler
.thumb_set NVIC_DebugMon_Handler,Default_Handler

.weak      NVIC_PendSV_Handler
.thumb_set NVIC_PendSV_Handler,Default_Handler

.weak      NVIC_SysTick_Handler
.thumb_set NVIC_SysTick_Handler,Default_Handler              

.weak      NVIC_WDG_IRQHandler                   
.thumb_set NVIC_WDG_IRQHandler,Default_Handler      

.weak      NVIC_PVD_IRQHandler      
.thumb_set NVIC_PVD_IRQHandler,Default_Handler

.weak      NVIC_TAMP_STAMP_IRQHandler            
.thumb_set NVIC_TAMP_STAMP_IRQHandler,Default_Handler

.weak      NVIC_RTC_WKUP_IRQHandler                  
.thumb_set NVIC_RTC_WKUP_IRQHandler,Default_Handler

.weak      NVIC_FLASH_IRQHandler         
.thumb_set NVIC_FLASH_IRQHandler,Default_Handler

.weak      NVIC_RCC_IRQHandler      
.thumb_set NVIC_RCC_IRQHandler,Default_Handler

.weak      NVIC_EXTI0_IRQHandler         
.thumb_set NVIC_EXTI0_IRQHandler,Default_Handler

.weak      NVIC_EXTI1_IRQHandler         
.thumb_set NVIC_EXTI1_IRQHandler,Default_Handler

.weak      NVIC_EXTI2_IRQHandler         
.thumb_set NVIC_EXTI2_IRQHandler,Default_Handler 

.weak      NVIC_EXTI3_IRQHandler         
.thumb_set NVIC_EXTI3_IRQHandler,Default_Handler

.weak      NVIC_EXTI4_IRQHandler         
.thumb_set NVIC_EXTI4_IRQHandler,Default_Handler

.weak      NVIC_DMA1_Stream0_IRQHandler               
.thumb_set NVIC_DMA1_Stream0_IRQHandler,Default_Handler

.weak      NVIC_DMA1_Stream1_IRQHandler               
.thumb_set NVIC_DMA1_Stream1_IRQHandler,Default_Handler

.weak      NVIC_DMA1_Stream2_IRQHandler               
.thumb_set NVIC_DMA1_Stream2_IRQHandler,Default_Handler

.weak      NVIC_DMA1_Stream3_IRQHandler               
.thumb_set NVIC_DMA1_Stream3_IRQHandler,Default_Handler 

.weak      NVIC_DMA1_Stream4_IRQHandler              
.thumb_set NVIC_DMA1_Stream4_IRQHandler,Default_Handler

.weak      NVIC_DMA1_Stream5_IRQHandler               
.thumb_set NVIC_DMA1_Stream5_IRQHandler,Default_Handler

.weak      NVIC_DMA1_Stream6_IRQHandler               
.thumb_set NVIC_DMA1_Stream6_IRQHandler,Default_Handler

.weak      NVIC_ADC_IRQHandler      
.thumb_set NVIC_ADC_IRQHandler,Default_Handler

.weak      NVIC_CAN1_TX_IRQHandler   
.thumb_set NVIC_CAN1_TX_IRQHandler,Default_Handler

.weak      NVIC_CAN1_RX0_IRQHandler                  
.thumb_set NVIC_CAN1_RX0_IRQHandler,Default_Handler

.weak      NVIC_CAN1_RX1_IRQHandler                  
.thumb_set NVIC_CAN1_RX1_IRQHandler,Default_Handler

.weak      NVIC_CAN1_SCE_IRQHandler                  
.thumb_set NVIC_CAN1_SCE_IRQHandler,Default_Handler

.weak      NVIC_EXTI9_5_IRQHandler   
.thumb_set NVIC_EXTI9_5_IRQHandler,Default_Handler

.weak      NVIC_TIM1_BRK_TIM9_IRQHandler            
.thumb_set NVIC_TIM1_BRK_TIM9_IRQHandler,Default_Handler

.weak      NVIC_TIM1_UP_TIM10_IRQHandler            
.thumb_set NVIC_TIM1_UP_TIM10_IRQHandler,Default_Handler

.weak      NVIC_TIM1_TRG_COM_TIM11_IRQHandler      
.thumb_set NVIC_TIM1_TRG_COM_TIM11_IRQHandler,Default_Handler

.weak      NVIC_TIM1_CC_IRQHandler   
.thumb_set NVIC_TIM1_CC_IRQHandler,Default_Handler

.weak      NVIC_TIM2_IRQHandler            
.thumb_set NVIC_TIM2_IRQHandler,Default_Handler

.weak      NVIC_TIM3_IRQHandler            
.thumb_set NVIC_TIM3_IRQHandler,Default_Handler

.weak      NVIC_TIM4_IRQHandler            
.thumb_set NVIC_TIM4_IRQHandler,Default_Handler

.weak      NVIC_I2C1_EV_IRQHandler   
.thumb_set NVIC_I2C1_EV_IRQHandler,Default_Handler

.weak      NVIC_I2C1_ER_IRQHandler   
.thumb_set NVIC_I2C1_ER_IRQHandler,Default_Handler

.weak      NVIC_I2C2_EV_IRQHandler   
.thumb_set NVIC_I2C2_EV_IRQHandler,Default_Handler

.weak      NVIC_I2C2_ER_IRQHandler   
.thumb_set NVIC_I2C2_ER_IRQHandler,Default_Handler

.weak      NVIC_SPI1_IRQHandler            
.thumb_set NVIC_SPI1_IRQHandler,Default_Handler

.weak      NVIC_SPI2_IRQHandler            
.thumb_set NVIC_SPI2_IRQHandler,Default_Handler

.weak      NVIC_USART1_IRQHandler      
.thumb_set NVIC_USART1_IRQHandler,Default_Handler

.weak      NVIC_USART2_IRQHandler      
.thumb_set NVIC_USART2_IRQHandler,Default_Handler

.weak      NVIC_USART3_IRQHandler      
.thumb_set NVIC_USART3_IRQHandler,Default_Handler

.weak      NVIC_EXTI15_10_IRQHandler               
.thumb_set NVIC_EXTI15_10_IRQHandler,Default_Handler

.weak      NVIC_RTC_Alarm_IRQHandler               
.thumb_set NVIC_RTC_Alarm_IRQHandler,Default_Handler

.weak      NVIC_OTG_FS_WKUP_IRQHandler         
.thumb_set NVIC_OTG_FS_WKUP_IRQHandler,Default_Handler

.weak      NVIC_TIM8_BRK_TIM12_IRQHandler         
.thumb_set NVIC_TIM8_BRK_TIM12_IRQHandler,Default_Handler

.weak      NVIC_TIM8_UP_TIM13_IRQHandler            
.thumb_set NVIC_TIM8_UP_TIM13_IRQHandler,Default_Handler

.weak      NVIC_TIM8_TRG_COM_TIM14_IRQHandler      
.thumb_set NVIC_TIM8_TRG_COM_TIM14_IRQHandler,Default_Handler

.weak      NVIC_TIM8_CC_IRQHandler   
.thumb_set NVIC_TIM8_CC_IRQHandler,Default_Handler

.weak      NVIC_DMA1_Stream7_IRQHandler               
.thumb_set NVIC_DMA1_Stream7_IRQHandler,Default_Handler

.weak      NVIC_FSMC_IRQHandler            
.thumb_set NVIC_FSMC_IRQHandler,Default_Handler

.weak      NVIC_SDIO_IRQHandler            
.thumb_set NVIC_SDIO_IRQHandler,Default_Handler

.weak      NVIC_TIM5_IRQHandler            
.thumb_set NVIC_TIM5_IRQHandler,Default_Handler

.weak      NVIC_SPI3_IRQHandler            
.thumb_set NVIC_SPI3_IRQHandler,Default_Handler

.weak      NVIC_UART4_IRQHandler         
.thumb_set NVIC_UART4_IRQHandler,Default_Handler

.weak      NVIC_UART5_IRQHandler         
.thumb_set NVIC_UART5_IRQHandler,Default_Handler

.weak      NVIC_TIM6_DAC_IRQHandler                  
.thumb_set NVIC_TIM6_DAC_IRQHandler,Default_Handler

.weak      NVIC_TIM7_IRQHandler            
.thumb_set NVIC_TIM7_IRQHandler,Default_Handler

.weak      NVIC_DMA2_Stream0_IRQHandler               
.thumb_set NVIC_DMA2_Stream0_IRQHandler,Default_Handler

.weak      NVIC_DMA2_Stream1_IRQHandler               
.thumb_set NVIC_DMA2_Stream1_IRQHandler,Default_Handler

.weak      NVIC_DMA2_Stream2_IRQHandler               
.thumb_set NVIC_DMA2_Stream2_IRQHandler,Default_Handler

.weak      NVIC_DMA2_Stream3_IRQHandler               
.thumb_set NVIC_DMA2_Stream3_IRQHandler,Default_Handler

.weak      NVIC_DMA2_Stream4_IRQHandler               
.thumb_set NVIC_DMA2_Stream4_IRQHandler,Default_Handler

.weak      NVIC_ETH_IRQHandler      
.thumb_set NVIC_ETH_IRQHandler,Default_Handler

.weak      NVIC_ETH_WKUP_IRQHandler                  
.thumb_set NVIC_ETH_WKUP_IRQHandler,Default_Handler

.weak      NVIC_CAN2_TX_IRQHandler   
.thumb_set NVIC_CAN2_TX_IRQHandler,Default_Handler

.weak      NVIC_CAN2_RX0_IRQHandler                  
.thumb_set NVIC_CAN2_RX0_IRQHandler,Default_Handler

.weak      NVIC_CAN2_RX1_IRQHandler                  
.thumb_set NVIC_CAN2_RX1_IRQHandler,Default_Handler

.weak      NVIC_CAN2_SCE_IRQHandler                  
.thumb_set NVIC_CAN2_SCE_IRQHandler,Default_Handler

.weak      NVIC_OTG_FS_IRQHandler      
.thumb_set NVIC_OTG_FS_IRQHandler,Default_Handler

.weak      NVIC_DMA2_Stream5_IRQHandler               
.thumb_set NVIC_DMA2_Stream5_IRQHandler,Default_Handler

.weak      NVIC_DMA2_Stream6_IRQHandler               
.thumb_set NVIC_DMA2_Stream6_IRQHandler,Default_Handler

.weak      NVIC_DMA2_Stream7_IRQHandler               
.thumb_set NVIC_DMA2_Stream7_IRQHandler,Default_Handler

.weak      NVIC_USART6_IRQHandler      
.thumb_set NVIC_USART6_IRQHandler,Default_Handler

.weak      NVIC_I2C3_EV_IRQHandler   
.thumb_set NVIC_I2C3_EV_IRQHandler,Default_Handler

.weak      NVIC_I2C3_ER_IRQHandler   
.thumb_set NVIC_I2C3_ER_IRQHandler,Default_Handler

.weak      NVIC_OTG_HS_EP1_OUT_IRQHandler         
.thumb_set NVIC_OTG_HS_EP1_OUT_IRQHandler,Default_Handler

.weak      NVIC_OTG_HS_EP1_IN_IRQHandler            
.thumb_set NVIC_OTG_HS_EP1_IN_IRQHandler,Default_Handler

.weak      NVIC_OTG_HS_WKUP_IRQHandler         
.thumb_set NVIC_OTG_HS_WKUP_IRQHandler,Default_Handler

.weak      NVIC_OTG_HS_IRQHandler      
.thumb_set NVIC_OTG_HS_IRQHandler,Default_Handler

.weak      NVIC_DCMI_IRQHandler            
.thumb_set NVIC_DCMI_IRQHandler,Default_Handler

.weak      NVIC_CRYP_IRQHandler            
.thumb_set NVIC_CRYP_IRQHandler,Default_Handler

.weak      NVIC_HASH_RNG_IRQHandler                  
.thumb_set NVIC_HASH_RNG_IRQHandler,Default_Handler   

.weak      NVIC_FPU_IRQHandler                  
.thumb_set NVIC_FPU_IRQHandler,Default_Handler  
