#ifndef REAL_HPP
#define RAEL_HPP

#include <iostream>
#include <cmath>
#include <tuple>
#include "complex_roots.hpp"

// поиск Д, поиск корней (+булиновской функции для определения корней в плоскости R or C)

std::tuple<bool, double, double> roots(double a, double b, double c) {
    double D = b * b - 4 * a * c; // дискриминант

    if (D < 0) { // рассмотрим как частный случай в "complex_roots.hpp"
        return std::make_tuple(false, 0, 0);
    }

    else if (D == 0) {
        double x = -b / (2 * a); // единственный корень
        if (b == 0) x = 0; // чтоб 0 без минуса выводился
        return std::make_tuple(true, x, x);
    }

    else { // D>0
        double x1 = (-b + std::sqrt(D)) / (2 * a); // первый корень
        double x2 = (-b - std::sqrt(D)) / (2 * a); // второй корень
        return std::make_tuple(true, x1, x2); 
    }
}
#endif 