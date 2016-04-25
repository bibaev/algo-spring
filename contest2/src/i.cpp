#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>
const int INFTY = 1000000;

struct mol {
    mol(size_t val, int number)
        : val(val),
          number(number),
          neigs() {}

    size_t val;
    int number;
    std::vector<std::shared_ptr<mol>> neigs;
};

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
using graph_t = std::vector<std::vector<std::shared_ptr<edge>>>;
using pic_t = std::vector<std::vector<std::shared_ptr<mol>>>;

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

size_t get_val(char c) {
    switch(c) {
    case 'H':
        return 1;
    case 'O':
        return 2;
    case 'N':
        return 3;
    case 'C':
        return 4;
    default:
        return 0;
    }
}

void add_edge(graph_t& graph, int from, int to, long long c) {
    auto e1 = std::make_shared<edge>(from, to, c, nullptr);
    auto e2 = std::make_shared<edge>(to, from, c, e1);
    e1->back = e2;
    graph[e1->from].push_back(e1);
    graph[e2->from].push_back(e2);
}

bool check(pic_t const& pic) {
    if(pic[0][0]->val > 2 || pic.back()[0]->val > 2 || pic[0].back()->val > 2 || pic.back().back()->val > 2) {
        return false;
    }

    for (int i = 1; i < pic.size() - 1; ++i) {
        if (pic[i][0]->val == 4 || pic[i].back()->val == 4) {
            return false;
        }
    }

    for (int j = 1; j < pic[0].size() - 1; ++j) {
        if(pic[0][j]->val == 4 || pic.back()[j]->val == 4) {
            return false;
        }
    }

    return true;
}

size_t count_of_non_dot_neig(std::shared_ptr<mol> ptr) {
    size_t result = 0;
    for(auto n : ptr->neigs) {
        if(n->val > 0) {
            ++result;
        }
    }

    return result;
}

int main() {
    using namespace std;
    size_t n, m;
    char c;
    cin >> n >> m;
    pic_t pic(n);
    vector<shared_ptr<mol>> left, right;
    size_t total_left = 0;
    size_t total_right = 0;
    for (size_t i = 0; i < n; ++i) {
        pic[i].resize(m);
        for (size_t j = 0; j < m; ++j) {
            cin >> c;
            auto molecule = make_shared<mol>(get_val(c), -1);
            if(molecule->val != 0) {
                if((i + j) % 2 == 0) {
                    left.push_back(molecule);
                    total_left += molecule->val;
                }
                else {
                    right.push_back(molecule);
                    total_right += molecule->val;
                }
            }

            if(i != 0) {
                pic[i - 1][j]->neigs.push_back(molecule);
                molecule->neigs.push_back(pic[i - 1][j]);
            }
            if(j != 0) {
                pic[i][j - 1]->neigs.push_back(molecule);
                molecule->neigs.push_back(pic[i][j - 1]);
            }

            pic[i][j] = molecule;
        }
    }

    if(total_left == 0 || total_left != total_right || !check(pic)) {
        cout << "Invalid" << endl;
        return 0;
    }

    if(right.size() > left.size()) {
        swap(left, right);
    }

    size_t v_num = 2;
    graph_t graph(2);
    for (auto molecule : left) {
        if (molecule->val == 0) continue;
        graph.emplace_back();
        molecule->number = v_num++;
        add_edge(graph, 0, molecule->number, molecule->val);
    }

    for(auto molecule : right) {
        if (molecule->val == 0) continue;
        graph.emplace_back();
        auto u = v_num++;
        add_edge(graph, u, 1, molecule->val);
        for(auto l : molecule->neigs) {
            if (l->val == 0) continue;
            add_edge(graph, l->number, u, 1);
        }
    }

    auto size = graph.size();
    vector<uint8_t> visited(size, 0);
    auto s = dfs(0, INFTY, graph, visited, 1);
    auto total = s;
    while(s != 0) {
        visited.assign(size, 0);
        s = dfs(0, INFTY, graph, visited, 1);
        total += s;
    }

    if(total == total_left) {
        cout << "Valid";
    }
    else {
        cout << "Invalid";
    }

    cout << endl;
}