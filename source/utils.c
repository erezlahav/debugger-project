#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/utils.h"

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
    printf("%s\n\n",path_env);
    char** env_paths = get_env_paths(path_env);
    for(int i =0;env_paths[i] != NULL;i++){
        printf("%s\n",env_paths[i]);
    }

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

