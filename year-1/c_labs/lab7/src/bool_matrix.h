#ifndef _BOOL_MATRIX_H_
#define _BOOL_MATRIX_H_

#include <stddef.h>

typedef struct Bool_Matrix Bool_Matrix;

char get_value(const Bool_Matrix* bool_matrix, size_t i, size_t j);
void set_value(Bool_Matrix* bool_matrix, size_t i, size_t j, char value);
Bool_Matrix* create_matrix(size_t n_rows, size_t n_columns);
void delete_matrix(Bool_Matrix* bool_matrix);

#endif // _BOOL_MATRIX_H_
