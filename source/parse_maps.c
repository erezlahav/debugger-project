#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "parse_maps.h"
#include "utils.h"
#include "debug.h"
#define CHUNK_SIZE 1024

extern debugee_process process_to_debug;
long get_base_adrr_from_maps_str(char* content){
    char** lines = parser(content,"\n");
    char* base_adress_str = return_str_base_addr(lines);
    long base = get_base_adrr_from_str_base(base_adress_str);
    free(content);
    free_double_str_ptr(lines);
    return base;
}

long get_base_adrr_from_str_base(char* base_adress_str){
    char* base = strtok(base_adress_str,"-");
    long long_base = strtol(base,NULL,16);
    return long_base;
}

char* return_str_base_addr(char** lines){
    char** parts_of_line;
    for(int i = 0;lines[i] != NULL;i++){
        parts_of_line = parser(lines[i]," "); //0 index : adress , 1 index : permissions, 2 index : xxxx, 3 index : date of mapping, 4 index : xxxx, 5 index : name of segment
        char* segment_name = parts_of_line[5];
        char* segment_permissions = parts_of_line[1];
        char* segment_mapped_adress = parts_of_line[0];
        if(strstr(segment_name,process_to_debug.elf_path) != NULL && strstr(segment_permissions,"x") != NULL){
            return segment_mapped_adress;
        }
    }
}

char* read_maps(pid_t pid){
    char pid_str[10];
    sprintf(pid_str,"%d",pid);
    char path_maps[50];
    strcpy(path_maps,"/proc/");
    strcat(path_maps,pid_str);
    strcat(path_maps,"/maps");

    
    FILE* maps_file_ptr = fopen(path_maps,"rb");
    if(maps_file_ptr == NULL){
        printf("fopen failed!\n");
        exit(0);
    }
    char* content = malloc(CHUNK_SIZE);
    fread(content,1,CHUNK_SIZE,maps_file_ptr);
    fclose(maps_file_ptr);

    return content;
}


long get_base_adress(pid_t pid){

    char* content = read_maps(pid);
    get_base_adrr_from_maps_str(content);
}








