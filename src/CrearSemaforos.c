/* CrearSemaforos.c */
/* $Id: CrearSemaforos.c,v 1.1.1.1 2003/06/19 19:00:15 fcardona Exp $ */
#include <windows.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
    HANDLE hSemaphore;
    int valorInicial = 0;
    int valorMaximo = 10000;
    
    if (argc < 2 || argc > 4) {
        fprintf(stderr, "Uso: %s <nombreSemaforo> [valorInicial] [valorMaximo]\n\r");
        ExitProcess(1L);
    }
    
    if (argc == 3) {
        valorInicial = atoi(argv[2]);
        if (valorInicial < 0) {
           fprintf(stderr, "El valorInicial debe ser mayor o igual a cero\n\r");
           ExitProcess(1L);
        }
    }
    
    if (argc == 4) {
        valorMaximo = atoi(argv[3]);
        if (valorMaximo < 0 && valorMaximo < valorInicial) {
           fprintf(stderr, "El valorMáximo debe ser mayor que cero y mayor que valorInical\n\r");
           ExitProcess(1L);
        }
    }
    
    hSemaphore = CreateSemaphore(NULL, // Atributos de seguridad
                                 valorInicial, // Valor inicial
                                 valorMaximo,  // Valor maximo
                                 argv[1]);     // Nombre
                                 
    if (hSemaphore == NULL) {
       long error = GetLastError();
       char *bufferError;
       
       FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER, 
                     NULL, 
                     error,
                     0,
                     bufferError,
                     256,
                     NULL);
                     
       fprintf(stderr, "No se pudo crear el semaforo debido a %ld: %s\n\r",
               GetLastError(), bufferError);
       ExitProcess(1L);
    }
    
    fprintf(stdout, "Todo ha salido bien\n\r");
    
    for (;;) {
    }
    ExitProcess(0L);
}         
