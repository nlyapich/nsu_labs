#include "task_queue.h"

#include <stdlib.h>

task_queue_t* create_task_queue(int capacity)
{
  task_queue_t* queue = (task_queue_t*) malloc(sizeof(task_queue_t));
  if (!queue)
  {
    return NULL;
  }

  task_t* data = (task_t*) malloc(sizeof(task_t) * capacity);
  if (!data)
  {
    free(queue);
    return NULL;
  }

  queue->capacity = capacity;
  queue->data = data;
  queue->count = 0;
  queue->pop_index = 0;
}

int is_empty_task_queue(const task_queue_t* queue)
{
  if (!queue)
  {
    return -1;
  }

  return (queue->count == 0);
}

int is_full_task_queue(const task_queue_t* queue)
{
  if (!queue)
  {
    return -1;
  }

  return (queue->count == queue->capacity);
}

int push_task_queue(task_queue_t* queue, task_t task)
{
  if (!queue)
  {
    return -1;
  }

  if (is_full_task_queue(queue))
  {
    return -1;
  }

  int push_index = (queue->pop_index + queue->count) % queue->capacity;
  queue->data[push_index] = task;
  queue->count++;

  return 0;
}

int pop_task_queue(task_queue_t* queue, task_t* task)
{
  if (!queue)
  {
    return -1;
  }

  if (is_empty_task_queue(queue))
  {
    return -1;
  }

  *task = queue->data[queue->pop_index];
  queue->pop_index = (queue->pop_index + 1) % queue->capacity;
  queue->count--;

  return 0;
}

void delete_task_queue(task_queue_t** queue)
{
  if (!queue || !(*queue))
  {
    return;
  }

  free((*queue)->data);
  free(*queue);

  *queue = NULL;
}
