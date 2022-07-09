#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "channel.h"
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define PATHSIZE 1024
#define LINEBUFSIZE 1024
#define SONGPATHSIZE 1024

struct Channel channel[CHANNEL_MAX]; /*频道结构体数组，包含节目单*/
struct Media_channel *media[CHANNEL_NUM];

extern struct sockaddr_in dest_addr;

/**
 * @brief 填充频道结构体
 * 
 * @param path 频道的路径
 * @return struct Channel* 
 */
static struct Channel * fill_channel(const char *path)
{
    FILE *fp;

    struct Channel * me;
    static chnid_t currid = 1;

    char pathstr[LINEBUFSIZE] = {0};
    char linebuf[LINEBUFSIZE] = {0};

    strcpy(pathstr, path);
    strcat(pathstr, "/desc.txt");

    if((fp = fopen(pathstr, "r")) == NULL)
        return NULL;

    if(fgets(linebuf, LINEBUFSIZE, fp) == NULL)
        return NULL;
    
    fclose(fp);

    if((me = malloc(sizeof(*me))) == NULL)
        return NULL;

    me->desc = strdup(linebuf);
    me->chnid = currid;
    currid++;

    return me;
}

struct Media_channel * fill_media_channel(const char *path)
{
    glob_t globres;

    static chnid_t currid = 1;
    struct Media_channel *me;



    char songpath[SONGPATHSIZE];

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
 * @brief 填充频道结构体数组
 * 
 * @return int 
 */
int fill_channel_array(void)
{   
    glob_t globres;

    struct Channel *res;

    char path[PATHSIZE] = {0};

    for(int i = 0; i < CHANNEL_MAX; i++)
    {
        channel[i].chnid = -1;
    }

    strcpy(path,"../../media/*");

    if(glob(path, 0, NULL, &globres) != 0)
        return -1;

    channel[0].chnid  = 0;
    channel[0].desc = "这是节目单频道";

    for(int i = 0; i < globres.gl_pathc; i++)
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
