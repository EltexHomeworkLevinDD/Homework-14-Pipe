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
        char** utils_names = malloc(commands_count*sizeof(char*));
        if (utils_names == NULL) {
            perror("Memory allocation");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < commands_count; i++)
            utils_names[i] = NULL;
        

        // Проверка
        for (int i = 0; i < commands_count; i++){
            get_util_name(commands[i], &(utils_names[i]));

            if (!check_util(utils_names[i])){
                utils_ok = 0;
                printf("Util '%s' not found\n", utils_names[i]);
                break;
            }
        }

        // Выполнение
        if (utils_ok){
            for (int i = 0; i < commands_count; i++) {
                exec_command(commands[i], utils_names[i]);
            }
        }

        for (int i = 0; i < commands_count; i++) {
            if (commands[i] != NULL)
                free(commands[i]);
            if (utils_names[i] != NULL)
                free(utils_names[i]);
        }
        if (commands != NULL)
            free(commands);
        if (utils_names != NULL)
            free(utils_names);
    }
    
    return 0;
}