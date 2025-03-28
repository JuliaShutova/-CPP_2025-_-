#include "utilities.h"
#include <random>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

vector<string> generateStudentNames(int count) {
    vector<string> firstNames = { "Лев", "Рюрик", "Владимир", "Юрий", "Ярослав",
                                "Володар", "Мстислав", "Глеб", "Борис", "Ярополк",
                                "Игорь", "Святослав", "Елисей", "Василий", "Сергей" };

    vector<string> lastNames = { "Белый", "Иванов", "Грузин", "Гордеев", "Кузнецов",
                               "Васильев", "Попов", "Новиков", "Фёдоров", "Морозов",
                               "Есенин", "Федотов", "Абрамов", "Куликов", "Кузьмов" };

    vector<string> names;
    mt19937 gen(random_device{}());
    uniform_int_distribution<int> firstDist(0, firstNames.size() - 1);
    uniform_int_distribution<int> lastDist(0, lastNames.size() - 1);

    for (int i = 0; i < count; ++i) {
        string name = firstNames[firstDist(gen)] + " " + lastNames[lastDist(gen)];
        names.push_back(name);
    }

    return names;
}

vector<Student*> createStudents(const vector<string>& names) {
    vector<Student*> students;
    mt19937 gen(random_device{}());
    uniform_int_distribution<int> typeDist(0, 2);

    for (const auto& name : names) {
        switch (typeDist(gen)) {
        case 0: students.push_back(new GoodStudent(name)); break;
        case 1: students.push_back(new AverageStudent(name)); break;
        case 2: students.push_back(new BadStudent(name)); break;
        }
    }
    return students;
}

vector<QuadraticEquation> generateValidEquations(const string& studentName, int count) {
    vector<QuadraticEquation> equations;
    mt19937 gen(hash<string>{}(studentName));
    uniform_real_distribution<double> coeffDist(-5.0, 5.0);

    while (equations.size() < count) {
        double a = round(coeffDist(gen) * 2) / 2;
        while (a == 0) a = round(coeffDist(gen) * 2) / 2;

        double b = round(coeffDist(gen) * 2) / 2;
        double c = round(coeffDist(gen) * 2) / 2;

        QuadraticEquation eq{ a, b, c };
        if (eq.hasRealRoots() || equations.size() >= count - 1) {
            equations.push_back(eq);
        }
    }
    return equations;
}