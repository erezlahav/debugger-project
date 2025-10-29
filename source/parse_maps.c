#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../include/parse_maps.h"

#define CHUNK_SIZE 1024



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
    printf("%s\n",content);
    
}





