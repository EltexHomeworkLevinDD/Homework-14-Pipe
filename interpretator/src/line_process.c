#include "line_process.h"

const char* MY_CMD_DEL = "|";
const char* util_bin_dir = "../../utils/bin";

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

    // Формируем 2D массив
    *cmd_array = malloc(cmd_count * sizeof(char*));
    if (*cmd_array == NULL) {
        perror("Memory allocation");
        exit(EXIT_FAILURE);
    }

    // Разбиваем на команды
    int index = 0;
    command = strtok(line, MY_CMD_DEL);
    while (command != NULL && index < cmd_count) {
        int cmd_len = strlen(command);
        (*cmd_array)[index] = malloc((cmd_len + 1)*sizeof(char));
        if ((*cmd_array)[index] == NULL) {
            perror("Memory allocation");
            exit(EXIT_FAILURE);
        }
        strncpy((*cmd_array)[index], command, cmd_len);
        (*cmd_array)[index][cmd_len] = '\0'; // Завершаем строку нулевым символом

        command = strtok(NULL, MY_CMD_DEL);
        index++;
    }


    return cmd_count;
}

int get_util_name(char* command, char** util_name){
    // Копирую строку для получения имени утилиты без пробелов
    int len = strlen(command);
    char* command_copy = malloc((len + 1)*sizeof(char));
    if (command_copy == NULL) {
        perror("Memory allocation");
        exit(EXIT_FAILURE);
    }
    strncpy(command_copy, command, len);

    // Получаю имя утилиты без пробелов
    char* util = strtok(command_copy, " ");
    if (util == NULL){
        perror("strtok");
        exit(EXIT_FAILURE);
    }
    int util_name_len = strlen(util);
    // Память под массив
    *util_name = malloc((util_name_len+1)*sizeof(char));
    if (*util_name == NULL){
        perror("Memory allocation");
        exit(EXIT_FAILURE);
    }
    strncpy(*util_name, util, util_name_len);
    (*util_name)[util_name_len] = '\0';

    free(command_copy);

    return util_name_len;
}

int check_util(char* util_name){
    // Получию список имеющихся утилит
    struct dirent** utils = NULL;
    int entries_count = scandir(util_bin_dir, &utils, NULL, alphasort);
    if (entries_count == -1) {
        perror("scandir binary directory");
        exit(EXIT_FAILURE);
    }

    int util_found = 0;
    for (int i = 2; i < entries_count; i++){ 
        if (utils[i]->d_type == DT_REG){
            if (strcmp(util_name, utils[i]->d_name) == 0){
                util_found = 1;
            }
        }
    }

    // Освобождаем
    for (int i = 0; i < entries_count; i++){
        free(utils[i]);
    }
    free(utils); utils = NULL;
    return util_found;
}

pid_t fork_command(void){
    pid_t pid = fork();
    if (pid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    return pid;
}

int wait_command(pid_t pid){
    int status;
    waitpid(pid, &status, 0);
    return status;
}

void exec_command(char* command, char* util_name){  
    // Формируем полный путь к утилите
    char full_util_pathname[PATH_MAX];
    if (getcwd(full_util_pathname, PATH_MAX) == NULL) {
        perror("Getcwd to full_util_pathname PATH_MAX size bytes");
        exit(EXIT_FAILURE);
    }

    // Получить каноничный полный путь к директории с бинарником (без ../)
    char* canonical_dest_path = canonicalize_file_name(util_bin_dir);
    if (canonical_dest_path == NULL) {
        perror("canonicalize_file_name");
        exit(EXIT_FAILURE);
    }
    // Формирование строки полного пути к бинарнику
    snprintf(full_util_pathname, PATH_MAX, "%s/%s", canonical_dest_path, util_name);
    free(canonical_dest_path);

    // Определяем количество аргументов (без защиты '')
    int argc = 0;
    int command_len = strlen(command);
    char* command_copy = malloc((command_len+1)*sizeof(char));
    char* token = strtok(command_copy, " ");
    argc++;
    while (token != NULL){
        token = strtok(NULL, " ");
        argc++;
    }
    free(command_copy);
    token = NULL;

    // Разбиваем на пробелы (без защиты '')
    // Создаем массив аргументов для передачи в новый процесс
    char *args[argc + 1]; // +1 for NULL
    token = strtok(command, " ");
    int index = 0;
    while (token != NULL) {
        // Получаем длину текущего токена
        int len = strlen(token);
        
        // Выделяем память для текущего аргумента в массиве args
        args[index] = malloc((len + 1) * sizeof(char));
        if (args[index] == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        
        // Копируем текущий токен в массив args
        strncpy(args[index], token, len);
        args[index][len] = '\0'; // Устанавливаем завершающий нуль
        
        // Переходим к следующему токену
        token = strtok(NULL, " ");
        index++;
    }
    args[index] = NULL; // Последний элемент массива аргументов должен быть NULL

    // for (int i = 0; i < argc; i++)
    // {
    //     printf("<%d>: '%s'\n", i, args[i]);
    // }
    
    execv(full_util_pathname, args);
    // if its wrong
    perror("execv");
    exit(EXIT_FAILURE);
    // Создаём дочерний процесс для бинарника
    // pid_t cpid = fork();
    // int cstatus;
    // switch (cpid)
    // {
    // case -1:
    //     perror("fork");
    //     exit(EXIT_FAILURE);
    //     break;
    // case 0:
    //     execv(full_util_pathname, args);

    //     perror("execv");
    //     exit(EXIT_FAILURE);
    //     break;
    // default:
    //     waitpid(cpid, &cstatus, 0);
    //     break;
    // }
}
