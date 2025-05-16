#ifndef REAL_HPP
#define RAEL_HPP

#include <iostream>
#include <cmath>
#include <tuple>
#include "complex_roots.hpp"

// ����� �, ����� ������ (+����������� ������� ��� ����������� ������ � ��������� R or C)

std::tuple<bool, double, double> roots(double a, double b, double c) {
    double D = b * b - 4 * a * c; // ������������

    if (D < 0) { // ���������� ��� ������� ������ � "complex_roots.hpp"
        return std::make_tuple(false, 0, 0);
    }

    else if (D == 0) {
        double x = -b / (2 * a); // ������������ ������
        if (b == 0) x = 0; // ���� 0 ��� ������ ���������
        return std::make_tuple(true, x, x);
    }

    else { // D>0
        double x1 = (-b + std::sqrt(D)) / (2 * a); // ������ ������
        double x2 = (-b - std::sqrt(D)) / (2 * a); // ������ ������
        return std::make_tuple(true, x1, x2); 
    }
}
#endif 