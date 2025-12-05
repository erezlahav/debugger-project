#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


#include "utils.h"
#include "debug.h"

#define AMOUNT_OF_PATHS 100
#define MAX_PATH_SIZE 150
#define SIZE_OF_PATH 50
extern debugee_process process_to_debug;


char* get_elf_path_by_pid(pid_t pid){
    char* elf_path = malloc(MAX_PATH_SIZE);
    char symlink_path[MAX_PATH_SIZE];
    symlink_path[0] = '\x00'; //because strcat always search for null byte at the start to start copy the buffer to
    strcat(symlink_path,"/proc/");
    char pid_str[10];
    sprintf(pid_str, "%d", pid);
    strcat(symlink_path,pid_str);
    strcat(symlink_path,"/exe");
    ssize_t bytes_read = readlink(symlink_path,elf_path,MAX_PATH_SIZE);
    if(bytes_read == -1){
        printf("error accured!\n");
        return NULL;
    }
    elf_path[bytes_read] = '\x00';
    printf("%s\n",elf_path);
    return elf_path;
}



FILE* get_file_pointer_by_path(char* path){
    FILE* file_ptr;
    char copy_path[MAX_PATH_SIZE];
    strncpy(copy_path,path,sizeof(copy_path));
    file_ptr = fopen(path,"rb");
    if(file_ptr != NULL){
        if(path[0] == '.'){
            getcwd(process_to_debug.elf_path,MAX_PATH_SIZE);
            strcat(process_to_debug.elf_path,copy_path+1);
        }
        return file_ptr;
    }
    char* path_env;
    path_env = getenv("PATH");
    char** env_paths = parser(path_env,":");
    file_ptr = get_file_ptr_by_name_from_env(env_paths,path);
    free_double_str_ptr(env_paths);
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
            strncpy(process_to_debug.elf_path,full_path,SIZE_OF_PATH);
            return ptr_to_file;
        }
    }
    return NULL;
}


char** parser(char* str,char* delim){
    int size = AMOUNT_OF_PATHS * MAX_PATH_SIZE;
    char* copy_str = malloc(size);
    strncpy(copy_str,str,size);
    char** strings_arr = malloc(sizeof(char*) * AMOUNT_OF_PATHS);
    int current_index = 0;
    char* current_str = strtok(copy_str,delim);
    while(current_str != NULL){
        strings_arr[current_index] = malloc(MAX_PATH_SIZE);
        strncpy(strings_arr[current_index],current_str,MAX_PATH_SIZE);
        current_str = strtok(NULL,delim);
        current_index++;
    }
    free(copy_str);
    strings_arr[current_index] = NULL;
    return strings_arr;
}

int free_double_str_ptr(char** env_paths){
    for(int i = 0;env_paths[i] != NULL;i++){
        free(env_paths[i]);
    }
    free(env_paths);
}

