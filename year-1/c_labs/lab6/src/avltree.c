#include "avltree.h"

#include <stddef.h>
#include <stdlib.h>

typedef struct TreeItem{
	int value;
	unsigned char height;
	struct TreeItem* left;
	struct TreeItem* right;
} TreeItem;

struct AVLTree{
	TreeItem* root;
	TreeItem* mas;
	size_t status_mas;
};

struct AVLTree* create_tree(size_t size){
	struct AVLTree* tree = (AVLTree*)malloc(sizeof(AVLTree));
	if (!tree) return NULL;
	tree->root = NULL;
	tree->status_mas = 0;
	tree->mas = (TreeItem*)malloc(sizeof(TreeItem) * size);
	if (tree->mas){
		return tree;
	}
	else {
		free(tree);
		return NULL;
	}
}

static TreeItem* init_node(TreeItem* node, int value){
	if (!node) return node;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;
	node->value = value;
	return node;
}

static unsigned short height_node(const TreeItem* node){
    return node ? node->height : 0;
}

static int balance_factor(const TreeItem* node){
	return node ? (height_node(node->right) - height_node(node->left)) : 0;
}

static void fix_height(TreeItem* node){
	if (!node) return;
	unsigned char hl = height_node(node->left);
	unsigned char hr = height_node(node->right);
	node->height = (hl>hr ? hl : hr) + 1;
}

static TreeItem* rotate_right(TreeItem* node){
	if (!node || !node->left)
    	return node;
	TreeItem* new = node->left;
	node->left = new->right;
	new->right = node;
	fix_height(node);
	fix_height(new);
	return new;
}
static TreeItem* rotate_left(TreeItem* node){
	if (!node || !node->right)
    	return node;
	TreeItem* new = node->right;
	node->right = new->left;
	new->left = node;
	fix_height(node);
	fix_height(new);
	return new;
}

static TreeItem* balance(TreeItem* node){
	fix_height(node);
	
	if (balance_factor(node) == 2){
		if (balance_factor(node->right) < 0) node->right = rotate_right(node->right);
		return rotate_left(node);
	}

	if (balance_factor(node) == -2){
		if (balance_factor(node->left) > 0) node->left = rotate_left(node->left);
		return rotate_right(node);
	}

	return node;
}

static TreeItem* add(TreeItem* root, TreeItem* new_el){
	if (!root){
		return new_el;
	}
	if (new_el->value < (root->value))
		root->left = add(root->left, new_el);
	else
		root->right = add(root->right, new_el);
	return balance(root);
}

void insert(struct AVLTree* tree, const int value){
	if (!tree) return;
	tree->root = add(tree->root, init_node(tree->mas + tree->status_mas, value));
	tree->status_mas++;
}

int height_tree(const struct AVLTree* tree){
	if (tree)
		return height_node(tree->root);
	return -1;
}

void del(struct AVLTree* tree){
	if (!tree) return;
	free(tree->mas);
	free(tree);
}
