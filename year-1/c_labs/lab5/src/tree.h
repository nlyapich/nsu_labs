#ifndef _SRC_TREE_H
#define _SRC_TREE_H

#include <stddef.h>

typedef struct {
	unsigned char symbol;
	size_t frequency;
	struct Tree* left;
	struct Tree* right;
} Tree;

Tree* create_node(unsigned char symbol, size_t frequency);
int node_comparator(const void* a, const void* b);
Tree* make_shared(const Tree* a, const Tree* b);
int is_leaf(const Tree* node);
void del_tree(Tree* root);
BitStream* print_tree(Tree* root, FILE* fin);
Tree* read_tree(const BitStream* bitstream, int count_sym);

#endif //_SRC_TREE_H
