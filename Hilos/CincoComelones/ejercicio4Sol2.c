//Solucion donde un filosofo toma el tenedor izquierdo en vez del derecho
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_TENEDORES 5

sem_t tenedores[NUM_TENEDORES];

void pensar(int num){
    printf("El filósofo %d está pensando...\n", num + 1);
    sleep(1);
}

void comer(int num){
    printf("El filósofo %d está comiendo...\n", num + 1);
    sleep(2);
}

void *filosofo(void* arg) {
    int i = *(int*)arg;
    int izquierdo = i;
    int derecho = (i + 1) % NUM_TENEDORES;

    // Cambiar el orden de espera de los tenedores para romper el deadlock
    if (i % 2 == 0) {
        int temp = izquierdo;
        izquierdo = derecho;
        derecho = temp;
    }

    while(1){
        pensar(i);
        sem_wait(&tenedores[izquierdo]);
        sem_wait(&tenedores[derecho]);
        comer(i);
        sem_post(&tenedores[izquierdo]);
        sem_post(&tenedores[derecho]);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t hilos[NUM_TENEDORES];
    int args_hilos[NUM_TENEDORES];
    
    // Inicializa los semáforos
    for (int i = 0; i < NUM_TENEDORES; i++) {
        if (sem_init(&tenedores[i], 0, 1) == -1) {
            perror("Fallo en la inicialización del semáforo");
            return 1;
        }
    }

    // Crea los hilos
    for (int i = 0; i < NUM_TENEDORES; i++) {
        args_hilos[i] = i;
        if (pthread_create(&hilos[i], NULL, filosofo, (void*)&args_hilos[i]) != 0) {
            perror("Fallo en la creación del hilo");
            return 1;
        }
    }

    // Espera a que los hilos terminen
    for (int i = 0; i < NUM_TENEDORES; i++) {
        if (pthread_join(hilos[i], NULL) != 0) {
            perror("Fallo al unir el hilo");
            return 1;
        }
    }

    // Destruye los semáforos
    for (int i = 0; i < NUM_TENEDORES; i++) {
        sem_destroy(&tenedores[i]);
    }

    printf("Fin del hilo principal\n");
    return 0;
}
