#ifndef _SRC_AVLTREE_H_
#define _SRC_AVLTREE_H_

#include <stddef.h>

typedef struct AVLTree AVLTree;

AVLTree* create_tree(size_t size);
void insert(AVLTree* tree, const int value);
int height_tree(const AVLTree* tree);
void del(AVLTree* tree);

#endif  // _SRC_AVLTREE_H_
