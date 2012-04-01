/*
 * Sinc1.c 
 * 
 * Procesos que se sincronizan para realizar una tarea
 */
#include <windows.h>
#include <stdio.h>

int
main(int argc, char *argv[]) {
 
  HANDLE hSem;
  
  if (argc != 2) {
    fprintf(stdout, "Uso: %s <nombre semaforo>\r\n", argv[0]);
    ExitProcess((DWORD) 1);
  }
  
  hSem = CreateSemaphore(NULL,
			 0,
			 1,
			 argv[1]);
  
  if (hSem == INVALID_HANDLE_VALUE) {
    long error = GetLastError();
    char *bufferError = NULL;
    
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER, 
		  NULL, 
		  error,
		  0,
		  bufferError,
		  256,
		  NULL);
    
    fprintf(stderr, "No se pudo crear el semaforo debido a %ld: %s\n\r",
	    error, bufferError);
    ExitProcess(1L);
  }
  
  Sleep(18000);
  
  if (!ReleaseSemaphore(hSem, 1L, NULL)) {
    long error = GetLastError();
    char *bufferError = NULL;
    
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER, 
		  NULL, 
		  error,
		  0,
		  bufferError,
		  256,
		  NULL);
    
    fprintf(stderr, "No se pudo senalar al semafor debido a %ld: %s\n\r",
	    error, bufferError);
    ExitProcess(1L);   
  }
  
  ExitProcess(0L);
}
