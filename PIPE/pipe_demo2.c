#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void child1_loop(int wfd);
void child2_loop(int rfd);

int main(int argc, char *argv[]) {
    int fd[2] = {0};
    pid_t pid1, pid2;

    if (pipe(fd) < 0) {
        printf("create pipe failed, errno: %d\n", errno);
        return -1;
    }
    
    pid1 = fork();
    if (pid1 == 0) {
        close(fd[0]);
        child1_loop(fd[1]);
    } else {
        pid2 = fork();
        if (pid2 == 0) {
            close(fd[1]);
            child2_loop(fd[0]);
        } else {
            while (1) {
                sleep(1);
            }
        }
    }
    return 0;
}

void child1_loop(int wfd) {
    static int cnt = 0;
    char buff[100];

    while (1) {
        snprintf(buff, sizeof(buff), "message_%d", cnt++);
        write(wfd, buff, sizeof(buff));
        printf("child1 send: %s\n", buff);
        sleep(1);
    }
}

void child2_loop(int rfd) {
    char buff[100];
    
    while (1) {
        memset(buff, 0, sizeof(buff));
        if (read(rfd, buff, sizeof(buff)) > 0) {
            printf("child2 recv: %s\n", buff);
        }
    }
}
