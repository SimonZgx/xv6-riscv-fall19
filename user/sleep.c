//
// Created by Simon on 2020/10/11.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(2, "usage: sleep [n] seconds\n");
        exit(1);
    }

    int n = atoi(argv[1]);
    sleep(n);
    exit(0);
}