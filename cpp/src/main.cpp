#include "benchmark.h"
#include "cxxopts.hpp"
#include "nsga2.h"

using benchmark::mlotz_functor;
using end_criteria::criterion_t;

int main(int argc, char **argv) {

    // clang-format off
    cxxopts::Options options(argv[0], "NSGA-II algorithm");
    options.add_options() 
      ("i,individual_size", "Size of the individual", cxxopts::value<size_t>())
      ("p,population_size", "Size of the population", cxxopts::value<size_t>())
      ("o,objective_size", "Size of the objective", cxxopts::value<size_t>())
      ("h,help", "Print usage");
    // clang-format on

    auto result = options.parse(argc, argv);

    if (result.arguments().empty() || result.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    size_t individual_size = result["individual_size"].as<size_t>();
    size_t population_size = result["population_size"].as<size_t>();
    size_t objective_size = result["objective_size"].as<size_t>();

    auto f = (nsga2::fn_t)mlotz_functor(objective_size);

    auto experiment =
        nsga2::NSGA2(individual_size, objective_size, population_size, f);

    criterion_t criterion =
        end_criteria::hit_mlotz_pareto_front(objective_size);

    experiment.run(criterion);

    return 0;
}