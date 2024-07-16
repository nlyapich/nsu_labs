#ifndef TASK_QUEUE_H_
#define TASK_QUEUE_H_

typedef struct
{
  int id;
  int rank;
  int weight;
} task_t;

typedef struct
{
  task_t* data;
  int capacity;
  int count;
  int pop_index;
} task_queue_t;

task_queue_t* create_task_queue(int capacity);
int is_empty_task_queue(const task_queue_t* queue);
int is_full_task_queue(const task_queue_t* queue);
int push_task_queue(task_queue_t* queue, task_t task);
int pop_task_queue(task_queue_t* queue, task_t* task);
void delete_task_queue(task_queue_t** queue);

#endif // TASK_QUEUE_H_
