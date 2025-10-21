#pragma once
#define INPUT_SIZE 50
typedef struct{
    char* command;
    int(*func_handler)(int argc, char** argv);
    char* help;
}command_table;



extern const command_table table_commands[];

int debug_process(pid_t pid);
int info(int argc,char** argv);
int breakpoint(int argc,char** argv);
int exit_debugger(int argc,char** argv);