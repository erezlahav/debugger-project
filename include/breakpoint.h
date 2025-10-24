#pragma once

#include <stdint.h>


typedef struct{
    void* adress;
    uint64_t orig_data; 
    char* print_breakpoint;
}breakpoint;



int set_breakpoint(int argc,char** argv);
int break_symbol(char* symbol);
int handle_break_adress(char** argv);
int set_break_raw_adress(char* addr_to_break);
int set_break_relitive_symbol(char* break_argument);
char* get_relitive_symbol_name(char* break_argument,int* plus_index);
long string_addr_to_long(char* string_adrr);
