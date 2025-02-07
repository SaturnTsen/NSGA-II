#include "individual.h"

#ifndef NSGAII_BENCHMARK_H

#define NSGAII_BENCHMARK_H

static int TEST_CONSTANT = 1;

namespace benchmark {
    using individual::individual_t;
    int lotzk(int k, individual::span x);

    /**
     * @brief The LeadingOnesTrailingZeros (LOTZ) function, a simple
     * bi-objective function.
     *
     * The first objective value counts the number of leading ones
     * and the second objective value counts the number of trailing zeros
     * in the binary representation of the individual.
     */
    // fn_t lotz = [](individual_t &x) -> objective::val_t;
    objective::val_t lotz(individual_t &x);

    /**
     * @brief A single objective of the mLOTZ multi-objective function.
     *
     * This returns the `k`-th coordinate of the mLOTZ value,
     * where `0 <= k < m`.
     */
    int mlotzk(int m, int k, individual::span x);

    /**
     * @brief The mLOTZ multi-objective function.
     *
     * This is a generalized version of the bi-objective LOTZ.
     */
    objective::val_t mlotz(int m, individual_t &x);
} // namespace benchmark

#endif