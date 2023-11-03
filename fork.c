#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

/** 
 * *SIZE: tamaño de los arreglos 
 * *FIFO_PATH: nombre del archivo fifo
 */
#define SIZE 100           
#define FIFO_PATH "mi_fifo"

int main(){
    int fifo_fd;
    int pares  [SIZE];
    int impares[SIZE];
    int suma   [SIZE];

    /**
     * @brief pidA: pid del proceso hijo A
     * @brief pidB: pid del proceso hijo B
     * @brief pidC: pid del proceso hijo C
     */
    pid_t pidA, pidB, pidC;
    
    // *Creación del archivo fifo
    if (mkfifo(FIFO_PATH, 0777) == -1){
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    // *Creación del proceso hijo A
    pidA = fork();
    if (pidA == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    // *Si el proceso es el hijo A se ejecuta el código
    if (pidA == 0){
        fifo_fd = open(FIFO_PATH, O_WRONLY);
        for(int i = 0; i < 100; i++){
            pares[i] = i*2;
            write(fifo_fd, &pares[i], sizeof(int));
        }
        close(fifo_fd);
        // *Termina el proceso hijo A
        exit(0);
    }

    // *Creación del proceso hijo B
    pidB = fork();
    if (pidB == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    // *Si el proceso es el hijo B se ejecuta el código
    if (pidB == 0){
        fifo_fd = open(FIFO_PATH, O_WRONLY);

        for(int j = 0; j < 100; j++){
            impares[j] = j*2 + 1;
            write(fifo_fd, &impares[j], sizeof(int));
        }
        close(fifo_fd);
        // *Termina el proceso hijo B
        exit(0);
    }

    // *Creación del proceso hijo C
    pidC = fork();
    if (pidC == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    // *Si el proceso es el hijo C se ejecuta el código
    if (pidC == 0){
        fifo_fd = open(FIFO_PATH, O_RDONLY);

        // *Inicialización del arreglo suma
        for (int k = 0; k < 100; k++){ suma[k] = 0; }

        for (int k = 0; k < 200; k++){
            int x;
            // *Se lee del archivo fifo y se almacena en x
            read(fifo_fd, &x, sizeof(int));

            /**
             * *Se divide x entre 2 para obtener el índice del arreglo suma
             * *Por ejemplo, si x = 5, entonces x/2 = 2, por lo que se suma
             * *x al arreglo suma en la posición 2.
             * *Si x = 4, entonces x/2 = 2, por lo que se suma x al arreglo
             * *suma en la posición 2.
             * *Como resultado, en la posición 2 del arreglo suma se almacena
             * *la suma de 4 y 5.
             * *Esto se hace para todos los valores de x.
             */
            suma[x/2] += x;
        }

        // *Se imprime el arreglo suma
        for (int k = 0; k < 100; k++){ printf("suma[%d] = %d\n", k, suma[k]); }
        
        close(fifo_fd);
        // *Termina el proceso hijo C
        exit(0);
    }

    // *Espera a que terminen los procesos hijos
    wait(NULL);
    wait(NULL);
    wait(NULL);

    // *Elimina el archivo fifo
    unlink(FIFO_PATH);

    return 0;
}