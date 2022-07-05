/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  sndgza
  * @version V1.0
  * @date    2022-3-17
  * @brief   MPU6050驱动
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */


#include "mpu6050.h"
#include "bsp_usart.h"
#include "bsp_i2c.h"



/**
  * @brief   写数据到MPU6050寄存器
  * @param   
  * @retval  
  */
void MPU6050_WriteReg(u8 reg_add,u8 reg_dat)
{
	i2c_Start();
	i2c_SendByte(MPU6050_SLAVE_ADDRESS);
	i2c_WaitAck();
	i2c_SendByte(reg_add);
	i2c_WaitAck();
	i2c_SendByte(reg_dat);
	i2c_WaitAck();
	i2c_Stop();
}


/**
  * @brief   从MPU6050寄存器读取数据
  * @param   
  * @retval  
  */
void MPU6050_ReadData(u8 reg_add,unsigned char*Read,u8 num)
{
	unsigned char i;
	
	i2c_Start();
	i2c_SendByte(MPU6050_SLAVE_ADDRESS);
	i2c_WaitAck();
	i2c_SendByte(reg_add);
	i2c_WaitAck();
	
	i2c_Start();
	i2c_SendByte(MPU6050_SLAVE_ADDRESS+1);
	i2c_WaitAck();
	
	for(i=0;i<(num-1);i++){
		*Read=i2c_ReadByte(1);
		Read++;
	}
	*Read=i2c_ReadByte(0);
	i2c_Stop();
}

/**
  * @brief   初始化MPU6050芯片
  * @param   
  * @retval  
  */
u8 MPU6050_Init(void)
{
  int i=0,j=0;
  u8 res;
  //在初始化之前要延时一段时间，若没有延时，则断电后再上电数据可能会出错
  MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x80);	    //解除休眠状态
  for(i=0;i<1000;i++)
  {
    for(j=0;j<1000;j++)
    {
      ;
    }
  }
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	    //解除休眠状态
  MPU_Set_Gyro_Fsr(3);					//陀螺仪传感器,±2000dps
	MPU_Set_Accel_Fsr(0);					//加速度传感器,±2g
  MPU_Set_Rate(50);						//设置采样率50Hz
	MPU6050_WriteReg(MPU6050_RA_INT_ENABLE , 0x00);	    //关闭所有中断
	MPU6050_WriteReg(MPU6050_RA_USER_CTRL , 0X00);	//I2C主模式关闭
	MPU6050_WriteReg(MPU6050_RA_FIFO_EN , 0x00);	  //关闭FIFO
	MPU6050_WriteReg(MPU6050_RA_INT_PIN_CFG ,0X80);	//INT引脚低电平有效
  res=MPU_ReadByte(MPU6050_WHO_AM_I); 
  if(res==MPU6050_ADDRESS_AD0_LOW)//器件ID正确
	{
		MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1,0X01);	//设置CLKSEL,PLL X轴为参考
		MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_2,0X00);	//加速度与陀螺仪都工作
		MPU_Set_Rate(50);						//设置采样率为50Hz
 	}else return 1;
	return 0;
}

/**
  * @brief   读取MPU6050的ID
  * @param   
  * @retval  
  */
uint8_t MPU6050ReadID(void)
{
	unsigned char Re = 0;
    MPU6050_ReadData(MPU6050_RA_WHO_AM_I,&Re,1);    //读器件地址
	if(Re != 0x68)
	{
		USART3_printf("MPU6050 dectected error!\r\n检测不到MPU6050模块，请检查模块与开发板的接线");
		return 0;
	}
	else
	{
		USART3_printf("MPU6050 ID = %d\r\n",Re);
		return 1;
	}
		
}
/**
  * @brief   读取MPU6050的加速度数据
  * @param   
  * @retval  
  */
void MPU6050ReadAcc(short *accData)
{
    u8 buf[6];
    MPU6050_ReadData(MPU6050_ACC_OUT, buf, 6);
    accData[0] = (buf[0] << 8) | buf[1];
    accData[1] = (buf[2] << 8) | buf[3];
    accData[2] = (buf[4] << 8) | buf[5];
}

/**
  * @brief   读取MPU6050的角加速度数据
  * @param   
  * @retval  
  */
void MPU6050ReadGyro(short *gyroData)
{
    u8 buf[6];
    MPU6050_ReadData(MPU6050_GYRO_OUT,buf,6);
    gyroData[0] = (buf[0] << 8) | buf[1];
    gyroData[1] = (buf[2] << 8) | buf[3];
    gyroData[2] = (buf[4] << 8) | buf[5];
}


/**
  * @brief   读取MPU6050的原始温度数据
  * @param   
  * @retval  
  */
void MPU6050ReadTemp(short *tempData)
{
	u8 buf[2];
    MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //读取温度值
    *tempData = (buf[0] << 8) | buf[1];
}


/**
  * @brief   读取MPU6050的温度数据，转化成摄氏度
  * @param   
  * @retval  
  */
void MPU6050_ReturnTemp(float *Temperature)
{
	short temp3;
	u8 buf[2];
	
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //读取温度值
  temp3= (buf[0] << 8) | buf[1];	
	*Temperature=((double) temp3/340.0)+36.53;

}


//IIC读一个字节 
//reg:寄存器地址 
//返回值:读到的数据
u8 MPU_ReadByte(u8 reg)
{
	u8 res;
  i2c_Start(); 
	i2c_SendByte(MPU6050_SLAVE_ADDRESS|0);//发送器件地址+写命令	
	i2c_WaitAck();		//等待应答 
  i2c_SendByte(reg);	//写寄存器地址
  i2c_WaitAck();		//等待应答
  i2c_Start();
	i2c_SendByte(MPU6050_SLAVE_ADDRESS|1);//发送器件地址+读命令	
  i2c_WaitAck();		//等待应答 
	res=i2c_ReadByte(0);//读取数据,发送nACK 
  i2c_Stop();			//产生一个停止条件 
	return res;		
}

//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU6050_RA_GYRO_CONFIG,fsr<<3);//设置陀螺仪满量程范围  
}
//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU6050_RA_ACCEL_CONFIG,fsr<<3);//设置加速度传感器满量程范围  
}
//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU6050_RA_SMPLRT_DIV,data);	//设置数字低通滤波器
 	return MPU_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}


//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败 
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU6050_RA_CONFIG ,data);//设置数字低通滤波器  
}




u8 MPU_Write_Byte(u8 reg,u8 data) 				 
{ 
  i2c_Start(); 
	i2c_SendByte(MPU6050_SLAVE_ADDRESS|0);//发送器件地址+写命令	
	if(i2c_WaitAck())	//等待应答
	{
		i2c_Stop();		 
		return 1;		
	}
  i2c_SendByte(reg);	//写寄存器地址
  i2c_WaitAck();		//等待应答 
	i2c_SendByte(data);//发送数据
	if(i2c_WaitAck())	//等待ACK
	{
		i2c_Stop();	 
		return 1;		 
	}		 
  i2c_Stop();	 
	return 0;
}

