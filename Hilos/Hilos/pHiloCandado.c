#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

int montoCuenta = 500;

pthread_mutex_t lock;

void *retira( void *p)
{
    int i = *(int *)p;
    printf("Cliente entrante = %d\n", i);
    pthread_mutex_lock( &lock );
    if ( montoCuenta >= 500 ){
        montoCuenta = montoCuenta - 500;
        printf("El cliente %d hizo un retiro exitoso\n", i);
    }
    else printf("El cliente %d no hizo un retiro exitoso\n", i);

    pthread_mutex_unlock( &lock );
    pthread_exit( NULL );
}

int main()
{
    pthread_t hilo[ 10 ];
    int clientes[] = { 1, 2 };

    for(int i = 0; i < 2; i++)
        pthread_create( &hilo[ i ], NULL, retira, &clientes[ i ] );

    for(int i = 0; i < 2; i++)
        pthread_join( hilo[ i ], NULL);

    printf("MontoCuenta = %d\n", montoCuenta );
    return 0;
}