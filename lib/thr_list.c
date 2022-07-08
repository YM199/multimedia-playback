#include <stdio.h>
#include "channel.h"
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include "thr_list.h"
#include <string.h>

struct List_channel *list_channel;

void *thr_handler(void *arg)
{
    int sockfd = *((int *)arg);

    while(1)
    {
        
    }

    return NULL;
}

/**
 * @brief 准备待发送的数据和创建线程
 * 
 * @param sockfd 套接字
 * @return int 
 */
int thr_list_create(int *sockfd)
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
    
    if( pthread_create(&tid, NULL, thr_handler, (void *)sockfd) != 0)
        return -1;

    return 0;
}