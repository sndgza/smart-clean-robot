#ifndef _BSP_SPEEDTEST_H
#define _BSP_SPEEDTEST_H
#include "stm32f10x.h"
#define             SPEED_CLK_TIM                   TIM6
#define             SPEED_CLK_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define             SPEED_CLK_TIM_CLK               RCC_APB1Periph_TIM6
#define             SPEED_CLK_TIM_Period            1000-1
#define             SPEED_CLK_TIM_Prescaler         71
#define             SPEED_CLK_TIM_IRQ               TIM6_IRQn
#define             SPEED_CLK_TIM_IRQHandler        TIM6_IRQHandler


#define             CAP_TIM                     TIM2
#define             CAP_TIM_CLC                 RCC_APB1Periph_TIM2
#define             CAP_TIM_IT_CCx              TIM_IT_CC3
#define             CAP_TIM_Channelx            TIM_Channel_3

#define             CAP_PORT                    GPIOA
#define             CAP_PIN                     GPIO_Pin_2
#define             CAP_PORT_CLC                RCC_APB2Periph_GPIOA





void speedtest_init(uint16_t arr,uint16_t prc);



#endif



