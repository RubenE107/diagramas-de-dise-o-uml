#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>

sem_t tenedores[5];
void pensar(int i);f
void comer(int i);

void *filosofo(void *p)
{
    int i=*(int*)p;
    while (1)
    {
        pensar(i);
        sem_wait(&tenedores[i]);
        sem_wait(&tenedores[(i+1)%5]);
        comer(i);
        sem_post(&tenedores[(i+1)%5]);
        sem_post(&tenedores[i]);
    }
    pthread_exit(NULL);
}

void pensar(int i)
{
    printf("Filosofo %d pensando..\n",i);
}

void comer(int i)
{
    printf("Filosofo %d comiendo..\n",i);
}

int main()
{

    pthread_t filosofos[5];
    sem_init(&tenedores,0,5);
    for (int i=0;i<5;i++)
        pthread_create(&filosofos[i],NULL,filosofo,(void*)&i);
    
    for (int i=0;i<5;i++)
        pthread_join(filosofos[i],NULL);

    sem_destroy(&tenedores);
    return 0;
}