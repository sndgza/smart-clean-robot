/**
  ******************************************************************************
  * @file    bsp_speedtest.c
  * @author  sndgza
  * @version V1.3
  * @date    2022-03-18
  * @brief   2路光电测速模块的驱动
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */


#include "bsp_speedtest.h"



static void GENERAL_TIM_NVIC_Config()
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为2
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
	// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn ;	
	// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	// 设置抢占优先级为2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// 中断优先级配置
static void BASIC_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为2
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
	// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = SPEED_CLK_TIM_IRQ ;	
	// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	// 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
void speed_GPIO_Config()
{
    GPIO_InitTypeDef    GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(CAP1_PORT_CLC, ENABLE);   //使能GPIOA时钟
    GPIO_InitStructure.GPIO_Pin  = CAP1_PIN  | CAP2_PIN;              //PA2 清除之前设置  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;           //PA0 输入  
    GPIO_Init(CAP1_PORT, &GPIO_InitStructure);
    //GPIO_ResetBits(CAP_PORT,CAP_PIN);                       //PA0 下拉
    

}

void speed_CAPTIM_init()
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;


    GENERAL_TIM_NVIC_Config();
    RCC_APB1PeriphClockCmd(CAP1_TIM_CLC, ENABLE);    //使能TIM2时钟
    //初始化定时器2 时基部分   
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;                      //设定计数器自动重装值 
    TIM_TimeBaseStructure.TIM_Prescaler =0;                         //预分频器   
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //TIM向上计数模式
    TIM_TimeBaseInit(CAP1_TIM, &TIM_TimeBaseStructure);                 //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    // 配置输入捕获的通道，需要根据具体的GPIO来配置
	TIM_ICInitStructure.TIM_Channel = CAP1_TIM_Channelx ;
    // 输入捕获信号的极性配置
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    // 输入通道和捕获通道的映射关系，有直连和非直连两种
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// 输入的需要被捕获的信号的分频系数
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// 输入的需要被捕获的信号的滤波系数
	TIM_ICInitStructure.TIM_ICFilter = 0;
	// 定时器输入捕获初始化
	TIM_ICInit(CAP1_TIM, &TIM_ICInitStructure);

    // 配置输入捕获的通道，需要根据具体的GPIO来配置
	TIM_ICInitStructure.TIM_Channel = CAP2_TIM_Channelx  ;
    // 输入捕获信号的极性配置
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    // 输入通道和捕获通道的映射关系，有直连和非直连两种
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// 输入的需要被捕获的信号的分频系数
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// 输入的需要被捕获的信号的滤波系数
	TIM_ICInitStructure.TIM_ICFilter = 0;
	// 定时器输入捕获初始化
	TIM_ICInit(CAP2_TIM, &TIM_ICInitStructure);

    
    // 清除更新和捕获中断标志位
    TIM_ClearFlag(CAP1_TIM, TIM_FLAG_Update | CAP1_TIM_IT_CCx | CAP2_TIM_IT_CCx);
    // 开启更新和捕获中断  
	TIM_ITConfig (CAP2_TIM, TIM_IT_Update | CAP1_TIM_IT_CCx | CAP2_TIM_IT_CCx, ENABLE );
    
    TIM_Cmd(CAP1_TIM,ENABLE );                                          //使能定时器2
} 

void speed_CLKTIM_init(uint16_t arr,uint16_t prc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
	// 开启定时器时钟,即内部时钟CK_INT=72M
    SPEED_CLK_TIM_APBxClock_FUN(SPEED_CLK_TIM_CLK, ENABLE);
	
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period = arr;	

	// 时钟预分频数为
    TIM_TimeBaseStructure.TIM_Prescaler= prc;
	
	// 时钟分频因子 ，基本定时器没有，不用管
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
	// 计数器计数模式，基本定时器只能向上计数，没有计数模式的设置
    //TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
	// 重复计数器的值，基本定时器没有，不用管
	//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	// 初始化定时器
    TIM_TimeBaseInit(SPEED_CLK_TIM, &TIM_TimeBaseStructure);
		
	// 清除计数器中断标志位
    TIM_ClearFlag(SPEED_CLK_TIM, TIM_FLAG_Update);
	  
	// 开启计数器中断
    TIM_ITConfig(SPEED_CLK_TIM,TIM_IT_Update,ENABLE);
		
	// 使能计数器
    TIM_Cmd(SPEED_CLK_TIM, ENABLE);
    BASIC_TIM_NVIC_Config();	
}
void speedtest_init(uint16_t arr, uint16_t prc)
{
    speed_GPIO_Config();
    speed_CAPTIM_init();
    speed_CLKTIM_init(arr,prc);
}

