#include <sys/ptrace.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "debug.h"
#include "elf_parser.h"
#include "utils.h"
#define SIZE_OF_PATH 50

debugee_process process_to_debug;
symbols_array* array_of_symbols;
int main(int argc,char* argv[],char* envp[]){
    

    if(argc != 3){
        printf("Usage: %s -mode <pid>/<path>\n",argv[0]);
        exit(0);
    }
    

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
        array_of_symbols = get_symbols_from_file(elf_target_ptr);
        fclose(elf_target_ptr);

        debug_process(process_to_debug.elf_path);
        /*
        int status;
        waitpid(child_pid, &status, 0);
        if (WIFSTOPPED(status)) {
            printf("The program is stopped, signal: %d\n", WSTOPSIG(status));
        }
        if (WIFEXITED(status)) {
            printf("The program exited with %d\n", WEXITSTATUS(status));
        }
        */
    }


    else if(strcmp(argv[1],"-attach") == 0){
        printf("executing -attach to process\n");
    }


    else{
        printf("mode undefined, use -run / -attach\n");
    }

}
