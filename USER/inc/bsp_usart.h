#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"  
#include "stdio.h"      
#include "stdarg.h"		
#include "string.h"     


/** 
  * 串口宏定义，不同的串口挂载的总线和IO不一样，移植时需要修改这几个宏
  * 1-修改总线时钟的宏，uart1挂载到apb2总线，其他uart挂载到apb1总线
  * 2-修改GPIO的宏
  */

#define  _USART1                          1
#define  _USART2                          0
#define  _USART3                          0
#define  _USART4                          0
#define  _USART5                          0

//是否开启接收功能  1：开启  0：关闭
#define  USART1_RX_ENABLE     1                                  
//定义串口1 发送缓冲区大小 256字节
#define  USART_TXBUFF_SIZE   256  

#define  DEBUG_USART1                    USART1
#define  DEBUG_USART2                    USART2
#define  DEBUG_USART3                    USART3
#define  DEBUG_USART4                    UART4
#define  DEBUG_USART5                    UART5

// 串口1-USART1
#if      _USART1

#define  USART_1                         1
#define  DEBUG_USART1_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART1_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  DEBUG_USART1_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  DEBUG_USART1_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART1_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART1_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART1_TX_GPIO_PIN        GPIO_Pin_9
#define  DEBUG_USART1_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART1_RX_GPIO_PIN        GPIO_Pin_10

#define  DEBUG_USART1_IRQ                USART1_IRQn
#define  DEBUG_USART1_IRQHandler         USART1_IRQHandler

#endif

//串口2-USART2
#if    _USART2

#define  USART_2                         1
#define  DEBUG_USART2_CLK                RCC_APB1Periph_USART2
#define  DEBUG_USART2_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  DEBUG_USART2_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  DEBUG_USART2_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART2_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
   
#define  DEBUG_USART2_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART2_TX_GPIO_PIN        GPIO_Pin_2
#define  DEBUG_USART2_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART2_RX_GPIO_PIN        GPIO_Pin_3

#define  DEBUG_USART2_IRQ                USART2_IRQn
#define  DEBUG_USART2_IRQHandler         USART2_IRQHandler


#endif

//串口3-USART3
#if    _USART3

#define  USART_3                         1
#define  DEBUG_USART3_CLK                RCC_APB1Periph_USART3
#define  DEBUG_USART3_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  DEBUG_USART3_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  DEBUG_USART3_GPIO_CLK           (RCC_APB2Periph_GPIOB)
#define  DEBUG_USART3_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
   
#define  DEBUG_USART3_TX_GPIO_PORT       GPIOB   
#define  DEBUG_USART3_TX_GPIO_PIN        GPIO_Pin_10
#define  DEBUG_USART3_RX_GPIO_PORT       GPIOB
#define  DEBUG_USART3_RX_GPIO_PIN        GPIO_Pin_11

#define  DEBUG_USART3_IRQ                USART3_IRQn
#define  DEBUG_USART3_IRQHandler         USART3_IRQHandler

#endif

//串口4-UART4
#if    _USART4

#define  USART_4                         1
#define  DEBUG_USART4_CLK                RCC_APB1Periph_UART4
#define  DEBUG_USART4_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  DEBUG_USART4_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  DEBUG_USART4_GPIO_CLK           (RCC_APB2Periph_GPIOC)
#define  DEBUG_USART4_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
   
#define  DEBUG_USART4_TX_GPIO_PORT       GPIOC   
#define  DEBUG_USART4_TX_GPIO_PIN        GPIO_Pin_10
#define  DEBUG_USART4_RX_GPIO_PORT       GPIOC
#define  DEBUG_USART4_RX_GPIO_PIN        GPIO_Pin_11

#define  DEBUG_USART4_IRQ                UART4_IRQn
#define  DEBUG_USART4_IRQHandler         UART4_IRQHandler

#endif

//串口5-UART5
#if    _USART5

#define  USART_5                         1
#define  DEBUG_USART5_CLK                RCC_APB1Periph_UART5
#define  DEBUG_USART5_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  DEBUG_USART5_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  DEBUG_USART5_GPIO_CLK           (RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD)
#define  DEBUG_USART5_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
   
#define  DEBUG_USART5_TX_GPIO_PORT       GPIOC   
#define  DEBUG_USART5_TX_GPIO_PIN        GPIO_Pin_12
#define  DEBUG_USART5_RX_GPIO_PORT       GPIOD
#define  DEBUG_USART5_RX_GPIO_PIN        GPIO_Pin_2

#define  DEBUG_USART5_IRQ                UART5_IRQn
#define  DEBUG_USART5_IRQHandler         UART5_IRQHandler


#endif




#define USART_RXBUFF_SIZE   256                     //定义串口1 接收缓冲区大小 256字节
extern char Usart1_RxCompleted ;                    //外部声明，其他文件可以调用该变量
extern unsigned int Usart1_RxCounter;               //外部声明，其他文件可以调用该变量
extern char Usart1_RxBuff[USART_RXBUFF_SIZE];       //外部声明，其他文件可以调用该变量

void USART1_printf(char*,...);                      //串口1 printf函数
void USART2_printf(char*,...);                      //串口2 printf函数
void USART3_printf(char*,...);                      //串口3 printf函数
void USART4_printf(char*,...);                      //串口4 printf函数
void USART5_printf(char*,...);                      //串口5 printf函数

void USART2_TxData(unsigned char *data);

void USART_Config(void);
#endif
