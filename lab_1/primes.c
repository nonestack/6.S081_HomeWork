#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int childFunc(int *p){
    //fprintf(1, "hello\n");
    close(p[1]);    //pipe for communicating between parent and this process
    int first;
    int ret = read(p[0], &first, sizeof(int));
    //fprintf(1, "ret = %d, first = %d\n", ret, first);
    if(ret == 0){   //pipe has closed
        exit(0);
    }
    fprintf(1, "prime %d\n", first);
    int p2[2];      //create pipe for communicating between this process and its child
    pipe(p2);
    int fp = fork();
    if(fp == -1){
        fprintf(1, "error\n");
        exit(-1);
    }
    else if(fp == 0){
        childFunc(p2);
    }
    else{
        int num;
        close(p2[0]);
        //ret = ;
        //printf("ret = %d, num = %d\n", ret);
        while(0 != read(p[0], &num, sizeof(int))){      //read == 0 represent parent closing its pipe
            //printf("num = %d, first = %d\n", num, first);
            if(num % first != 0){
                write(p2[1], &num, sizeof(int));
            }
        }
        close(p2[1]);
        wait((int *)0);
    }
    exit(0);
}

int main(int argc, char *argv[]){
    int p[2];
    pipe(p);
    if(fork() == 0){
        childFunc(p);
    }
    else{
        close(p[0]);
        for(int i = 2; i <= 35; ++i){
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        wait((int *)0);
    }
    exit(0);
}