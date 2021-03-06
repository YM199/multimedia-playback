#include "token.h"
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include "server.h"
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "thr_channel.h"

static int next_song(struct Media_channel *me)
{
    close(me->fd);
    me->pos++;

    if(me->pos >= me->globres.gl_pathc)
        me->pos = 0;

    if((me->fd = open(me->globres.gl_pathv[me->pos], O_RDONLY)) < 0)
        return -1;

    return 0;
}

/**
 * @brief 发送歌曲数据
 * 
 * @param arg 媒体频道结构体
 * @return void* 
 */
static void *thr_channel_handle(void *arg)
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
        if(size > len)
        {
            return_token(me->chnid, size - len);
        }
        if(len < 0)
        {
            fprintf(stderr, " %s %d %s\n",__FILE__, __LINE__, strerror(errno));
            continue;
        }
        else if(len == 0)
        {
            next_song(me);
            offset = 0;
        }
        else
        {
            offset += len;
            if(sendto(sockse, me, SIZE+sizeof(chnid_t), 0, (struct sockaddr *)&addr, sizeof(addr)) < 0)
            {
                fprintf(stderr, " %s %d %s\n",__FILE__, __LINE__, strerror(errno));
            }
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