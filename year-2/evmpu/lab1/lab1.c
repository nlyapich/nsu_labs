#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.1415926535

double calc_sin(double x, long int n)
{
  x = x * PI / 180;
  double sum = x;
  double sin = 0;

  for(long int i = 1; i <= n; ++i)
  {
   sin += sum;
   sum *= (-1)*x*x/(2*i)/(2*i+1);
  }

  return sin;
}

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    printf("Bad input\n");
    return 0;
  }

  long int n = atol(argv[1]);

  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);

  double sin = calc_sin(60, n);

  clock_gettime(CLOCK_MONOTONIC_RAW, &end);

  printf("sin = %lf\n", sin);
  printf("Time taken: %lf sec.\n", end.tv_sec-start.tv_sec + 0.000000001*(end.tv_nsec-start.tv_nsec));

  return 0;
}
