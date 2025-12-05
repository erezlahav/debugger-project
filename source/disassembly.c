
#include <capstone/capstone.h>
#include <stdio.h>
#include <sys/ptrace.h>

#include "elf_parser.h"



void print_disassemble_symbol(symbol* symbol){

}


void print_disassemble_bytes(unsigned char* bytes_array,size_t size,long start_addr){
    csh handle;
    cs_insn *insn;
    size_t count;
    cs_open(CS_ARCH_X86, CS_MODE_64, &handle);
    
    count = cs_disasm(handle, bytes_array, size, start_addr, 0, &insn);
    if (count > 0) {
        for (size_t i = 0; i < count; i++) {
            printf("0x%lx:\t%s\t%s\n",insn[i].address,insn[i].mnemonic,insn[i].op_str);
        }
        cs_free(insn, count);
    }
     else {
        printf("Disassembly failed\n");
    }

    cs_close(&handle);
}






