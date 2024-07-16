#include "heapsort.h"

#include <stdlib.h>

static void swap(void* arr, size_t size, size_t i, size_t j){
	char* bi = (char*)arr + size*i;
	char* bj = (char*)arr + size*j; 
	for (size_t k = 0; k < size; k++){
		char tmp = *(bi + k);
		*(bi + k) = *(bj + k);
		*(bj + k) = tmp;
	}
}

static void convert_heap(void* arr, size_t size, int (*compare)(const void*, const void*), size_t len, size_t now){
	size_t most  = now;

	size_t left = 2*now + 1;
	size_t right = 2*now + 2;

	if (left < len && compare((char*)arr + left*size, (char*)arr + most*size) > 0)
		most = left;

	if (right < len && compare((char*)arr + right*size, (char*)arr + most*size) > 0)
		most = right;

	if (most != now){
		swap(arr, size, now, most);
		convert_heap(arr, size, compare, len, most);
	}
}

void heapsort(void* arr, size_t len, size_t size, int (*compare)(const void*, const void*)){
	if (!arr) return;
	
	for (size_t i = len/2; i > 0; i--)
		convert_heap(arr, size, compare, len, i - 1);

	for (size_t i = len; i > 0; i--){
		swap(arr, size, 0, i - 1);
		convert_heap(arr, size, compare, i - 1, 0);
	}
}
