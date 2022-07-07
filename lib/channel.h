#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include <stdint.h>
#include <glob.h>

#define CHANNEL_NUM 2

typedef uint8_t chnid_t;



/*频道结构体*/
struct Channel
{
    chnid_t chnid; /*频道号*/
    char *desc; /*频道描述*/
};



extern struct Channel channel[CHANNEL_NUM+1];

int fill_channel_array(void);

#endif