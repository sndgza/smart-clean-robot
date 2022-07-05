/*
*******************************************************************************
* @file    bsp_hcsr.c
* @author  sndgza
* @version V1.1
* @date    2022-02-12
* @brief   路径规划算法
******************************************************************************
* @attention
******************************************************************************
*/
#include "Path_plan.h"
char Path_Map[30][30];                               //地图
int  Path_view[30][30];                              //探索区域
int  Path_road[4][2] = {-1,0,0,1,1,0,0,-1};      //四个前进路线
int  turn_dirction;

struct Location Car_Location;
/**
 *@brief   队列初始化函数                         
 *@param   Queue* queue  要初始化的队列    
 *@retval  无                                       
 */
void QueueInit(struct Queue* queue)
{
    queue->front = NULL;
    queue->rear  = NULL;
    queue->size  = 0;
}

/**
 *@brief   判断队列是否为空函数                         
 *@param   Queue* queue  要进行判断的队列    
 *@retval  队列为空返回1，不为空返回0                                       
 */
int QueueEmpty(struct Queue* queue)
{
    return (queue->size == 0);
}

/**
 *@brief   入队函数                         
 *@param   Queue* queue  要操作的队列    
 *@param   Location data  入队的数据    
 *@retval  无                                       
 */
void QueuePush(struct Queue* queue,struct Location data)
{
    struct Link* node;

    //给节点分配空间
    node = (struct Link*)malloc(sizeof(struct Link));
    //assert(node !=NULL);

    //数据压入节点中
    node->data = data;
    node->next = NULL;

    //队列为空则将节点设为对队头，否则将节点加入队尾
    if(QueueEmpty(queue))
    {
        queue->front = node;
        queue->rear  = node;
    }
    else
    {
        queue->rear->next = node;
        queue->rear = node;
    }
    //队列长度加一
    ++queue->size;
}

/**
 *@brief   出队函数                         
 *@param   Queue* queue  要操作的队列    
 *@param   Location data  出队的数据    
 *@retval  无                                       
 */
void QueuePop(struct Queue* queue,struct Location* data)
{
    struct Link* temp;

    //队列为空 直接返回
    if(QueueEmpty(queue))
    {
        return ;
    }

    //取出节点
    temp = queue->front;
    //取出数据
    *data = queue->front->data;
    //队头出队
    queue->front = queue->front->next;
    //释放节点空间
    free(temp);
    //队列长度减一
    --queue->size;
}

/**
 *@brief   队列销毁函数                         
 *@param   Queue* queue  要操作的队列    
 *@retval  无                                       
 */
void QueueDestory(struct Queue* queue)
{
    struct Link* tmp;
    while(queue->front)
    {
        //取出队头节点
        tmp = queue->front;
        //队头出队
        queue->front = queue->front->next;
        //释放节点空间
        free(tmp);
    }
}
int search(struct Location *sxy,int dirction)
{
    int temp_x,temp_y,temp_view_now,temp_view_before;           //中间变量
    int i,j,z,k;
    struct Queue q;
    struct Location xy;
    struct Location next_xy;
    xy.x = sxy->x;
    xy.y = sxy->y;
    QueueInit(&q);
    QueuePush(&q,xy);
    //将地图中拜访过的痕迹清零
    for(i = 0; i <= 9; i++)
    {
        for(j = 0; j <= 9; j++)
        {
            Path_view[i][j] = 0;
        }
    }
    //标明该位置是从个位置移动过来的
    Path_view[xy.x][xy.y] = dirction;
    //广度搜索距离最近的空地
    while(!QueueEmpty(&q))
    {
        QueuePop(&q,&xy);
        //查询四个方向上的路径是否可以走
        for(i = 0;i < 4 ; i++)
        {
            next_xy.x = xy.x + Path_road[i][0];
            next_xy.y = xy.y + Path_road[i][1];
            //在地图范围内
            if(next_xy.x < 10 && next_xy.x >= 0 && next_xy.y < 10 && next_xy.y >= 0)
            {
                //printf("%d %d\n",next_xy.x,next_xy.y);
                //找到出口
                if(Path_view[next_xy.x][next_xy.y] == 0 && Path_Map[next_xy.x][next_xy.y] == 0)
                {
                    //销毁队列，释放空间
                    QueueDestory(&q);
                    //标记该位置从何处来
                    Path_view[next_xy.x][next_xy.y] = i;
                    
                    // printf("%d %d %d\n",next_xy.x,next_xy.y,dirction);
                    // for(z = 0; z < 10; z++)
                    // {
                    //     for(k = 0; k < 10; k++)
                    //     {
                    //         printf("%d ",view[z][k]);
                    //     }
                    //     printf("\n");
                    // }
                    //从结束位置回溯到起点位置并标明最短路径
                    temp_view_before = i;
                    while(next_xy.x != sxy->x || next_xy.y != sxy->y)
                    {
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
                        //printf("kajsjkfagjf\n");
                        //Sleep(2000);
                        //system("cls");
                        Path_Map[next_xy.x][next_xy.y] = 4;
                        for(z = 0; z < 10; z++)
                        {
                            for(k = 0; k < 10; k++)
                            {
                                printf("%d ",Path_Map[z][k]);
                            }
                            printf("\n");
                        }
                        temp_x = next_xy.x;
                        temp_y = next_xy.y;
                        next_xy.x = next_xy.x + Path_road[Path_view[temp_x][temp_y]][0];
                        next_xy.y = next_xy.y + Path_road[Path_view[temp_x][temp_y]][1];
                    }
                    //将找到的最近出口设为起点
                    *sxy = next_xy;
                    return 1;
                }
                //不是最近的空地但没有经过的路径
                if(Path_view[next_xy.x][next_xy.y] == 0 && Path_Map[next_xy.x][next_xy.y] != 1 )
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
    return 0;
}
