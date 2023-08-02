#include <stdio.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

static int pid;

void *send_task(void *arg);
void *recv_task(void *arg);

int main(int argc, char *argv[])
{
    mqd_t mqd_recv, mqd_send;
    char mq_recv_name[100], mq_send_name[100];
    char msg_recv[100], msg_send[100];
    unsigned int prio_recv, prio_send = 1;
    int cnt = 0;
    pthread_t tid_recv, tid_send;
    int ret;

    if (argc < 3) {
        printf("format wrong. eg: ./posix_mq_demo [recv mq name] [send mq name]\n");
        return -1;
    }

    pid = getpid();

    snprintf(mq_recv_name, sizeof(mq_recv_name), "/%s", argv[1]);
    mqd_recv = mq_open(mq_recv_name,
                  O_RDWR | O_CREAT,
                  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH,
                  NULL);
    if (mqd_recv < 0) {
        printf("mq_open recv mqueue[%s] faild, errno: %d\n", mq_recv_name, errno);
        return -2;
    }

    snprintf(mq_send_name, sizeof(mq_send_name), "/%s", argv[2]);
    mqd_send = mq_open(mq_send_name,
                  O_RDWR | O_CREAT,
                  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH,
                  NULL);
    if (mqd_send < 0) {
        printf("mq_open send mqueue[%s] faild, errno: %d\n", mq_send_name, errno);
        return -2;
    }

    ret = pthread_create(&tid_recv, NULL, recv_task, &mqd_recv);
    if (ret < 0) {
        printf("pthread_create faild, errno: %d\n", errno);
        return -3;
    }

    ret = pthread_create(&tid_send, NULL, send_task, &mqd_send);
    if (ret < 0) {
        printf("pthread_create faild, errno: %d\n", errno);
        return -3;
    }

    pthread_join(tid_recv, NULL);
    pthread_join(tid_send, NULL);
    
    return 0;
}

void *recv_task(void *arg)
{
    mqd_t mqd = *((mqd_t *)arg);
    char msg[8192];
    unsigned int prio;
    ssize_t bytes;
    struct timespec timeout;

    while (1) {
        bytes = mq_receive(mqd, msg, sizeof(msg), &prio);
        // timeout.tv_sec = 1000000;
        // timeout.tv_nsec = 0;
        // bytes = mq_timedreceive(mqd, msg, sizeof(msg), &prio, &timeout);
        // printf("************ pid_%d recv task, bytes: %ld, errno: %d\n", pid, bytes, errno);
        if (bytes > 0) {
            printf("Process_%d recv message: %s\n", pid, msg);
            continue;
        }
        // sleep(1);
    }
    return NULL;
}

void *send_task(void *arg)
{
    mqd_t mqd = *((mqd_t *)arg);
    char msg[8192];
    static int cnt = 0;

    while (1) {
        snprintf(msg, sizeof(msg), "message_%d", cnt++);
        printf("Process_%d send message: %s\n", pid, msg);
        mq_send(mqd, msg, sizeof(msg), 1);
        sleep(1);
    }
    return NULL;
}