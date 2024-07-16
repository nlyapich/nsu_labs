#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  char* p = malloc(100 * sizeof(char));
  strcpy(p, "1: hello world");
  printf("%s\n", p);

  free(p);

  printf("2: %s\n", p);

  p = malloc(100 * sizeof(char));
  strcpy(p, "hello world");
  printf("%s\n", p);
  p += 50;
  free(p);
  printf("%s\n", p);

  return 0;
}
