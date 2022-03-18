/*
*******************************************************************************
* @file    bsp_usart.c
* @author  sndgza
* @version V1.0
* @date    2022-03-17
* @brief   串口控制
******************************************************************************
* @attention
******************************************************************************
*/ 

#include "bsp_usart.h"     										//包含需要的头文件


char Usart1_RxCompleted = 0;            						//定义一个变量 0：表示接收未完成 1：表示接收完成 
unsigned int Usart1_RxCounter = 0;      						//定义一个变量，记录串口1总共接收了多少字节的数据
char Usart1_RxBuff[USART_RXBUFF_SIZE]; 							//定义一个数组，用于保存串口1接收到的数据   	


__align(8) char Usart1_TxBuff[USART_TXBUFF_SIZE];  
/**
 *@brief   串口1 printf函数                         
 *@param   * fmt,...  格式化输出字符串和参数    
 *@retval  无                                       
 */
void USART1_printf(char* fmt,...) 
{  
	unsigned int i,length;
	va_list ap;
	va_start(ap,fmt);
	vsprintf(Usart1_TxBuff,fmt,ap);
	va_end(ap);	
	
	length=strlen((const char*)Usart1_TxBuff);		
	while((DEBUG_USART1->SR&0X40)==0);
	for(i = 0;i < length;i ++)
	{			
		DEBUG_USART1->DR = Usart1_TxBuff[i];
		while((DEBUG_USART1->SR&0X40)==0);	
	}	
}


__align(8) char Usart2_TxBuff[USART_TXBUFF_SIZE];  
/**
 *@brief   串口2 printf函数                         
 *@param   * fmt,...  格式化输出字符串和参数    
 *@retval  无                                       
 */
void USART2_printf(char* fmt,...) 
{  
	unsigned int i,length;
	va_list ap;
	va_start(ap,fmt);
	vsprintf(Usart1_TxBuff,fmt,ap);
	va_end(ap);	
	
	length=strlen((const char*)Usart1_TxBuff);		
	while((DEBUG_USART2->SR&0X40)==0);
	for(i = 0;i < length;i ++)
	{			
		DEBUG_USART2->DR = Usart1_TxBuff[i];
		while((DEBUG_USART2->SR&0X40)==0);	
	}	
}

__align(8) char Usart3_TxBuff[USART_TXBUFF_SIZE];  
/**
 *@brief   串口3 printf函数                         
 *@param   * fmt,...  格式化输出字符串和参数    
 *@retval  无                                       
 */
void USART3_printf(char* fmt,...) 
{  
	unsigned int i,length;
	va_list ap;
	va_start(ap,fmt);
	vsprintf(Usart1_TxBuff,fmt,ap);
	va_end(ap);	
	
	length=strlen((const char*)Usart1_TxBuff);		
	while((DEBUG_USART3->SR&0X40)==0);
	for(i = 0;i < length;i ++)
	{			
		DEBUG_USART3->DR = Usart1_TxBuff[i];
		while((DEBUG_USART3->SR&0X40)==0);	
	}	
}



void USART2_TxData(unsigned char *data)
{
	int	i;	
	while((DEBUG_USART3->SR&0X40)==0);
	for(i = 1;i <= (data[0]*256+data[1]);i ++){			
		DEBUG_USART3->DR = data[i+1];
		while((DEBUG_USART3->SR&0X40)==0);	
	}
}


__align(8) char Usart4_TxBuff[USART_TXBUFF_SIZE];  
/**
 *@brief   串口4 printf函数                         
 *@param   * fmt,...  格式化输出字符串和参数    
 *@retval  无                                       
 */
void USART4_printf(char* fmt,...) 
{  
	unsigned int i,length;
	va_list ap;
	va_start(ap,fmt);
	vsprintf(Usart1_TxBuff,fmt,ap);
	va_end(ap);	
	
	length=strlen((const char*)Usart1_TxBuff);		
	while((DEBUG_USART4->SR&0X40)==0);
	for(i = 0;i < length;i ++)
	{			
		DEBUG_USART4->DR = Usart1_TxBuff[i];
		while((DEBUG_USART4->SR&0X40)==0);	
	}	
}


__align(8) char Usart5_TxBuff[USART_TXBUFF_SIZE];  
/**
 *@brief   串口5 printf函数                         
 *@param   * fmt,...  格式化输出字符串和参数    
 *@retval  无                                       
 */
void USART5_printf(char* fmt,...) 
{  
	unsigned int i,length;
	va_list ap;
	va_start(ap,fmt);
	vsprintf(Usart1_TxBuff,fmt,ap);
	va_end(ap);	
	
	length=strlen((const char*)Usart1_TxBuff);		
	while((DEBUG_USART5->SR&0X40)==0);
	for(i = 0;i < length;i ++)
	{			
		DEBUG_USART5->DR = Usart1_TxBuff[i];
		while((DEBUG_USART5->SR&0X40)==0);	
	}	
}


 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /* 配置USART为中断源 */

  #ifdef USART_1
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART1_IRQ;
  #endif

  #ifdef USART_2
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART2_IRQ;
  #endif
    
  #ifdef USART_3
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART3_IRQ;
  #endif
    
  #ifdef USART_4
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART4_IRQ;
  #endif
    
  #ifdef USART_5
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART5_IRQ;
  #endif

  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  USART GPIO 配置,工作参数配置
  * @param  无
  * @retval 无
  */
void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//串口1使能
	#ifdef USART_1
	{
		// 打开串口GPIO的时钟
		DEBUG_USART1_GPIO_APBxClkCmd(DEBUG_USART1_GPIO_CLK, ENABLE);
		
		// 打开串口外设的时钟
		DEBUG_USART1_APBxClkCmd(DEBUG_USART1_CLK, ENABLE);

		// 将USART Tx的GPIO配置为推挽复用模式
		GPIO_InitStructure.GPIO_Pin = DEBUG_USART1_TX_GPIO_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(DEBUG_USART1_TX_GPIO_PORT, &GPIO_InitStructure);

		// 将USART Rx的GPIO配置为浮空输入模式
		GPIO_InitStructure.GPIO_Pin = DEBUG_USART1_RX_GPIO_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(DEBUG_USART1_RX_GPIO_PORT, &GPIO_InitStructure);
		
		// 配置串口的工作参数
		// 配置波特率
		USART_InitStructure.USART_BaudRate = DEBUG_USART1_BAUDRATE;
		// 配置 针数据字长
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		// 配置停止位
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		// 配置校验位
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		// 配置硬件流控制
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		// 配置工作模式，收发一起
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		// 完成串口的初始化配置
		USART_Init(DEBUG_USART1, &USART_InitStructure);
		
		// 串口中断优先级配置
		NVIC_Configuration();
		
		// 使能串口接收中断
		USART_ITConfig(DEBUG_USART1, USART_IT_RXNE, ENABLE);	
		
		// 使能串口
		USART_Cmd(DEBUG_USART1, ENABLE);
	}
	#endif
	//串口2使能
	#ifdef USART_2
	{
		// 打开串口GPIO的时钟
		DEBUG_USART2_GPIO_APBxClkCmd(DEBUG_USART2_GPIO_CLK, ENABLE);
		
		// 打开串口外设的时钟
		DEBUG_USART2_APBxClkCmd(DEBUG_USART2_CLK, ENABLE);

		// 将USART Tx的GPIO配置为推挽复用模式
		GPIO_InitStructure.GPIO_Pin = DEBUG_USART2_TX_GPIO_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(DEBUG_USART2_TX_GPIO_PORT, &GPIO_InitStructure);

		// 将USART Rx的GPIO配置为浮空输入模式
		GPIO_InitStructure.GPIO_Pin = DEBUG_USART2_RX_GPIO_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(DEBUG_USART2_RX_GPIO_PORT, &GPIO_InitStructure);
		
		// 配置串口的工作参数
		// 配置波特率
		USART_InitStructure.USART_BaudRate = DEBUG_USART2_BAUDRATE;
		// 配置 针数据字长
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		// 配置停止位
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		// 配置校验位
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		// 配置硬件流控制
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		// 配置工作模式，收发一起
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		// 完成串口的初始化配置
		USART_Init(DEBUG_USART2, &USART_InitStructure);
		
		// 串口中断优先级配置
		NVIC_Configuration();
		
		// 使能串口接收中断
		USART_ITConfig(DEBUG_USART2, USART_IT_RXNE, ENABLE);	
		
		// 使能串口
		USART_Cmd(DEBUG_USART2, ENABLE);
	}
	#endif
	//使能串口3
	#ifdef USART_3
	{
		// 打开串口GPIO的时钟
		DEBUG_USART3_GPIO_APBxClkCmd(DEBUG_USART3_GPIO_CLK, ENABLE);
		
		// 打开串口外设的时钟
		DEBUG_USART3_APBxClkCmd(DEBUG_USART3_CLK, ENABLE);

		// 将USART Tx的GPIO配置为推挽复用模式
		GPIO_InitStructure.GPIO_Pin = DEBUG_USART3_TX_GPIO_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(DEBUG_USART3_TX_GPIO_PORT, &GPIO_InitStructure);

		// 将USART Rx的GPIO配置为浮空输入模式
		GPIO_InitStructure.GPIO_Pin = DEBUG_USART3_RX_GPIO_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(DEBUG_USART3_RX_GPIO_PORT, &GPIO_InitStructure);
		
		// 配置串口的工作参数
		// 配置波特率
		USART_InitStructure.USART_BaudRate = DEBUG_USART3_BAUDRATE;
		// 配置 针数据字长
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		// 配置停止位
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		// 配置校验位
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		// 配置硬件流控制
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		// 配置工作模式，收发一起
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		// 完成串口的初始化配置
		USART_Init(DEBUG_USART3, &USART_InitStructure);
		
		// 串口中断优先级配置
		NVIC_Configuration();
		
		// 使能串口接收中断
		USART_ITConfig(DEBUG_USART3, USART_IT_RXNE, ENABLE);	
		
		// 使能串口
		USART_Cmd(DEBUG_USART3, ENABLE);
	}
	#endif
	//使能串口4
	#ifdef USART_4
	{
		// 打开串口GPIO的时钟
		DEBUG_USART4_GPIO_APBxClkCmd(DEBUG_USART4_GPIO_CLK, ENABLE);
		
		// 打开串口外设的时钟
		DEBUG_USART4_APBxClkCmd(DEBUG_USART4_CLK, ENABLE);

		// 将USART Tx的GPIO配置为推挽复用模式
		GPIO_InitStructure.GPIO_Pin = DEBUG_USART4_TX_GPIO_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(DEBUG_USART4_TX_GPIO_PORT, &GPIO_InitStructure);

	// 将USART Rx的GPIO配置为浮空输入模式
		GPIO_InitStructure.GPIO_Pin = DEBUG_USART4_RX_GPIO_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(DEBUG_USART4_RX_GPIO_PORT, &GPIO_InitStructure);
		
		// 配置串口的工作参数
		// 配置波特率
		USART_InitStructure.USART_BaudRate = DEBUG_USART4_BAUDRATE;
		// 配置 针数据字长
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		// 配置停止位
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		// 配置校验位
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		// 配置硬件流控制
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		// 配置工作模式，收发一起
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		// 完成串口的初始化配置
		USART_Init(DEBUG_USART4, &USART_InitStructure);
		
		// 串口中断优先级配置
		NVIC_Configuration();
		
		// 使能串口接收中断
		USART_ITConfig(DEBUG_USART4, USART_IT_RXNE, ENABLE);	
		
		// 使能串口
		USART_Cmd(DEBUG_USART4, ENABLE);

	}
	#endif

	//使能串口5
	#ifdef USART_5
	{
		// 打开串口GPIO的时钟
		DEBUG_USART5_GPIO_APBxClkCmd(DEBUG_USART5_GPIO_CLK, ENABLE);
		
		// 打开串口外设的时钟
		DEBUG_USART5_APBxClkCmd(DEBUG_USART5_CLK, ENABLE);

		// 将USART Tx的GPIO配置为推挽复用模式
		GPIO_InitStructure.GPIO_Pin = DEBUG_USART5_TX_GPIO_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(DEBUG_USART5_TX_GPIO_PORT, &GPIO_InitStructure);

	// 将USART Rx的GPIO配置为浮空输入模式
		GPIO_InitStructure.GPIO_Pin = DEBUG_USART5_RX_GPIO_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(DEBUG_USART5_RX_GPIO_PORT, &GPIO_InitStructure);
		
		// 配置串口的工作参数
		// 配置波特率
		USART_InitStructure.USART_BaudRate = DEBUG_USART5_BAUDRATE;
		// 配置 针数据字长
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		// 配置停止位
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		// 配置校验位
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		// 配置硬件流控制
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		// 配置工作模式，收发一起
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		// 完成串口的初始化配置
		USART_Init(DEBUG_USART5, &USART_InitStructure);
		
		// 串口中断优先级配置
		NVIC_Configuration();
		
		// 使能串口接收中断
		USART_ITConfig(DEBUG_USART5, USART_IT_RXNE, ENABLE);	
		
		// 使能串口
		USART_Cmd(DEBUG_USART5, ENABLE);
	}
	#endif 
}


