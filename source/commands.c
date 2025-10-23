#include <stdio.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <stdlib.h>
#include "commands.h"



int run_process(int argc,char** argv){
    printf("in run process\n");
}

int continue_proc(int argc,char** argv){
    printf("in continue process\n");
    //ptrace(PTRACE_CONT,pid,NULL,NULL);
}

int breakpoint(int argc,char** argv){
    printf("in breakpoint\n");
}
int exit_debugger(int argc,char** argv){
    printf("in exit\n");
    exit(0);
}