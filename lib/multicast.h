#ifndef __MULTICAST_H__
#define __MULTICAST_H__

#define MCAST_ADDR  "224.0.0.88"
#define CLIENT_PORT 8000
#define SERVER_PORT CLIENT_PORT

#define BIT_RATE 320                /*歌曲的比特率*/
#define SIZE     (BIT_RATE*1000/8)  /*歌曲每秒的数据大小(字节)*/
#define CPS      SIZE               /*令牌值每秒累加值*/
#define BURST    (SIZE*3)           /*令牌值上限*/

void server_multicast(void);
void client_multicast(void);
#endif