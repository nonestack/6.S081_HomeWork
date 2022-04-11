#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"

#include "user/user.h"

char *strcat(char *dest, char *src){
    int cur = strlen(dest);
    char *p = src;
    //printf("strcat: dest = %s, src = %s\n", dest, src);
    while(*p != 0){
        //printf("%d\n", *p);
        dest[cur++] = *p++;
    }
    //printf("strcat: dest = %s\n", dest);
    dest[cur] = 0;
    //printf("strcat: dest = %s\n", dest);
    return dest;
}

int find(char *dir, char *filename){
    int fd;
    struct stat st;
    struct dirent dt;
    char buf[512];
    if((fd = open(dir, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", dir);
        return -1;
    }
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", dir);
        return -1;
    }
    if(st.type == T_DIR){
        if(strlen(dir) + 1 + DIRSIZ + 1 > sizeof(buf)){
            printf("find: directory is too long\n");
        }
        else{
            while(read(fd, &dt, sizeof(dt)) == sizeof(dt)){
                if(dt.inum == 0){
                    continue;
                }
                if((strcmp(dt.name, ".") == 0) || (strcmp(dt.name, "..") == 0)){
                    continue;
                }
                if(strcmp(dt.name, filename) == 0){
                    fprintf(1, "%s/%s\n", dir, filename);
                }
                else{
                    strcpy(buf, dir);
                    strcat(buf, "/");
                    strcat(buf, dt.name);
                    find(buf, filename);
                }
            }
        }
    }
    close(fd);
    return 0;
}

int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(1, "usage: find [directory] filename");
        exit(-1);
    }
    if(argc == 2){
        find(".", argv[1]);
    }
    else{
        //printf("I'm here\n");
        find(argv[1], argv[2]);
    }
    exit(0);
}