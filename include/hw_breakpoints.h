#pragma once


int set_hardware_breakpoint(long adress);
int clear_hardware_breakpoint(int index);
void print_dr();
int return_first_avalieble_hbp_register(unsigned long dr7_reg);
