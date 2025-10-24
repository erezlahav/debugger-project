#include <stdio.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>


#include "debug.h"
#include "parser.h"
#include "info_commands.h"
#include "commands.h"



const command_table table_commands[] = {
    {"info",info,"help displaying data like functions/registers and more..."},
    {"break",set_breakpoint,"set breakpoint in adress you choose"},
    {"exit",exit_debugger,"exit from debugger"},
    {"c",continue_proc,"continue the execution of the process"},
    {"r",run_process,"run the current process after breaking on stop signal before main"},
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


int debug_process(char* elf_path){
    char* input_command = malloc(sizeof(char)*INPUT_SIZE);
    int exit = 0;

    while(!exit){
        printf(">");
        fgets(input_command,INPUT_SIZE,stdin);
        handle_command(input_command);
    }
}


