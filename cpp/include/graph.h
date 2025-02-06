#include <vector>

#ifndef NSGAII_GRAPH_H

#define NSGAII_GRAPH_H

template <typename V>
using AdjacencyList = std::vector<V>;

/**
 * A directed graph implemented by adjacency lists.
 *
 * Vertices must be of integer type `V` and identified by 0 .. n-1.
 */
template <typename V, typename E>
class Graph {
  public:
    /* Create an empty graph with `n` vertices. */
    Graph(int n);

    /* Returns the neighbors of a vertex. */
    AdjacencyList<V> &neighbors(V v);

  private:
    std::vector<AdjacencyList<V>> adjacency_lists;
};

#endif