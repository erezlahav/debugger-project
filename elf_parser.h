#pragma once

Elf64_Ehdr* get_elf_header(char* elf_path);
Elf64_Addr get_entry_point(Elf64_Ehdr* elf_header);
Elf64_Shdr* get_section_headers(Elf64_Ehdr* elf_header,char* elf_path);
Elf64_Shdr* find_shstrtab_in_section_headers(Elf64_Shdr* section_headers,Elf64_Ehdr* elf_header);
char* get_section_headers_names(Elf64_Shdr* shstrtab_section_header,char* elf_path);
void print_section_headers(Elf64_Shdr* section_headers,char* section_header_names,uint16_t number_of_section_headers);