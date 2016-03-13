#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<std::vector<std::pair<int, int>>> weighted_graph;
typedef std::vector<std::pair<size_t, size_t>> timeline;

const int INF = 1000001;

void dfs(size_t vertex, weighted_graph const& graph, std::vector<uint8_t>& visited, timeline& time, std::vector<int>& deep, int current_level) {
    static size_t t = 0;
    visited[vertex] = 1;
    time[vertex].first = t++;
    deep[vertex] = current_level;
    for (auto const v : graph[vertex]) {
        if (visited[v.first] == 0) {
            dfs(v.first, graph, visited, time, deep, current_level + 1);
        }
    }

    time[vertex].second = t++;
}
std::pair<int, int> get_min_lca(std::vector<std::vector<std::pair<int, int>>> const& dp, std::vector<int>& deep, int u, int v) {
    if(u == v) {
        return std::make_pair(v, 0);
    }
    
    auto min_weight = INF;
    const auto log_n = static_cast<int>(log2(dp.size()));
    if(deep[v] > deep[u]) {
        std::swap(u, v);
    }

    for (auto i = log_n - 1; i >= 0; --i) {
        if (deep[u] - deep[v] >= pow(2, i)) {
            min_weight = std::min(min_weight, dp[u][i].second);
            u = dp[u][i].first;
        }
    }

    if(u == v) {
        return std::make_pair(v, min_weight);
    }

    for (auto i = log_n - 1; i >= 0; --i) {
        if(dp[v][i].first != dp[u][i].first) {
            min_weight = std::min(std::min(dp[v][i].second, dp[u][i].second), min_weight);
            v = dp[v][i].first;
            u = dp[u][i].first;
        }
    }

    if (v != u && dp[v][0].first == dp[u][0].first)
        return std::make_pair(dp[v][0].first, std::min(std::min(dp[v][0].second, dp[u][0].second), min_weight));
    else
        return std::make_pair(v, min_weight);
}

int main() {
    int n;
    std::cin >> n;
    weighted_graph graph(n);
    std::vector<int> parent(n, 0);
    std::vector<int> weights(n, INF);
    int p, w;
    for (auto i = 1; i < n; ++i) {
        std::cin >> p >> w; --p;
        graph[i].emplace_back(p, w);
        graph[p].emplace_back(i, w);
        parent[i] = p;
        weights[i] = w;
    }

    std::vector<std::pair<size_t, size_t>> times(n);
    std::vector<uint8_t> visited(n, 0);
    std::vector<int> d(n);
    dfs(0, graph, visited, times, d, 0);

    auto log_n = static_cast<int>(log2(n));
    std::vector<std::vector<std::pair<int, int>>> dp(n);
    for (auto i = 0; i < n; ++i) {
        dp[i].resize(log_n);
        dp[i][0].first = parent[i];
        dp[i][0].second = weights[i];
    }

    for (auto i = 0; i < log_n; ++i) {
        dp[0][i].second = weights[0];
        dp[0][i].first = 0;
    }

    for (auto j = 1; j < log_n; ++j) {
        for (auto i = 1; i < n; ++i) {
            auto const& prev_node = dp[i][j - 1];
            auto const& new_node = dp[prev_node.first][j - 1];
            dp[i][j].first = new_node.first;
            dp[i][j].second = std::min(prev_node.second, new_node.second);
        }
    }

    int m;
    std::cin >> m;
    std::vector<int> result(m);
    int u, v;
    for (auto i = 0; i < m; ++i) {
        std::cin >> u >> v; --u; --v;
        result[i] = get_min_lca(dp, d, u, v).second;
    }

    std::for_each(result.begin(), result.end(), [](int val) {std::cout << val << std::endl; });

    return 0;
}