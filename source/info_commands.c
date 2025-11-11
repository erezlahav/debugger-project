#include <stdio.h>
#include <string.h>

#include "info_commands.h"
#include "elf_parser.h"
#include "debug.h"
#include "breakpoint.h"
extern debugee_process process_to_debug;
info_commands functions_info[] = {
    {"registers",info_registers,"info all the current thread registers"},
    {"functions",info_functions,"info all functions"},
    {"breakpoints",info_breakpoints,"info all breakpoints"},
    {NULL,NULL,NULL}
};

int info(int argc,char** argv){
    if(argc != 2){
        return 0;
    }
    for(int i = 0; functions_info[i].command != NULL;i++){
        if(strcmp(argv[1],functions_info[i].command) == 0){
            functions_info[i].info_func_handler(argc,argv);
            return 1;
        }
    }

    return 0;
}



int info_registers(int argc, char** argv){
    printf("in info registers\n");
    if(process_to_debug.proc_state != STOPPED || process_to_debug.proc_state != RUNNING){
        printf("no registers avalible\n");
    }
}



int info_functions(int argc, char** argv){
    printf("in info functions!\n");
    for(int i = 0; i < process_to_debug.array_of_symbols->number_of_symbols;i++){
        if(process_to_debug.array_of_symbols->symbols[i].type == FUNC){
            printf("0x%08lx     %s\n",process_to_debug.array_of_symbols->symbols[i].adress,process_to_debug.array_of_symbols->symbols[i].name);
        }
    }
}


int info_breakpoints(int argc, char** argv){
    printf("in info breakpoints\n");
    print_breakpoints();
}