//
// Created by Koko on 22.05.2020.
//

#include "Test.h"

int Variable = 2;

Test::Test(int n) : m_n(n) {}

Test::~Test() {
    std::cout << "Hello from destructor!" << std::endl;
}

void Test::print() const {
    std::cout << "Hello form static library!" << std::endl;
}