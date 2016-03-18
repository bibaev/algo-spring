#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

typedef std::vector<std::vector<int>> graph_t;
typedef std::vector<std::set<int>> brigdes_t;
typedef std::vector<std::vector<int>> lca_t;

void dfs(int vertex, int parent, graph_t const& graph, std::vector<uint8_t>& visited, std::vector<uint32_t>& time_in, std::vector<uint32_t>& fup, brigdes_t& brigdes,
    std::vector<uint32_t>& deep)  {
    static uint32_t timer = 0;
    visited[vertex] = 1;
    if(parent != -1) {
        deep[vertex] = deep[parent] + 1;
    }

    time_in[vertex] = fup[vertex] = timer++;
    for(auto u : graph[vertex]) {
        if (u == parent) continue;
        if(visited[u]) {
            fup[vertex] = std::min(fup[vertex], time_in[u]);
        }
        else {
            dfs(u, vertex, graph, visited, time_in, fup, brigdes, deep);
            fup[vertex] = std::min(fup[vertex], fup[u]);
            if (fup[u] > fup[vertex]) {
                brigdes[u].insert(vertex);
                brigdes[vertex].insert(u);
            }
        }
    }
}

void build_spanning_tree(int vertex, graph_t const& g, std::vector<int>& spanning_tree,
    std::vector<uint8_t>& visited, brigdes_t const& brigdes, std::vector<uint32_t>& b_count, uint32_t brigde_count) {
    visited[vertex] = 1;
    b_count[vertex] = brigde_count;
    for(auto u : g[vertex]) {
        if(!visited[u]) {
            auto new_bridges_count = brigde_count + (brigdes[vertex].count(u) != 0 ? 1 : 0);
            build_spanning_tree(u, g, spanning_tree, visited, brigdes, b_count, new_bridges_count);
            spanning_tree[u] = vertex;
        }
    }
}

void preprocess(int capital, graph_t const& g, std::vector<int>& spanning_tree, std::vector<uint32_t>& bridges_count_to_root, std::vector<uint32_t>& deep) {
    int n = g.size();
    std::vector<uint8_t> visited(n, 0);
    std::vector<uint32_t> time_in(n, 0);
    std::vector<uint32_t> fup(n, 0);
    brigdes_t brigdes(n);
    dfs(capital, -1, g, visited, time_in, fup, brigdes, deep);
    
    visited.assign(n, 0);
    spanning_tree[capital] = capital;
    build_spanning_tree(capital, g, spanning_tree, visited, brigdes, bridges_count_to_root, 0);
}

void build_lca(std::vector<int>& spanning_tree, lca_t& lca) {
    int n = spanning_tree.size();
    auto log_n = static_cast<int>(ceil(log2(n)) + 1);
    for (auto i = 0; i < n; ++i) {
        lca[i].resize(log_n);
        lca[i][0] = spanning_tree[i];
    }

    for (auto j = 1; j < log_n; ++j) {
        for (auto i = 1; i < n; ++i) {
            lca[i][j] = lca[lca[i][j - 1]][j - 1];
        }
    }
}

int get_lca(lca_t const& lca, std::vector<uint32_t>const& deep, int u, int v) {
    if (u == v) {
        return v;
    }

    const auto log_n = static_cast<int>(ceil(log2(lca.size())));
    if (deep[v] > deep[u]) {
        std::swap(u, v);
    }

    for (auto i = log_n; i >= 0; --i) {
        if(deep[u] - deep[v] >= 1 << i) {
            u = lca[u][i];
        }
    }

    if (u == v) {
        return v;
    }

    for (auto i = log_n; i >= 0; --i) {
        if(lca[v][i] != lca[u][i]) {
            v = lca[v][i];
            u = lca[u][i];
        }
    }

    return lca[v][0];
}

int main() {
    int n, m;
    std::cin >> n >> m;
    int capital;
    std::cin >> capital; --capital;
    graph_t g(n);

    int from, to;
    for (auto i = 0; i < m; ++i) {
        std::cin >> from >> to; --from; --to;
        g[from].push_back(to);
        g[to].push_back(from);
    }

    std::vector<int> spanning_tree(n);
    std::vector<uint32_t> bridges_count_to_root(n, 0);
    std::vector<uint32_t> deep(n);
    deep[capital] = 0;
    preprocess(capital, g, spanning_tree, bridges_count_to_root, deep);

    lca_t lca(n);
    build_lca(spanning_tree, lca);

    int k;
    std::cin >> k;

    std::vector<int> result(k);
    int u, v;
    for (auto i = 0; i < k; ++i) {
        std::cin >> u >> v; --u; --v;
        result[i] = bridges_count_to_root[get_lca(lca, deep, u, v)];
    }

    std::for_each(result.begin(), result.end(), [](int val) {std::cout << val << std::endl; });

    return 0;
}
