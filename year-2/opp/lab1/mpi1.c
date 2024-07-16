#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mpi.h"

#define N 848
#define T 0.000001
#define E 0.00001

void init_rcount(int* rcount, size_t size, int size_mpi)
{
  for (size_t i = 0; i < size_mpi; ++i)
  {
    rcount[i] = size / size_mpi;
  }
}

void init_displs(int* displs, size_t size, int size_mpi)
{
  for (size_t i = 0; i < size_mpi; ++i)
  {
    displs[i] = (i * size / size_mpi);
  }
}

void init_matrix_A(double* A, size_t columns, size_t rows, int rank)
{
  size_t offset = rows * rank;
  for (size_t i = 0; i < rows; ++i)
  {
    for (size_t j = 0; j < columns; ++j)
    {
      A[i * columns + j] = ((i + offset) == j) ? 2.0 : 1.0;
    }
  }
}

void init_vector_b(double* b, size_t size)
{
  for (size_t i = 0; i < size; ++i)
  {
    b[i] = size + 1;
  }
}

void init_vector_x(double* x, size_t size)
{
  for (size_t i = 0; i < size; ++i)
  {
    x[i] = 0.0;
  }
}

void multiply_matrix_vector(const double* matrix, const double* vector, size_t size, double* res, int rank, int size_mpi, const int* rcount, const int* displs)
{
  for (size_t i = 0; i < (size / size_mpi); ++i)
  {
    res[i + (size * rank / size_mpi)] = 0.0;
    for(size_t j = 0; j < size; ++j)
    {
      res[i + (size * rank / size_mpi)] += matrix[i * size + j] * vector[j];
    }
  }

  MPI_Allgatherv(MPI_IN_PLACE, size / size_mpi, MPI_DOUBLE, res, rcount, displs, MPI_DOUBLE, MPI_COMM_WORLD);
}
void subtract_vectors(double* a, const double* b, size_t size)
{
  for (size_t i = 0; i < size; ++i)
  {
    a[i] = a[i] - b[i];
  }
}

void multiply_on_scalar(double* a, size_t size, double scalar)
{
  for (size_t i = 0; i < size; ++i)
  {
    a[i] *= scalar;
  }
}

double get_norm(const double* a, size_t size, int rank, int size_mpi)
{
  double sum = 0;
  double tmp_sum = 0;

  for (size_t i = (rank * size / size_mpi); i < ((rank + 1) * size / size_mpi); ++i)
  {
    tmp_sum += pow(a[i], 2);
  }
  MPI_Allreduce(&tmp_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

  return sqrt(sum);
}

int main(int argc, char **argv)
{
  int rank, size_mpi;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size_mpi);

  double* A;
  A = (double*)malloc(sizeof(double) * N * N / size_mpi);
  if (!A) goto bad_A;

  double* b;
  b = (double*)malloc(sizeof(double) * N);
  if (!b) goto bad_b;

  double* x;
  x = (double*)malloc(sizeof(double) * N);
  if (!x) goto bad_x;

  double* tmp;
  tmp = (double*)malloc(sizeof(double) * N);
  if (!tmp) goto bad_tmp;

  int* rcount;
  rcount = (int*)malloc(sizeof(int) * size_mpi);
  if (!rcount) goto bad_rcount;

  int* displs;
  displs = (int*)malloc(sizeof(int) * size_mpi);
  if (!displs) goto bad_displs;

  init_rcount(rcount, N, size_mpi);
  init_displs(displs, N, size_mpi);
  init_matrix_A(A, N, N / size_mpi, rank);
  init_vector_b(b, N);
  init_vector_x(x, N);

  double norm_b;

  struct timespec start = {}, end = {};
  if (rank == 0)
  {
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  }

  norm_b = get_norm(b, N, rank, size_mpi);

  while (1)
  {
    multiply_matrix_vector(A, x, N, tmp, rank, size_mpi, rcount, displs);

    subtract_vectors(tmp, b, N);

    double norm_tmp = get_norm(tmp, N, rank, size_mpi);

    if ((norm_tmp / norm_b) < E) break;

    multiply_on_scalar(tmp, N, T);
    subtract_vectors(x, tmp, N);
  }

  if (rank == 0)
  {
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    printf("%lf\n", end.tv_sec-start.tv_sec + 0.000000001*(end.tv_nsec-start.tv_nsec));
  }

  free(displs);

  bad_displs:
  free(rcount);

  bad_rcount:
  free(tmp);

  bad_tmp:
  free(x);

  bad_x:
  free(b);

  bad_b:
  free(A);

  bad_A:

  MPI_Finalize();

  return 0;
}
