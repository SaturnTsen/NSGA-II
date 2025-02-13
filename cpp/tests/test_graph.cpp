#include "graph.h"
#include <algorithm>
#include <cassert>
#include <print>
#include <vector>

using namespace std;
void test1() {
    Graph<int> g;
    vector<int> nodes{0, 1, 2, 3};

    for (auto node : nodes) {
        g.add_node(node);
    }
    assert(g.size() == 4);
    g.add_edge(0, 1);
    g.add_edge(1, 2);
    g.add_edge(2, 0);
    g.add_edge(2, 3);

    vector<int> expected_in_degrees{1, 1, 1, 1};

    for (size_t i = 0; i < nodes.size(); i++) {
        println("{0}", g.get_in_degree(nodes[i]));
        assert(g.get_in_degree(nodes[i]) == expected_in_degrees[i]);
    }

    g.add_edge(3, 2);
    g.add_edge(3, 1);

    expected_in_degrees = {1, 2, 2, 1};

    for (size_t i = 0; i < nodes.size(); i++) {
        println("{0}", g.get_in_degree(nodes[i]));
        assert(g.get_in_degree(nodes[i]) == expected_in_degrees[i]);
    }
}
void test2() {
    Graph<int> g;
    for (int i = 0; i < 5; i++) {
        g.add_node(i);
    }
    g.add_edge(0, 1);
    g.add_edge(2, 4);
    g.add_edge(0, 4);
    g.add_edge(4, 3);
    g.add_edge(0, 2);
    g.add_edge(1, 3);

    auto fronts = g.pop_and_get_fronts();

    if (fronts.size() != 4)
        throw runtime_error("fronts size is not 4");

    sort(fronts[0].begin(), fronts[0].end());
    if (fronts[0] != vector<int>{0})
        throw runtime_error("fronts[0] is not {0}");

    sort(fronts[1].begin(), fronts[1].end());
    if (fronts[1] != vector<int>{1, 2})
        throw runtime_error("fronts[1] is not {1, 2}");

    if (fronts[2] != vector<int>{4})
        throw runtime_error("fronts[2] is not {4}");

    if (fronts[3] != vector<int>{3})
        throw runtime_error("fronts[3] is not {3}");
}

void test3() {
    // multi leading nodes
    Graph<int> g;
    for (int i = 0; i < 7; i++) {
        g.add_node(i);
    }

    g.add_edge(0, 1);
    g.add_edge(2, 4);
    g.add_edge(0, 4);
    g.add_edge(4, 3);
    g.add_edge(0, 2);
    g.add_edge(1, 3);
    g.add_edge(5, 6);

    auto fronts = g.pop_and_get_fronts();

    if (fronts.size() != 4)
        throw runtime_error("fronts size is not 4");

    sort(fronts[0].begin(), fronts[0].end());
    if (fronts[0] != vector<int>{0, 5})
        throw runtime_error("fronts[0] is not {0, 5}");

    sort(fronts[1].begin(), fronts[1].end());
    if (fronts[1] != vector<int>{1, 2, 6})
        throw runtime_error("fronts[1] is not {1, 2}");

    if (fronts[2] != vector<int>{4})
        throw runtime_error("fronts[2] is not {4}");

    if (fronts[3] != vector<int>{3})
        throw runtime_error("fronts[3] is not {3}");
}
int main() {
    test1();
    test2();
    test3();
    return 0;
}