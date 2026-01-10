#pragma once



#include "elf_parser.h"







int print_backtrace(int argc,char** argv);
symbol* get_symbol_by_adress(unsigned long adress);