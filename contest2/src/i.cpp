#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

struct molecule {
    int val;
    std::vector<std::shared_ptr<molecule>> neigs;
    int part;
    int num = -1;
};

using b_graph_t = std::vector<std::vector<int>>;
using picture = std::vector<std::vector<std::shared_ptr<molecule>>>;

const int INFTY = 100000000;
struct edge;
using graph_t = std::vector<std::vector<std::shared_ptr<edge>>>;

struct edge {
    edge(int from, int to, int c, std::shared_ptr<edge> back)
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

int get_val(char c) {
    switch(c) {
    case 'H':
        return 1;
    case 'O':
        return 2;
    case 'N':
        return 3;
    case 'C' :
        return 4;
    default:
        return 0;
    }
}

void add_egde(graph_t& graph, int from, int to, int c) {
    auto e1 = std::make_shared<edge>(from, to, c, nullptr);
    auto e2 = std::make_shared<edge>(to, from, c, e1);
    e1->back = e2;
    graph[from].push_back(e1);
    graph[to].push_back(e2);
}

int main() {
    using namespace std;
    size_t n, m;
    cin >> n >> m;
    picture pic(n);
    int l_part, r_part;
    l_part = r_part = 0;
    vector<shared_ptr<molecule>> left;
    vector<shared_ptr<molecule>> right;
    for (size_t i = 0; i < n; ++i) {
        char c;
        pic[i].resize(m);
        for (size_t j = 0; j < m; ++j) {
            cin >> c;
            auto mol = make_shared<molecule>();
            mol->val = get_val(c);
            mol->part = (i + j) % 2 == 1 ? 1 : 2;
            if(mol->val != 0) {
                if(mol->part == 1) {
                    l_part += mol->val;
                    left.push_back(mol);
                }
                else {
                    r_part += mol->val;
                    right.push_back(mol);
                }

                if (i != 0 && pic[i - 1][j]->val != 0) {
                    pic[i - 1][j]->neigs.push_back(mol);
                    mol->neigs.push_back(pic[i - 1][j]);
                }
                if (j != 0 && pic[i][j - 1]->val != 0) {
                    pic[i][j - 1]->neigs.push_back(mol);
                    mol->neigs.push_back(pic[i][j - 1]);
                }
            }

            pic[i][j] = mol;
        }
    }

    if(l_part != r_part || r_part == 0) {
        cout << "Invalid" << endl;
        return 0;
    }

    graph_t graph(2);
    auto v_num = 2;
    for(auto molecule : right) {
        if (molecule->val == 0) continue;
        graph.emplace_back();
        molecule->num = v_num++;
        add_egde(graph, 1, molecule->num, molecule->val);
    }

    for(auto molecule : left) {
        if (molecule->val == 0) continue;
        for(auto r : molecule->neigs) {
            if (r->val == 0) continue;
            graph.emplace_back();
            auto v = v_num++;
            add_egde(graph, r->num, v, 1);
            for (int k = 0; k < molecule->val; ++k) {
                graph.emplace_back();
                auto u = v_num++;
                add_egde(graph, v, u, 1);
                add_egde(graph, u, 0, 1);
            }
        }
    }

    int size = graph.size();
    vector<uint8_t> visited(size, 0);
    auto c = dfs(1, INFTY, graph, visited, 0);
    auto total = c;
    while (c != 0) {
        visited.assign(size, 0);
        c = dfs(1, INFTY, graph, visited, 0);
        total += c;
    }

    if(total == l_part) {
        cout << "Valid" << endl;
    }
    else {
        cout << "Invalid" << endl;
    }
}