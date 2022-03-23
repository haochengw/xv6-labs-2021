#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "user/user.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
//   memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  buf[strlen(p)] = 0;
  return buf;
}

// 两个参数为两个指向char的指针
void find(char *path, char *target) {

    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    fd = open(path, O_RDONLY);
    if (fd < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        exit(1);
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        exit(1);
    }

    switch(st.type) {
    case T_FILE:
        if (strcmp(fmtname(path), target) == 0) {
            printf("%s\n", path);
        }
        break;
    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/'; // p后接文件名和一个'/'
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0) { // inum = 0是什么意思?
                continue;
            }
            memmove(p, de.name, DIRSIZ); 
            p[DIRSIZ] = 0; // 标识符，表示字符串结尾
            if (strcmp(de.name, ".") && strcmp(de.name, "..")) {
                find(buf, target);
            }
        }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        find(".", argv[1]);
    } else if (argc == 3) {
        find(argv[1], argv[2]);
    } else {
        printf("usage: find <path> <name>\n");
        exit(1);
    }
    exit(0);
}


