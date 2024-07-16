#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

void* mythread(void* arg)
{
	printf("mythread [%ld]: Hello from mythread!\n", pthread_self());
	return NULL;
}

int main()
{
	pthread_t tid;
	int err;

	printf("main [%d %d %d %ld]: Hello from main!\n", getpid(), getppid(), gettid(), pthread_self());

	// sleep(10);

	for (int i = 0; i < 5; ++i)
	{
		err = pthread_create(&tid, NULL, mythread, NULL);

		// printf("tid = %ld\n", tid);

		if (err)
		{
			printf("main: pthread_create() failed: %s\n", strerror(err));
			return -1;
		}

	  pthread_join(tid, NULL);
	}

  return 0;
}
