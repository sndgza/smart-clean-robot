#include "stm32f10x.h"
#include "bsp_motor.h"
#include "bsp_SysTick.h"
#include "bsp_speedtest.h"
#include "stdio.h"
#include "bsp_oled.h"
#include "bsp_pid.h"
#include "bsp_Bluetooth.h"

volatile uint16_t CNT = 0;

int leftSpeeSet   = 10;//mm/s
int rightSpeedSet = 30;//mm/s
int leftSpeedNow  =0;
int rightSpeedNow =0;
char char_buffer[20];
int main()
{
    int oled_count =0;
    
    SysTick_Init();
    GPIO_OLED_InitConfig();
    sprintf(char_buffer,"CNT = \0");
    OLED_Clear();
    OLED_ShowString(0,0,char_buffer);
    USART_Config();
    sprintf(char_buffer,"MLPWM = \0");
    OLED_ShowString(0,1,char_buffer);
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
        OLED_ShowString(60,1,char_buffer);
        TIM_SetCompare1(TIM3,motorLeft);
        if(Usart3_RxCompleted == 1)
        {
            oled_count =0;
            while (Usart3_RxCounter--)
            {
                char_buffer[oled_count] = Usart3_RxBuff[oled_count];
                oled_count++;
            }
            char_buffer[oled_count] = '\0';
            Usart3_RxCounter=0;
            Usart3_RxCompleted = 0;
            OLED_ClearRow(2);
            OLED_ShowString(0,2,char_buffer);
        }
    }
}
