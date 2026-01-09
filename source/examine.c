#include <sys/user.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <errno.h>

#include "examine.h"
#include "utils.h"
#include "debug.h"
#include "disassembly.h"
#include "breakpoint.h"

extern debugee_process process_to_debug;


static enum format{
        HEXADECIMAL,
        DECIMAL,
        INSTRUCTION,
};


static int get_size(char str_size){
    int size_to_ret; 

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
        size_to_ret=4;
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




void* get_data_array(int count, int size,long adress){
    void* data_ptr = malloc(size*count);
    int data_ptr_index = 0;
    breakpoint* bp = NULL;
    while(data_ptr_index < count*size){
        bp = get_breakpoint_by_addr(adress+data_ptr_index);
        if(bp != NULL){
            unsigned char original_data = (unsigned char)(bp->orig_data & 0xFF);
            memcpy(data_ptr + data_ptr_index,&original_data,1);
            data_ptr_index++;
        }
        long res = ptrace(PTRACE_PEEKDATA,process_to_debug.pid,adress+data_ptr_index,NULL);
        if(res != -1 && errno == 0){
            memcpy(data_ptr + data_ptr_index,&res,size);
            data_ptr_index += size;         
        }
        else{
            free(data_ptr);
            return NULL;
        }
    }
    return data_ptr;
}


static __uint64_t get_mask_by_size(int size){
    if(size >= 8) return ~0ULL;
    
    return (1ULL << (size*8))-1;
}


void print_data_array(long adress, void* data,int count,int size,const char* format_string){
    int data_index = 0;
    __uint64_t mask = get_mask_by_size(size);
    for(int i = 0; i < count;i++){
        printf("%lx: ",adress + data_index);
        printf(format_string,*((long*)(data+data_index)) & mask); 
        printf("\n");
        data_index += size;
    }
}



static const char* get_format_string(enum format FORMAT, int size)
{
    switch (FORMAT)
    {
    case HEXADECIMAL:
        switch (size)
        {
        case 1: return "%02x";
        case 2: return "%04x";
        case 4: return "%08x";
        case 8: return "%016lx";
        }
        break;

    case DECIMAL:
        switch (size)
        {
        case 1: return "%d";
        case 2: return "%d";
        case 4: return "%d";
        case 8: return "%ld";
        }
        break;
    }

    return NULL; 
}

int exemine(int argc,char** argv){ // x/[COUNT][SIZE][FORMAT] ADDRESS/REGISTER 
    if(process_to_debug.proc_state == LOADED || process_to_debug.proc_state == NOT_LOADED){
        printf("process is not running yet\n");
        return 0;
    }
    if(argv[1] == NULL){
        printf("no adress/register specified\n");
        return 0;
    }

    int argv_len = strlen(argv[0]);
    char* first_str = malloc(argv_len+1);

    
    long adress = 0x0; //default no adress
    struct user_regs_struct regs;
    char register_name[5];
    strncpy(first_str,argv[0],argv_len+1);
    

    const char* format_string;

    int COUNT = 1; //default
    enum format FORMAT = HEXADECIMAL; //default

    int SIZE = sizeof(long); //default

    if(strcmp(first_str,"x") == 0){ //x ADDRESS case 
        if(argv[1][0] == '$'){ //register case
            int status = 0;
            ptrace(PTRACE_GETREGS,process_to_debug.pid,NULL,&regs);
            strncpy(register_name,argv[1]+1,sizeof(register_name));
            register_name[sizeof(register_name)] = '\x00';
            unsigned long register_value = get_register(&regs,register_name,&status);
            if(!register_value && status == -1){ //error in get registers
                return 0;
            }
            adress = register_value;
        }
        else{ //adress case
            adress = convert_str_addr_to_long(argv[1]);
        }


        void* data = get_data_array(COUNT,SIZE,adress); //get data from adress
        if(data == NULL){
            printf("can not access memory at: %lx\n",adress);
            return 0;
        }
        format_string = get_format_string(FORMAT,SIZE);

        if(data == -1 && errno != 0){ //error in fetching data from adress
            printf("ptrace failed");
            return 0;
        }
        else{ //success
            print_data_array(adress,data,COUNT,SIZE,format_string);
            free(data);
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
        if(COUNT == 0) COUNT = 1;

        if(*after_slash != 'i'){
            SIZE = get_size(*after_slash);
            after_slash++;
        }
        else{ //x/i adress/register format
            SIZE = 1;
            FORMAT = INSTRUCTION;
        }
        
        if(argv[1][0] == '$'){ //register case
            int status = 0;
            ptrace(PTRACE_GETREGS,process_to_debug.pid,NULL,&regs);
            strncpy(register_name,argv[1]+1,sizeof(register_name));
            register_name[sizeof(register_name)] = '\x00';
            unsigned long register_value = get_register(&regs,register_name,&status);
            if(!register_value && status == -1){ //error in get registers
                return 0;
            }
            adress = register_value;
        }
        else{ //adress case
            adress = convert_str_addr_to_long(argv[1]);
        }
        if(get_breakpoint_by_addr(adress-1)) adress--; //not null



        void* data = NULL;
        if(FORMAT == INSTRUCTION){
            data = get_data_array(COUNT*15,SIZE,adress);
        }
        else{
            data = get_data_array(COUNT,SIZE,adress);
        } 


        if(data == NULL){
            printf("can not access memory at: %lx\n",adress);
            return 0;
        }
        if(FORMAT == INSTRUCTION){
            print_disassemble_bytes((unsigned char*)data,SIZE * COUNT * 15,adress,COUNT);
        }
        else{
            switch (*after_slash) //deciding format
            {
            case 'd':
                FORMAT = DECIMAL;
                break;
            case 'x':
                FORMAT = HEXADECIMAL;
                break;
            default:
                FORMAT = HEXADECIMAL;
                break;
            }
        }
        if(FORMAT != INSTRUCTION){
            format_string = get_format_string(FORMAT,SIZE);
            print_data_array(adress,data,COUNT,SIZE,format_string);
        }
        free(data);


    }
    free(first_str);
}









