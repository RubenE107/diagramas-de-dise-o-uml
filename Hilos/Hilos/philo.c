#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

int montoCuenta = 500;

void *retira( void *p)
{
    int i = *(int *)p;
    printf("Cliente entrante = %d\n", i);

    if ( montoCuenta >= 500 ){
        sleep( 1 );
        montoCuenta = montoCuenta - 500;
        printf("El cliente %d hizo un retiro exitoso\n", i);
    }
    else printf("El cliente %d no hizo un retiro exitoso\n", i);
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