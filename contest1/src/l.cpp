#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <stdint.h>
#pragma comment(linker, "/STACK:20000000")
using namespace std;
struct path {
    path(path&& other)
        : vertices(move(other.vertices))
        , rmq(move(other.rmq))
    {}

    explicit path(vector<int>&& vert)
        : vertices(move(vert)){
        rmq.resize(4 * vertices.size());
        rmq.assign(rmq.size(), 0);
    }
    vector<int> vertices;
    vector<int> rmq;

    int get_upper_vertex() const {
        return vertices.back();
    }

    void update(int pos, int value) {
        update(1, 0, vertices.size() - 1, pos, value);
    }

    int max_suffix(int l) const {
        return max(l, vertices.size() - 1);
    }

    int max() const {
        return max(0, vertices.size() - 1);
    }

    int max(int l, int r) const {
        return max(1, 0, vertices.size() - 1, l, r);
    }

private:

    void update(int v, int tl, int tr, int pos, int value) {
        if (tl == tr) {
            rmq[v] += value;
        }
        else {
            auto tm = (tl + tr) / 2;
            auto left = v * 2;
            auto right = v * 2 + 1;
            if (pos <= tm) {
                update(left, tl, tm, pos, value);
            }
            else {
                update(right, tm + 1, tr, pos, value);
            }
            rmq[v] = std::max(rmq[left], rmq[right]);
        }
    }

    int max(int v, int tl, int tr, int l, int r) const {
        if(l > r) {
            return 0;
        }

        if(l == tl && r == tr) {
            return rmq[v];
        }

        auto tm = (tl + tr) / 2;
        auto m_left = max(v * 2, tl, tm, l, std::min(r, tm));
        auto m_right = max(v * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r);
        return std::max(m_left, m_right);
    }
};

typedef vector<vector<int>> graph_t;

vector<uint8_t> to_parent_edge_heavy;
// depth first search. Evaluate count of nodes in subtree
void dfs(int v, graph_t const& graph, vector<uint8_t>& visited, vector<int>& sub_tree_size, 
    vector<uint8_t>& has_heavy_edge, vector<int>& parent, vector<int>& deep) {
    visited[v] = 1;
    sub_tree_size[v] = 1;
    for (auto u : graph[v]) {
        if (!visited[u]) {
            parent[u] = v;
            deep[u] = deep[v] + 1;
            dfs(u, graph, visited, sub_tree_size, has_heavy_edge, parent, deep);
            sub_tree_size[v] += sub_tree_size[u];
        }
    }

    auto this_subtree_size = sub_tree_size[v];
    for(auto u : graph[v]) {
        if(u != parent[v]) {
            if (2 * sub_tree_size[u] >= this_subtree_size) {
                has_heavy_edge[v] = 1;
                to_parent_edge_heavy[u] = 1;
            }
        }
    }
}

// build heavy-light decomposition for tree defined parent vector.
void build_heavy_light(vector<vector<int>>& decomposition, vector<uint8_t> const& has_heavy_edge, vector<int> const& parent, vector<pair<int, int>>& location) {
    int n = has_heavy_edge.size();
    auto number = 0;
    for (auto i = 0; i < n; ++i) {
        if (has_heavy_edge[i] == 0) {
            vector<int> path;
            int loc = 0;
            path.push_back(i);
            location[i].first = number;
            location[i].second = loc++;
            auto p = i;
            while(to_parent_edge_heavy[p] == 1){
                p = parent[p];
                location[p].first = number;
                location[p].second = loc++;
                path.push_back(p);
            }

            decomposition.emplace_back(move(path));
            ++number;
        }
    }
}

void build_lca(vector<vector<int>>& dp, int const log_n, vector<int> const& parent, int const root) {
    int n = dp.size();
    for (auto i = 0; i < n; ++i) {
        dp[i].resize(log_n);
        dp[i][0] = parent[i];
    }

    for (auto i = 0; i < log_n; ++i) {
        dp[root][i] = root;
    }

    for (auto j = 1; j < log_n; ++j) {
        for (auto i = 0; i < n; ++i) {
            dp[i][j] = dp[dp[i][j - 1]][j - 1];
        }
    }
}

int get_lca(vector<vector<int>> const& dp, vector<int> const& deep, int u, int v) {
    if (u == v) {
        return v;
    }

    if (deep[v] > deep[u]) {
        std::swap(v, u);
    }

    int log_n = dp[0].size();

    for (auto i = log_n - 1; i >= 0; --i) {
        if (deep[u] - deep[v] >= 1 << i) {
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

int get_max(int p, int v, vector<path> const& decomposition, vector<pair<int, int>> const& path_info, vector<int> const& parent) {
    if (p == v) {
        auto const& path = decomposition[path_info[p].first];
        auto from = path_info[p].second;
        return path.max(from, from);
    }

    auto parent_path_ix = path_info[p].first;
    if (parent_path_ix == path_info[v].first) {
        auto const& path = decomposition[parent_path_ix];
        auto from = path_info[v].second;
        auto to = path_info[p].second;
        return path.max(from, to);
    }

    // Need to move v to p
    auto const& v_path = decomposition[path_info[v].first];
    auto max = v_path.max_suffix(path_info[v].second);
    v = parent[v_path.get_upper_vertex()];

    while(path_info[v].first != parent_path_ix) {
        auto const& current_path = decomposition[path_info[v].first];
        max = std::max(max, current_path.max_suffix(path_info[v].second));
        v = parent[current_path.get_upper_vertex()];
    }

    auto const& parent_path = decomposition[parent_path_ix];
    auto to_parent = parent_path.max(path_info[v].second, path_info[p].second);
    return std::max(to_parent, max);
}

int main() {

    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n;
    std::cin >> n;
    int u, v;
    graph_t graph(n);
    for (auto i = 0; i < n - 1; ++i) {
        std::cin >> u >> v; --u; --v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    // preprocess
    vector<uint8_t> visited(n, 0);
    vector<int> sub_tree_size(n, 0);
    vector<uint8_t> has_heavy_edge(n, 0);
    vector<int> parent(n, 0);
    vector<int> deep(n, 0);
    to_parent_edge_heavy.resize(n);
    to_parent_edge_heavy.assign(n, 0);
    dfs(0, graph, visited, sub_tree_size, has_heavy_edge, parent, deep);

    vector<vector<int>> heavy_pathes;
    vector<pair<int, int>> path_location(n);
    build_heavy_light(heavy_pathes, has_heavy_edge, parent, path_location);
    vector<path> decomposition;
    for (auto& heavy_path : heavy_pathes) {
        decomposition.emplace_back(path(move(heavy_path)));
    }

    auto log_n = static_cast<int>(ceil(log2(n + 1)) + 1);
    vector<vector<int>> dp(n);
    build_lca(dp, log_n, parent, 0);

    int q;
    std::cin >> q;
    std::vector<int> result;
    char query_type;
    for (auto i = 0; i < q; ++i) {
        std::cin >> query_type >> u >> v; --u;
        if (query_type == 'I') {
            auto path_num = path_location[u].first;
            auto num_in_path = path_location[u].second;
            decomposition[path_num].update(num_in_path, v);
        }
        else {
            --v;
            auto lca = get_lca(dp, deep, u, v);
            result.push_back(std::max(get_max(lca, u, decomposition, path_location, parent), get_max(lca, v, decomposition, path_location, parent)));
        }
    }

    std::for_each(result.begin(), result.end(), [](int val) {std::cout << val << std::endl; });
}
