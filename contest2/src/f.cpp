#include <iostream>
#include <vector>
#include <queue>
#include <memory>
const int INFTY = 100000000;
using graph_t = std::vector<std::vector<std::pair<int, int>>>;

struct cmp {
    bool operator()(std::pair<int, int> const& left, std::pair<int, int> const& right) const {
        return left.second < right.second;
    }
};

void dijkstra(int v, graph_t const& graph, std::vector<long long>& dist, std::vector<int>& prev) {
    auto n = graph.size();
    dist.clear(); dist.resize(n); dist.assign(n, INFTY);
    prev.clear(); prev.resize(n); prev.assign(n, -1);

    dist[v] = 0;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, cmp> queue;
    queue.push(std::make_pair(v, 0));
    while(!queue.empty()) {
        auto p = queue.top(); queue.pop();
        if (p.second > dist[p.first]) continue;
        for(auto neig : graph[p.first]) {
            auto new_dist = dist[p.first] + neig.second;
            if(new_dist < dist[neig.first]) {
                dist[neig.first] = new_dist;
                prev[neig.first] = p.first;
                queue.push({ neig.first, new_dist });
            }
        }
    }
}

std::vector<int> build_path(int to, std::vector<int> const& prev) {
    std::vector<int> result;
    auto current = to;
    while (current != -1) {
        result.push_back(current);
        current = prev[current];
    }
    reverse(result.begin(), result.end());
    return result;
}

int main() {
    using namespace std;
    size_t n, m, apricot, home;
    cin >> n >> m >> apricot >> home; --apricot; --home;

    graph_t graph(n);
    for (size_t i = 0; i < m; ++i) {
        int from, to;
        cin >> from >> to; --from; --to;
        graph[from].emplace_back(to, 1);
    }

    vector<long long> dist;
    vector<int> prev;

    dijkstra(apricot, graph, dist, prev);
    if(dist[home] == INFTY) {
        cout << "NO" << endl;
        return 0;
    }

    auto path1(build_path(home, prev));

    for (size_t i = 0; i < path1.size() - 1; ++i) {
        size_t ix = path1[i + 1];
        auto pos = std::find_if(graph[i].begin(), graph[i].end(), [ix](pair<int, int> const& p) { return p.first == ix; });
        pos->second = 2 * n;
    }

    dijkstra(apricot, graph, dist, prev);
    if(dist[home] > n) {
        cout << "NO" << endl;
        return 0;
    }

    auto path2(build_path(home, prev));

    cout << "YES" << endl;
    for(auto v : path1) {
        std::cout << v + 1 << " ";
    }
    cout << endl;

    for (auto v : path2) {
        std::cout << v + 1 << " ";
    }
    cout << endl;
}