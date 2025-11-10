#pragma once


#define MEMORY_MAPPINGS_SIZE 1024

typedef enum{
    BINARY,
    HEAP,
    STACK
}region_type;


typedef struct{
    region_type type;
    void* start;
    void* end; 
}memory_region;



void parse_lines_of_maps(char** lines);
char* read_maps(pid_t pid);
int load_proc_info(pid_t pid);