# client

当出现

```bash
mplayer: could not connect to socket
mplayer: No such file or directory
Failed to open LIRC support. You will not be able to use your remote control.
```

错误时修改mplayer即可

```bash
vim ~/.mplayer/config 
加入
lirc=no
```


出现

```bash
Cannot seek backward in linear streams!
Seek failed
```

应该是和mplayer这条程序有关，和我写的代码无关
