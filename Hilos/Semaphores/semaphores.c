#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

#define CLIENTES 7
#define LUGARES_EN_LA_MESA 4

sem_t semaforo;

unsigned int x = 0;

void *mesa( void *p ){
    sem_wait( &semaforo );

    while( 1 ) ++x;

    sem_post( & semaforo );
    pthread_exit( NULL );
}

int main(){
    int k;
    pthread_t hilo[ CLIENTES ];

    sem_init( &semaforo,
                      0,    //Semaforo entre hilos
                      LUGARES_EN_LA_MESA    //valor inicial del coontador
            );

    for(k = 0; k < CLIENTES; k++){
        pthread_create( &hilo[ k ], NULL, mesa, NULL);
    }

    for(k = 0; k < CLIENTES; k++){
        pthread_join(hilo[ k ], NULL );
    }

    sem_destroy( &semaforo );
    printf("fin hilo principal\n");
    return 0;
}