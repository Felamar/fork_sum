#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#define SIZE 100
#define FIFO_PATH "mi_fifo"

int main(){
    int fifo_fd;
    int pares  [SIZE];
    int impares[SIZE];
    int suma   [SIZE];

    pid_t pidA, pidB, pidC;
    
    if (mkfifo(FIFO_PATH, 0777) == -1){
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    pidA = fork();
    if (pidA == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pidA == 0){
        printf("first\n");
        fifo_fd = open(FIFO_PATH, O_WRONLY);
        for(int i = 0; i < 100; i++){
            pares[i] = i*2;
            // printf("%d pair = %d\n\n", i, pares[i]);
            write(fifo_fd, &pares[i], sizeof(int));
        }
        close(fifo_fd);
        exit(0);
    }

    pidB = fork();
    if (pidB == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pidB == 0){
        printf("second\n");
        fifo_fd = open(FIFO_PATH, O_WRONLY);
        for(int j = 0; j < 100; j++){
            impares[j] = j*2 + 1;
            // printf("%d odd = %d\n\n", j, impares[j]);
            write(fifo_fd, &impares[j], sizeof(int));
        }
        close(fifo_fd);
        exit(0);
    }

    pidC = fork();
    if (pidC == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pidC == 0){
        printf("third\n");
        fifo_fd = open(FIFO_PATH, O_RDONLY);
        for (int k = 0; k < 100; k++){
            suma[k] = 0;
        }
        for (int k = 0; k < 200; k++){
            int x;
            read(fifo_fd, &x, sizeof(int));
            suma[x/2] += x;
        }
        for (int k = 0; k < 100; k++){
            printf("suma[%d] = %d\n", k, suma[k]);
        }
        close(fifo_fd);
        exit(0);
    }

    wait(NULL);
    wait(NULL);
    wait(NULL);
    unlink(FIFO_PATH);

    return 0;
}