#include <stdio.h>
#include <stdlib.h>

#include "priority_queue.h"

typedef struct {
	_data data;
	unsigned int priority;
} QueueItem;
struct PQueue{
	QueueItem* mas;
	size_t size;
	size_t cur_size;
};

static void init_queue_item(QueueItem* cur_item, unsigned int priority, _data data){
	if (!cur_item) return;

	cur_item->data = data;
	cur_item->priority = priority;
}

PQueue* create_pq(size_t size){
	PQueue* pqueue = (PQueue*)malloc(sizeof(PQueue));
	
	if (!pqueue) return NULL;
	
	pqueue->mas = (QueueItem*)malloc(sizeof(QueueItem) * size);
	if (!pqueue->mas) {
		free(pqueue);
		return NULL;
	}
	pqueue->size = size;
	pqueue->cur_size = 0;

	return pqueue;
}

int empty_pq(const PQueue* pqueue){
	return pqueue->cur_size == 0;
}
static QueueItem* get_item(const PQueue* pqueue, size_t pos){
	if (!pqueue) return NULL;
	if (pqueue->cur_size < pos || pos == 0) return NULL;

	return pqueue->mas + pos - 1;
}
static size_t get_pos_min_child(const PQueue* pqueue, size_t pos_parent){
	if (!pqueue) return 0;

	size_t left = 2*(pos_parent - 1) + 1 + 1;
	size_t right = 2*(pos_parent - 1) + 2 + 1;

	size_t cur_size = pqueue->cur_size;
	
	if (left > cur_size) return 0;
	if (left == cur_size) return left;

	QueueItem* left_item = get_item(pqueue, left);
	QueueItem* right_item = get_item(pqueue, right);

	return left_item->priority > right_item->priority ? right: left;
}
static void swap(QueueItem* a, QueueItem* b){
	if (!a || !b) return;

	QueueItem tmp = *b;
	*b = *a;
	*a = tmp;
}
static void restore_heap_down(PQueue* pqueue, size_t pos){
	if (!pqueue) return;
	if (pqueue->cur_size < pos) return;

	size_t pos_min_child = get_pos_min_child(pqueue, pos);
	QueueItem* min_child = get_item(pqueue, pos_min_child);
	if (!min_child) return;

	QueueItem* cur_item = get_item(pqueue, pos);

	if (cur_item->priority > min_child->priority){
		swap(cur_item, min_child);
		restore_heap_down(pqueue, pos_min_child);
	}
}
static void update(PQueue* pqueue){
	if (!pqueue) return;

	QueueItem last_item = pqueue->mas[pqueue->cur_size];
	pqueue->mas[0] = last_item;

	restore_heap_down(pqueue, 1);
}

_data extract_min_pq(PQueue* pqueue){
	if (empty_pq(pqueue)) return 0;

	_data data = pqueue->mas[0].data;

	pqueue->cur_size--;

	update(pqueue);

	return data;
}
static size_t get_pos_item(const PQueue* pqueue, _data data){
	if (!pqueue) return 0;

	size_t cur_size = pqueue->cur_size;
	QueueItem* mas = pqueue->mas;
	for (size_t i = 0; i < cur_size; ++i){
		if (mas[i].data == data) return i + 1;
	}
	return 0;
}
static size_t get_pos_parent(const PQueue* pqueue, size_t pos){
	if (!pqueue) return 0;
	if (pqueue->cur_size < pos) return 0;
	if (pos == 1 || pos == 0) return 0;

	size_t pos_parent = (pos - 2) / 2 + 1;
	return pos_parent;
}
static void restore_heap_up(PQueue* pqueue, size_t pos){
	if (!pqueue) return;
	if (pqueue->cur_size < pos) return;

	size_t pos_parent = get_pos_parent(pqueue, pos);
	QueueItem* parent = get_item(pqueue, pos_parent);
	if (!parent) return;

	QueueItem* cur_item = get_item(pqueue, pos);

	if (cur_item->priority < parent->priority){
		swap(cur_item, parent);
		restore_heap_up(pqueue, pos_parent);
	}
}
void add_pq(PQueue* pqueue, unsigned int priority, _data data){
	if (!pqueue) return;
	if (pqueue->cur_size >= pqueue->size) return;

	init_queue_item(pqueue->mas + pqueue->cur_size, priority, data);
	pqueue->cur_size++;

	restore_heap_up(pqueue, pqueue->cur_size);
}
void decrease_priority(PQueue* pqueue, _data data, unsigned int new_priority){
	if (!pqueue) return;

	size_t pos_cur_item = get_pos_item(pqueue, data);
	if (!pos_cur_item) return;

	if (pqueue->mas[pos_cur_item - 1].priority < new_priority) return;

	pqueue->mas[pos_cur_item - 1].priority = new_priority;

	restore_heap_up(pqueue, pos_cur_item);
}

void del_pq(PQueue* pqueue){
	if (!pqueue) return;

	free(pqueue->mas);
	free(pqueue);
}
