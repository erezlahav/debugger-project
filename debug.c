#include <stdio.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "debug.h"

int debug_process(pid_t pid){
    char buf[40];
    int pid_status;
    int res = waitpid(pid,&pid_status,0);
    printf("result : %d\n",res);
    if(WIFSTOPPED(pid_status)) {
            printf("Child stopped by signal %d\n", WSTOPSIG(pid_status));
            printf("ready to debug!\n");
            printf(">");
            scanf("%s",buf);
    } 
}
