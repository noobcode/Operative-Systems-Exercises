#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// questo programma stampa :
// the quick brown fox jumped over the lazy dogs
// the quick brown fox jumped over the lazy dogs
// perché non è stato fatto il flush dello stdout

int main (void) {
  printf("The quick brown fox jumped over ");
  //fflush(stdout);
  fork();
  printf("the lazy dogs\n");
  return 0;
}
