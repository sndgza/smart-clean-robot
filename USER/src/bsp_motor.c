/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2022-03-17
  * @brief   电机驱动函数接口
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

#include "bsp_motor.h"




 /**
  * @brief  初始化控制电机的驱动
  * @param  arr: 自动重装载寄存器的值
  * @param  psc: 预分频系数
  * @retval 无
  */
void motor1_init(u16 arr,u16 psc)
{  

	GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    // 输出比较通道1 GPIO 初始化
    RCC_APB2PeriphClockCmd(MOTOR1_PORT1_CLK , ENABLE);
    GPIO_InitStructure.GPIO_Pin =  MOTOR1_PIN1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MOTOR1_PORT1, &GPIO_InitStructure);


    RCC_APB1PeriphClockCmd(MOTOR1_TIM_CLK,ENABLE);
   	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period= arr;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= psc;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(MOTOR1_TIM, &TIM_TimeBaseStructure);



    // 配置为PWM模式1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// 输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// 输出通道电平极性配置	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	// 输出通道空闲电平极性配置
	//TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	// 输出占空比
	TIM_OCInitStructure.TIM_Pulse = 3599;
	TIM_OC1Init(MOTOR1_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(MOTOR1_TIM, TIM_OCPreload_Enable);
    //TIM_ARRPreloadConfig(TIM3,ENABLE);
    TIM_Cmd(MOTOR1_TIM, ENABLE);
 
}




