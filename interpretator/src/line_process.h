#ifndef LINE_PROCESS_H
#define LINE_PROCESS_H

#ifndef _GNU_SOURCE // Для canonicalize_file_name() и scandir
#define _GNU_SOURCE
#endif//_GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern const char* util_bin_dir;
extern const char* MY_CMD_DEL;      // New command delimeter
#define MAX_LINE_SZ 1024            // Max line size (length = MAX_LINE_SZ-1 for '\0')

/*
Split line to commands
* line - input line
* cmd_array - empty char** 2D array of commands, , free it by yourself
returns commands count
*/
int split_line(char* line, char*** cmd_array);
/*
Returns 0 if util not found
Returns 1 if util found
util_name - empty char** pointer for util_name, free it by yourself
*/
int check_util(char* util_name);
/*
Destroys command string
*/
pid_t fork_command(void);
int wait_command(pid_t pid);
void exec_command(char* command, char* util_name);
int get_util_name(char* command, char** util_name);
#endif