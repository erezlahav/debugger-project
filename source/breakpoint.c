#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ptrace.h>
#include <errno.h>
#include "breakpoint.h"
#include "elf_parser.h"
#include "debug.h"
extern debugee_process process_to_debug;

int ptrace_breakpoint(breakpoint* bp){
    if(process_to_debug.proc_state == NOT_LOADED){
        return 0;
    }
    if(bp->state != RESOLVED){
        return 0;
    }
    errno = 0;
    long res = ptrace(PTRACE_PEEKDATA,process_to_debug.pid,bp->abs_adress,NULL);
    if (res == -1 && errno != 0) {
        printf("ptrace failed: %s\n", strerror(errno));
    } 
    bp->orig_data = res;
    long new_instruction_with_cc = (res & 0xffffffffffffff00) | 0xcc;
    ptrace(PTRACE_POKEDATA,process_to_debug.pid,bp->abs_adress,new_instruction_with_cc);
}


int create_pending_breakpoint(symbol* bp_symbol,long offset_from_symbol,long abs_adress){
    process_to_debug.array_of_breakpoints.arr_breakpoints[process_to_debug.array_of_breakpoints.number_of_breakpoints].index = process_to_debug.array_of_breakpoints.number_of_breakpoints;
    process_to_debug.array_of_breakpoints.arr_breakpoints[process_to_debug.array_of_breakpoints.number_of_breakpoints].bp_symbol = bp_symbol;
    process_to_debug.array_of_breakpoints.arr_breakpoints[process_to_debug.array_of_breakpoints.number_of_breakpoints].offset_from_symbol = offset_from_symbol;
    process_to_debug.array_of_breakpoints.arr_breakpoints[process_to_debug.array_of_breakpoints.number_of_breakpoints].abs_adress = abs_adress;
    process_to_debug.array_of_breakpoints.arr_breakpoints[process_to_debug.array_of_breakpoints.number_of_breakpoints].state = PENDING;
    process_to_debug.array_of_breakpoints.number_of_breakpoints++;
}

int create_resolved_breakpoint(symbol* bp_symbol,long offset_from_symbol,long abs_adress){
    if(process_to_debug.proc_state == NOT_LOADED){
        return 0;
    }
    process_to_debug.array_of_breakpoints.arr_breakpoints[process_to_debug.array_of_breakpoints.number_of_breakpoints].index = process_to_debug.array_of_breakpoints.number_of_breakpoints;
    process_to_debug.array_of_breakpoints.arr_breakpoints[process_to_debug.array_of_breakpoints.number_of_breakpoints].abs_adress = abs_adress;
    process_to_debug.array_of_breakpoints.arr_breakpoints[process_to_debug.array_of_breakpoints.number_of_breakpoints].offset_from_symbol = offset_from_symbol;
    process_to_debug.array_of_breakpoints.arr_breakpoints[process_to_debug.array_of_breakpoints.number_of_breakpoints].bp_symbol = bp_symbol;
    if(bp_symbol != NULL){
        long symbol_adress = bp_symbol->adress;
        process_to_debug.array_of_breakpoints.arr_breakpoints[process_to_debug.array_of_breakpoints.number_of_breakpoints].abs_adress = symbol_adress + offset_from_symbol;
    }
    process_to_debug.array_of_breakpoints.arr_breakpoints[process_to_debug.array_of_breakpoints.number_of_breakpoints].state = RESOLVED;
    
    ptrace_breakpoint(&process_to_debug.array_of_breakpoints.arr_breakpoints[process_to_debug.array_of_breakpoints.number_of_breakpoints]);
    process_to_debug.array_of_breakpoints.number_of_breakpoints++;
}



breakpoint* get_breakpoint_by_addr(long adress){
    breakpoints_array arr_of_breakpoints = process_to_debug.array_of_breakpoints;
    for(int i = 0; i < arr_of_breakpoints.number_of_breakpoints;i++){
        if(arr_of_breakpoints.arr_breakpoints[i].abs_adress == adress){
            return &arr_of_breakpoints.arr_breakpoints[i];
        }
    }
    return NULL; //if breakpoint not exists
}

int create_breakpoint(symbol* bp_symbol,long offset_from_symbol,long abs_adress){
    if(process_to_debug.proc_state == NOT_LOADED){
        create_pending_breakpoint(bp_symbol, offset_from_symbol, abs_adress);
    }
    else{
        create_resolved_breakpoint(bp_symbol, offset_from_symbol, abs_adress);
    }
}

int resolve_breakpoints(){
    if(process_to_debug.proc_state == NOT_LOADED){return 0;}
    for(int i = 0; i < process_to_debug.array_of_breakpoints.number_of_breakpoints;i++){
        if(process_to_debug.array_of_breakpoints.arr_breakpoints[i].state == PENDING){
            if(process_to_debug.array_of_breakpoints.arr_breakpoints[i].bp_symbol != NULL){
                long symbol_adress = process_to_debug.array_of_breakpoints.arr_breakpoints[i].bp_symbol->adress;
                long offset_from_symbol = process_to_debug.array_of_breakpoints.arr_breakpoints[i].offset_from_symbol;
                process_to_debug.array_of_breakpoints.arr_breakpoints[i].abs_adress = symbol_adress + offset_from_symbol;
            }
            process_to_debug.array_of_breakpoints.arr_breakpoints[i].state = RESOLVED;
            ptrace_breakpoint(&process_to_debug.array_of_breakpoints.arr_breakpoints[i]);
        }
        
    }
}

void print_breakpoint(breakpoint* bp){
    printf("breakpoint %d : adress : %ld, state : %d\n",bp->index,bp->abs_adress,bp->state);
}

void print_breakpoints(){
    for(int i = 0; i < process_to_debug.array_of_breakpoints.number_of_breakpoints;i++){
        breakpoint current_breakpoint = process_to_debug.array_of_breakpoints.arr_breakpoints[i];
        print_breakpoint(&current_breakpoint);
    }
}


int set_breakpoint(int argc,char** argv){
    if(argc != 2){
        printf("breakpoint syntax incorrect\n");
        return 0;
    }

    if(argv[1][0] != '*'){ //no * in argv[1]
        break_symbol(argv[1]);
    }
    else{ //* in argv, means its raw adrress or relitive symbol
        handle_star_breakpoint(argv);
    }
    
}


int break_symbol(char* symbol_name){
    symbol* target_symbol = find_symbol_by_name(process_to_debug.array_of_symbols,symbol_name);
    if(target_symbol == NULL){
        printf("symbol not found\n");
        return 0;
    }
    else{
        long adress_of_symbol = target_symbol->adress;
        create_breakpoint(target_symbol,0,adress_of_symbol); //0 in offset becuase this is a condition of only symbol no offset
    }
}




int handle_star_breakpoint(char** argv){
    char* break_arg = argv[1];
    if(break_arg[0] != '*'){return 0;};

    break_arg++;
    printf("%s\n",break_arg);
    if(strncmp(break_arg,"0x",2) == 0 || strncmp(break_arg,"0X",2) == 0){ //break adress case like b *0x007fffc120
        set_break_raw_adress(break_arg);
    }

    else{
        set_break_in_star_symbol(break_arg);
    }
}

int set_break_raw_adress(char* addr_to_break){
    long break_adress = string_addr_to_long(addr_to_break);
    create_breakpoint(NULL,0,break_adress); //NULL and 0 becuase this is a condition of only raw adress
}


int break_in_relitive_symbol(char* symbol_name,long offset_from_symbol){
    symbol* target_symbol = find_symbol_by_name(process_to_debug.array_of_symbols,symbol_name);
    if(target_symbol == NULL){
        printf("symbol not found!\n");
    }
    else{
        long adress_of_relitive_symbol = target_symbol->adress;
        create_breakpoint(target_symbol,offset_from_symbol,adress_of_relitive_symbol+offset_from_symbol); //absolute adress still unknown due to PIE and will be calculated after
    }
    return 0;
}


int set_break_in_star_symbol(char* break_argument){
    int plus_index;
    char* symbol_name = get_relitive_symbol_name_and_plus_index(break_argument,&plus_index);
    long relitive_symbol_offset = 0;
    char* chars_after_plus;
    if(plus_index != -1){ //have offset after symbol
        chars_after_plus = break_argument + plus_index + 1;
        if(strncmp(chars_after_plus,"0x",2) == 0 || strncmp(chars_after_plus,"0X",2) == 0){ //offset in hexadecimal
            relitive_symbol_offset = string_addr_to_long(chars_after_plus);
        }
        else{
            relitive_symbol_offset = atoi(chars_after_plus); //offset in decimal
        }

        break_in_relitive_symbol(symbol_name,relitive_symbol_offset);

    }
    else{
        break_symbol(symbol_name); //only symbol
    }
    
}

char* get_relitive_symbol_name_and_plus_index(char* break_argument,int* plus_index){
    int length = strlen(break_argument);
    char* symbol_name = malloc(length+1);
    *plus_index = -1;
    for(int i = 0;break_argument[i] != '\x00';i++){
        if(break_argument[i] == '+'){
            *plus_index = i;
            break;
        }   
    }
    if(*plus_index != -1){
        strncpy(symbol_name,break_argument,*plus_index);
        symbol_name[*plus_index] = '\x00';
    }
    else{
        strncpy(symbol_name,break_argument,length);
        symbol_name[length] = '\x00';
    }
    return symbol_name;
}

long string_addr_to_long(char* string_adrr){
    return strtol(string_adrr,NULL,16);
}

