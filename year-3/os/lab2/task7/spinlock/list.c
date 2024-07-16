#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int lock_init(Node* node)
{
  return my_spinlock_init(&(node->sync));
}

int destroy_lock(Node* node)
{
  return 0;
}

int lock_node(Node* node)
{
  return my_spin_lock(&(node->sync));
}

int unlock_node(Node* node)
{
  return my_spin_unlock(&(node->sync));
}

Storage* new_storage()
{
  Storage* s = (Storage *)malloc(sizeof(Storage));
  if (!s)
  {
    return NULL;
  }

  s->first = NULL;
  s->last = NULL;

  return s;
}

int delete_storage(Storage* storage)
{
  int err;
  Node *current = storage->first;
  if (current)
  {
    while (current->next)
    {
      Node *temp = current->next;
      err = destroy_lock(current);
      if (err)
      {
        return err;
      }
      free(current);
      current = temp;
    }
    err = destroy_lock(current);
    if (err)
    {
      return err;
    }
    free(current);
  }
  free(storage);
  return 0;
}

int add_storage(Storage* storage, const char* str)
{
  int err;
  Node *new_node = (Node *)malloc(sizeof(Node));
  if (!new_node) {
    return -1;
  }
  new_node->next = NULL;
  err = lock_init(new_node);
  if (err) {
    free(new_node);
    return -1;
  }
  strncpy(new_node->value, str, STRING_LENGTH);
  if (storage->first)
  {
    storage->last->next = new_node;
    storage->last = storage->last->next;
  }
  else
  {
    storage->first = storage->last = new_node;
  }
  return 0;
}
