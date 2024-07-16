#ifndef _SRC_PRIORITY_QUEUE_H
#define _SRC_PRIORITY_QUEUE_H

typedef unsigned int _data;

typedef struct PQueue PQueue;

PQueue* create_pq();
int empty_pq(const PQueue* pqueue);
void add_pq(PQueue* pqueue, unsigned int priority, _data data);
_data extract_min_pq(PQueue* pqueue);
void decrease_priority(PQueue* pqueue, _data data, unsigned int new_priority);
void del_pq(PQueue* pqueue);

#endif // _SRC_PRIORITY_QUEUE_H
