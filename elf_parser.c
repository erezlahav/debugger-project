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




