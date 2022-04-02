#include "stm32f10x.h"
#include "bsp_motor.h"
#include "bsp_SysTick.h"
#include "bsp_speedtest.h"
#include "stdio.h"
#include "bsp_oled.h"
#include "bsp_pid.h"
#include "bsp_Bluetooth.h"
#include "bsp_hcsr.h"
#include "mpu6050.h"
#include "bsp_i2c.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 


extern int Time_count;

uint16_t hcsr_time;
float hcsr_distance;
int distance_flag;

int PID_flag;

volatile uint16_t CNT = 0;
volatile uint16_t CNT1 = 0;

s32 leftSpeedSet   = 5;//mm/s
s32 rightSpeedSet = 5;//mm/s
s32 leftSpeedNow  =0;
s32 rightSpeedNow =0;


extern enum System_status Car_Status;


void distancetest()
{
    TIM_Cmd(TIM2,ENABLE);
    GPIO_SetBits(GPIOB,GPIO_Pin_6);
    Delay_us(15);
    GPIO_ResetBits(GPIOB,GPIO_Pin_6);

    while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) == 0);
    TIM_SetCounter(TIM2,0);
    HCsr_EXTI_Open();
    // while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) );
    // TIM_Cmd(TIM2,DISABLE);
    // hcsr_time = TIM_GetCounter(TIM2);
    // USART3_printf("time = %d\r\n",hcsr_time);
    // hcsr_distance = hcsr_time * 0.017;
    // USART3_printf("distance = %.2f\r\n",hcsr_distance);
}


void stop()
{
    GPIO_ResetBits(GPIOE,GPIO_Pin_8);
    GPIO_ResetBits(GPIOE,GPIO_Pin_9);
    GPIO_ResetBits(GPIOE,GPIO_Pin_10);
    GPIO_ResetBits(GPIOE,GPIO_Pin_11);
}

void move()
{
    GPIO_SetBits(GPIOE,GPIO_Pin_8);
    GPIO_ResetBits(GPIOE,GPIO_Pin_9);
    GPIO_SetBits(GPIOE,GPIO_Pin_10);
    GPIO_ResetBits(GPIOE,GPIO_Pin_11);
}


void Status_Compare()
{
    
    if(Car_Status == Start_run)
    {
        move();
        
    }
    if(Car_Status == Start_wait)
    {
        stop();
    }
}

void findroad()
{

}

int main()
{

    //float Pitch,Roll,Yaw
    //int _flag;
    //int oled_count =0;

    //系统时钟初始化
    SysTick_Init();
    //GPIO_OLED_InitConfig();
    //sprintf(char_buffer,"CNT = \0");
    //OLED_Clear();
    //OLED_ShowString(0,0,char_buffer);

    //串口初始化
    USART_Config();
    //sprintf(char_buffer,"MLPWM = \0");
    //OLED_ShowString(0,1,char_buffer);
    
    //红外测速模块初始化
    speedtest_init(999,7199);

    //PID初始化
    PID_Init();

    //电机模块初始化
    motor1_init(7199,0);
    motor_gpio_config();

    //超声波测距模块初始化
    Hcsr_TIMx_Count_Config();
    HCsr_GPIO_Config();
    
    //i2c_GPIO_Config();

    Car_Status = Wait_CMD;
    hcsr_distance = 999;

    Time_count = 0;
    distance_flag = 0;
    PID_flag = 0;
    // if(MPU6050_Init())
	// {
	// 	USART3_printf("\r\nMPU6050 dectected error!\r\n");
	// }

	// else 
	// {
	// 	USART3_printf("MPU6050 ID = 0X68\r\n");	 
	// 	_flag = mpu_dmp_init();
	// 	if(_flag) USART3_printf("\r\nMPU6050 Error %d\r\n",_flag);
	// 	// while(_flag)
	// 	// {
	// 	// 	Delay_ms(100);
			
	// 	// 	USART1_printf("\r\nMPU6050 Error %d\r\n",_flag);
	// 	// }

	// }

    while(1)
    {

        //Delay_ms(900);
		// while(mpu_dmp_get_data(&Pitch,&Roll,&Yaw)!=0);

        // USART3_printf("\r\nPitch=%f, Roll=%f, Yaw=%f\r\n",Pitch,Roll,Yaw);

        // 测量距离
        if(distance_flag == 1){
            distance_flag = 0;   
            distancetest();
        }
        //Delay_ms(100);
        if(Usart3_RxCompleted == 1)
        {
            USART3_printf("%s\r\n",Usart3_RxBuff);
            Bluetooth_CMD();
            Status_Compare();
        }

        if(Car_Status == Wait_CMD)
        {
            continue;
        }

        if(Car_Status == Start_run)
        {
            //获得当前车轮速度
            pid_Task_Letf.SpeedSet  = leftSpeedSet;
            pid_Task_Right.SpeedSet = rightSpeedSet;
            pid_Task_Letf.SpeedNow  = leftSpeedNow;
            pid_Task_Right.SpeedNow = rightSpeedNow;



            if(hcsr_distance < 5)
            {
                Car_Status = Start_wait;
                stop();
                continue;
            }

            if(PID_flag)
            {

                //执行PID控制函数
                Pid_Ctrl(&motorLeft,&motorRight);
                //sprintf(char_buffer,"%d\0",motorLeft);
                //OLED_ShowString(60,1,char_buffer);
                
                //显示PID控制调整后的占空比
                //USART3_printf("LeftPWM = %d   RightPWM = %d\n",motorLeft,motorRight);

                USART3_printf("\r\nmotorleft = %d, motorright = %d",motorLeft,motorRight);
                //修改占空比
                TIM_SetCompare1(TIM3,motorLeft);
                TIM_SetCompare2(TIM3,motorRight); 
            }

        }

        if(Car_Status == Start_wait)
        {
            //findroad(); 
        }
    }
    // while(1)
    // {

    //     Delay_ms(100);
    //     //获得当前车轮速度
    //     pid_Task_Letf.SpeedSet  = leftSpeedSet;
    //     pid_Task_Right.SpeedSet = rightSpeedSet;
    //     pid_Task_Letf.SpeedNow  = leftSpeedNow;
    //     pid_Task_Right.SpeedNow = rightSpeedNow;

    //     distancetest();


    //     // 距离障碍物小于5cm 停车
    //     if(hcsr_distance < 5) 
    //     {
    //         stop();
    //     }
    //     else 
    //     {
    //         move();


    //         //执行PID控制函数
    //         Pid_Ctrl(&motorLeft,&motorRight);
    //         //sprintf(char_buffer,"%d\0",motorLeft);
    //         //OLED_ShowString(60,1,char_buffer);

    //         //显示PID控制调整后的占空比
    //         //USART3_printf("LeftPWM = %d   RightPWM = %d\n",motorLeft,motorRight);

    //         //修改占空比
    //         TIM_SetCompare1(TIM3,motorLeft);
    //         TIM_SetCompare2(TIM3,motorRight);        
    //     }


    //     //蓝牙收到信息后进行处理
    //     if(Usart3_RxCompleted == 1)
    //     {
    //         // oled_count =0;
    //         // while (Usart3_RxCounter--)
    //         // {
    //         //     char_buffer[oled_count] = Usart3_RxBuff[oled_count];
    //         //     oled_count++;
    //         // }
    //         // //char_buffer[oled_count] = '\0';
    //         Usart3_RxCounter=0;
    //         Usart3_RxCompleted = 0;
    //         //OLED_ClearRow(2);
    //         //OLED_ShowString(0,2,char_buffer);
    //     }
    // }
}
