#include <sys/ptrace.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include "debug.h"
#include "elf_parser.h"
#include "utils.h"
#include "breakpoint.h"
#define SIZE_OF_PATH 150
#define MAX_AMOUNT_OF_BREAKPOINTS 100

debugee_process process_to_debug;
int main(int argc,char* argv[],char* envp[]){
    

    if(argc != 3){
        printf("Usage: %s -mode <pid>/<path>\n",argv[0]);
        exit(0);
    }
    
    process_to_debug.array_of_breakpoints.number_of_breakpoints = 0;
    process_to_debug.array_of_breakpoints.arr_breakpoints = malloc(sizeof(breakpoint)* MAX_AMOUNT_OF_BREAKPOINTS);

    if(strcmp(argv[1],"-run") == 0){
        printf("executing -run to process\n");
        process_to_debug.proc_state = NOT_LOADED;
        process_to_debug.elf_path = malloc(SIZE_OF_PATH);
        process_to_debug.pid = -1;
        strncpy(process_to_debug.elf_path,argv[2],SIZE_OF_PATH);
        FILE* elf_target_ptr = get_file_pointer_by_path(process_to_debug.elf_path);
        if(elf_target_ptr == NULL){
            printf("fopen failed!\n");
            exit(0);
        }
        process_to_debug.PIE = get_pie_status(elf_target_ptr);
        process_to_debug.array_of_symbols = get_symbols_from_file(elf_target_ptr);
        fclose(elf_target_ptr);

        debug_process(process_to_debug.elf_path);
    }


    else if(strcmp(argv[1],"-attach") == 0){
        printf("executing -attach to process\n");
    }


    else{
        printf("mode undefined, use -run / -attach\n");
    }

}
