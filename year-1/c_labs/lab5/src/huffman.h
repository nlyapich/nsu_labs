#ifndef _SRC_HUFFMAN_H
#define _SRC_HUFFMAN_H

#include <stdio.h>

void zip(FILE* fin, FILE* fout, unsigned int working_pos);
void unzip(FILE* fin, FILE* fout, unsigned int working_pos);

#endif //_SRC_HUFFMAN_H
