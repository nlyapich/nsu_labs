#ifndef MY_MUTEX_H
#define MY_MUTEX_H

#include <stdint.h>

typedef struct my_mutex
{
  uint32_t lock;
} my_mutex_t;

int my_mutex_init(my_mutex_t* m);

int my_mutex_lock(my_mutex_t* m);

int my_mutex_unlock(my_mutex_t* m);

#endif // MY_MUTEX_H
