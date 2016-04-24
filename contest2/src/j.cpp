#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <memory>
#include <set>
#include <cassert>

const int INFTY = 100000;
struct loc_t;
struct edge {
    edge(int from, int to, int c, edge* back)
        : from(from),
        to(to),
        c(c),
        f(0),
        back(back) {
    }

    int from;
    int to;
    long long c;
    long long f;
    edge* back;
    loc_t* loc;
};

using graph_t = std::vector<std::vector<edge*>>;

void dfs(int v, graph_t const& graph, std::vector<uint8_t>& visited) {
    visited[v] = 1;
    for (auto e : graph[v]) {
        if (!visited[e->to] && e->f < e->c) {
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
    static std::vector<edge*> prev(graph.size());
    static std::vector<uint8_t> visited(graph.size());
    prev.assign(graph.size(), nullptr);
    clear(queue);
    visited.assign(graph.size(), 0);
    queue.push(s);
    while (!queue.empty()) {
        auto v = queue.front(); queue.pop();
        if (visited[v]) continue;
        visited[v] = 1;
        for (auto e : graph[v]) {
            if (!visited[e->to] && e->f < e->c) {
                queue.push(e->to);
                if (prev[e->to] == nullptr) {
                    prev[e->to] = e;
                    if (e->to == t) {
                        clear(queue);
                        break;
                    }
                }
            }
        }
    }

    if (prev[t] == nullptr) {
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
    while (c != 0) {
        total += c;
        c = iteration(graph, s, t);
    }

    std::vector<uint8_t> visited(graph.size(), 0);
    dfs(s, graph, visited);
    return visited;
}

enum loc_type {
    CAN_NOT_BUILD,
    CAN_BUILD,
    MOUNT
};

struct loc_t {
    loc_t(loc_type type, int x, int y)
        : type(type),
        num(-1),
        x(x),
        y(y) {
    }

    loc_type type;
    int num;
    int x;
    int y;
    int up_num;
    int right_num;
    int left_num;
    int down_num;

};

std::vector<edge*> edges;
void add_edges(graph_t& graph, int from, int to, int c, loc_t* loc) {
    auto e1 = new edge(from, to, c, nullptr);
    auto e2 = new edge(to, from, c, e1);
    e1->loc = loc;
    e1->back = e2;
    graph[from].push_back(e1);
    graph[to].push_back(e2);
    edges.push_back(e1);
}

int main() {
    using namespace std;
    int n, m;
    cin >> n >> m;

    if (n == m && m == 1) {
        cout << -1 << endl;
        return 0;
    }

    vector<vector<loc_t*>> loc(m);
    for (size_t i = 0; i < m; ++i) {
        loc[i].resize(n, nullptr);
        for (size_t j = 0; j < n; ++j) {
            loc[i][j] = new loc_t(CAN_NOT_BUILD, j + 1, i + 1);
        }
    }

    int k, l;
    cin >> k >> l;
    size_t x, y;
    for (size_t j = 0; j < k; ++j) {
        cin >> x >> y; --x; --y;
        loc[y][x] = new loc_t(MOUNT, x + 1, y + 1);
    }

    std::vector<loc_t*> can_build;
    for (size_t i = 0; i < l; ++i) {
        cin >> x >> y; --x; --y;
        loc[y][x] = new loc_t(CAN_BUILD, x + 1, y + 1);
        can_build.push_back(loc[y][x]);
    }

    int ax, ay, bx, by;
    cin >> ax >> ay >> bx >> by; --ax; --bx; --ay; --by;

    graph_t graph;
    int v_num = 0;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            auto current = loc[i][j];
            graph.emplace_back();
            current->num = v_num++;
            if (current->type != CAN_NOT_BUILD) continue;
            if (i != 0 && loc[i - 1][j]->type == CAN_NOT_BUILD) {
                add_edges(graph, current->num, loc[i - 1][j]->num, INFTY, nullptr);
            }
            if(j != 0 && loc[i][j - 1]->type == CAN_NOT_BUILD) {
                add_edges(graph, current->num, loc[i][j - 1]->num, INFTY, nullptr);
            }
        }
    }

    for(auto v : can_build) {
        graph.emplace_back();
        graph.emplace_back();
        graph.emplace_back();
        graph.emplace_back();
        v->up_num = v_num++;
        v->right_num = v_num++;
        v->down_num = v_num++;
        v->left_num = v_num++;
        add_edges(graph, v->up_num, v->right_num, 1, v);
        add_edges(graph, v->right_num, v->down_num, 1, v);
        add_edges(graph, v->down_num, v->left_num, 1, v);
        add_edges(graph, v->left_num, v->up_num, 1, v);
    }

    for(auto v : can_build) {
        int i = v->y - 1;
        int j = v->x - 1;
        if(i != 0 && loc[i - 1][j]->type != MOUNT) {
            auto up = loc[i - 1][j];
            if(up->type == CAN_BUILD) {
                add_edges(graph, v->up_num, up->down_num, INFTY, nullptr);
            }
            else {
                add_edges(graph, v->up_num, up->num, INFTY, nullptr);
            }
        }

        if(j != 0 && loc[i][j - 1]->type != MOUNT) {
            auto left = loc[i][j - 1];
            if(left->type == CAN_BUILD) {
                add_edges(graph, v->left_num, left->right_num, INFTY, nullptr);
            }
            else {
                add_edges(graph, v->left_num, left->num, INFTY, nullptr);
            }
        }
        if(i != m - 1 && loc[i + 1][j]->type != MOUNT) {
            auto down = loc[i + 1][j];
            if(down->type == CAN_BUILD) {
                add_edges(graph, v->down_num, down->up_num, INFTY, nullptr);
            }
            else {
                add_edges(graph, v->down_num, down->num, INFTY, nullptr);
            }
        }
        if(j != n - 1 && loc[i][j + 1]->type != MOUNT) {
            auto right = loc[i][j + 1];
            if(right->type == CAN_BUILD) {
                add_edges(graph, v->right_num, right->left_num, INFTY, nullptr);
            }
            else {
                add_edges(graph, v->right_num, right->num, INFTY, nullptr);
            }
        }
    }

    auto s = loc[ay][ax]->num;
    auto t = loc[by][bx]->num;

    long long total = 0;
    auto visited = edmonds_karp(graph, s, t, total);
    if (total >= INFTY) {
        cout << -1 << endl;
        return 0;
    }

    set<loc_t*> wall;
    for (auto e : edges) {
        if (e-> f < 0 && (visited[e->from] && !visited[e->to] || !visited[e->from] && visited[e->to])) {
            wall.insert(e->loc);
        }
    }

    cout << wall.size() << endl;
    for (auto w : wall) {
        cout << w->x << " " << w->y << endl;
    }

    return 0;
}