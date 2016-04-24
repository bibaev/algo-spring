#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

const int INFTY = 1000000;
using b_graph_t = std::vector<std::vector<int>>;

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
        if(!visited[e->to] && e->f < e->c) {
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
    clear(queue);
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
int main() {
    using namespace std;
    int n, m, k;
    cin >> n >> m >> k;
    b_graph_t graph(n);
    b_graph_t edges(n);
    vector<int> matching(m, -1);
    for (size_t i = 0; i < k; ++i) {
        int from, to;
        cin >> from >> to; --from; --to;
        edges[from].push_back(to);
    }

    int left = 0, right = 0;
    long result = 0;
    std::vector<uint8_t> visited(n, 0);
    std::vector<int> l_mathing(n, -1);
    size_t matching_size = 0;
    while(left < n && right < n) {
        while(right < n && matching_size < m) {
            graph[right] = move(edges[right]);
            visited.assign(n, 0);
            l_mathing[right] = build_path(right, graph, visited, matching);
            matching_size += l_mathing[right] != -1 ? 1 : 0;
            ++right;
        }

        while (matching_size == m) {
            if(l_mathing[left] != -1) {
                matching_size -= 1;
            }

            matching[l_mathing[left]] = -1;
            graph[left].clear();
            ++left;
            result += n + 1 - right;
        }
        
        
    }

    cout << result << endl;

    return 0;
}
