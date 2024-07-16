#include <cmath>
#include <iostream>
#include <iomanip>

double f1(double x)
{
  return (x*x*x - 10*x + 20);
}

double pf1(double x)
{
  return (3*x*x - 10);
}

double f2(double x)
{
  return (x*x*x - 4*x - 10);
}

double pf2(double x)
{
  return (3*x*x - 4);
}

void method_newton(double epsilon, double x0, double z, double (*f)(double), double (*pf)(double), int max_iteration)
{
  double xn = x0;
  double xn1 = x0;

  std::cout << std::setw(5) << "n"
              << std::setw(18) << "xn"
              << std::setw(18) << "f(xn)"
              << std::setw(18) << "dxn"
              << std::setw(18) << "dxn<epsilon"
              << std::setw(18) << "dn"
              << std::setw(18) << "dn<epsilon"
              << std::setw(18) << "|f(xn)|<epsilon"
              << std::setw(18) << "dn<dn-1^2"
              << std::endl;
  std::cout << std::endl;

  std::cout << std::setw(5) << 0
            << std::setw(18) << x0
            << std::setw(18) << f(x0)
            << std::setw(18) << "-"
            << std::setw(18) << "-"
            << std::setw(18) << "-"
            << std::setw(18) << "-"
            << std::setw(18) << ((f(x0) < epsilon) ? "Да" : "Нет")
            << std::setw(18) << "-"
            << std::endl;

  int cur_iteration = 0;

  while (true)
  {
    cur_iteration++;

    xn = xn1;
    xn1 = xn - f(xn)/pf(xn);

    double dxn = fabs(xn - xn1);
    double fxn1 = f(xn1);

    double dn = fabs(xn - z);
    double dn1 = fabs(xn1 - z);

    std :: cout << std::setw(5) << cur_iteration
                << std::setw(18) << xn1
                << std::setw(18) << fxn1
                << std::setw(18) << dxn
                << std::setw(18) << ((dxn < epsilon) ? "Да" : "Нет")
                << std::setw(18) << dn1
                << std::setw(18) << ((dn1 < epsilon) ? "Да" : "Нет")
                << std::setw(18) << ((fxn1 < epsilon) ? "Да" : "Нет")
                << std::setw(18) << ((dn1 < (dn*dn)) ? "Да" : "Нет")
                << std::endl;

    if (fabs(xn - xn1) < epsilon)
    {
      std::cout << std::endl;
      std::cout << "Count of iteration = " << cur_iteration << std::endl;
      break;
    }

    if (cur_iteration == max_iteration)
    {
      std::cout << std::endl;
      std::cout << "The number of iterations has reached its maximum" << std::endl;
      break;
    }
  }
}

void simplified_method_newton(double epsilon, double x0, double z, double (*f)(double), double (*pf)(double), int max_iteration)
{
  double xn = x0;
  double xn1 = x0;

  double pfx0 = pf(x0);

  std :: cout << std::setw(5) << "n"
              << std::setw(18) << "xn"
              << std::setw(18) << "f(xn)"
              << std::setw(18) << "dxn"
              << std::setw(18) << "dxn<epsilon"
              << std::setw(18) << "dn"
              << std::setw(18) << "dn<epsilon"
              << std::setw(18) << "|f(xn)|<epsilon"
              << std::setw(18) << "dn<dn-1^2"
              << std::endl;
  std::cout << std::endl;

  std::cout << std::setw(5) << 0
            << std::setw(18) << x0
            << std::setw(18) << f(x0)
            << std::setw(18) << "-"
            << std::setw(18) << "-"
            << std::setw(18) << "-"
            << std::setw(18) << "-"
            << std::setw(18) << ((f(x0) < epsilon) ? "Да" : "Нет")
            << std::setw(18) << "-"
            << std::endl;

  int cur_iteration = 0;

  while (true)
  {
    cur_iteration++;

    xn = xn1;
    xn1 = xn - f(xn)/pfx0;

    double dxn = fabs(xn - xn1);
    double fxn1 = f(xn1);

    double dn = fabs(xn - z);
    double dn1 = fabs(xn1 - z);

    std :: cout << std::setw(5) << cur_iteration
                << std::setw(18) << xn1
                << std::setw(18) << fxn1
                << std::setw(18) << dxn
                << std::setw(18) << ((dxn < epsilon) ? "Да" : "Нет")
                << std::setw(18) << dn1
                << std::setw(18) << ((dn1 < epsilon) ? "Да" : "Нет")
                << std::setw(18) << ((fxn1 < epsilon) ? "Да" : "Нет")
                << std::setw(18) << ((dn1 < (dn*dn)) ? "Да" : "Нет")
                << std::endl;

    if (fabs(xn - xn1) < epsilon)
    {
      std::cout << std::endl;
      std::cout << "Count of iteration = " << cur_iteration << std::endl;
      break;
    }

    if (cur_iteration == max_iteration)
    {
      std::cout << std::endl;
      std::cout << "The number of iterations has reached its maximum" << std::endl;
      break;
    }
  }
}

void secant_method(double epsilon, double x0, double x1, double z, double (*f)(double), int max_iteration)
{
  double xn0 = x0;
  double xn = x1;
  double xn1 = x0;

  std :: cout << std::setw(5) << "n"
              << std::setw(18) << "xn"
              << std::setw(18) << "f(xn)"
              << std::setw(18) << "dxn"
              << std::setw(18) << "dxn<epsilon"
              << std::setw(18) << "dn"
              << std::setw(18) << "dn<epsilon"
              << std::setw(18) << "|f(xn)|<epsilon"
              << std::setw(18) << "dn<dn-1^2"
              << std::endl;
  std::cout << std::endl;

  std::cout << std::setw(5) << 0
            << std::setw(18) << x0
            << std::setw(18) << f(x0)
            << std::setw(18) << "-"
            << std::setw(18) << "-"
            << std::setw(18) << "-"
            << std::setw(18) << "-"
            << std::setw(18) << ((f(x0) < epsilon) ? "Да" : "Нет")
            << std::setw(18) << "-"
            << std::endl;

  int cur_iteration = 0;

  while (true)
  {
    cur_iteration++;

    xn0 = xn;
    xn = xn1;

    xn1 = xn - (f(xn)*(xn - xn0))/(f(xn) - f(xn0));

    double dxn = fabs(xn - xn1);
    double fxn1 = f(xn1);

    double dn = fabs(xn - z);
    double dn1 = fabs(xn1 - z);

    std :: cout << std::setw(5) << cur_iteration
                << std::setw(18) << xn1
                << std::setw(18) << fxn1
                << std::setw(18) << dxn
                << std::setw(18) << ((dxn < epsilon) ? "Да" : "Нет")
                << std::setw(18) << dn1
                << std::setw(18) << ((dn1 < epsilon) ? "Да" : "Нет")
                << std::setw(18) << ((fxn1 < epsilon) ? "Да" : "Нет")
                << std::setw(18) << ((dn1 < (dn*dn)) ? "Да" : "Нет")
                << std::endl;

    if (fabs(xn - xn1) < epsilon)
    {
      std::cout << std::endl;
      std::cout << "Count of iteration = " << cur_iteration << std::endl;
      break;
    }

    if (cur_iteration == max_iteration)
    {
      std::cout << std::endl;
      std::cout << "The number of iterations has reached its maximum" << std::endl;
      break;
    }
  }
}

int main()
{
  double epsilon = 0.00001;

  double x01 = -3.6;
  double z1 = -3.89102041;

  // std::cout << "f1(x) = x^3 - 10x + 20" << std::endl;

  std::cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
  std::cout << std::setw(85) << "NEWTON'S METHOD (f1)" << std::endl;
  std::cout << std::endl;
  method_newton(epsilon, x01, z1, f1, pf1, 100);
  std::cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

  std::cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
  std::cout << std::setw(85) << "SIMPLIFIED NEWTON'S METHOD (f1)" << std::endl;
  std::cout << std::endl;
  simplified_method_newton(epsilon, x01, z1, f1, pf1, 100);
  std::cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

  std::cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
  std::cout << std::setw(85) << "SECANT METHOD (f1)" << std::endl;
  std::cout << std::endl;
  secant_method(epsilon, x01, x01 + 0.05, z1, f1, 100);
  std::cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

  double x02 = 3;
  double z2 = 2.76081783;

  // std::cout << "f2(x) = x^3 - 4x - 10" << std::endl;

  std::cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
  std::cout << std::setw(85) << "NEWTON'S METHOD (f2)" << std::endl;
  std::cout << std::endl;
  method_newton(epsilon, x02, z2, f2, pf2, 100);
  std::cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

  std::cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
  std::cout << std::setw(85) << "SIMPLIFIED NEWTON'S METHOD (f2)" << std::endl;
  std::cout << std::endl;
  simplified_method_newton(epsilon, x02, z2, f2, pf2, 100);
  std::cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

  std::cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
  std::cout << std::setw(85) << "SECANT METHOD (f2)" << std::endl;
  std::cout << std::endl;
  secant_method(epsilon, x02, x02 + 0.05, z2, f2, 100);
  std::cout << "----------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

  return 0;
}
