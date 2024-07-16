#define _CRT_NO_SECURE_WARNINGS

#include <stdio.h>

#include "huffman.h"

#define INPUT_FILE "in.txt"
#define OUTPUT_FILE "out.txt"

unsigned int get_working_pos(FILE* fin){
	if (!fin) return 0;

	fseek(fin, 0, SEEK_SET);

	char cur = 0;
	unsigned int count_pos = 0;
	
	while (cur != '\n' && fread(&cur, sizeof(char), 1, fin)){
		count_pos++;
	}

	return count_pos;
}

int main(void){
	FILE* fin;
	fin = fopen(INPUT_FILE, "rb");
	if (!fin) return 0;
	
	FILE* fout;
	fout = fopen(OUTPUT_FILE, "wb+");

	char status = 0;
	if (!fread(&status, sizeof(char), 1, fin)) goto close_files;

	unsigned int working_pos = get_working_pos(fin);

	if (status == 'c') zip(fin, fout, working_pos);
	else if (status == 'd') unzip(fin, fout, working_pos);
	else fprintf(fout, "bad input\n");

	close_files:

	fclose(fin);
	fclose(fout);

	return 0;
}
