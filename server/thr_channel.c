#include "token.h"
#include <stdio.h>
#include <errno.h>
#include "server.h"
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "thr_channel.h"

/**
 * @brief 发送歌曲数据
 * 
 * @param arg 媒体频道结构体
 * @return void* 
 */
void *thr_channel_handle(void *arg)
{
    struct Media_channel *me = (struct Media_channel *)arg;
    struct sockaddr_in addr = me->dest_addr;
    int size;
    int offset = 0;
    int len;
    while(1)
    {
        size = get_token(me->chnid,SIZE);
        len = pread(me->fd , me->data, size, offset);
        if(len < 0)
        {
            fprintf(stderr, " %s %d %s\n",__FILE__, __LINE__, strerror(errno));
        }
        else if(len == 0)
        {
            break; /*歌曲播放完毕*/
        }
        else
        {
            offset += sizeof(me->data);
            if(sendto(sockse, me, SIZE+sizeof(chnid_t), 0, (struct sockaddr *)&addr, sizeof(addr)) < 0)
            {
                fprintf(stderr, " %s %d %s\n",__FILE__, __LINE__, strerror(errno));
            }
            printf("%d\n\n", me->data[50]);
        }
    }
}

/**
 * @brief 创建频道线程
 * 
 * @param arg 媒体频道结构体数组
 * @return int 
 */
int thr_channel_create(struct Media_channel **arg)
{
    int ret;
    pthread_t tid;
    static int i = 0;

    if(ret = pthread_create(&tid, NULL, thr_channel_handle, arg[i]) != 0)
        return -1;
    i++;

    return 0;
}