#ifndef QUADRATIC_EQUATION_H
#define QUADRATIC_EQUATION_H

#include <string>
#include <cmath>
#include <limits>

struct QuadraticEquation {
    double a, b, c;

    bool operator==(const QuadraticEquation& other) const;
    bool hasRealRoots() const;
    std::string toString() const;
};

struct Solution {
    double x1, x2;
    bool hasTwoRoots;

    std::string toString() const;
};

#endif
