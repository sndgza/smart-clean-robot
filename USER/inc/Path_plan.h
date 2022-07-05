#ifndef _PATH_PLAN_H
#define _PATH_PLAN_H
#include "stdio.h"
#include "stdlib.h"

extern char Path_Map[30][30];                               //地图
extern int  Path_view[30][30];                              //探索区域
extern int  Path_road[4][2];                                //四个前进路线
extern int  turn_dirction;
/** 
  * @brief  坐标结构体定义  
  */
struct Location
{
    int x;
    int y;
};

/** 
  * @brief  队列节点定义 
  */
struct Link
{
    struct Location data;
    struct Link* next;
};
/** 
  * @brief  队列定义 
  */
struct Queue
{
    struct Link* front;
    struct Link* rear;
    int size;
};


void QueueInit(struct Queue* queue);
int  QueueEmpty(struct Queue* queue);
void QueuePush(struct Queue* queue,struct Location data);
void QueuePop(struct Queue* queue,struct Location* data);
void QueueDestory(struct Queue* queue);
int  search(struct Location *sxy,int dirction);

extern struct Location Car_Location;

#endif
