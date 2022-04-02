/*
*******************************************************************************
* @file    bsp_hcsr.c
* @author  sndgza
* @version V1.1
* @date    2022-02-11
* @brief   超声波模块的应用
******************************************************************************
* @attention
******************************************************************************
*/ 
#include "bsp_hcsr.h"


 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 配置NVIC为优先级组2 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = HCSR_EXTI_IRQ;
  /* 配置抢占优先级 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 配置子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* 配置中断源：按键2，其他使用上面相关配置 */  
  NVIC_InitStructure.NVIC_IRQChannel = HCSR_EXTI_IRQ;
  NVIC_Init(&NVIC_InitStructure);
}


void Hcsr_TIMx_Count_Config()
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

    HCSR_TIM_APBxClock_FUN(HCSR_TIM_CLK,ENABLE);

    TIM_TimeBaseInitStruct.TIM_Period = HCSR_TIM_PERIOD;
    TIM_TimeBaseInitStruct.TIM_Prescaler = HCSR_TIM_PRESCALER;
    TIM_TimeBaseInitStruct.TIM_CounterMode = HCSR_TIM_COUNTERMODE;

    TIM_TimeBaseInit(HCSR_TIM,&TIM_TimeBaseInitStruct);

    RCC_APB2PeriphClockCmd(HCSR_TIM_CLK,DISABLE);
}


void HCsr_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    HCSR_GPIO_OUT_APBxClock_FUN(HCSR_GPIO_OUTCLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = HCSR_GPIO_OUTPIN;
    GPIO_InitStructure.GPIO_Speed = HCSR_GPIO_OUTSPEED;
    GPIO_InitStructure.GPIO_Mode = HCSR_GPIO_OUTMODE;
    GPIO_Init(HCSR_GPIO_OUTPORT,&GPIO_InitStructure);

    HCSR_GPIO_IN_APBxClock_FUN(HCSR_GPIO_INCLK, ENABLE); 
    GPIO_InitStructure.GPIO_Pin = HCSR_GPIO_INPIN;
    GPIO_InitStructure.GPIO_Speed = HCSR_GPIO_INSPEED;
    GPIO_InitStructure.GPIO_Mode = HCSR_GPIO_INMODE;
    GPIO_Init(HCSR_GPIO_INPORT,&GPIO_InitStructure);
}

void HCsr_EXTI_Open()
{
    EXTI_InitTypeDef EXTI_InitStructure;
    RCC_APB2PeriphClockCmd(HCSR_EXTI_INCLK,ENABLE);
    /* 配置 NVIC 中断*/
	NVIC_Configuration();

    /* 选择EXTI的信号源 */
    GPIO_EXTILineConfig(HCSR_EXTI_PORTSOURCE, HCSR_EXTI_PINSOURCE); 
    EXTI_InitStructure.EXTI_Line = HCSR_EXTI_LINE;
	
	/* EXTI为中断模式 */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 上升沿中断 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    /* 使能中断 */	
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}

void HCsr_EXTI_Close()
{
    EXTI_InitTypeDef EXTI_InitStructure;
    RCC_APB2PeriphClockCmd(HCSR_EXTI_INCLK,ENABLE);
    /* 配置 NVIC 中断*/
	NVIC_Configuration();

    /* 选择EXTI的信号源 */
    GPIO_EXTILineConfig(HCSR_EXTI_PORTSOURCE, HCSR_EXTI_PINSOURCE); 
    EXTI_InitStructure.EXTI_Line = HCSR_EXTI_LINE;
	
	/* EXTI为中断模式 */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 上升沿中断 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    /* 使能中断 */	
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);
}
