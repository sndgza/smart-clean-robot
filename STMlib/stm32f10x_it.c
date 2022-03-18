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

#include "bsp_oled.h"

extern volatile uint16_t CNT;
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
        TIM_ClearITPendingBit(SPEED_CLK_TIM,TIM_IT_Update);  //清除TIMx更新中断标志 
        
        if(CNT < 10)
        {
          sprintf(speed,"%d  \0",CNT);
          OLED_ShowString(50,0,speed);
        }
        else if(CNT < 100)
        {
          sprintf(speed,"%d \0",CNT);
          OLED_ShowString(50,0,speed);
        }
        else 
        {
          sprintf(speed,"%d\0",CNT);
          OLED_ShowString(50,0,speed);
        }

        CNT = 0;
        TIM_SetCounter(TIM2,0);
    }
}


void TIM2_IRQHandler()
{
  // 当要被捕获的信号的周期大于定时器的最长定时时，定时器就会溢出，产生更新中断
	// 这个时候我们需要把这个最长的定时周期加到捕获信号的时间里面去
	if ( TIM_GetITStatus ( CAP_TIM, TIM_IT_Update) != RESET )               
	{	
		//TIM_ICUserValueStructure.Capture_Period ++;		
		TIM_ClearITPendingBit ( CAP_TIM, TIM_FLAG_Update ); 		
	}
  // 上升沿捕获中断
	if ( TIM_GetITStatus (CAP_TIM, CAP_TIM_IT_CCx ) != RESET)
	{
    CNT ++;
    TIM_ClearITPendingBit (CAP_TIM, CAP_TIM_IT_CCx );
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
