#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char* argv[])
{
  int err;

  printf("pid %d\n", getpid());

  sleep(1);

  err = execl("./main5", NULL);

  printf("exec() failed: %s\n", strerror(errno));

  return 0;
}
