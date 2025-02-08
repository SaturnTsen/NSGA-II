#pragma once

#include <cassert>
#include <cstddef>
#include <queue>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
/**
 * @brief A simple graph implementation using adjacency list
 *
 * @tparam T
 */
template <typename T>
class Graph {
    std::unordered_map<T, std::unordered_set<T>> adj_list;
    std::unordered_map<T, int> in_degree;

    using rank_t = size_t;
    using front_t = std::vector<T>;
    using fronts_t = std::vector<front_t>;
    bool destructed = false;

  public:
    /**
     * @brief Add a node to the graph
     *
     * @param node
     */
    void addNode(const T &node) {
        if (destructed) {
            throw std::runtime_error("Graph has been destructed");
        }
        if (adj_list.find(node) == adj_list.end()) {
            adj_list[node] = std::unordered_set<T>();
            in_degree[node] = 0;
        }
    }

    /**
     * @brief Add an edge between two nodes
     *
     * @param from
     * @param to
     */
    void addEdge(const T &from, const T &to) {
        if (destructed) {
            throw std::runtime_error("Graph has been destructed");
        }
        assert(adj_list.find(from) != adj_list.end());
        assert(adj_list.find(to) != adj_list.end());

        if (adj_list[from].insert(to).second) {
            in_degree[to]++;
        }
    }

    fronts_t popAndGetFronts() {
        if (destructed) {
            throw std::runtime_error("Graph has been destructed");
        }
        fronts_t fronts;

        std::queue<T> q;
        std::unordered_set<T> visited;
        // Initialization
        for (const auto &[node, degree] : in_degree) {
            if (degree == 0) {
                q.push(node);
                visited.insert(node);
            }
        }

        // Core algorithm
        while (!q.empty()) {
            fronts.push_back(front_t());
            auto &last_front = fronts.back();
            while (!q.empty() && in_degree[q.front()] == 0) {
                T node = q.front();
                q.pop();
                last_front.push_back(node);
            }

            for (const auto &node : last_front) {
                // We fall into the following loop N times
                for (const auto &neighbor : adj_list[node]) {
                    // The loop will repeat at most N times
                    in_degree[neighbor]--;
                    if (in_degree[neighbor] == 0 &&
                        visited.find(neighbor) == visited.end()) {
                        q.push(neighbor);
                        visited.insert(node);
                    }
                }
            }
        }
        // Thus, the time complexity is O(N^2)
        destructed = true;
        return fronts;
    }

    /**
     * @brief Get the in degree of a node
     *
     * @param node
     * @return int
     */
    int getInDegree(const T &node) {
        if (destructed) {
            throw std::runtime_error("Graph has been destructed");
        }
        if (in_degree.find(node) == in_degree.end()) {
            throw std::runtime_error("Node does not exist");
        }
        return in_degree[node];
    }

    size_t size() const {
        if (destructed) {
            throw std::runtime_error("Graph has been destructed");
        }
        return adj_list.size();
    }
};
