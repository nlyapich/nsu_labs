#include <iostream>
#include <x86intrin.h>

const size_t N = 200000000;

double countTime(const int* arr)
{

  const int numberRepetitions = 1;
  int k = 0;

  double countTime = __rdtsc();
  for(int i = 0; i < numberRepetitions; ++i)
  {
    auto start = __rdtsc();
    for(int j = 0; j < N; ++j) k = arr[k];
    auto end = __rdtsc();

    double tmp = end - start;
    if(tmp < countTime)
      countTime = tmp;
  }

  return countTime/N;
}

void fillArray(int *arr, size_t fragments, size_t offset, size_t size)
{
  size_t i = 0;
  size_t j = 1;

  for(i = 0; i < size/fragments; ++i)
  {
    for(j = 1; j < fragments; ++j)
      arr[i + (j - 1) * offset] = i + j * offset;

    arr[i + (j - 1) * offset] = i + 1;
  }

  arr[i - 1 + (j - 1) * offset] = 0;
}

void runTask(int* arr, size_t fragments, size_t offset, size_t size)
{
  fillArray(arr, fragments, offset, size);
  std::cout << countTime(arr) << std::endl;
}

int main()
{
  int* arr = new int[N];

  for(int n = 1; n <= 32; ++n)
      runTask(arr, n, 16 * 1024 * 1024 / sizeof(int), 4 * 1024 * 1024 / sizeof(int));

  delete[] arr;

  return 0;
}
