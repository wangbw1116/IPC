# Pipe

```c
#include <unistd.h>

int pipe(int pipefd[2]);
int pipe2(int pipefd[2], int flags);
```

创建一个用于进程间通信的单向通道，pipefd返回两个文件描述符，分别指向读端和写端。在写端写入的数据会被内核缓存，直到在读端被读取。

pipe是单向通道，只能在读端读，在写端写。

pipe2 flags取值：

| flags      | 说明                                                                    |
| ---------- | ----------------------------------------------------------------------- |
| 0          | pipe2与pipe相同                                                         |
| O_CLOEXEC  |                                                                         |
| O_DIRECT   | I/O表现为“包"模式，每次write会被处理为一个单独的包，每次read读取一个包 |
| O_NONBLOCK | 设置为非阻塞                                                            |

示例：

1.pipe_demo1为父子进程之间通信示例；

2.pipe_demo2为兄弟进程之间通信示例；

3.pipe_demo3为父子进程间双向通信示例（打开两个管道)；
