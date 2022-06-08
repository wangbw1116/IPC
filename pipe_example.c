#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void parent_loop(int rfd, int wfd);
void child_loop(int rfd, int wfd);

int main(int argc, char *argv[]) {
    int fd1[2] = {0}, fd2[2] = {0};
    if (pipe(fd1) < 0) {
        printf("pipe fd1 failed, errno: %d\n", errno);
        return -1;
    }
    if (pipe(fd2) < 0) {
        printf("pipe fd2 failed, errno: %d\n", errno);
        return -1;
    }
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        child_loop(fd1[0], fd2[1]);
    } else {
        parent_loop(fd2[0], fd1[1]);
    }
    return 0;
}

void parent_loop(int rfd, int wfd) {
    printf("parent process\n");
    int tmp;
    ssize_t bytes;
    while (1) {
        bytes = read(rfd, &tmp, sizeof(tmp));
        if (bytes > 0) {
            printf("parent_loop recv: %d\n", tmp);
            tmp++;
            write(wfd, &tmp, sizeof(tmp));
        }
        sleep(1);
    }
}

void child_loop(int rfd, int wfd) {
    printf("child process\n");
    int tmp = 0;
    write(wfd, &tmp, sizeof(tmp));
    ssize_t bytes;
    while (1) {
        bytes = read(rfd, &tmp, sizeof(tmp));
        if (bytes > 0) {
            printf("child_loop recv: %d\n", tmp);
            tmp++;
            write(wfd, &tmp, sizeof(tmp));
        }
        sleep(1);
    }
}