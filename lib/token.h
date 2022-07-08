#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <pthread.h>

#define BIT_RATE 320                /*歌曲的比特率*/
#define SIZE     (BIT_RATE*1000/8)  /*歌曲每秒的数据大小(字节)*/
#define CPS      SIZE               /*令牌值每秒累加值*/
#define BURST    (SIZE*3)           /*令牌值上限*/

struct Token
{
    long cps;    /*令牌值每秒累加值*/
    long burst;  /*令牌值上限*/
    volatile long num;  /*令牌值*/

    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

int token_init(void);
int get_token(int i,long size);
void return_token(int i, long size);

#endif
