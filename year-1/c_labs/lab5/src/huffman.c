#define _CRT_NO_SECURE_WARNINGS

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bitstream.h"
#include "huffman.h"
#include "priority_queue.h"
#include "tree.h"

#define SIZE_FREQUENCY 256

static void read_file(FILE* fin, size_t* frequency, unsigned int working_pos){
	if (!fin || !frequency) return;

	fseek(fin, working_pos, SEEK_SET);

	unsigned char cur;

	for (size_t i = 0; fread(&cur, sizeof(unsigned char), 1, fin); ++i){
		frequency[cur]++;
	}
}
static size_t* get_frequency(FILE* fin, unsigned int working_pos){
	if (!fin) return NULL;

	size_t* frequency = (size_t*)calloc(SIZE_FREQUENCY, sizeof(size_t));
	if (!frequency) return NULL;

	read_file(fin, frequency, working_pos);

	return frequency;
}
static void make_code(const Tree* node, char** code, char* str, int len){
	if (!node) return;

	if (node->left){
		str[len] = '0';
		make_code((Tree*)(node->left), code, str, len + 1);
	}
	if (node->right){
		str[len] = '1';
		make_code((Tree*)(node->right), code, str, len + 1);
	}
	if (is_leaf(node) == 1){
		if (len == 0){
			str[len] = '0';
			len++;
		}
		*(code + (int)(node->symbol)) = (char*)calloc(len + 1, sizeof(char));
		if (!*(code + (int)(node->symbol))) return;
		for (int i = 0; i < len; ++i){
			*(*(code + (int)(node->symbol)) + i) = str[i];
		}
	}
}
static char** get_code(const Tree* tree){
	if (!tree) return NULL;

	char** code = (char**)calloc(SIZE_FREQUENCY, sizeof(char*));
	if (!code) return NULL;

	char str[SIZE_FREQUENCY] = {0};
	int len = 0;
	
	make_code(tree, code, &str[0], len);

	return code;
}
static void fill_queue(PQueue* pqueue, const size_t* frequency){
	if (!pqueue || !frequency) return;

	for (unsigned int i = 0; i < SIZE_FREQUENCY; ++i){
		if (frequency[i] == 0) continue;
		Tree* node = create_node(i, frequency[i]);
		add_pq(pqueue, node);
	}
}
static Tree* build_tree(const size_t* frequency){
	if (!frequency) return NULL;

	PQueue* pqueue = create_pq(SIZE_FREQUENCY, node_comparator);
	if (!pqueue) return NULL;

	fill_queue(pqueue, frequency);

	while (get_size_pq(pqueue) > 1){
		Tree* root_1 = extract_min_pq(pqueue);
		Tree* root_2 = extract_min_pq(pqueue);
		if (!root_1 || !root_2) return NULL;

		Tree* shared_tree = make_shared(root_1, root_2);
		add_pq(pqueue, shared_tree);
	}

	Tree* tree = extract_min_pq(pqueue);

	del_pq(pqueue);

	return tree;
}
static int get_count_unique_sym(const size_t* frequency){
	if (!frequency) return 0;

	int count = 0;
	for (int i = 0; i < SIZE_FREQUENCY; ++i){
		if (frequency[i]) count++;
	}

	return count;
}
static void write_count_unique_sym(FILE* fout, const size_t* frequency){
	if (!fout || !frequency) return;

	int count_sym = get_count_unique_sym(frequency);

	fwrite(&count_sym, sizeof(int), 1, fout);
}

static int get_mod(FILE* fout, char** code, const size_t* frequency){
	if (!fout || !code || !frequency) return 0;

	int mod = 0;

	for (int i = 0; i < SIZE_FREQUENCY; ++i){
		if (!*(code + i)) continue;
		mod = (mod + (strlen(*(code + i)) % CHAR_BIT) * (frequency[i] % CHAR_BIT)) % CHAR_BIT;
	}

	return (CHAR_BIT - mod) % CHAR_BIT;
}
static void write_mod(FILE* fout, int mod){
	if (!fout) return;

	fwrite(&mod, sizeof(int), 1, fout);
}
static void write_code(FILE* fin, FILE* fout, char** code, int mod, unsigned int working_pos){
	if (!fin || !fout || !code) return;

	fseek(fin, working_pos, SEEK_SET);

	unsigned char cur;
	unsigned char buffer = 0;
	int count_buffer = mod;

	while (fread(&cur, sizeof(unsigned char), 1, fin)){
		if (!*(code + (int)cur)) continue;
		int len = strlen(*(code + (int)cur));

		for (int i = 0; i < len; ++i){
			if (count_buffer == CHAR_BIT){
				fwrite(&buffer, sizeof(unsigned char), 1, fout);
				buffer &= 0;
				count_buffer = 0;				
			}
			buffer = (buffer << 1) | ((*(*(code + (int)cur) + i)) - '0');
			count_buffer++;
		}
	}
	fwrite(&buffer, sizeof(unsigned char), 1, fout);
}
static void write_file(FILE* fin, FILE* fout, const size_t* frequency, char** code, unsigned int working_pos, Tree* tree){
	if (!fin || !fout || !frequency || !code || !tree) return;

	write_count_unique_sym(fout, frequency);

	int mod = get_mod(fout, code, frequency);
	write_mod(fout, mod);
	
	BitStream* bitstream = print_tree(tree, fout);
	if (!bitstream) return;
	size_t size_bitstream = get_cur_size_bytes(bitstream);
	fwrite(&size_bitstream, sizeof(size_t), 1, fout);

	print_bitstream(bitstream, fout);
	del_bitstream(bitstream);
	write_code(fin, fout, code, mod, working_pos);
}
static void del_code(char** code){
	if (!code) return;
	
	for (int i = 0; i < SIZE_FREQUENCY; ++i){
		if (*(code + i)) free(*(code + i));
	}
	free(code);
}
void zip(FILE* fin, FILE* fout, unsigned int working_pos){
	if (!fin || !fout) return;

	size_t* frequency = get_frequency(fin, working_pos);
	if (!frequency) return;

	Tree* tree = build_tree(frequency);
	if (!tree) goto failed_tree;

	char** code = get_code(tree);
	if (!code) goto failed_code;


	write_file(fin, fout, frequency, code, working_pos, tree);

	del_tree(tree);
	del_code(code);
	free(frequency);

	return;

	failed_code:
	del_tree(tree);
	failed_tree:
	free(frequency);
}

static int read_count_unique_sym(FILE* fin){
	if (!fin) return 0;

	int count_sym = 0;
	if (!fread(&count_sym, sizeof(int), 1, fin)) return 0;

	return count_sym;
}
static int read_mod(FILE* fin){
	if (!fin) return 0;

	int mod = 0;
	if (!fread(&mod, sizeof(int), 1, fin)) return 0;

	return mod;
}
static Tree* go_to_tree(Tree* node, int status){
	if (!node) return NULL;

	if (status && node->right) return (Tree*)node->right;
	else if (!status && node->left) return (Tree*)node->left;
	else return node; 
}
static void decrypt(FILE* fin, FILE* fout, Tree* tree, int mod){
	if (!fin || !fout || !tree) return;

	Tree* cur_node = tree;
	unsigned char str = 0;
	if (!fread(&str, sizeof(unsigned char), 1, fin)) return;
	str = (str << mod);
	int count_str = mod;
	while (1){
		while (count_str != CHAR_BIT){
			int status = (str & 128) >> (CHAR_BIT - 1);

			cur_node = go_to_tree(cur_node, status);

			if (is_leaf(cur_node)){
				fwrite(&cur_node->symbol, sizeof(unsigned char), 1, fout);
				cur_node = tree;
			}

			str <<= 1;
			count_str++;
		}
		count_str = 0;
		if (!fread(&str, sizeof(unsigned char), 1, fin)) break;
	}
}
static size_t read_size_tree(FILE* fin){
	if (!fin) return 0;

	size_t size = 0;
	if (!fread(&size, sizeof(size_t), 1, fin)) return 0;

	return size;

}
static BitStream* read_tree_space(FILE* fin, size_t size_tree){
	if (!fin) return NULL;

	BitStream* bitstream = create_bitstream(size_tree);
	if (!bitstream) return NULL;

	for (size_t i = 0; i < size_tree; ++i){
		unsigned char cur = 0;
		if (!fread(&cur, sizeof(unsigned char), 1, fin)){
			del_bitstream(bitstream);
			return NULL;
		}
		set_byte(bitstream, i, cur);
	}

	return bitstream;
}
void unzip(FILE* fin, FILE* fout, unsigned int working_pos){
	if (!fin || !fout) return;

	fseek(fin, working_pos, SEEK_SET);

	int count_sym = read_count_unique_sym(fin);
	int mod = read_mod(fin);
	size_t size_tree = read_size_tree(fin);

	BitStream* bitstream = read_tree_space(fin, size_tree);

	Tree* tree = read_tree(bitstream, count_sym);
	if (!tree){
		del_bitstream(bitstream);
		return;
	}

	decrypt(fin, fout, tree, mod);

	del_tree(tree);
	del_bitstream(bitstream);
}
