#ifndef __TOKEN_H__
#define __TOKEN_H__

#define BIT_RATE 320                /*歌曲的比特率*/
#define SIZE     (BIT_RATE*1000/8)  /*歌曲每秒的数据大小(字节)*/
#define CPS      SIZE               /*令牌值每秒累加值*/
#define BURST    (SIZE*3)           /*令牌值上限*/

void wait_token(void);
int token_init(void);

#endif
