#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <string>
#include "students.h"

std::vector<std::string> generateStudentNames(int count);
std::vector<Student*> createStudents(const std::vector<std::string>& names);
std::vector<QuadraticEquation> generateValidEquations(const std::string& studentName, int count);

#endif