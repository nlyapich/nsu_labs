#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mpi.h"

#define N 848
#define T 0.000001
#define E 0.00001

#define NUM_DIMS 1

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

void multiply_matrix_vector(const double* matrix, double* vector, size_t size, double* res, int rank, int size_mpi, MPI_Comm comm_cart, int sour, int dest)
{
  MPI_Status st;

  for(int k = 0; k < size_mpi; ++k)
  {
    /* Каждая ветвь вычисляет координаты (вдоль
    * строки) для результирующих элементов
    * матрицы C, которые зависят от номера
    * цикла k и ранга компьютера. */
    int d = ((rank + k)%size_mpi)*size/size_mpi;

    /* Каждая ветвь производит умножение своей
    * полосы матрицы A на текущую полосу матрицы
    * B */
    for(int i = 0; i < size/size_mpi; ++i)
    {
      if (k == 0)
      {
        res[i] = 0.0;
      }
      for(int j = d, jv = 0; j < d + size/size_mpi; ++j, ++jv)
      {
        res[i] += matrix[i*size + j] * vector[jv];
      }
    }

    /* Умножение полосы строк матрицы A на полосу
    * столбцов матрицы B в каждой ветви завершено
    */
    /* Каждая ветвь передает своим соседним ветвям
    * с меньшим рангом вертикальные полосы
    * матрицы B. Т.е. полосы матрицы B сдвигаются
    * вдоль кольца компьютеров */
    MPI_Sendrecv_replace(vector, size/size_mpi, MPI_DOUBLE, dest, 12, sour, 12, comm_cart, &st);
  }
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

double get_norm(const double* a, size_t size)
{
  double sum = 0;
  double tmp_sum = 0;

  for (size_t i = 0; i < size; ++i)
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

  double* A = (double*)malloc(sizeof(double) * N * N / size_mpi);
  if (!A) goto bad_A;

  double* b = (double*)malloc(sizeof(double) * N / size_mpi);
  if (!b) goto bad_b;

  double* x = (double*)malloc(sizeof(double) * N / size_mpi);
  if (!x) goto bad_x;

  double* tmp = (double*)malloc(sizeof(double) * N / size_mpi);
  if (!tmp) goto bad_tmp;

  int* rcount = (int*)malloc(sizeof(int) * size_mpi);
  if (!rcount) goto bad_rcount;

  int* displs = (int*)malloc(sizeof(int) * size_mpi);
  if (!displs) goto bad_displs;

  init_rcount(rcount, N, size_mpi);
  init_displs(displs, N, size_mpi);
  init_matrix_A(A, N, N / size_mpi, rank);
  init_vector_b(b, N / size_mpi);
  init_vector_x(x, N / size_mpi);

  int d, sour, dest;
  int dims[NUM_DIMS], periods[NUM_DIMS];
  int new_coords[NUM_DIMS];
  int reorder = 0;
  MPI_Comm comm_cart;

  /* Обнуляем массив dims и заполняем массив
  * periods для топологии "кольцо" */
  for(int i = 0; i < NUM_DIMS; ++i)
  {
    dims[i] = 0;
    periods[i] = 1;
  }
  /* Заполняем массив dims, где указываются
  * размеры (одномерной) решетки */
  MPI_Dims_create(size_mpi, NUM_DIMS, dims);

  /* Создаем топологию "кольцо" с
  comm_cart */
  MPI_Cart_create(MPI_COMM_WORLD, NUM_DIMS, dims, periods, reorder, &comm_cart);

  /* Отображаем ранги на координаты компьютеров,
  с целью оптимизации отображения заданной
  виртуальной топологии на физическую
  топологию системы. */
  MPI_Cart_coords(comm_cart, rank, NUM_DIMS, new_coords);

  /* Каждая ветвь находит своих соседей вдоль
  * кольца, в направлении меньших значений
  * рангов */
  MPI_Cart_shift(comm_cart, 0, -1, &sour, &dest);

  double norm_b;

  struct timespec start, end;

  if (rank == 0)
  {
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  }

  norm_b = get_norm(b, N / size_mpi);

  while (1)
  {
    multiply_matrix_vector(A, x, N, tmp, rank, size_mpi, comm_cart, sour, dest);
    subtract_vectors(tmp, b, N / size_mpi);

    double norm_tmp = get_norm(tmp, N / size_mpi);

    if ((norm_tmp / norm_b) < E) break;

    multiply_on_scalar(tmp, N / size_mpi, T);
    subtract_vectors(x, tmp, N / size_mpi);
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

  /* Все ветви завершают системные процессы,
  * с топологией comm_cart и завершают
  * выполнение программы */
  MPI_Comm_free(&comm_cart);

  MPI_Finalize();

  return 0;
}
