#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char *argv[]) {
    if (argc != 1) {
        fprintf(2, "error argument count");
        exit(1);
    }
    int childPipe[2]; // p[0] -> in, p[1] -> out
    int parentPipe[2];
    pipe(childPipe);
    pipe(parentPipe);


    // 发送
    char buf[512];
    buf[0] = 'a';
    write(childPipe[1], buf, 1);

    if (fork() == 0) {
        int pid = getpid();
        close(childPipe[1]);
        int receivedBytesNum = read(childPipe[0], buf, 1);
        printf("%d: received ping\n", pid);
        write(parentPipe[1], buf, receivedBytesNum);
        exit(0);
    } else {
        read(parentPipe[0], buf, 1);
        close(parentPipe[1]);
        int pid = getpid();
        printf("%d: received pong\n", pid);
        exit(0);
    }
}