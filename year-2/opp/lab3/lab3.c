#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<time.h>
#include<sys/time.h>

#define N1 2400
#define N2 4800
#define N3 1200

#define NUM_DIMS 2

#define X 0 // номер измерения X. Отвечает за направление "по строкам".
#define Y 1 // направление "по столбцам". индекс доступа.

int grid_rank, row_rank, column_rank, cnt_processes;
int *send_row_cnt, *send_row_begin_pos, *send_row_size;
int *send_column_cnt, *send_column_begin_pos, *send_column_size;

void create_communicators(MPI_Comm *grid_comm, MPI_Comm *row_comm, MPI_Comm *column_comm, int *dims, int *coords)
{
  int reorder = 1; // флаг о проведении перенумеровки процессов для топологии сетки
  int periods[NUM_DIMS] = {0, 0}; // логический массив с флагами цикличности по каждому из направлений

  MPI_Dims_create(cnt_processes, NUM_DIMS, dims); // определение оптимальной кофигурации сетки и запись ее в dims
  MPI_Cart_create(MPI_COMM_WORLD, NUM_DIMS, dims, periods, reorder, grid_comm); // создание коммуникатора сетки
  MPI_Comm_rank(*grid_comm, &grid_rank);
  MPI_Cart_get(*grid_comm, 2, dims, periods, coords); // получение координат процесса в сетке

  MPI_Comm_split(*grid_comm, coords[Y], coords[X], row_comm); // Здесь "color" == нахождение в одной строке
  MPI_Comm_split(*grid_comm, coords[X], coords[Y], column_comm);
  /*
   *  split -  разделение топологии сетки на непересекающиеся подгруппы. Разделение по признаку "color".
   * "key" - для упорядочивания процессов внутри подгруппы.
   * "одинаковые" группы - получатся с "одинаковой с 0" нумерацией
   */
  row_rank = coords[X];
  column_rank = coords[Y];
}

void init_matrix_A(double* A, size_t size1, size_t size2)
{
  for (size_t i = 0; i < size1; ++i)
  {
    for (size_t j = 0; j < size2; ++j)
    {
      A[i*size2 + j] = 1;
    }
  }
}

void init_matrix_B(double* B, size_t size1, size_t size2)
{
  for (size_t i = 0; i < size1; ++i)
  {
    for (size_t j = 0; j < size2; ++j)
    {
      B[i*size2 + j] = 2;
    }
  }
}

void send_matrices_parts(const double *A, const double *B, double *A_part, double *B_part, MPI_Comm row_comm, MPI_Comm column_comm, int *coords)
{
  // рассылаем A по первому столбцу по всем его строкам
  if (coords[Y] == 0)
  {
    MPI_Scatterv(A, send_row_size, send_row_begin_pos, MPI_DOUBLE, A_part, send_row_size[row_rank], MPI_DOUBLE, 0, row_comm);
  }

  // рассылаем B по первой строке и всем её столбцам
  if (coords[X] == 0)
  {
    MPI_Datatype row_t;
    MPI_Type_contiguous(N2, MPI_DOUBLE, &row_t);
    MPI_Type_commit(&row_t);

    MPI_Datatype column_t;
    MPI_Type_vector(N2, 1, N3, MPI_DOUBLE, &column_t);
    MPI_Type_commit(&column_t);

    MPI_Datatype column_shell_t;
    MPI_Type_create_resized(column_t, 0, sizeof(double), &column_shell_t);
    MPI_Type_commit(&column_shell_t);

    MPI_Scatterv(B, send_column_cnt, send_column_begin_pos, column_shell_t, B_part, send_column_cnt[column_rank], row_t, 0, column_comm);

    MPI_Type_free(&column_t);
    MPI_Type_free(&row_t);
    MPI_Type_free(&column_shell_t);
  }

  MPI_Bcast(A_part, send_row_size[row_rank], MPI_DOUBLE, 0, column_comm);
  MPI_Bcast(B_part, send_column_size[column_rank], MPI_DOUBLE, 0, row_comm);
}

void multiply_matrices(const double *A, const double *B, double* C, size_t size1, size_t size2, size_t size3)
{
  for (int i = 0; i < size1; ++i)
  {
    for (int k = 0; k < size3; ++k)
    {
      for (int j = 0; j < size2; ++j)
      {
        C[i * size3 + k] += A[i * size2 + j] * B[k * size2 + j];
      }
    }
  }
}

void collecting_C(double *C_part, double *C, const int *dims, MPI_Comm grid_comm)
{
  int *displs = (int*) malloc(cnt_processes * sizeof(int));
  int *recv_counts = (int*) malloc(cnt_processes * sizeof(int));

  int offset = 0;
  for (int i = 0; i < dims[X]; ++i) {
    for (int j = 0; j < dims[Y]; ++j) {
      displs[i * dims[Y] + j] = offset; // считает сдвиги в размерности принимаемого типа
      recv_counts[i * dims[Y] + j] = 1;
      offset += send_column_cnt[j];
    }
    offset += (send_row_cnt[i] - 1) * N3;
  }

  MPI_Datatype send_part_t;
  MPI_Type_contiguous(send_column_cnt[0] * send_row_cnt[0], MPI_DOUBLE, &send_part_t);
  MPI_Type_commit(&send_part_t);

  MPI_Datatype recv_part_t;
  MPI_Type_vector(send_row_cnt[0], send_column_cnt[0], N3, MPI_DOUBLE, &recv_part_t);
  MPI_Type_commit(&recv_part_t);

  MPI_Datatype recv_part_shell_t;
  MPI_Type_create_resized(recv_part_t, 0, sizeof(double), &recv_part_shell_t);
  MPI_Type_commit(&recv_part_shell_t);

  MPI_Gatherv(C_part, 1, send_part_t, C, recv_counts, displs, recv_part_shell_t, 0, grid_comm);

  MPI_Type_free(&send_part_t);
  MPI_Type_free(&recv_part_shell_t);

  free(displs);
  free(recv_counts);
}

void print_matrix(const double *matrix, size_t size1, size_t size2)
{
  for (int i = 0; i < size1; ++i)
  {
    for (int j = 0; j < size2; ++j)
    {
      printf("%f ", matrix[i * size2 + j]);
    }
    printf("\n");
  }
  printf("\n");
}

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &cnt_processes);

  int dims[NUM_DIMS] = {0, 0}; // числов процессов вдоль каждого измерения (X, Y)
  int coords[NUM_DIMS]; // координаты процесса в 2d сетке

  MPI_Comm grid_comm;
  MPI_Comm row_comm;
  MPI_Comm column_comm;

  create_communicators(&grid_comm, &row_comm, &column_comm, dims, coords);

  send_row_cnt = (int*) malloc(sizeof(int) * dims[X]);
  send_row_begin_pos = (int*) malloc(sizeof(int) * dims[X]);
  send_row_size = (int*) malloc(sizeof(int) * dims[X]);

  send_column_cnt = (int*) malloc(sizeof(int) * dims[Y]);
  send_column_begin_pos = (int*) malloc(sizeof(int) * dims[Y]);
  send_column_size = (int*) malloc(sizeof(int) * dims[Y]);

  int offset = 0;
  for (int rank_in_row_comm = 0; rank_in_row_comm < dims[X]; ++rank_in_row_comm)
  {
    if (rank_in_row_comm < N1 % dims[X])
    {
      send_row_cnt[rank_in_row_comm] = N1 / dims[X] + 1;
    }
    else
    {
      send_row_cnt[rank_in_row_comm] = N1 / dims[X];
    }
    send_row_size[rank_in_row_comm] = send_row_cnt[rank_in_row_comm] * N2;
    send_row_begin_pos[rank_in_row_comm] = offset;
    offset += send_row_size[rank_in_row_comm];
  }

  offset = 0;
  for (int rank_in_column_comm = 0; rank_in_column_comm < dims[Y]; ++rank_in_column_comm)
  {
    if (rank_in_column_comm < N3 % dims[Y])
    {
      send_column_cnt[rank_in_column_comm] = N3 / dims[Y] + 1;
    }
    else
    {
      send_column_cnt[rank_in_column_comm] = N3 / dims[Y];
    }
    send_column_size[rank_in_column_comm] = send_column_cnt[rank_in_column_comm] * N2;
    send_column_begin_pos[rank_in_column_comm] = offset;
    offset += send_column_cnt[rank_in_column_comm];
  }

  double *A = NULL, *B = NULL, *C = NULL;

  if (grid_rank == 0)
  {
    A = (double*) malloc(sizeof(double) * N1 * N2);
    B = (double*) malloc(sizeof(double) * N2 * N3);
    C = (double*) malloc(sizeof(double) * N1 * N3);
    init_matrix_A(A, N1, N2);
    init_matrix_B(B, N2, N3);
  }

  double *A_part = (double *) malloc(sizeof(double) * send_row_cnt[row_rank] * N2);
  double *B_part = (double *) malloc(sizeof(double) * send_column_cnt[column_rank] * N2);
  double *C_part = (double *) malloc(sizeof(double) * send_row_cnt[row_rank] * send_column_cnt[column_rank]);

  struct timespec start, end;

  if (grid_rank == 0)
  {
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  }

  send_matrices_parts(A, B, A_part, B_part, row_comm, column_comm, coords);

  multiply_matrices(A_part, B_part, C_part, send_row_cnt[row_rank], N2, send_column_cnt[column_rank]);

  collecting_C(C_part, C, dims, grid_comm);

  if (grid_rank == 0)
  {
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    printf("%lf\n", end.tv_sec-start.tv_sec + 0.000000001*(end.tv_nsec-start.tv_nsec));
  }

  if (grid_rank == 0)
  {
    // printf("matrix C:\n");
    // print_matrix(C, N1, N3);

    free(A);
    free(B);
    free(C);
  }

  free(A_part);
  free(B_part);
  free(C_part);

  free(send_row_size);
  free(send_row_begin_pos);
  free(send_row_cnt);
  free(send_column_size);
  free(send_column_begin_pos);
  free(send_column_cnt);

  MPI_Finalize();

  return 0;
}
