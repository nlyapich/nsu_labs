#include <mpi.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "task_queue.h"

#define TASK_COUNT              2000
#define REQUEST_TAG             0
#define RESPONSE_TAG            1
#define EMPTY_QUEUE_RESPONSE    (-1)
#define TERMINATION_SIGNAL      (-2)

int process_count = 0;
int rank = 0;
int proc_sum_weight = 0;
int termination = 0;
task_queue_t* task_queue = NULL;

pthread_mutex_t mutex;
pthread_cond_t executor_cond;
pthread_cond_t receiver_cond;

void init_tasks()
{
  const int TOTAL_SUM_WEIGHT = 50000000;
  int min_weight = 2 * TOTAL_SUM_WEIGHT / (TASK_COUNT * (process_count + 1));
  int task_id = 1;

  for (int i = 0; i < TASK_COUNT; ++i)
  {
    // Create task
    task_t task =
    {
      .id = task_id,
      .rank = rank,
      .weight = min_weight * (i % process_count + 1)
    };

    if (i % process_count == rank)
    {
      push_task_queue(task_queue, task);
      task_id++;
      proc_sum_weight += task.weight;
    }
  }
}

void execute_tasks()
{
  while (1)
  {
    task_t task;

    pthread_mutex_lock(&mutex);
    if (is_empty_task_queue(task_queue))
    {
      pthread_mutex_unlock(&mutex);
      break;
    }
    pop_task_queue(task_queue, &task);
    pthread_mutex_unlock(&mutex);

    // printf("Worker %d executing task %d of process %d and weight %d\n", rank, task.id, task.rank, task.weight);

    // в место этого можно вставить вычисления
    usleep(task.weight);
  }
}

void* executor_thread_func(void* args)
{
  init_tasks();

  MPI_Barrier(MPI_COMM_WORLD);

  while(1)
  {
    execute_tasks();

    pthread_mutex_lock(&mutex);
    while (is_empty_task_queue(task_queue) && !termination)
    {
      pthread_cond_signal(&receiver_cond);
      pthread_cond_wait(&executor_cond, &mutex);
    }

    if (termination)
    {
      pthread_mutex_unlock(&mutex);
      break;
    }
    pthread_mutex_unlock(&mutex);
  }

  printf("Executor %d finished\n", rank);
  pthread_exit(0);
}

void* receiver_thread_func(void* args)
{
  int termination_signal = TERMINATION_SIGNAL;

  while (!termination)
  {
    int received_tasks = 0;
    task_t task;

    pthread_mutex_lock(&mutex);
    while (!is_empty_task_queue(task_queue))
    {
      pthread_cond_wait(&receiver_cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    for (int i = 0; i < process_count; ++i)
    {
      if (i == rank)
      {
        continue;
      }

      // printf("Receiver %d sent request to process %d\n", rank, i);
      MPI_Send(&rank, 1, MPI_INT, i, REQUEST_TAG, MPI_COMM_WORLD);
      MPI_Recv(&task, sizeof(task), MPI_BYTE, i, RESPONSE_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      if (task.id != EMPTY_QUEUE_RESPONSE)
      {
        // printf("Receiver %d received task %d from process %d\n", rank, task.id, i);

        pthread_mutex_lock(&mutex);
        push_task_queue(task_queue, task);
        pthread_mutex_unlock(&mutex);

        received_tasks++;
      }
    }

    if (received_tasks == 0)
    {
      pthread_mutex_lock(&mutex);
      termination = 1;
      pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&executor_cond);
    pthread_mutex_unlock(&mutex);
  }

  // Receiver destruction synchronization
  MPI_Barrier(MPI_COMM_WORLD);

  // printf("Receiver %d sent termination signal\n", rank);
  MPI_Send(&termination_signal, 1, MPI_INT, rank, REQUEST_TAG, MPI_COMM_WORLD);

  printf("Receiver %d finished\n", rank);

  pthread_exit(0);
}

void* sender_thread_func(void* args)
{
  while (1)
  {
    int receive_process_id;
    task_t task;

    // printf("Sender %d waiting for request\n", rank);
    MPI_Recv(&receive_process_id, 1, MPI_INT, MPI_ANY_SOURCE, REQUEST_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    if (receive_process_id == TERMINATION_SIGNAL)
    {
      // printf("Sender %d received termination signal\n", rank);
      break;
    }

    // printf("Sender %d received request from process %d\n", rank, receive_process_id);

    pthread_mutex_lock(&mutex);
    if (!is_empty_task_queue(task_queue))
    {
      pop_task_queue(task_queue, &task);
      // printf("Sender %d sent task %d of process %d to process %d\n", rank, task.id, task.rank, receive_process_id);
    }
    else
    {
      task.id = EMPTY_QUEUE_RESPONSE;
      task.weight = 0;
      task.rank = rank;
      // printf("Sender %d sent empty queue response to process %d\n", rank, receive_process_id);
    }
    pthread_mutex_unlock(&mutex);

    MPI_Send(&task, sizeof(task), MPI_BYTE, receive_process_id, RESPONSE_TAG, MPI_COMM_WORLD);
  }

  printf("Sender %d finished\n", rank);

  pthread_exit(0);
}

int main()
{
  int provided;

  double start_time;
  double end_time;

  pthread_t executor_thread;
  pthread_t receiver_thread;
  pthread_t sender_thread;

  MPI_Init_thread(NULL, NULL, MPI_THREAD_MULTIPLE, &provided);
  if (provided != MPI_THREAD_MULTIPLE)
  {
    printf("Error\n");
    MPI_Finalize();
    return 0;
  }

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &process_count);

  // Create task queue
  task_queue = create_task_queue(TASK_COUNT);

  // Initialize mutex and condition variable
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&executor_cond, NULL);
  pthread_cond_init(&receiver_cond, NULL);

  if (rank == 0)
  {
    start_time = MPI_Wtime();
  }

  pthread_create(&executor_thread, NULL, executor_thread_func, NULL);
  pthread_create(&receiver_thread, NULL, receiver_thread_func, NULL);
  pthread_create(&sender_thread, NULL, sender_thread_func, NULL);

  pthread_join(executor_thread, NULL);
  pthread_join(receiver_thread, NULL);
  pthread_join(sender_thread, NULL);

  MPI_Barrier(MPI_COMM_WORLD);

  if (rank == 0)
  {
    end_time = MPI_Wtime();
    printf("Time: %lf\n", end_time - start_time);
  }

  // MPI_Barrier(MPI_COMM_WORLD);
  // printf("Summary weight %d: %d\n", rank, proc_sum_weight);

  delete_task_queue(&task_queue);

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&executor_cond);
  pthread_cond_destroy(&receiver_cond);

  MPI_Finalize();

  return 0;
}
