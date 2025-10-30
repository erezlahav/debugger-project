#pragma once


FILE* get_file_pointer_by_path(char* path);
FILE* get_file_ptr_by_name_from_env(char** nullble_env_paths,char* target_file);
char** parser(char* str,char* delim);
int free_double_str_ptr(char** env_paths);
