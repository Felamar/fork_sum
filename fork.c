#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define SIZE 100
#define FIFO_PATH "mi_fifo"
int main(){
    int fifo_fd;
    int pares  [SIZE];
    int impares[SIZE];
    int suma   [SIZE];
    pid_t pidA, pidB, pidC;
    
    if (mkfifo(FIFO_PATH, 0666) == -1){
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    pidA = fork();
    if (pidA == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pidA == 0){
        fifo_fd = open(FIFO_PATH, O_WRONLY);
        for(int i = 0; i < 100; i++){
            pares[i] = i*2;
            suma [i] = 0;
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
        fifo_fd = open(FIFO_PATH, O_WRONLY);
        for(int i = 0; i < 100; i++){
            impares[i] = i*2 + 1;
            write(fifo_fd, &impares[i], sizeof(int));
        }
        close(fifo_fd);
        exit(0);
    }
    if (pidC == 0){
        fifo_fd = open(FIFO_PATH, O_RDONLY);
        for(int i = 0; i < 100; i++){
            int pair;
            read(fifo_fd, &pair, sizeof(int));
            printf("%d\n", &pair);
            suma[i] += pair;
        }
        close(fifo_fd);
        exit(0);
    }
    else{
        fifo_fd = open(FIFO_PATH, O_RDONLY);
        for(int i = 0; i < 100; i++){
            int odd;
            read(fifo_fd, &odd, sizeof(int));
            printf("%d\n",& odd);
            suma[i] += odd;
        }
        close(fifo_fd);
        exit(0);
    }
}