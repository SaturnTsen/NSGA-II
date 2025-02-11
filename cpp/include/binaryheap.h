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


    template <typename K, typename I>
    struct Node {
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

        /** Add an element in the binary heap. */
        void emplace(K key, I id) {
            assert(indices.find(id) == indices.end());

            int index = size();
            nodes.push_back(Node{key, id});
            indices.emplace(id, index);

            while (index > 0 && nodes[(index - 1) / 2].key > nodes[index].key) {
                // Swap the node with its parent while the parent has a greater key
                swap_nodes((index - 1) / 2, index);
            }
        }

        /** Extract and return the minimum value of this binary heap. */
        K extract_min() {
            int n = size();
            assert(n > 0);

            K min = nodes[0].key;

            // Remove the root node
            indices.erase(nodes[0].id);

            // Replace the root with the last element
            nodes[0] = nodes[n - 1];
            nodes.pop_back();

            sift_down(0);
            return min;
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
                // Swap the node with one of its children with a lower key
                if (c1 < n && nodes[c1].key < nodes[i].key) {
                    swap_nodes(c1, i);
                    i = c1;
                } else if (c2 < n && nodes[c2].key < nodes[i].key) {
                    swap_nodes(c2, i);
                    i = c2;
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
    };
} // namespace binary_heap

