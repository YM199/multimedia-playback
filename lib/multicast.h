#ifndef __MULTICAST_H__
#define __MULTICAST_H__

#include <glob.h>
#include <stdint.h>
#include <netinet/in.h>

#define MCAST_ADDR  "224.0.0.88"
#define CLIENT_PORT 8000
#define SERVER_PORT CLIENT_PORT

#define BIT_RATE 320                 /*歌曲的比特率*/
#define SIZE     (BIT_RATE*1000/8)   /*歌曲每秒的数据大小(字节)*/
#define CPS      SIZE                /*令牌值每秒累加值*/
#define BURST    (SIZE*3)            /*令牌值上限*/

#define MSG_LIST_MAX (65536 -20 -8)   /*最大UPD报的大小，20:IP包报头, 8:UDP包报头*/

typedef uint8_t chnid_t;

/*
 * 节目单频道结构体
 * 可以用在网络传输
*/
struct List_channel
{
    chnid_t chnid;                   /*频道号*/
    uint16_t len;                    /*结构体数组每个成员长度*/
    char desc[0];                    /*频道描述*/
}__attribute__ ((packed));

/*
 * 媒体频道结构体
 * 可以网络传输 
*/
struct Media_channel
{
    chnid_t chnid;                    /*频道号*/
    char data[SIZE];                  /*歌曲数据*/

    /*下面的数据在网络传输时不会发送，被截断*/
    int fd;                          /*歌曲的文件描述符*/
    int pos;                         /*歌曲标号*/
    glob_t globres;
    struct sockaddr_in dest_addr;    /*IP地址放在结构体里面是为了方便传参给子线程*/
}__attribute__ ((packed));

#endif