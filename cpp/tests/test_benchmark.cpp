#include "benchmark.h"
#include "individual.h"
#include <cassert>

using namespace individual;
using namespace benchmark;
using namespace objective;

void test_mlotz() {
    individual_t x{
        1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0,
    };

    val_t v = mlotz(8, x);
    std::vector<double> expected{3, 0, 2, 1, 0, 0, 1, 2};

    std::cout << "Individual:\t" << x << std::endl;
    std::cout << "8LOTZ output:\t" << v << std::endl;
    std::cout << "Expected:\t" << expected << std::endl;
    assert(v == expected);

    individual_t y{1, 0, 1, 1, 1, 0};

    val_t w = mlotz(6, y);
    val_t expected_w{1, 1, 2, 0, 1, 1};
    assert(w == expected_w);

    w = mlotz(4, y);
    expected_w = {1, 0, 2, 1};
    assert(w == expected_w);

    return;
}

void test_pareto_front() {
    individual_t y{1, 1, 1, 0, 0, 0};
    assert(is_lotz_pareto_front(y));

    individual_t z{1, 1, 1, 0, 0, 1};
    assert(!is_lotz_pareto_front(z));

    individual_t w{1, 1, 1, 0, 0, 1, 1, 1};
    assert(!is_mlotz_pareto_front(4, w));

    individual_t p{1, 0, 1, 1, 1, 0};
    assert(is_mlotz_pareto_front(6, p));
    assert(!is_mlotz_pareto_front(4, p));

    //    000100
    individual_t q{0, 0, 0, 1, 0, 0};
    assert(is_mlotz_pareto_front(4, q));
    return;
}

int main() {
    test_mlotz();
    test_pareto_front();
    return 0;
}