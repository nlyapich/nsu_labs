#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "avltree.h"

#define INPUT_FILE  "in.txt"
#define OUTPUT_FILE "out.txt"

int lab6(FILE* fin){
	int n;
	if (!fscanf(fin, "%d", &n)) return -1;

	if (n == 0){
		return 0;
	}

	int cur;
	if (!fscanf(fin, "%d", &cur)) return -1;
	
	AVLTree* tree = create_tree(n);
	if (!tree) return -1;

	insert(tree, cur);
	int h = 0;
	for (int i = 0; i < n - 1; i++){
		if (fscanf(fin, "%d", &cur) == 1){
			insert(tree, cur);
		} else {
			h = -1;
			break;
		}
	}

	if (h != -1)
		h = height_tree(tree);

	del(tree);

	return h;
}

int main(void){
	FILE* fin;
	fin = fopen(INPUT_FILE, "r");
	if (!fin) return 0;
	
	FILE* fout;
	fout = fopen(OUTPUT_FILE, "w+");

	int height = lab6(fin);

	if (height != -1) {
		fprintf(fout, "%d\n", height);
// 		printf("%d\n", height);
	}
	else {
		fprintf(fout, "bad input\n");
// 		printf("bad input\n");
	}

	fclose(fin);
	fclose(fout);

	return 0;
}
