#include <stdio.h>
#include <sys/user.h>
#include <sys/ptrace.h>

#include "debug.h"
#include "elf_parser.h"

extern debugee_process process_to_debug;



symbol* get_symbol_by_adress(unsigned long adress){


    symbol* symbol_array_ptr = process_to_debug.array_of_symbols->symbols;
    uint16_t number_of_symbols =  process_to_debug.array_of_symbols->number_of_symbols;

    for(int i = 0; i < number_of_symbols; i++){
        long start_adress = symbol_array_ptr[i].adress;
        long end_adress = symbol_array_ptr[i].adress + symbol_array_ptr[i].size;
        if(adress > start_adress && adress < end_adress){
            return &symbol_array_ptr[i];
        }
    }
    return NULL;
}





int print_backtrace(int argc,char** argv){
    struct user_regs_struct regs;
    get_registers(process_to_debug.pid, &regs);
    unsigned long current_rbp = regs.rbp;
    unsigned long current_rip = regs.rip;
    symbol* current_symbol = get_symbol_by_adress(current_rip);
    printf("%s\n",current_symbol->name);
    printf("^\n");
    while(current_rbp != 0){
        long return_adress = ptrace(PTRACE_PEEKDATA,process_to_debug.pid,(void*)(current_rbp+8),NULL);
        printf("return adress : %lx\n",return_adress);
        current_symbol = get_symbol_by_adress(return_adress);
        printf("%s\n",current_symbol->name);
        printf("^\n");
    }
}






