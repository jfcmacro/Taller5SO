/*
 * MostrarArchivo.c
 * 
 * Un archivo que se muestra así mismo.
 */
#include <windows.h>
#include <stdio.h>

#define BUFFER_SIZE 10

int
main(int argc, char *argv[]) {
  HANDLE hFile,hStdOutput;
  char buffer[BUFFER_SIZE]; // Poco optimo
  DWORD dwBytesRead, dwBytesWritten;
  BOOL  bTerminar;

  hFile = CreateFile(TEXT("MostrarArchivo.c")      // pszName
		     , GENERIC_READ          // dwDesiredAccess 
		     , FILE_SHARE_READ       // dwShareMode
		     , NULL                  // psa
                     , OPEN_EXISTING         // dwCreationDisposition
                     , FILE_ATTRIBUTE_NORMAL // dwFlagsAndAttributes
                     , NULL                  // hFileTemplate
                     );

  if (hFile == INVALID_HANDLE_VALUE) {
    fprintf(stderr, "Error abriendo: %ld\r\n", GetLastError());
    ExitProcess((DWORD) 1);
  }

  hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);

  if (hStdOutput == INVALID_HANDLE_VALUE) {
    fprintf(stderr, "Error obteniendo descriptor: %ld\r\n", GetLastError());
    ExitProcess((DWORD) 1);
  }

  while (ReadFile(hFile                  // hFile
		  , (void *) buffer      // pvBuffer
                  , (DWORD) BUFFER_SIZE  // nNumBytesToRead
                  , &dwBytesRead         // pdwNumBytes
                  , NULL                 // pOverlapped
                  )) {

    if (dwBytesRead == 0) {
      break;
    }

    

    if (!WriteFile(hStdOutput         // hFile
		   , (void *) buffer  // pvBuffer
                   , dwBytesRead      // nNumBytesToWrite
                   , &dwBytesWritten  // pdwNumBytes
                   , NULL             // pOverlapped
                   )) {
      fprintf(stderr, "MostrarArchivo - Error escribiendo: %ld \r\n", GetLastError());
      ExitProcess((DWORD) 3);
    }
  }
  CloseHandle(hFile);
  ExitProcess((DWORD) 0);
  return 0; // Esto no es necesario
}
