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
}__attribute__ ((packed));

/*节目单结构体*/
struct List
{
    chnid_t chnid; /*频道号*/
    struct Channel my_channel[0];
}__attribute__ ((packed));

int mlib_getchnlist(struct Channel **result);

#endif