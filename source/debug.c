#include <stdio.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/user.h>

#include "debug.h"
#include "parser.h"
#include "info_commands.h"
#include "commands.h"
#include "breakpoint.h"

extern debugee_process process_to_debug;

const command_table table_commands[] = {
    {"info",info,"help displaying data like functions/registers and more..."},
    {"break",set_breakpoint,"set breakpoint in adress you choose"},
    {"exit",exit_debugger,"exit from debugger"},
    {"c",continue_proc,"continue the execution of the process"},
    {"r",run_process,"run the current process after breaking on stop signal before main"},
    {NULL,NULL,NULL}
};



int get_registers(pid_t pid, struct user_regs_struct* regs){
    if(ptrace(PTRACE_GETREGS,pid,NULL,regs) == -1){
        return 0;
    }
    return 1;
}

int set_registers(pid_t pid, struct user_regs_struct* regs){
    if(ptrace(PTRACE_SETREGS,pid,NULL,regs) == -1){
        return 0;
    }
    return 1;
}


int handle_command(char* command){
    printf("command : %s\n",command);
    int* argc = malloc(sizeof(int));
    char** commands = parse_command(command,argc);
    if(commands == NULL){
        return 0;
    }

    for(int i =0; table_commands[i].command!= NULL;i++){
        if(commands[0] == NULL){break;}
        if(strcmp(commands[0],table_commands[i].command) == 0){
            table_commands[i].func_handler(*argc,commands);
            return 1;
        }
    }
    return 0;
}


int debug_process(char* elf_path){
    char* input_command = malloc(sizeof(char)*INPUT_SIZE);
    int status;
    while(1){
        if(process_to_debug.proc_state == NOT_LOADED || process_to_debug.proc_state == STOPPED){
            printf(">");
            fgets(input_command,INPUT_SIZE,stdin);
            handle_command(input_command);
        }
        else if(process_to_debug.proc_state == RUNNING){
            waitpid(process_to_debug.pid,&status,0);
            if(WIFSTOPPED(status)){
                process_to_debug.proc_state = STOPPED;
                int sig = WSTOPSIG(status);
                if(sig == SIGTRAP){
                    printf("breakpoint hit!\n");
                }
            }
            else if(WIFEXITED(status)){
                printf("child exited!\n");
                break;
            }
        }
    }
}


