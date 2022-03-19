#include "stm32f10x.h"
#include "bsp_motor.h"
#include "bsp_SysTick.h"
#include "bsp_speedtest.h"
#include "stdio.h"
#include "bsp_oled.h"
#include "bsp_pid.h"

volatile uint16_t CNT = 0;

int leftSpeeSet   = 10;//mm/s
int rightSpeedSet = 30;//mm/s
int leftSpeedNow  =0;
int rightSpeedNow =0;
//extern s32 motorLeft;
//extern s32 motorRight;
char char_buffer[20];
int main()
{

    SysTick_Init();
    GPIO_OLED_InitConfig();
    sprintf(char_buffer,"CNT = \0");
    OLED_Clear();
    OLED_ShowString(0,0,char_buffer);
    
    sprintf(char_buffer,"MLPWM = \0");
    OLED_ShowString(0,3,char_buffer);
    speedtest_init(9999,7199);

    PID_Init();
    motor1_init(7199,0);
    while(1)
    {
        Delay_ms(1000);
        pid_Task_Letf.SpeedSet  = leftSpeeSet;
        pid_Task_Right.SpeedSet = rightSpeedSet;
        pid_Task_Letf.SpeedNow  = leftSpeedNow;
        pid_Task_Right.SpeedNow = rightSpeedNow;

        //执行PID控制函数
        Pid_Ctrl(&motorLeft,&motorRight);
        sprintf(char_buffer,"%d\0",motorLeft);
        OLED_ShowString(60,3,char_buffer);
        TIM_SetCompare1(TIM3,motorLeft);

    }
}
