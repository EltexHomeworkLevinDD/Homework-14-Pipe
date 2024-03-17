#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include "subtog.h"

int main(int argc, char* argv[]){
    // Выводим аргументы
    printf("[%s] Количество аргументов: %d\n", argv[0], argc);
    for (int i = 0; i < argc; i++) {
        printf("Arg[%d]: %s\n", i, argv[i]);
    }

    // Получаем флаги stdin
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl");
        return 1;
    }
    // Устанавливаем файловый дескриптор stdin в неблокирующий режим
    if (fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl");
        return 1;
    }

    char buffer[1024]; // Увеличили размер буфера
    ssize_t bytes_read;

    fd_set rfds;
    struct timeval tv;
    int retval;

    // Ждем доступности данных во входном потоке в течение 1 секунды
    FD_ZERO(&rfds);
    FD_SET(STDIN_FILENO, &rfds);
    tv.tv_sec = 0;
    tv.tv_usec = 1;

    retval = select(STDIN_FILENO + 1, &rfds, NULL, NULL, &tv);
    if (retval == -1) {
        perror("select");
        return 1;
    } else if (retval) {
        // stdin готов к чтению
        bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
        if (bytes_read > 0) {
            // Данные доступны, выводим их
            printf("Прочитано из stdin: \n[-->'%.*s'<--]", (int)bytes_read, buffer);
        }
    } else {
        // Нет данных во входном потоке
        printf("Данные в stdin отсутствуют (нет данных для чтения).\n");
    }

    // Возвращаем stdin в блокирующий режим
    if (fcntl(STDIN_FILENO, F_SETFL, flags) == -1) {
        perror("fcntl");
        return 1;
    }

    printf("\n");
    return 0;
}