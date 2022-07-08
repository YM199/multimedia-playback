#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include <stdint.h>
#include <glob.h>

#define CHANNEL_NUM 2               /*频道数目(不包含节目单)*/
#define CHANNEL_MAX (CHANNEL_NUM+1) /*频道数目(包含节目单)*/

typedef uint8_t chnid_t;

/*
 * 频道结构体
 * 该结构体用到了指针，无法网络传输
*/
struct Channel
{
    chnid_t chnid; /*频道号*/
    char *desc; /*频道描述*/
};



extern struct Channel channel[CHANNEL_MAX];

int fill_channel_array(void);

#endif