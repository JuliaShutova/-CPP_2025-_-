#include "quadratic_equation.h"
#include <sstream>
#include <iomanip>

bool QuadraticEquation::operator==(const QuadraticEquation& other) const {
    const double eps = 1e-6;
    return abs(a - other.a) < eps &&
        abs(b - other.b) < eps &&
        abs(c - other.c) < eps;
}

bool QuadraticEquation::hasRealRoots() const {
    return (b * b - 4 * a * c) >= 0;
}

std::string QuadraticEquation::toString() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << a << "x^2 + " << b << "x + " << c << " = 0";
    return oss.str();
}

std::string Solution::toString() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    if (hasTwoRoots) {
        oss << "x1 = " << x1 << ", x2 = " << x2;
    }
    else {
        oss << "x = " << x1;
    }
    return oss.str();
}