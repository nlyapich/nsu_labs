#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#define INPUT_FILE "in.txt"
#define OUTPUT_FILE "out.txt"

void bad_input(FILE *fin, FILE *fout);
void quicksort(int* arr, const int low, const int high);
void partition(int* arr, int* left, int* right);
void swap(int *arr, const int i, const int j);

int main(void){

	FILE *fin;
	FILE *fout;

	fin = fopen(INPUT_FILE, "r");
	fout = fopen(OUTPUT_FILE, "w+");

	int n = 0;
	if(fscanf(fin, "%d", &n) != 1){
		bad_input(fin, fout);
		return 0;
	}
	
	int arr[n];
	for (int i = 0; i < n; i++){
		if(fscanf(fin, "%d", &arr[i]) != 1){
			bad_input(fin, fout);
			return 0;
		}
	}

	quicksort(&arr[0], 0, n - 1);

	for(int i = 0; i < n; i++){
		// printf("%d ", arr[i]);
		fprintf(fout, "%d ", arr[i]);
	}

	fprintf(fout, "\n");
	// printf("\n");

	fclose(fin);
	fclose(fout);

	return 0;
}

void bad_input(FILE *fin, FILE *fout){
	// printf("bad input\n");
	fprintf(fout, "bad input\n");
	fclose(fin);
	fclose(fout);
}

void quicksort(int* arr, const int low, const int high){
	int left = low;
	int right = high;
	
	partition(arr, &left, &right);

	if (low < right)
		quicksort (arr, low, right);
	
	if (high > left)
		quicksort (arr, left, high);
}

void partition(int* arr, int* left, int* right){
	int pivot = *(arr + (*left + *right) / 2); 
	
	while (*left <= *right) {

		while (*(arr + *left) < pivot)
			(*left)++;
		
		while (*(arr + *right) > pivot)
			(*right)--;
		
		if (*left <= *right)
			swap (arr, (*left)++, (*right)--);
	}

}

void swap(int *arr, const int i, const int j){
  int s = *(arr + i);
  *(arr + i) = *(arr + j);
  *(arr + j) = s;
}
