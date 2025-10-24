#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "../include/breakpoint.h"





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
        handle_break_adress(argv);
    }
    
}


int break_symbol(char* symbol){
    printf("in break symbol\n");
}




int handle_break_adress(char** argv){
    char* break_arg = argv[1];
    if(break_arg[0] != '*'){return 0;};

    break_arg++;
    if(strncmp(break_arg,"0x",2) == 0 || strncmp(break_arg,"0X",2) == 0){ //break adress case like b *0x007fffc120
        set_break_raw_adress(break_arg);
    }

    else{
        set_break_relitive_symbol(break_arg);
    }
}

int set_break_raw_adress(char* addr_to_break){
    printf("in break adress\n");
}


int set_break_relitive_symbol(char* break_argument){
    printf("in set_break_relitive_symbol\n");
    int* plus_index = malloc(sizeof(int));
    char* symbol_name = get_relitive_symbol_name(break_argument,plus_index);
    printf("symbol name : %s\n",symbol_name);

}

char* get_relitive_symbol_name(char* break_argument,int* plus_index){
    int length = strlen(break_argument);
    char* symbol_name = malloc(length);
    *plus_index = -1;
    for(int i = 0;break_argument[i] != '\x00';i++){
        if(break_argument[i] == '+'){
            *plus_index = i;
            break;
        }   
    }
    if(*plus_index != -1){
        strncpy(symbol_name,break_argument,*plus_index);
    }
    else{
        strncpy(symbol_name,break_argument,length);
    }
    return symbol_name;
}

long string_addr_to_long(char* string_adrr){
    return strtol(string_adrr,NULL,16);
}

