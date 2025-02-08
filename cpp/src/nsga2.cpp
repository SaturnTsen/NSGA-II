#include "nsga2.h"
#include "benchmark.h"
#include "graph.h"
#include <algorithm>
#include <cstddef>
#include <numeric>
#include <random>

using end_criteria::criterion_t;     // callable terminating condition
using index_t = std::size_t;         // index of an individual in a population
using rank_t = std::size_t;          // rank of an individual in a population
using ranks_t = std::vector<rank_t>; // ranks of all individuals in a population
using scores_t = std::vector<double>; // crowding distances of all individuals
                                      // in a certain population subset
namespace end_criteria {

    max_iterations::max_iterations(size_t max_iterations)
        : max_iters(max_iterations) {}

    bool max_iterations::operator()(nsga2::population_t &population,
                                    const size_t iteration) {
        return iteration >= max_iters;
    }

    hit_mlotz_pareto_front::hit_mlotz_pareto_front(size_t m) : m(m) {}

    bool hit_mlotz_pareto_front::operator()(nsga2::population_t &population,
                                            const size_t iteration) {
        for (nsga2::individual_t &individual : population) {
            if (!benchmark::is_mlotz_pareto_front(m, individual))
                return false;
        }
        return true;
    }
} // namespace end_criteria

namespace nsga2 {

    NSGA2::NSGA2(const size_t individual_size, const size_t objective_size,
                 const size_t population_size, objective::fn_t &f,
                 const double mutation_rate)
        : f(f), mutation_rate(mutation_rate) {

        (this->bit_gen) = std::mt19937(std::random_device()());

        (this->population).resize(population_size);

        for (auto &individual : population) {
            individual.resize(individual_size);
            for (auto &gene : individual) {
                gene = bit_gen() % 2;
            }
        }
    }

    NSGA2::NSGA2(const size_t individual_size, const size_t objective_size,
                 const size_t population_size, objective::fn_t &f)
        : f(f), mutation_rate(1 / (double)population_size) {
        NSGA2(individual_size, objective_size, population_size, f,
              mutation_rate);
    }

    void NSGA2::mutate(population_t &population) {
        double mutation_rate = 1.0 / population.size();
        size_t size = population.size();
        population.resize(size * 2);
        for (auto &individual : population) {
            for (index_t i = size; i < size * 2; i++) {
                if (rand() < mutation_rate) {
                    individual[i] = ~individual[i];
                }
            }
        }
    }

    ranks_t NSGA2::non_dominate_sort(population_t &population) {
        Graph<index_t> graph;
        size_t size = population.size();

        for (index_t i = 0; i < size; i++)
            graph.addNode(i);

        for (index_t i = 0; i < size; i++) {
            for (index_t j = 0; j < size; j++) {
                individual_t &individual = population[i];
                individual_t &other = population[j];
                if (individual::strictly_dominates(individual, other,
                                                   this->f)) {
                    graph.addEdge(i, j);
                }
            }
        }
        // while there are nodes with in-degree 0
        // remove them and add them to the current rank
        ranks_t ranks(size);
        size_t current_rank = 0;
        while (graph.size() > 0) {
            for (index_t i = 0; i < size; i++) {
                if (graph.getInDegree(i) == 0) {
                    ranks[i] = current_rank;
                    graph.removeNode(i);
                }
            }
            current_rank++;
        }
        return ranks;
    }

    scores_t NSGA2::crowding_distance(population_t &population) {
        size_t size = population.size();
        std::vector<double> distances(size, 0);

        std::vector<std::vector<double>> obj_vals(size);
        for (size_t i = 0; i < size; i++) {
            obj_vals[i] = f(population[i]);
        }

        size_t out_size = obj_vals[0].size();

        for (size_t j = 0; j < out_size; j++) {
            std::vector<index_t> indices(size);
            std::iota(indices.begin(), indices.end(), 0);
            std::sort(indices.begin(), indices.end(),
                      [&](index_t i, index_t k) {
                          return obj_vals[i][j] < obj_vals[k][j];
                      });

            distances[indices[0]] = std::numeric_limits<double>::infinity();
            distances[indices[size - 1]] =
                std::numeric_limits<double>::infinity();

            double min = obj_vals[indices[0]][j];
            double max = obj_vals[indices[size - 1]][j];
            double range = max - min;

            if (range == 0) {
                continue;
            }
            for (size_t i = 1; i < size - 1; i++) {
                distances[indices[i]] += (obj_vals[indices[i + 1]][j] -
                                          obj_vals[indices[i - 1]][j]) /
                                         range;
            }
        }

        return distances;
    }

    void NSGA2::select(population_t &population, ranks_t &ranks) {
        size_t size = population.size();
        std::sort(population.begin(), population.end(),
                  [&](const individual_t &a, const individual_t &b) {
                      size_t index_a = &a - &population[0];
                      size_t index_b = &b - &population[0];
                      return ranks[index_a] < ranks[index_b];
                  });
        for (int l = 0; l < size; l++) {
            int r = l + 1;
            while (r < size && ranks[r] == ranks[l]) {
                r++;
            }
            if (r == size) {
                break;
            }
        }
    }

    population_t NSGA2::run(criterion_t criterion) {
        size_t iter = 0;
        while (!criterion(population, iter)) {
            mutate(population);
            ranks_t ranks = non_dominate_sort(population);
            // fetch all individuals in the current rank
            select(population, ranks);
        }
        return population;
    }

} // namespace nsga2