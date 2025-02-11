#include "benchmark.h"
#include "cxxopts.hpp"
#include "nsga2.h"
#include "utils.h"
#include <cstddef>

void fire(size_t individual_size, size_t population_size, size_t max_iters, size_t objective_size,
          uint32_t seed, std::string filename) {
    using benchmark::mlotz_functor;
    using end_criteria::Task6Logger;

    assert(objective_size % 2 == 0);
    assert(individual_size % (objective_size / 2) == 0);

    auto f = mlotz_functor(objective_size);

    individual::individual_t dummy_individual(individual_size, 0);
    assert(f(dummy_individual).size() == objective_size);

    auto criterion = end_criteria::Task6Logger(individual_size, population_size, objective_size,
                                               max_iters, filename, 2);

    auto experiment = nsga2::NSGA2(individual_size, objective_size, population_size, f, seed);

    nsga2::population_t pop = experiment.run(criterion);
}

int main(int argc, char **argv) {
    using namespace cxxopts;
    // clang-format off
    cxxopts::Options options(argv[0], "NSGA-II algorithm");
    options.add_options() 
      ("n,individual_size", "Size of the individual",value<size_t>())
      ("N,population_size", "Size of the population",value<size_t>())
      ("m,objective_size", "Size of the objective", value<size_t>())
      ("max_iters", "Maximum number of iterations", value<size_t>())
      ("seed", "Seed for the random number generator", value<uint32_t>())
      ("filename", "Name of the json file to save the log", value<std::string>())
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
    size_t max_iters = result["max_iters"].as<size_t>();
    uint32_t seed = result["seed"].as<uint32_t>();
    std::string filename = result["filename"].as<std::string>();

    fire(individual_size, population_size, max_iters, objective_size, seed, filename);

    std::cout << "Done!" << std::endl;
    return 0;
}