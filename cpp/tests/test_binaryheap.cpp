#include "binaryheap.h"
#include <cassert>
#include <print>

using namespace binary_heap;


int main() {

    BinaryHeap<double, int> queue{};

    std::vector<double> values{1., -4., 3., 7., 2.};
    int n = values.size();

    for (int i = 0; i < n; ++i) {
        queue.emplace(values[i], i);
    }
    // 7.0 -> -1.0
    queue.decrease_key(3, -1.0);

    std::vector<double> ordered;
    for (int i = 0; i < n; ++i) {
        double m = queue.extract_min();
        ordered.push_back(m);
        std::println("{0}", m);
    }
    std::vector<double> expected{-4., -1., 1., 2., 3.};
    assert(ordered == expected);

    return 0;
}