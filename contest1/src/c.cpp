#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<std::vector<int>> graph;

int get_green_egde_count() {
    return 0;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    int capital;
    std::cin >> capital;
    graph graph(n);

    int from, to;
    for (auto i = 0; i < m; ++i) {
        std::cin >> from >> to; --from; --to;
        graph[from].push_back(to);
        graph[to].push_back(from);
    }

    int k;
    std::cin >> k;

    std::vector<int> result(k);
    for (auto i = 0; i < k; ++i) {
        result[i] = get_green_egde_count();
    }

    std::for_each(result.begin(), result.end(), [](int val) {std::cout << val << std::endl; });

    return 0;
}
