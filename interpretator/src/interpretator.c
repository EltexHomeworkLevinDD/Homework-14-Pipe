/*
3) Реализовать аналог командного интерпретатора bash. При запуске 
программы пользователю предлагается ввести имя программы и опции 
запуска программы. Программа порождает процесс и в нем выполняет 
введенную программу с заданными опциями, ждет завершения 
дочернего процесса. Снова возвращается к вводу следующей 
программы. Выход из интерпретатора по команде exit. 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "line_process.h"

#define CMD_EXIT "exit"

int main(){

    char input[MAX_LINE_SZ]; 
    char** commands;
    char* util_name = NULL;
    int commands_count = 0;

    int flag = 1;
    int utils_ok;
    while (flag == 1){
        utils_ok = 1;
        printf("kekw_interpretator >: ");
        fgets(input, sizeof(input), stdin);
        input[strlen(input)-1] = '\0';

        if (strcmp(input, "exit") == 0){
            flag = 0;
            break;
        }

        commands_count = split_line(input, &commands);

        for (int i = 0; i < commands_count; i++){
            if (!check_util(commands[i], &util_name)){
                //printf("Util '%s' not found\n", util_name);
                utils_ok = 0;
                //printf("util_name[%d]: '%s'\n", i, util_name);
                free(util_name);
                break;
            }
            //printf("util_name[%d]: '%s'\n", i, util_name);
            free(util_name);
        }
        if (utils_ok){
            for (int i = 0; i < commands_count; i++) {
                //printf("cmd[%d]: '%s'\n", i, commands[i]);
                int st = exec_command(commands[i], util_name);
                if (st != 1) {
                    perror("Exec error");
                }
            }
        }

        for (int i = 0; i < commands_count; i++) {
            free(commands[i]);
        }
        free(commands);
    }
    
    return 0;
}