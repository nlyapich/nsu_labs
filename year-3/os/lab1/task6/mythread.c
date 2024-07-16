#include "mythread.h"

#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ucontext.h>

#define PAGE 4096
#define STACK_SIZE 3*PAGE

struct _mythread
{
  int mythread_id;
  void *arg;
  start_routine_t start_routine;
  void *retval;
  volatile int finished;
  volatile int joined;

  ucontext_t before_start_routine;
};

mythread_t gtid;

int thread_startup(void *arg)
{
  mythread_t tid = (mythread_t)arg;
  mythread_struct_t *thread = tid;

  printf("thread startup: starting a thread function for the thread %d\n", thread->mythread_id);
  getcontext(&(thread->before_start_routine));

  thread->retval = thread->start_routine(thread->arg);

  thread->finished = 1;

  printf("thread_startup: waiting for join() the thread %d\n", thread->mythread_id);
  while(!thread->joined)
  {
    sleep(1);
  }

  printf("thread_startup: the thread function finished for the thread %d\n", thread->mythread_id);

  return 0;
}

void* create_stack(off_t size, int mytid)
{
  char stack_file[128];
  int stack_fd;
  void* stack;

  stack = mmap(NULL, STACK_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

  mprotect(stack + PAGE, STACK_SIZE - PAGE, PROT_READ | PROT_WRITE);

  memset(stack + PAGE, 0x7f, STACK_SIZE - PAGE);

  return stack;
}

int mythread_create(mythread_t *mytid, void *(*start_routine) (void *), void *arg)
{
  static int mythread_id = 0;
  mythread_struct_t *thread;
  int child_pid;
  void *child_stack;

  mythread_id++;

  printf("mythread_create: creating thread %d\n", mythread_id);

  child_stack = create_stack(STACK_SIZE, mythread_id);

  thread = (mythread_struct_t*)(child_stack + STACK_SIZE - sizeof(mythread_struct_t));
  thread->mythread_id = mythread_id;
  thread->arg = arg;
  thread->start_routine = start_routine;
  thread->retval = NULL;
  thread->finished = 0;
  thread->joined = 0;

  gtid = thread;

  child_stack = (void*)thread;

  printf("child_stack %p; mythread_struct %p; \n", child_stack, thread);

  child_pid = clone(thread_startup, child_stack, CLONE_VM |CLONE_FILES | CLONE_THREAD | CLONE_SIGHAND | SIGCHLD, thread);
  if (child_pid == -1)
  {
    printf("clone failed: %s\n", strerror(errno));
    exit(-1);
  }

  *mytid = thread;

  return 0;
}

void mythread_join(mythread_t mytid, void **retval)
{
  mythread_struct_t *thread = mytid;

  printf("thread_join: waiting for the thread %d to finish\n", thread->mythread_id);

  while(!thread->finished)
  {
    usleep(1);
  }

  printf("thread_join: the thread %d finished\n", thread->mythread_id);

  *retval = thread->retval;

  thread->joined = 1;
}
