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

#include "line_process.h"

#define CMD_EXIT "exit"

int main(){

    char input[MAX_LINE_SZ]; 
    char** commands;
    int commands_count = 0;

    int flag = 1;
    while (flag == 1){
        printf("kekw_interpretator >: ");
        fgets(input, sizeof(input), stdin);
        input[strlen(input)-1] = '\0';

        if (strcmp(input, "exit") == 0){
            flag = 0;
            break;
        }

        commands_count = split_line(input, &commands);

        for (int i = 0; i < commands_count; i++){
            printf("cmd[%d]: '%s'\n", i, commands[i]);
        }
        
        // if (exec_command(&cmd_info) == 0){
        //     perror("Binary file not found!\n");
        //     exit(EXIT_FAILURE);
        // }

        // Освобождаем память, выделенную для массива
        for (int i = 0; i < commands_count; i++) {
            free(commands[i]);
        }
        free(commands);
    }
    
    return 0;
}