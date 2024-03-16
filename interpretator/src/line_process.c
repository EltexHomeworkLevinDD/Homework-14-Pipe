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

    int index = 0;
    int cmd_len = 0;
    // Формируем 2D массив
    *cmd_array = malloc(cmd_count);

    // Разбиваем на команды
    command = strtok(line, MY_CMD_DEL);
    cmd_len = strlen(command);
    (*cmd_array)[index] = malloc(cmd_len + 1);
    if ((*cmd_array)[index] == NULL) {
        perror("Memory allocation");
        exit(EXIT_FAILURE);
    }
    strncpy((*cmd_array)[index], command, cmd_len);
    
    // Разбиваем на команды
    while (command != NULL) {
        command = strtok(NULL, MY_CMD_DEL);

        if (command != NULL){
            index++;
            int cmd_len = strlen(command);
            (*cmd_array)[index] = malloc(cmd_len + 1);
            if ((*cmd_array)[index] == NULL) {
                perror("Memory allocation");
                exit(EXIT_FAILURE);
            }
            strncpy((*cmd_array)[index], command, cmd_len);
        }
    }

    return cmd_count;
}

int check_util(char* command, char** util_name){
    // Копирую строку для получения имени утилиты без пробелов
    int len = strlen(command);
    char* command_copy = malloc(len + 1);
    if (command_copy == NULL) {
        perror("Memory allocation");
        exit(EXIT_FAILURE);
    }
    strncpy(command_copy, command, len);
    // Получаю имя утилиты без пробелов
    char* util = strtok(command_copy, " ");
    int util_name_len = strlen(util);
    printf("%d\n", util_name_len);
    printf(">'%s'\n", command_copy);
    printf(">'%s'\n", util);
    *util_name = malloc(util_name_len+1);
    if (*util_name == NULL){
        perror("Memory allocation");
        exit(EXIT_FAILURE);
    }
    strncpy(*util_name, util, util_name_len);

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
            if (strcmp(util, utils[i]->d_name) == 0){
                util_found = 1;
            }
        }
    }

    // Освобождаем
    for (int i = 0; i < entries_count; i++){
        free(utils[i]);
    }
    free(utils); utils = NULL;
    free(command_copy);
    return util_found;
}

int exec_command(char* command, char* util_name){

    // Создаем массив аргументов для передачи в новый процесс
    // +1 - Первый аргумент - имя
    // +1 - Второй аргумент - строка
    // +1 - Последний аргумент NULL
    char *args[3];
    args[0] = util_name;
    args[1] = command;
    args[2] = NULL;
  
    // Формируем полный путь к утилите
    char full_util_pathname[PATH_MAX];
    if (getcwd(full_util_pathname, PATH_MAX) == NULL) {
        perror("Getcwd to full_util_pathname PATH_MAX size bytes");
        exit(EXIT_FAILURE);
    }

    // Получить каноничный полный путь (без ../)
    char* canonical_dest_path = canonicalize_file_name(util_bin_dir);
    if (canonical_dest_path == NULL) {
        perror("canonicalize_file_name");
        exit(EXIT_FAILURE);
    }

    snprintf(full_util_pathname, PATH_MAX, "%s/%s", canonical_dest_path, util_name);
    free(canonical_dest_path);

    // Создаём дочерний процесс для бинарника
    pid_t cpid = fork();
    int cstatus;
    switch (cpid)
    {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
        break;
    case 0:
        execv(full_util_pathname, args);

        perror("execv");
        exit(EXIT_FAILURE);
        break;
    default:
        waitpid(cpid, &cstatus, 0);
        return 1;
        break;
    }
}
