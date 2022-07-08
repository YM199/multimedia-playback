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

#define SONG_PATH "../../media/ch1/乱舞春秋.mp3"

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
    {
        return -1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    int fd;
    int len;
    int offset = 0;
    uint8_t data[SIZE];

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
    if((fd = open(SONG_PATH, O_RDONLY)) < 0)
    {
        close(sockse);
        fprintf(stderr, " %s %d %s\n",__FILE__, __LINE__, strerror(errno));
        exit(EXIT_FAILURE);
    }

    fill_channel_array();

    thr_list_create(dest_addr);
    int size;
    while(1)
    {
        // size = get_token(1,SIZE);
        // len = pread(fd , data, size, offset);
        // if(len < 0)
        // {
        //     fprintf(stderr, " %s %d %s\n",__FILE__, __LINE__, strerror(errno));
        // }
        // else if(len == 0)
        // {
        //     break; /*歌曲播放完毕*/
        // }
        // else
        // {
        //     offset += sizeof(data);
        //     if(sendto(sockse, data, sizeof(data), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0)
        //     {
        //         fprintf(stderr, " %s %d %s\n",__FILE__, __LINE__, strerror(errno));
        //     }
        // }
    }
    close(sockse);
    close(fd);
    exit(EXIT_SUCCESS);
}