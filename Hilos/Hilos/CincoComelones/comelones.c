#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_TENEDORES 5

sem_t tenedores[NUM_TENEDORES];

void pensar(int num){
    printf("El filósofo %d esta pensando...\n", num + 1);
    sleep( 1 );
    printf("HOLA\n");
}

void comer(int num){
    printf("El filósofo %d esta comiendo...\n", num + 1);
    sleep( 2 );
}

void *filosofo(void* arg) {
    int i = *(int*)arg;

    while(1){
        pensar(i);
        sem_wait(&tenedores[i]); // Espera a que se señalice el semáforo
        sem_wait(&tenedores[(i + 1) % NUM_TENEDORES]); // Espera a que se señalice el semáforo
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
        if (sem_init(&tenedores[i], 0, 0) == -1) {
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
