//
// Created by Simon on 2020/10/11.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {

    int p[2];
    pipe(p);
    char val = 1;
    if (fork() == 0) {
        int pid = getpid();
        while (read(p[1], &val, sizeof val) <= 0);
        printf("%d:received ping\n", pid);
        exit();
    } else {
        int pid = getpid();
        while (write(p[0], &val, sizeof val) <= 0);
        while (read(p[1], &val, sizeof val) <= 0);
        printf("%d:received pong\n", pid);
        wait();
    }

    close(p[0]);
    close(p[1]);

    exit();
}