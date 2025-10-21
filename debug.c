#include <stdio.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "debug.h"
#include "parser.h"


const command_table table_commands[] = {
    {"info",info,"help displaying data like functions/registers and more..."},
    {"break",breakpoint,"set breakpoint in adress you choose"},
    {"exit",exit_debugger,"exit from debugger"},
    {NULL,NULL,NULL}
};


int handle_command(char* command){
    int* argc = malloc(sizeof(int));
    char** commands = parse_command(command,argc);
    if(commands == NULL){
        return 0;
    }
    for(int i =0; table_commands[i].command!= NULL;i++){
        if(strcmp(commands[0],table_commands[i].command) == 0){
            table_commands[i].func_handler(*argc,commands);
            return 1;
        }
    }
    return 0;
}

int info(int argc,char** argv){
    printf("%s",argv[1]);
}
int breakpoint(int argc,char** argv){
    printf("%s",argv[1]);
}
int exit_debugger(int argc,char** argv){
    exit(0);
}



int debug_process(pid_t pid){
    char* input_command = malloc(sizeof(char)*INPUT_SIZE);
    int pid_status;
    int exit = 0;
    int res = waitpid(pid,&pid_status,0);
    printf("result : %d\n",res);
    if(WIFSTOPPED(pid_status)) {
            printf("Child stopped by signal %d\n", WSTOPSIG(pid_status));
            printf("ready to debug!\n");
    } 

    while(!exit){
        printf(">");
        fgets(input_command,INPUT_SIZE,stdin);
        handle_command(input_command);
    }
}


