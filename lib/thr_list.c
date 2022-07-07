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

int thr_list_create(int *sockfd)
{
    int size =0;
    int len[CHANNEL_NUM+1];


    for(int i = 0; i < CHANNEL_NUM + 1; i++)
    {
        size += strlen(channel[i].desc) + sizeof(chnid_t) + sizeof(int);
        len[i] = strlen(channel[i].desc) + sizeof(chnid_t) + sizeof(int);
    }


    list_channel = (struct List_channel *)malloc(size);

    struct List_channel *ptr = list_channel;

    for(int i = 0; i < CHANNEL_NUM + 1; i++)
    {
        ptr->chnid = channel[i].chnid;
        ptr->len = len[i];
        strcpy(ptr->desc, channel[i].desc);
        ptr = (struct List_channel *)((char *)ptr + len[i]);
    }

    ptr = list_channel;

    for(int i = 0; i < CHANNEL_NUM + 1; i++)
    {
        printf("%d\n", ptr->chnid);
        printf("%d\n", ptr->len);
        printf("%s\n", ptr->desc);

        ptr = (struct List_channel *)((char *)ptr + len[i]);
    }

    pthread_t tid;
    int ret;
    ret = pthread_create(&tid, NULL, thr_handler, (void *)sockfd);
    if( ret != 0)
    {
        perror("pthread_create");
    }

    return 0;
}