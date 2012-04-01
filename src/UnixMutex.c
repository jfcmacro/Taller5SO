/* UnixMutex.c */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void*
f_hilo1(void *arg) {
  pthread_mutex_t *mutex_control;
  int i;

  mutex_control = (pthread_mutex_t *) arg;

  for (i = 0; i < 3; i++) {
    
    fprintf(stdout, "Hilo 1 va entrar en la región critica\n");
    pthread_mutex_lock(mutex_control);
    fprintf(stdout, "Hilo 1 dentro de la region critica\n");
    sleep(3);
    pthread_mutex_unlock(mutex_control);
    fprintf(stdout, "Hilo 1 por fuera de la sección critica");
  }

  return (void *) 0;
}

void*
f_hilo2(void *arg) {
  pthread_mutex_t *mutex_control;
  int i;

  mutex_control = (pthread_mutex_t *) arg;

  for (i = 0; i < 4; i++) {
    
    fprintf(stdout, "Hilo 2 va entrar en la región critica\n");
    pthread_mutex_lock(mutex_control);
    fprintf(stdout, "Hilo 2 dentro de la region critica\n");
    sleep(4);
    pthread_mutex_unlock(mutex_control);
    fprintf(stdout, "Hilo 2 por fuera de la sección critica");
  }

  return (void *) 0;
}

int
main() {

  /* Ejecuta el hilo 1 */
  pthread_mutex_t mutex_control = PTHREAD_MUTEX_INITIALIZER;
  pthread_t hilo1;
  pthread_t hilo2;

  pthread_create(&hilo1, NULL, f_hilo1, (void *) &mutex_control);
  pthread_create(&hilo2, NULL, f_hilo2, (void *) &mutex_control);

  pthread_join(hilo1, NULL);
  pthread_join(hilo2, NULL);

  exit(0);
}
