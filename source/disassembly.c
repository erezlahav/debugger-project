
#include <capstone/capstone.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <string.h>

#include "debug.h"
#include "elf_parser.h"
#include "breakpoint.h"
#include "utils.h"
extern debugee_process process_to_debug;

int live_disassemble_symbol(symbol* symbol){
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




int static_disassemble_symbol(symbol* symbol){
    FILE* elf_target_ptr = fopen(process_to_debug.elf_path,"rb");
    if(elf_target_ptr == NULL){
        printf("fopen failed!\n");
        return 0;
    }
    unsigned char* bytes_array = get_bytes_array_code_from_symbol(symbol,elf_target_ptr);
    print_disassemble_bytes(bytes_array,symbol->size,symbol->adress);
}


void print_disassemble_bytes(unsigned char* bytes_array,size_t size,long start_addr){
    printf("adress : %lx\n",start_addr);
    printf("first byte : %x\n",*bytes_array);
    printf("size : %ld\n",size);
    csh handle;
    cs_insn *insn;
    size_t count;
    cs_open(CS_ARCH_X86, CS_MODE_64, &handle);
    
    count = cs_disasm(handle, bytes_array, size, start_addr, 0, &insn);
    printf("count : %ld\n",count);
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



int is_call_instruction(long opcode){
    unsigned char lsb = (opcode & 0xFF);
    unsigned char second_lsb = (opcode >> 8) & 0xFF;
    if(lsb == 0xE8){ //relitive call 
        return 1;
    }
    if(lsb == 0xFF && ( ((second_lsb >> 3) & 7) == 2)){//first check if the LSB is 0xFF and then get the 3-5 bits and check if they are 010b then we know its unrelitive call instruction
        return 1;
    }
    return 0;
}


int get_length_of_instruction(unsigned char* bytes_array,size_t size,long start_addr){
    csh handle;
    cs_insn *insn;
    size_t count;

    cs_open(CS_ARCH_X86, CS_MODE_64, &handle);

    count = cs_disasm(handle,bytes_array,size,start_addr,1,&insn);
    if (count > 0) {
        cs_free(insn, count);
        return insn[0].size;
    }
    else{
        return 0;
    }

}





