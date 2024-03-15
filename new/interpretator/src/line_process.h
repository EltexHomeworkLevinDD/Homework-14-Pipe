#ifndef LINE_PROCESS_H
#define LINE_PROCESS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

extern const char* MY_CMD_DEL;      // New command delimeter
#define MAX_LINE_SZ 1024            // Max line size (length = MAX_LINE_SZ-1 for '\0')

// Разбиает строку на массив команд
int split_line(char* line, char*** cmd_array);

#endif