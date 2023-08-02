#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void parent_loop(int rfd, int wfd);
void child_loop(int rfd, int wfd);

int main(int argc, char *argv[]) {
    int fd1[2] = {0}, fd2[2] = {0};
    pid_t pid;

    if (pipe(fd1) < 0 || pipe(fd2) < 0) {
        printf("create pipe failed, errno: %d\n", errno);
        return -1;
    }
    
    pid = fork();
    if (pid == 0) {
        close(fd1[1]);//关闭不需要的写端
        close(fd2[0]);//关闭不需要的读端
        child_loop(fd1[0], fd2[1]);
    } else {
        close(fd1[0]);//关闭不需要的读端
        close(fd2[1]);//关闭不需要的写端
        parent_loop(fd2[0], fd1[1]);
    }
    return 0;
}

void parent_loop(int rfd, int wfd) {
    static int cnt = 0;
    char buff[100];

    while (1) {
        memset(buff, 0, sizeof(buff));
        if (read(rfd, buff, sizeof(buff)) > 0) {
            printf("parent recv: %s\n", buff);
        }

        snprintf(buff, sizeof(buff), "parent_message_%d", cnt++);
        write(wfd, buff, sizeof(buff));
        sleep(1);
    }
}

void child_loop(int rfd, int wfd) {
    static int cnt = 0;
    char buff[100];

    while (1) {
        snprintf(buff, sizeof(buff), "child_message_%d", cnt++);
        write(wfd, buff, sizeof(buff));

        memset(buff, 0, sizeof(buff));
        if (read(rfd, buff, sizeof(buff)) > 0) {
            printf("child  recv: %s\n", buff);
        }
        sleep(1);
    }
}