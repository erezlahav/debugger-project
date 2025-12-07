
#include <capstone/capstone.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <string.h>

#include "debug.h"
#include "elf_parser.h"
#include "breakpoint.h"
extern debugee_process process_to_debug;

void live_disassemble_symbol(symbol* symbol){
    size_t symbol_size = symbol->size;
    long symbol_adress = symbol->adress;
    unsigned char* bytes_array = malloc(symbol_size);
    long current_index = 0;
    long ptrace_res = 0;
    breakpoint* curr_bp = NULL;
    while(current_index < symbol_size){
        curr_bp = get_breakpoint_by_addr(symbol_adress+current_index);
        if(curr_bp == NULL){
            ptrace_res = ptrace(PTRACE_PEEKDATA,process_to_debug.pid,symbol_adress+current_index,NULL);
        }
        else{
            ptrace_res = curr_bp->orig_data;
        }
        
        memcpy(bytes_array + current_index,&ptrace_res,sizeof(ptrace_res));
        current_index += sizeof(ptrace_res);
    }
    print_disassemble_bytes(bytes_array,symbol_size,symbol_adress);
    free(bytes_array);
}




void static_disassemble_symbol(symbol* symbol){
    printf("in static_disassemble_symbol\n");
}


void print_disassemble_bytes(unsigned char* bytes_array,size_t size,long start_addr){
    csh handle;
    cs_insn *insn;
    size_t count;
    cs_open(CS_ARCH_X86, CS_MODE_64, &handle);
    
    count = cs_disasm(handle, bytes_array, size, start_addr, 0, &insn);
    if (count > 0) {
        for (size_t i = 0; i < count; i++) {
            printf("0x%lx\t%s\t%s\n",insn[i].address,insn[i].mnemonic,insn[i].op_str);
        }
        cs_free(insn, count);
    }
     else {
        printf("Disassembly failed\n");
    }

    cs_close(&handle);
}






