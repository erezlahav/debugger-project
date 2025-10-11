#include <sys/ptrace.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



int main(int argc,char* argv[],char* envp[]){
    if(argc != 3){
        printf("Usage: %s -mode <pid>/<path>\n",argv[0]);
        exit(0);
    }

    if(strcmp(argv[1],"-run") == 0){
        printf("executing -run to process\n");
        pid_t child_pid = fork();
        if(child_pid ==0){
            //child code 
            
        }

        else{
            //parent(current process) code 
        }
    }


    else if(strcmp(argv[1],"-attach") == 0){
        printf("executing -attach to process\n");
    }


    else{
        printf("mode undefined, use -run / -attach\n");
    }

}
