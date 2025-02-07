#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>

// ввод параметров
void input(double &a, double &b, double &c) {

    std::cout << "¬ведите коэффициенты <a b c> (ax ^ 2 + bx + c = 0) : ";
    std::cin >> a >> b >> c;
    
}
#endif 