#include <stdio.h>
#include <unistd.h>

int i = 1;

void f()
{
  int a[4096];

  printf("%d; %d\n", i++, getpid());
  usleep(100000);
  f();
}

int main()
{
  printf("%d\n", getpid());
  sleep(10);
  f();
  return 0;
}
