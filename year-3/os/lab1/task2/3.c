#define _GNU_SOURCE
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

void *mythread(void *arg)
{
  printf("mythread [%d %d %d %ld]: Working\n", getpid(), getppid(), gettid(), pthread_self());
  pthread_detach(pthread_self());
  return NULL;
}

int main()
{
  printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  // pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  pthread_t tid;
  for (int i = 0; 1; ++i)
  {
    int e = pthread_create(&tid, &attr, mythread, NULL);
    if (e)
    {
      perror("pthread_create");
      break;
    }
    // pthread_create(&tid, NULL, mythread, NULL);
    // pthread_join(tid, NULL);
  }

  pthread_attr_destroy(&attr);
  return 0;
}
