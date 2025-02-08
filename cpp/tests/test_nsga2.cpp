#include "benchmark.h"
#include "nsga2.h"
#include <cstdint>

using benchmark::mlotz_functor;
using nsga2::population_t;

int main() {

    size_t individual_size = 12;
    size_t population_size = 36;
    size_t objective_size = 4;
    size_t max_iters = 10;
    uint8_t seed = 0;

    auto f = (nsga2::fn_t)mlotz_functor(objective_size);

    auto criterion = end_criteria::hit_mlotz_pareto_front(objective_size);

    auto experiment =
        nsga2::NSGA2(individual_size, objective_size, population_size, f, 1);

    population_t pop = experiment.run(criterion);

    return 0;
}