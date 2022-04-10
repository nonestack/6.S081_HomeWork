#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    int p[2];
    pipe(p);
    int fp = fork();
    if(fp == 0){    //this is child process
        char c = 127;
        read(p[1], &c, 1);
        fprintf(1, "%d: received ping\n", getpid());
        write(p[0], (const char  *)'t', 1);
        exit(0);
    }
    else{           //this is parent process
        write(p[0], (const char *)'a', 1);
        wait((int *)0);
        char c = 127;
        read(p[1], &c, 1);
        fprintf(1, "%d: received pong\n", getpid());
        exit(0);
    }
    exit(0);
}