#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
/* la union semun se define al incluir <sys/sem.h> */
#else
/* según X/OPEN tenemos que definirla nosostros mismos */
union semun {
  int val;                    /* valor para SETVAL */
  struct semid_ds *buf;       /* buffer para IPC_STAT, IPC_SET */
  unsigned short int *array;  /* array para GETALL, SETALL */
  struct seminfo *__buf;      /* buffer para IPC_INFO */
};
#endif

void
hijo(int semid) {
  
  struct sembuf semBuff;
  
  for (;;) {
    semBuff.sem_num = 0;
    semBuff.sem_op = -1;
    semBuff.sem_flg = 0;
    
    fprintf(stdout, "Entrando a la sección critica");
    fflush(stdout);
    
    if (semop(semid, &semBuff, 1) < 0) {
      
      fprintf(stderr, "Error al entrar en la sección critica\n");
      exit(1);
    }

    sleep(10);

    fprintf(stdout, "Voy a salir de la sección critica");
    fflush(stdout);
    
    semBuff.sem_num = 0;
    semBuff.sem_op = 1;
    semBuff.sem_flg = 0;
    
    if (semop(semid, &semBuff, 1) < 0) {
      
      fprintf(stderr, "Error al entrar en la sección critica\n");
      exit(1);
    }
    fprintf(stdout, "Estoy fuera de la sección critica\n");
    fflush(stdout);
  }
}

void
padre(int semid) {
  struct sembuf semBuff;
  
  for (;;) {
    semBuff.sem_num = 0;
    semBuff.sem_op = -1;
    semBuff.sem_flg = 0;
    
    fprintf(stdout, "Entrando a la sección critica");
    fflush(stdout);
    
    if (semop(semid, &semBuff, 1) < 0) {
      
      fprintf(stderr, "Error al entrar en la sección critica\n");
      exit(1);
    }
    
    sleep(5);
    
    fprintf(stdout, "Voy a salir de la sección critica");
    fflush(stdout);

    semBuff.sem_num = 0;
    semBuff.sem_op = 1;
    semBuff.sem_flg = 0;
    
    if (semop(semid, &semBuff, 1) < 0) {
      
      fprintf(stderr, "Error al entrar en la sección critica\n");
      exit(1);
    }
    fprintf(stdout, "Estoy fuera de la sección critica\n");
    fflush(stdout);
  }
}

int
main(int argc, char *argv[])
{
  int semid;
  key_t key;
  pid_t processID;
  mode_t um;
  mode_t mode;
  int flags;
  union semun un;
  
  if (argc != 2) {
    fprintf(stderr, "Uso: %s <clave>\n", argv[0]);
    exit(1);
  }
  
  key = (key_t) atoi(argv[1]);
  
  um = umask(077);
  umask(um);
  mode = 0666 & ~um;
  flags = IPC_CREAT | IPC_EXCL; // Lo crea exclusivo
  
  if ((semid = semget(key, 1, mode|flags)) < 0) {
    
    fprintf(stderr, "Error al crear el semaforo %ld", errno);
    exit(1);
  }

  fprintf(stdout, "Se va a inicializar el semáforo\n");
  
  /* Inicializando el valor del semaforo */
  un.val = 1;
  if (semctl(semid, 0, SETVAL, un) < 0) {
    fprintf(stderr, "Error al cambiar el valor del semaforo %ld", errno);
    exit(1);
  }
  
  processID = fork();
  
  if (processID < 0) {
    fprintf(stderr, "No se pudo crear el proceso hijo");
    exit(1);
  }
  

  fprintf(stdout, "Listos los procesos\n");
  
  switch (processID) {

  case 0:
    hijo(semid);
    break;
    
  default:
    padre(semid);
    break;
   }
  
  exit(0);
}
