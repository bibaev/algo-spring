#include <iostream>
#include <vector>
#include <queue>
#include <memory>
#include <algorithm>
const int INFTY = 100000000;

struct edge;
using graph_t = std::vector<std::vector<std::shared_ptr<edge>>>;

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
    bool is_back;
    long long f;
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

void dfs(int v, graph_t const& graph, std::vector<uint8_t>& visited, std::vector<std::shared_ptr<edge>>& prev, int to) {
    if(v == to) {
        return;
    }

    visited[v] = 1;
    for(auto e : graph[v]) {
        if(!visited[e->to] && e->f != 0) {
            prev[e->to] = e;
            if (e->to == to) return;
            dfs(e->to, graph, visited, prev, to);
        }
    }
}

std::vector<int> build_path(int to, std::vector<std::shared_ptr<edge>> const& prev) {
    std::vector<int> result;
    if (prev[to] == nullptr) return result;
    auto current = prev[to];
    result.push_back(current->to);
    while (current != nullptr) {
        current->f = current->back->f = 0;
        result.push_back(current->from);
        current = prev[current->from];
    }

    reverse(result.begin(), result.end());
    return result;
}

void print(std::vector<int> const& v) {
    if (v.size() == 0) return;
    for (size_t i = 0; i < v.size() - 1; ++i) {
        std::cout << v[i] + 1 << " ";
    }

    std::cout << v.back() + 1 << std::endl;
}

int main() {
    using namespace std;
    size_t n, m, apricot, home;
    cin >> n >> m >> apricot >> home; --apricot; --home;

    graph_t graph(n);
    for (size_t i = 0; i < m; ++i) {
        int from, to;
        cin >> from >> to; --from; --to;
        if (from == to) continue;
        auto e1 = make_shared<edge>(from, to, 1, nullptr);
        auto e2 = make_shared<edge>(to, from, 0, e1);
        e2->is_back = true;
        e1->back = e2;
        graph[from].push_back(e1);
        graph[to].push_back(e2);
    }

    vector<uint8_t> visited(n, 0);
    dfs(apricot, INFTY, graph, visited, home);
    visited.assign(n, 0);
    auto c = dfs(apricot, INFTY, graph, visited, home);
    if(c == 0) {
        cout << "NO" << endl;
        return 0;
    }

    vector<shared_ptr<edge>> prev(n, nullptr);
    visited.assign(n, 0);
    dfs(apricot, graph, visited, prev, home);
    auto p1 = build_path(home, prev);
    prev.assign(n, nullptr);
    visited.assign(n, 0);
    dfs(apricot, graph, visited, prev, home);
    auto p2 = build_path(home, prev);

    //if(p1.size() == 0 || p2.size() == 0) {
    //    cout << "NO" << endl;
    //    return 0;
    //}

    cout << "YES" << endl;
    print(p1);
    print(p2);
}