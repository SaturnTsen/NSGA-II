#include "modified_nsga2.h"
#include "binaryheap.h"

namespace nsga2 {
    void PositionDoublyLinkedList::remove(index_t x) {
        SortedPosition pos = nodes[x];

        if (pos.has_prev() && pos.has_next()) {
            nodes[pos.prev].next = pos.next;
            nodes[pos.next].prev = pos.prev;
        } else if (pos.has_prev()) {
            // prev has no right neighbor
            nodes[pos.prev].delete_next();
        } else if (pos.has_next()) {
            // next has no left neighbor
            nodes[pos.next].delete_prev();
        }
        deleted[x] = true;
    }

    PositionDoublyLinkedList::PositionDoublyLinkedList(front_t &sorted_front) :
        PositionDoublyLinkedList(sorted_front.size())
    {
        int n = sorted_front.size();
        for (int i = 0; i < n; ++i) {
            SortedPosition pos{
                .prev = sorted_front[i],
                .self = sorted_front[i],
                .next = sorted_front[i],
            };
            if (i > 0) {
                pos.prev = sorted_front[i - 1];
            }
            if (i < n - 1) {
                pos.next = sorted_front[i + 1];
            }
            nodes[sorted_front[i]] = pos;
        }
    }

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
        // O(NlogN) in the worst case

        // Sort the front according to crowding distance. This gives the initial
        // ordering of individuals in `positions`.
        std::sort(front.begin(), front.end(),
                          [&](index_t a, index_t b) { return scores[a] > scores[b]; });
        
        // `positions[x]` gives the position of `x` according to crowding distance,
        // if `index_t x` is the population index of an individual.
        PositionDoublyLinkedList positions(front);
        BinaryHeap<double, index_t> pqueue{};
        for (const auto &score : scores) {
            Node<double, index_t> node{ .key = score.second, .id = score.first };
            std::cout << "score " << score.second << std::endl;
            // FIXME: this line fails with the 8th vector insertion:
            // `malloc(): mismatching next->prev_size (unsorted)`
            pqueue.emplace(node);
        }

        // This value is guaranteed not to change since the border elements have
        // their crowding distance equal to positive infinity, so they would be last
        // to be removed
        double d = scores[front[front.size() - 1]] - scores[front[0]];

        // Remove the individuals with smallest crowding distance
        while (pqueue.size() > remaining) {
            // TODO: break ties at random
            index_t index = pqueue.extract_min().id;

            SortedPosition pos = positions[index];

            // Remove `index` and update the neighbor links in the linked list
            positions.remove(index);

            // Recompute the crowding distance of its neighbors.
            // If `pos` is a border node, its crowding distance is already infinity.
            // Moreover, `pos` cannot have become a "border" node, since border nodes
            // are removed last as the priority queue pops the min element first.
            // FIXME: Once only nodes with infinite crowding distance remain,
            // there might be problems with inf - inf
            if (pos.has_prev() && pos.has_next()) {
                // Note that the positions have been updated, so `pos_prev` and `pos_next`
                // are actually adjacent.
                SortedPosition pos_prev = positions[pos.prev];
                if (pos_prev.has_prev() && std::isfinite(scores[pos.prev])) {
                    double dist = (scores[pos_prev.next] - scores[pos_prev.prev]) / d;
                    pqueue.decrease_key(pos.prev, dist);
                }
                SortedPosition pos_next = positions[pos.next];
                assert(pos_prev.next == pos_next.self && pos_next.prev == pos_prev.self);
                if (pos_next.has_next() && std::isfinite(scores[pos.next])) {
                    double dist = (scores[pos_next.next] - scores[pos_next.prev]) / d;
                    pqueue.decrease_key(pos.next, dist);
                }
            }
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