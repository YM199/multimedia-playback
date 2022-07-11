#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "channel.h"
#include <sys/stat.h>
#include <sys/types.h>

#define BUFSIZE 1024

int channel_num = 0; /*频道数目(不包含节目单)*/
int channel_max = 0; /*频道数目(包含节目单)*/

struct Channel *channel;      /*频道结构体数组，包含节目单*/
struct Media_channel **media; /*频道结构体数组，不包含节目单，用来网络传输*/

extern struct sockaddr_in dest_addr;

/**
 * @brief 填充频道结构体
 * 
 * @param path 频道的路径
 * @return struct Channel* 返回值是malloc的，需要free
 */
static struct Channel * fill_channel(const char *path)
{
    FILE *fp;
    struct Channel * me;
    static chnid_t currid = 1;
    char pathstr[BUFSIZE] = {0};
    char linebuf[BUFSIZE] = {0};

    strcpy(pathstr, path);
    strcat(pathstr, "/desc.txt");

    if((fp = fopen(pathstr, "r")) == NULL)
        return NULL;

    if(fgets(linebuf, BUFSIZE, fp) == NULL)
        return NULL;
    
    fclose(fp);

    if((me = malloc(sizeof(*me))) == NULL)
        return NULL;

    me->desc = strdup(linebuf);
    me->chnid = currid;
    currid++;

    return me;
}

/**
 * @brief 填充 Media_channel 结构体
 * 
 * @param path 频道路径
 * @return struct Media_channel* 返回值是malloc的，需要free
 */
struct Media_channel * fill_media_channel(const char *path)
{
    glob_t globres;
    static chnid_t currid = 1;
    struct Media_channel *me;
    char songpath[BUFSIZE] = {0};

    strcpy(songpath, path);
    strcat(songpath, "/*.mp3");

    if(glob(songpath, 0, NULL, &globres) != 0)
        return NULL;

    if((me = malloc(sizeof(*me))) == NULL)
        return NULL;

    me->chnid = currid;
    me->pos = 0;
    me->fd = open(globres.gl_pathv[me->pos], O_RDONLY);
    me->dest_addr = dest_addr;
    me->globres = globres;
    currid++;

    return me;
}

/**
 * @brief 填充频道结构体数组：channel和media
 * 
 * @return int 
 */
int fill_channel_array(void)
{   
    glob_t globres;
    struct Channel *res;
    char path[BUFSIZE] = {0};

    strcpy(path,"../../media/*");

    if(glob(path, 0, NULL, &globres) != 0)
        return -1;

    channel_num = globres.gl_pathc;
    channel_max = channel_num +1;

    channel = (struct Channel *)malloc(sizeof(struct Channel) * channel_max);
    media = (struct Media_channel **)malloc(sizeof(struct Media_channel *) * channel_num);
    for(int i = 0; i < channel_num; i++)
    {
        media[i] = (struct Media_channel *)malloc(sizeof(struct Media_channel));
    }

    channel[0].chnid  = 0;
    channel[0].desc = "这是节目单频道";

    for(int i = 0; i < channel_num; i++)
    {
        if((res = fill_channel(globres.gl_pathv[i])) != NULL)
        {
            channel[res->chnid] = *res;
        }
        media[i] = fill_media_channel(globres.gl_pathv[i]);
    }

    globfree(&globres);

    return 0;
}
