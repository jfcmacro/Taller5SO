/* CapturarSenales.c */
/* $Id: CapturarSenales.c,v 1.1.1.1 2003/06/19 19:00:15 fcardona Exp $ */
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static int contador = 0;

void
manejador(int signo) {

  signal(SIGINT, manejador); /* Reinstala el manejador */
  ++contador;
  write(1, "Obtuvo SIGNINT\n", 15);
}

int
main(int argc, char *argv[]) {
  struct sigaction sa_old;
  struct sigaction sa_new;
  
  sa_new.sa_handler = manejador;
  sigemptyset(&sa_new.sa_mask);
  sa_new.sa_flags = 0;
  sigaction(SIGINT, &sa_new, &sa_old);

  while (contador < 4) {
    puts("Esperando por SIGINT..");
    sleep(4);
  }

  sigaction(SIGINT, &sa_old, 0);

  puts("Terminado.");
  return 0;
}
/* CapturarSenales.c */
