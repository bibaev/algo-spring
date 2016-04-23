#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <queue>

using b_graph_t = std::vector<std::vector<int>>;
const int INFTY = 1000000;

bool build_path(int v, b_graph_t const& graph, std::vector<uint8_t>& visited, std::vector<int>& matching) {
    if (visited[v]) {
        return false;
    }

    visited[v] = 1;
    for (auto u : graph[v]) {
        if (matching[u] == -1 || build_path(matching[u], graph, visited, matching)) {
            matching[u] = v;
            return true;
        }
    }

    return false;
}

void bfs(int vertex, std::vector<uint8_t>& visited, b_graph_t const& graph) {
    int v;
    std::queue<int> queue;
    queue.push(vertex);
    while (!queue.empty()) {
        v = queue.front();
        queue.pop();
        visited[v] = 1;
        for (auto v_ : graph[v]) {
            if (visited[v_] == 0) {
                queue.push(v_);
            }
        }
    }
}

void print(std::vector<int> const& v) {
    if (v.size() == 0) return;
    for (auto i = 0; i < v.size() - 1; ++i) {
        std::cout << v[i] + 1 << " ";
    }

    std::cout << v.back() + 1 << std::endl;
}

void solve(int n, int m) {
    using namespace std;
    b_graph_t graph(n);
    vector<uint8_t> right_part_covered(m);
    std::vector<std::pair<int, int>> edges;
    for (size_t j = 0; j < n; ++j) {
        right_part_covered.assign(m, 0);
        size_t val;
        cin >> val;
        while (val != 0) {
            right_part_covered[val - 1] = 1;
            cin >> val;
        }

        for (size_t i = 0; i < m; ++i) {
            if(!right_part_covered[i]) {
                graph[j].push_back(i);
                edges.push_back({ j, i + n });
            }
        }
    }

    size_t matching_size = 0;
    vector<uint8_t> visited(n, 0);
    vector<int> matching(m, -1);
    for (size_t j = 0; j < n; ++j) {
        visited.assign(n, 0);
        matching_size += build_path(j, graph, visited, matching) ? 1 : 0;
    }

    vector<uint8_t> left_free(n, 1);
    b_graph_t directed_graph(n + m);
    for(auto const & e : edges) {
        auto left = e.first;
        auto right = e.second;
        if(matching[right - n] == left) {
            directed_graph[right].push_back(left);
            left_free[left] = 0;
        }
        else {
            directed_graph[left].push_back(right);
        }
    }

    vector<uint8_t> visited_directed(n + m);
    visited_directed.assign(n + m, 0);
    for (size_t i = 0; i < n; ++i) {
        if (left_free[i] == 0 && !visited_directed[i]) continue;

        bfs(i, visited_directed, directed_graph);
    }

    vector<int> lefts, rights;
    for (size_t i = 0; i < n; ++i) {
        if(visited_directed[i] == 1) {
            lefts.push_back(i);
        }
    }

    for (size_t i = n; i < n + m; ++i) {
        if (visited_directed[i] == 0) {
            rights.push_back(i - n);
        }
    }

    cout << lefts.size() + rights.size() << endl;
    cout << lefts.size() << " " << rights.size() << endl;

    print(lefts);
    print(rights);
}

int main() {
    using namespace std;
    int test_count;
    cin >> test_count;
    for (auto i = 0; i < test_count; ++i) {
        int n, m;
        cin >> n >> m;
        
        solve(n, m);
    }
}

