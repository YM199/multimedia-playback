#include "token.h"
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include "channel.h"
#include "thr_list.h"
#include <sys/stat.h>
#include "multicast.h"
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "thr_channel.h"

int sockse;
struct sockaddr_in dest_addr; /*目标IP*/

/**
 * @brief 初始化多播
 * 
 * @return int 
 */
static int sock_init(void)
{
    memset(&dest_addr, 0,sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(MCAST_ADDR); /*目标地址就是多播地址*/
    dest_addr.sin_port = htons(SERVER_PORT);           /*多播服务器的端口*/
    
    if ((sockse = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return -1;

    return 0;
}

/**
 * @brief SIGINT信号的处理函数
 * 
 * @param signal 
 */
static void sigint_handler(int signal)
{
    printf("服务器已退出\n");
    exit(EXIT_SUCCESS);
}

/**
 * @brief 设置SIGINT 信号
 * 
 * @return int 
 */
static int sigint_init(void)
{
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if(sigaction(SIGINT, &sa, NULL) < 0)
        return -1;

    return 0;
}

/**
 * @brief 清理函数
 * 
 */
void socket_destroy(void)
{
    close(sockse);
}

int main(int argc, char **argv)
{
    if(sigint_init() < 0)
    {
        fprintf(stderr, " %s %d %s\n",__FILE__, __LINE__, strerror(errno));
        exit(EXIT_FAILURE);        
    }

    if(sock_init() < 0)
    {
        fprintf(stderr, " %s %d %s\n",__FILE__, __LINE__, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if(atexit(socket_destroy) != 0)
    {
        fprintf(stderr, " %s %d %s\n",__FILE__, __LINE__, strerror(errno));
        exit(EXIT_FAILURE);        
    }

    if(fill_channel_array() < 0)
    {
        fprintf(stderr, " %s %d %s\n",__FILE__, __LINE__, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /*
     * token_init的调用一定要放在fill_channel_array之后
     * 因为fill_channel_array 给channel_num和channel_max赋值
    */
    if(token_init() < 0)
    {
        fprintf(stderr, " %s %d %s\n",__FILE__, __LINE__, strerror(errno));
        exit(EXIT_FAILURE);
    }


    if(thr_list_create(dest_addr) < 0)
    {
        fprintf(stderr, " %s %d %s\n",__FILE__, __LINE__, strerror(errno));
        exit(EXIT_FAILURE);        
    }

    for(int i = 0; i < channel_num; i++)
    {
        thr_channel_create(media);
    }
        
    while(1);
    exit(EXIT_SUCCESS);
}