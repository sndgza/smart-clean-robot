#include "stm32f10x.h"
#include "bsp_motor.h"
#include "bsp_SysTick.h"
#include "bsp_speedtest.h"
#include "stdio.h"
#include "bsp_oled.h"

volatile uint16_t CNT = 0;
char char_buffer[20];
int main()
{

    SysTick_Init();
    GPIO_OLED_InitConfig();
    sprintf(char_buffer,"CNT = \0");
    OLED_Clear();
    OLED_ShowString(0,0,char_buffer);

    speedtest_init(9999,7199);
    while(1)
    {
        //Delay_ms(5000);
        //TIM_SetCompare1(TIM3,3599);
    }
}
