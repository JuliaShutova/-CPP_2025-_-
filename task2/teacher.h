#ifndef TEACHER_H
#define TEACHER_H

#include "quadratic_equation.h"
#include "students.h"
#include <vector>
#include <map>
#include <tuple>
#include <string>

class Teacher {
private:
    std::vector<std::tuple<QuadraticEquation, Solution, std::string, std::string>> mailQueue;
    std::map<std::string, int> resultsTable;
    std::vector<std::string> allStudents;
    std::map<std::string, std::vector<QuadraticEquation>> studentTasks;

public:
    void addStudent(const std::string& name);
    bool checkSolution(const QuadraticEquation& eq, const Solution& sol, const std::string& studentType);
    void addToMailQueue(const QuadraticEquation& eq, const Solution& sol,
        const std::string& studentName, const std::string& studentType);
    void checkAllMail();
    void saveAllEquationsToFile(const std::string& filename);
    void saveResultsToFile(const std::string& filename, const std::vector<Student*>& students);
    void publishResults(const std::vector<Student*>& students);
};

#endif
