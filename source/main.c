#include <sys/ptrace.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "debug.h"
RUNNING_STATE proc_state = NOT_LOADED;
pid_t debugee_pid = -1;
int main(int argc,char* argv[],char* envp[]){
    

    if(argc != 3){
        printf("Usage: %s -mode <pid>/<path>\n",argv[0]);
        exit(0);
    }

    if(strcmp(argv[1],"-run") == 0){
        printf("executing -run to process\n");
        char executble_path[50];
        strncpy(executble_path,argv[2],sizeof(executble_path));


        debug_process(executble_path);
        /*
        int status;
        waitpid(child_pid, &status, 0);
        if (WIFSTOPPED(status)) {
            printf("The program is stopped, signal: %d\n", WSTOPSIG(status));
        }
        if (WIFEXITED(status)) {
            printf("The program exited with %d\n", WEXITSTATUS(status));
        }
        */
    }


    else if(strcmp(argv[1],"-attach") == 0){
        printf("executing -attach to process\n");
    }


    else{
        printf("mode undefined, use -run / -attach\n");
    }

}
