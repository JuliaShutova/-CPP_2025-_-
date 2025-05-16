#ifndef STUDENTS_H
#define STUDENTS_H

#include "quadratic_equation.h"
#include <string>
#include <random>

class Student {
protected:
    std::string name;
    std::mt19937 gen;
public:
    Student(const std::string& _name);
    virtual Solution solveEquation(const QuadraticEquation& eq) = 0;
    std::string getName() const;
    virtual std::string getType() const = 0;
    virtual ~Student() {}
};

class GoodStudent : public Student {
public:
    GoodStudent(const std::string& _name);
    Solution solveEquation(const QuadraticEquation& eq) override;
    std::string getType() const override;
};

class AverageStudent : public Student {
    std::uniform_real_distribution<double> dist;
public:
    AverageStudent(const std::string& _name);
    Solution solveEquation(const QuadraticEquation& eq) override;
    std::string getType() const override;
};

class BadStudent : public Student {
    std::uniform_real_distribution<double> dist;
public:
    BadStudent(const std::string& _name);
    Solution solveEquation(const QuadraticEquation& eq) override;
    std::string getType() const override;
};

#endif
