#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#define INPUT_FILE "in.txt"
#define OUTPUT_FILE "out.txt"

void bad_input(FILE *fin, FILE *fout);
void heapsort(int* arr, const int len);
void convert_heap(int* arr, const int len, const int now);
void swap(int* arr, const int i, const int j);

int main(void){
	FILE* fin;
	FILE* fout;

	fin = fopen(INPUT_FILE, "r");
	fout = fopen(OUTPUT_FILE, "w+");

	int len = 0;
	if(fscanf(fin, "%d", &len) != 1){
		bad_input(fin, fout);
		return 0;
	}
	
	int arr[len];
	for (int i = 0; i < len; i++){
		if(fscanf(fin, "%d", &arr[i]) != 1){
			bad_input(fin, fout);
			return 0;
		}
	}

	heapsort(&arr[0], len);

	for (int i = 0; i < len; i++){
// 		printf("%d ", arr[i]);
		fprintf(fout, "%d ", arr[i]);
	}

// 	printf("\n");
	fprintf(fout, "\n");

	fclose(fin);
	fclose(fout);

	return 0;
}

void bad_input(FILE *fin, FILE *fout){
// 	printf("bad input\n");
	fprintf(fout, "bad input\n");

	fclose(fin);
	fclose(fout);
}

void heapsort(int* arr, const int len){
	for (int i = len/2 - 1; i >= 0; i--)
		convert_heap(arr, len, i);

	for (int i = len - 1; i >= 0; i--){
		swap(arr, 0, i);
		convert_heap(arr, i, 0);
	}
}

void convert_heap(int* arr, const int len, const int now){
	int most  = now;

	int left = 2*now + 1;
	int right = 2*now + 2;

	if (left < len && *(arr + left) > *(arr + most))
		most = left;

	if (right < len && *(arr + right) > *(arr + most))
		most = right;

	if (most != now){
		swap(arr, now, most);
		convert_heap(arr, len, most);
	}
}

void swap(int* arr, const int i, const int j){
	int s = *(arr + i);
	*(arr + i) = *(arr + j);
	*(arr + j) = s;
}
