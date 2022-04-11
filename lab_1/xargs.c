#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"

#include "user/user.h"

#define MAXLINE 1024

int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(2, "usage: xargs command [argument...]\n");
        exit(-1);
    }
    //for(int i = 0; i < argc; ++i){
    //    printf("No.%d argument = %s\n", i + 1, argv[i]);
    //}
    //int p[2];
    //pipe(p);
    if(fork() == 0){
        //close(0);
        //dup(p[0]);
        //close(p[0]);
        //close(p[1]);
        
        char *myArgv[MAXARG];
        for(int i = 1; i < argc; ++i){
            myArgv[i - 1] = argv[i];
        }
        char *buf = (char *)malloc(512 * sizeof(char));
        char line[MAXLINE];
        char *pipeInput[MAXARG];
        int pIndex = 0;
        //char c;
        int bufIndex = 0;
        int ret;
        while(1){
            ret = read(0, line, MAXLINE);
            if(ret < 0){
                fprintf(2, "read err\n");
                exit(-1);
            }
            if(ret == 0){
                //printf("end while\n");
                break;
            }
            //printf("%s\n", line);
            //if(c == '\n'){
            //    pipeInput[pIndex++] = buf;
            //    buf = (char *)malloc(512 * sizeof(char));
            //    memset(buf, 0, sizeof(buf));
            //    bufIndex = 0;
            //}
            //else{
            //    buf[bufIndex++] = c;
            //}
            for(int i = 0; i < ret; ++i){
                if(line[i] == '\n'){
                    buf[bufIndex] = 0;
                    pipeInput[pIndex++] = buf;
                    buf = (char *)malloc(512 * sizeof(char));
                    memset(buf, 0, sizeof(buf));
                    bufIndex = 0;
                }
                else{
                    buf[bufIndex++] = line[i];
                }
            }
        }
        //printf("ok\n");
        //for(int i = 0; i < pIndex; ++i){
        //    printf("%s\n", pipeInput[i]);
        //}
        //printf("pIndex = %d\n", pIndex);
        for(int i = 0; i < pIndex; ++i){
            myArgv[argc - 1] = pipeInput[i];
            myArgv[argc] = 0;
            //for(int i = 0; i < argc + 1; ++i){
            //    printf("%s\t", myArgv[i]);
            //}
            //printf("\n");
            if(fork() == 0){
                exec(myArgv[0], myArgv);
            }
            wait((int *)0);
        }
    }
    else{
        //close(p[0]);
        //close(p[1]);
        wait((int *)0);
    }
    exit(0);
}