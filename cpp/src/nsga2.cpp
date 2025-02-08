#include "nsga2.h"
#include "graph.h"
#include "individual.h"
#include "utils.h"
#include <algorithm>
#include <cstddef>
#include <random>

const double eps = 1e-8;

using end_criteria::criterion_t; // callable terminating condition

namespace nsga2 {

    // clang-format off
    NSGA2::NSGA2(const size_t individual_size, 
                 const size_t objective_size,
                 const size_t population_size,
                 objective::fn_t &f,
                 const double mutation_rate,
                 uint32_t seed) :
        individual_size(individual_size),
        objective_size(objective_size),
        population_size(population_size),
        mutation_rate(mutation_rate),
        dist(mutation_rate),
        gen(seed),
        f(f) {
            individual_t dummy_individual(individual_size);
            assert(f(dummy_individual).size() == objective_size);
        }

    NSGA2::NSGA2(const size_t individual_size,
                 const size_t objective_size,
                 const size_t population_size,
                 objective::fn_t &f,
                 uint32_t seed) :
        NSGA2(individual_size, objective_size, population_size,
              f, 1.0 / (double)individual_size, seed) {}

    // clang-format on

    void NSGA2::mutate(population_t &population) {
        // TODO Sanity check
        population.resize(population_size * 2);
        for (int i = population_size; i < population_size * 2; i++) {
            population[i] = population[i - population_size];
            for (int j = 0; j < individual_size; j++) {
                if (bit_gen()) {
                    population[i][j] = !population[i][j];
                }
            }
        }
    }

    fronts_t NSGA2::non_dominate_sort(population_t &population) {
        // TODO This function is not tested and may contain bugs.
        // TODO Performance improvements

        Graph<index_t> graph;
        size_t size = population.size();

        // O(N) N = population size
        for (index_t i = 0; i < size; i++)
            graph.addNode(i);

        // O(N^2)
        // Is there a clever way to do this? e.g. dynamic pruning?
        for (index_t i = 0; i < size; i++) {
            for (index_t j = 0; j < size; j++) {
                individual_t &individual = population[i];
                individual_t &other = population[j];
                if (individual::strictly_dominates(individual, other, f)) {
                    graph.addEdge(i, j);
                }
            }
        }

        // O(N^2)
        return graph.popAndGetFronts();
    }

    /**
     * @brief Calculate the crowding distance for each individual in the front.
     *
     * @param population The total population.
     * @param front the list of indices of the individuals in the front.
     * @return scores_t
     */

    scores_t NSGA2::crowding_distance(const population_t &population,
                                      front_t &indices) {

        // TODO This function is not tested and may contain bugs.
        // TODO Performance improvements

        size_t size = indices.size();
        scores_t distances;
        assert(size > 0);

        // initialize the distances
        for (index_t idx : indices) {
            distances[idx] = 0.0;
        }

        for (size_t m = 0; m < objective_size; m++) {

            // sort the front based on the objective O(NlogN)
            std::sort(indices.begin(), indices.end(),
                      [&](index_t a, index_t b) {
                          return population[a][m] < population[b][m];
                      });

            // set the boundary points to infinity
            distances[indices[0]] = std::numeric_limits<double>::infinity();
            distances[indices[size - 1]] =
                std::numeric_limits<double>::infinity();

            // Added eps to avoid division by zero
            double d = population[indices[size - 1]][m] -
                       population[indices[0]][m] + eps;

            // O(N), recall that `distances` is a hash map
            for (size_t j = 1; j < size - 1; j++) {
                distances[indices[j]] += (population[indices[j + 1]][m] -
                                          population[indices[j - 1]][m]) /
                                         d;
            }
        }

        return distances;
    }

    /**
     * @brief returns the next generation of individuals based on the fronts.
     *
     * @param population
     * @param fronts
     * @return population_t
     */
    population_t NSGA2::crowding_distance_select(population_t &population,
                                                 fronts_t &fronts) {
        // TODO Test
        population_t new_population;
        size_t target_size = population_size;
        size_t front_idx = 0;

        // select low ranked fronts until the target size is reached
        for (front_idx = 0; front_idx < fronts.size(); front_idx++) {
            front_t &front = fronts[front_idx];
            if (new_population.size() + front.size() > target_size)
                break;
            for (index_t idx : front)
                new_population.push_back(std::move(population[idx]));
        }

        if (new_population.size() == target_size)
            return new_population;

        // crowding distance selection
        front_t &front = fronts[front_idx];
        scores_t scores = crowding_distance(population, front);
        size_t remaining = target_size - new_population.size();

        // Cast: O(N)
        std::vector<std::pair<index_t, double>> vec(scores.begin(),
                                                    scores.end());

        // O(NlogN)
        std::partial_sort(vec.begin(), vec.begin() + remaining, vec.end(),
                          [](const std::pair<size_t, double> &a,
                             const std::pair<size_t, double> &b) {
                              return a.second > b.second;
                          });

        // O(N) in the worst case
        for (size_t i = 0; i < remaining; i++) {
            new_population.push_back(std::move(population[vec[i].first]));
        }

        if (new_population.size() != target_size) {
            throw std::runtime_error("crowding_distance_select: "
                                     "new_population.size() != target_size. "
                                     "check if there is a bug.");
        }
        return new_population;
    }

    /**
     * @brief init the population uniformly.
     *
     * @param individual_size
     * @param population_size
     */
    void NSGA2::init_population(size_t individual_size,
                                size_t population_size) { // Tested
        std::cout << "initializing population" << std::endl;
        std::bernoulli_distribution distribution(0.5);
        population.resize(population_size);

        int count_1 = 0;
        for (auto &individual : population) {
            individual.resize(individual_size);
            for (auto &gene : individual) {
                gene = distribution(this->gen);
                count_1 += gene;
            }
        }

        std::cout << "proportion of successful mutations(~0.5): "
                  << (double)count_1 / (individual_size * population_size)
                  << std::endl;
    }

    population_t NSGA2::run(criterion_t criterion) {
        std::cout << "running NSGA2" << std::endl;
        std::cout << "mutation rate: " << mutation_rate << std::endl;
        std::cout << "individual size: " << individual_size << std::endl;
        std::cout << "population size: " << population_size << std::endl;
        std::cout << "objective size: " << objective_size << std::endl;
        std::cout << "-----------------------------------" << std::endl;

        init_population(individual_size, population_size);
        size_t iter = 0;
        fronts_t fronts;
        while (!criterion(population, iter)) {
            mutate(population);
            fronts = std::move(non_dominate_sort(population));

            population =
                std::move(crowding_distance_select(population, fronts));
            iter++;

            // std::cout << std::format("Actual mutation ratio {} during iter
            // {}",
            //                          mutation_ratio(), iter)
            //           << std::endl;
        }
        return population;
    }

    uint8_t NSGA2::bit_gen() {
        if (dist(gen)) {
            successful_mutations++;
            mutation_attempts++;
            return 1;
        } else {
            mutation_attempts++;
            return 0;
        }
    }

    double NSGA2::mutation_ratio() {
        return (double)successful_mutations / (double)mutation_attempts;
    }
} // namespace nsga2