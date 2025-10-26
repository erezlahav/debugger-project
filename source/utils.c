#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

#define AMOUNT_OF_PATHS 100
#define MAX_PATH_SIZE 70

FILE* get_file_pointer_by_path(char* path){
    FILE* file_ptr;
    file_ptr = fopen(path,"rb");
    if(file_ptr != NULL){
        return file_ptr;
    }
    char* path_env;
    path_env = getenv("PATH");
    char** env_paths = get_env_paths(path_env);
    file_ptr = get_file_ptr_by_name_from_env(env_paths,path);
    free_env_paths(env_paths);
    return file_ptr;
}

FILE* get_file_ptr_by_name_from_env(char** nullble_env_paths,char* target_file){
    char slash[100];
    strcpy(slash,"/");
    char* file_name = strcat(slash,target_file);
    char* full_path;
    FILE* ptr_to_file;
    for(int i = 0; nullble_env_paths[i] != NULL;i++){
        full_path = strcat(nullble_env_paths[i],file_name);
        ptr_to_file = fopen(full_path,"rb");
        if(ptr_to_file != NULL){
            return ptr_to_file;
        }
    }
    return NULL;
}



char** get_env_paths(char* path_env){
    char** env_paths = malloc(sizeof(char*) * AMOUNT_OF_PATHS);
    int current_index = 0;
    char* current_path = strtok(path_env,":");
    while(current_path != NULL){
        env_paths[current_index] = malloc(MAX_PATH_SIZE);
        strncpy(env_paths[current_index],current_path,MAX_PATH_SIZE);
        current_path = strtok(NULL,":");
        current_index++;
    }
    env_paths[current_index] = NULL;
    return env_paths;
}



int free_env_paths(char** env_paths){
    for(int i = 0;env_paths[i] != NULL;i++){
        free(env_paths[i]);
    }
    free(env_paths);
}

