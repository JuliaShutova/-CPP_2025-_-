#include "students.h"
#include <cmath>
#include <limits>

Student::Student(const std::string& _name) : name(_name), gen(std::random_device{}()) {}

std::string Student::getName() const { return name; }

GoodStudent::GoodStudent(const std::string& _name) : Student(_name) {}

Solution GoodStudent::solveEquation(const QuadraticEquation& eq) {
    double discriminant = eq.b * eq.b - 4 * eq.a * eq.c;

    if (discriminant > 0) {
        double x1 = (-eq.b + sqrt(discriminant)) / (2 * eq.a);
        double x2 = (-eq.b - sqrt(discriminant)) / (2 * eq.a);
        return { x1, x2, true };
    }
    else if (discriminant == 0) {
        double x = -eq.b / (2 * eq.a);
        return { x, 0, false };
    }
    else {
        return { std::numeric_limits<double>::quiet_NaN(),
                std::numeric_limits<double>::quiet_NaN(),
                false };
    }
}

std::string GoodStudent::getType() const { return "Хороший"; }

AverageStudent::AverageStudent(const std::string& _name) :
    Student(_name), dist(0.0, 1.0) {}

Solution AverageStudent::solveEquation(const QuadraticEquation& eq) {
    if (dist(gen) < 0.4) {
        double discriminant = eq.b * eq.b - 4 * eq.a * eq.c;
        if (discriminant > 0) {
            double x1 = (-eq.b + sqrt(discriminant)) / (2 * eq.a);
            double x2 = (-eq.b - sqrt(discriminant)) / (2 * eq.a);
            return { x1, x2, true };
        }
        else if (discriminant == 0) {
            double x = -eq.b / (2 * eq.a);
            return { x, 0, false };
        }
    }

    std::uniform_real_distribution<double> wrong(-10.0, 10.0);
    return { wrong(gen), wrong(gen), dist(gen) > 0.5 };
}

std::string AverageStudent::getType() const { return "Средний"; }

BadStudent::BadStudent(const std::string& _name) :
    Student(_name), dist(-100.0, 100.0) {}

Solution BadStudent::solveEquation(const QuadraticEquation& eq) {
    return { dist(gen), dist(gen), dist(gen) > 0.5 };
}

std::string BadStudent::getType() const { return "Плохой"; }