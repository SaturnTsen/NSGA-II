#pragma once

#include <cassert>
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
    std::unordered_map<T, std::unordered_set<T>> adj_list_rev;
    std::unordered_map<T, int> in_degree;

  public:
    /**
     * @brief Add a node to the graph
     *
     * @param node
     */
    void addNode(const T &node) {
        if (adj_list.find(node) == adj_list.end()) {
            adj_list[node] = std::unordered_set<T>();
            adj_list_rev[node] = std::unordered_set<T>();
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
        assert(adj_list.find(from) != adj_list.end());
        assert(adj_list.find(to) != adj_list.end());

        if (adj_list[from].insert(to).second &&
            adj_list_rev[to].insert(from).second) {
            in_degree[to]++;
        }
    }

    /**
     * @brief Remove a node from the graph
     *
     * @param node
     */
    void removeNode(const T &node) {
        if (adj_list.find(node) == adj_list.end()) {
            throw std::runtime_error("Node does not exist");
        }

        for (auto &neighbor : adj_list[node]) {
            adj_list_rev[neighbor].erase(node);
            in_degree[neighbor]--;
        }

        for (auto &neighbor : adj_list_rev[node]) {
            adj_list[neighbor].erase(node);
        }

        adj_list.erase(node);
        adj_list_rev.erase(node);
        in_degree.erase(node);
    }

    /**
     * @brief Get the in degree of a node
     *
     * @param node
     * @return int
     */
    int getInDegree(const T &node) {
        if (in_degree.find(node) == in_degree.end()) {
            throw std::runtime_error("Node does not exist");
        }
        return in_degree[node];
    }

    size_t size() const { return adj_list.size(); }
};
