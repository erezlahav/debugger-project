#pragma once

#include <stdint.h>


typedef struct{
    void* adress;
    uint64_t orig_data; 
    char* print_breakpoint;
}breakpoint;


int ptrace_breakpoint(long adress);
int set_breakpoint(int argc,char** argv);
int break_symbol(char* symbol);
int handle_star_breakpoint(char** argv);
int set_break_raw_adress(char* addr_to_break);
int break_in_relitive_symbol(char* symbol_name,long offset_from_symbol);
int set_break_in_star_symbol(char* break_argument);
char* get_relitive_symbol_name_and_plus_index(char* break_argument,int* plus_index);
long string_addr_to_long(char* string_adrr);
