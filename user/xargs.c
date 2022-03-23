#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

#define MAX_LINE 1024
#define MAX_LEN 1024

int main(int argc, char *argv[]) {
    char *params[MAXARG]; // xargs后命令的相关参数
    int p_offset = argc - 1;
    char br; // 从标准输入读取的字符
    char *param = malloc(MAX_LEN); // 从标准输入读取的参数
    int cursor = 0; // 已读取的字母数量

    for (int i = 1; i < argc; i++) {
        params[i-1] = argv[i];
    }

    // 每读取到`\n`执行一次命令
    while (1) {
        int read_result;
        while ((read_result = read(0, &br, 1)) > 0 && br != '\n') {
            if (br != ' ') {
                param[cursor++] = br;
            } else {
                param[cursor] = '0';
                // strcpy(params[p_offset++], param); // 添加一个参数
                params[p_offset++] = param;
                cursor = 0;
            }
        }

        if (read_result == 0) {
            break;
        }

        if (br == '\n') {
            param[cursor] = 0;
            // strcpy(params[p_offset++], param);
            params[p_offset++] = param;
            cursor = 0;
            // 执行命令
            if (fork() == 0) {
                exec(params[0], params);
                exit(0);
            } else {
                wait(0);
                // 初始化相关的参数们
                for (int i = argc - 1; i < p_offset; i++) {
                    strcpy(params[i], 0);
                }
                p_offset = argc - 1;
            }
        }
        
    }
    exit(0);
}
