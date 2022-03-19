#ifndef _BSP_MOTOR_H
#define _BSP_MOTOR_H
#include "stm32f10x.h"
#include "math.h"
#include "stdlib.h"

#define     MOTOR1_PORT1            GPIOA          
#define     MOTOR1_PIN1             GPIO_Pin_6
#define     MOTOR1_PORT1_CLK        RCC_APB2Periph_GPIOA 
#define     MOTOR1_TIM_CLK          RCC_APB1Periph_TIM3
#define     MOTOR1_TIM              TIM3
#define     MOTOR1_PULSE            5300


extern s32 motorLeft;
extern s32 motorRight;   

void motor1_init(u16 arr,u16 psc);


#endif
