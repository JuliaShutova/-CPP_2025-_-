#ifndef RED_HPP
#define RED_HPP

#include <iostream>
#include <string.h>

bool red_flag(double a, double b, double c) { // �������� �� ��������

    // ������ 1: ��� ����������� ����� 0
    if ((a == 0) * (b == 0)) {
        std::cout << "��������� �� ����� ������." << std::endl;
        return (false);
    }

    // ������ 2: a = 0
    if ((a == 0) * (b != 0)) {
        std::cout << "��������� ������ ������ (� �� ��������) � ����� ������ ���� ������." << std::endl;
        std::cout << "x = " << -c / b << std::endl;
        return (false);
    }

    else { // ��������
        return (true);
    }

}
#endif 