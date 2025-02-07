#include <iostream>
#include "print.hpp"
#include "input.hpp"

int main() {

    setlocale(LC_ALL, "Rus");
    double a, b, c;
    input(a, b, c);
    print(a, b, c);

    return 0;
}