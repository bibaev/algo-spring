#include <iostream>
#include <vector>

using b_graph_t = std::vector<std::vector<int>>;

struct order {
    order(int time, const std::pair<int, int> const& from, std::pair<int, int> const& to)
        : time(time),
          from(from),
          to(to) {}

    int time;
    std::pair<int, int> from;
    std::pair<int, int> to;
};

int distance(std::pair<int, int> const& loc1, std::pair<int, int> const& loc2) {
    return abs(loc1.first - loc2.first) + abs(loc1.second - loc2.second);
}

bool can_after(order const& o1, order const& o2) {
    return o2.time > o1.time + distance(o1.from, o1.to) + distance(o1.to, o2.from);
}

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

int main() {
    using namespace std;
    int n;
    cin >> n;
    vector<order> orders;
    for (size_t i = 0; i < n; ++i) {
        int hour, minute;
        char del;
        int x1, y1, x2, y2;
        cin >> hour >> del >> minute >> x1 >> y1 >> x2 >> y2; 
        auto time = 60 * hour + minute;
        orders.push_back(order(time, { x1, y1 }, { x2, y2 }));
    }
    
    b_graph_t graph(n);
    for (size_t i = 0; i < n; ++i) {
        auto const& o1 = orders[i];
        for (auto j = i + 1; j < n; ++j) {
            auto const& o2 = orders[j];
            if (can_after(o1, o2)) {
                graph[i].push_back(j);
            }

            if(can_after(o2, o1)) {
                graph[j].push_back(i);
            }
        }
    }

    size_t matching_size = 0;
    vector<uint8_t> visited(n, 0);
    vector<int> matching(n, -1);
    for (size_t i = 0; i < n; ++i) {
        visited.assign(n, 0);
        matching_size += build_path(i, graph, visited, matching) ? 1 : 0;
    }

    cout << n - matching_size << endl;

    return 0;
}