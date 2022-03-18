#include "stm32f10x.h"
#include "bsp_motor.h"
#include "bsp_SysTick.h"
#include "bsp_speedtest.h"
#include "bsp_usart.h"

extern volatile uint16_t CNT = 0;

int main()
{
    USART_Config();
    //motor1_init(0,7199);
    speedtest_init(9999,7199);
    while(1)
    {
        Delay_ms(5000);
        //TIM_SetCompare1(TIM3,3599);
    }
}
