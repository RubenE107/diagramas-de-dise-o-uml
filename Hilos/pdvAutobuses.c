#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

int maxBoletos = 10;
int boletos[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int contador = 0;

pthread_mutex_t lock;

void *compra( void *p)
{
    int i = *(int *)p;
    printf("Cliente entrante = %d\n", i);
    pthread_mutex_lock( &lock );
    if ( boletos[contador] == -1){
        boletos[contador] = i;
        printf("El cliente %d hizo un retiro exitoso\n", i);
        contador += 1;
    }
    else printf("El cliente %d no hizo un retiro exitoso\n", i);

    pthread_mutex_unlock( &lock );
    pthread_exit( NULL );
}

int main()
{
    pthread_t hilo[ 10 ];
    int clientes[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    for(int i = 0; i < 10; i++)
        pthread_create( &hilo[ i ], NULL, compra, &clientes[ i ] );

    for(int i = 0; i < 2; i++)
        pthread_join( hilo[ i ], NULL);

    for(int i = 0; i < maxBoletos; i++)
        printf("Asiento %d -> Cliente %d\n", i, boletos[i]);
    return 0;
}