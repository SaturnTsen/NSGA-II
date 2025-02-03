#include <stdexcept>
#include "individual.h"
//#include "benchmark.h"


using individual::individual_t;

// An index of an individual in a list.
typedef int index_t;

typedef std::vector<index_t> front_t;

typedef int rank_t;

std::vector<front_t> non_dominated_sorting(
    std::vector<individual_t> population,
    objective::fn_t f
) {
    
    // Idea: f defines a partial relationship order which can be represented
    // as a directed acyclic graph with potentially multiple connected components.
    // We need to store it as an adjacency list for efficient graph search algorithms.
    // This makes it clear the runtime complexity can't be better than O(N^2).

    // Precision: in order to take equivalent individuals into account, we can consider
    // a 0/1 directed weighted graph where bidirectional edges of zero weight are added
    // between two equivalent individuals.
    
    // The algorithms works as follows: start from a vertex v0 and perform
    // depth-first search. For each other vertex v, store the maximum depth
    // at which it has been encountered (the depth is relative to v0).
    // Repeat the previous operation for each vertex of the graph.
    // At any point, the depth-first search can be backtracked if the current depth
    // is lower or equal than the stored rank.

    int n = population.size();
    std::vector<int> ranks(n, 0);

    throw std::runtime_error("Unimplemented");
}