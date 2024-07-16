#include <stdio.h>
#include <unistd.h>

int my_global_not_inited[8192];
int my_global_inited = 10;

const int my_global_const = 20;

char* my_global_pstr = "my_global hello";

void f()
{
  int my_local;
  static int my_static_not_inited;
  static int my_static_inited = 5;

  char* my_local_pstr = "my_local hello";

  const int my_local_const = 30;

  char my_local_array[] = "my_local hello array";

  printf("\nfunction variables:\n");
  printf("local:              0x%p (%d)\n", &my_local, my_local);
  printf("static inited:      0x%p (%d)\n", &my_static_inited, my_static_inited);
  printf("static not inited:  0x%p (%d)\n", &my_static_not_inited, my_static_not_inited);
  printf("local pstr:         0x%p 0x%p (%s)\n", &my_local_pstr, my_local_pstr, my_local_pstr);
  printf("local const:        0x%p (%d)\n", &my_local_const, my_local_const);
  printf("local array:        0x%p %p (%s)\n", &my_local_array, &my_local_array[5], my_local_array);
}

int main()
{
  printf("function f():       0x%p\n", f);

  printf("\nglobal variables:\n");
  printf("global inited:      0x%p (%d)\n", &my_global_inited, my_global_inited);
  printf("global not inited:  0x%p 0x%p (%d)\n", &my_global_not_inited, &my_global_not_inited[8100], my_global_not_inited);
  printf("global pstr:        0x%p 0x%p (%s)\n", &my_global_pstr, my_global_pstr, my_global_pstr);
  printf("global const:       0x%p (%d)\n", &my_global_const, my_global_const);

  f();

  printf("pid: %d\n", getpid());
  sleep(30);

  return 0;
}
