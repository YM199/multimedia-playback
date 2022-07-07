#include <stdio.h>
#include <stdlib.h>
#include "channel.h"
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define PATHSIZE 1024
#define LINEBUFSIZE 1024

struct Channel channel[CHANNEL_NUM+1];

static struct Channel * path2entry(const char *path)
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
        
    
    fclose(fp);

    if((me = malloc(sizeof(*me))) == NULL)
        return NULL;

    me->desc = strdup(linebuf);
    me->chnid = currid;
    currid++;

    return me;
}

int mlib_getchnlist(struct Channel **result)
{   
    glob_t globres;

    struct Channel *ptr;
    struct Channel *res;

    char path[PATHSIZE] = {0};

    for(int i = 0; i < CHANNEL_NUM + 1; i++)
    {
        channel[i].chnid = -1;
    }

    strcpy(path,"../../media/*");

    if(glob(path, 0, NULL, &globres) != 0)
        return -1;

    if((ptr = malloc(sizeof(struct Channel) * globres.gl_pathc)) == NULL)
        perror("malloc\n");

    for(int i = 0; i < globres.gl_pathc; i++)
    {
        if((res = path2entry(globres.gl_pathv[i])) != NULL)
        {
            channel[res->chnid] = *res;
            ptr[i].chnid = res->chnid;
            ptr[i].desc = strdup(res->desc);
        }
    }

    *result = ptr;

    return 0;
}





void destory(void)
{
    //globfree(&gl);
}