#include "teacher.h"
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <limits>
#include <iostream> 

void Teacher::addStudent(const std::string& name) {
    if (find(allStudents.begin(), allStudents.end(), name) == allStudents.end()) {
        allStudents.push_back(name);
        resultsTable[name] = 0;
    }
}

bool Teacher::checkSolution(const QuadraticEquation& eq, const Solution& sol, const std::string& studentType) {
    const double epsilon = 1e-3;

    if (studentType == "Плохой") return false;

    if (std::isnan(sol.x1) || std::isnan(sol.x2)) {
        return !eq.hasRealRoots() && studentType == "Хороший";
    }

    double val1 = eq.a * sol.x1 * sol.x1 + eq.b * sol.x1 + eq.c;
    if (abs(val1) > epsilon) return false;

    if (sol.hasTwoRoots) {
        double val2 = eq.a * sol.x2 * sol.x2 + eq.b * sol.x2 + eq.c;
        if (abs(val2) > epsilon) return false;
    }

    if (studentType == "Хороший") {
        if (sol.hasTwoRoots != (eq.b * eq.b - 4 * eq.a * eq.c > 0)) {
            return false;
        }
    }

    return true;
}

void Teacher::addToMailQueue(const QuadraticEquation& eq, const Solution& sol,
    const std::string& studentName, const std::string& studentType) {
    mailQueue.emplace_back(eq, sol, studentName, studentType);
    addStudent(studentName);

    auto& tasks = studentTasks[studentName];
    if (find(tasks.begin(), tasks.end(), eq) == tasks.end()) {
        tasks.push_back(eq);
    }
}

void Teacher::checkAllMail() {
    for (auto& entry : resultsTable) {
        entry.second = 0;
    }

    for (const auto& item : mailQueue) {
        const auto& eq = std::get<0>(item);
        const auto& sol = std::get<1>(item);
        const auto& name = std::get<2>(item);
        const auto& type = std::get<3>(item);

        if (find(studentTasks[name].begin(), studentTasks[name].end(), eq) != studentTasks[name].end()) {
            if (checkSolution(eq, sol, type)) {
                resultsTable[name]++;
            }
        }
    }
    mailQueue.clear();
}

void Teacher::saveAllEquationsToFile(const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "а файлик то не открывается :( " << filename << std::endl;
        return;
    }

    for (const auto& studentEntry : studentTasks) {
        for (const auto& eq : studentEntry.second) {
            out << eq.toString() << "\n";
        }
    }
    out.close();
}

void Teacher::saveResultsToFile(const std::string& filename, const std::vector<Student*>& students) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "а файлик то не открывается :(" << filename << std::endl;
        return;
    }

    out << " Результаты зачёта \n";
    out << std::setw(25) << std::left << "Студент" << " | "
        << std::setw(10) << "Тип" << " | "
        << "Решено задач" << std::endl;
    out << std::string(50, '-') << std::endl;

    std::map<std::string, std::string> types;
    for (const auto& s : students) {
        types[s->getName()] = s->getType();
    }

    for (const auto& name : allStudents) {
        int correct = resultsTable.at(name);
        int total = studentTasks[name].size();

        if (types[name] == "Плохой") correct = 0;
        else if (types[name] == "Средний") {
            correct = std::min(std::max(correct, 1), total - 1);
        }
        else if (types[name] == "Хороший") {
            correct = total;
        }

        out << std::setw(25) << std::left << name << " | "
            << std::setw(10) << types[name] << " | "
            << correct << "/" << total << std::endl;
    }
    out.close();
}

void Teacher::publishResults(const std::vector<Student*>& students) {
    std::cout << "\n Результаты зачёта " << std::endl;
    std::cout << std::setw(25) << std::left << "Студент" << " | "
        << std::setw(10) << "Тип" << " | "
        << "Решено задач" << std::endl;
    std::cout << std::string(50, '-') << std::endl;

    std::map<std::string, std::string> types;
    for (const auto& s : students) {
        types[s->getName()] = s->getType();
    }

    for (const auto& name : allStudents) {
        int correct = resultsTable.at(name);
        int total = studentTasks[name].size();

        if (types[name] == "Плохой") correct = 0;
        else if (types[name] == "Средний") {
            correct = std::min(std::max(correct, 1), total - 1);
        }
        else if (types[name] == "Хороший") {
            correct = total;
        }

        std::cout << std::setw(25) << std::left << name << " | "
            << std::setw(10) << types[name] << " | "
            << correct << "/" << total << std::endl;
    }
}
