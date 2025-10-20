#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"


#define DELIM " \r\t\n"
#define AMOUNT_OF_COMMANDS 20
#define MAX_COMMAND_LENGTH 20

char** parser_command(char* command){
    char** commands = malloc(sizeof(char*) * AMOUNT_OF_COMMANDS);
    char* curr_substring = strtok(command,DELIM);
    int index = 0;
    while(curr_substring != NULL){
        commands[index] = malloc(MAX_COMMAND_LENGTH);
        strncpy(commands[index],curr_substring,MAX_COMMAND_LENGTH);
        commands[index][strlen(curr_substring)] = '\x00';
        curr_substring = strtok(NULL,DELIM);
        index++;
    }
    commands[index] = NULL;
    return commands;
}


int main(){
    char str[20] = "info registers";
    char** commands = parser_command(str);
    for(int i = 0; commands[i] != NULL;i++){
        printf("%s\n",commands[i]);
    }
}