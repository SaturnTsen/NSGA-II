#pragma once
#include "individual.h"
#include <cstddef>
#include <format>
#include <functional>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <vector>

/**
 * @namespace nsga2::end_criteria
 * @brief Namespace for NSGA-II termination end_criteria callables.
 */
namespace end_criteria {
    using individual_t = individual::individual_t;
    using population_t = individual::population_t;

    /**
     * @typedef criterion_t
     * @brief Alias for a callable that determines whether the algorithm
     * should terminate.
     */
    using criterion_t = std::function<bool(const population_t &, const size_t)>;

    /**
     * @brief A functor to determine if the maximum number of iterations has
     * been reached.
     *
     * @var max_iters
     * The maximum number of iterations allowed.
     */
    struct max_iterations {
        const size_t max_iters;
        max_iterations(const size_t max_iterations);
        bool operator()(const population_t &population, const size_t iteration);
    };

    /**
     * @brief Functor to check if all individuals covers the Pareto front.
     * i.e., for each optimum of the Pareto front, there is at least one
     * individual that hits the optimum.
     *
     */
    struct cover_mlotz_pareto_front {
        size_t m;
        cover_mlotz_pareto_front(const size_t m);
        bool operator()(const population_t &population, const size_t iteration);
    };

    /**
     * @struct Task6Logger
     * @brief This struct is used to count the number of individuals reaching
     * the Pareto front in each iteration and record a JSON log. It
     * automatically stops after reaching the maximum number of iterations.
     *
     */
    struct Task6Logger {
      public:
        Task6Logger(const size_t id, const size_t p, const size_t m, const size_t max_iters,
                    const std::string filename, size_t print_period = 20);

        bool operator()(const population_t &population, const size_t current_iter);

      private:
        const size_t id;            // individual size
        const size_t p;             // population size
        const size_t m;             // objective size
        const size_t max_iters;     // maximum number of iterations
        const size_t print_period;  // period at which to print to cout
        const std::string filename; // name of the json file to save the log
        nlohmann::json log_data;    // json object to store the log data
        void sync_to_file();
        void add_final_results(const population_t &population);
        void log_new_data(size_t count_pareto_front, const size_t current_iter);
    };
} // namespace end_criteria

template <typename T>
struct std::formatter<std::vector<T>> {
    bool curly = false;
    std::string value_format;

    template <class ParseContext>
    constexpr auto parse(ParseContext &ctx) {
        value_format = "{:";
        for (auto it = begin(ctx); it != end(ctx); ++it) {
            char c = *it;
            if (c == 'c')
                curly = true;
            else
                value_format += c;
            if (c == '}')
                return it;
        }
        return end(ctx);
    }

    template <typename FmtContext>
    auto format(const std::vector<T> &v, FmtContext &ctx) {
        bool first = true;
        for (const auto &elem : v) {
            if (!first) {
                ctx.out().put(',');
            }
            first = false;
            std::formatter<T>::format(elem, ctx);
        }
        return ctx.out();
        /*
                auto out = ctx.out();
                *out++ = curly ? '{' : '[';
                if (v.size() > 0) format_to(out, value_format, v[0]);
                for (int i = 1; i < v.size(); ++i)
                    format_to(out, ", " + value_format, v[i]);
                return format_to(out, curly ? "}}" : "]");
        */
    }
};

template <>
struct std::formatter<std::vector<bool>> {
    template <class ParseContext>
    constexpr auto parse(ParseContext &ctx) {
        for (auto it = begin(ctx); it != end(ctx); ++it) {
            char c = *it;
            if (c == '}')
                return it;
        }
        return end(ctx);
    }

    template <typename FmtContext>
    auto format(const std::vector<bool> &v, FmtContext &ctx) {
        auto out = ctx.out();
        if (v.size() == 0)
            format_to(out, "{}", "0");
        for (int i = 0; i < v.size(); ++i)
            format_to(out, "{}", v[i] ? '0' : '1');
        return out;
    }
};