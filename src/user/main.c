#include "interrupt.h"
#include "main.h"
#include "misc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

int blablabla=43;

extern __IO uint32_t TimingDelay;
void Delay(__IO uint32_t val)
{
    uint32_t temp=10000;
    temp*=val;
    while(temp--);
}

void user_main(void)
{
    //ativa o barramento do gpioD
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

    //configura o pino GPIOD 15
    GPIO_InitTypeDef conf_pin;
    conf_pin.GPIO_Pin = GPIO_Pin_15;
    conf_pin.GPIO_Mode = GPIO_Mode_OUT;
    conf_pin.GPIO_Speed = GPIO_Speed_50MHz;
    conf_pin.GPIO_OType = GPIO_OType_PP;
    conf_pin.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &conf_pin);

    //desliga pino GPIOD pin15
    GPIO_ResetBits(GPIOD, GPIO_Pin_15);

    //pisca o led a cada 500ms
    while(1)
    {
        GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
        Delay(500);
    }
}
