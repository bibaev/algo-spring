#include <iostream>
#include <memory>
#include <vector>
#include <queue>
const int INFTY = 100000000;
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

void dfs(int v, graph_t const& graph, std::vector<uint8_t>& visited) {
    visited[v] = 1;
    for(auto e : graph[v]) {
        if(!visited[v] && e->f < e->c) {
            dfs(e->to, graph, visited);
        }
    }
}

void clear(std::queue<int> &q) {
    std::queue<int> empty;
    swap(q, empty);
}

long long iteration(graph_t graph, int s, int t) {
    static std::queue<int> queue;
    static std::vector<std::shared_ptr<edge>> prev(graph.size());
    static std::vector<uint8_t> visited(graph.size());
    prev.assign(graph.size(), nullptr);
    visited.assign(graph.size(), 0);
    queue.push(s);
    while(!queue.empty()) {
        auto v = queue.front(); queue.pop();
        if (visited[v]) continue;
        visited[v] = 1;
        for(auto e : graph[v]) {
            if (!visited[e->to] && e->f < e->c) {
                queue.push(e->to);
                if(prev[e->to] == nullptr) {
                    prev[e->to] = e;
                    if(e->to == t) {
                        clear(queue);
                        break;
                    }
                }
            }
        }
    }

    if(prev[t] == nullptr) {
        return 0;
    }

    auto current = prev[t];
    long long c_min = INFTY;
    while (current != nullptr) {
        c_min = std::min(c_min, current->c - current->f);
        current = prev[current->from];
    }
    
    current = prev[t];
    while (current != nullptr) {
        current->f += c_min;
        current->back->f -= c_min;
        current = prev[current->from];
    }

    return c_min;
}

std::vector<uint8_t> edmonds_karp(graph_t& graph, int s, int t, long long& total) {
    auto c = iteration(graph, s, t);
    total = 0;
    while(c != 0) {
        total += c;
        c = iteration(graph, s, t);
    }

    std::vector<uint8_t> visited(graph.size(), 0);
    dfs(s, graph, visited);
    return visited;
}

int main() {
    using namespace std;
    int n, m;
    cin >> n >> m;
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

    long long total = 0;
    auto visited = edmonds_karp(graph, 0, n - 1, total);
    vector<int> result;
    auto i = 0;
    for(auto e : edges) {
        if(visited[e->from] && !visited[e->to] || !visited[e->from] && visited[e->to]) {
            result.push_back(i);
        }
        ++i;
    }

    cout << result.size() << " " << total << endl;
    for (auto j = 0; j < result.size() -1; ++j) {
        cout << result[j] + 1 << " ";
    }

    cout << result.back() + 1 << endl;
    return 0;
}