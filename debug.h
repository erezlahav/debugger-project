#pragma once

typedef struct{
    char* command;
    int(*func_handler)(int argc, char** argv);
    char* help;
}command_table



int debug_process(pid_t pid);
