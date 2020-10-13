//
// Created by Administrator on 2020/10/12.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void filter(int pipeFd) {

    int p[2];
    if (pipe(p) < 0) {
        printf("create pipe error. pid:%d\n", getpid());
        exit();
    }

    int val, filterNum = 0, childNum = 0;
    while (read(pipeFd, &val, sizeof val) > 0) {
        if (!filterNum) {
            filterNum = val;
            continue;
        }
        if (val % filterNum != 0) {
            if (!childNum) {
                if (fork() == 0) {
                    close(p[1]);
                    filter(p[0]);
                    close(p[0]);
                    return;
                } else {
                    close(p[0]);
                    childNum++;
                }
            }
            write(p[1], &val, sizeof val);
        }
    }

    close(p[0]);
    close(pipeFd);
    printf("prime %d\n", filterNum);
}

int main() {

    int p[2];
    if (pipe(p) < 0) {
        printf("create pipe error\n");
        exit();
    }

    if (fork() == 0) {
        //first child process
        close(p[1]);
        filter(p[0]);
    } else {
        close(p[0]);
        for (int i = 2; i < 35; ++i) {
            write(p[1], &i, sizeof i);
        }
        close(p[1]);
    }
    exit();
}
