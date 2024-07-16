#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 9000
#define T 0.00001
#define E 0.00001

void init_matrix_A(double* A, size_t size)
{
  #pragma omp parallel for
  for (size_t i = 0; i < size; ++i)
  {
    #pragma omp parallel for
    for (size_t j = 0; j < size; ++j)
    {
      A[i * size + j] = (i == j) ? 2.0 : 1.0;
    }
  }
}

void init_vector_b(double* b, size_t size)
{
  #pragma omp parallel for
  for (size_t i = 0; i < size; ++i)
  {
    b[i] = size + 1;
  }
}

void init_vector_x(double* x, size_t size)
{
  #pragma omp parallel for
  for (size_t i = 0; i < size; ++i)
  {
    x[i] = 0.0;
  }
}

void multiply_matrix_vector(const double* matrix, const double* vector, size_t size, double* res)
{
  #pragma omp parallel for
  for(size_t i = 0; i < size; ++i)
  {
    res[i] = 0.0;
    for(size_t j = 0; j < size; ++j)
    {
      res[i] += matrix[i * size + j] * vector[j];
    }
  }
}

void subtract_vectors(double* a, const double* b, size_t size)
{
  #pragma omp parallel for
  for (size_t i = 0; i < size; ++i)
  {
    a[i] = a[i] - b[i];
  }
}

void multiply_on_scalar(double* a, size_t size, double scalar)
{
  #pragma omp parallel for
  for (size_t i = 0; i < size; ++i)
  {
    a[i] *= scalar;
  }
}

double get_norm(const double* a, size_t size)
{
  double sum = 0;

  #pragma omp parallel for reduction (+:sum)
  for (size_t i = 0; i < size; ++i)
  {
    sum += a[i] * a[i];
  }

  return sqrt(sum);
}

int main()
{
  double* A = (double*)malloc(sizeof(double) * N * N);
  if (!A) goto bad_A;

  double* b = (double*)malloc(sizeof(double) * N);
  if (!b) goto bad_b;

  double* x = (double*)malloc(sizeof(double) * N);
  if (!x) goto bad_x;

  double* tmp = (double*)malloc(sizeof(double) * N);
  if (!tmp) goto bad_tmp;

  init_matrix_A(A, N);
  init_vector_b(b, N);
  init_vector_x(x, N);

  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);

  double norm_b = get_norm(b, N);

  while (1)
  {
    multiply_matrix_vector(A, x, N, tmp);
    subtract_vectors(tmp, b, N);

    double norm_tmp = get_norm(tmp, N);
    if ((norm_tmp / norm_b) < E) break;

    multiply_on_scalar(tmp, N, T);
    subtract_vectors(x, tmp, N);
  }

  // for (size_t i = 0; i < N; ++i)
  // {
  //   printf("%.1f ", x[i]);
  // }
  // printf("\n");

  clock_gettime(CLOCK_MONOTONIC_RAW, &end);
  printf("%lf\n", end.tv_sec-start.tv_sec + 0.000000001*(end.tv_nsec-start.tv_nsec));

  free(tmp);

  bad_tmp:
  free(x);

  bad_x:
  free(b);

  bad_b:
  free(A);

  bad_A:

  return 0;
}
