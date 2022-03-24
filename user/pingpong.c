#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

# define READEND 0
# define WRITEEND 1

int main(int argc, char *argv[]) {
    char buf[1];
    buf[0] = 'a';

    int childPipe[2]; 
    int parentPipe[2];
    pipe(childPipe);
    pipe(parentPipe);

    if (fork() == 0) {
        close(childPipe[WRITEEND]);
        close(parentPipe[READEND]);
        read(childPipe[READEND], buf, 1);
        printf("%d: received ping\n", getpid());
        close(childPipe[READEND]);
        write(parentPipe[WRITEEND], buf, 1);
        close(parentPipe[WRITEEND]);
    } else { // 父进程，发送数据后读取数据
        close(parentPipe[WRITEEND]);
        close(childPipe[READEND]);
        write(childPipe[WRITEEND], buf, 1);
        close(childPipe[WRITEEND]);
        read(parentPipe[READEND], buf, 1);
        close(parentPipe[READEND]);
        printf("%d: received pong\n", getpid());
    }
    exit(0);
}