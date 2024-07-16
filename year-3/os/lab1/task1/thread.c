#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

int my_global = 5;

void *mythread(void *arg)
{
	printf("----------------------------------------\n");
	int my_local = 10;
	const int my_const_local = 15;
	static int my_static_local = 20;
  printf("my_global:   0x%p (%d)\n", &my_global, my_global);
	printf("my_local:    0x%p (%d)\n", &my_local, my_local);
  printf("my_const_local:   0x%p (%d)\n", &my_const_local, my_const_local);
  printf("my_static_local:   0x%p (%d)\n", &my_static_local, my_static_local);
	printf("mythread [%d %d %d %ld]: Hello from mythread!\n", getpid(), getppid(), gettid(), pthread_self());
	my_global++;
	my_local++;
	my_static_local++;
	printf("----------------------------------------\n");
	return NULL;
}

int main()
{
	const int count_threads = 5;
	pthread_t tid[count_threads];
	int err[count_threads];

	printf("main [%d %d %d %ld]: Hello from main!\n", getpid(), getppid(), gettid(), pthread_self());

	sleep(10);

	for (int i = 0; i < count_threads; ++i)
	{
		err[i] = pthread_create(tid + i, NULL, mythread, NULL);
		sleep(2);
	}

	for (int i = 0; i < count_threads; ++i)
	{
		printf("tid[%d] = %ld\n", i, tid[i]);
	}

	for (int i = 0; i < count_threads; ++i)
	{
		if (err[i])
		{
			printf("main: pthread_create() failed: %s\n", strerror(err[i]));
			// return -1;
		}
	}

	printf("%d\n", pthread_equal(tid[0], tid[1]));

	sleep(20);

	pthread_exit(0);
}
