
#include <iostream>

int main() {
    int n;
    std::cin >> n;
    int u, v;
    for (auto i = 0; i < n - 1; ++i) {
        std::cin >> u >> v; --u; --v;
    }

    // preprocess

    int q;
    std::cin >> q;
    char query_type;
    for (auto i = 0; i < q; ++i) {
        std::cin >> query_type >> u >> v;
        if (query_type == 'I') {
            
        }
        else {
            
        }
    }
}
