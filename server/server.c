#include "token.h"
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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
int sock_init(void)
{
    memset(&dest_addr, 0,sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(MCAST_ADDR); /*目标地址就是多播地址*/
    dest_addr.sin_port = htons(SERVER_PORT);           /*多播服务器的端口*/
    
    if ((sockse = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return -1;

    return 0;
}

int main(int argc, char **argv)
{
    if(sock_init() < 0)
    {
        fprintf(stderr, " %s %d %s\n",__FILE__, __LINE__, strerror(errno));
        exit(EXIT_FAILURE);
    }
    if(token_init() < 0)
    {
        fprintf(stderr, " %s %d %s\n",__FILE__, __LINE__, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if(fill_channel_array() < 0)
    {
        fprintf(stderr, " %s %d %s\n",__FILE__, __LINE__, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if(thr_list_create(dest_addr) < 0)
    {
        fprintf(stderr, " %s %d %s\n",__FILE__, __LINE__, strerror(errno));
        exit(EXIT_FAILURE);        
    }

    for(int i = 0; i < CHANNEL_NUM; i++)
    {
        thr_channel_create(media);
    }

    while(1)
    {
    }
    close(sockse);
    exit(EXIT_SUCCESS);
}