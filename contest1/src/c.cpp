#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<std::vector<int>> graph_t;
typedef std::vector<std::vector<int>> lca_t;

int timer = 0;
std::vector<int> tin;
std::vector<int> fup;

// find bridges and parents
void dfs1(int v, graph_t const& graph, std::vector<uint8_t>& visited, std::vector<int>& parent, std::vector<uint8_t>& brigde) {
    visited[v] = 1;
    tin[v] = fup[v] = timer++;
    for(auto u : graph[v]) {
        if (u == parent[v]) {
            continue;
        }

        if(!visited[u]) {
            parent[u] = v;
            dfs1(u, graph, visited, parent, brigde);
            fup[v] = std::min(fup[v], fup[u]);
            if (fup[u] > tin[v]) {
                brigde[u] = 1;
            }
        }
        else {
            fup[v] = std::min(fup[v], tin[u]);
        }
    }
}

// find deeps, bridges count
void dfs2(int v, graph_t const& graph, std::vector<uint8_t>& visited, std::vector<int>& deep, std::vector<uint8_t> const& bridge, std::vector<int>& bridge_count) {
    visited[v] = 1;
    for(auto u : graph[v]) {
        if(!visited[u]) {
            deep[u] = deep[v] + 1;
            bridge_count[u] = bridge_count[v];
            if (bridge[u])
                ++bridge_count[u];
            dfs2(u, graph, visited, deep, bridge, bridge_count);
        }
    }
}

void build_lca(std::vector<std::vector<int>>& dp, int const log_n, std::vector<int> const& parent, int const capital) {
    int n = dp.size();
    for (auto i = 0; i < n; ++i) {
        dp[i].resize(log_n);
        dp[i][0] = parent[i];
    }

    for (auto i = 0; i < log_n; ++i) {
        dp[capital][i] = capital;
    }

    for (auto j = 1; j < log_n; ++j) {
        for (auto i = 0; i < n; ++i) {
            dp[i][j] = dp[dp[i][j - 1]][j - 1];
        }
    }
}

int get_lca(std::vector<std::vector<int>> const& dp, std::vector<int> const& deep, int u, int v) {
    if (u == v) {
        return v;
    }

    if (deep[v] > deep[u]) {
        std::swap(v, u);
    }

    int log_n = dp[0].size();

    for (auto i = log_n - 1; i >= 0; --i) {
        if(deep[u] - deep[v] >= 1 << i) {
            u = dp[u][i];
        }
    }

    if (u == v) {
        return v;
    }

    for (auto i = log_n - 1; i >= 0; --i) {
        if (dp[v][i] != dp[u][i]) {
            v = dp[v][i];
            u = dp[u][i];
        }
    }

    return dp[v][0];
}

int main() {
    lca_t lca;
    int n, m;
    int capital;

    std::cin.sync_with_stdio(false);
    std::cout.sync_with_stdio(false);

    std::cin >> n >> m >> capital; --capital;
    graph_t graph(n);

    int u, v;
    for (auto i = 0; i < m; ++i) {
        std::cin >> u >> v; --u; --v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    tin.resize(n);
    fup.resize(n);
    std::vector<uint8_t> visited(n, 0);
    std::vector<uint8_t> brigde(n, 0);
    std::vector<int> parent(n, 0);
    parent[capital] = capital;
    dfs1(capital, graph, visited, parent, brigde);

    visited.assign(n, 0);
    std::vector<int> deep(n, 0);
    std::vector<int> bridge_count(n, 0);
    dfs2(capital, graph, visited, deep, brigde, bridge_count);

    auto log_n = static_cast<int>(ceil(log2(n)));
    std::vector<std::vector<int>> dp(n);
    build_lca(dp, log_n, parent, capital);

    int k;
    std::cin >> k;
    std::vector<int> result(k);
    for (auto i = 0; i < k; ++i) {
        std::cin >> u >> v; --u; --v;
        result[i] = bridge_count[get_lca(dp, deep, u, v)];
    }

    std::for_each(result.begin(), result.end(), [](int val) {std::cout << val << std::endl; });
}
