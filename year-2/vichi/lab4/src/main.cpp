#include <iostream>
#include <vector>

std::vector<double> sweep_method(std::vector<double> a, std::vector<double> b, std::vector<double> c, std::vector<double> f)
{
  int n = f.size();

  // пр. проход

  std::vector<double> alpha(n);
  std::vector<double> beta(n);

  alpha[0] = b[0] / c[0];
  beta[0] = f[0] / c[0];

  for (int i = 1; i < n; ++i)
  {
    alpha[i] = b[i] / (-a[i] * alpha[i-1] + c[i]);
    beta[i] = (f[i] + a[i] * beta[i-1]) / (-a[i] * alpha[i-1] + c[i]);
  }

  // об. проход

  std::vector<double> x(n);
  x[n-1] = beta[n-1];

  for (int i = n-2; i >= 0; --i)
  {
    x[i] = alpha[i] * x[i + 1] + beta[i];
  }

  return x;
}

int main()
{
  int n;
  double epsilon, gamma;
  std::cout << "Введите n, epsilon и gamma: ";
  std::cin >> n >> epsilon >> gamma;
  std::cout << std::endl;

  std::cout << "Test1:" << std::endl;

  std::vector<double> a1(n), b1(n), c1(n), f1(n);
  for (int i = 0; i < n; ++i)
  {
    a1[i] = 1;
    b1[i] = 1;
    c1[i] = 2;
    f1[i] = 2;
  }

  std::vector<double> x1 = sweep_method(a1, b1, c1, f1);
  for (int i = 0; i < n; ++i)
  {
    std::cout << x1[i] << " ";
  }
  std::cout << std::endl;

  std::cout << std::endl;
  std::cout << "Test2:" << std::endl;

  std::vector<double> a2(n), b2(n), c2(n), f2(n);
  for (int i = 0; i < n; ++i)
  {
    a2[i] = 1;
    b2[i] = 1;
    c2[i] = 2;
    f2[i] = 2 + epsilon;
  }

  std::vector<double> x2 = sweep_method(a2, b2, c2, f2);
  for (int i = 0; i < n; ++i)
  {
    std::cout << x2[i] << " ";
  }
  std::cout << std::endl;

  std::cout << std::endl;
  std::cout << "Test3:" << std::endl;

  std::vector<double> a3(n), b3(n), c3(n), f3(n);
  for (int i = 0; i < n; ++i)
  {
    a3[i] = 1;
    b3[i] = 1;
    c3[i] = 2*i + gamma;
    f3[i] = 2*(i+1) + gamma;
  }

  std::vector<double> x3 = sweep_method(a3, b3, c3, f3);
  for (int i = 0; i < n; ++i)
  {
    std::cout << x3[i] << " ";
  }
  std::cout << std::endl;

  std::cout << std::endl;
  std::cout << "Test4:" << std::endl;

  n = 7;
  gamma = 5;

  std::vector<double> a4(n), b4(n), c4(n), f4(n);
  for (int i = 0; i < n; ++i)
  {
    a4[i] = 1;
    b4[i] = 1;
    c4[i] = 7 + 2*i;
    f4[i] = 9 + 2*i;
  }

  std::vector<double> x4 = sweep_method(a4, b4, c4, f4);
  for (int i = 0; i < n; ++i)
  {
    std::cout << x4[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}
