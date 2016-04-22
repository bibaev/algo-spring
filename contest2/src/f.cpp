#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using graph_t = std::vector<std::vector<int>>;

std::vector<int> get_reversed_path(int from, int to, std::vector<int> const& prev) {
    std::vector<int> result;
    auto current = to;
    while (current != from) {
        result.push_back(current);
        current = prev[current];
    }
    result.push_back(from);

    return result;
}

void remove_path(graph_t& graph, std::vector<int> const& path) {
    auto n = graph.size();
    for (size_t i = 0; i < path.size() - 1; ++i) {
        auto pos = find(graph[path[i]].begin(), graph[path[i]].end(), path[i + 1]);
        graph[path[i]].erase(pos);
    }
}

void bfs(int v, graph_t const& graph, std::vector<int>& prev) {
    std::vector<uint8_t> visited(graph.size(), 0);
    std::queue<int> queue;
    queue.push(v);
    while(!queue.empty()) {
        auto u = queue.front(); queue.pop();
        if (visited[u]) continue;
        visited[u] = 1;
        for (auto n : graph[u]) {
            if (!visited[n]) {
                if (prev[n] == -1) {
                    prev[n] = u;
                }
                queue.push(n);
            }
        }
    }
}

//void dfs(int v, graph_t const& graph, std::vector<uint8_t>& visited, std::vector<int>& prev) {
//    visited[v] = 1;
//    for (auto u : graph[v]) {
//        if (!visited[u]) {
//            prev[u] = v;
//            dfs(u, graph, visited, prev);
//        }
//    }
//}

int main() {
    using namespace std;
    size_t n, m, apricot, home;
    cin >> n >> m >> apricot >> home; --apricot; --home;
    if (apricot == home) {
        cout << "YES" << endl;
        cout << home + 1 << endl;
        cout << home + 1 << endl;
        return 0;
    }

    graph_t graph(n);
    for (size_t i = 0; i < m; ++i) {
        int from, to;
        cin >> from >> to; --from; --to;
        graph[from].push_back(to);
    }

    vector<int> prev(n, -1);
    bfs(apricot, graph, prev);
    if (prev[home] == -1) {
        cout << "NO" << endl;
        return 0;
    }

    auto path1(get_reversed_path(apricot, home, prev));
    reverse(path1.begin(), path1.end());

    remove_path(graph, path1);

    prev.assign(n, -1);
    bfs(apricot, graph, prev);
    if (prev[home] == -1) {
        cout << "NO" << endl;
        return 0;
    }

    auto path2(get_reversed_path(apricot, home, prev));
    reverse(path2.begin(), path2.end());

    cout << "YES" << endl;
    for (size_t i = 0; i < path1.size() - 1; ++i) {
        cout << path1[i] + 1 << " ";
    }

    cout << path1.back() + 1 << endl;

    for (size_t i = 0; i < path2.size() - 1; ++i) {
        cout << path2[i] + 1 << " ";
    }

    cout << path2.back() + 1 << endl;

    return 0;
}