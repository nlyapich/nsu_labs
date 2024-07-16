#ifndef _SRC_PRIORITY_QUEUE_H
#define _SRC_PRIORITY_QUEUE_H

typedef struct PQueue PQueue;

PQueue* create_pq(size_t size, int (*compare)(const void*, const void*));
size_t get_size_pq(const PQueue* pqueue);
int empty_pq(const PQueue* pqueue);
void add_pq(PQueue* pqueue, void* data);
void* extract_min_pq(PQueue* pqueue);
void del_pq(PQueue* pqueue);

#endif // _SRC_PRIORITY_QUEUE_H
