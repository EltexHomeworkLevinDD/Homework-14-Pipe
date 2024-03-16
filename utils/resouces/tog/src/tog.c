#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "subtog.h"

int main(int argc, char* argv[]){

    printf("Количество аргументов: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("Arg[%d]: %s\n", i, argv[i]);
    }
    
    printf("\n");
    return 0;
}