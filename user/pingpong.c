//
// Created by Simon on 2020/10/11.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {

    int p[2][2];
    pipe(p[0]);
    pipe(p[1]);
    char val = 1;
    if (fork() == 0) {
        //child process
        int pid = getpid();
        close(p[0][1]);
        close(p[1][0]);
        read(p[0][0], &val, sizeof val);
        write(p[1][1], &val, sizeof val);
        close(p[1][1]);
        close(p[0][0]);
        printf("%d:received ping\n", pid);
        exit();
    } else {
        //parent process
        int pid = getpid();
        close(p[0][0]);
        close(p[1][1]);
        write(p[0][1], &val, sizeof val);
        read(p[1][0], &val, sizeof val);
        printf("%d:received pong\n", pid);
        close(p[1][0]);
        close(p[0][1]);
        wait();
    }

    exit();
}