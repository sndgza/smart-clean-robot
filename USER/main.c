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
#include "Path_plan.h"

int onestep_flag;
int move_onestep_flag;
int move_onestep_count;


uint16_t hcsr_time;

float hcsr_distance;

float YAW_now;
float YAW_before;

int distance_flag;

int PID_flag;

int time_count;

volatile uint16_t CNT = 0;
volatile uint16_t CNT1 = 0;

s32 leftSpeedSet   = 10;//mm/s
s32 rightSpeedSet = 10;//mm/s
s32 leftSpeedNow  =0;
s32 rightSpeedNow =0;




void pidtest()
{

    pid_Task_Letf.SpeedSet  = leftSpeedSet;
    pid_Task_Right.SpeedSet = rightSpeedSet;
    pid_Task_Letf.SpeedNow  = leftSpeedNow;
    pid_Task_Right.SpeedNow = rightSpeedNow;
    if(PID_flag == 0) return;
    PID_flag =0;
    //执行PID控制函数
    Pid_Ctrl(&motorLeft,&motorRight);

    //显示PID控制调整后的占空比
    //USART3_printf("\r\nmotorleft = %d, motorright = %d\r\n",motorLeft,motorRight);
    //修改占空比
    TIM_SetCompare1(TIM3,motorLeft);
    TIM_SetCompare2(TIM3,motorRight);     
}

void distancetest()
{
    TIM_Cmd(TIM7,ENABLE);
    GPIO_SetBits(GPIOB,GPIO_Pin_6);
    Delay_us(15);
    GPIO_ResetBits(GPIOB,GPIO_Pin_6);

    while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) == 0);
    TIM_SetCounter(TIM7,0);
    HCsr_EXTI_Open();
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

void turn_left()
{
    GPIO_SetBits(GPIOE,GPIO_Pin_9);
    GPIO_ResetBits(GPIOE,GPIO_Pin_8);
    GPIO_SetBits(GPIOE,GPIO_Pin_10);
    GPIO_ResetBits(GPIOE,GPIO_Pin_11);    
}

void turn_right()
{
    GPIO_SetBits(GPIOE,GPIO_Pin_8);
    GPIO_ResetBits(GPIOE,GPIO_Pin_9);
    GPIO_SetBits(GPIOE,GPIO_Pin_11);
    GPIO_ResetBits(GPIOE,GPIO_Pin_10);
}

void turn_back()
{
    GPIO_SetBits(GPIOE,GPIO_Pin_9);
    GPIO_ResetBits(GPIOE,GPIO_Pin_8);
    GPIO_SetBits(GPIOE,GPIO_Pin_11);
    GPIO_ResetBits(GPIOE,GPIO_Pin_10);
}

void turn_right_90()
{
    float Pitch,Roll,Yaw,Yaw_temp_before,Yaw_temp_now;
    int _flag;
    _flag = mpu_dmp_init();
	if(_flag)
    {
        USART3_printf("\r\nMPU6050 DMP Error %d\r\n",_flag);
        Car_Status = Start_wait;
    }
    Delay_ms(1000);
    while(mpu_dmp_get_data(&Pitch,&Roll,&Yaw)!=0);
    USART3_printf("\r\nYaw=%.2f\r\n",Yaw);
    Yaw_temp_before = Yaw;
    Car_Status = Start_turnright;
    turn_right();
    while (1)
    {
        pidtest();
        //stop();
        while(mpu_dmp_get_data(&Pitch,&Roll,&Yaw)!=0);
        Yaw_temp_now = Yaw;
        Yaw_temp_now = Yaw_temp_before - Yaw;
        if(Yaw_temp_now < 0) continue;
        if(Yaw_temp_now > 85 && Yaw_temp_now < 88)
        {
            stop();
            turn_dirction++;
            turn_dirction %= 4;
            break;
        }
        else if(Yaw_temp_now > 88)
        {
            turn_left();
        }
        else 
        {
            turn_right();
        }
        //Delay_ms(50);
    }   
}

void turn_left_90()
{
    float Pitch,Roll,Yaw,Yaw_temp_before,Yaw_temp_now;
    int _flag;
    _flag = mpu_dmp_init();
	if(_flag)
    {
        USART3_printf("\r\nMPU6050 DMP Error %d\r\n",_flag);
        Car_Status = Start_wait;
    }
    Delay_ms(1000);
    while(mpu_dmp_get_data(&Pitch,&Roll,&Yaw)!=0);
    USART3_printf("\r\nYaw=%.2f\r\n",Yaw);
    Yaw_temp_before = Yaw;
    turn_left();
    Car_Status = Start_turnleft;
    while (1)
    {
        pidtest();
        while(mpu_dmp_get_data(&Pitch,&Roll,&Yaw)!=0);
        Yaw_temp_now = Yaw - Yaw_temp_before;
        if(Yaw_temp_now < 0) continue;
        if(Yaw_temp_now > 85 && Yaw_temp_now < 88)
        {
            stop();
            turn_dirction += 3;
            turn_dirction %= 4;
            break;
        }
        else if(Yaw_temp_now > 88)
        {
            turn_right();
        }
        else
        {
            turn_left();
        }
    }   
}

void turn_180()
{
    turn_left_90();
    turn_left_90();
}


void Status_Compare()
{
    
    if(Car_Status == Start_run)
    {
        //move();
        
    }
    if(Car_Status == Start_wait)
    {
        //stop();
    }
    if(Car_Status == Start_turnleft)
    {
        //turn_dirction += 3;
        //turn_dirction %= 4;
        //turn_left();
    }
    if(Car_Status == Start_turnright)
    {
        //turn_dirction += 1;
        //turn_dirction %= 4;        
        //turn_right();
    }
}

void move_onestep()
{
    onestep_flag = 0;
    CNT = 0;
    CNT1 = 0;
    Car_Status = Start_run;
    move();
    while (onestep_flag == 0)
    {
        pidtest();
    }
    stop();
    Car_Location.x += Path_road[turn_dirction][0];
    Car_Location.y += Path_road[turn_dirction][1];
    if(Car_Location.x >= 30 || Car_Location.x < 0 || Car_Location.y >= 30 || Car_Location.y < 0)
    {
        USART3_printf("\r\nerror111111111\r\n");
        Car_Status = Wait_CMD;
    }
    else 
    {
        USART3_printf("\r\nx = %d y = %d dir = %d\r\n",Car_Location.x,Car_Location.y,turn_dirction);
        Path_Map[Car_Location.x][Car_Location.y] = 1;
    }
}
int findroad()
{
    int temp_x,temp_y,temp_view_now,temp_view_before;           //中间变量
    int i,j,z,k;
    struct Queue q;
    struct Location xy;
    struct Location next_xy;
    xy.x = Car_Location.x;
    xy.y = Car_Location.y;
    QueueInit(&q);
    QueuePush(&q,xy);
    //将地图中拜访过的痕迹清零
    for(i = 0; i < 30; i++)
    {
        for(j = 0; j <  30; j++)
        {
            Path_view[i][j] = 0;
        }
    }
    //标明该位置是从哪个位置移动过来的
    Path_view[xy.x][xy.y] = turn_dirction;
    
    //广度搜索距离最近的空地
    while(!QueueEmpty(&q))
    {
        USART3_printf("\r\nhello\r\n");
        QueuePop(&q,&xy);
        //查询四个方向上的路径是否可以走
        for(i = 0;i < 4 ; i++)
        {
            next_xy.x = xy.x + Path_road[i][0];
            next_xy.y = xy.y + Path_road[i][1];
            //在地图范围内
            if(next_xy.x < 30 && next_xy.x >= 0 && next_xy.y < 30 && next_xy.y >= 0)
            {
                //printf("%d %d\n",next_xy.x,next_xy.y);
                //找到出口
                if(Path_view[next_xy.x][next_xy.y] == 0 && Path_Map[next_xy.x][next_xy.y] == 0)
                {
                    //销毁队列，释放空间
                    QueueDestory(&q);
                    //标记该位置从何处来
                    Path_view[next_xy.x][next_xy.y] = i;
                    USART3_printf("\r\n%d %d %d\r\n",Car_Location.x,Car_Location.y,turn_dirction);
                    USART3_printf("\r\n%d %d %d\r\n",next_xy.x,next_xy.y,i);
                    // for(z = 0; z < 30; z++)
                    // {
                    //     for(k = 0; k < 30; k++)
                    //     {
                    //         USART3_printf("%d ",Path_Map[z][k]);
                    //     }
                    //     USART3_printf("\r\n\r\n");
                    // }
                    //从结束位置回溯到起点位置并标明最短路径
                    temp_view_before = i;
                    while(next_xy.x != Car_Location.x || next_xy.y != Car_Location.y)
                    {
                        USART3_printf("\r\n%d %d %d\r\n",next_xy.x,next_xy.y,Path_view[next_xy.x][next_xy.y]);
                        temp_x = next_xy.x;
                        temp_y = next_xy.y;
                        next_xy.x = next_xy.x - Path_road[temp_view_before][0];
                        next_xy.y = next_xy.y - Path_road[temp_view_before][1];
                        temp_view_now = Path_view[next_xy.x][next_xy.y];
                        Path_view[next_xy.x][next_xy.y] = temp_view_before;
                        temp_view_before = temp_view_now;
                    }
                    // printf("%d %d\n",next_xy.x,next_xy.y);
                    // //Sleep(2000);
                    // for(z = 0; z < 10; z++)
                    // {
                    //     for(k = 0; k < 10; k++)
                    //     {
                    //         printf("%d ",view[z][k]);
                    //     }
                    //     printf("\n");
                    // }
                    // Sleep(10000);

                    //从起点位置出发到最近空地
                    while(next_xy.x != (xy.x + Path_road[i][0]) || next_xy.y != (xy.y + Path_road[i][1]))
                    {
                        USART3_printf("\r\n%d %d %d\r\n",next_xy.x,next_xy.y,Path_view[next_xy.x][next_xy.y]);
                        //printf("kajsjkfagjf\n");
                        //Sleep(2000);
                        //system("cls");
                        // Path_Map[next_xy.x][next_xy.y] = 4;
                        // for(z = 0; z < 10; z++)
                        // {
                        //     for(k = 0; k < 10; k++)
                        //     {
                        //         printf("%d ",Path_Map[z][k]);
                        //     }
                        //     printf("\n");
                        // }
                        temp_x = next_xy.x;
                        temp_y = next_xy.y;
                        // while(turn_dirction != Path_view[temp_x][temp_y])
                        // {
                        //     turn_left_90();
                        // }
                        if(turn_dirction != Path_view[temp_x][temp_y])
                        {
                            if(turn_dirction > Path_view[temp_x][temp_y])
                            {
                                if(turn_dirction - Path_view[temp_x][temp_y] == 1)
                                {
                                    turn_left_90();
                                }
                                else if(turn_dirction - Path_view[temp_x][temp_y] == 2)
                                {
                                    turn_left_90();
                                    turn_left_90();
                                }
                                else
                                {
                                    turn_right_90();
                                }
                            }
                            else 
                            {
                                if(turn_dirction - Path_view[temp_x][temp_y] == -3)
                                {
                                    turn_left_90();
                                }
                                else if(turn_dirction - Path_view[temp_x][temp_y] == -2)
                                {
                                    turn_left_90();
                                    turn_left_90();
                                }
                                else
                                {
                                    turn_right_90();
                                }
                            }
                        }
                        distance_flag = 0; 
                        distancetest();
                        while (distance_flag == 0);
                        if(hcsr_distance > 11)
                        {
                            move_onestep();
                            stop();
                        }
                        else
                        {
                            Path_Map[temp_x][temp_y] = 2;
                            break;
                        }
                        // if(Path_view[temp_x][temp_y] == 0)
                        // {
                        //     distance_flag = 0; 
                        //     distancetest();
                        //     while (distance_flag == 0);
                        //     if(hcsr_distance > 11)
                        //     {
                        //         move_onestep();
                        //         stop();
                        //     }
                        //     else
                        //     {
                        //         Path_Map[temp_x][temp_y] = 2;
                        //         break;
                        //     }
                        // }
                        // else if(Path_view[temp_x][temp_y] == 1)
                        // {
                        //     turn_right_90();
                        //     distance_flag = 0; 
                        //     distancetest();
                        //     while (distance_flag == 0);
                        //     if(hcsr_distance > 11)
                        //     {
                        //         move_onestep();
                        //         stop();
                        //     }
                        //     else
                        //     {
                        //         Path_Map[temp_x][temp_y] = 2;
                        //         break;
                        //     }
                        // }
                        // else if(Path_view[temp_x][temp_y] == 2)
                        // {
                        //     turn_180();
                        //     distance_flag = 0; 
                        //     distancetest();
                        //     while (distance_flag == 0);
                        //     if(hcsr_distance > 11)
                        //     {
                        //         move_onestep();
                        //         stop();
                        //     }
                        //     else
                        //     {
                        //         Path_Map[temp_x][temp_y] = 2;
                        //         break;
                        //     }
                        // }
                        // else if(Path_view[temp_x][temp_y] == 3)
                        // {
                        //     turn_left_90();
                        //     distance_flag = 0; 
                        //     distancetest();
                        //     while (distance_flag == 0);
                        //     if(hcsr_distance > 11)
                        //     {
                        //         move_onestep();
                        //         stop();
                        //     }
                        //     else
                        //     {
                        //         Path_Map[temp_x][temp_y] = 2;
                        //         break;
                        //     }
                        // }
                        next_xy.x = next_xy.x + Path_road[Path_view[temp_x][temp_y]][0];
                        next_xy.y = next_xy.y + Path_road[Path_view[temp_x][temp_y]][1];
                    }
                    //将找到的最近出口设为起点
                    //Car_Location = next_xy;
                    return 0;
                }
                //不是最近的空地但没有经过的路径
                if(Path_view[next_xy.x][next_xy.y] == 0 && Path_Map[next_xy.x][next_xy.y] == 1 )
                {

                    //Sleep(10000)
                    //if(Map[next_xy.x][next_xy.y] == 0) Map[next_xy.x][next_xy.y] = 2;
                    //标记拜访这个位置时的路线
                    Path_view[next_xy.x][next_xy.y] = i;
                    //将该位置作为节点数据入队
                    QueuePush(&q,next_xy);
                }
            }
        }
    }
    return 1;
}

int main()
{

    //float Pitch,Roll,Yaw;
    int _flag,i,j;
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
    i2c_GPIO_Config();

    Car_Status = Wait_CMD;
    hcsr_distance = 999;

    distance_flag = 1;
    PID_flag = 0;
    time_count = 0;
    turn_dirction = 0;

    Car_Location.x = 15;
    Car_Location.y = 15;
    Delay_ms(1000);
    for(i = 0; i < 30; i++)
    {
        for(j = 0; j < 30; j++)
        {
            Path_Map[i][j] = 0;
            Path_view[i][j] = 0;
        }
    }

    Path_Map[Car_Location.x][Car_Location.y] = 1;

    if(MPU6050_Init())
	{
		USART3_printf("\r\nMPU6050 dectected error!\r\n");
	}

	else 
	{
		USART3_printf("MPU6050 ID = 0X68\r\n");	 
		_flag = mpu_dmp_init();
		if(_flag) USART3_printf("\r\nMPU6050 DMP Error %d\r\n",_flag);
		// while(_flag)
		// {
		// 	Delay_ms(100);
			
		// 	USART1_printf("\r\nMPU6050 Error %d\r\n",_flag);
		// }

	}
    while(1)
    {

        pidtest();
        //Delay_ms(100);
        if(Usart3_RxCompleted == 1)
        {
            USART3_printf("\r\n%s\r\n",Usart3_RxBuff);
            Bluetooth_CMD();
            Status_Compare();
        }

        if(Car_Status == Wait_CMD)
        {
            stop();
            continue;
        }

        else if(Car_Status == Start_run)
        {
            distance_flag = 0; 
            distancetest();
            while (distance_flag == 0);
            if(hcsr_distance > 11 && Path_Map[Car_Location.x+Path_road[turn_dirction][0]][Car_Location.y+Path_road[turn_dirction][1]] == 0)
            {
                
                move_onestep();
                onestep_flag = 0;
            }
            //遇到障碍物
            else
            {
                //USART3_printf("/r/ndistance = %.2f/r/n",hcsr_distance);
                //停车
                stop();
                Car_Status = Start_wait;
                //标记前方为障碍物
                USART3_printf("\r\nThere is no way, turn_dirction = %d\r\n",turn_dirction);
                if(hcsr_distance < 11)
                {
                    Path_Map[Car_Location.x+Path_road[turn_dirction][0]][Car_Location.y+Path_road[turn_dirction][1]] = 2;
                }
                USART3_printf("\r\n map = %d\r\n",Path_Map[Car_Location.x+Path_road[(turn_dirction+1)%4][0]][Car_Location.y+Path_road[(turn_dirction+1)%4][1]]);
                
                //当前位置右边为没有走过的路径
                if(Path_Map[Car_Location.x+Path_road[(turn_dirction+1)%4][0]][Car_Location.y+Path_road[(turn_dirction+1)%4][1]] == 0)
                {
                    USART3_printf("\r\n???????????????\r\n");
                    //进行90°右转
                    Car_Status = Start_turnright;

                }
                else if(Path_Map[Car_Location.x+Path_road[(turn_dirction+3)%4][0]][Car_Location.y+Path_road[(turn_dirction+3)%4][1]] == 0)      //左边
                {

                    //90°左转
                    Car_Status = Start_turnleft;
                }
                else            //无路可走
                {
                    stop();
                    //寻找路径
                    if(findroad())
                    {
                        Path_Map[15][15] = 0;
                        findroad();
                    }
                    Car_Status = Wait_CMD;
                }
                continue;
            }



        }
        else if(Car_Status == Start_turnleft)
        {
                stop();
                turn_left_90();
                distance_flag = 0; 
                distancetest();
                while (distance_flag == 0);  
                if(hcsr_distance > 11)
                {
                    move_onestep();
                    turn_left_90();
                    stop();
                }
                else 
                {
                    USART3_printf("\r\nno way\r\n");
                    Path_Map[Car_Location.x+Path_road[turn_dirction][0]][Car_Location.y+Path_road[turn_dirction][1]] = 2;
                }
                Car_Status = Start_run;
            // }
        }
        else if(Car_Status == Start_turnright)
        {
                stop();
                turn_right_90();
                distance_flag = 0; 
                distancetest();
                while (distance_flag == 0);
                //USART3_printf("\r\ndis = %.2f\r\n",hcsr_distance);
                if(hcsr_distance > 11)
                {
                    move_onestep();
                    turn_right_90();
                    stop();
                }
                else 
                {
                    USART3_printf("\r\nno way\r\n");
                    Path_Map[Car_Location.x+Path_road[turn_dirction][0]][Car_Location.y+Path_road[turn_dirction][1]] = 2;
                }
                Car_Status = Start_run;
        }
        else if(Car_Status == Start_wait)
        {
            //findroad(); 
        }
    }
}
