#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

const int INFTY = 10000000;
using b_graph_t = std::vector<std::vector<int>>;

int build_path(int v, b_graph_t const& graph, std::vector<uint8_t>& visited, std::vector<int>& matching, std::vector<int>& rev_matching) {
    if (visited[v]) {
        return false;
    }

    visited[v] = 1;
    for (auto u : graph[v]) {
        if (matching[u] == -1 || build_path(matching[u], graph, visited, matching, rev_matching)) {
            matching[u] = v;
            rev_matching[v] = u;
            return true;
        }
    }

    return false;
}

int main() {
    using namespace std;
    int n, m, k;
    cin >> n >> m >> k;
    b_graph_t graph(n);
    vector<vector<int>> d(n + 1);
    for (int i = 0; i < k; ++i) {
        int left, right;
        cin >> left >> right; --left; --right;
        graph[left].push_back(right);
    }

    for (int i = 0; i < n + 1; ++i) {
        d[i].resize(n, 0);
    }

    for (int i = 0; i < n; ++i) {
        d[i][i] = (m == 1 && graph[i].size() > 0) ? 1 : 0;
    }

    vector<int> min_r(n, INFTY);
    vector<int> matching(m, -1);
    vector<uint8_t> visited(n, 0);
    vector<int> rev_matching(n, -1);
    long matching_size = 0;
    int right = 0;
    for (int left = 0; left < n; left++) {
        while (matching_size < m && right < n) {
            visited.assign(n, 0);
            matching_size += build_path(right++, graph, visited, matching, rev_matching) ? 1 : 0;
        }

        if(matching_size == m) {
            min_r[left] = right - 1;
            if(rev_matching[left] != -1) {
                matching[rev_matching[left]] = -1;
                rev_matching[left] = -1;
                matching_size -= 1;
            }
        }
    }

    for (int j = 1; j < n; ++j) {
        for (int i = j - 1; i >= 0; --i) {
            d[i][j] = d[i + 1][j] + d[i][j - 1] + (min_r[i] <= j ? 1 : 0) - d[i + 1][j - 1];
        }
    }

    cout << d[0].back() << endl;
    return 0;
}