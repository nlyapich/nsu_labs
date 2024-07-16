from scipy.stats import norm, nct, chi2, uniform, f
import numpy as np
from math import sqrt

def load_data():
    a = -1
    sigma_sq = 0.7
    epsilon = 0.06

    data_1 = np.array([-1.011, -0.529, -0.688, -0.196, -1.024, 0.292, -0.752, -0.332, -2.590, -0.076,
                      -0.558, -1.296, -1.360, -1.635, -0.830, -0.914, 0.853, -0.037, -1.567, -1.359])

    data_2 = np.array([-1.385, -1.078, 0.491, -1.546, -0.466, -1.497, -2.264, -0.750, -1.099, -3.107,
                      -1.137, -2.230, 0.339, -1.197, 0.249, -1.277, -1.950, -1.202, -2.454, -0.791,
                      -2.530, -1.311, 0.175, -0.045, -0.316, -1.884, -1.648, -1.576, -1.103, -0.750])

    data_1_2 = np.array([-1.011, -0.529, -0.688, -0.196, -1.024, 0.292, -0.752, -0.332, -2.590, -0.076,
                      -0.558, -1.296, -1.360, -1.635, -0.830, -0.914, 0.853, -0.037, -1.567, -1.359,
                      -1.385, -1.078, 0.491, -1.546, -0.466, -1.497, -2.264, -0.750, -1.099, -3.107,
                      -1.137, -2.230, 0.339, -1.197, 0.249, -1.277, -1.950, -1.202, -2.454, -0.791,
                      -2.530, -1.311, 0.175, -0.045, -0.316, -1.884, -1.648, -1.576, -1.103, -0.750])

    data_3 = np.array([0.376, 0.949, 0.108, 0.289, 0.046, 0.879, 0.513, 0.468, 0.565, 0.837,
                      0.398, 0.704, 0.402, 0.410, 0.602, 0.440, 0.810, 0.749, 0.082, 0.233,
                      0.118, 0.983, 0.668, 0.921, 0.766, 0.343, 0.379, 0.687, 0.826, 0.677])

    return a, sigma_sq, epsilon, data_1, data_2, data_1_2, data_3

def task_1(a, sigma_sq, epsilon, X):
    print("\n--------------------<< ЗАДАНИЕ 1 >>-----------------------\n")
    pair_a_with_sigma = []
    pair_a_without_sigma = []
    pair_sigma_with_a = []
    pair_sigma_without_a = []

    n = X.size

    X__ = np.average(X)
    print("X__ = ", X__, "\n")

    q_norm = round(norm.ppf(1 - epsilon/2), 4)
    # print("Квантиль нормального распределения: ", q_norm, "\n")

    pair_a_with_sigma.append(round(-q_norm * sqrt(sigma_sq/n) + X__, 4))
    pair_a_with_sigma.append(round(q_norm * sqrt(sigma_sq/n) + X__, 4))
    print("а) Доверительный интервал для а при известной сигме")
    print("(", pair_a_with_sigma[0],", ", pair_a_with_sigma[1], ")\n")

    st = nct(df = n - 1, nc = 0)
    q_stud = round(st.ppf(1 - epsilon/2), 4)
    # print("Квантиль распределения Стьюдента: ", q_stud)

    S2 = np.average(X * X) - X__*X__ # выборочная дисперсия

    S2_0 = (n/(n-1)) * S2 # несмещенная выборочная дисперсия
    print("S2_0 = ", S2_0)
    pair_a_without_sigma.append(round(X__ - q_stud*sqrt(S2_0/n), 4))
    pair_a_without_sigma.append(round(X__ + q_stud*sqrt(S2_0/n), 4))
    print("б) Доверительный интервал для а при неизвестной сигме")
    print("(", pair_a_without_sigma[0],", ", pair_a_without_sigma[1], ")")
    print("S2_0 = ", S2_0, "\n")

    ch = chi2(df = n - 1)
    q_chi_1 = round(ch.ppf(epsilon/2), 4)
    q_chi_2 = round(ch.ppf(1 - epsilon/2), 4)
    print("Квантиль Хи-квадрат: левый: ", q_chi_1,", правый: ", q_chi_2, "\n")

    X__a = X - a*np.ones(n)
    # print("(x-a)__", X__a)
    new_S = sum(X__a * X__a)
    print("(x-a)^2__ = ", new_S/n)
    pair_sigma_with_a.append(round(new_S/q_chi_2, 4))
    pair_sigma_with_a.append(round(new_S/q_chi_1, 4))
    print("в) Доверительный интервал для сигма при известном а")
    print("(", pair_sigma_with_a[0],", ", pair_sigma_with_a[1], ")\n")

    pair_sigma_without_a.append(round(n*S2/q_chi_2, 4))
    pair_sigma_without_a.append(round(n*S2/q_chi_1, 4))
    print("г) Доверительный интервал для сигма при неизвестном а")
    print("(", pair_sigma_without_a[0],", ", pair_sigma_without_a[1], ")")

    print("\n----------------------------------------------------------\n")

def task_2(epsilon, data_3):
    print("\n--------------------<< ЗАДАНИЕ 2 >>-----------------------\n")
    X = np.array(data_3)
    n = X.size
    X__ = np.average(X)

    def edf(X, y):
        sum = 0
        for i in X:
            if i < y:
                sum += 1
        return sum/X.size

    X_sort = np.sort(X)
    cdf_values_ = np.array([uniform().cdf(x) for x in X_sort])
    edf_values_ = np.array([edf(X, x) for x in X_sort])

    # print(edf_values_, "\n")
    # print(X_sort, "\n")

    def f_emp(edf_values_, X_sort, x):
        x_right = 0
        x_left = 0
        for i in range(X_sort.size):
            if (X_sort[i] >= x):
                x_right = i
                x_left = i - 1
                break

        if (X_sort[X_sort.size - 1] < x):
            return 1

        return edf_values_[x_right]

    print("f_emp = ", f_emp(edf_values_, X_sort, 0.082), "\n")

    q_k = 1.3
    D_n = max(abs(edf_values_ - cdf_values_))
    D = sqrt(n)*D_n

    print("a) Критерий Колмогорова")
    print("D_n = ", round(D_n, 4))
    print("D = ", round(D, 4))

    if D < q_k:
        print("Выборка равномерная")
    else:
        print("Не равномерная")

    print("\nб) Критерий Пирсона")

    X_sort = np.sort(X)

    def get_elems_in_intervals(interval):
        M = []
        i, c = 0,0
        for el_int in interval:
            while X_sort[i] <= el_int:
                c+= 1
                if i < n-1: i+= 1
                else: break
            M.append(c)
            c = 0
        return M[1:]

    K = 6
    ch = chi2(df = K - 1)
    q_ch = ch.ppf(1-epsilon)

    intervals = np.linspace(0, 1, K+1)
    print("intervals: ", intervals)

    M = get_elems_in_intervals(intervals)
    print("M: ", M)
    psi_n = 0
    p = intervals[1]
    n_p = n*p
    for i in range(K):
        psi_n += ((M[i] - n_p)**2) / n_p

    print("psi_n = ", round(psi_n, 4))
    print(q_ch)
    if psi_n < q_ch:
        print("Выборка равномерная")
    else:
        print("Не равномерная")

    print("\n----------------------------------------------------------\n")

def task_3(a, sigma_sq, epsilon, data_1, data_2):
    print("\n--------------------<< ЗАДАНИЕ 3 >>-----------------------\n")

    print("a) Гипотеза о совпадении дисперий")

    X = np.array(data_1)
    n = X.size
    X__ = np.average(X)
    print("X__ = ", X__)

    S2_x = np.average(X * X) - X__*X__
    print("S^2_x = ", S2_x)

    S2_0_x = (n/(n-1)) * S2_x
    print("S^2_0_x = ", S2_0_x)
    Y = np.array(data_2)
    m = Y.size
    Y__ = np.average(Y)
    print("Y__ = ", Y__)

    S2_y = np.average(Y * Y) - Y__*Y__
    print("S^2_y = ", S2_y)

    S2_0_y = (m/(m-1)) * S2_y
    print("S^2_0_y = ", S2_0_y)

    fisher = f(dfn = n-1, dfd = m-1)
    st = nct(df = m+n-2, nc = 0)

    f1 = round(fisher.ppf(epsilon/2), 4)
    print("F(e/2): ", f1)
    f2 = round(fisher.ppf(1-epsilon/2), 4)
    print("F(1-e/2): ", f2)

    D_f = S2_0_x/S2_0_y
    print("D_f = ", D_f)

    if f1 <= D_f and D_f <= f2:
        print("Сигмы совпали")
    else:
        print("Сигмы не совпали")

    print("\nб) Гипотеза о совпадении средних")
    sq = sqrt((n*m)/(n+m))
    psi = ((n-1)*S2_0_x + (m-1)*S2_0_y) / (n+m-2)

    D_st = sq*(X__ - Y__)/sqrt(psi)

    q_stud = round(st.ppf(1-epsilon/2), 4)
    print("q_stud = ", q_stud)
    print("D_st = ", round(D_st, 4))

    if abs(D_st)<=q_stud:
        print("а совпали")
    else:
        print("а не совпали")

    print("\n----------------------------------------------------------\n")

def main():
    a, sigma_sq, epsilon, data_1, data_2, data_1_2, data_3 = load_data()
    task_1(a, sigma_sq, epsilon, data_1_2)
    task_2(epsilon, data_3)
    task_3(a, sigma_sq, epsilon, data_1, data_2)

if __name__ == "__main__":
    main()
