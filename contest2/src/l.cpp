#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <random>
#include <memory>
#include <list>

const int INFTY = 10000000;
using b_graph_t = std::vector<std::vector<int>>;
int build_path(int v, b_graph_t const& graph, uint8_t* visited, std::vector<int>& matching, std::vector<int>& rev_matching) {
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

int build_path(int v, std::vector<std::pair<int, std::vector<int>>>& graph, uint8_t* visited, std::vector<int>& matching, std::vector<int>& rev_matching, int left) {
    if (visited[v]) {
        return false;
    }

    visited[v] = 1;
    auto n = graph[v].second.size();
    int i = graph[v].first;
    while (i < n && graph[v].second[i] < left) { ++i; }
    graph[v].first = i;

    for (size_t j = graph[v].first; j < n; ++j) {
        auto u = graph[v].second[j];
        if (matching[u] == -1 || build_path(matching[u], graph, visited, matching, rev_matching, left)) {
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
    std::vector<pair<int, vector<int>>> right_graph(m);
    for (int i = 0; i < k; ++i) {
        int left, right;
        scanf("%d%d", &left, &right);
        --left; --right;
        graph[left].push_back(right);
    }
    for (int i = 0; i < right_graph.size(); ++i) {
        right_graph[i].second.reserve(2000);
    }

    vector<int> matching(m, -1);
    vector<uint8_t> visited(n, 0);
    vector<uint8_t> visited_right(m, 0);
    vector<int> rev_matching(n, -1);
    long matching_size = 0;
    int right = 0;
    long result = 0;
    int unmached = 0;
    for (int left = 0; left < n; left++) {
        while (matching_size < m && right < n) {
            for (auto u : graph[right]) {
                right_graph[u].second.push_back(right);
            }
            right++;

            fill(visited_right.begin(), visited_right.end(), 0);
            auto res = build_path(unmached, right_graph, visited_right.data(), rev_matching, matching, left) ? 1 : 0;
            while(res != 0 && matching_size != m) {
                matching_size += res;
                ++unmached;
                fill(visited_right.begin(), visited_right.end(), 0);
                res = build_path(unmached, right_graph, visited_right.data(), rev_matching, matching, left) ? 1 : 0;
            }
        }

        if (matching_size == m) {
            result += n + 1 - right;
            if (rev_matching[left] != -1) {
                unmached = rev_matching[left];
                matching[unmached] = -1;
                rev_matching[left] = -1;
                matching_size -= 1;
                fill(visited_right.begin(), visited_right.end(), 0);
                matching_size += build_path(unmached, right_graph, visited_right.data(), rev_matching, matching, left + 1);
            }
        }
    }

    std::cout << result << endl;

    return 0;
}