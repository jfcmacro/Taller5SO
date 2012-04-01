/*
 * Sinc2.c
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
 
    hSem = OpenSemaphore(SYNCHRONIZE, FALSE, argv[1]);
    
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
    
    fprintf(stdout, "Esperando a que los procesos se sincronicen\r\n");
    WaitForSingleObject(hSem, INFINITE);
    
    fprintf(stdout, "Listo la sincronizacion\r\n");
    
    ExitProcess(0L);
}
