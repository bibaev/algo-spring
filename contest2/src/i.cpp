#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

struct molecule {
    int val;
    std::vector<std::shared_ptr<molecule>> neigs;
    std::vector<int> vertices;
    int part;
};

using b_graph_t = std::vector<std::vector<int>>;
using picture = std::vector<std::vector<std::shared_ptr<molecule>>>;

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

inline int get_val(char c) {
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

int main() {
    using namespace std;
    int n, m;
    cin >> n >> m;
    size_t total_val = 0;
    picture pic(n);
    int l_part = 0;
    int r_part = 0;
    for (size_t i = 0; i < n; ++i) {
        char c;
        pic[i].resize(m);
        for (size_t j = 0; j < m; ++j) {
            cin >> c;
            auto mol = make_shared<molecule>();
            mol->val = get_val(c);
            total_val += mol->val;
            mol->part = (i + j) % 2 == 1 ? 1 : 2;
            if(mol->val != 0) {
                if(mol->part == 1) {
                    l_part += mol->val;
                }
                else {
                    r_part += mol->val;
                }
            }
            if(i != 0) {
                pic[i - 1][j]->neigs.push_back(mol);
                mol->neigs.push_back(pic[i - 1][j]);
            }
            if(j != 0) {
                pic[i][j - 1]->neigs.push_back(mol);
                mol->neigs.push_back(pic[i][j - 1]);
            }

            pic[i][j] = mol;
        }
    }

    int left, right;
    left = right = 0;
    vector<shared_ptr<molecule>> lefts;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            auto mol = pic[i][j];
            for (size_t k = 0; k < mol->val; ++k) {
                if(mol->part == 1) {
                    mol->vertices.push_back(left++);
                    lefts.push_back(mol);
                }
                else {
                    mol->vertices.push_back(right++);
                }
            }
        }
    }

    b_graph_t graph(left);
    for(auto l : lefts) {
        for(auto v : l->vertices) {
            for(auto neig : l->neigs) {
                for(auto u : neig->vertices) {
                    graph[v].push_back(u);
                }
            }
        }
    }

    size_t matching_size = 0;
    vector<uint8_t> visited(left, 0);
    vector<int> matching(right, -1);
    for (size_t i = 0; i < left; ++i) {
        visited.assign(left, 0);
        matching_size += build_path(i, graph, visited, matching) ? 1 : 0;
    }

    if (matching_size == l_part && l_part == r_part && total_val != 0) 
        cout << "Valid" << endl; 
    else
        cout << "Invalid" << endl;
}