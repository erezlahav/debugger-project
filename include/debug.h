#pragma once
#define INPUT_SIZE 200

#include "maps_parsing.h"
#include "breakpoint.h"
#include "elf_parser.h"


typedef struct{
    char* command;
    int(*func_handler)(int argc, char** argv);
    char* help;
}command_table;

typedef enum{
    RUNNING,
    STOPPED,
    EXITED,
    LOADED,
    NOT_LOADED
}RUNNING_STATE;

typedef struct{
    char* elf_path;
    bool PIE;
    RUNNING_STATE proc_state;
    pid_t pid;
    regions_array array_of_regions;
    breakpoints_array array_of_breakpoints;
    symbols_array* array_of_symbols;
}debugee_process;

extern const command_table table_commands[];