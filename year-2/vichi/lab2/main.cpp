#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

double calculate_area(double a, double b)
{
  double d = (b - a) / 6;

  std::vector<double> x = {a, a + 2*d, a + 4*d, a + 6*d};

  std::vector<double> f(4);
  for (int i = 0; i < 4; ++i)
  {
    f[i] = std::exp(x[i]);
  }

  double result = 3 * (f[0] + 3*f[1] + 3*f[2] + f[3]) * d / 4;

  return result;
}

double quadrature_integration(double a, double b, int n)
{
  double d = (b - a) / n;
  double result = 0;
  double cur_coor = a;
  for (int i = 0; i < n; ++i)
  {
    result += calculate_area(cur_coor, cur_coor + d);
    cur_coor += d;
  }
  return result;
}

double trapezoid_integration(double a, double b, int n)
{
  double h = (b - a) / n;

  std::vector<double> v(n + 1);

  for (int i = 0; i < v.size(); ++i) {
    v[i] = std::exp(a + i * h);
  }

  double result = 0;
  for (int i = 0; i < v.size() - 1; ++i)
  {
    result += (v[i] + v[i + 1]) / 2 * h;
  }
  return result;
}

double simpson_integration(double a, double b, int n)
{
  if ((n % 2) == 1)
  {
    n++;
  }

  double h = (b - a) / n;
  double sum = std::exp(a) + std::exp(b);

  for (int i = 1; i < n; i += 2) {
      sum += 4 * std::exp(a + i * h);
      sum += 2 * std::exp(a + (i + 1) * h);
  }

  return h * sum / 3;
}

double runges_rule(double (*integration_method)(double, double, int), double a, double b, int n)
{
    double I1 = integration_method(a, b, n);
    double I2 = integration_method(a, b, 2*n);
    double I3 = integration_method(a, b, 4*n);

    return fabsl((I1 - I2) / (I2 - I3));
}

int main()
{
  int n1;
  std::cout << "Введите n1: ";
  std::cin >> n1;
  std::cout << std::endl;

  double a = 5.0;
  double b = 7.0;
  int n2 = n1 * 2;
  int n3 = n2 * 2;

  std::cout << "Формула трапеции: " << trapezoid_integration(a, b, n1) << std::endl;
  // std::cout << "Правило рунге для формулы трапеции: " << runges_rule(trapezoid_integration, a, b, n1) << std::endl;

  std::cout << std::endl;

  std::cout << "Формула Симпсона: " << simpson_integration(a, b, n2) << std::endl;
  // std::cout << "Правило рунге для формулы Симпсона: " << runges_rule(simpson_integration, a, b, n2) << std::endl;

  std::cout << std::endl;

  std::cout << "Квадратурная формула: " << quadrature_integration(a, b, n3) << std::endl;
  // std::cout << "Правило рунге для квадратурной формулы: " << runges_rule(quadrature_integration, a, b, n3) << std::endl;

  return 0;
}
