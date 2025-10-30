#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "parse_maps.h"
#include "utils.h"
#define CHUNK_SIZE 1024



long get_base_adrr_from_maps_str(char* content){
    char** lines = parser(content,"\n");
    return_text_segment_line(lines);
}

char* return_text_segment_line(char** lines){
    char** parts_of_line;
    for(int i = 0;lines[i] != NULL;i++){
        parts_of_line = parser(lines[i]," ");
        printf("first part : %s\n",parts_of_line[5]);
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







