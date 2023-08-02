#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void parent_loop(int wfd);
void child_loop(int rfd);

int main(int argc, char *argv[]) {
    int fd[2] = {0};
    pid_t pid;

    if (pipe(fd) < 0) {
        printf("create pipe failed, errno: %d\n", errno);
        return -1;
    }
    
    pid = fork();
    if (pid == 0) {
        close(fd[1]);
        child_loop(fd[0]);
    } else {
        close(fd[0]);
        parent_loop(fd[1]);
    }
    return 0;
}

void parent_loop(int wfd) {
    static int cnt = 0;
    char buff[100];

    while (1) {
        snprintf(buff, sizeof(buff), "message_%d", cnt++);
        write(wfd, buff, sizeof(buff));
        printf("parent send: %s\n", buff);
        sleep(1);
    }
}

void child_loop(int rfd) {
    char buff[100];
    
    while (1) {
        memset(buff, 0, sizeof(buff));
        if (read(rfd, buff, sizeof(buff)) > 0) {
            printf("child  recv: %s\n", buff);
        }
    }
}
