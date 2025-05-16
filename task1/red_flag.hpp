#ifndef RED_HPP
#define RED_HPP

#include <iostream>
#include <string.h>

bool red_flag(double a, double b, double c) { // проверка на параболу

    // случай 1: оба коэффицента равны 0
    if ((a == 0) * (b == 0)) {
        std::cout << "Уравнение не имеет смысла." << std::endl;
        return (false);
    }

    // случай 2: a = 0
    if ((a == 0) * (b != 0)) {
        std::cout << "Уравнение задает прямую (а не параболу) и имеет только один корень." << std::endl;
        std::cout << "x = " << -c / b << std::endl;
        return (false);
    }

    else { // парабола
        return (true);
    }

}
#endif 