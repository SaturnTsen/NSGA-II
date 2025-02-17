#include "binaryheap.h"
#include <cassert>
#include <print>
#include <limits>

using namespace binary_heap;


int main() {

    BinaryHeap<double, int> queue{};

    double inf = std::numeric_limits<double>::infinity();
    assert(inf == inf);
    std::vector<double> values{1., -4., 3., inf, 7., 2., 9., inf, 8., 2., 6.};
    int n = values.size();

    for (int i = 0; i < n; ++i) {
        queue.emplace(Node{.key = values[i], .id = i});
        assert(queue.has_heap_property());
    }
    std::print("Priority queue contents: ");
    for (int i = 0; i < n; ++i) {
        std::print("{0} ", queue[i].key);
    }
    std::println("");

    // 7.0 -> -1.0
    queue.decrease_key(4, -1.0);
    assert(queue.has_heap_property());

    std::vector<double> ordered;
    std::println("Popping values");
    for (int i = 0; i < n; ++i) {
        double m = queue.extract_min().key;
        assert(queue.has_heap_property());
        ordered.push_back(m);
        std::print("{0} ", m);
    }
    std::println("");
    std::vector<double> expected{-4., -1., 1., 2., 2., 3., 6., 8., 9., inf, inf};
    assert(ordered == expected);

    return 0;
}