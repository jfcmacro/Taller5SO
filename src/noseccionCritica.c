#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
 

void
hijo() {

  for (;;) {
    fprintf(stdout, "Entrando a la sección critica");
    fflush(stdout);
    
    sleep(10);
    
     fprintf(stdout, "Voy a salir de la sección critica");
     fflush(stdout);
  }
}

void
padre() {

  for (;;) {
    fprintf(stdout, "Entrando a la sección critica");
    fflush(stdout);
    
    sleep(10);
    
     fprintf(stdout, "Voy a salir de la sección critica");
     fflush(stdout);
   }
}

int
main(int argc, char *argv[])
{
  pid_t processID;
  
  processID = fork();

   if (processID < 0) {
     fprintf(stderr, "No se pudo crear el proceso hijo");
     exit(1);
   }
   
   fprintf(stdout, "Listos los procesos\n");
   
   switch (processID) {

   case 0:
     hijo();
     break;
     
   default:
     padre();
     break;
   }
   
   exit(0);
}
