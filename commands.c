#include <stdio.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <stdlib.h>
#include "commands.h"



int continue_proc(int argc,char** argv,pid_t pid){
    ptrace(PTRACE_CONT,pid,NULL,NULL);
    printf("process continue\n");
}

int breakpoint(int argc,char** argv,pid_t pid){
    printf("%s",argv[1]);
}
int exit_debugger(int argc,char** argv, pid_t pid){
    exit(0);
}