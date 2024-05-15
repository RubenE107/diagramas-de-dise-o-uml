//Solucion con un camarero que controla uantos filosofos comen a la vez.

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_TENEDORES 5
#define MAX_COMENSALES (NUM_TENEDORES - 1)

sem_t tenedores[NUM_TENEDORES];
sem_t camarero;

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

    while(1){
        pensar(i);
        sem_wait(&camarero);
        sem_wait(&tenedores[i]);
        sem_wait(&tenedores[(i + 1) % NUM_TENEDORES]);
        sem_post(&camarero);
        comer(i);
        sem_post(&tenedores[i]);
        sem_post(&tenedores[(i + 1) % NUM_TENEDORES]);
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

    // Inicializar el semáforo del camarero
    if (sem_init(&camarero, 0, MAX_COMENSALES) == -1) {
        perror("Fallo en la inicialización del semáforo del camarero");
        return 1;
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

    // Destruir el semáforo del camarero
    sem_destroy(&camarero);

    printf("Fin del hilo principal\n");
    return 0;
}
