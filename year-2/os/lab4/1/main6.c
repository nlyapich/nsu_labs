#include <stdio.h>
#include <stdlib.h>

int main()
{
  char* var = getenv("MY_VAR");
  printf("var = %s\n", var);
  setenv("MY_VAR", "NEW_VALUE", 1);
  var = getenv("MY_VAR");
  printf("var = %s\n", var);
  return 0;
}
