#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int nCrit = 0;

void*
hilo1(void* arg)
{
  sem_t* semaforo = (sem_t *) arg;

  for (;;) {

    sem_wait(semaforo);

    nCrit++;
    if (nCrit > 1) { printf("Error en seccion critica\n"); exit(0); }
    nCrit--;

    printf("Hilo 1: %ld\n", pthread_self());
    sem_post(semaforo);
    sleep(random() % 5);
  }

  return (void *) 0;
}

void*
proceso2(void *arg)
{
  sem_t* semaforo = (sem_t *) arg;

  for (;;) {

    sem_wait(semaforo);

    nCrit++;
    if (nCrit > 1) { printf("Error en seccion critica\n"); exit(0); }
    nCrit--;
    printf("Hilo 2: %ld\n", pthread_self());
    sem_post(semaforo);
    sleep(random() % 5);
  }

   return (void *) 0;
}

int
main(int argc, char *argv[])
{
  sem_t semaforo;
  pthread_t thread1, thread2;

  srandom(time(NULL));
  sem_init(&semaforo, 0, 1);

  pthread_create(&thread1, NULL, hilo1, (void *) &semaforo);

  pthread_create(&thread2, NULL, proceso2, (void *) &semaforo);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
}
