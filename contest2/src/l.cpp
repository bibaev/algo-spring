#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

const int INFTY = 1000000;

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

using graph_t = std::vector<std::vector<std::shared_ptr<edge>>>;

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

std::shared_ptr<edge> add_edge(graph_t& graph, int from, int to, long long c) {
    auto e1 = std::make_shared<edge>(from, to, c, nullptr);
    auto e2 = std::make_shared<edge>(to, from, 0, e1);
    e1->back = e2;
    graph[e1->from].push_back(e1);
    graph[e2->from].push_back(e2);
    
    return e1;
}

bool decrease_stream(graph_t const& graph, std::shared_ptr<edge> e, int to) {
    if(e->f == 0) {
        return false;
    }

    e->f = e->back->f = 0;
    if (e->to == to) {
        return true;
    }

    for(auto n : graph[e->to]) {
        if(n-> c != 0 && n->f != 0) {
            return decrease_stream(graph, n, to);
        }
    }

    return false;
}

int main() {
    using namespace std;
    int n, m, k;
    cin >> n >> m >> k;
    graph_t graph(2 + m + n);

    for (size_t i = 0; i < k; ++i) {
        int from, to;
        cin >> from >> to; --from; --to;
        add_edge(graph, from + 2, to + 2 + n, 1);
    }

    vector<shared_ptr<edge>> lefts;
    for (int i = 0; i < n; ++i) {
        auto e = add_edge(graph, 0, i + 2, 0);
        lefts.push_back(e);
    }

    for (int i = 0; i < m; ++i) {
        add_edge(graph, n + 2 + i, 1, 1);
    }

    long long result = 0;
    int size = graph.size();
    vector<uint8_t> visited(size, 0);
    int right = 0;
    int left = 0;
    auto stream = 0;
    auto c = 0;
    while(left < n && right < n) {
        while (right < n && stream < m) {
            lefts[right++]->c = 1;
            visited.assign(size, 0);
            c = dfs(0, INFTY, graph, visited, 1);
            stream += c;
        }

        if(stream == m) {
            result += n + 1 - right;
            stream -= decrease_stream(graph, lefts[left++], 1) ? 1 : 0;
            lefts[left - 1]->c = 0;
        }
    }
    

    cout << result << endl;

    return 0;
}
