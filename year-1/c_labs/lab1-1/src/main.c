#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE  "in.txt"
#define OUTPUT_FILE "out.txt"
#define SIZE_OF_BUFFER 80
#define SIZE_OF_TEMPLATE 18

void open_files(FILE **fin, FILE **fout);
void close_files(FILE *fin, FILE *fout);

int get_template(unsigned char* template, FILE* fin);
int get_buffer(unsigned char *buffer, FILE* fin);
void update_buffer(unsigned char *buffer, FILE *fin, const int length_of_template);

int get_hash_of_buffer(const int length_of_template, const unsigned char *buffer);
int get_hash_of_template(const int length_of_template, const unsigned char *template);

void rabin_karp(FILE *fin, FILE *fout);
void update_hash(int pos, int *hash_of_buffer, const unsigned char *buffer, const int length_of_buffer, const int length_of_template);

int main(void){
	FILE *fin;
	FILE *fout;

	open_files(&fin, &fout);

	rabin_karp(fin, fout);

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

void rabin_karp(FILE *fin, FILE *fout){
	unsigned char template[SIZE_OF_TEMPLATE] = {'\0'};

	int length_of_template = get_template(&template[0], fin);

	int hash_of_template = get_hash_of_template(length_of_template, &template[0]);

	fprintf(fout, "%d ", hash_of_template);
	// printf("%d ", hash_of_template);

	unsigned char buffer[SIZE_OF_BUFFER + 1] = {'\0'};
	int length_of_buffer = get_buffer(&buffer[0], fin);
	if ((buffer[0] == 255) || (buffer[0] == '\0') || (length_of_buffer < length_of_template)){
		return;
	}

	if(length_of_buffer < length_of_template){
		return;
	}

	int hash_of_buffer = get_hash_of_buffer(length_of_template, &buffer[0]);
	int count_of_buffers = 0;
	while(1){
		if ((buffer[0] == '\0') || (buffer[0] == 255)){
			break;
		}
		for(int pos = 0; pos <= length_of_buffer - length_of_template; pos++){

			if (hash_of_buffer == hash_of_template){
				for(int j = 0; j < length_of_template; j++){

					fprintf(fout, "%d ", pos + j + 1 + count_of_buffers);
					// printf("%d ", pos + j + 1 + count_of_buffers);

					if(template[j] != buffer[pos + j]) {
						break;
					}
				}
			}

			update_hash(pos, &hash_of_buffer, &buffer[0], length_of_buffer, length_of_template);
		}
		update_buffer(&buffer[0], fin, length_of_template);
		hash_of_buffer = get_hash_of_buffer(length_of_template, &buffer[0]);
		count_of_buffers += SIZE_OF_BUFFER - length_of_template;
	}

	fprintf(fout, "\n");
	// printf("\n");
}

int get_template(unsigned char* template, FILE* fin){
	for (int j = 0; j < SIZE_OF_TEMPLATE; j++){
		int now_sym = 0;
		now_sym = fgetc(fin);
		if(now_sym != 10) {
			*template = now_sym;
		} else {
			return j;
		}
		template++;
	}
	return SIZE_OF_TEMPLATE;
}

int get_hash_of_template(const int length_of_template, const unsigned char *template){

	int hash_of_template = 0;

	for (int i = 0; i < length_of_template; i++) {
		hash_of_template += (int)((*(template + i) % 3) * pow(3, i));
	}
	return hash_of_template;
}

int get_hash_of_buffer(const int length_of_template, const unsigned char *buffer){

	int hash_of_buffer = 0;

	for (int i = 0; i < length_of_template; i++) {
		hash_of_buffer += (int)((*(buffer + i) % 3) * pow(3, i));
	}
	return hash_of_buffer;
}

void update_hash(int pos, int *hash_of_buffer, const unsigned char *buffer, const int length_of_buffer, const int length_of_template){
	if (pos < length_of_buffer - length_of_template) {
		*hash_of_buffer -= (int)((*(buffer + pos) % 3));
		*hash_of_buffer -= (*hash_of_buffer / 3) * 2;
		*hash_of_buffer += (int)((*(buffer+ pos + length_of_template) % 3) * pow(3, length_of_template - 1));
	}
}

void update_buffer(unsigned char *buffer, FILE *fin, const int length_of_template){
	for (int i = 0; i < length_of_template; i++){
		*(buffer + i) = *(buffer + SIZE_OF_BUFFER - (length_of_template - i));
	}
	buffer += length_of_template;
	for (int j = length_of_template; j < SIZE_OF_BUFFER; j++){
		int now_sym = 0;
		now_sym = fgetc(fin);
		if(now_sym != -1) {
			*buffer = now_sym;
		} else {
			for (int i = j; i < SIZE_OF_BUFFER; i++){
				*buffer = '\0';
				buffer++;
			}
			break;
		}
		buffer++;
	}
}

int get_buffer(unsigned char* buffer, FILE* fin){
	for (int j = 0; j < SIZE_OF_BUFFER; j++){
		int now_sym = 0;
		now_sym = fgetc(fin);
		if(now_sym != -1) {
			*buffer = now_sym;
		} else {
			for (int i = j; i < SIZE_OF_BUFFER; i++){
				*buffer = '\0';
				buffer++;
			}
			return j;
		}
		buffer++;
	}
	return SIZE_OF_BUFFER;
}
