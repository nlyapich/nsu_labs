#ifndef LIST_H
#define LIST_H

#include <pthread.h>
#include "my_mutex.h"

#define STRING_LENGTH 100

typedef struct _Node
{
  char value[STRING_LENGTH];
  struct _Node* next;
  my_mutex_t sync;
} Node;

typedef struct _Storage
{
  Node* first;
  Node* last;
} Storage;

int lock_init(Node* node);

int destroy_lock(Node* node);

int lock_node(Node* node);

int unlock_node(Node* node);

Storage* new_storage();

int delete_storage(Storage* storage);

int add_storage(Storage* storage, const char* str);

#endif // LIST_H
