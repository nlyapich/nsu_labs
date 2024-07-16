#include <ctime>
#include <iostream>
#include <x86intrin.h> //for __rdtsc()

void fillStraight(int* arr, size_t length)
{
  for (size_t i = 0; i < length; ++i)
  {
    arr[i] = i + 1;
  }
  arr[length - 1] = 0;
}

void fillReverse(int* arr, size_t length)
{
  for (size_t i = 0; i < length; ++i)
  {
    arr[i] = i - 1;
  }
  arr[0] = length - 1;
}

void fillRandom(int* arr, size_t length)
{
  srand(time(nullptr));

  bool *used = new bool[length];

  for (size_t i = 0; i < length; ++i)
  {
    used[i] = false;
  }

  int j = 0;
  used[0] = true;

  for (size_t i = 0; i < length - 1; ++i)
  {
    int tmp;

    while (used[tmp = rand() % length] != false);

    arr[j] = tmp;
    used[tmp] = true;
    j = tmp;
  }

  arr[j] = 0;
  delete[] used;
}

double countTimeInTicksDetour(const int* arr, size_t length)
{
  const int numberRepetitions = 100;
  double countTime = __rdtsc();
  for (int j = 0; j < numberRepetitions; ++j)
  {
    int k = 0;
    double start = __rdtsc();
    for (size_t i = 0; i < length; ++i) k = arr[k];
    double end = __rdtsc();

    if (k != 0)
      std::cout << "k != 0" << std::endl;

    double tmp = end - start;
    if (tmp < countTime)
      countTime = tmp;
  }

  return (countTime / length);
}

void runTask(size_t startSize, size_t endSize, double multyTurn, void (*fillArray)(int*, size_t))
{
  int* arr = new int[endSize];

  for (size_t nowSize = startSize; nowSize <= endSize; nowSize *= multyTurn)
  {
    fillArray(arr, nowSize);
    double time = countTimeInTicksDetour(arr, nowSize);

    // std::cout << nowSize << " " << time << std::endl;
    std::cout << time << std::endl;
  }

  delete[] arr;
}

int main()
{
  const size_t _startSize = 1024 / sizeof(int);
  const size_t _endSize = 32 * 1024 * 1024 / sizeof(int);
  const double _multyTurn = 2;

  std::cout << "----------------------FakeGoRandom----------------------" << std::endl;
  runTask(_startSize, _endSize, _multyTurn, fillRandom);

  std::cout << "----------------------Straight----------------------" << std::endl;
  runTask(_startSize, _endSize, _multyTurn, fillStraight);

  std::cout << "----------------------Reverse----------------------" << std::endl;
  runTask(_startSize, _endSize, _multyTurn, fillReverse);

  std::cout << "----------------------Random----------------------" << std::endl;
  runTask(_startSize, _endSize, _multyTurn, fillRandom);

  return 0;
}
