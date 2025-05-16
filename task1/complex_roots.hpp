#ifndef COMPLEX_HPP
#define COMPLEX_HPP

#include <iostream>
#include <cmath>
#include <complex>
#include <tuple>

// ����� ������ � ���������� ���������� 

std::tuple<bool, std::complex<double>, std::complex<double>> �omplex_roots(double a, double b, double c) {
    using namespace std::complex_literals;
    double D = b * b - 4 * a * c; // ������������
    std::complex<double> realPart = -b / (2 * a);
    if (b == 0) realPart = 0; // ���� 0 ��� ������ ���������
    std::complex<double> imagPart = std::sqrt(-D) / (2 * a);
    std::complex<double> root1 = realPart + imagPart * 1i; // ������ ������
    std::complex<double> root2 = realPart - imagPart * 1i; // ������ ������

    return std::make_tuple(true, root1, root2);
}
#endif 