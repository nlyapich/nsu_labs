#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

int main()
{
  long size = 0;
  const int block = 1024*1024;

  printf("pid %d, size %ld\n", getpid(), size);
  sleep(10);

  while(1)
  {
    malloc(block);
    size += block;

    printf("pid %d, size %ld\n", getpid(), size);
    usleep(100000);

    if (size > block*50)
    {
      mmap(NULL, 4096, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    }

  }

  return 0;
}
