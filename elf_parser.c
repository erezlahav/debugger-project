#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include "elf_parser.h"



Elf64_Ehdr* get_elf_header(FILE* elf_file_ptr){
    Elf64_Ehdr* elf_header = malloc(sizeof(Elf64_Ehdr));
    size_t bytes_read = fread(elf_header, 1 , sizeof(Elf64_Ehdr), elf_file_ptr);
    return elf_header;
}


Elf64_Addr get_entry_point(Elf64_Ehdr* elf_header){
    return elf_header->e_entry;
}


Elf64_Shdr* get_section_headers(Elf64_Ehdr* elf_header,FILE* elf_file_ptr){
    Elf64_Off section_headers_offset = elf_header->e_shoff;
    Elf64_Half section_header_size = elf_header->e_shentsize;
    Elf64_Half number_of_section_headers = elf_header->e_shnum;

    Elf64_Shdr* section_headers_array = malloc(sizeof(Elf64_Shdr) * number_of_section_headers);

    fseek(elf_file_ptr,section_headers_offset,SEEK_SET);
    for(int i = 0; i < number_of_section_headers;i++){
        fread(&section_headers_array[i],sizeof(Elf64_Shdr),1,elf_file_ptr);
    }


    return section_headers_array;
}

Elf64_Shdr* find_shstrtab_in_section_headers(Elf64_Shdr* section_headers,Elf64_Ehdr* elf_header){
    Elf64_Shdr* shstrtab =  &section_headers[elf_header->e_shstrndx];
    return shstrtab;
}

uint16_t get_number_of_section_headers(Elf64_Ehdr* elf_header){
    return elf_header->e_shnum;
}

char* get_section_headers_names(Elf64_Shdr* shstrtab_section_header,FILE* elf_file_ptr){
    char* section_header_names = malloc(shstrtab_section_header->sh_size);
    fseek(elf_file_ptr,shstrtab_section_header->sh_offset,SEEK_SET);
    fread(section_header_names, 1, shstrtab_section_header->sh_size, elf_file_ptr);
    return section_header_names;
}

void print_section_headers(Elf64_Shdr* section_headers,char* section_header_names,uint16_t number_of_section_headers){
    for(int i = 0;i < number_of_section_headers;i++){
        printf("%s\n",(section_header_names + section_headers[i].sh_name));
    }
}

Elf64_Shdr* get_section_header_by_type(Elf64_Shdr* section_headers, uint32_t type, uint16_t number_of_section_headers){
    for(int i = 0 ; i < number_of_section_headers;i++){
        if(section_headers[i].sh_type == type){
            return &section_headers[i];
        }
    }
    return NULL;
}

Elf64_Sym* get_symbols_from_symbol_section_header(Elf64_Shdr* symbol_section_header,FILE* elf_file_ptr){

    Elf64_Sym* symbols = malloc(symbol_section_header->sh_size);
    fseek(elf_file_ptr,symbol_section_header->sh_offset,SEEK_SET);
    fread(symbols,1,symbol_section_header->sh_size,elf_file_ptr);
    return symbols;
}


uint16_t get_number_of_symbols_from_symbol_sh(Elf64_Shdr* symbol_section_header){
    return (uint16_t)(symbol_section_header->sh_size / symbol_section_header->sh_entsize);
}

uint32_t get_string_table_index_for_symbol_sh(Elf64_Shdr* symbol_section_header){
    return symbol_section_header->sh_link;
}


Elf64_Shdr* get_section_header_from_offset(Elf64_Shdr* first_section_header,uint32_t index_number){
    return first_section_header+index_number;
}

char* get_string_table_values_from_string_table_sh(Elf64_Shdr* strtab_sh,FILE* elf_file_ptr){
    char* strtab_values = malloc(strtab_sh->sh_size);

    fseek(elf_file_ptr,strtab_sh->sh_offset,SEEK_SET);
    fread(strtab_values,strtab_sh->sh_size,1,elf_file_ptr);

    return strtab_values;
}


symbol* get_symbols_from_section_header_symbol(Elf64_Shdr* symbol_section_header,Elf64_Shdr* string_table_sh, FILE* elf_file_ptr){
    Elf64_Sym* symbols = get_symbols_from_symbol_section_header(symbol_section_header,elf_file_ptr);
    char* strtab_values = get_string_table_values_from_string_table_sh(string_table_sh,elf_file_ptr);
    uint16_t number_of_symbols = get_number_of_symbols_from_symbol_sh(symbol_section_header);
    symbol_table_type table_type;
    if(symbol_section_header->sh_type == SHT_SYMTAB){
        table_type = symtab;
    }
    else if(symbol_section_header->sh_type == SHT_DYNSYM){
        table_type = dynsym;
    }
    symbol* symbols_to_return = malloc(sizeof(symbol) * number_of_symbols);
    for(int i = 0; i < number_of_symbols;i++){
        if(symbols[i].st_info == STT_FUNC){
            symbols_to_return[i].type = FUNC;
        }
        char* symbol_name = strtab_values + symbols[i].st_name;
        symbols_to_return[i].name = malloc(strlen(symbol_name)+1);
        strncpy(symbols_to_return[i].name,symbol_name, strlen(symbol_name)+1);
        symbols_to_return[i].adress = symbols[i].st_value;
        symbols_to_return[i].size = symbols[i].st_size;
        symbols_to_return[i].table_type = table_type;
    }
    return symbols_to_return;
}

symbol* get_symbols_from_file(FILE* elf_file_ptr){
    Elf64_Ehdr* elf_header = get_elf_header(elf_file_ptr);
    Elf64_Shdr* section_headers = get_section_headers(elf_header,elf_file_ptr);
    uint16_t number_of_section_headers = get_number_of_section_headers(elf_header);
    Elf64_Shdr* symtab_section_header = get_section_header_by_type(section_headers,SHT_SYMTAB,number_of_section_headers);
    Elf64_Shdr* dynsym_section_header = get_section_header_by_type(section_headers,SHT_DYNSYM,number_of_section_headers);
    symbol* symbols_to_return;
    //get symtab symbols
    symbol* symtab_symbols;
    if(symtab_section_header != NULL){
        uint32_t strtab_index = get_string_table_index_for_symbol_sh(symtab_section_header);
        Elf64_Shdr* strtab_sh = get_section_header_from_offset(section_headers,strtab_index);
        symtab_symbols = get_symbols_from_section_header_symbol(symtab_section_header,strtab_sh,elf_file_ptr);
    }
    else
    {
        symtab_symbols = NULL;
    }
    uint16_t number_of_symbols_symtab = get_number_of_symbols_from_symbol_sh(symtab_section_header);
    for(int i=0;i<number_of_symbols_symtab;i++){
        printf("%s\n",symtab_symbols[i].name);
    }
    //get dynsym symbols
    symbol* dynsym_symbols;
    if(dynsym_section_header != NULL){
        uint32_t dynstr_index = get_string_table_index_for_symbol_sh(dynsym_section_header);
        Elf64_Shdr* dynstr_sh = get_section_header_from_offset(section_headers,dynstr_index);
        dynsym_symbols = get_symbols_from_section_header_symbol(dynsym_section_header,dynstr_sh,elf_file_ptr);
    }
    else
    {
        dynsym_symbols = NULL;
    }
    uint16_t number_of_symbols_dynsym = get_number_of_symbols_from_symbol_sh(dynsym_section_header);
    for(int i=0;i<number_of_symbols_dynsym;i++){
        printf("%s\n",dynsym_symbols[i].name);
    }

    symbols_to_return = malloc(sizeof(symbol)*(number_of_symbols_symtab + number_of_symbols_dynsym));
    if(symtab_symbols != NULL){
        memcpy(symbols_to_return,symtab_symbols,number_of_symbols_symtab * sizeof(symbol));
    }
    if(dynsym_symbols != NULL){
        memcpy(symbols_to_return+number_of_symbols_symtab, dynsym_symbols, number_of_symbols_dynsym * sizeof(symbol));
    }


    return symbols_to_return;
}





