#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

void* mythread(void* arg)
{
	printf("----------------------------------------\n");
	printf("mythread [%d %d %d %ld]: Hello from mythread!\n", getpid(), getppid(), gettid(), pthread_self());
	printf("----------------------------------------\n");
	return (void*) 42;
}

int main()
{
	pthread_t tid;
	int err;

	printf("main [%d %d %d %ld]: Hello from main!\n", getpid(), getppid(), gettid(), pthread_self());

	// sleep(10);

	err = pthread_create(&tid, NULL, mythread, NULL);

	printf("tid = %ld\n", tid);

	if (err)
	{
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

  int return_value;
  pthread_join(tid, (void*) &return_value);

  printf("%d\n", return_value);

  return 0;
}
