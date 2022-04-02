#ifndef  _HCSR_H
#define  _HCSR_H

#include "stm32f10x.h"

#define  HCSR_TIM_APBxClock_FUN             RCC_APB1PeriphClockCmd
#define  HCSR_TIM                           TIM2
#define  HCSR_TIM_CLK                       RCC_APB1Periph_TIM2
#define  HCSR_TIM_PERIOD                    0xFFFF
#define  HCSR_TIM_PRESCALER                 71
#define  HCSR_TIM_COUNTERMODE               TIM_CounterMode_Up

#define  HCSR_GPIO_OUT_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define  HCSR_GPIO_OUTPORT                  GPIOB
#define  HCSR_GPIO_OUTCLK                   RCC_APB2Periph_GPIOB
#define  HCSR_GPIO_OUTPIN                   GPIO_Pin_6
#define  HCSR_GPIO_OUTSPEED                 GPIO_Speed_50MHz
#define  HCSR_GPIO_OUTMODE                  GPIO_Mode_Out_PP

#define  HCSR_GPIO_IN_APBxClock_FUN         RCC_APB2PeriphClockCmd
#define  HCSR_GPIO_INPORT                   GPIOB
#define  HCSR_GPIO_INCLK                    RCC_APB2Periph_GPIOB
#define  HCSR_GPIO_INPIN                    GPIO_Pin_7
#define  HCSR_GPIO_INSPEED                  GPIO_Speed_50MHz
#define  HCSR_GPIO_INMODE                   GPIO_Mode_IN_FLOATING


#define  HCSR_EXTI_INCLK                    RCC_APB2Periph_AFIO
#define  HCSR_EXTI_PORTSOURCE               GPIO_PortSourceGPIOB
#define  HCSR_EXTI_PINSOURCE                GPIO_PinSource7
#define  HCSR_EXTI_LINE                     EXTI_Line7
#define  HCSR_EXTI_IRQ                      EXTI9_5_IRQn
#define  HCSR_IRQHandler                    EXTI9_5_IRQHandler

void Hcsr_TIMx_Count_Config(void);
void HCsr_GPIO_Config(void);
void HCsr_EXTI_Close(void);
void HCsr_EXTI_Open(void);


#endif
