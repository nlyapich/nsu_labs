#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "bitstream.h"
#include "tree.h"

#define SIZE_BITSTREAM 400

static void init_node(Tree* node, unsigned char symbol, size_t frequency){
	if (!node) return;

	node->symbol = symbol;
	node->frequency = frequency;

	node->left = NULL;
	node->right = NULL;
}

Tree* create_node(unsigned char symbol, size_t frequency){
	Tree* node = (Tree*)malloc(sizeof(Tree));
	if (!node) return NULL;

	init_node(node, symbol, frequency);

	return node;
}

int node_comparator(const void* a, const void* b){
	Tree* a_node = (Tree*)a;
	Tree* b_node = (Tree*)b;
	if (a_node->frequency > b_node->frequency) return 1;
	if (a_node->frequency < b_node->frequency) return -1;
	return 0;
}

Tree* make_shared(const Tree* a, const Tree* b){
	if (!a || !b) return NULL;
	
	Tree* joint = create_node(0, a->frequency + b->frequency);
	if (!joint) return NULL;

	joint->left = (struct Tree*)a;
	joint->right = (struct Tree*)b;

	return joint;
}

int is_leaf(const Tree* node){
	if (!node) return -1;

	if (!node->left && !node->right) return 1;

	return 0;
}

void del_tree(Tree* root){
	if (!root) return;

	if (root->left) del_tree((Tree*)(root->left));
	if (root->right) del_tree((Tree*)(root->right));

	free(root);
}

static void write_tree(Tree* root, BitStream* bitstream){
	if (!root || !bitstream) return;

	if (is_leaf(root)){
		set_next_bit_bitstream(bitstream, 1);
		for (int i = CHAR_BIT - 1; i >= 0; --i){
			set_next_bit_bitstream(bitstream, (root->symbol >> i) & 1);
		}
	}
	else {
		set_next_bit_bitstream(bitstream, 0);
		if (root->left){
			write_tree((Tree*)root->left, bitstream);
		}
		if (root->right){
			write_tree((Tree*)root->right, bitstream);
		}
	}
}
BitStream* print_tree(Tree* root, FILE* fin){
	if (!root || !fin) return NULL;

	BitStream* bitstream = create_bitstream(SIZE_BITSTREAM);
	if (!bitstream) return NULL;

	write_tree(root, bitstream);

	return bitstream;
}
static Tree* write_node(const BitStream* bitstream, int cur_bit){
	if (!bitstream) return NULL;

	char tmp = 0;
	for (int i = 0; i < CHAR_BIT; ++i){
		tmp = (tmp << 1) | get_bit(bitstream, cur_bit + i);
	}
	Tree* new_node = create_node((unsigned char)tmp, 0);
	if (!new_node) return NULL;

	return new_node;
}
static void scan_tree(const BitStream* bitstream, size_t* cur_pos, Tree** cur, int *cur_count_sym){
	if (!bitstream) return;

	int cur_bit = get_bit(bitstream, *cur_pos);
	if (cur_bit < 0) return;

	*cur_pos = *cur_pos + 1;
	
	if (cur_bit){
		Tree* cur_node = write_node(bitstream, *cur_pos);
		if (!cur_node) return;
		*cur_count_sym = *cur_count_sym - 1;
		*cur = cur_node;
		*cur_pos = *cur_pos + CHAR_BIT;
	}
	else {
		*cur = create_node(0, 0);
		if (!*cur) return;

		if (*cur_count_sym == 0 || get_bit(bitstream, *cur_pos) < 0) return;
		scan_tree(bitstream, cur_pos, (Tree**)&(*cur)->left, cur_count_sym);
		if (*cur_count_sym == 0 || get_bit(bitstream, *cur_pos) < 0) return;
		scan_tree(bitstream, cur_pos, (Tree**)&(*cur)->right, cur_count_sym);
	}
}
Tree* read_tree(const BitStream* bitstream, int count_sym){
	if (!bitstream) return NULL;

	Tree* root = NULL;

	size_t cur_pos = 0;
	int cur_count_sym = count_sym;
	scan_tree(bitstream, &cur_pos, &root, &cur_count_sym);

	return root;
}
