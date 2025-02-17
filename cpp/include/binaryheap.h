#pragma once

#include <cassert>
#include <unordered_map>
#include <vector>

namespace binary_heap {

    /**
     * @brief A min-priority queue implemented by a binary heap.
     * 
     * @tparam T `Comparable`
     */
    template <typename T>
    class BasicBinaryHeap {
        std::vector<T> data;

        public:

        BasicBinaryHeap() : data() {}

        /** Add an element in the binary heap. */
        void emplace(T x) {
            int index = data.size();

            data.push_back(x);
            while (index > 0 && data[(index - 1) / 2] > data[index]) {
                std::swap(data[(index - 1) / 2], data[index]);
            }
        }

        /** Extract and return the minimum value of this binary heap. */
        T extract_min() {
            int n = size();
            assert(n > 0);

            T max = data[0];

            // Replace the root with the last element
            data[0] = data[n - 1];

            int i = 0;
            // Sift down this element until it is correctly placed
            while (i < n - 1) {
                int c1 = 2*i + 1;
                int c2 = c1 + 1;
                if (c1 < n && data[c1] < data[i]) {
                    std::swap(data[c1], data[i]);
                    i = c1;
                } else if (c2 < n && data[c2] < data[i]) {
                    std::swap(data[c2], data[i]);
                    i = c2;
                } else {
                    break;
                }
            }
            return max;
        }

        /** The number of elements in the heap. */
        size_t size() {
            return data.size();
        }
    };

    /* An element of a `BinaryHeap` compared by `key` and indexed by `id`. */
    template <typename K, typename I>
    struct Node {
        public:
        K key;
        I id;
    };

    /**
     * @brief A min-priority queue with indices implemented by a binary heap.
     *
     * @details This data structure implements the `decrease_key` operation.
     *
     * @tparam I `Hashable` : id
     * @tparam K `Comparable` : key
     */
    template <typename K, typename I>
    class BinaryHeap {
        std::unordered_map<I, int> indices;
        std::vector<Node<K, I>> nodes;

        public:

        BinaryHeap() : indices(), nodes() {}

        /** The number of elements in the heap. */
        size_t size() {
            return nodes.size();
        }

        Node<K, I> &operator[](int i) {
            return nodes[i];
        }

        /** Add an element in the binary heap. */
        void emplace(Node<K, I> node) {
            assert(indices.find(node.id) == indices.end());

            int index = size();
            nodes.push_back(node);
            indices.emplace(node.id, index);

            sift_up(index);
        }

        /** Extract and return the minimum value of this binary heap. */
        Node<K, I> extract_min() {
            int n = size();
            assert(n > 0);

            Node min = nodes[0];

            // Remove the root node
            indices.erase(min.id);

            // Replace the root with the last element
            nodes[0] = nodes[n - 1];
            indices[nodes[0].id] = 0;
            nodes.pop_back();

            sift_down(0);
            return min;
        }

        /** Get the key associated to an id. */
        K get_key(I id) {
            return nodes[indices[id]].key;
        }

        /** Decrease the key of the element specified by `id`. */
        void decrease_key(I id, K new_key) {
            assert(indices.find(id) != indices.end());
            int index = indices[id];
            assert(nodes[index].key >= new_key);
            nodes[index].key = new_key;
            sift_up(index);
        }

        private:

        void swap_nodes(int i, int j) {
            indices[nodes[i].id] = j;
            indices[nodes[j].id] = i;
            std::swap(nodes[i], nodes[j]);
        }

        
        /* Sift down this element until it is correctly placed. */
        void sift_down(int i) {
            int n = size();
            while (i < n - 1) {
                int c1 = 2*i + 1;
                int c2 = c1 + 1;
                if (c2 < n && nodes[c1].key > nodes[c2].key) {
                    std::swap(c1, c2);
                    assert(nodes[c1].key <= nodes[c2].key);
                }
                // Swap the node with its children with the lowest key
                if (c1 < n && nodes[c1].key < nodes[i].key) {
                    swap_nodes(c1, i);
                    i = c1;
                } else {
                    break;
                }
            }
        }

        /* Sift down this element until it is correctly placed. */
        void sift_up(int index) {
            while (index > 0) {
                int parent = (index - 1) / 2;
                if (nodes[parent].key <= nodes[index].key) {
                    break;
                }
                swap_nodes(parent, index);
                index = parent;
            }
        }

        public:
        /* A quick sanity check that ensures the heap is well-formed. */
        bool has_heap_property() {
            int n = size();
            for (int i = 0; i < n; ++i) {
                int c1 = 2 * i + 1;
                int c2 = 2 * i + 2;
                if (c1 < n && nodes[c1].key < nodes[i].key) {
                    return false;
                }
                if (c2 < n && nodes[c2].key < nodes[i].key) {
                    return false;
                }
            }
            return true;
        }
    };
} // namespace binary_heap

