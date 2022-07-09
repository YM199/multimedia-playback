#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include <glob.h>
#include <stdint.h>
#include <multicast.h>

/*
 * 频道结构体
 * 该结构体用到了指针，无法网络传输
 * 作用可以理解是频道描述的临时存放点
*/
struct Channel
{
    chnid_t chnid; /*频道号*/
    char *desc; /*频道描述*/
};

extern struct Channel channel[CHANNEL_MAX];
extern struct Media_channel *media[CHANNEL_NUM];

int fill_channel_array(void);

#endif