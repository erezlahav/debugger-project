#include <sys/ptrace.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "debug.h"

int main(int argc,char* argv[],char* envp[]){
    if(argc != 3){
        printf("Usage: %s -mode <pid>/<path>\n",argv[0]);
        exit(0);
    }

    if(strcmp(argv[1],"-run") == 0){
        printf("executing -run to process\n");
        char executble_path[50];
        strncpy(executble_path,argv[2],sizeof(executble_path));

        pid_t child_pid = fork();
        

        if(child_pid ==0){
            //child code 

            ptrace(PTRACE_TRACEME,child_pid,NULL,NULL);
         
            int exceve_res = execlp(executble_path,executble_path,NULL);
            if(exceve_res == -1){
                printf("failed execlp : %s\n",strerror(errno));
                exit(0);
            }
            
        }

        else{
            //parent(current process) code 
            debug_process(child_pid);
            

        }


    }


    else if(strcmp(argv[1],"-attach") == 0){
        printf("executing -attach to process\n");
    }


    else{
        printf("mode undefined, use -run / -attach\n");
    }

}
