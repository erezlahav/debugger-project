#include <stdio.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <stdlib.h>
#include "commands.h"
#include "debug.h"

extern debugee_process process_to_debug;

int run_process(int argc,char** argv){
    process_to_debug.pid = fork();
    if(process_to_debug.pid == 0){
        ptrace(PTRACE_TRACEME,process_to_debug.pid,NULL,NULL);
        int res = execlp(process_to_debug.elf_path,process_to_debug.elf_path,NULL);
        if(res == -1){
            printf("execlp failed\n");
        }


    }

    else{
        int status;
        waitpid(process_to_debug.pid,&status,NULL);
        if(WIFSTOPPED(status)){
            printf("process stopped!\n");
            process_to_debug.proc_state = STOPPED;
        }
    }
    
}

int continue_proc(int argc,char** argv){
    if(process_to_debug.proc_state == LOADED || process_to_debug.proc_state == NOT_LOADED){
        printf("process is not running yet\n");
        return 0;
    }


    if(process_to_debug.pid != -1){
        ptrace(PTRACE_CONT,process_to_debug.pid,NULL,NULL);
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