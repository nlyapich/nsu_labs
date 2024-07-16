#define _GNU_SOURCE

#include <pthread.h>
#include <assert.h>
#include <unistd.h>

#include "queue.h"

qNode *create_node(int val) {
	qNode *new = malloc(sizeof(qNode));
	if (!new) {
		printf("Cannot allocate memory for new node\n");
		abort();
	}
	new->val = val;
	new->next = NULL;

	return new;
}

void append(queue_t* q, qNode* qnode) {
	if (!q->first) {
	q->first = q->last = qnode;
	} else {
		q->last->next = qnode;
		q->last = q->last->next;
	}
	q->count++;
	q->add_count++;
}

int pop(queue_t *q) {
	qNode *tmp = q->first;

	int ret = tmp->val;
	q->first = q->first->next;

	free(tmp);
	q->count--;
	q->get_count++;

	return ret;
}

void *qmonitor(void *arg) {
	queue_t *q = (queue_t *) arg;

	printf("qmonitor: [%d %d %d]\n", getpid(), getppid(), gettid());

	while (1) {
		queue_print_stats(q);
		usleep(1000);
	}
}

queue_t *queue_init(int max_count) {
	queue_t *q = malloc(sizeof(queue_t));
	if (!q) {
		printf("Cannot allocate memory for a queue\n");
		abort();
	}

	q->first = NULL;
	q->last = NULL;
	q->max_count = max_count;
	q->count = 0;

	q->add_attempts = q->get_attempts = 0;
	q->add_count = q->get_count = 0;

	int err1 = sem_init(&(q->sem_available), 0, 1);
	if (err1) {
		printf("queue_init: sem_init() failed: %s\n", strerror(err1));
		abort();
	}

	int err2 = sem_init(&(q->sem_empty), 0, 0);
	if (err2) {
		printf("queue_init: sem_init() failed: %s\n", strerror(err2));
		abort();
	}

	int err3 = sem_init(&(q->sem_full), 0, max_count);
	if (err3) {
		printf("queue_init: sem_init() failed: %s\n", strerror(err3));
		abort();
	}

	int err4 = pthread_create(&q->qmonitor_tid, NULL, qmonitor, q);
	if (err4) {
		printf("queue_init: pthread_create() failed: %s\n", strerror(err4));
		abort();
	}

	return q;
}

void queue_destroy(queue_t *q) {
	int err1 = pthread_cancel(q->qmonitor_tid);
	if (err1) {
		printf("queue_destroy(): pthread_cancel() failed: %s\n", strerror(err1));
	}

	int err2 = sem_destroy(&(q->sem_available));
	if (err2) {
		printf("queue_destroy: sem_destroy() failed: %s\n", strerror(err2));
	}

	int err3 = sem_destroy(&(q->sem_empty));
	if (err3) {
		printf("queue_destroy: sem_destroy() failed: %s\n", strerror(err3));
	}

	int err4 = sem_destroy(&(q->sem_full));
	if (err4) {
		printf("queue_destroy: sem_destroy() failed: %s\n", strerror(err4));
	}

	qNode *cur = q->first;
	while (cur != NULL) {
		qNode *next = cur->next;
		free(cur);
		cur = next;
	}

	free(q);
}

int queue_add(queue_t *q, int val) {
  sem_wait(&(q->sem_full));
  sem_wait(&(q->sem_available));
  q->add_attempts++;
  assert(q->count <= q->max_count);
  qNode *new_node = create_node(val);
  append(q, new_node);
  sem_post(&(q->sem_available));
  sem_post(&(q->sem_empty));

	usleep(10);

  return 1;
}

int queue_get(queue_t *q, int *val) {
  sem_wait(&(q->sem_empty));
  sem_wait(&(q->sem_available));
  q->get_attempts++;
  assert(q->count >= 0);
  *val = pop(q);
  sem_post(&(q->sem_available));
  sem_post(&(q->sem_full));

  return 1;
}

void queue_print_stats(queue_t *q) {
	sem_wait(&(q->sem_available));
	const int count = q->count;
	const long add_attempts = q->add_attempts;
	const long get_attempts = q->get_attempts;
	const long add_count = q->add_count;
	const long get_count = q->get_count;
	sem_post(&(q->sem_available));

	printf("queue stats: current size %d; attempts: (%ld %ld %ld); counts (%ld %ld %ld)\n",
		count,
		add_attempts, get_attempts, add_attempts - get_attempts,
		add_count, get_count, add_count - get_count);
}
