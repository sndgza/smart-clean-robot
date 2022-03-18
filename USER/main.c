#include "stm32f10x.h"
#include "bsp_motor.h"
#include "bsp_SysTick.h"
int main()
{

    motor1_init(0,7199);

    while(1)
    {
        Delay_ms(5000);
        TIM_SetCompare1(TIM3,3599);
    }
}
