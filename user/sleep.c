#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"



int
main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(2, "error message, unacceptable count of args.\n");
        exit(1);
    } 
    fprintf(0, argv[0]);
    int tickNum = atoi(argv[1]);
    sleep(tickNum);
    exit(0);
}
