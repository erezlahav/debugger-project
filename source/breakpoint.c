#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "breakpoint.h"
#include "elf_parser.h"
#include "debug.h"
extern symbols_array* array_of_symbols;
extern debugee_process process_to_debug;
extern breakpoints_array array_of_breakpoints;

int create_pending_breakpoint(long adress){
    array_of_breakpoints.arr_breakpoints[array_of_breakpoints.number_of_breakpoints].adress = (void*)adress;
    array_of_breakpoints.arr_breakpoints[array_of_breakpoints.number_of_breakpoints].state = PENDING;
    array_of_breakpoints.number_of_breakpoints++;
}

int resolve_breakpoints(){
    if(process_to_debug.proc_state == NOT_LOADED){return 0;}
    for(int i = 0; i < array_of_breakpoints.number_of_breakpoints;i++){
        breakpoint current_breakpoint = array_of_breakpoints.arr_breakpoints[i];
        
    }
}

void print_breakpoints(){
    for(int i = 0; i < array_of_breakpoints.number_of_breakpoints;i++){
        breakpoint current_breakpoint = array_of_breakpoints.arr_breakpoints[i];
        printf("breakpoint %d : adress : %ld, state : %d\n",i,current_breakpoint.adress,current_breakpoint.state);
    }
}


int set_breakpoint(int argc,char** argv){
    printf("in breakpoint\n");
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
    symbol* target_symbol = find_symbol_by_name(array_of_symbols,symbol_name);
    if(target_symbol == NULL){
        printf("symbol not found\n");
        return 0;
    }
    else{
        long adress_of_relitive_symbol = target_symbol->adress;
        create_pending_breakpoint(adress_of_relitive_symbol);        
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
    create_pending_breakpoint(break_adress);
}


int break_in_relitive_symbol(char* symbol_name,long offset_from_symbol){
    symbol* target_symbol = find_symbol_by_name(array_of_symbols,symbol_name);
    if(target_symbol == NULL){
        printf("symbol not found!\n");
    }
    else{
        long adress_of_relitive_symbol = target_symbol->adress;
        adress_of_relitive_symbol += offset_from_symbol;
        create_pending_breakpoint(adress_of_relitive_symbol);
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

