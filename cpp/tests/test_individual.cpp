#include "individual.h"
#include "utils.h"
#include <cassert>
#include <iostream>

using individual::individual_t;
using objective::val_t;

val_t f(individual_t &x) {
    val_t v;
    for (bool b : x) {
        v.push_back((float)b);
    }
    return v;
}

int main() {
    individual_t x = {1, 0, 1};
    individual_t y = {1, 1, 1};
    individual_t z = {1, 1, 0};
    val_t vx = f(x);
    val_t vy = f(y);
    val_t vz = f(z);

    {
        using namespace individual;
        using namespace objective;
        std::cout << x << "\t" << y << "\t" << z << std::endl;
        std::cout << vx << "\t" << vy << "\t" << vz << std::endl;
    }

    assert(individual::to_bits_be(z) == 0b110);
    assert(individual::to_bits_le(z) == 0b011);

    assert(pareto::compare(vx, vx) == 0);
    assert(pareto::compare(vy, vx) > 0);
    assert(pareto::compare(vx, vy) < 0);

    assert(pareto::dominates(vx, vx));
    assert(!pareto::strictly_dominates(vx, vx));
    assert(pareto::strictly_dominates(vy, vx));
    assert(!pareto::dominates(vx, vy));

    assert(pareto::compare(vx, vz) == pareto::order::unordered);

    assert(individual::compare(x, x, *f) == 0);
    assert(individual::compare(y, x, *f) > 0);
    assert(individual::compare(x, y, *f) < 0);

    assert(individual::dominates(x, x, *f));
    assert(!individual::strictly_dominates(x, x, *f));
    assert(individual::strictly_dominates(y, x, *f));
    assert(!individual::dominates(x, y, *f));

    assert(individual::compare(x, z, *f) == pareto::order::unordered);

    std::cout << "Success" << std::endl;

    return 0;
}