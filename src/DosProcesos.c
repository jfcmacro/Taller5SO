/* DosProcesos.c

  Este es el código muestra como conectar dos procesos a
  través de una tubería. En primer lugar se crea la tubería antes
  de crear los procesos, luego se crea el primer proceso que será
  el que lea los datos de la tuberia, por otra parte el proceso
  padre crea el segundo hijo, que establecerá conexión con el
  otro proceso a través del otro extremo de la tuberia. Una
  vez en ambos procesos hijos se ha establecido el redirecionamiento
  de los procesos, se reemplaza la imagen del proceso por el programa
  a ejecutar y estos intercambian datos. Mientras tanto el proceso
  padre espera que su hijos termine y una vez esto termine mostrar
  el estado de finalización de ambos.

  Compilación (Linux): gcc -o twoProcess twoProcess.c

  $Id: DosProcesos.c,v 1.1.1.1 2003/06/19 19:00:15 fcardona Exp $
 */

/* Archivos de cabecera necesarios para una buena compilación */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{

  pid_t ptChild1, ptChild2;       /* identificadores de los procesos */
  int valRetChild1, valRetChild2; /* Valores de retorno de los hijos */
  int fd[2];

  /* Crea la tubería que conectará los dos procesos */
  if (pipe(fd) < 0) {

    /* Algo fue mal */
    fprintf(stderr, "Error: Debido a: %d %s\n", errno, strerror(errno));
    exit(1);
  }

  /* Crea el primer hijo */
  if ((ptChild1 = fork()) == 0) {

    /* Cierra la entrada estándar (stdin) y la asocia
       a través de dup2 con el descriptor de lectura de la tuberia */
    close(0);
    dup2(fd[0], 0);
    close(fd[1]); /* Cierra por seguridad
                     el descriptor de escritura de la tuberia */

    /* Ejecuta el primer programa wc, en este punto el programa
       ya hereda como entrara estándar el descriptor de lectura 
       de la tuberia */
    execl("/usr/bin/wc", "wc", "-l", NULL);

    /* Algo malo ocurrió  */
    fprintf(stderr, "Error: este proceso nunca debería alcanzar este punto\n");
    exit(2);
  }
  else if ((ptChild2 = fork()) == 0) {

    /* Cierra la salida estándar (stdout) y la asocia
       a través de dup2 con el descriptor de escritura de la tuberia */
    close(1);
    dup2(fd[1], 1);
    close(fd[0]); /* Cierra por seguridad
                     el descriptor de lectura de la tuberia */

    execl("/bin/cat", "cat", "DosProcesos.c", NULL);

    /* Algo malo ocurrió */
    fprintf(stderr, "Error: este proceso nunca debería alcanzar este punto\n");
    exit(2);
  }

  /* Este código es ejecutado únicamente por el padre, cierra
     los descriptor de la tubería asociados debido a que no va a
     comunicarse */
  close(fd[0]);
  close(fd[1]);

  waitpid(ptChild2, &valRetChild2, 0);
  waitpid(ptChild1, &valRetChild1, 0);

  fprintf(stdout, 
	  "%d termina con estado: %d\n", ptChild2,
	  WEXITSTATUS(valRetChild2));
  fprintf(stdout, 
	  "%d termina con estado: %d\n", ptChild1,
	  WEXITSTATUS(valRetChild1));

  exit(0);
}
/* DosProcesos.c */
