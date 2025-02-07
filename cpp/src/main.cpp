#include "benchmark.h"
#include "individual.h"
#include <iostream>
void print() { std::cout << "Hello, World!" << std::endl; }

using individual::individual_t;

int main() {
    // test
    individual_t x = {1, 0, 1, 0, 1, 0, 1, 0};
    std::cout << x[0] << std::endl;
    return 0;
}