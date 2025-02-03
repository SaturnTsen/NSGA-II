#include "graph.h"

template<typename V, typename E>
class Graph {
    public:
        Graph(int n) : adjacency_lists(n) {}

        AdjacencyList<V> &neighbors(V v) {
            return &adjacency_lists[v];
        }
};