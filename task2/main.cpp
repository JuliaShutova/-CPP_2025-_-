#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <random>
#include <cmath>
#include <algorithm>
#include <tuple>
#include <limits>
#include "students.h"
#include "teacher.h"
#include "utilities.h"
#include "quadratic_equation.h"
using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(nullptr));

    std::vector<std::string> studentNames = generateStudentNames(15);
    std::vector<Student*> students = createStudents(studentNames);

    cout << setw(25) << left << "Студент" << " | "
        << setw(10) << "Тип" << "\n";
    cout << string(40, '-') << endl;
    for (const auto& student : students) {
        cout << setw(25) << left << student->getName() << " |  " << student->getType() << endl;
    }

    Teacher teacher;
    const int TASKS_COUNT = 5;

    for (auto& student : students) {
        vector<QuadraticEquation> equations = generateValidEquations(student->getName(), TASKS_COUNT);

        cout << "\n---" << student->getName() << " (" << student->getType() << ") ---" << endl;
        for (size_t i = 0; i < equations.size(); ++i) {
            const auto& eq = equations[i];
            Solution sol = student->solveEquation(eq);
            teacher.addToMailQueue(eq, sol, student->getName(), student->getType());

            cout << "Задача " << (i + 1) << ": " << eq.toString() << endl;
            cout << "Решение: " << sol.toString() << endl << endl;
        }
    }

    teacher.checkAllMail();

    teacher.saveAllEquationsToFile("equations.txt");
    teacher.saveResultsToFile("results.txt", students);
    teacher.publishResults(students);

    for (auto student : students) {
        delete student;
    }

    return 0;
}