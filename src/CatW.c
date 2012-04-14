#include <windows.h>
#include <stdio.h>

#define BUFFER_SIZE 10

int
main(int argc, char *argv[]) {

  HANDLE hStdInput, hStdOutput;
  char buffer[BUFFER_SIZE]; // Poco optimo
  int i;
  DWORD dwBytesRead, dwBytesWritten;

  hStdInput = GetStdHandle(STD_INPUT_HANDLE);
  hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);

  while (ReadFile(hStdInput            // hFile
		  , (PVOID) buffer      // pvBuffer
		  , (DWORD) BUFFER_SIZE // nNumBytesToRead
                  , &dwBytesRead        // pdwNumBytes
                  , NULL                // pOverlapped
		  )) {

    if (dwBytesRead == 0) {
      break;
    }

    for (i = 0; i < dwBytesRead; i++) {
      if (isalpha(buffer[i])) {
	buffer[i] = toupper(buffer[i]);
      }
    }

    if (!WriteFile(hStdOutput        // hFile
		   , (PVOID) buffer  // pvBuffer
                   , dwBytesRead     // pdwNumBytes
                   , &dwBytesWritten // pdwNumBytes
                   , NULL)) {
      fprintf(stderr, "Error escribiendo: %ld\r\n", GetLastError());
      ExitProcess((DWORD) 1);
    }
  }

  ExitProcess((DWORD) 0);
  return 0;
}
