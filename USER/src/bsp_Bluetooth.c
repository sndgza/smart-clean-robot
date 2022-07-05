/*
*******************************************************************************
* @file    bsp_Bluetooth.c
* @author  sndgza
* @version V1.1
* @date    2022-03-20
* @brief   蓝牙驱动
******************************************************************************
* @attention
******************************************************************************
*/ 


#include "bsp_Bluetooth.h"



enum System_status Car_Status;
char Usart3_RxCompleted = 0;            						//定义一个变量 0：表示接收未完成 1：表示接收完成 
unsigned int Usart3_RxCounter = 0;      						//定义一个变量，记录串口1总共接收了多少字节的数据
char Usart3_RxBuff[USART_RXBUFF_SIZE]; 							//定义一个数组，用于保存串口1接收到的数据   	


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
	vsprintf(Usart3_TxBuff,fmt,ap);
	va_end(ap);	
	
	length=strlen((const char*)Usart3_TxBuff);		
	while((DEBUG_USART3->SR&0X40)==0);
	for(i = 0;i < length;i ++)
	{			
		DEBUG_USART3->DR = Usart3_TxBuff[i];
		while((DEBUG_USART3->SR&0X40)==0);	
	}	
}


void USART3_TxData(unsigned char *data)
{
	int	i;	
	while((DEBUG_USART3->SR&0X40)==0);
	for(i = 1;i <= (data[0]*256+data[1]);i ++){			
		DEBUG_USART3->DR = data[i+1];
		while((DEBUG_USART3->SR&0X40)==0);	
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
    NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART3_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    /* 子优先级 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
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
    USART_ITConfig ( DEBUG_USART3, USART_IT_IDLE, ENABLE ); //使能串口总线空闲中断
    // 使能串口
    USART_Cmd(DEBUG_USART3, ENABLE);
}

void Bluetooth_CMD(void)
{
  int CMD_lenth;
  char Bluetooch_buff[20];

  CMD_lenth = 0;
  

  while ( Usart3_RxCounter-- )
  {
    Bluetooch_buff [CMD_lenth] = Usart3_RxBuff[CMD_lenth];
    CMD_lenth++; 
  }

  Usart3_RxCounter = 0;

  if( strstr (Bluetooch_buff,"move"))
  {
    Car_Status = Start_run;
  }
  if(strstr(Bluetooch_buff,"stop"))
  {
    Car_Status = Start_wait;
  }
  if(strstr(Bluetooch_buff,"sleep"))
  {
    Car_Status = Sleep_CMD;
  }
  if(strstr(Bluetooch_buff,"left"))
  {
    // turn_dirction += 3;
    // turn_dirction %= 4;
    Car_Status = Start_turnleft;
  }
  if(strstr(Bluetooch_buff,"right"))
  {
    // turn_dirction ++;
    // turn_dirction %= 4;
    Car_Status = Start_turnright;
  }
  if(strstr(Bluetooch_buff,"back"))
  {
    Car_Status = Start_turnback;
  }
  Usart3_RxCompleted = 0;
}


