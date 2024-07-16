#include <stdio.h>
#include <stdlib.h>

#include "priority_queue.h"

typedef struct {
	void* data;
} QueueItem;
struct PQueue{
	QueueItem* mas;
	size_t size;
	size_t cur_size;
	int (*compare)(const void*, const void*);
};

static void init_queue_item(QueueItem* cur_item, void* data){
	if (!cur_item) return;

	cur_item->data = data;
}

PQueue* create_pq(size_t size, int (*compare)(const void*, const void*)){
	PQueue* pqueue = (PQueue*)malloc(sizeof(PQueue));
	
	if (!pqueue) return NULL;
	
	pqueue->mas = (QueueItem*)malloc(sizeof(QueueItem) * size);
	if (!pqueue->mas) {
		free(pqueue);
		return NULL;
	}
	pqueue->size = size;
	pqueue->cur_size = 0;
	pqueue->compare = compare;

	return pqueue;
}

size_t get_size_pq(const PQueue* pqueue){
	return pqueue->cur_size;
}
static void swap(QueueItem* a, QueueItem* b){
	if (!a || !b) return;

	QueueItem tmp = *b;
	*b = *a;
	*a = tmp;
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

	return pqueue->compare(left_item->data, right_item->data) > 0 ? right: left;
}
static size_t get_pos_parent(const PQueue* pqueue, size_t pos){
	if (!pqueue) return 0;
	if (pqueue->cur_size < pos) return 0;
	if (pos == 1 || pos == 0) return 0;

	size_t pos_parent = (pos - 2) / 2 + 1;
	return pos_parent;
}
static void restore_heap_down(PQueue* pqueue, size_t pos){
	if (!pqueue) return;
	if (pqueue->cur_size < pos) return;

	size_t pos_min_child = get_pos_min_child(pqueue, pos);
	QueueItem* min_child = get_item(pqueue, pos_min_child);
	if (!min_child) return;

	QueueItem* cur_item = get_item(pqueue, pos);

	if (pqueue->compare(cur_item->data, min_child->data) > 0){
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

static void restore_heap_up(PQueue* pqueue, size_t pos){
	if (!pqueue) return;
	if (pqueue->cur_size < pos) return;

	size_t pos_parent = get_pos_parent(pqueue, pos);
	QueueItem* parent = get_item(pqueue, pos_parent);
	if (!parent) return;

	QueueItem* cur_item = get_item(pqueue, pos);

	if (pqueue->compare(cur_item->data, parent->data) < 0){
		swap(cur_item, parent);
		restore_heap_up(pqueue, pos_parent);
	}
}
void add_pq(PQueue* pqueue, void* data){
	if (!pqueue) return;
	if (pqueue->cur_size >= pqueue->size) return;

	init_queue_item(pqueue->mas + pqueue->cur_size, data);
	pqueue->cur_size++;

	restore_heap_up(pqueue, pqueue->cur_size);
}
void* extract_min_pq(PQueue* pqueue){
	if (empty_pq(pqueue)) return NULL;

	void* data = pqueue->mas[0].data;

	pqueue->cur_size--;

	update(pqueue);

	return data;
}
void del_pq(PQueue* pqueue){
	if (!pqueue) return;

	free(pqueue->mas);
	free(pqueue);
}
