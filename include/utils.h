#pragma once


FILE* get_file_pointer_by_path(char* path);
char** get_env_paths(char* path_env);
int free_env_paths(char** env_paths);