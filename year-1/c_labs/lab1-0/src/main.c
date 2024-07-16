#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE  "in.txt"
#define OUTPUT_FILE "out.txt"
#define SIZE_OF_BUFFER 80

void update_buffer(char *buffer, FILE *fin, const int size_of_template);
void get_buffer(char* buffer, FILE* fin);

int main(void){
	FILE *fin;
	FILE *fout;

	fin = fopen(INPUT_FILE, "r");
	char template[18] = {'\0'};
	if (fgets(template, 18, fin) == NULL){
		printf("bad input");
		return 0;
	}
	template[strlen(template) - 1] = '\0';


	int size_of_template = strlen(template);
	const int number_of_last_elem = size_of_template - 1;
	fout = fopen(OUTPUT_FILE, "w+");


	int stop_sym[256];
    for (int i = 0; i <  256; i++) {
        stop_sym[i] = size_of_template;
    }

    stop_sym[(int)template[size_of_template - 2]] = 1;
    for (int i = size_of_template - 3; i >= 0; i--) {
    	if (stop_sym[(int)template[i]] != size_of_template){
    		continue;
    	}
        stop_sym[(int)template[i]] = (size_of_template - 2) - i + 1;
    }

	char buffer[SIZE_OF_BUFFER] = {'\0'};
	
	get_buffer(&buffer[0], fin);
	if ((buffer[0] == '\0') || (buffer[0] == -1) || ((int)strlen(buffer) < size_of_template)){
		fclose(fin);
		fclose(fout);
		return 0;
	}

	int count_of_buffers = 0;
	int pos = size_of_template - 1;

	while(1){
		if ((buffer[pos] == '\0') || (buffer[pos] == -1)){
			break;
		}

		fprintf(fout, "%d ", pos + count_of_buffers + 1);
		// printf("%d ", pos + count_of_buffers + 1);

		if (buffer[pos] == template[number_of_last_elem]){

			for (int i = pos - 1; i >= pos - (size_of_template - 1); i--){

				fprintf(fout, "%d ", i + count_of_buffers + 1);
				// printf("%d ", i + count_of_buffers + 1);

				if (buffer[i] == template[number_of_last_elem - (pos - i)]){
					if (pos - i == size_of_template - 1){
						pos += size_of_template;
					}
				} else {
					pos += stop_sym[(int)template[number_of_last_elem]];
					break;
				}

			}

		} else {
			pos += stop_sym[(int)buffer[pos]];
		}

		if (pos >= SIZE_OF_BUFFER){
			pos -= SIZE_OF_BUFFER - size_of_template;
			count_of_buffers += SIZE_OF_BUFFER - size_of_template;
			update_buffer(&buffer[0], fin, size_of_template);
		}
	}

	fprintf(fout, "\n");
	// printf("\n");
	fclose(fin);
	fclose(fout);

	return 0;
}

void update_buffer(char *buffer, FILE *fin, const int size_of_template){
	for (int i = 0; i < size_of_template; i++){
		*(buffer + i) = *(buffer + SIZE_OF_BUFFER - (size_of_template - i));
	}
	buffer += size_of_template;
	for (int j = size_of_template; j < SIZE_OF_BUFFER; j++){
		int now_sym = fgetc(fin);
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

void get_buffer(char* buffer, FILE* fin){
	for (int j = 0; j < SIZE_OF_BUFFER; j++){
		int now_sym = fgetc(fin);
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
