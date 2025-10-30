#pragma once




long get_base_adrr_from_maps_str(char* content);
char* return_text_segment_line(char** lines);
char* read_maps(pid_t pid);
long get_base_adress(pid_t pid);