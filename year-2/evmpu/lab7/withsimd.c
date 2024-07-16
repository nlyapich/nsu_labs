#include <xmmintrin.h>
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
  float maxRS = matrix[0];

  for (size_t i = 0; i < N; ++i)
  {
    float tmpSum = 0;
    for (size_t j = 0; j < N; ++j)
    {
      tmpSum += matrix[i*N + j];
    }
    if (tmpSum > maxRS)
    {
      maxRS = tmpSum;
    }
  }

  return maxRS;
}

float maxColumnSum(const float* matrix)
{
  float maxCS = matrix[0];

  for (size_t i = 0; i < N; ++i)
  {
    float tmpSum = 0;
    for (size_t j = 0; j < N; ++j)
    {
      tmpSum += matrix[j*N + i];
    }
    if (tmpSum > maxCS)
    {
      maxCS = tmpSum;
    }
  }

  return maxCS;
}

void multiplyOnScalarMatrix(float* B, float scalar)
{
  __m128 number = _mm_set1_ps(scalar);

  for (int i = 0; i < N*N; i += 4)
  {
    __m128 line = _mm_load_ps(B + i);
    __m128 res = _mm_mul_ps(line, number);
    _mm_store_ps(B + i, res);
  }
}

void multiplyMatrices(const float* a, const float* b, float* res)
{
  for (int i = 0; i < N; i++)
  {
    for (int k = 0; k < N; k+=4)
    {
      __m128 sum = _mm_setzero_ps();
      for (int j = 0; j < N; j++)
      {
        __m128 line1 = _mm_set1_ps(a[i * N + j]);
        __m128 line2 = _mm_load_ps(b + j * N + k);
        __m128 tmpMult = _mm_mul_ps(line1, line2);
        sum = _mm_add_ps(sum, tmpMult);
      }
      _mm_store_ps(res + i * N + k, sum);
    }
  }
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
  __m128 a_mm = _mm_load_ps(a);
  __m128 b_mm = _mm_load_ps(b);
  __m128 sub = _mm_sub_ps(a_mm, b_mm);
  _mm_store_ps(res, sub);
}

void sumMatrices(const float* a, const float* b, float* res)
{
  __m128 a_mm = _mm_load_ps(a);
  __m128 b_mm = _mm_load_ps(b);
  __m128 sum = _mm_sub_ps(a_mm, b_mm);
  _mm_store_ps(res, sum);
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

  float* I = (float*)malloc(sizeof(float)*N*N);
  identityMatrix(I);

  float* R = (float*)malloc(sizeof(float)*N*N);
  subtractMatrices(I, BA, R);

  float* multiR = (float*)malloc(sizeof(float)*N*N);
  memcpy(multiR, R, sizeof(float)*N*N);

  memcpy(res, I, sizeof(float)*N*N);

  float* tmp = (float*)malloc(sizeof(float)*N*N);
  for (size_t i = 1; i < M; ++i)
  {
    memcpy(tmp, multiR, sizeof(float)*N*N);

    multiplyMatrices(tmp, R, multiR);

    sumMatrices(res, multiR, res);
  }

  memcpy(tmp, res, sizeof(float)*N*N);

  multiplyMatrices(tmp, B, res);

  free(tmp);
  free(multiR);
  free(R);
  free(I);
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
