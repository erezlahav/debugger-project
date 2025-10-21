#pragma once
#define DELIM " \r\t\n"
#define AMOUNT_OF_COMMANDS 20
#define MAX_COMMAND_LENGTH 20
char** parse_command(char* command,int* argc_out);