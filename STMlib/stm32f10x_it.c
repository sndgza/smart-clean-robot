/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_SysTick.h"
#include "bsp_speedtest.h"
#include <stdio.h>
#include <string.h> 
#include "bsp_Bluetooth.h"
#include "bsp_oled.h"
#include "bsp_hcsr.h"
#include "Path_plan.h"

extern int time_count;
extern uint16_t hcsr_time;
extern float hcsr_distance;
extern int distance_flag;
extern int onestep_flag;

extern int PID_flag;

extern volatile uint16_t CNT1;
extern volatile uint16_t CNT;
extern s32 leftSpeedNow;
extern s32 rightSpeedNow;



int speed_left_test;
int speed_right_test;

char    speed[5];
//char char_buffer[5];

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	//unsigned char i;
  TimingDelay_Decrement();
	// for(i=0;i<NumOfTask;i++)
	// {
	// 	if(Task_Delay[i])
	// 	{
	// 		Task_Delay[i]--;
	// 	}
	// }
}



void SPEED_CLK_TIM_IRQHandler(void)
{
    if(TIM_GetITStatus(SPEED_CLK_TIM,TIM_IT_Update)!= RESET)      //检查TIM3更新中断发生与否
    {
        TIM_ClearITPendingBit(SPEED_CLK_TIM,TIM_IT_Update);  //清除TIMx更新中断标志 .ty.h
        time_count++;
        if(time_count >= 10)
        {   
          time_count = 0;
          leftSpeedNow = speed_left_test;
          rightSpeedNow = speed_right_test;
          speed_left_test = 0;
          speed_right_test = 0;
        
          if(Car_Status == Start_run || Car_Status == Start_turnleft || Car_Status == Start_turnright)
          {
            PID_flag = 1;
          }
          if(Car_Status == Start_run)
          {
            //USART3_printf("\r\nleftSpeedNow = %d\r\n",leftSpeedNow);
            //USART3_printf("\r\nrightSpeedNow = %d\r\n",rightSpeedNow);
          }
        }
      } 
      TIM_SetCounter(TIM2,0);
}


void TIM2_IRQHandler()
{
  // 当要被捕获的信号的周期大于定时器的最长定时时，定时器就会溢出，产生更新中断
	// 这个时候我们需要把这个最长的定时周期加到捕获信号的时间里面去
	if ( TIM_GetITStatus ( CAP1_TIM, TIM_IT_Update) != RESET )               
	{	
		//TIM_ICUserValueStructure.Capture_Period ++;		
		TIM_ClearITPendingBit ( CAP1_TIM, TIM_FLAG_Update ); 		
	}
  // 上升沿捕获中断
	if ( TIM_GetITStatus (CAP1_TIM, CAP1_TIM_IT_CCx ) != RESET)
	{
    speed_left_test++;
    CNT ++;
    TIM_ClearITPendingBit (CAP1_TIM, CAP1_TIM_IT_CCx );
  }

  // 上升沿捕获中断
	if ( TIM_GetITStatus (CAP2_TIM, CAP2_TIM_IT_CCx ) != RESET)
	{
    speed_right_test++;
    CNT1++;
    TIM_ClearITPendingBit (CAP2_TIM, CAP2_TIM_IT_CCx );
  }
  if(CNT >= 10 && CNT1 >= 10 && Car_Status == Start_run)
  {
    CNT = 0;
    CNT1 = 0;
    onestep_flag = 1;
  }
}

void DEBUG_USART3_IRQHandler()
{
  uint8_t ucCh;
  if( USART_GetITStatus ( DEBUG_USART3, USART_IT_RXNE ) != RESET )
  {
    ucCh  = USART_ReceiveData( DEBUG_USART3 );
    if( Usart3_RxCounter < (USART_RXBUFF_SIZE-1) )
    {
      Usart3_RxBuff[Usart3_RxCounter++] = ucCh;
    }
  }
  	if ( USART_GetITStatus( DEBUG_USART3, USART_IT_IDLE ) == SET )                                         //数据帧接收完毕
	{
    Usart3_RxCompleted = 1;
		
		ucCh = USART_ReceiveData( DEBUG_USART3 );                                                              //由软件序列清除中断标志位(先读USART_SR，然后读USART_DR)	
		
  }	
}


void HCSR_IRQHandler()
{
	if(EXTI_GetITStatus(HCSR_EXTI_LINE) != RESET) 
	{

    HCsr_EXTI_Close();
    TIM_Cmd(TIM7,DISABLE);
    hcsr_time = TIM_GetCounter(TIM7);
    //USART3_printf("\r\ntime = %d\r\n",hcsr_time);
    hcsr_distance = hcsr_time * 0.017;
    distance_flag = 1;
    //USART3_printf("\r\ndistance = %.2f\r\n",hcsr_distance);
		EXTI_ClearITPendingBit(HCSR_EXTI_LINE); 
    TIM_SetCounter(TIM7,0);
	} 
}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
