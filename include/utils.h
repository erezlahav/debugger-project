#pragma once


FILE* get_file_pointer_by_path(char* path);
FILE* get_file_ptr_by_name_from_env(char** nullble_env_paths,char* target_file);
char** get_env_paths(char* path_env);
int free_env_paths(char** env_paths);
