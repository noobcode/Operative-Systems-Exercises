#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main ()
{
   char src[50], dest[50];
	int* a = malloc(23423423);
   strcpy(src,  "This is source");
   strcpy(dest, "This is destination");
	free(a); // commentoi
   strcat(dest, src);
	for(int i =0; i)
   printf("Final  destination string : |%s|", dest);
   
   return(0);
}
