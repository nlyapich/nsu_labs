#include <cblas.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define N 2048
#define M 10

void fillRandomMatrix(float* A)
{
  srand(time(NULL));
  for (size_t i = 0; i < N*N; ++i)
  {
    A[i] = rand() % 10;
  }
}

void transponateMatrix(const float* A, float* res)
{
  for (size_t i = 0; i < N; ++i)
  {
    for (size_t j = 0; j < N; ++j)
    {
      res[i * N + j] = A[j * N + i];
    }
  }
}

float maxRowSum(const float* matrix)
{
  float *tmpSum = (float *)malloc(sizeof(float)*N);

  for (int i = 0; i < N; ++i)
  {
    tmpSum[i] = cblas_sasum(N, matrix + N * i, 1);
  }
  float maxRS = tmpSum[cblas_isamax(N, tmpSum, 1)];
  free (tmpSum);

  return maxRS;
}

float maxColumnSum(const float* matrix)
{
  float *tmpSum = (float *)malloc(sizeof(float)*N);
  for (int i = 0; i < N; ++i)
  {
    tmpSum[i] = cblas_sasum(N, matrix + i, N);
  }

  float maxCS = tmpSum[cblas_isamax(N, tmpSum, 1)];
  free (tmpSum);

  return maxCS;
}

void multiplyOnScalarMatrix(float* B, float scalar)
{
  cblas_sscal(N * N, scalar, B, 1);
}

void multiplyMatrices(const float* a, const float* b, float* res)
{
  cblas_sgemm
  (
    CblasRowMajor, CblasNoTrans, CblasNoTrans,
    N, N, N,
    1.0, a, N,
    b, N, 0.0,
    res, N
  );
  // res = a * b
}

void identityMatrix(float* a)
{
  memset(a, 0, sizeof(float)*N*N);
  for (size_t i = 0; i < N; ++i)
  {
    a[i*N + i] = 1;
  }
}

void subtractMatrices(const float* a, const float* b, float* res)
{
  cblas_scopy(N * N, a, 1, res, 1); // res = a
  cblas_saxpy(N * N, -1, b, 1, res, 1); // res -= b
}

void sumMatrices(const float* a, const float* b, float* res)
{
  cblas_scopy(N * N, a, 1, res, 1); // res = a
  cblas_saxpy(N * N, 1, b, 1, res, 1); // res += b
}

float* inverseMatrix(const float* A)
{
  float* res = (float*)malloc(sizeof(float)*N*N);
  float* B = (float*)malloc(sizeof(float)*N*N);
  transponateMatrix(A, B);

  float maxRS = maxRowSum(A);
  float maxCS = maxColumnSum(A);

  float scalar = 1 / (maxRS * maxCS);
  multiplyOnScalarMatrix(B, scalar);

  float* BA = (float*)malloc(sizeof(float)*N*N);
  multiplyMatrices(B, A, BA);

  float* mI = (float*)malloc(sizeof(float)*N*N);
  identityMatrix(mI);

  float* R = (float*)malloc(sizeof(float)*N*N);
  subtractMatrices(mI, BA, R);

  float* multiR = (float*)malloc(sizeof(float)*N*N);
  // memcpy(multiR, R, sizeof(float)*N*N);
  cblas_scopy(N * N, R, 1, multiR, 1);

  // memcpy(res, I, sizeof(float)*N*N);
  cblas_scopy(N * N, mI, 1, res, 1);

  float* tmp = (float*)malloc(sizeof(float)*N*N);
  for (size_t i = 1; i < M; ++i)
  {
    // memcpy(tmp, multiR, sizeof(float)*N*N);
    cblas_scopy(N * N, multiR, 1, tmp, 1);

    multiplyMatrices(tmp, R, multiR);

    sumMatrices(res, multiR, res);
  }

  // memcpy(tmp, res, sizeof(float)*N*N);
  cblas_scopy(N * N, res, 1, tmp, 1);

  multiplyMatrices(tmp, B, res);

  free(tmp);
  free(multiR);
  free(R);
  free(mI);
  free(BA);
  free(B);

  return res;
}

void printMatrix(const float *matrix) {
  for (size_t i = 0; i < N; ++i)
  {
    for (size_t j = 0; j < N; ++j)
    {
      printf("%.9f ", matrix[i*N + j]);
    }
    printf("\n");
  }
  printf("------------------\n");
}

int main()
{
  float* A = (float*)malloc(sizeof(float)*N*N);
  fillRandomMatrix(A);

  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);

  float* inverseA = inverseMatrix(A);

  clock_gettime(CLOCK_MONOTONIC_RAW, &end);

  printf("Time taken: %lf sec.\n", end.tv_sec-start.tv_sec + 0.000000001*(end.tv_nsec-start.tv_nsec));

  float* res = (float*)malloc(sizeof(float)*N*N);
  multiplyMatrices(A, inverseA, res);

  float maxRS = maxRowSum(res);
  float maxCS = maxColumnSum(res);

  printf("maxRS = %.9f, maxCS = %.9f\n", maxRS, maxCS);

  free(res);
  free(A);
  free(inverseA);

  return 0;
}
