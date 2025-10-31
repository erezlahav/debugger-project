#pragma once




long get_base_adrr_from_maps_str(char* content);
long get_base_adrr_from_str_base(char* base_adress_str);
char* return_str_base_addr(char** lines);
char* read_maps(pid_t pid);
long get_base_adress(pid_t pid);