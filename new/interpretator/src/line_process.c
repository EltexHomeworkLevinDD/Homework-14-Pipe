#include "line_process.h"

const char* MY_CMD_DEL = "|";

/*
Split line to commands
* line - input line
* cmd_array - empty char** 2D array of commands
returns commands count
*/
int split_line(char* line, char*** cmd_array){
    char* command;    
    int cmd_count = 0;

    // Количество команд
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == MY_CMD_DEL[0]) {
            cmd_count++;
        }
    }
    cmd_count++;

    int index = 0;
    int cmd_len = 0;
    // Формируем 2D массив
    *cmd_array = malloc(cmd_count);

    // Разбиваем на команды
    command = strtok(line, MY_CMD_DEL);
    printf("%s\n", command);
    cmd_len = strlen(command);
    (*cmd_array)[index] = malloc(cmd_len + 1);
    strncpy((*cmd_array)[index], command, cmd_len);
    
    // Разбиваем на команды
    while (command != NULL) {
        command = strtok(NULL, MY_CMD_DEL);

        if (command != NULL){
            index++;
            int cmd_len = strlen(command);
            (*cmd_array)[index] = malloc(cmd_len + 1);
            strncpy((*cmd_array)[index], command, cmd_len);
        }
    }

    return cmd_count;
}