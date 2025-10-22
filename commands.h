#pragma once


int debug_process(pid_t pid);
int continue_proc(int argc,char** argv,pid_t pid);
int breakpoint(int argc,char** argv,pid_t pid);
int exit_debugger(int argc,char** argv, pid_t pid);