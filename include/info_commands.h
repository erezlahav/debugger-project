#pragma once
#include <unistd.h>


typedef struct{
    char* command;
    int(*info_func_handler)(int argc,char** argv,pid_t pid);
    char* help;
}info_commands;


int info(int argc,char** argv,pid_t pid);

int info_registers(int argc, char** argv,pid_t pid);
int info_functions(int argc, char** argv,pid_t pid);
int info_breakpoints(int argc, char** argv,pid_t pid);