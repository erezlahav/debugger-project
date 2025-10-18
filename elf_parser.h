#pragma once

typedef enum{
    symtab,
    dynsym
}symbol_table_type;

typedef enum{
    FUNC,
    GLOBAL_VAR
}symbol_type;

typedef struct{
    uint64_t adress;
    char* name;
    uint64_t size;
    symbol_type type;
    symbol_table_type table_type;
}symbol;

Elf64_Ehdr* get_elf_header(FILE* elf_file_ptr);
Elf64_Addr get_entry_point(Elf64_Ehdr* elf_header);
Elf64_Shdr* get_section_headers(Elf64_Ehdr* elf_header,FILE* elf_file_ptr);
Elf64_Shdr* find_shstrtab_in_section_headers(Elf64_Shdr* section_headers,Elf64_Ehdr* elf_header);
uint16_t get_number_of_section_headers(Elf64_Ehdr* elf_header);
char* get_section_headers_names(Elf64_Shdr* shstrtab_section_header, FILE* elf_file_ptr);
void print_section_headers(Elf64_Shdr* section_headers,char* section_header_names,uint16_t number_of_section_headers);
Elf64_Shdr* get_symtab_section_header(Elf64_Shdr* section_headers,uint16_t number_of_section_headers);
Elf64_Sym* get_symbols_from_symtab_sh(Elf64_Shdr* symtab_section_header,FILE* elf_file_ptr);
uint16_t get_number_of_symbols_from_symtab_sh(Elf64_Shdr* symtab_section_header);
uint32_t get_strtab_sh_index_from_symtab_sh(Elf64_Shdr* symtab_section_header);
Elf64_Shdr* get_strtab_sh_pointer(Elf64_Shdr* first_section_header,uint32_t index_number);
char* get_strtab_values(Elf64_Shdr* strtab_sh,FILE* elf_file_ptr);
symbol* get_symbols_from_file(FILE* elf_file_ptr);
