#include <stdio.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <stdlib.h>
#include "commands.h"
#include "debug.h"
extern RUNNING_STATE proc_state;
extern pid_t debugee_pid;
int run_process(int argc,char** argv){
    printf("in run process\n");
}

int continue_proc(int argc,char** argv){
    if(proc_state == LOADED || proc_state == NOT_LOADED){
        printf("process is not running yet\n");
        return 0;
    }


    if(debugee_pid != -1){
        ptrace(PTRACE_CONT,debugee_pid,NULL,NULL);
        return 1;
    }

    
    return 0;
}

int breakpoint(int argc,char** argv){
    printf("in breakpoint\n");
}
int exit_debugger(int argc,char** argv){
    printf("in exit\n");
    exit(0);
}