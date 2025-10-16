#pragma once

Elf64_Ehdr* get_elf_header(char* elf_path);
Elf64_Addr get_entry_point(Elf64_Ehdr* elf_header);
Elf64_Shdr* get_section_headers(Elf64_Ehdr* elf_header,char* elf_path);
Elf64_Shdr* find_shstrtab_in_section_headers(Elf64_Shdr* section_headers,Elf64_Ehdr* elf_header);
uint16_t get_number_of_section_headers(Elf64_Ehdr* elf_header);
char* get_section_headers_names(Elf64_Shdr* shstrtab_section_header,char* elf_path);
void print_section_headers(Elf64_Shdr* section_headers,char* section_header_names,uint16_t number_of_section_headers);
Elf64_Shdr* get_symtab_section_header(Elf64_Shdr* section_headers,uint16_t number_of_section_headers);
Elf64_Sym* get_symbols_from_symtab_sh(Elf64_Shdr* symtab_section_header,char* elf_path);
uint16_t get_number_of_symbols_from_symtab_sh(Elf64_Shdr* symtab_section_header);
uint32_t get_strtab_sh_index_from_symtab_sh(Elf64_Shdr* symtab_section_header);
Elf64_Shdr* get_strtab_sh_pointer(Elf64_Shdr* first_section_header,uint32_t index_number);
char* get_strtab_values(Elf64_Shdr* strtab_sh,char* elf_path);
void print_symtab_symbols(char* elf_path);
