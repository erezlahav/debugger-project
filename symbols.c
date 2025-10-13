#include <elf.h>
#include <stdio.h>
#include <stdlib.h>

#include "symbols.h"




Elf64_Ehdr* get_elf_header(char* elf_path){
    FILE* elf_file_ptr= fopen(elf_path,"rb");
    Elf64_Ehdr* elf_header = malloc(sizeof(Elf64_Ehdr));
    size_t bytes_read = fread(elf_header, 1 , sizeof(Elf64_Ehdr), elf_file_ptr);
}

/*int main(){
    get_elf_header("/bin/ls");
}
*/