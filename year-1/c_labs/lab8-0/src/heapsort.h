#ifndef _SRC_HEAPSORT_H_
#define _SRC_HEAPSORT_H_

#include <stddef.h>

void heapsort(void* arr, size_t len, size_t size, int (*compare)(const void*, const void*));

#endif // _SRC_HEAPSORT_H_
