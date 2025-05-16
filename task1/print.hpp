#ifndef PRINT_HPP
#define PRINT_HPP

#include <iostream>
#include <complex>
#include <string.h>
#include "red_flag.hpp"
#include "real_roots.hpp"
#include "complex_roots.hpp"

// вывод ответа
void print(double a, double b, double c) {

    bool parabola = red_flag(a, b, c); // проверка на параболу

    if (parabola) {

        bool hasReal;
        double root1, root2;
        std::tie(hasReal, root1, root2) = roots(a, b, c);

        if (hasReal) { // вывод вещественных корней
            std::cout << "Корни уравнения: x1 = " << root1 << ", x2 = " << root2 << std::endl;
        }
        else { // вывод комплесных корней
            std::complex<double> r1, r2;
            std::tie(hasReal, r1, r2) = сomplex_roots(a, b, c);

            std::cout << " x1 = " << r1.real() << " +" << r1.imag() << "i" << std::endl;
            std::cout << " x2 = " << r2.real() << " " << r2.imag() << "i" << std::endl;
        }
    }
}
#endif 