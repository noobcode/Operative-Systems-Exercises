#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// la prima printf viene persa perché non si effettua il flush dello stdout

int main (void) {
	int pid;
  printf("The quick brown fox jumped over ");
  //fflush(stdout);
  pid = fork();
  if(pid)
  	execl("/bin/echo","echo","the","lazy","dogs.",NULL);
  	// la exec non ritorna
  else
  	execl("/bin/echo","echo","fuck","the","system.",NULL);
  return 0;
}
