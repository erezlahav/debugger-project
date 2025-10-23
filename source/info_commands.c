#include <stdio.h>
#include <string.h>

#include "info_commands.h"


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
}



int info_functions(int argc, char** argv){
    printf("in info functions\n");
}


int info_breakpoints(int argc, char** argv){
    printf("in info breakpoints\n");
}