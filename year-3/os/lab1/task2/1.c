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
  // int res = 42;
  // printf("mythread [%d %d %d %ld]: Return 42\n", getpid(), getppid(), gettid(), pthread_self());
  // return (void *) res;

  int *res = malloc(sizeof(int));
  *res = 42;
  printf("mythread [%d %d %d %ld]: Return %d\n", getpid(), getppid(), gettid(), pthread_self(), *res);
  return res;
}

int main()
{
  printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

  pthread_t tid;
  pthread_create(&tid, NULL, mythread, NULL);
  printf("main [%d %d %d]: create thread %ld\n", getpid(), getppid(), gettid(), tid);

  int* return_value;
  pthread_join(tid, (void*) &return_value);

  printf("main [%d %d %d]: mythread return %d\n", getpid(), getppid(), gettid(), *(int*)return_value);
  free(return_value);

  return 0;
}
