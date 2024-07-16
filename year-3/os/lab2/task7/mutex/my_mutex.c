#include "my_mutex.h"

#include <errno.h>
#include <linux/futex.h>
#include <stdatomic.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <unistd.h>

int my_mutex_init(my_mutex_t* m)
{
  m->lock = 0;
  return 0;
}

int my_mutex_lock(my_mutex_t* m)
{
  while (__sync_lock_test_and_set(&m->lock, 1) != 0)
  {
    syscall(SYS_futex, &m->lock, FUTEX_WAIT, 1, NULL, NULL, 0);
  }
  return 0;
}

int my_mutex_unlock(my_mutex_t* m)
{
  __sync_lock_release(&m->lock);
  syscall(SYS_futex, &m->lock, FUTEX_WAKE, 1, NULL, NULL, 0);
  return 0;
}
