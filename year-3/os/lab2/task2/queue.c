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

	int err1 = pthread_spin_init(&(q->spinlock), PTHREAD_PROCESS_PRIVATE);
	if (err1) {
		printf("queue_init: pthread_spin_init() failed: %s\n", strerror(err1));
		abort();
	}

	int err2 = pthread_create(&q->qmonitor_tid, NULL, qmonitor, q);
	if (err2) {
		printf("queue_init: pthread_create() failed: %s\n", strerror(err2));
		abort();
	}

	return q;
}

void queue_destroy(queue_t *q) {
	int err1 = pthread_cancel(q->qmonitor_tid);
	if (err1) {
		printf("queue_destroy(): pthread_cancel() failed: %s\n", strerror(err1));
	}

	int err2 = pthread_spin_destroy(&(q->spinlock));
	if (err2) {
		printf("queue_destroy: pthread_spin_destroy() failed: %s\n", strerror(err2));
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
	pthread_spin_lock(&(q->spinlock));
	q->add_attempts++;
	assert(q->count <= q->max_count);
	if (q->count == q->max_count) {
		pthread_spin_unlock(&(q->spinlock));
		return 0;
	}
	qNode *new_node = create_node(val);
	append(q, new_node);
	pthread_spin_unlock(&(q->spinlock));

	return 1;
}

int queue_get(queue_t *q, int *val) {
	pthread_spin_lock(&(q->spinlock));
	q->get_attempts++;
	assert(q->count >= 0);
	if (q->count == 0) {
		pthread_spin_unlock(&(q->spinlock));
		return 0;
	}
	*val = pop(q);
	pthread_spin_unlock(&(q->spinlock));

	return 1;
}

void queue_print_stats(queue_t *q) {
	pthread_spin_lock(&(q->spinlock));
	const int count = q->count;
	const long add_attempts = q->add_attempts;
	const long get_attempts = q->get_attempts;
	const long add_count = q->add_count;
	const long get_count = q->get_count;
	pthread_spin_unlock(&(q->spinlock));

	printf("queue stats: current size %d; attempts: (%ld %ld %ld); counts (%ld %ld %ld)\n",
		count,
		add_attempts, get_attempts, add_attempts - get_attempts,
		add_count, get_count, add_count - get_count);
}
