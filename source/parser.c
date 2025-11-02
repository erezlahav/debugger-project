#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"


#define DELIM " \r\t\n"
#define AMOUNT_OF_COMMANDS 20
#define MAX_COMMAND_LENGTH 150

char** parse_command(char* command,int* argc_out){
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
    *argc_out = index;
    return commands;
}
