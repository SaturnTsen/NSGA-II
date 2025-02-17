#pragma once

#include "individual.h"
#include "nsga2.h"

namespace nsga2 {
    /** @brief  wrapper over an `index_t` that contains metadata about its neighbors
     * in the sorted indices.
     * 
     * `prev` is equal to `self` if the individual is first in the sorted list.
     * Likewise, `next` is equal to `self` if the individual is last in the sorted list.
     * 
     * This represents a node of a linked list.
     * */
    struct SortedPosition {
        // The previous individual in the sorted list.
        index_t prev;

        // The individual indicated by this sorted position. This value should never change.
        index_t self;

        // The next individual in the sorted list.
        index_t next;

        bool has_prev() {
            return prev != self;
        }

        bool has_next() {
            return next != self;
        }

        void delete_prev() {
            prev = self;
        }

        void delete_next() {
            next = self;
        }
    };

    /** A doubly-linked list, indexed by `index_t`. */
    using PositionLinkedList = std::vector<SortedPosition>;

    /**
     * @brief A fixed-capacity doubly-linked list, indexed by `index_t`.
     * 
     * This data structure is initialized once and only implements removal.
     * Inner nodes can be accessed by indexing.
     * */
    class PositionDoublyLinkedList {
        // FIXME: check if SortedPosition as a struct is not copied when indexing in this vector.
        std::vector<SortedPosition> nodes;
        std::vector<bool> deleted;

        public:

        PositionDoublyLinkedList(int n) : nodes(n), deleted(n, false) {}

        /* Construct a linked list of population indices from a sorted front. */
        PositionDoublyLinkedList(front_t &sorted_front);

        SortedPosition operator[](index_t x) {
            if (deleted[x]) {
                throw std::runtime_error("Tried to access a deleted node");
            }
            return nodes[x];
        }

        /** Remove `x` and update its neighbor links in the linked list. */
        void remove(index_t x);
    };

    class ModifiedNSGA2 : public NSGA2 {
        public:

        using NSGA2::NSGA2;

        population_t crowding_distance_select(population_t &population, fronts_t &fronts);
    };
}