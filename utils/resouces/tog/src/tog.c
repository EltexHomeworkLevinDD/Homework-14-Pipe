#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "subtog.h"

int main(int argc, char* argv[]){
    printf("Количество аргументов: %d\n", argc);
    printf("Аргументы командной строки:\n");

    printf("Utilit: %s\n", argv[0]);
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-'){
            printf("Key: %s", argv[i]);
            if (argv[i+1][0] != '-'){
                printf("\tValue: %s\n", argv[i+1]);
                i++;
            }
            else {
                printf("\n");
            }
        }
        else{
            printf("Arg: %s\n", argv[i]);
        }
    }

    return 0;
}