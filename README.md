# multimedia-playback

基于UDP组播的音乐播放

## 歌曲比特率

比特率：每秒播放（传输）的音频数据大小，320比特率就是320kpbs，就是每秒播放32000比特大小的音频。

所以说每s需要发送32000/8字节的数据给客户端。

查看歌曲比特率：`mplayer -identify  歌曲`，查看 ID_AUDIO_BITRATE

## TODO

struct Channel channel[CHANNEL_NUM+1];的每一个成员都是 malloc 的。

需要调用free释放

clien.c中的    list_channel = (struct List_channel *)malloc(MSG_LIST_MAX); 记得free
