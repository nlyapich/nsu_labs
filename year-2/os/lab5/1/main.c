#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int my_global_inited = 5;

int main()
{
  int my_local_inited = 10;
  int status;

  printf("my_local_inited:    0x%p (%d)\n", &my_local_inited, my_local_inited);
  printf("my_global_inited:   0x%p (%d)\n", &my_global_inited, my_global_inited);

  printf("pid %d\n", getpid());

  int f = fork();

  if (f == 0)
  {
    printf("I'm child proc. pid: %d, parent pid: %d\n", getpid(), getppid());
    printf("my_local_inited:    0x%p (%d)\n", &my_local_inited, my_local_inited);
    printf("my_global_inited:   0x%p (%d)\n", &my_global_inited, my_global_inited);

    my_global_inited = 55;
    my_local_inited = 1010;

    printf("\nAfter changing:\n");
    printf("my_local_inited:    0x%p (%d)\n", &my_local_inited, my_local_inited);
    printf("my_global_inited:   0x%p (%d)\n", &my_global_inited, my_global_inited);

    exit(5);
  }
  if (f > 0)
  {
    printf("I'm parent proc\n");
    printf("my_local_inited:    0x%p (%d)\n", &my_local_inited, my_local_inited);
    printf("my_global_inited:   0x%p (%d)\n", &my_global_inited, my_global_inited);

    sleep(10);

    printf("\nFrom parent proc. After changing from child proc:\n");
    printf("my_local_inited:    0x%p (%d)\n", &my_local_inited, my_local_inited);
    printf("my_global_inited:   0x%p (%d)\n", &my_global_inited, my_global_inited);

    if (wait(&status) >= 0)
    {

      if (WIFEXITED(status))
      {
        printf("status: %d\n", WEXITSTATUS(status));
      }
    }

  }

  sleep(10);

  return 0;
}
