#include "modified_nsga2.h"
#include "binaryheap.h"

namespace nsga2 {
    population_t ModifiedNSGA2::crowding_distance_select(population_t &population, fronts_t &fronts) {
        using binary_heap::BinaryHeap;
        using binary_heap::Node;

        // TODO Test & Performance improvements
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
        // O(mNlogN) N is the size of the front, m is the number of objectives
        scores_t scores = std::move(crowding_distance(population, front));
        size_t remaining = target_size - new_population.size();
        
        BinaryHeap<double, index_t> pqueue;
        for (const auto &score : scores) {
            // TODO: store neighbor info in `id` (sort by score before?)
            // feels like doubly-linked list
            pqueue.emplace(Node { .key = score.second, .id = score.first });
        }
        // Remove the individuals with smallest crowding distance
        while (pqueue.size() > remaining) {
            // TODO: break ties at random
            index_t index = pqueue.extract_min().id;

            // TODO: recompute crowding distance here
            //pqueue.decrease_key();
        }
        // Retain the remaining individuals
        for (size_t i = 0; i < remaining; i++) {
            index_t index = pqueue[i].id;
            new_population.push_back(population[index]);
        }

        if (new_population.size() != target_size) {
            throw std::runtime_error("new_population.size() != target_size. "
                                    "check if there is a bug.");
        }
        return new_population;
    }
}