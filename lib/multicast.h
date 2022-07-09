#ifndef __MULTICAST_H__
#define __MULTICAST_H__

#include <glob.h>
#include <stdint.h>
#include <netinet/in.h>

#define MCAST_ADDR  "224.0.0.88"
#define CLIENT_PORT 8000
#define SERVER_PORT CLIENT_PORT

#define BIT_RATE 320                /*歌曲的比特率*/
#define SIZE     (BIT_RATE*1000/8)  /*歌曲每秒的数据大小(字节)*/
#define CPS      SIZE               /*令牌值每秒累加值*/
#define BURST    (SIZE*3)           /*令牌值上限*/

#define CHANNEL_NUM 2               /*频道数目(不包含节目单)*/
#define CHANNEL_MAX (CHANNEL_NUM+1) /*频道数目(包含节目单)*/

#define MSG_LIST_MAX (65536 -20 -8)

typedef uint8_t chnid_t;

/*
 * 频道结构体
 * 可以用在网络传输
*/
struct List_channel
{
    chnid_t chnid; /**频道号*/
    int len;       /*结构体数组每个成员长度*/
    char desc[0];  /*频道描述*/
}__attribute__ ((packed));

struct Media_channel
{
    chnid_t chnid; /**/
    char data[SIZE];
    
    int fd;
    int pos;
    glob_t globres;
    struct sockaddr_in dest_addr;
    
}__attribute__ ((packed));

#endif