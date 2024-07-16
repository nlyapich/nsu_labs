#include <cmath>
#include <iostream>
#include <vector>

#include <time.h>

#include "mpi.h"

const int ROWS = 256;
const int COLUMNS = 256;
int RESULT_SIZE = 256;

void init_field(char* field, size_t rows, size_t columns)
{
  for (size_t i = 0; i < rows; ++i)
  {
    for (size_t j = 0; j < columns; ++j)
    {
      field[i * columns + j] = 0;
    }
  }

  field[0 * columns + 1] = 1;
  field[1 * columns + 2] = 1;
  field[2 * columns + 0] = 1;
  field[2 * columns + 1] = 1;
  field[2 * columns + 2] = 1;
}

void init_count(int* count, size_t rows, size_t columns, int size_mpi, int rank)
{
  double cr = (double) rows / (double) size_mpi;
  int count_rows = ceil((rank + 1) * cr) - ceil(rank * cr);

  count[0] = ceil(cr) * columns;


  for(int i = 1; i < size_mpi; ++i)
  {
    count[i] = count_rows * columns;
  }
}

void init_offset(int* offset, const int* count, int size_mpi)
{
  offset[0] = 0;

  for(int i = 1; i < size_mpi; ++i)
  {
    offset[i] = offset[i - 1] + count[i - 1];
  }
}

char get(const char* src, int columns, int i, int j)
{
  return src[i * columns + ((j % columns + columns) % columns)];
}

int get_count_neightbor(const char* first, const char* middle, const char* last, int columns, int j)
{
  return get(first, columns, 0, j - 1) + get(first, columns, 0, j) + get(first, columns, 0, j + 1) +
         get(middle, columns, 0, j - 1) + get(middle, columns, 0, j + 1) +
         get(last, columns, 0, j - 1) + get(last, columns, 0, j) + get(last, columns, 0, j + 1);
}

int compare(char *prev, char *current, int rows, int columns)
{
  for(int i = 0; i < rows; ++i)
  {
    for(int j = 0; j < columns; ++j)
    {
      if(prev[i * columns + j] != current[i * columns + j])
      {
        return 0;
      }
    }
  }
  return 1;
}

char get_next_state(int prev_state, int neighbors)
{
  if(prev_state)
  {
    return ((neighbors == 2 || neighbors == 3) ? 1 : 0);
  }

  return ((neighbors == 3) ? 1 : 0);
}

int main(int argc, char** argv)
{
  int rank, size_mpi;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size_mpi);

  MPI_Comm comm = MPI_COMM_WORLD;

  char* field = nullptr;

  int* offset = new int[size_mpi];
  int* count = new int[size_mpi];

  init_count(count, ROWS, COLUMNS, size_mpi, rank);
  init_offset(offset, count, size_mpi);

  if (rank == 0)
  {
    field = new char[ROWS * COLUMNS];

    init_field(field, ROWS, COLUMNS);
  }

  char* first_row = new char[COLUMNS];
  char* last_row = new char[COLUMNS];

  int end = 0;
  int result_block_size = RESULT_SIZE / size_mpi;
  int count_iterations = 0;

  std::vector<char> all_results(RESULT_SIZE);
  std::vector<char> result(RESULT_SIZE);
  std::vector<char*> fields;

  int next_rank = (rank + 1) % size_mpi;
  int prev_rank = (rank + size_mpi - 1) % size_mpi;

  int count_rows = count[rank] / COLUMNS;
  char* current_field = new char[count_rows * COLUMNS];

  struct timespec start_time, end_time;
  if (rank == 0)
  {
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_time);
  }

  MPI_Scatterv(field, count, offset, MPI_CHAR, current_field, count_rows * COLUMNS, MPI_CHAR, 0, comm);

  while(1)
  {
    MPI_Request next_send_req, prev_send_req;
    MPI_Request next_recv_req, prev_recv_req;

    // отправляем первую и последнюю строки
    MPI_Isend(current_field, COLUMNS, MPI_CHAR, prev_rank, rank + 100, comm, &prev_send_req);
    char* tmp_sendbuf = current_field + COLUMNS * (count_rows - 1);
    MPI_Isend(tmp_sendbuf, COLUMNS, MPI_CHAR, next_rank, rank + 200, comm, &next_send_req);

    // принимаем первую и последнюю строки
    MPI_Irecv(first_row, COLUMNS, MPI_CHAR, prev_rank, prev_rank + 200, comm, &prev_recv_req);
    MPI_Irecv(last_row, COLUMNS, MPI_CHAR, next_rank, next_rank + 100, comm, &next_recv_req);

    // расширяем результирующий вектор, если необходимо
    if(count_iterations == RESULT_SIZE)
    {
      RESULT_SIZE *= 2;
      result.resize(RESULT_SIZE);
      result_block_size = RESULT_SIZE / size_mpi;
      all_results.resize(RESULT_SIZE);
    }

    // вычисляем результирующий вектор
    for(int i = 0; i < count_iterations; ++i)
    {
      char* prev_field = fields[i];
      result[i] = compare(prev_field, current_field, count_rows, COLUMNS);
    }

    // отправляем результат
    MPI_Request all_req;
    MPI_Ialltoall(result.data(), result_block_size, MPI_CHAR, all_results.data(), result_block_size, MPI_CHAR, comm, &all_req);

    char* next_field = new char[count_rows * COLUMNS];

    // обновляем середину
    for(int i = 1; i < count_rows - 1; ++i)
    {
      for(int j = 0; j < COLUMNS; ++j)
      {
        char* row = current_field + COLUMNS * i;
        int neighbors = get_count_neightbor(row - COLUMNS, row, row + COLUMNS, COLUMNS, j);
        next_field[i * COLUMNS + j] = get_next_state(current_field[i * COLUMNS + j], neighbors);
      }
    }

    MPI_Wait(&prev_send_req, MPI_STATUS_IGNORE);
    MPI_Wait(&prev_recv_req, MPI_STATUS_IGNORE);

    // обновляем первую строку
    for(int i = 0; i < COLUMNS; ++i)
    {
      int j = 0;
      int neighbors = get_count_neightbor(first_row, current_field, current_field + COLUMNS, COLUMNS, i);
      next_field[j * COLUMNS + i] = get_next_state(current_field[j * COLUMNS + i], neighbors);
    }

    MPI_Wait(&next_send_req, MPI_STATUS_IGNORE);
    MPI_Wait(&next_recv_req, MPI_STATUS_IGNORE);

    // обновляем последнюю строку
    for(int i = 0; i < COLUMNS; ++i)
    {
      int j = count_rows - 1;
      char *prev_row = current_field + COLUMNS * (j - 1);
      int neighbors = get_count_neightbor(prev_row, prev_row + COLUMNS, last_row, COLUMNS, i);
      next_field[j * COLUMNS + i] = get_next_state(current_field[j * COLUMNS + i], neighbors);
    }

    MPI_Wait(&all_req, MPI_STATUS_IGNORE);

    // проверяем вектора флагов
    int is_end = 1;
    for(int i = 0; i < result_block_size; ++i)
    {
      is_end = 1;
      for(int j = 0; j < size_mpi; ++j)
      {
        if(all_results[j * result_block_size + i] == 0)
        {
          is_end = 0;
          break;
        }
      }
      if(is_end == 1)
      {
        break;
      }
    }

    MPI_Allreduce(&is_end, &end, 1, MPI_INT, MPI_SUM, comm);
    if(end)
    {
      break;
    }

    fields.push_back(current_field);
    current_field = next_field;
    count_iterations++;
  }


  if (rank == 0)
  {
    clock_gettime(CLOCK_MONOTONIC_RAW, &end_time);
    printf("%lf\n", end_time.tv_sec-start_time.tv_sec + 0.000000001*(end_time.tv_nsec-start_time.tv_nsec));
  }

  for(int i = 0; i < fields.size(); ++i)
  {
    delete[] fields[i];
  }

  delete[] offset;
  delete[] count;
  delete[] first_row;
  delete[] last_row;
  delete[] current_field;

  if (rank == 0)
  {
    delete[] field;
  }

  MPI_Finalize();

  return 0;
}
