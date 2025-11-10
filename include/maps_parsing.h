#pragma once


#define MEMORY_MAPPINGS_SIZE 1024

typedef enum{
    BINARY,
    HEAP,
    STACK
}region_type;


typedef struct{
    region_type type;
    long start;
    long end; 
}memory_region;

typedef struct{
    memory_region arr[MEMORY_MAPPINGS_SIZE];
    int regions_index;
}regions_array;

void parse_lines_of_maps(char** lines);
char* read_maps(pid_t pid);
int load_proc_info(pid_t pid);