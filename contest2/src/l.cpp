#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <random>

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
    for (int i = 0; i < k; ++i) {
        int left, right;
        cin >> left >> right; --left; --right;
        graph[left].push_back(right);
    }

    reverse(graph.begin(), graph.end());
    vector<int> min_r(n, INFTY);
    vector<int> matching(m, -1);
    vector<uint8_t> visited(n, 0);
    vector<int> rev_matching(n, -1);
    long matching_size = 0;
    int right = 0;
    for (int left = 0; left < n; left++) {
        while (matching_size < m && right < n) {
            fill(visited.begin(), visited.end(), 0);
            matching_size += build_path(right++, graph, visited, matching, rev_matching) ? 1 : 0;
        }

        if(matching_size == m) {
            min_r[left] = right - 1;
            if(rev_matching[left] != -1) {
                matching[rev_matching[left]] = -1;
                rev_matching[left] = -1;
                matching_size -= 1;
                for (size_t t = right - 1; t >= left + 1; --t) {
                    if(rev_matching[t] == -1) {
                        fill(visited.begin(), visited.end(), 0);
                        auto c = build_path(t, graph, visited, matching, rev_matching) ? 1 : 0;
                        matching_size += c;
                        if (c) break;
                    }
                }
            }
        }
    }

    long result = 0;
    for (int i = 0; i < n; ++i) {
        if(min_r[i] != INFTY)
            result += n - min_r[i];
    }

    cout << result << endl;

    return 0;
}