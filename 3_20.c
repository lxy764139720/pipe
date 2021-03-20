//
// Created by lxy on 2021/3/20.
// 操作系统概念3.20题

#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>

#define PERMS 0666  // 文件访问权限：对所有者、所有者组和其他成员均可读写

void error(char *, ...);

void error(char *fmt, ...) {
    va_list args;   // 指向可变参数的指针

    va_start(args, fmt);    // 令指针指向fmt字符串
    fprintf(stderr, "error: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);   // 回收指针
    exit(1);
}

int main(int argc, char *argv[]) {
    int file1, file2, n;
    int filedes[2];
    char buf[BUFSIZ];
    pid_t pid;

    if (argc != 3) {
        error("Usage: cp from to");
    }

    pipe(filedes);
    pid = fork();
    if (pid > 0) {
        printf("This is in the father process, here read a file and write to the pipe.\n");
        if ((file1 = open(argv[1], O_RDONLY, 0)) == -1) {
            error("cp: can't open %s", argv[1]);
        }
        while ((n = read(file1, buf, BUFSIZ)) > 0) {    // 读取文件file1
            if (write(filedes[1], buf, n) != n) {   // 写入管道
                error("cp: write error on pipe");
            }
        }
        close(file1);
    } else if (pid == 0) {
        printf("This is the child process, here read from the pipe and write to a file.\n");
        if ((file2 = creat(argv[2], PERMS)) == -1) {    // 无论是否存在文件都新建一个文件
            error("cp: can't create %s, mode %03o", argv[2], PERMS);
        }
        // 阻塞直到父进程写入管道
        while ((n = read(filedes[0], buf, BUFSIZ)) > 0) {   // 从管道读取
            if (write(file2, buf, n) != n) {    // 写入file2
                error("cp: write error on file %s", argv[2]);
            }
        }
        close(file2);
    }

    waitpid(0, NULL, 0);    // 等待子进程结束
    close(filedes[0]);
    close(filedes[1]);
    return 0;
}