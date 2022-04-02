#ifndef     _BSP_PID_H
#define     _BSP_PID_H
#define     L_Kp         (6*1024)
#define     L_Ki         (0*1024)
#define     L_Kd         (2*1024)
#define     L_Ur         (7199*1024)

#define     R_Kp         (6*1024)
#define     R_Ki         (0*1024)
#define     R_Kd         (2*1024)
#define     R_Ur         (7199*1024)
#include "stm32f10x.h"

typedef struct bsp_pid
{
    s32 Kp;
    s32 Ki;
    s32 Kd;
    s32 Adjust;
    s32 En;
    s32 SpeedSet;
    s32 SpeedNow;
    s32 Ur;
    s32 U_kk;
    s32 ekk;
    s32 ekkk;
       /* data */
}pid_uint;


extern pid_uint pid_Task_Letf;
extern pid_uint pid_Task_Right;


void PID_Init(void);
void reset_Uk(pid_uint *p);
s32 PID_common(int set,int jiance,pid_uint *p);
void Pid_Which(pid_uint *pl,pid_uint *pr);
void Pid_Ctrl(int *leftMotor,int  *rightMotor);

#endif
