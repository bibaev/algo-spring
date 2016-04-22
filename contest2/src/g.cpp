#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <map>

const int INFTY = 1000000;
struct edge;
using graph_t = std::vector<std::vector<std::shared_ptr<edge>>>;


using stream_info_t = std::multimap<std::pair<int, int>, std::shared_ptr<edge>>;
struct edge {
    edge(int from, int to, int c, std::shared_ptr<edge> back)
        : from(from),
        to(to),
        c(c),
        f(0),
        is_back(false),
        back(back) {
    }

    int from;
    int to;
    long long c;
    long long f;
    bool is_back;
    std::shared_ptr<edge> back;
};

long long dfs(int v, long long c_min, graph_t const& graph, std::vector<uint8_t>& visited, int to) {
    if (v == to) {
        return c_min;
    }

    visited[v] = 1;
    for (auto e : graph[v]) {
        if (!visited[e->to] && e->f < e->c) {
            auto d = dfs(e->to, std::min(c_min, e->c - e->f), graph, visited, to);
            if (d > 0) {
                e->f += d;
                e->back->f -= d;
                return d;
            }
        }
    }

    return 0;
}

int main() {
    using namespace std;
    size_t n, m;
    cin >> n >> m;
    stream_info_t stream;
    graph_t graph(n);
    vector<shared_ptr<edge>> edges;
    for (size_t i = 0; i < m; ++i) {
        int from, to, c;
        cin >> from >> to >> c; --from; --to;
        auto e1 = make_shared<edge>(from, to, c, nullptr);
        auto e2 = make_shared<edge>(to, from, c, e1);
        e1->back = e2;
        graph[from].push_back(e1);
        graph[to].push_back(e2);
        edges.emplace_back(e1);
    }

    vector<uint8_t> visited(n, 0);
    auto c = dfs(0, INFTY, graph, visited, n - 1);
    auto total = c;
    while (c != 0) {
        visited.assign(n, 0);
        c = dfs(0, INFTY, graph, visited, n - 1);
        total += c;
    }

    cout << total << endl;
    for (auto const& e : edges) {
        cout << e->f << endl;
    }

    return 0;
}
