#include <stdio.h>
#include "channel.h"
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include "thr_list.h"
#include <string.h>
#include "../server/server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

struct List_channel *list_channel;

void *thr_handler(void *arg)
{
    // struct sockaddr_in *dest_addr = (struct sockaddr_in *)arg;
    // if(sendto(sockse, list_channel, sizeof(*list_channel), 0, (struct sockaddr *)dest_addr, sizeof(*dest_addr)) < 0)
    // {
    //     fprintf(stderr, " %s %d %s\n",__FILE__, __LINE__, strerror(errno));
    // }
    return NULL;
}

/**
 * @brief 准备待发送的数据和创建线程
 * 
 * @param addr 套接字
 * @return int 
 */
int thr_list_create(struct sockaddr_in addr)
{
    int size =0;
    pthread_t tid;
    int len[CHANNEL_MAX];

    for(int i = 0; i < CHANNEL_MAX; i++)
    {
        size += strlen(channel[i].desc) + sizeof(struct List_channel);
        len[i] = strlen(channel[i].desc) + sizeof(struct List_channel);
    }


    list_channel = (struct List_channel *)malloc(size);

    struct List_channel *ptr = list_channel;

    for(int i = 0; i < CHANNEL_MAX; i++)
    {
        ptr->chnid = channel[i].chnid;
        ptr->len = len[i];
        strcpy(ptr->desc, channel[i].desc);
        ptr = (struct List_channel *)((char *)ptr + len[i]);
    }

    ptr = list_channel;

    for(int i = 0; i < CHANNEL_MAX; i++)
    {
        printf("%d\n", ptr->chnid);
        printf("%d\n", ptr->len);
        printf("%s\n", ptr->desc);

        ptr = (struct List_channel *)((char *)ptr + len[i]);
    }
    
    if( pthread_create(&tid, NULL, thr_handler,(void *)&addr) != 0)
        return -1;

    return 0;
}