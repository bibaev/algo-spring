#include <iostream>
#include <vector>

using b_graph_t = std::vector<std::vector<int>>;

bool build_path(int v, b_graph_t const& graph, std::vector<uint8_t>& visited, std::vector<int>& matching) {
    if (visited[v]) {
        return false;
    }

    visited[v] = 1;
    for (auto u : graph[v]) {
        if(matching[u] == -1 || build_path(matching[u], graph, visited, matching)) {
            matching[u] = v;
            return true;
        }
    }

    return false;
}

int main() {
    using namespace std;
    size_t n, m;
    cin >> n >> m;
    b_graph_t graph(n);
    for (size_t i = 0; i < n; ++i) {
        size_t val;
        cin >> val;
        while(val != 0) {
            graph[i].push_back(val - 1);
            cin >> val;
        }
    }

    size_t matching_size = 0;
    vector<uint8_t> visited(n, 0);
    vector<int> matching(m, -1);
    for (size_t i = 0; i < n; ++i) {
        visited.assign(n, 0);
        matching_size += build_path(i, graph, visited, matching) ? 1 : 0;
    }

    cout << matching_size << endl;

    for (size_t i = 0; i < m; ++i) {
        if(matching[i] != -1) {
            cout << matching[i] + 1 << " " << i + 1 << endl;
        }
    }

    return 0;
}
