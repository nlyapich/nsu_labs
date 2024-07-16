#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE  "in.txt"
#define OUTPUT_FILE "out.txt"
#define SIZE_OF_TEMPLATE 13

void open_files(FILE **fin, FILE **fout);
void close_files(FILE *fin, FILE *fout);

void bad_input(FILE *fin, FILE *fout);

int dijkstra(FILE *fin, FILE *fout);

int get_template(char *template, FILE *fin);
int check(const char template[]);
int factorial(int n);
void swap(char *a, const int i, const int j);

void permutation_output(char *per, const int number_of_permutations, const int len ,FILE *fout);

int main(void){
	FILE *fin;
	FILE *fout;

	open_files(&fin, &fout);

	if (dijkstra(fin, fout)){
		bad_input(fin, fout);
		return 0;
	}

	close_files(fin, fout);

	return 0;
}

void open_files(FILE **fin, FILE **fout){
	*fin = fopen(INPUT_FILE, "r");
	*fout = fopen(OUTPUT_FILE, "w+");
}

void close_files(FILE *fin, FILE *fout){
	fclose(fin);
	fclose(fout);
}

void bad_input(FILE *fin, FILE *fout){
// 	printf("bad input\n");
	fprintf(fout, "bad input\n");
	close_files(fin, fout);
}

int dijkstra(FILE *fin, FILE *fout){
	char template[SIZE_OF_TEMPLATE] = {'\0'};

	if (get_template(&template[0], fin)){
		return 1;
	}

	int number_of_permutations = 0;

	if (fscanf(fin, "%d", &number_of_permutations) == 0){
		return 1;
	}

	if (number_of_permutations == 0 || strlen(template) == 1){
		return 0;
	}
	int len = strlen(template);
	int fact = factorial(len);

	if (number_of_permutations > fact - 1){
		number_of_permutations = fact - 1;
	}

	char per[number_of_permutations][len];

  	for(int i = 0; i < number_of_permutations; i++){
  		int j = len - 2;
		while (j != -1 && template[j] >= template[j + 1]) j--;
		if (j == -1) break; // больше перестановок нет
		int k = len - 1;
		while (template[j] >= template[k]) k--;
		swap(&template[0], j, k);
		int l = j + 1, r = len - 1; // сортируем оставшуюся часть последовательности
		while (l < r) {
			swap(&template[0], l++, r--);
		}
		for (int j = 0; j < len; j++){
  			per[i][j] = template[j];
  		}
  	}

  	if (per[0][0] - '0' > 9 || per[0][0] - '0' < 0){
		return 0;
	}

	permutation_output(&per[0][0], number_of_permutations, len, fout);
	return 0;
}

int get_template(char *template, FILE *fin){
	char str[SIZE_OF_TEMPLATE] = {'\0'};
	if (fgets(str, SIZE_OF_TEMPLATE, fin) == NULL){
		return 1;
	}

	if (str[11] != '\0'){
		return 1;
	}
	str[strlen(str) - 1] = '\0';

	if (check(str)){
		return 1;
	}

	for (int i = 0; i < (int)strlen(str); i++){
		*template = str[i];
		template++;
	}

	return 0;
}

int check(const char template[]){
	int check[10];
	for (int i = 0; i < 10; i++){
		check[i] = 1;
	}
	int len = strlen(template);

	for (int i = 0; i < len; i++){
		if ((template[i] - '0' <= 9) && (template[i] - '0' >= 0) && (check[template[i] - '0'])){
			check[template[i] - '0'] = 0;
		} else {
			return 1;
		}
	}

	return 0;
}

int factorial(int n){
  return (n < 2) ? 1 : n * factorial (n - 1);
}

void swap(char *a, const int i, const int j){
  char s = *(a + i);
  *(a + i) = *(a + j);
  *(a + j) = s;
}

void permutation_output(char *per, const int number_of_permutations, const int len ,FILE *fout){
	for (int i = 0; i < number_of_permutations; i++){
		for (int j = 0; j < len; j++){
// 			printf("%c", *(per + i*len + j));
			fprintf(fout, "%c", *(per + i*len + j));
		}
// 		printf("\n");
		fprintf(fout, "\n");
	}
}
