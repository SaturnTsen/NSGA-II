#include "graph.h"

template <typename V, typename E>
Graph<V, E>::Graph(int n) : adjacency_lists(n) {}

template <typename V, typename E>
AdjacencyList<V> &Graph<V, E>::neighbors(V v) {
    return adjacency_lists[v];
}
