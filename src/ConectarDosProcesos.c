#include <windows.h>
#include <stdio.h>

int
main(int argc, char *argv[]) {

  STARTUPINFO startupInfoP1, startupInfoP2;
  PROCESS_INFORMATION piProcInfoP1, piProcInfoP2;
  HANDLE hReadPipe, hWritePipe;

  GetStartupInfo(&startupInfoP1);
  GetStartupInfo(&startupInfoP2);

  if (!CreatePipe(&hReadPipe   // hReadPipe
		  ,&hWritePipe // hWritePipe
		  ,NULL        // lpPipeAttributes
		  ,0)) {       // nSize
    fprintf(stderr, "Error creando tuberias: %ld\n", GetLastError());
    ExitProcess((DWORD) 1); 
  }

  startupInfoP1.hStdInput  = GetStdHandle(STD_INPUT_HANDLE);
  startupInfoP1.hStdOutput = hWritePipe;
  startupInfoP1.hStdError  = GetStdHandle(STD_ERROR_HANDLE);
  startupInfoP2.hStdInput  = hReadPipe;
  startupInfoP2.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  startupInfoP2.hStdError  = GetStdHandle(STD_ERROR_HANDLE);

  if (!CreateProcess(NULL, TEXT("MostrarArchivo"), NULL, NULL,
                     FALSE, 0, NULL, NULL, &startupInfoP1, &piProcInfoP1)) {
    fprintf(stderr, "Error creando proceso Mostrar Archivo: %ld\n", 
	    GetLastError());
    ExitProcess((DWORD) 2);
  }

  if (!CreateProcess(NULL, TEXT("CatW"), NULL, NULL,
                     FALSE, 0, NULL, NULL, &startupInfoP2, &piProcInfoP2)) {
    fprintf(stderr, "Error creando proceso CatW: %ld\n", 
	    GetLastError());
    ExitProcess((DWORD) 3);
  }

  WaitForObject(piProcinfoP2.hProcess, 0);
  ExitProcess((DWORD) 0);
}
  
