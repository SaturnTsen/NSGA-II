#include <span>
#include <cassert>
#include "benchmark.h"

using namespace individual;
using namespace benchmark;
using namespace objective;

int main() {
    individual_t x {
        1, 1, 1,
        1, 1, 0,
        0, 0, 1,
        1, 0, 0,
    };

    val_t v = mlotz(8, x);
    std::vector<float> expected{3, 0, 2, 1, 0, 0, 1, 2};

    std::cout << "Individual:\t" << x << std::endl;
    std::cout << "8LOTZ output:\t" << v << std::endl;
    std::cout << "Expected:\t" << expected << std::endl;
    assert(v == expected);

    return 0;
}