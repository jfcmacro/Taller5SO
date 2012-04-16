#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int
main() {

  pid_t child;
  int status;
  int pipe1[2];

  pipe(pipe1);

  child = fork();

  if (child == 0) { /* Ejecutar el hijo */
    dup2(pipe1[1], 1);
    close(pipe1[0]);
    close(pipe1[1]);
    execl("/bin/ls", "ls", "-l", NULL);
  }
  else { 
    child = fork();
    if (child == 0) {
      dup2(pipe1[0],0);
      close(pipe1[0]);
      close(pipe1[1]);
      execl("/usr/bin/wc", "wc", NULL);
    }
    else {
      close(pipe1[0]);
      close(pipe1[1]);
      wait(&status); 
      wait(&status); 
    }
  }

  return 0;
}
