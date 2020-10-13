//
// Created by Administrator on 2020/10/13.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


#define MAX_FILENAME_LENGTH 255

const char *filename(const char *path) {
    static char buf[MAX_FILENAME_LENGTH + 1];
    memset(buf, '\0', sizeof buf);
    const char *p;

    for (p = path + strlen(path); p >= path && *p != '/'; p--);
    p++;

    memmove(buf, p, strlen(p));
    return buf;
}

void find(const char *path, const char *target) {
    char buf[512], *p;
    memset(buf, '\0', sizeof buf);
    int fd;
    struct dirent de;
    struct stat st;
    int pathLen = strlen(path);
    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    printf("find %s %s\n", path, target);

    switch (st.type) {
        case T_FILE:
            if (strcmp(filename(path), target) == 0) {
                printf("%s\n", path);
            }
            break;
        case T_DIR:
            if (pathLen + 1 > sizeof buf) {
                printf("ls: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0) {
                    continue;
                }
                memset(p, '\0', sizeof(buf) - pathLen);
                if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
                    continue;
                }
                memmove(p, de.name, strlen(de.name));
                if (stat(buf, &st) < 0) {
                    printf("find: cannot stat %s\n", buf);
                    continue;
                }
                find(buf, target);
            }
            break;
        default:
            break;
    }
    close(fd);
}

int main(int argc, char *argv[]) {

    if (argc < 3) {
        fprintf(2, "usage: find [directory] [filename]\n");
        exit();
    }

    find(argv[1], argv[2]);
    return 0;
}
