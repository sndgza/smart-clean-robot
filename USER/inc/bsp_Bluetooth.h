#ifndef _BSP_BLUETOOTH_H
#define _BSP_BLUETOOTH_H

#include "stm32f10x.h"  
#include "stdio.h" 
#include "Path_plan.h"     
#include "stdarg.h"		
#include "string.h"  

#define  DEBUG_USART3                    USART3
#define  DEBUG_USART3_CLK                RCC_APB1Periph_USART3
#define  DEBUG_USART3_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  DEBUG_USART3_BAUDRATE           9600

// USART GPIO 引脚宏定义
#define  DEBUG_USART3_GPIO_CLK           (RCC_APB2Periph_GPIOB)
#define  DEBUG_USART3_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
   
#define  DEBUG_USART3_TX_GPIO_PORT       GPIOB   
#define  DEBUG_USART3_TX_GPIO_PIN        GPIO_Pin_10
#define  DEBUG_USART3_RX_GPIO_PORT       GPIOB
#define  DEBUG_USART3_RX_GPIO_PIN        GPIO_Pin_11

#define  DEBUG_USART3_IRQ                USART3_IRQn
#define  DEBUG_USART3_IRQHandler         USART3_IRQHandler

#define  USART_TXBUFF_SIZE   256 
#define  USART_RXBUFF_SIZE   256                     //定义串口1 接收缓冲区大小 256字节

enum System_status{
    Wait_CMD,
    Start_run,
    Start_turnleft,
    Start_turnright,
    Start_turnback,
    Start_wait,
    Sleep_CMD
} ;


extern char Usart3_RxCompleted ;                    //外部声明，其他文件可以调用该变量
extern unsigned int Usart3_RxCounter;               //外部声明，其他文件可以调用该变量
extern char Usart3_RxBuff[USART_RXBUFF_SIZE];       //外部声明，其他文件可以调用该变量
extern enum System_status Car_Status;

void USART_Config(void);
void USART3_printf(char* fmt,...);

void Bluetooth_CMD(void);



#endif
