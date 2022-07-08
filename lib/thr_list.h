#ifndef __THR_LIST_H__
#define __THR_LIST_H__

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

int thr_list_create(int *sockfd);

#endif
