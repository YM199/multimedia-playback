#include "token.h"
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

static volatile int token = 0; /*令值*/

/**
 * @brief 更新令牌值
 * 
 * @param signal 
 */
void alarm_handler(int signal)
{
    token += CPS;
    if(token > BURST)
        token = BURST;
    alarm(1);
}

/**
 * @brief 数组SIGALRM信号的处理函数
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
 * @brief 等待令牌值有效
 * 
 */
void wait_token(void)
{
    while(token <= 0)
        pause();
    token -= SIZE;
    if(token < 0)
        token = 0;
}

/**
 * @brief 令牌初始化
 * 
 * @return int 
 */
int token_init(void)
{
    if(alarm_init() < 0)
        return -1;
    alarm(1);

    return 0;
}