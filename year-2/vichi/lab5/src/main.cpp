#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

double x1_1(double x1, double x2)
{
  return (0.5 * x2 + 0.5);
}

double x2_1(double x1, double x2)
{
  return (0.25 * x2 + 0.75);
}

double x1_2(double x1, double x2)
{
  return (0.5 * x2 + 0.5);
}

double x2_2(double x1, double x2)
{
  return (0.5 * x1 + 0.5);
}

void method_yakobi(std::vector<double> x, std::vector<std::vector<double>> S_yakobi, std::vector<double> x_0, std::vector<double> yakobi_addition, double epsilon, int max_iter)
{
  std::vector<double> x_m = x_0;
  std::vector<double> x_m1 = x_0;

  double zm = fabs(x_m[0] - x[0] + x_m[1] - x[1]);
  int count_iter = 0;

  std::cout << std::setw(5) << "k"
            << std::setw(18) << "x_1^k"
            << std::setw(18) << "x_2^k"
            << std::setw(18) << "||z^k||"
            << std::setw(18) << "||z^k||<epsilon"
            << std::endl;
  std::cout << std::endl;

  std::cout << std::setw(5) << count_iter
            << std::setw(18) << x_m[0]
            << std::setw(18) << x_m[1]
            << std::setw(18) << zm
            << std::setw(18) << ((zm < epsilon) ? "Да" : "Нет")
            << std::endl;

  while((count_iter < max_iter) && !(zm < epsilon))
  {
    count_iter++;

    std::vector<double> tmp = x_m1;

    x_m1[0] = S_yakobi[0][0] * x_m[0] + S_yakobi[0][1] * x_m[1] + yakobi_addition[0];
    x_m1[1] = S_yakobi[1][0] * x_m[0] + S_yakobi[1][1] * x_m[1] + yakobi_addition[1];

    zm = fabs(x_m1[0] + x_m1[1] - x[0] - x[1]);

    std::cout << std::setw(5) << count_iter
              << std::setw(18) << x_m1[0]
              << std::setw(18) << x_m1[1]
              << std::setw(18) << zm
              << std::setw(18) << ((zm < epsilon) ? "Да" : "Нет")
              << std::endl;

    x_m = tmp;
  }

  std::cout << std::endl;
}

void method_zeidel(std::vector<double> x, std::vector<std::vector<double>> S_zeidel, std::vector<double> x_0, std::vector<double> zeidel_addition, double epsilon, int max_iter)
{
  std::vector<double> x_m = x_0;
  std::vector<double> x_m1 = x_0;

  double zm = fabs(x_m[0] - x[0] + x_m[1] - x[1]);

  int count_iter = 0;

  std::cout << std::setw(5) << "k"
            << std::setw(18) << "x_1^k"
            << std::setw(18) << "x_2^k"
            << std::setw(18) << "||z^k||"
            << std::setw(18) << "||z^k||<epsilon"
            << std::endl;
  std::cout << std::endl;

  std::cout << std::setw(5) << count_iter
            << std::setw(18) << x_m[0]
            << std::setw(18) << x_m[1]
            << std::setw(18) << zm
            << std::setw(18) << ((zm < epsilon) ? "Да" : "Нет")
            << std::endl;

  while((count_iter < max_iter) && !(zm < epsilon))
  {
    count_iter++;

    std::vector<double> tmp = x_m1;

    x_m1[0] = S_zeidel[0][0] * x_m[0] + S_zeidel[0][1] * x_m[1] + zeidel_addition[0];
    x_m1[1] = S_zeidel[1][0] * x_m[0] + S_zeidel[1][1] * x_m[1] + zeidel_addition[1];

    zm = fabs(x_m1[0] + x_m1[1] - x[0] - x[1]);

    std::cout << std::setw(5) << count_iter
              << std::setw(18) << x_m1[0]
              << std::setw(18) << x_m1[1]
              << std::setw(18) << zm
              << std::setw(18) << ((zm < epsilon) ? "Да" : "Нет")
              << std::endl;

    x_m = tmp;
  }

  std::cout << std::endl;
}

int main()
{
  double epsilon = 0.001;
  int max_iter = 100;

  std::vector<double> x_0 = {0, 0};

  std::vector<double> x = {1, 1};
  std::vector<double> f = {1, 1};
  std::vector<std::vector<double>> A = {{2, -1}, {-1, 2}};
  std::vector<std::vector<double>> B_yakobi = {{2, 0}, {0, 2}};
  std::vector<std::vector<double>> B_yakobi_inverse = {{0.5, 0}, {0, 0.5}};
  std::vector<std::vector<double>> S_yakobi_1 = {{0, 0.5}, {0.5, 0}};
  std::vector<double> yakobi_addition_1 = {0.5, 0.5};

  std::vector<std::vector<double>> S_yakobi_2 = {{0, 0.5}, {0.5, 0}};
  std::vector<double> yakobi_addition_2 = {0.5, 0.5};

  std::cout << "------------------------------------------------------------------------------------" << std::endl;
  std::cout << std::setw(50) << "YAKOBI'S METHOD" << std::endl;
  std::cout << std::endl;
  method_yakobi(x, S_yakobi_1, x_0, yakobi_addition_1, epsilon, max_iter);
  std::cout << "------------------------------------------------------------------------------------" << std::endl;
  std::cout << std::endl;
  std::cout << "------------------------------------------------------------------------------------" << std::endl;

  std::vector<std::vector<double>> B_zeidel = {{2, 0}, {-1, 2}};
  std::vector<std::vector<double>> B_zeidel_inverse = {{0.5, 0}, {0.25, 0.5}};
  std::vector<std::vector<double>> S_zeidel_1 = {{0, 0.5}, {0, 0.25}};
  std::vector<double> zeidel_addition_1 = {0.5, 0.75};

  std::vector<std::vector<double>> S_zeidel_2 = {{0, 0.5}, {0, 0.25}};
  std::vector<double> zeidel_addition_2 = {0.5, 0.75};

  std::cout << "------------------------------------------------------------------------------------" << std::endl;
  std::cout << std::setw(50) << "ZEIDEL'S METHOD" << std::endl;
  std::cout << std::endl;
  method_zeidel(x, S_zeidel_1, x_0, zeidel_addition_1, epsilon, max_iter);
  std::cout << "------------------------------------------------------------------------------------" << std::endl;
  std::cout << "------------------------------------------------------------------------------------" << std::endl;

  return 0;
}
