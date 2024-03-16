#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "subshowdir.h"

int main(int argc, char* argv[]){

    printf("Utilit: %s\n", argv[0]);
    printf("arg1  : %s\n", argv[1]);
    
    int command_len = strlen(argv[1]);
    int i = 0;
    // Идем по пробелам до начала текста
    for (i; argv[1][i] == ' '; i++) continue;
    // Идём по тексту до начала пробела
    for (i; argv[1][i] != ' '; i++) continue;
    printf("%d", i);
    // int size = (command_len-i+1)*sizeof(char);
    // char* str = malloc(size);
    // if (str == NULL){
    //     perror("Memory allocation");
    //     free(str);
    //     exit(EXIT_FAILURE);
    // }
    // if (memcpy(str, argv[1][i], size) == NULL){
    //     perror("Memcpy");
    //     exit(EXIT_FAILURE);
    // }
    
    //free(str);
    return 0;
}