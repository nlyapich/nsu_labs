#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "list.h"

int fill_string_random_length(char* str, int len)
{
  static const char* alphabet = "abcdefghijklmnopqrstuvwxyz[],./;"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ{}<>?:"
                                 "`1234567890-="
                                 "~!@#$%^&*()_+";
  static const int alphabet_len = 32 * 2 + 13 * 2;
  int target_len = rand() % len;
  for (int i = 0; i < target_len; ++i)
  {
    int letter_pos = rand() % alphabet_len;
    str[i] = alphabet[letter_pos];
  }
  str[target_len] = '\0';
  return target_len;
}

int fill_string_static_length(char* str, int len)
{
  static const char* alphabet = "abcdefghijklmnopqrstuvwxyz[],./;"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ{}<>?:"
                                 "`1234567890-="
                                 "~!@#$%^&*()_+";
  static const int alphabet_len = 32 * 2 + 13 * 2;
  for (int i = 0; i < len - 1; ++i)
  {
    int letter_pos = rand() % alphabet_len;
    str[i] = alphabet[letter_pos];
  }
  str[len - 1] = '\0';
  return len - 1;
}

atomic_ullong increasing_length_total_counter = 0;
atomic_ullong decreasing_length_total_counter = 0;
atomic_ullong equal_length_total_counter = 0;
atomic_ullong swap_total_counter = 0;

void* increasing_length_checker(void* arg)
{
  Storage* storage = (Storage*)arg;
  int err;
  while (1)
  {
    Node* current = storage->first;
    if (!current)
    {
      fprintf(stderr, "list is empty\n");
      return NULL;
    }
    err = rlock_node(current);
    if (err)
    {
      perror("increasing length checker: error locking first storage element");
      return NULL;
    }
    int current_len = strlen(current->value);
    Node* next = current->next;
    int count_increasing = 0;
    while (next)
    {
      err = rlock_node(next);
      if (err)
      {
        perror("increasing length checker: error locking node");
        return NULL;
      }
      int next_len = strlen(next->value);
      if (next_len > current_len)
      {
        count_increasing++;
      }
      err = unlock_node(current);
      if (err)
      {
        perror("increasing length checker: error unlocking node");
        return NULL;
      }
      current = next;
      next = current->next;
      current_len = next_len;
    }
    printf("count_increasing = %d\n", count_increasing);
    err = unlock_node(current);
    if (err)
    {
      perror("increasing length checker: error unlocking node");
      return NULL;
    }
    atomic_fetch_add(&increasing_length_total_counter, 1);
  }
  return NULL;
}

void* decreasing_length_checker(void* arg)
{
  Storage* storage = (Storage*)arg;
  int err;
  while (1)
  {
    Node* current = storage->first;
    if (!current)
    {
      fprintf(stderr, "list is empty\n");
      return NULL;
    }
    err = rlock_node(current);
    if (err)
    {
      perror("decreasing length checker: error locking first storage element");
      return NULL;
    }
    int current_len = strlen(current->value);
    Node* next = current->next;
    int count_decreasing = 0;
    while (next)
    {
      err = rlock_node(next);
      if (err)
      {
        perror("decreasing length checker: error locking node");
        return NULL;
      }
      int next_len = strlen(next->value);
      if (next_len < current_len)
      {
        count_decreasing++;
      }
      err = unlock_node(current);
      if (err)
      {
        perror("decreasing length checker: error unlocking node");
        return NULL;
      }
      current = next;
      next = current->next;
      current_len = next_len;
    }
    printf("count_decreasing = %d\n", count_decreasing);
    err = unlock_node(current);
    if (err)
    {
      perror("decreasing length checker: error unlocking node");
      return NULL;
    }
    atomic_fetch_add(&decreasing_length_total_counter, 1);
  }
  return NULL;
}

void* equal_length_checker(void* arg)
{
  Storage* storage = (Storage*)arg;
  int err;
  while (1)
  {
    Node* current = storage->first;
    if (!current)
    {
      fprintf(stderr, "list is empty\n");
      return NULL;
    }
    err = rlock_node(current);
    if (err)
    {
      perror("equal length checker: error locking second storage element");
      return NULL;
    }
    int current_len = strlen(current->value);
    Node* next = current->next;
    int count_equal = 0;
    while (next)
    {
      err = rlock_node(next);
      if (err)
      {
        perror("equal length checker: error locking node");
        return NULL;
      }
      int next_len = strlen(next->value);
      if (next_len == current_len)
      {
        count_equal++;
      }
      err = unlock_node(current);
      if (err)
      {
        perror("equal length checker: error unlocking node");
        return NULL;
      }
      current = next;
      next = current->next;
      current_len = next_len;
    }
    printf("count_equal = %d\n", count_equal);
    err = unlock_node(current);
    if (err)
    {
      perror("equal length checker: error unlocking node");
      return NULL;
    }
    atomic_fetch_add(&equal_length_total_counter, 1);
  }
  return NULL;
}

void* swapper(void* arg)
{
  Storage* storage = (Storage*)arg;
  int err;
  while (1)
  {
    Node* base = storage->first;
    if (!base)
    {
      fprintf(stderr, "list is empty\n");
      return NULL;
    }
    err = wlock_node(base);
    if (err)
    {
      perror("swapper: error locking first element");
      return NULL;
    }
    Node* first = base->next;
    if (!first)
    {
      fprintf(stderr, "list contains only 1 element\n");
      return NULL;
    }
    err = wlock_node(first);
    if (err)
    {
      perror("swapper: error locking second element");
      return NULL;
    }
    Node* second = first->next;
    int count_swapper = 0;
    while (second)
    {
      int should_not_change = rand() % 6;
      if (should_not_change)
      {
        err = unlock_node(base);
        if (err)
        {
          perror("swapper: error unlocking base element");
          return NULL;
        }
        base = first;
        first = second;
        err = wlock_node(first);
        if (err)
        {
          perror("swapper: error locking node");
          return NULL;
        }
        second = first->next;
      }
      else
      {
        err = wlock_node(second);
        if (err)
        {
          perror("swapper: error locking element");
          return NULL;
        }
        first->next = second->next;
        second->next = first;
        base->next = second;
        err = unlock_node(base);
        if (err)
        {
          perror("swapper: error unlocking element");
          return NULL;
        }
        base = second;
        second = first->next;
        atomic_fetch_add(&swap_total_counter, 1);
        count_swapper++;
      }
    }
    printf("count_swapper = %d\n", count_swapper);
    err = unlock_node(base);
    if (err)
    {
      perror("swapper: error unlocking element");
      return NULL;
    }
    err = unlock_node(first);
    if (err)
    {
      perror("swapper: error unlocking element");
      return NULL;
    }
  }
}

void* stats_printer(void* arg)
{
  while (1)
  {
    sleep(1);
    printf("printer:\n"
           "total increasing: %llu\n"
           "total decreasing: %llu\n"
           "total equal:      %llu\n"
           "total swap:       %llu\n",
           increasing_length_total_counter, decreasing_length_total_counter,
           equal_length_total_counter, swap_total_counter);
  }
}

int main()
{
  int err;
  int storage_size = STRING_LENGTH;
  Storage* storage = new_storage();
  for (int i = 0; i < storage_size; ++i)
  {
    int len = STRING_LENGTH;
    char str[STRING_LENGTH];
    fill_string_random_length(str, len);
    add_storage(storage, str);
  }

  Node* current = storage->first;
  int counter = 0;
  while (current) {
    ++counter;
    current = current->next;
  }

  printf("COUNTER = %d\n", counter);

  pthread_t increaser, decreaser, equaler, swapper1, swapper2, swapper3, printer;
  err = pthread_create(&increaser, NULL, increasing_length_checker, storage);
  if (err)
  {
    perror("main: error creating increaser");
    return -1;
  }
  err = pthread_create(&decreaser, NULL, decreasing_length_checker, storage);
  if (err)
  {
    perror("main: error creating decreaser");
    return -1;
  }
  err = pthread_create(&equaler, NULL, equal_length_checker, storage);
  if (err)
  {
    perror("main: error creating equaler");
    return -1;
  }
  err = pthread_create(&swapper1, NULL, swapper, storage);
  if (err)
  {
    perror("main: error creating swapper1");
    return -1;
  }
  err = pthread_create(&swapper2, NULL, swapper, storage);
  if (err)
  {
    perror("main: error creating swapper2");
    return -1;
  }
  err = pthread_create(&swapper3, NULL, swapper, storage);
  if (err)
  {
    perror("main: error creating swapper3");
    return -1;
  }
  err = pthread_create(&printer, NULL, stats_printer, storage);
  if (err)
  {
    perror("main: error creating printer");
    return -1;
  }
  pthread_exit(NULL);
  return 0;
}
