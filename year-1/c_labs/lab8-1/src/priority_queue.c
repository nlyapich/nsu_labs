#include <stdio.h>
#include <stdlib.h>

#include "priority_queue.h"

struct QueueItem{
	_data data;
	unsigned int priority;
	struct QueueItem* prev;
	struct QueueItem* next;
};
struct PQueue{
	struct QueueItem* head;
	struct QueueItem* tail;
};

static struct QueueItem* create_queue_item(unsigned int priority, _data data){
	if (!data) return NULL;

	struct QueueItem* new_item = (struct QueueItem*)malloc(sizeof(struct QueueItem));

	if (!new_item) return NULL;

	new_item->data = data;
	new_item->priority = priority;
	new_item->prev = NULL;
	new_item->next = NULL;

	return new_item;
}

PQueue* create_pq(){
	PQueue* pqueue = (PQueue*)malloc(sizeof(PQueue));
	
	if (!pqueue) return NULL;
	
	pqueue->head = NULL;
	pqueue->tail = NULL;

	return pqueue;
}

int empty_pq(const PQueue* pqueue){
	return pqueue->head == NULL;
}

static void add_in_begin(PQueue* pqueue, struct QueueItem* new_item){
	if (!pqueue || !new_item) return;

	new_item->next = pqueue->head;
	pqueue->head->prev = new_item;
	pqueue->head = new_item;
}
static void add_after(struct QueueItem* cur, struct QueueItem* new_item){
	if (!cur || !new_item) return;

	new_item->next  = cur->next;
	if (cur->next)
		cur->next->prev = new_item;
	new_item->prev = cur;
	cur->next = new_item;
}

static void push_pq(PQueue* pqueue, struct QueueItem* new_item){
	if (!pqueue) return;
	if (!new_item) return;

	if (empty_pq(pqueue)){
		pqueue->head = pqueue->tail = new_item;
		return;
	}

	struct QueueItem* last = pqueue->tail;
	while (last && last->priority > new_item->priority){//>=
		last = last->prev;
	}

	if (!last){
		add_in_begin(pqueue, new_item);
	} else{
		add_after(last, new_item);
		if (last == pqueue->tail)
			pqueue->tail = new_item;
	}
}
void add_pq(PQueue* pqueue, unsigned int priority, _data data){
	if (!pqueue) return;

	struct QueueItem* new_item = create_queue_item(priority, data);
	if (!new_item) return;

	push_pq(pqueue, new_item);
}

_data extract_min_pq(PQueue* pqueue){
	if (empty_pq(pqueue)) return 0;

	struct QueueItem* cur = pqueue->head;
	
	_data data = cur->data;
	
	pqueue->head = cur->next;
	if (!pqueue->head)
		pqueue->tail = NULL;
	else
		pqueue->head->prev = NULL;

	free(cur);
	
	return data;
}

void decrease_priority(PQueue* pqueue, _data data, unsigned int new_priority){
	if (!pqueue || !data) return;

	struct QueueItem* tmp = pqueue->head;
	while (tmp){
		if (tmp->data == data) break;
		tmp = tmp->next;
	}

	if (!tmp) return;
	if (tmp->priority <= new_priority) return;

	tmp->priority = new_priority;

    if (!tmp->prev) return;
    
	tmp->prev->next = tmp->next;
	if (tmp->next)
		tmp->next->prev = tmp->prev;

	struct QueueItem* cur = tmp->prev;
	
	tmp->prev = NULL;
	tmp->next = NULL;

	while (cur && cur->priority > new_priority){
		cur = cur->prev;
	}

	if (!cur)
		add_in_begin(pqueue, tmp);
	else
		add_after(cur, tmp);
}

void del_pq(PQueue* pqueue){
	if (!pqueue) return;

	struct QueueItem* cur = pqueue->head;
	while (cur){
		struct QueueItem* tmp = cur;
		cur = cur->next;
		free(tmp);
	}
	free(pqueue);
}
