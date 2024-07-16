#ifndef MY_SPINLOCK_H
#define MY_SPINLOCK_H

typedef struct
{
  int lock;
} my_spinlock_t;

int my_spinlock_init(my_spinlock_t* spin);

int my_spin_lock(my_spinlock_t* spin);

int my_spin_unlock(my_spinlock_t* spin);

#endif // MY_SPINLOCK_H
