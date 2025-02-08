#include "graph.h"
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    Graph<int> g;
    vector<int> nodes{0, 1, 2, 3};

    for (auto node : nodes) {
        g.addNode(node);
    }
    assert(g.size() == 4);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(2, 3);

    vector<int> expected_in_degrees{1, 1, 1, 1};

    for (size_t i = 0; i < nodes.size(); i++) {
        cout << g.getInDegree(nodes[i]) << endl;
        assert(g.getInDegree(nodes[i]) == expected_in_degrees[i]);
    }

    g.addEdge(3, 2);
    g.addEdge(3, 1);

    expected_in_degrees = {1, 2, 2, 1};

    for (size_t i = 0; i < nodes.size(); i++) {
        cout << g.getInDegree(nodes[i]) << endl;
        assert(g.getInDegree(nodes[i]) == expected_in_degrees[i]);
    }

    g.removeNode(2);
    assert(g.getInDegree(0) == 0);
    assert(g.getInDegree(1) == 2);
    assert(g.getInDegree(3) == 0);
    assert(g.size() == 3);
    return 0;
}