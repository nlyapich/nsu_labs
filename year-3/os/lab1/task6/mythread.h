#ifndef MYTHREAD_H
#define MYTHREAD_H

#define _GNU_SOURCE

typedef void *(*start_routine_t) (void*);

struct _mythread;
typedef struct _mythread mythread_struct_t;
typedef mythread_struct_t* mythread_t;

int mythread_create(mythread_t *mytid, void *(*start_routine) (void *), void *arg);
void mythread_join(mythread_t mytid, void **retval);

#endif // MYTHREAD_H
