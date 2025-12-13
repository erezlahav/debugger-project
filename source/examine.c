#include <sys/user.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "examine.h"
#include "utils.h"



static int get_size(char str_size){
    int size_to_ret = sizeof(long);

    switch (str_size) //deciding size
    {
    case 'g':
        size_to_ret = sizeof(long);
        break;
    case 'w':
        size_to_ret = 4;
        break;
    case 'h':
        size_to_ret = 2;
        break;
    case 'b':
        size_to_ret = 1;
        break;
    default:
        size_to_ret=sizeof(long);
        break;
    }
    return size_to_ret;
}



static unsigned long get_register(struct user_regs_struct* regs_ptr,char* str_register,int* status){
    if (strcmp(str_register, "rip") == 0) return regs_ptr->rip;
    if (strcmp(str_register, "rax") == 0) return regs_ptr->rax;
    if (strcmp(str_register, "rbx") == 0) return regs_ptr->rbx;
    if (strcmp(str_register, "rcx") == 0) return regs_ptr->rcx;
    if (strcmp(str_register, "rdx") == 0) return regs_ptr->rdx;
    if (strcmp(str_register, "rsi") == 0) return regs_ptr->rsi;
    if (strcmp(str_register, "rdi") == 0) return regs_ptr->rdi;
    if (strcmp(str_register, "rsp") == 0) return regs_ptr->rsp;
    if (strcmp(str_register, "rbp") == 0) return regs_ptr->rbp;

    *status = -1;
    return 0;
}


int exemine(int argc,char** argv){ // x/[COUNT][SIZE][FORMAT] ADDRESS/REGISTER 
    /*
    if(process_to_debug.proc_state == LOADED || process_to_debug.proc_state == NOT_LOADED){
        printf("process is not running yet\n");
        return 0;
    }
*/
    if(argv[1] == NULL){
        printf("no adress/register specified\n");
        return 0;
    }
    int argv_len = strlen(argv[0]);
    char* first_str = malloc(argv_len+1);

    
    long adress = 0x0; //default no adress
    char register_name[5];
    strncpy(first_str,argv[0],argv_len+1);
    

    typedef enum{
        HEXADECIMAL,
        DECIMAL,
        INSTRUCTION,
    }format;

    int COUNT = 0; //default
    format FORMAT = HEXADECIMAL; //default

    int SIZE = sizeof(long); //default

    if(strcmp(first_str,"x") == 0){ //x ADDRESS case 
        if(argv[1][0] == '$'){ //register case
            strncpy(register_name,argv[1]+1,sizeof(register_name));
        }
        else{ //adress case
            printf("adress case\n");
            adress = convert_str_addr_to_long(argv[1]);

        }
    }
    else{  //    x/[COUNT][SIZE][FORMAT] ADDRESS/REGISTER expected
        if(first_str[1] != '/'){
            printf("invalid format command\n");
            return 0;
        }
        char* after_slash = first_str + 2;
        COUNT = atoi(after_slash);
        while(*after_slash >= '0' && *after_slash <= '9'){
            after_slash++;
        }


        SIZE = get_size(*after_slash);
        
        after_slash++;

        if(argv[1][0] == '$'){ //register case
            strncpy(register_name,argv[1]+1,sizeof(register_name));
        }
        else{ //adress case
            printf("adress case\n");
            adress = convert_str_addr_to_long(argv[1]);
        }


        switch (*after_slash) //deciding format
        {
        case 'd':
            FORMAT = DECIMAL;
            break;
        case 'x':
            FORMAT = HEXADECIMAL;
            break;
        case 'i':
            FORMAT = INSTRUCTION;
            break;
        default:
            FORMAT = HEXADECIMAL;
            break;
        }
        



    }
}









