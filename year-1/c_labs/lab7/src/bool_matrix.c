#include "bool_matrix.h"

#include <stdlib.h>

const size_t kNBit = 8;

typedef unsigned int T;

struct Bool_Matrix{
    size_t n_rows;
    size_t n_bool_columns;
    size_t n_columns;
    size_t item_size;
    T* matrix;
};

static size_t div_up(size_t numerator, size_t denominator){
    return (numerator + denominator - 1) / denominator;
}

static int check_correct_indexes(const struct Bool_Matrix* bool_matrix, size_t i, size_t j){
    if (i >= bool_matrix->n_rows || j >= bool_matrix->n_bool_columns){
        return 0;
    }
    return 1;
}

char get_value(const struct Bool_Matrix* bool_matrix, size_t i, size_t j){
    if (!check_correct_indexes(bool_matrix, i, j)) return 2;
    T item = bool_matrix->matrix[i * bool_matrix->n_columns + j / bool_matrix->item_size];
    return (item & (1 << (j % bool_matrix->item_size))) || 0;
}

void set_value(struct Bool_Matrix* bool_matrix, size_t i, size_t j, char value){
    if (!check_correct_indexes(bool_matrix, i, j)) return;
    T* item = &(bool_matrix->matrix[i * bool_matrix->n_columns + j / bool_matrix->item_size]);
    if (value){
        *item |= 1 << (j % bool_matrix->item_size);
    } else {
        *item &= ~(1 << (j % bool_matrix->item_size));
    }
}

struct Bool_Matrix* create_matrix(size_t n_rows, size_t n_columns){
    struct Bool_Matrix* bool_matrix = (struct Bool_Matrix*)malloc(sizeof(struct Bool_Matrix));
    if (!bool_matrix) return NULL;
    bool_matrix->n_bool_columns = n_columns;
    bool_matrix->n_rows = n_rows;
    bool_matrix->item_size = sizeof(T) * kNBit;
    bool_matrix->n_columns = div_up(n_columns, bool_matrix->item_size);
    bool_matrix->matrix = calloc(n_rows * bool_matrix->n_columns, sizeof(T));
    if (!bool_matrix->matrix){
        free(bool_matrix);
        return NULL;
    }
    return bool_matrix;
}

void delete_matrix(struct Bool_Matrix* bool_matrix){
    if(!bool_matrix) return;
    free(bool_matrix->matrix);
    free(bool_matrix);
}
