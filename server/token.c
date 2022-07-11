#include "token.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "channel.h"
#include "multicast.h"

struct Token *token;/*每个频道对应一个单独使用的令牌*/
pthread_mutex_t token_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief 更新令牌值
 * 
 * @param signal 
 */
void alarm_handler(int signal)
{
    pthread_mutex_lock(&token_mutex);
    for(int i = 0; i < channel_max; i++)
    {
        pthread_mutex_lock(&token[i].mutex);
        token[i].num += token[i].cps;
        if(token[i].num > token[i].burst)
        {
            token[i].num = token[i].burst;
        }
        pthread_cond_signal(&token[i].cond);
        pthread_mutex_unlock(&token[i].mutex);
    }
    pthread_mutex_unlock(&token_mutex);
    alarm(1);
}

/**
 * @brief 获取令牌值
 * 
 * @param i 数组下标
 * @param size 值的大小
 * @return int 实际获取到的大小
 */
int get_token(int i,long size)
{
    pthread_mutex_lock(&token[i].mutex);
    while(token[i].num <= 0)
        pthread_cond_wait(&token[i].cond, &token[i].mutex);
    int ret = (size < token[i].num ? size : token[i].num);
    token[i].num -= ret;

    pthread_mutex_unlock(&token[i].mutex);

    return ret;
}

/**
 * @brief 归还令牌值
 * 
 * @param i 数组小标
 * @param size 值的大小
 */
void return_token(int i, long size)
{
    pthread_mutex_lock(&token[i].mutex);
    token[i].num += size;
    if(token[i].num > token[i].burst)
    {
        token[i].num = token[i].burst;
    }
    pthread_mutex_unlock(&token[i].mutex);
}

/**
 * @brief 设置SIGALRM信号的处理函数
 * 
 * @return int 
 */
static int alarm_init(void)
{
    struct sigaction sa;
    sa.sa_handler = alarm_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if(sigaction(SIGALRM, &sa, NULL) < 0)
    {
        return -1;
    }

    return 0;
}

/**
 * @brief 令牌初始化
 * 
 * @return int 
 */
int token_init(void)
{
    token = (struct Token *)malloc(sizeof(struct Token) * channel_max);
    if(token == NULL)
        return -1;
    
    for(int i = 0; i < channel_max; i++)
    {
        token[i].cps = CPS;
        token[i].burst = BURST;
        token[i].num = 0;
        pthread_mutex_init(&token[i].mutex, NULL);
        pthread_cond_init(&token[i].cond, NULL);
    }

    if(alarm_init() < 0)
        return -1;
    alarm(1);

    return 0;
}

void token_destory(void)
{
    for(int i = 0; i < channel_max; i++)
    {
        pthread_mutex_destroy(&token[i].mutex);
        pthread_cond_destroy(&token[i].cond);
    }    
}