#include "benchmark.h"
#include <iostream>

void print() { std::cout << "Hello, World!" << std::endl; }

int main() {
    // test
    std::cout << "Hello, World!" << std::endl;
    std::cout << TEST_CONSTANT << std::endl;
    print();
    return 0;
}