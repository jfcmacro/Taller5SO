#include <windows.h>
#include <stdio.h>

int
main(int argc, char *argv[]) {

  STARTUPINFO startupInfo;
  PROCESS_INFORMATION piProcInfoP1, piProcInfoP2;
  HANDLE hReadPipe, hWritePipe;
  SECURITY_ATTRIBUTES saAttr;

  saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
  saAttr.bInheritHandle = TRUE;
  saAttr.lpSecurityDescriptor = NULL;

  // GetStartupInfo(&startupInfoP1);
  // GetStartupInfo(&startupInfoP2);

  
  if (!CreatePipe(&hReadPipe   // hReadPipe
		  ,&hWritePipe // hWritePipe
		  ,&saAttr     // lpPipeAttributes
		  ,0)) {       // nSize
    fprintf(stderr, "Error creando tuberias: %ld\n", GetLastError());
    ExitProcess((DWORD) 1); 
  }

  // Inicializando las estructuras de la informaci�n de los procesos
  ZeroMemory(&piProcInfoP1, sizeof(PROCESS_INFORMATION));

  // Inicializando la informaci�n que cada proceso necesita
  ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
  startupInfo.cb = sizeof(STARTUPINFO);
  startupInfo.hStdInput  = GetStdHandle(STD_INPUT_HANDLE);
  startupInfo.hStdOutput = hWritePipe;
  startupInfo.hStdError  = GetStdHandle(STD_ERROR_HANDLE);
  startupInfo.dwFlags    |= STARTF_USESTDHANDLES;

  if (!CreateProcess(NULL, TEXT("MostrarArchivo"), NULL, NULL,
                     TRUE, 0, NULL, NULL, &startupInfo, &piProcInfoP1)) {
    fprintf(stderr, "Error creando proceso Mostrar Archivo: %ld\n", 
	    GetLastError());
    ExitProcess((DWORD) 2);
  }

  // Inicializando las estructuras de la informaci�n de los procesos
  ZeroMemory(&piProcInfoP2, sizeof(PROCESS_INFORMATION));

  // Inicializando la informaci�n que cada proceso necesita
  ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
  startupInfo.cb = sizeof(STARTUPINFO);
  startupInfo.hStdInput  = hReadPipe;
  startupInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  startupInfo.hStdError  = GetStdHandle(STD_ERROR_HANDLE);
  startupInfo.dwFlags    |= STARTF_USESTDHANDLES;


  if (!CreateProcess(NULL, TEXT("CatW"), NULL, NULL,
                     TRUE, 0, NULL, NULL, &startupInfo, &piProcInfoP2)) {
    fprintf(stderr, "Error creando proceso CatW: %ld\n", 
	    GetLastError());
    ExitProcess((DWORD) 3);
  }

  
  WaitForSingleObject(piProcInfoP1.hProcess, INFINITE);
  WaitForSingleObject(piProcInfoP2.hProcess, INFINITE);
  
  CloseHandle(piProcInfoP1.hProcess);
  CloseHandle(piProcInfoP1.hThread);
  CloseHandle(piProcInfoP2.hProcess);
  CloseHandle(piProcInfoP2.hThread);
  ExitProcess((DWORD) 0);
}

