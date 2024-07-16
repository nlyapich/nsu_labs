#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>

#include "mythread.h"

void *mythread(void *arg)
{
  char *str = (char*)arg;

  for (int i = 0; i < 5; ++i)
  {
    printf("hello from my thread1 '%s'\n", str);
    sleep(1);
  }

  return "bye1";
}

void *mythread2(void *arg)
{
  char *str = (char*)arg;

  for (int i = 0; i < 5; ++i)
  {
    printf("hello from my thread2 '%s'\n", str);
    sleep(1);
  }

  return "bye2";
}

int main()
{
  mythread_t mytid;
  mythread_t mytid2;
  void *retval;
  void *retval2;

  printf("main [%d %d %d]\n", getpid(), getppid(), gettid());

  mythread_create(&mytid, mythread, "hello from main1");
  mythread_create(&mytid2, mythread2, "hello from main2");

  mythread_join(mytid, &retval);
  mythread_join(mytid2, &retval2);

  printf("main [%d %d %d] thread1 returned '%s'\n", getpid(), getppid(), gettid(), (char*)retval);
  printf("main [%d %d %d] thread2 returned '%s'\n", getpid(), getppid(), gettid(), (char*)retval2);

  return 0;
}
