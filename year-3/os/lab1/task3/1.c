#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

struct test_t
{
  int num;
  char *str;
};

void *mythread(void *arg)
{
  struct test_t *my_arg = (struct test_t *) arg;

  printf("mythread [%d %d %d %ld]: Data: [num=%d; str=%s]\n", getpid(), getppid(), gettid(), pthread_self(), my_arg->num, my_arg->str);

  free(my_arg);
  return NULL;
}

int main()
{
  printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

  // struct test_t my_arg;
  // my_arg.num = 10;
  // my_arg.str = malloc(6);
  // strcpy(my_arg.str, "Hello");

  struct test_t* my_arg = malloc(sizeof(struct test_t));
  my_arg->num = 10;
  my_arg->str = malloc(6);
  strcpy(my_arg->str, "Hello");

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  pthread_t tid;
  // pthread_create(&tid, NULL, mythread, &my_arg);
  pthread_create(&tid, &attr, mythread, my_arg);
  printf("main [%d %d %d]: Create thread %ld\n", getpid(), getppid(), gettid(), tid);
  //
  // pthread_join(tid, NULL);
  // printf("main [%d %d %d]: Join mythread\n", getpid(), getppid(), gettid());

  // free(my_arg.str);
  // free(my_arg);

  pthread_exit(0);

  return 0;
}
