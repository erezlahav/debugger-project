#pragma once
#define INPUT_SIZE 50


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
    RUNNING_STATE proc_state;
    pid_t pid;
}debugee_process;

extern const command_table table_commands[];