/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  sndgza
  * @version V1.0
  * @date    2022-3-17
  * @brief   MPU6050����
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */


#include "mpu6050.h"
#include "bsp_usart.h"
#include "bsp_i2c.h"



/**
  * @brief   д���ݵ�MPU6050�Ĵ���
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
  * @brief   ��MPU6050�Ĵ�����ȡ����
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
  * @brief   ��ʼ��MPU6050оƬ
  * @param   
  * @retval  
  */
u8 MPU6050_Init(void)
{
  int i=0,j=0;
  u8 res;
  //�ڳ�ʼ��֮ǰҪ��ʱһ��ʱ�䣬��û����ʱ����ϵ�����ϵ����ݿ��ܻ����
  MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x80);	    //�������״̬
  for(i=0;i<1000;i++)
  {
    for(j=0;j<1000;j++)
    {
      ;
    }
  }
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	    //�������״̬
  MPU_Set_Gyro_Fsr(3);					//�����Ǵ�����,��2000dps
	MPU_Set_Accel_Fsr(0);					//���ٶȴ�����,��2g
  MPU_Set_Rate(50);						//���ò�����50Hz
	MPU6050_WriteReg(MPU6050_RA_INT_ENABLE , 0x00);	    //�ر������ж�
	MPU6050_WriteReg(MPU6050_RA_USER_CTRL , 0X00);	//I2C��ģʽ�ر�
	MPU6050_WriteReg(MPU6050_RA_FIFO_EN , 0x00);	  //�ر�FIFO
	MPU6050_WriteReg(MPU6050_RA_INT_PIN_CFG ,0X80);	//INT���ŵ͵�ƽ��Ч
  res=MPU_ReadByte(MPU6050_WHO_AM_I); 
  if(res==MPU6050_ADDRESS_AD0_LOW)//����ID��ȷ
	{
		MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1,0X01);	//����CLKSEL,PLL X��Ϊ�ο�
		MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_2,0X00);	//���ٶ��������Ƕ�����
		MPU_Set_Rate(50);						//���ò�����Ϊ50Hz
 	}else return 1;
	return 0;
}

/**
  * @brief   ��ȡMPU6050��ID
  * @param   
  * @retval  
  */
uint8_t MPU6050ReadID(void)
{
	unsigned char Re = 0;
    MPU6050_ReadData(MPU6050_RA_WHO_AM_I,&Re,1);    //��������ַ
	if(Re != 0x68)
	{
		USART3_printf("MPU6050 dectected error!\r\n��ⲻ��MPU6050ģ�飬����ģ���뿪����Ľ���");
		return 0;
	}
	else
	{
		USART3_printf("MPU6050 ID = %d\r\n",Re);
		return 1;
	}
		
}
/**
  * @brief   ��ȡMPU6050�ļ��ٶ�����
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
  * @brief   ��ȡMPU6050�ĽǼ��ٶ�����
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
  * @brief   ��ȡMPU6050��ԭʼ�¶�����
  * @param   
  * @retval  
  */
void MPU6050ReadTemp(short *tempData)
{
	u8 buf[2];
    MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //��ȡ�¶�ֵ
    *tempData = (buf[0] << 8) | buf[1];
}


/**
  * @brief   ��ȡMPU6050���¶����ݣ�ת�������϶�
  * @param   
  * @retval  
  */
void MPU6050_ReturnTemp(float *Temperature)
{
	short temp3;
	u8 buf[2];
	
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //��ȡ�¶�ֵ
  temp3= (buf[0] << 8) | buf[1];	
	*Temperature=((double) temp3/340.0)+36.53;

}


//IIC��һ���ֽ� 
//reg:�Ĵ�����ַ 
//����ֵ:����������
u8 MPU_ReadByte(u8 reg)
{
	u8 res;
  i2c_Start(); 
	i2c_SendByte(MPU6050_SLAVE_ADDRESS|0);//����������ַ+д����	
	i2c_WaitAck();		//�ȴ�Ӧ�� 
  i2c_SendByte(reg);	//д�Ĵ�����ַ
  i2c_WaitAck();		//�ȴ�Ӧ��
  i2c_Start();
	i2c_SendByte(MPU6050_SLAVE_ADDRESS|1);//����������ַ+������	
  i2c_WaitAck();		//�ȴ�Ӧ�� 
	res=i2c_ReadByte(0);//��ȡ����,����nACK 
  i2c_Stop();			//����һ��ֹͣ���� 
	return res;		
}

//����MPU6050�����Ǵ����������̷�Χ
//fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU6050_RA_GYRO_CONFIG,fsr<<3);//���������������̷�Χ  
}
//����MPU6050���ٶȴ����������̷�Χ
//fsr:0,��2g;1,��4g;2,��8g;3,��16g
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU6050_RA_ACCEL_CONFIG,fsr<<3);//���ü��ٶȴ����������̷�Χ  
}
//����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
//rate:4~1000(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU6050_RA_SMPLRT_DIV,data);	//�������ֵ�ͨ�˲���
 	return MPU_Set_LPF(rate/2);	//�Զ�����LPFΪ�����ʵ�һ��
}


//����MPU6050�����ֵ�ͨ�˲���
//lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ:0,���óɹ�
//    ����,����ʧ�� 
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU6050_RA_CONFIG ,data);//�������ֵ�ͨ�˲���  
}




u8 MPU_Write_Byte(u8 reg,u8 data) 				 
{ 
  i2c_Start(); 
	i2c_SendByte(MPU6050_SLAVE_ADDRESS|0);//����������ַ+д����	
	if(i2c_WaitAck())	//�ȴ�Ӧ��
	{
		i2c_Stop();		 
		return 1;		
	}
  i2c_SendByte(reg);	//д�Ĵ�����ַ
  i2c_WaitAck();		//�ȴ�Ӧ�� 
	i2c_SendByte(data);//��������
	if(i2c_WaitAck())	//�ȴ�ACK
	{
		i2c_Stop();	 
		return 1;		 
	}		 
  i2c_Stop();	 
	return 0;
}

