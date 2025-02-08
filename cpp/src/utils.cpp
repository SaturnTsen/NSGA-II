#include "utils.h"
// #include <iostream>
// #include <ostream>
// #include <unordered_map>

// template <typename T>
// void indexed_set<T>::insert(const T &value) {
//     if (index_map.find(value) != index_map.end()) {
//         return;
//     }
//     arr.push_back(value);
//     index_map[value] = arr.size() - 1;
// }

// template <typename T>
// void indexed_set<T>::erase(const T &value) {
//     if (index_map.find(value) == index_map.end()) {
//         return;
//     }
//     int idx = index_map[value];
//     int last_idx = arr.size() - 1;

//     if (idx != last_idx) {
//         std::swap(arr[idx], arr[last_idx]);
//         index_map[arr[idx]] = idx;
//     }

//     arr.pop_back();
//     index_map.erase(value);
// }

// template <typename T>
// T indexed_set<T>::get(int index) {
//     return arr[index]; // O(1) 访问
// }

// template <typename T>
// int indexed_set<T>::index_of(const T &value) {
//     return index_map.count(value) ? index_map[value] : -1;
// }

// template <typename T>
// int indexed_set<T>::size() {
//     return arr.size();
// }

// template <typename T>
// std::ostream &operator<<(std::ostream &os, const indexed_set<T> &set) {
//     os << "[";
//     for (int i = 0; i < set.size(); ++i) {
//         os << set.get(i);
//         if (i < set.size() - 1) {
//             os << ", ";
//         }
//     }
//     os << "]";
//     return os;
// }