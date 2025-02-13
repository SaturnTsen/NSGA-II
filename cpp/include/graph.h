#pragma once

#include <cassert>
#include <cstddef>
#include <queue>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
/**
 * @brief A simple graph implementation using adjacency lists.
 *
 * @tparam T
 */
template <typename T>
class Graph {
    std::unordered_map<T, std::unordered_set<T>> adj_list;
    std::unordered_map<T, size_t> in_degree;

    using rank_t = size_t;
    using front_t = std::vector<T>;
    using fronts_t = std::vector<front_t>;
    bool destructed = false;

  public:
    /**
     * @brief Add a node to the graph.
     *
     * @param node
     */
    void add_node(const T &node) {
        if (destructed)
            throw std::runtime_error("Graph has been destructed");
        if (!adj_list.contains(node)) {
            adj_list[node] = std::unordered_set<T>();
            in_degree[node] = 0;
        }
    }

    /**
     * @brief Add an edge between two nodes.
     *
     * @param from
     * @param to
     */
    void add_edge(const T &from, const T &to) {
        if (destructed)
            throw std::runtime_error("Graph has been destructed");
        assert(adj_list.find(from) != adj_list.end());
        assert(adj_list.find(to) != adj_list.end());

        if (adj_list[from].insert(to).second) {
            in_degree[to]++;
        }
    }

    /**
     * Gets the fronts of a directed acyclic graph and destroy the graph.
     */
    fronts_t pop_and_get_fronts() {
        if (destructed)
            throw std::runtime_error("Graph has been destructed");
        fronts_t fronts;

        // Use a queue to perform breadth-first search from the first front
        std::queue<T> q;
        std::unordered_set<T> visited;
        // Initialization: store the first front in the queue
        for (const auto &[node, degree] : in_degree) {
            // Nodes with in degree equal to zero are not dominated
            if (degree == 0) {
                q.push(node);
                visited.insert(node);
            }
        }

        // Core algorithm
        while (!q.empty()) {
            // Move the last computed front from the queue into the fronts
            fronts.push_back(front_t());
            auto &last_front = fronts.back();
            while (!q.empty() && in_degree[q.front()] == 0) {
                T node = q.front();
                q.pop();
                last_front.push_back(node);
            }

            // Explore the next front from the last computed front
            // and gather its nodes into the queue
            for (const auto &node : last_front) {
                for (const auto &neighbor : adj_list[node]) {
                    // This simulates the removal of the edge `node -> neighbor`.
                    in_degree[neighbor]--;
                    // Eventually, all of the nodes inside `last_front` will have been
                    // "removed", so the nodes in the next front becomes non-dominated.
                    if (in_degree[neighbor] == 0 && !visited.contains(neighbor)) {
                        q.push(neighbor);
                        visited.insert(neighbor);
                    }
                }
            }
        }
        // Since we perform BFS, the time complexity is O(N + M) = O(N^2)
        destructed = true;
        return fronts;
    }

    /**
     * @brief Get the in degree of a node.
     *
     * @param node
     * @return int
     */
    size_t get_in_degree(const T &node) const {
        if (destructed)
            throw std::runtime_error("Graph has been destructed");
        if (!in_degree.contains(node))
            throw std::runtime_error("Node does not exist");
        return in_degree.at(node);
    }

    /**
     * @brief Get the number of nodes in the graph.
     *
     * @return size_t
     */
    size_t size() const {
        if (destructed)
            throw std::runtime_error("Graph has been destructed");
        return adj_list.size();
    }
};
