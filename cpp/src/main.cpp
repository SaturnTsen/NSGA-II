#include<iostream>
#include "benchmark.h"

void print() {
    std::cout << "Hello, World!" << std::endl;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    std::cout << TEST_CONSTANT << std::endl;
    print();
    return 0;
}