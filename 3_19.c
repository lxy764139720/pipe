//
// Created by lxy on 2021/3/20.
// 操作系统概念3.19题

#include <unistd.h>
#include <stdio.h>
#include <wait.h>

int main(void) {
    int filedes[2]; // 储存文件描述符
    char buf[80];
    pid_t pid;

    pipe(filedes);  // 建立管道，filedes[0]为读取端，filedes[1]为输入端
    pid = fork();   // 创建子进程
    if (pid > 0) {
        printf("This is in the father process,here write a string to the pipe.\n");
        char s[] = "Hi There\n";
        printf("Original string: %s\n", s);
        write(filedes[1], s, sizeof(s));
        sleep(1);
        printf("This is in the father process,here read a string from the pipe.\n");
        read(filedes[0], s, sizeof(s));
        printf("String read from the pipe: %s\n", s);
        close(filedes[0]);
        close(filedes[1]);
    } else if (pid == 0) {
        printf("This is in the child process,here read a string from the pipe.\n");
        read(filedes[0], buf, sizeof(buf));
        printf("String read from the pipe: %s\n", buf);
        for (int index = 0; index < 80; ++index) {
            if ('A' <= buf[index] && buf[index] <= 'Z') {
                buf[index] += 32;
            } else if ('a' <= buf[index] && buf[index] <= 'z') {
                buf[index] -= 32;
            }
        }
        printf("String write back to the pipe: %s\n", buf);
        write(filedes[1], buf, sizeof(buf));
        close(filedes[0]);
        close(filedes[1]);
    }

    waitpid(pid, NULL, 0);  // 等待子进程结束
    return 0;
}