#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>

int main() {
    std::vector<int> vec = {10, 20, 30, 40, 50, 60, 70};
    std::unordered_set<size_t> indices_to_remove = {1, 4,
                                                    6}; // 下标集合 {1, 4, 6}

    // 使用 std::remove_if 和 set 来移除指定下标的元素
    auto it = std::remove_if(vec.begin(), vec.end(), [&](int value) {
        static size_t index = 0;
        return indices_to_remove.count(index++) >
               0; // 如果下标在集合中，则返回 true，表示移除该元素
    });

    // 删除实际的元素
    vec.erase(it, vec.end());

    // 输出处理后的结果
    for (int val : vec) {
        std::cout << val << " ";
    }

    return 0;
}
