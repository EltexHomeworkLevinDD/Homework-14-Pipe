#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

/* --------------------*/

#define SWITCH_MODE

/* --------------------*/

#ifdef SWITCH_MODE

#define P_READ_PIPE 0
#define P_WRITE_PIPE 1
#define C_READ_PIPE 0
#define C_WRITE_PIPE 1

int main(){
    int pipedes[2]; 
    if (pipe(pipedes) == -1){
        perror("Pipe creation error");
        exit(EXIT_FAILURE);
    }

    pid_t child = fork();
    if (child == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    int child_st;

    if (child == 0){ // --> Child
        close(pipedes[C_WRITE_PIPE]); 
        char rmsg[20];
        int readed = 0;

        // Чтение
        while ((readed = read(pipedes[C_READ_PIPE], rmsg, sizeof(rmsg))) > 0){
            printf("%.*s", readed, rmsg);
        }

        if (readed == -1) {
            perror("Read error");
            close(pipedes[C_READ_PIPE]);
            exit(EXIT_FAILURE);
        }
        printf("\n");
        close(pipedes[C_READ_PIPE]);
        exit(EXIT_SUCCESS);
        // Child <--
    } else { // Parent -->
        close(pipedes[P_READ_PIPE]);
        char smsg[] = "Hello, world!";
        write(pipedes[P_WRITE_PIPE], smsg, sizeof(smsg));
        close(pipedes[P_WRITE_PIPE]);
        waitpid(child, &child_st, 0);
    }

    return 0;
}

#endif // SWITCH_MODE

#ifndef SWITCH_MODE

#include <sys/stat.h>
#define FIFO_PATH "/tmp/my_fifo"

int main(){
    
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    pid_t child = fork();
    if (child == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    int child_st;

    // --> Child
    if (child == 0){
        int fd;
        char buffer[20];

        fd = open(FIFO_PATH, O_RDONLY);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        int readed = 0;

        while ((readed = read(fd, buffer, sizeof(buffer))) > 0){
            printf("%.*s", readed, buffer);
        }

        if (readed == -1) {
            perror("Read error");
            close(fd);
            exit(EXIT_FAILURE);
        }
        close(fd);
        printf("\n");
        exit(EXIT_SUCCESS);
        // Child <--
    } else { // Parent -->
        int fd;
        char buffer[] = "Hello, process!";

        fd = open(FIFO_PATH, O_WRONLY);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        write(fd, buffer, sizeof(buffer));
        close(fd);
        
        unlink(FIFO_PATH);
        waitpid(child, &child_st, 0);
    }

}
#endif // SWITCH_MODE