#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include "multicast.h"
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "multicast.h"
int sockfd;

/**
 * @brief 初始化多播
 * 
 * @return int 
 */
int sock_init(void)
{
    struct ip_mreq mreq;

    /*client的IP地址*/
    struct sockaddr_in local_addr;
    memset(&local_addr, 0,sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(CLIENT_PORT);
    
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return -1;
    if(bind(sockfd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0)
        return -1;

    mreq.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR); /*设置多播组的地址*/
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);     /*将本机加入多播组*/
    /*加入多播组*/
    if(setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
        return -1;
    
    /*关闭回送到本地的回环接口*/
    u_char flag = 0;
    if(setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP, &flag, sizeof(flag)) < 0)
        return -1;

    return 0;
}

int main(int argc, char *argv[])
{
    pid_t pid;
    int fd[2]; /*管道*/
    
    struct List_channel *list_channel;
    list_channel = (struct List_channel *)malloc(MSG_LIST_MAX);


    if(sock_init() < 0)
        fprintf(stderr, " %s %d %s\n",__FILE__, __LINE__, strerror(errno));

    if(pipe(fd) < 0)
    {
        fprintf(stderr, " %s %d %s\n",__FILE__, __LINE__, strerror(errno));
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if(pid < 0)
    {
        fprintf(stderr, " %s %d %s\n",__FILE__, __LINE__, strerror(errno));
        exit(EXIT_FAILURE);
    }
    else if(pid == 0) /*子进程*/
    {
        close(fd[1]);
        dup2(fd[0], 0);
        if(fd[0] > 0)
            close(fd[0]);
        execl("/bin/bash", "sh", "-c", "mplayer - > /dev/null", NULL); /*-表示从标准输入读取数据*/
    }
    else /*父进程*/
    {
        // close(fd[0]);
        // while(1)
        // {
        //     if(recvfrom(sockfd, data, sizeof(data), 0, NULL, NULL) < 0)
        //     {
        //         fprintf(stderr, " %s %d %s\n",__FILE__, __LINE__, strerror(errno));
        //     }
        //     write(fd[1], data, sizeof(data));
        // }
        int len;
        while(1)
        {
            struct List_channel *pos; 
            len = recvfrom(sockfd, list_channel, MSG_LIST_MAX, 0, NULL, NULL);
            for(pos = list_channel; (char *)pos < ((char *)list_channel + len); pos = (struct List_channel *)((char *)pos + pos->len))
            {
                printf("%d\n", pos->chnid);
                printf("%d\n", pos->len);
                printf("%s\n", pos->desc);
            }
            break;
        }
    }
    exit(EXIT_SUCCESS);
}