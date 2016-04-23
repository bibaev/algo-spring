#include <iostream>
#include <vector>

enum loc_type {
    CAN_NOT_BUILD,
    CAN_BUILD,
    MOUNT
};

int main() {
    using namespace std;
    int n, m;
    cin >> m >> n;
    vector<vector<loc_type>> loc(m);
    for (size_t i = 0; i < m; ++i) {
        loc[i].resize(m, CAN_NOT_BUILD);
    }

    int k, l;
    cin >> k >> l;
    size_t x, y;
    for (size_t i = 0; i < k; ++i) {
        cin >> x >> y; --x; --y;
        loc[y][x] = CAN_BUILD;
    }

    for (size_t j = 0; j < l; ++j) {
        cin >> x >> y; --x; --y;
        loc[y][x] = MOUNT;
    }

    return 0;
}
