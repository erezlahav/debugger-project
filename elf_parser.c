#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include "elf_parser.h"




Elf64_Ehdr* get_elf_header(char* elf_path){
    FILE* elf_file_ptr= fopen(elf_path,"rb");
    Elf64_Ehdr* elf_header = malloc(sizeof(Elf64_Ehdr));
    size_t bytes_read = fread(elf_header, 1 , sizeof(Elf64_Ehdr), elf_file_ptr);
    fclose(elf_file_ptr);
    return elf_header;
}


Elf64_Addr get_entry_point(Elf64_Ehdr* elf_header){
    return elf_header->e_entry;
}


Elf64_Shdr* get_section_headers(Elf64_Ehdr* elf_header,char* elf_path){
    Elf64_Off section_headers_offset = elf_header->e_shoff;
    Elf64_Half section_header_size = elf_header->e_shentsize;
    Elf64_Half number_of_section_headers = elf_header->e_shnum;

    Elf64_Shdr* section_headers_array = malloc(sizeof(Elf64_Shdr) * number_of_section_headers);
    FILE* elf_file_ptr = fopen(elf_path,"rb");
    fseek(elf_file_ptr,section_headers_offset,SEEK_SET);
    for(int i = 0; i < number_of_section_headers;i++){
        fread(&section_headers_array[i],sizeof(Elf64_Shdr),1,elf_file_ptr);
    }

    fclose(elf_file_ptr);
    return section_headers_array;
}

Elf64_Shdr* find_shstrtab_in_section_headers(Elf64_Shdr* section_headers,Elf64_Ehdr* elf_header){
    Elf64_Shdr* shstrtab =  &section_headers[elf_header->e_shstrndx];
    return shstrtab;
}

uint16_t get_number_of_section_headers(Elf64_Ehdr* elf_header){
    return elf_header->e_shnum;
}

char* get_section_headers_names(Elf64_Shdr* shstrtab_section_header,char* elf_path){
    char* section_header_names = malloc(shstrtab_section_header->sh_size);
    FILE* f = fopen(elf_path,"rb");
    fseek(f,shstrtab_section_header->sh_offset,SEEK_SET);
    fread(section_header_names, 1, shstrtab_section_header->sh_size, f);
    fclose(f);
    return section_header_names;
}

void print_section_headers(Elf64_Shdr* section_headers,char* section_header_names,uint16_t number_of_section_headers){
    for(int i = 0;i < number_of_section_headers;i++){
        printf("%s\n",(section_header_names + section_headers[i].sh_name));
    }
}

Elf64_Shdr* get_symtab_section_header(Elf64_Shdr* section_headers,uint16_t number_of_section_headers){
    for(int i = 0 ; i < number_of_section_headers;i++){
        if(section_headers[i].sh_type == SHT_SYMTAB){
            return &section_headers[i];
        }
    }
    return NULL;
}


Elf64_Sym* get_symbols_from_symtab_sh(Elf64_Shdr* symtab_section_header,char* elf_path){
    uint16_t number_of_symbols = get_number_of_symbols_from_symtab_sh(symtab_section_header);
    Elf64_Sym* symbols = malloc(sizeof(Elf64_Sym) * number_of_symbols);
    FILE* elf_file_ptr = fopen(elf_path,"rb");
    fseek(elf_file_ptr,symtab_section_header->sh_offset,SEEK_SET);

    for(int i = 0; i <number_of_symbols;i++){
        fread(&symbols[i],sizeof(Elf64_Sym),1,elf_file_ptr);
    }
    fclose(elf_file_ptr);
    return symbols;
}


uint16_t get_number_of_symbols_from_symtab_sh(Elf64_Shdr* symtab_section_header){
    return (uint16_t)(symtab_section_header->sh_size / symtab_section_header->sh_entsize);
}

uint32_t get_strtab_sh_index_from_symtab_sh(Elf64_Shdr* symtab_section_header){
    return symtab_section_header->sh_link;
}


Elf64_Shdr* get_strtab_sh_pointer(Elf64_Shdr* first_section_header,uint32_t index_number){
    return first_section_header+index_number;
}

char* get_strtab_values(Elf64_Shdr* strtab_sh,char* elf_path){
    char* strtab_values = malloc(strtab_sh->sh_size);
    FILE* f = fopen(elf_path,"rb");
    fseek(f,strtab_sh->sh_offset,SEEK_SET);
    fread(strtab_values,strtab_sh->sh_size,1,f);
    fclose(f);
    return strtab_values;
}

void print_symtab_symbols(char* elf_path){
    Elf64_Ehdr* elf_header = get_elf_header(elf_path);
    Elf64_Shdr* section_headers = get_section_headers(elf_header,elf_path);
    uint16_t number_of_section_headers = get_number_of_section_headers(elf_header);
    Elf64_Shdr* symtab_section_header = get_symtab_section_header(section_headers,number_of_section_headers);
    if(symtab_section_header == NULL){
        puts("binary is stripped, no symbols");
        return;
    }
    Elf64_Sym* symbols = get_symbols_from_symtab_sh(symtab_section_header,elf_path);
    uint32_t strtab_index = get_strtab_sh_index_from_symtab_sh(symtab_section_header);
    Elf64_Shdr* strtab_sh = get_strtab_sh_pointer(section_headers,strtab_index);
    char* strtab_values = get_strtab_values(strtab_sh,elf_path);
    uint16_t number_of_symbols = get_number_of_symbols_from_symtab_sh(symtab_section_header);
    for(int i = 0;i < number_of_symbols;i++){
        printf("symbol : %s, adress : %lx\n",strtab_values + symbols[i].st_name,symbols[i].st_value);
    }
}





