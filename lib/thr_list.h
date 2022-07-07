#ifndef __THR_LIST_H__
#define __THR_LIST_H__

struct List_channel
{
    chnid_t chnid;
    int len;
    char desc[0];
}__attribute__ ((packed));

int thr_list_create(int *sockfd);

#endif
