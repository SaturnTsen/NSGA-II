#pragma once

#include "individual.h"
#include <cstddef>

namespace benchmark {
    using individual::individual_t;
    int lotzk(const int k, const individual::span &x);

    /**
     * @brief The LeadingOnesTrailingZeros (LOTZ) function, a simple
     * bi-objective function.
     *
     * The first objective value counts the number of leading ones
     * and the second objective value counts the number of trailing zeros
     * in the binary representation of the individual.
     */
    // fn_t lotz = [](individual_t &x) -> objective::val_t;
    objective::val_t lotz(const individual::span &x);

    /**
     * @brief A single objective of the mLOTZ multi-objective function.
     *
     * This returns the `k`-th coordinate of the mLOTZ value,
     * where `0 <= k < m`.
     */
    int mlotzk(const int m, const int k, const individual::span &x);

    /**
     * @brief The mLOTZ multi-objective function.
     *
     * This is a generalized version of the bi-objective LOTZ.
     */
    objective::val_t mlotz(const int m, const individual_t &x);

    /**
     * @brief The mLOTZ functor
     *
     */
    struct mlotz_functor {
        const size_t m;
        mlotz_functor(const size_t m);
        objective::val_t operator()(const individual_t &x);
    };

    /**
     * @brief Check if an individual is on the Pareto front of the LOTZ
     * function.
     *
     * This function checks if the number of leading ones and trailing zeros
     * sum to the length of the individual.
     */
    bool is_lotz_pareto_front(const individual_t &x);

    /**
     * @brief Check if an individual is on the Pareto front of the mLOTZ
     * function.
     *
     * This function checks if each slice of the individual is on the LOTZ
     * Pareto front.
     */
    bool is_mlotz_pareto_front(const int m, const individual_t &x);

} // namespace benchmark
