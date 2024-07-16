#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

void *mythread(void *arg)
{
  while (1)
  {
    printf("mythread [%d %d %d %ld]: Running\n", getpid(), getppid(), gettid(), pthread_self());
  }

  return NULL;
}

int main()
{
  printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

  pthread_t tid;
  pthread_create(&tid, NULL, mythread, NULL);
  printf("main [%d %d %d]: Create thread %ld\n", getpid(), getppid(), gettid(), tid);

  sleep(2);

  pthread_cancel(tid);
  printf("main [%d %d %d]: Cancel mythread\n", getpid(), getppid(), gettid());

  pthread_join(tid, NULL);
  printf("main [%d %d %d]: Join mythread\n", getpid(), getppid(), gettid());

  return 0;
}
