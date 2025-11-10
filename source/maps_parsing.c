#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "maps_parsing.h"
#include "utils.h"
#include "debug.h"
#define CHUNK_SIZE 4096

extern debugee_process process_to_debug;
regions_array array_of_regions;

void parse_lines_of_maps(char** lines){ //parsing the lines of maps file and putting it in global array of mappings 
    array_of_regions.regions_index = 0;
    char** parts_of_line;
    char** two_adresses;
    for(int i = 0;lines[i] != NULL;i++){
        long* str_start_addr = malloc(sizeof(long));
        long* str_end_addr = malloc(sizeof(long));
        parts_of_line = parser(lines[i]," "); //0 index : adress , 1 index : permissions, 2 index : xxxx, 3 index : date of mapping, 4 index : xxxx, 5 index : name of segment
        char* segment_name;
        if(parts_of_line[5] == NULL){
            segment_name = " ";
        }
        else{
            segment_name = parts_of_line[5];
        }
        char* segment_permissions = parts_of_line[1];
        char* segment_mapped_adress = parts_of_line[0];
        two_adresses = parser(segment_mapped_adress,"-");
        *str_start_addr = strtol(two_adresses[0],NULL,16);
        *str_end_addr = strtol(two_adresses[1],NULL,16);
        if(strstr(segment_name,process_to_debug.elf_path) != NULL && strstr(segment_permissions,"x") != NULL){
           array_of_regions.arr[array_of_regions.regions_index].type = BINARY;
            array_of_regions.arr[array_of_regions.regions_index].start = (void*)(str_start_addr);
            array_of_regions.arr[array_of_regions.regions_index].end = (void*)(str_end_addr);
            array_of_regions.regions_index++;
        }
        else if(strstr(segment_name,"[heap]") != NULL){
            array_of_regions.arr[array_of_regions.regions_index].type = HEAP;
            array_of_regions.arr[array_of_regions.regions_index].start = (void*)(str_start_addr);
            array_of_regions.arr[array_of_regions.regions_index].end = (void*)(str_end_addr);
            array_of_regions.regions_index++;
        }
        else if(strstr(segment_name,"[stack]") != NULL){
            array_of_regions.arr[array_of_regions.regions_index].type = STACK;
            array_of_regions.arr[array_of_regions.regions_index].start = (void*)(str_start_addr);
            array_of_regions.arr[array_of_regions.regions_index].end = (void*)(str_end_addr);
            array_of_regions.regions_index++;
        }
        free_double_str_ptr(two_adresses);
        free_double_str_ptr(parts_of_line);
    }
}

void print_mem_regions(){
    for(int i = 0; i < array_of_regions.regions_index;i++){
        printf("mem region : %d, start : %ld, end : %ld\n",array_of_regions.arr[i].type,*(long*)array_of_regions.arr[i].start, *(long*)array_of_regions.arr[i].end);
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
        return NULL;
    }
    char* content = malloc(CHUNK_SIZE);
    int offset_to_content = 0;
    int bytes_read = 0;
    while((bytes_read = fread(content + offset_to_content,1,CHUNK_SIZE,maps_file_ptr)) > 0){
        offset_to_content += bytes_read;
        content = realloc(content,offset_to_content+CHUNK_SIZE);
    }
    fclose(maps_file_ptr);

    return content;
}



int load_proc_info(pid_t pid){
    char* content = read_maps(pid);
    printf("%s\n",content);
    char** lines = parser(content,"\n");
    parse_lines_of_maps(lines);
    free(content);
    free_double_str_ptr(lines);
}







