#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<std::vector<int>> graph;
typedef std::vector<std::pair<size_t, size_t>> timeline;

void dfs(size_t vertex, graph const& graph, std::vector<uint8_t>& visited, timeline& time) {
    static size_t t = 0;
    visited[vertex] = 1;
    time[vertex].first = t++;
    for(auto const v : graph[vertex]) {
        if(!visited[v]) {
            dfs(v, graph, visited, time);
        }
    }
    time[vertex].second = t++;
}

bool parent_check(timeline const& time, int v, int u) {
    return time[v].first < time[u].first && time[u].second < time[v].second;
}

int main() {
    int n;
    std::cin >> n;
    
    int v, u;
    graph g(n);
    auto root_number = 0;
    for (auto u = 0; u < n; ++u) {
        std::cin >> v;
        if (v == 0) {
            root_number = v;
        }
        else {
            --v;
            g[u].push_back(v);
            g[v].push_back(u);
        }
    }

    std::vector<std::pair<size_t, size_t>> times(n);
    std::vector<uint8_t> visited(n, 0);
    dfs(root_number, g, visited, times);

    int m;
    std::cin >> m;
    std::vector<uint8_t> result(m);
    for (auto i = 0; i < m; ++i) {
        std::cin >> u >> v;
        --u; --v;
        result[i] = parent_check(times, u, v);
    }

    std::for_each(result.begin(), result.end(), [](int val) {std::cout << val << std::endl; });
}
