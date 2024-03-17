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
#define READ_PIPE 0
#define WRITE_PIPE 1

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

        // A - если одна команда, не использовать pipe, просто выполнить
        // B - если команды две, использовать 1 pipe
        // С - если команд больше двух, использовать 2 pipe
        if (utils_ok){
                // [B/C]Если команд больше одной, использовать pipe
                if (commands_count > 1)
                { 
                    // [B] Если команды всего две, использовать 1 pipe
                    if (commands_count == 2)
                    {
                        // Создать 1 pipe
                        int pipefd[2];
                        if (pipe(pipefd) == -1) {
                            perror("pipe");
                            exit(EXIT_FAILURE);
                        }
                        pid_t pid;
                        // Выполнить команды последовательно
                        for (int i = 0; i < commands_count; i++){
                            if (i == 0) 
                            {
                                pid = fork_command();
                                if (pid == 0){
                                    close(pipefd[READ_PIPE]);
                                    dup2(pipefd[WRITE_PIPE], STDOUT_FILENO);
                                    close(pipefd[WRITE_PIPE]);

                                    exec_command(commands[i], utils_names[i]);
                                }
                                wait_command(pid);
                            }
                            else 
                            {
                                pid = fork_command();
                                if (pid == 0){
                                    close(pipefd[WRITE_PIPE]);
                                    dup2(pipefd[READ_PIPE], STDIN_FILENO);
                                    close(pipefd[READ_PIPE]);

                                    exec_command(commands[i], utils_names[i]);
                                }
                                wait_command(pid);
                            }
                        }
                        close(pipefd[READ_PIPE]);
                        close(pipefd[WRITE_PIPE]);
                    } 
                    // [C] Если команд больше двух, использовать 2 pipe
                    // !!! Если канал будет заполнен, то записать ничего не выйдет 
                    else {
                        // Создать 2 pipe и сделать цикл для всех команд если их больше двух
                        int pipefd1[2];
                        int pipefd2[2];
                        if (pipe(pipefd1) == -1 || pipe(pipefd2) == -1) {
                            perror("pipe");
                            exit(EXIT_FAILURE);
                        }

                        pid_t pid;
                        for (int i = 0; i < commands_count; i++){
                            pid = fork_command();
                            if (pid == 0) {
                                // Первая команда
                                if (i == 0){
                                    close(pipefd1[READ_PIPE]);
                                    dup2(pipefd1[WRITE_PIPE], STDOUT_FILENO);
                                }
                                // Последняя команда
                                else if (i == commands_count-1){
                                    close(pipefd1[WRITE_PIPE]);
                                    dup2(pipefd1[READ_PIPE], STDIN_FILENO);
                                // Промежуточная команда
                                } else {
                                    dup2(pipefd1[READ_PIPE], STDIN_FILENO);
                                    dup2(pipefd1[WRITE_PIPE], STDOUT_FILENO);
                                }
                                exec_command(commands[i], utils_names[i]);
                            }
                            wait_command(pid);
                        }
                        

                        close(pipefd1[READ_PIPE]);
                        close(pipefd1[WRITE_PIPE]);
                        close(pipefd2[READ_PIPE]);
                        close(pipefd2[WRITE_PIPE]);
                    }
                } 
                // [A] Если команда всего одна
                else {
                    printf("Commands: 1\n");
                    pid_t pid;
                    pid = fork_command();
                    if (pid == 0) 
                        exec_command(commands[0], utils_names[0]);

                    wait_command(pid);
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