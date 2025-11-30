#pragma once

#include <stdint.h>
#include "elf_parser.h"
typedef enum{
    PENDING,
    RESOLVED,
    FAILED
}bp_state;

typedef struct{
    int index;
    long abs_adress;
    long orig_data;
    symbol* bp_symbol; //NULL if NOT exists
    long offset_from_symbol; //0 if not symbol and 0 if no offset
    bp_state state;
}breakpoint;

typedef struct{
    breakpoint* arr_breakpoints;
    int number_of_breakpoints;
}breakpoints_array;

int ptrace_breakpoint(breakpoint* bp);
int create_pending_breakpoint(symbol* bp_symbol,long offset_from_symbol,long abs_adress);
int create_resolved_breakpoint(symbol* bp_symbol,long offset_from_symbol,long abs_adress);
breakpoint* get_breakpoint_by_addr(long adress);
int create_breakpoint(symbol* bp_symbol,long offset_from_symbol,long abs_adress);
int resolve_breakpoints();
void print_breakpoint(breakpoint* bp);
void print_breakpoints();
int set_breakpoint(int argc,char** argv);
int break_symbol(char* symbol_name);
int handle_star_breakpoint(char** argv);
int set_break_raw_adress(char* addr_to_break);
int break_in_relitive_symbol(char* symbol_name,long offset_from_symbol);
int set_break_in_star_symbol(char* break_argument);
char* get_relitive_symbol_name_and_plus_index(char* break_argument,int* plus_index);
long string_addr_to_long(char* string_adrr);
