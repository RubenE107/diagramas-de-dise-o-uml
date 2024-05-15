#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_THREADS 4

sem_t sem[NUM_THREADS];

void* imprimir_mensaje(void* arg) {
    int num_hilo = *((int*)arg);
    printf("Número %d en la barrera.\n", num_hilo + 1);
    sem_wait(&sem[num_hilo]); // Espera a que se señalice el semáforo

    printf("El número %d ya paso la barrera.\n", num_hilo + 1);
    
    if (num_hilo < NUM_THREADS) // Señaliza el siguiente semáforo
        sem_post(&sem[(num_hilo + 1) % NUM_THREADS]);
    
    pthread_exit(NULL);
}

int main() {
    pthread_t hilos[NUM_THREADS];
    int args_hilos[NUM_THREADS];
    
    // Inicializa los semáforos
    for (int i = 0; i < NUM_THREADS; i++) {
        if (sem_init(&sem[i], 0, 0) == -1) {
            perror("Fallo en la inicialización del semáforo");
            return 1;
        }
    }

    // Crea los hilos
    for (int i = 0; i < NUM_THREADS; i++) {
        args_hilos[i] = i;
        if (pthread_create(&hilos[i], NULL, imprimir_mensaje, (void*)&args_hilos[i]) != 0) {
            perror("Fallo en la creación del hilo");
            return 1;
        }
    }

    // Inicia la ejecución señalizando el primer semáforo
    sem_post(&sem[NUM_THREADS - 1]);

    // Espera a que los hilos terminen
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(hilos[i], NULL) != 0) {
            perror("Fallo al unir el hilo");
            return 1;
        }
    }

    // Destruye los semáforos
    for (int i = 0; i < NUM_THREADS; i++) {
        sem_destroy(&sem[i]);
    }

    printf("Fin del hilo principal\n");
    return 0;
}
