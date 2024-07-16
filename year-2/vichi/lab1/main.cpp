#include <iostream>
#include <vector>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

double a, b, c, epsilon, delta;

static double f(double x);
static int find_extremes(double *extreme_1, double *extreme_2);
static std::vector<std::vector<double>> find_roots();
static double find_border(double border_1, bool to_right);
static double dichotomy_method(double border_1, double border_2);

int main(int argc, char **argv) {

    std::cout << "Enter numbers a, b, c, epsilon" << std::endl;
    std::cin >> a >> b >> c >> epsilon;
    if (epsilon == 0)
    {
      std::cout << "Error! Bad input: epsilon == 0" << std::endl;
      return 0;
    }

    delta = 1000;
    std::vector<std::vector<double>> roots = find_roots();

    if (!roots.empty())
    {
      for (auto elem : roots)
      {
        std::cout << "root: " << elem[0] << "; multicity: " << elem[1] << std::endl;
      }
    }
    else
    {
      std::cout << "No roots" << std::endl;
    }

    return 0;
}

double f(double x) {
    return x * x * x + a * x * x + b * x + c;
}

std::vector<std::vector<double>> find_roots() {
    std::vector<std::vector<double>> roots{};
    int res;
    double extreme_1, extreme_2;

    res = find_extremes(&extreme_1, &extreme_2);

    if (res == 0) {
        double border_1 = 0;
        double f_0 = f(border_1);
        if (fabs(f_0) < epsilon) {
            roots.push_back({border_1, 3});
        } else if (f_0 < -epsilon) {
            double border_2 = find_border(0, true);
            roots.push_back({dichotomy_method(border_1, border_2), 1});
        } else {
            double border_2 = find_border(0, false);
            roots.push_back({dichotomy_method(border_1, border_2), 1});
        }
    } else {
        double f_1 = f(extreme_1);
        double f_2 = f(extreme_2);

        if (f_1 >= epsilon && f_2 >= epsilon) {
            double border_1 = find_border(extreme_1, false);
            roots.push_back({dichotomy_method(border_1, extreme_1), 1});
        } else if (f_1 <= -epsilon && f_2 <= -epsilon) {
            double border_2 = find_border(extreme_2, true);
            roots.push_back({dichotomy_method(extreme_1, border_2), 1});
        } else if (fabs(f_1) < epsilon && fabs(f_2) < epsilon) {
            double new_dote = (extreme_1 + extreme_2) / 2;
            roots.push_back({new_dote, 3});
        } else if (f_1 >= epsilon && fabs(f_2) < epsilon) {
            double border_1 = find_border(extreme_1, false);
            roots.push_back({dichotomy_method(border_1, extreme_1), 1});
            roots.push_back({extreme_2, 2});
        } else if (fabs(f_1) < epsilon && f_2 <= -epsilon) {
            double border_2 = find_border(extreme_2, true);
            roots.push_back({extreme_1, 2});
            roots.push_back({dichotomy_method(extreme_2, border_2), 1});
        } else if (f_1 >= epsilon && f_2 <= -epsilon) {
            double border_1 = find_border(extreme_1, false);
            double border_2 = find_border(extreme_2, true);
            roots.push_back({dichotomy_method(border_1, extreme_1), 1});
            roots.push_back({dichotomy_method(extreme_1, extreme_2), 1});
            roots.push_back({dichotomy_method(extreme_2, border_2), 1});
        }
    }
    return roots;
}

int find_extremes(double *extreme_1, double *extreme_2) {
    double discriminant = a * a - 3 * b;
    double y_0 = -discriminant / 3;

    if (y_0 >= epsilon) return 0;

    *extreme_1 = (-a - sqrt(discriminant)) / 3;
    *extreme_2 = (-a + sqrt(discriminant)) / 3;

    return 2;
}

double find_border(double border_1, bool to_right) {
    double value_1 = f(border_1);
    double step = delta * (to_right ? 1 : -1);
    double border_2 = border_1 + step;

    while (value_1 * f(border_2) > 0) {
        border_2 += step;
    }

    return border_2;
}

double dichotomy_method(double border_1, double border_2) {
    double f_1 = f(border_1);
    double f_2 = f(border_2);

    if (fabs(f_1) < epsilon) return border_1;
    if (fabs(f_2) < epsilon) return border_2;

    int count = 0;
    while (true) {
        count++;
        double cc = (border_1 + border_2) / 2;
        double f_cc = f(cc);

        if (fabs(f_cc) < epsilon){
          std::cout << "Count = " << count << std::endl;
          return cc;
        }
        if (f_1 * f_cc < 0) {
            border_2 = cc;
        } else {
            border_1 = cc;
            f_1 = f_cc;
        }
    }
}
