#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define STDDER_FILENO 2
#define READEND 0
#define WRITEEND 1
#define MAXN 35

void 
seive(int p[]) {
    int n;
    int fd[2];
    close(p[WRITEEND]);
    if (read(p[READEND], &n, sizeof(int)) != 0) {
        pipe(fd);
        if (fork() == 0) {
            seive(fd);
        } else {
            close(fd[READEND]);
            int prime = n;
            printf("prime %d \n", n);
            while (read(p[READEND], &n, sizeof(int)) != 0) {
                if (n % prime != 0) {
                    write(fd[WRITEEND], &n, sizeof(int));
                }
            }
            close(p[READEND]);
            close(fd[WRITEEND]);
            wait((int *) 0);
            exit(0);
        }
    } else {
        close(p[READEND]);
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    int p[2];
    pipe(p);
    if (fork() == 0) 
        seive(p);
    else {
        close(p[READEND]);
        for (int i = 2; i <= 35; i++) {
            write(p[WRITEEND], &i, sizeof(int));
        }
        close(p[WRITEEND]);
        wait((int *) 0);
    }
    exit(0);
}
