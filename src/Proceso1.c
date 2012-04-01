/* Proceso1.c */
/* $Id: Proceso1.c,v 1.1.1.1 2003/06/19 19:00:15 fcardona Exp $ */
#include <windows.h>
#include <stdio.h>

int
main(int argc, char *argv[]) {

   HANDLE hSemaforo;
   
   if (argc != 2) {
      fprintf(stderr, "Uso: %s <nombreSemaforo>\n\r");
      ExitProcess(1L);
   }   
   
   hSemaforo = OpenSemaphore(SYNCHRONIZE,
                             FALSE,
                             argv[1]);
      
   if (hSemaforo == NULL) {
      fprintf(stderr, "No se pudo crear el semaforo %ld\n\r", 
              GetLastError());
      ExitProcess(1L);
   }
                          
   for (;;) {
   
      /* Proceso entrara a sección critica */
      fprintf(stdout, "El proceso entrará a la sección critica %ld\n\r", 
              GetCurrentProcessId());
      
      WaitForSingleObject(hSemaforo, 0);
      
      fprintf(stdout, "Esta dentro de la sección critica %ld\n\r",
              GetCurrentProcessId());
    
      Sleep(1000);  
      ReleaseSemaphore(hSemaforo, 1, NULL);
   }
   
   ExitProcess(0L);
}
