#pragma once

#include <compare>
#include <functional>
#include <iostream>
#include <span>
#include <vector>

/* Individual values. */
namespace individual {
    // FIXME: use char instead of bool in order to avoid
    // std::vector<bool> specialization which breaks std::span.
    // This works thanks to automatic char -> bool coercion.
    // Unfortunately, that means invalid individuals can be constructed.
    // Can this be fixed by using another data structure?

    // Comment:
    // 1. I think this does not matter if we define non-zero values
    // as true and zero values as false. This way, we can still well define
    // the individuals. For the operators, we can tweak them using bitwise
    // operations to make sure that the values are either 0 or 1. But this
    // will make the operators less efficient.

    // 2. Another way is to override the constructor of the individual_t class
    // to make sure that the values are either 0 or 1 on construction.
    // And use std::move when constructing a new individual_t object from
    // std::vector<uint8_t> object. But this is not worth the cost since it
    // takes too much time.
    /**
     * @brief An individual, which represents a possible solution
     * to an optimization problem.
     *
     * An individual is a bit string, i.e a vector of booleans.
     */
    using individual_t = std::vector<uint8_t>;

    using population_t = std::vector<individual_t>;

    /* An `individual_t` span. */
    using span = std::span<const uint8_t>;

    /* Converts an individual to an integer in big-endian format. */
    size_t to_bits_be(const individual_t &x);

    /* Converts an individual to an integer in little-endian format. */
    size_t to_bits_le(const individual_t &x);

    /* Converts an individual to a string. */
    std::string to_string(const individual_t &x);

    individual_t operator&(const individual_t &a, const individual_t &b);

    individual_t operator|(const individual_t &a, const individual_t &b);

    individual_t operator^(const individual_t &a, const individual_t &b);

    individual_t operator~(const individual_t &a);

    std::ostream &operator<<(std::ostream &os, const individual_t &v);
} // namespace individual

/* Objective values. */
namespace objective {
    using individual::individual_t;

    /* An multi-objective value is a floating-point vector. */
    using val_t = std::vector<double>;

    /**
     * @brief An objective function that returns an objective value
     * from an individual.
     *
     * An objective function is used to Pareto-compare individuals in order
     * to determine optimal solutions to an optimization problem.
     *
     * This is a function pointer type, which can be created from either
     * a static function or a lambda.
     *
     * ## Example
     *
     * ```c++
     * objective::val_t (*f)(individual_t&) = +[](individual_t x) { ... };
     * ```
     */
    using fn_t = std::function<val_t(const individual_t &)>;

    std::ostream &operator<<(std::ostream &os, const val_t &v);
} // namespace objective

/* Utility classes and functions for Pareto comparison. */
namespace pareto {
    using individual::individual_t;
    using objective::val_t;

    using order = std::partial_ordering;

    order compare(const val_t &a, const val_t &b);

    /* Returns `true` if the left objective value strictly Pareto-dominates
        the right value. */
    bool strictly_dominates(const val_t &a, const val_t &b);

    /* Returns `true` if the left objective value Pareto-dominates
        the right value. */
    bool dominates(const val_t &a, const val_t &b);
} // namespace pareto

namespace individual {
    /* Pareto-compare two individuals with respect to an objective function. */
    pareto::order compare(const individual_t &a, const individual_t &b, const objective::fn_t &f);

    /* Returns `true` if the left individual strictly Pareto-dominates
       the right individual with respect to an objective function. */
    bool strictly_dominates(const individual_t &a, const individual_t &b, const objective::fn_t &f);

    /* Returns `true` if the left individual Pareto-dominates
       the right individual with respect to an objective function. */
    bool dominates(const individual_t &a, const individual_t &b, const objective::fn_t &f);
} // namespace individual